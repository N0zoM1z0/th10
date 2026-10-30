#!/usr/bin/env python3
"""Replay the complete integer selector owner and review the nonexact float owner."""

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


def inventory(image, address, size, code_size, symbols=()):
    raw = image.read_address(address, size)
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw[:code_size], address))
    assert sum(i.size for i in instructions) == code_size
    starts = {i.address - address for i in instructions}
    assert instructions[1].mnemonic == 'add' and instructions[1].operands[1].imm == 10000
    assert instructions[2].mnemonic == 'cmp' and instructions[2].operands[1].imm == 50
    switches = [i for i in instructions if i.mnemonic == 'jmp' and i.operands[0].type == CS_OP_MEM]
    assert len(switches) == 1 and switches[0].operands[0].mem.disp == address + code_size
    assert size - code_size == 204
    table = [int.from_bytes(raw[p:p + 4], 'little') - address
             for p in range(code_size, size, 4)]
    assert len(table) == 51 and all(e in starts for e in table)
    if symbols:
        report = linked_code_fields(image, list(symbols), address, size)
        assert report['normalization_complete']
        fields = report['fields']
    else:
        # The retail image has stripped relocations; inspect its actual operands.
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
    identities = []
    normalized = bytearray(raw)
    for field in fields:
        offset, width, destination = field['offset'], field['width'], field['candidate_target']
        normalized[offset:offset + width] = b'\0' * width
        row = {'offset': offset, 'width': width, 'type': field['type']}
        if address <= destination < address + size:
            row['destination'] = {'local_offset': destination - address}
        elif symbols:
            names = sorted(s for s in field['candidate_symbols'] if not s.startswith('_th10_'))
            assert names, f'unbound native field at {offset:#x}'
            row['destination'] = {'symbols': names}
        else:
            row['destination'] = {'target_address': f'0x{destination:08X}'}
        identities.append(row)
    branches = []
    for instruction in instructions:
        if instruction.group(CS_GRP_JUMP) and instruction.operands[0].type == CS_OP_IMM:
            destination = instruction.operands[0].imm - address
            assert destination in starts
            branches.append({'offset': instruction.address - address,
                             'destination': destination, 'mnemonic': instruction.mnemonic})
    return {'complete_owned_bytes': size, 'code_and_alignment_bytes': code_size,
            'instruction_count': len(instructions),
            'normalized_sha256': hashlib.sha256(normalized).hexdigest(),
            'field_identities': identities, 'fixed_local_branches': branches,
            'table_entry_offsets': table,
            'calls': [{'offset': i.address - address, 'destination': f'0x{i.operands[0].imm:08X}'}
                      for i in instructions if i.mnemonic == 'call'] if not symbols else None,
            'returns': [{'offset': i.address - address, 'operand': i.op_str}
                        for i in instructions if i.mnemonic == 'ret']}


def review(directory):
    verify_capstone(tomllib.loads((ROOT / 'config/tools.lock.toml').read_text())['capstone'])
    compare = runpy.run_path(str(ROOT / 'scripts/compare-linked-function.py'))
    target = compare['verified_target']()
    native = PEImage(directory / 'source.exe')
    functions = linked_functions(directory / 'source.exe', directory / 'source.map',
                                 directory / 'source.pdb')['functions']
    symbols = map_publics(directory / 'source.map') + map_statics(directory / 'source.map')
    manifest = tomllib.loads((ROOT / 'config/match-units.toml').read_text())
    unit = manifest['units']['enemy-read-int-operand']
    graph = {key: copy.deepcopy(unit[key]) for key in (
        'source', 'support_sources', 'profile', 'support_profiles', 'entry_symbol',
        'link_profile', 'harness')}
    output = {}
    for key, symbol, address, size, code in (
        ('int', '?ReadIntOperand@EnemyFullObjectView@@UAEHH@Z', 0x411FC0, 820, 616),
        ('float', '?ReadFloatOperand@EnemyFullObjectView@@UAEMH@Z', 0x412350, 884, 680)):
        owner, = [f for f in functions if f['symbol'] == symbol]
        assert owner['extent_source'] == 'pdb-section-contribution'
        assert owner['map_object'] == 'source.ltcg.obj'
        candidate = inventory(native, int(owner['address'], 0), owner['size'], owner['size'] - 204, symbols)
        retail = inventory(target, address, size, code)
        assert target.read_address(address + size, 12) == b'\xcc' * 12
        assert (ROOT / 'resources/th10.exe').read_bytes().count((address + code).to_bytes(4, 'little')) == 1
        if key == 'int':
            fields = unit['linkages']
            assert [(f['offset'], f['type'], int(f['destination']['target_address'], 0)
                     if 'target_address' in f['destination'] else address + f['destination']['local_offset'])
                    for f in retail['field_identities']] == [
                        (f['offset'], f['type'], f['target']) for f in fields]
            assert len(fields) == 95 and sum(bool(f.get('local')) for f in fields) == 52
            for property_ in ('complete_owned_bytes', 'code_and_alignment_bytes',
                              'instruction_count', 'normalized_sha256', 'fixed_local_branches',
                              'table_entry_offsets', 'returns'):
                assert candidate[property_] == retail[property_]
        output[key] = {'target_review': retail, 'native_reproduction': candidate}
    selected = {}
    for name in ('enemy-read-int-operand', 'anm-vm-id-get-vm', 'enemy-angle-from-player'):
        selected_unit = manifest['units'][name]
        producer = selected_unit.get('pdb_source', selected_unit['source'])
        selected_unit.update(copy.deepcopy(graph))
        selected_unit['pdb_source'] = producer
        selected_unit['output_dir'] = str(directory)
        result = compare['compare_unit'](name, _unit_manifest=manifest)
        assert result['result'] == 'exact', result
        selected[name] = {'result': result['result'], 'matched_bytes': result['matched_bytes'],
                          'field_count': len(result['linkages'])}
    assert sum(r['matched_bytes'] for r in selected.values()) == 908
    output['selected_strict_comparisons'] = selected
    return output


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--artifact-dir', type=Path, required=True)
    args = parser.parse_args()
    directory = args.artifact_dir.resolve()
    directory.relative_to(ROOT / 'build')
    proof = json.loads(Path(__file__).with_name('results.json').read_text())
    assert hashlib.sha256((ROOT / 'resources/th10.exe').read_bytes()).hexdigest() == proof['target_sha256']
    unit = tomllib.loads((ROOT / 'config/match-units.toml').read_text())['units']['enemy-read-int-operand']
    assert {key: unit[key] for key in proof['graph']} == proof['graph']
    actual = review(directory)
    assert actual == proof['review'], 'complete owner receipt differs'
    print(json.dumps({'result': 'int-exact-float-nonexact', 'new_exact_bytes': 820,
                      'bound_fields': 95, 'fixed_table_entries': 51,
                      'same_image_guard_bytes': 88, 'float_candidate_size': 876,
                      'float_target_size': 884}))


if __name__ == '__main__':
    main()
