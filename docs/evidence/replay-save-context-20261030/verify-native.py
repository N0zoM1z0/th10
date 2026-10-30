#!/usr/bin/env python3
"""Reproduce the complete nonexact replay-save contribution and selected guards."""

import argparse
import copy
import hashlib
import json
from pathlib import Path
import runpy
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / 'scripts'))
from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_GRP_JUMP, CS_OP_IMM, CS_OP_MEM
from linked_image import PEImage, linked_code_fields, linked_functions, map_publics, map_statics, verify_capstone

SYMBOL = '?UpdateReplaySave@FrontEndControllerView@@SGHPAU1@@Z'
GUARDS = ('replay-manager-destroy', 'replay-manager-load',
          'enemy-sound-queue-sample', 'game-complete-stage-transition')


def inventory(image, address, size, symbols=()):
    raw = image.read_address(address, size)
    code_size = size - 20
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw[:code_size], address))
    assert sum(i.size for i in instructions) == code_size
    starts = {i.address - address for i in instructions}
    switches = [i for i in instructions if i.mnemonic == 'jmp'
                and i.operands[0].type == CS_OP_MEM]
    assert len(switches) == 1
    assert switches[0].operands[0].mem.disp == address + code_size
    table = [int.from_bytes(raw[p:p + 4], 'little') - address
             for p in range(code_size, size, 4)]
    assert all(destination in starts for destination in table)
    if symbols:
        fields = linked_code_fields(image, list(symbols), address, size)
        assert fields['normalization_complete']
        fields = fields['fields']
    else:
        # Retail has stripped relocations; enumerate every encoded image operand.
        def in_image(value):
            return any(image.image_base + s.rva <= value <
                       image.image_base + s.rva + s.virtual_size for s in image.sections)
        fields = []
        for instruction in instructions:
            for operand in instruction.operands:
                if operand.type == CS_OP_IMM and in_image(operand.imm):
                    if instruction.group(CS_GRP_JUMP):
                        assert operand.imm - address in starts
                        continue
                    fields.append({'offset': instruction.address - address + instruction.imm_offset,
                                   'width': instruction.imm_size,
                                   'type': 'REL32' if instruction.mnemonic == 'call' else 'DIR32',
                                   'candidate_target': operand.imm})
                elif operand.type == CS_OP_MEM and operand.mem.base == 0 and in_image(operand.mem.disp):
                    fields.append({'offset': instruction.address - address + instruction.disp_offset,
                                   'width': instruction.disp_size, 'type': 'DIR32',
                                   'candidate_target': operand.mem.disp})
        fields += [{'offset': p, 'width': 4, 'type': 'DIR32',
                    'candidate_target': address + table[(p - code_size) // 4]}
                   for p in range(code_size, size, 4)]
    normalized = bytearray(raw)
    identities = []
    input_symbols = [s['address'] for s in symbols
                     if s['symbol'] == '?g_FrontEndInput@@3UFrontEndInputView@@A']
    for field in fields:
        offset, width, destination = field['offset'], field['width'], field['candidate_target']
        normalized[offset:offset + width] = b'\0' * width
        identity = {'offset': offset, 'width': width, 'type': field['type']}
        if address <= destination < address + size:
            identity['destination'] = {'local_offset': destination - address}
        elif input_symbols and destination in (input_symbols[0] + 4, input_symbols[0] + 6):
            identity['destination'] = {'symbol': '?g_FrontEndInput@@3UFrontEndInputView@@A',
                                       'addend': destination - input_symbols[0]}
        elif symbols:
            names = sorted(s for s in field['candidate_symbols'] if not s.startswith('_th10_'))
            assert names, f'unbound native destination {destination:#x}'
            identity['destination'] = {'symbols': names}
        else:
            identity['destination'] = {'target_address': f'0x{destination:08X}'}
        identities.append(identity)
    branches = []
    for instruction in instructions:
        if not instruction.group(CS_GRP_JUMP):
            continue
        if instruction.operands[0].type != CS_OP_IMM:
            assert instruction == switches[0]
            continue
        destination = instruction.operands[0].imm - address
        assert destination in starts
        branches.append({'offset': instruction.address - address,
                         'destination': destination, 'mnemonic': instruction.mnemonic})
    return {'complete_owned_bytes': size, 'code_bytes': code_size,
            'raw_sha256': hashlib.sha256(raw).hexdigest() if not symbols else None,
            'normalized_sha256': hashlib.sha256(normalized).hexdigest(),
            'instruction_count': len(instructions), 'field_identities': identities,
            'fixed_local_branches': branches, 'table_entry_offsets': table,
            'returns': [{'offset': i.address - address, 'operand': i.op_str}
                        for i in instructions if i.mnemonic == 'ret']}


def review(directory, graph):
    verify_capstone(tomllib.loads((ROOT / 'config/tools.lock.toml').read_text())['capstone'])
    compare = runpy.run_path(str(ROOT / 'scripts/compare-linked-function.py'))
    target = compare['verified_target']()
    native = PEImage(directory / 'source.exe')
    owners = linked_functions(directory / 'source.exe', directory / 'source.map',
                             directory / 'source.pdb')['functions']
    owner, = [f for f in owners if f['symbol'] == SYMBOL]
    assert owner['extent_source'] == 'pdb-section-contribution'
    assert owner['map_object'] == 'support-10-FrontEnd.ltcg.obj'
    symbols = map_publics(directory / 'source.map') + map_statics(directory / 'source.map')
    candidate = inventory(native, int(owner['address'], 0), owner['size'], symbols)
    retail = inventory(target, 0x433570, 1460)
    assert target.read_address(0x433B24, 12) == b'\xcc' * 12
    assert (ROOT / 'resources/th10.exe').read_bytes().count((0x433B10).to_bytes(4, 'little')) == 1
    manifest = tomllib.loads((ROOT / 'config/match-units.toml').read_text())
    guards = {}
    for name in GUARDS:
        unit = manifest['units'][name]
        producer = unit.get('pdb_source', unit['source'])
        unit.update(copy.deepcopy(graph))
        unit['pdb_source'] = producer
        unit['output_dir'] = str(directory)
        result = compare['compare_unit'](name, _unit_manifest=manifest)
        assert result['result'] == 'exact', result
        guards[name] = {'result': result['result'], 'matched_bytes': result['matched_bytes'],
                        'field_count': len(result['linkages']),
                        'linkage_spec_sha256': hashlib.sha256(json.dumps(
                            [(f['offset'], f['type'], f['symbol'], f['target'])
                             for f in unit['linkages']]).encode()).hexdigest()}
        if name == 'replay-manager-load':
            binding = result['linkages'][0]['allocation_eh']
            keys = ('saved_storage_offset', 'region_bytes', 'unwind_states', 'func_info_bytes')
            guards[name]['allocation_eh_binding'] = {
                'candidate': {k: binding['candidate'][k] for k in keys},
                'target': {k: binding['target'][k] for k in keys},
                'authority': binding['authority']}
    assert sum(g['matched_bytes'] for g in guards.values()) == 733
    return {'target_review': retail, 'native_reproduction': candidate, 'selected_guards': guards}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--artifact-dir', type=Path, required=True)
    args = parser.parse_args()
    directory = args.artifact_dir.resolve()
    directory.relative_to(ROOT / 'build')
    proof = json.loads(Path(__file__).with_name('results.json').read_text())
    actual = review(directory, proof['graph'])
    assert actual == proof['review'], 'complete native receipt differs'
    print(json.dumps({'result': 'native-candidate-reproduced-nonexact',
                      'candidate_size': actual['native_reproduction']['complete_owned_bytes'],
                      'target_size': 1460, 'selected_exact_guard_bytes': 733,
                      'new_exact_credit': 0}))


if __name__ == '__main__':
    main()
