#!/usr/bin/env python3
"""Reproduce the complete nonexact GUI message interpreter and all four tables."""

import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / 'scripts'))

from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_GRP_JUMP, CS_OP_IMM, CS_OP_MEM
from linked_image import PEImage, linked_code_fields, linked_functions, map_publics, verify_capstone


def inventory(directory):
    image = PEImage(directory / 'source.exe')
    functions = linked_functions(directory / 'source.exe', directory / 'source.map',
                                 directory / 'source.pdb')['functions']
    owners = [f for f in functions if f['symbol'] == '?Run@GuiMessageVmView@@SGHPAU1@@Z']
    assert len(owners) == 1
    owner = owners[0]
    assert owner['extent_source'] == 'pdb-section-contribution'
    assert owner['map_object'] == 'support-14-Gui.ltcg.obj'
    address, size = int(owner['address'], 0), owner['size']
    raw = image.read_address(address, size)
    prefix_size = size - 172
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw[:prefix_size], address))
    assert sum(i.size for i in instructions) == prefix_size
    starts = {i.address - address for i in instructions}
    switches = [i for i in instructions if i.mnemonic == 'jmp'
                and i.operands[0].type == CS_OP_MEM and i.operands[0].mem.index != 0]
    assert len(switches) == 4
    tables = sorted([{'instruction_offset': i.address - address,
                      'offset': i.operands[0].mem.disp - address} for i in switches],
                    key=lambda t: t['offset'])
    assert tables[0]['offset'] == prefix_size
    for index, table in enumerate(tables):
        end = tables[index + 1]['offset'] if index + 1 < len(tables) else size
        table['entry_offsets'] = [int.from_bytes(raw[p:p + 4], 'little') - address
                                  for p in range(table['offset'], end, 4)]
        assert len(table['entry_offsets']) == [24, 7, 7, 5][index]
        assert all(offset in starts for offset in table['entry_offsets'])
    fields = linked_code_fields(image, map_publics(directory / 'source.map'), address, size)
    assert fields['normalization_complete']
    normalized = bytearray(raw)
    identities = []
    for field in fields['fields']:
        offset, width = field['offset'], field['width']
        normalized[offset:offset + width] = b'\0' * width
        destination = field['candidate_target']
        identity = {'offset': offset, 'width': width, 'type': field['type']}
        if address <= destination < address + size:
            identity['destination'] = {'local_offset': destination - address}
        else:
            symbols = sorted(s for s in field['candidate_symbols'] if not s.startswith('_th10_'))
            assert symbols
            identity['destination'] = {'symbols': symbols}
        identities.append(identity)
    branches = []
    for instruction in instructions:
        if not instruction.group(CS_GRP_JUMP):
            continue
        if instruction.operands[0].type != CS_OP_IMM:
            assert instruction in switches
            continue
        destination = instruction.operands[0].imm - address
        assert destination in starts
        branches.append({'offset': instruction.address - address,
                         'destination': destination, 'mnemonic': instruction.mnemonic})
    call_counts = Counter()
    for field in fields['fields']:
        if field['type'] == 'REL32' and field.get('instruction') == 'call':
            names = [s for s in field['candidate_symbols'] if not s.startswith('_th10_')]
            assert len(names) == 1
            call_counts[names[0]] += 1
    return {'complete_owned_bytes': size, 'code_and_alignment_bytes': prefix_size,
            'decoded_instructions': len(instructions),
            'normalized_sha256': hashlib.sha256(normalized).hexdigest(),
            'field_identities': identities, 'fixed_local_branches': branches,
            'tables': tables, 'direct_call_counts': dict(sorted(call_counts.items())),
            'physical_opcode_order': sorted(range(24), key=lambda n: tables[0]['entry_offsets'][n]),
            'returns': [{'offset': i.address - address, 'operand': i.op_str}
                        for i in instructions if i.mnemonic == 'ret']}



def check_target(proof):
    image = PEImage(ROOT / 'resources/th10.exe')
    address = 0x415E90
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(image.read_address(address, 4306), address))
    assert sum(i.size for i in instructions) == 4306
    assert len(instructions) == proof['target_review']['instruction_count']
    starts = {i.address - address for i in instructions}
    local_count = 0
    field_count = 43
    calls = []
    def mapped(value):
        return any(image.image_base + section.rva <= value <
                   image.image_base + section.rva + section.virtual_size
                   for section in image.sections)
    for instruction in instructions:
        if instruction.group(CS_GRP_JUMP):
            if instruction.operands[0].type == CS_OP_IMM:
                assert instruction.operands[0].imm - address in starts
                local_count += 1
            continue
        if instruction.mnemonic == 'call' and instruction.operands[0].type == CS_OP_IMM:
            calls.append({'offset': instruction.address - address,
                          'target': f'0x{instruction.operands[0].imm:08X}'})
            field_count += 1
        elif instruction.disp_size == 4 and any(
                o.type == CS_OP_MEM and o.mem.base == 0 and mapped(o.mem.disp)
                for o in instruction.operands):
            field_count += 1
        elif instruction.imm_size == 4 and any(
                o.type == CS_OP_IMM and mapped(o.imm) for o in instruction.operands):
            field_count += 1
    # The four indexed table bases are also absolute address operands.
    field_count += 4
    assert field_count == proof['target_review']['field_count'] == 337
    assert local_count == 121 and calls == proof['target_review']['direct_calls']
    assert [{'offset': i.address - address, 'operand': i.op_str}
            for i in instructions if i.mnemonic == 'ret'] == proof['target_review']['returns']
    tables = proof['target_review']['tables']
    for index, table in enumerate(tables):
        entries = [int.from_bytes(image.read_address(address + table['offset'] + n * 4, 4),
                                  'little') - address for n in range([24, 7, 7, 5][index])]
        assert entries == table['entry_offsets'] and all(e in starts for e in entries)
    assert sorted(range(24), key=lambda n: tables[0]['entry_offsets'][n]) == \
        proof['native_reproduction']['physical_opcode_order']


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--artifact-dir', type=Path, required=True)
    args = parser.parse_args()
    directory = args.artifact_dir.resolve()
    directory.relative_to(ROOT / 'build')
    proof = json.loads(Path(__file__).with_name('results.json').read_text())
    assert hashlib.sha256((ROOT / 'resources/th10.exe').read_bytes()).hexdigest() == proof['target_sha256']
    verify_capstone(tomllib.loads((ROOT / 'config/tools.lock.toml').read_text())['capstone'])
    check_target(proof)
    actual = inventory(directory)
    assert actual == proof['native_reproduction'], 'native receipt differs'
    print(json.dumps({'result': 'native-candidate-reproduced-nonexact',
                      'candidate_size': actual['complete_owned_bytes'], 'target_size': 4480,
                      'field_count': len(actual['field_identities']),
                      'fixed_local_branches': len(actual['fixed_local_branches']),
                      'table_entries': 43, 'direct_calls': sum(actual['direct_call_counts'].values()),
                      'exact_credit': 0}))


if __name__ == '__main__':
    main()
