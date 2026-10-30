#!/usr/bin/env python3
"""Replay the repaired Ending contracts and the complete nonexact Run inventory."""

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
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from linked_image import PEImage, linked_code_fields, linked_functions, map_publics, map_statics, verify_capstone


def review(directory):
    verify_capstone(tomllib.loads((ROOT / 'config/tools.lock.toml').read_text())['capstone'])
    comparator = runpy.run_path(str(ROOT / 'scripts/compare-linked-function.py'))
    target = comparator['verified_target']()
    image = PEImage(directory / 'source.exe')
    functions = linked_functions(directory / 'source.exe', directory / 'source.map',
                                 directory / 'source.pdb')['functions']
    symbols = map_publics(directory / 'source.map') + map_statics(directory / 'source.map')
    manifest = tomllib.loads((ROOT / 'config/match-units.toml').read_text())
    unit = manifest['units']['ending-text-constructor']
    graph = {k: copy.deepcopy(unit[k]) for k in (
        'source', 'support_sources', 'support_profiles', 'profile',
        'entry_symbol', 'link_profile', 'harness')}
    selected = {}
    for name in ('ending-text-constructor', 'ending-load-script', 'game-decomphelp-fulltu-00417010'):
        selected_unit = manifest['units'][name]
        producer = selected_unit.get('pdb_source', selected_unit['source'])
        selected_unit.update(copy.deepcopy(graph))
        selected_unit['pdb_source'] = producer
        selected_unit['output_dir'] = str(directory)
        result = comparator['compare_unit'](name, _unit_manifest=manifest)
        assert result['result'] == 'exact', result
        selected[name] = {'result': result['result'], 'matched_bytes': result['matched_bytes'],
                          'field_count': len(result['linkages'])}
        if name == 'ending-text-constructor':
            binding, = [f['worker_member_eh'] for f in result['linkages'] if 'worker_member_eh' in f]
            selected[name]['worker_member_eh_binding'] = {
                'authority': binding['authority'],
                'candidate': {k: v for k, v in binding['candidate'].items()
                              if k not in ('handler', 'unwind_action', 'func_info', 'unwind_map')},
                'target': {k: v for k, v in binding['target'].items()
                           if k not in ('handler', 'unwind_action', 'func_info', 'unwind_map')}}
    assert sum(r['matched_bytes'] for r in selected.values()) == 795
    audit = runpy.run_path(str(ROOT / 'scripts/audit-ending-run-native.py'))
    owner, = [f for f in functions if f['symbol'] == '?Run@EndingTextVmView@@QAGHXZ']
    assert owner['extent_source'] == 'pdb-section-contribution'
    assert owner['map_object'] == 'source.ltcg.obj'
    address, size = int(owner['address'], 0), owner['size']
    native = audit['inventory_run'](image, address, size)
    retail = audit['inventory_run'](target, 0x40BD80, 1568)
    assert len(native['fields']) == len(retail['fields']) == 92
    assert len(native['local_branches']) == len(retail['local_branches']) == 53
    assert retail['code_and_alignment_size'] == 1496
    assert retail['table_words'] == 18
    assert [f['offset'] for f in retail['virtual_only_immediate_fields']] == [0x204, 0x284]
    assert all(f['destination'] == '0x492590' for f in retail['virtual_only_immediate_fields'])
    assert (ROOT / 'resources/th10.exe').read_bytes().count((0x40C358).to_bytes(4, 'little')) == 1
    report = linked_code_fields(image, symbols, address, size)
    assert report['normalization_complete']
    assert [(f['offset'], f['width'], f['type']) for f in report['fields']] == [
        (f['offset'], f['width'], f['type']) for f in native['fields']]
    normalized = bytearray(image.read_address(address, size))
    input_base, = [s['address'] for s in symbols if s['symbol'] == '?g_FrontEndInput@@3UFrontEndInputView@@A']
    identities = []
    for field in report['fields']:
        offset, width, destination = field['offset'], field['width'], field['candidate_target']
        normalized[offset:offset + width] = b'\0' * width
        row = {'offset': offset, 'width': width, 'type': field['type']}
        if address <= destination < address + size:
            row['destination'] = {'local_offset': destination - address}
        elif destination == input_base + 6:
            row['destination'] = {'symbol': '?g_FrontEndInput@@3UFrontEndInputView@@A', 'addend': 6}
        else:
            names = sorted(s for s in field['candidate_symbols'] if not s.startswith('_th10_'))
            assert names, f'unbound native Run field {offset:#x}'
            row['destination'] = {'symbols': names}
        identities.append(row)
    native = {k: v for k, v in native.items() if k not in ('fields', 'virtual_only_immediate_fields')}
    native['field_identities'] = identities
    native['normalized_sha256'] = hashlib.sha256(normalized).hexdigest()
    initializer, = [f for f in functions if f['symbol'] == '?InitializeVm@AnmLoadedView@@QAEXPAUAnmVmView@@H@Z']
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    instructions = list(decoder.disasm(image.read_address(int(initializer['address'], 0), initializer['size']),
                                     int(initializer['address'], 0)))
    assert instructions[-1].mnemonic == 'ret' and instructions[-1].op_str == '8'
    assert sum(i.size for i in instructions) == initializer['size'] == 207
    return {'strict_comparisons': selected, 'target_run_inventory': retail,
            'native_run_reproduction': native,
            'initializer_interface': {'candidate_size': 207, 'target_size': 217, 'return_operand': '8',
                                      'body_exact': False}}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--artifact-dir', type=Path, required=True)
    args = parser.parse_args()
    directory = args.artifact_dir.resolve()
    directory.relative_to(ROOT / 'build')
    proof = json.loads(Path(__file__).with_name('results.json').read_text())
    assert hashlib.sha256((ROOT / 'resources/th10.exe').read_bytes()).hexdigest() == proof['target_sha256']
    units = tomllib.loads((ROOT / 'config/match-units.toml').read_text())['units']
    for name in ('ending-text-constructor', 'ending-load-script'):
        assert {k: units[name][k] for k in proof['graph']} == proof['graph']
    actual = review(directory)
    assert actual == proof['review'], 'complete repaired context receipt differs'
    print(json.dumps({'result': 'ending-contracts-restored-run-nonexact',
                      'canonical_bytes': 754, 'same_image_decoder_bytes': 41,
                      'run_candidate_size': 1564, 'run_target_size': 1568,
                      'native_run_fields': 92, 'new_exact_credit': 0}))


if __name__ == '__main__':
    main()
