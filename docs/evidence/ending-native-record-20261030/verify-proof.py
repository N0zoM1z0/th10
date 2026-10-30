#!/usr/bin/env python3
"""Reproduce complete Ending candidates and explicitly selected exact contracts."""

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
from capstone import Cs, CS_ARCH_X86, CS_MODE_32, CS_OP_IMM, CS_OP_MEM
from linked_image import PEImage, linked_code_fields, linked_functions, map_publics, map_statics


def path_order(image, address, size, path_address, table_address=None):
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(image.read_address(address, size), address))
    assert sum(i.size for i in instructions) == size
    stores = [n for n, i in enumerate(instructions) if i.mnemonic == 'mov'
              and i.operands[0].type == CS_OP_MEM and i.operands[0].mem.base == 0
              and i.operands[0].mem.disp == path_address]
    assert len(stores) == 1
    if table_address is None:
        loads = [n for n, i in enumerate(instructions) if i.mnemonic == 'mov'
                 and len(i.operands) == 2 and i.operands[1].type == CS_OP_MEM
                 and i.operands[1].mem.disp == 0x70 and i.operands[1].mem.base != 0]
    else:
        loads = [n for n, i in enumerate(instructions) if i.mnemonic == 'mov'
                 and len(i.operands) == 2 and i.operands[1].type == CS_OP_MEM
                 and i.operands[1].mem.base == 0
                 and i.operands[1].mem.disp == table_address]
    assert len(loads) == 1 and loads[0] < stores[0]
    return {'path_load_offset': instructions[loads[0]].address - address,
            'buffer_clear_offset': instructions[stores[0]].address - address}


def review(directory, graph):
    previous = runpy.run_path(str(ROOT / 'docs/evidence/ending-context-repair-20261030/verify-proof.py'))
    result = previous['review'](directory)
    target_inventory = result['target_run_inventory']
    native_inventory = result['native_run_reproduction']
    assert native_inventory['size'] == target_inventory['size'] == 1568
    assert native_inventory['code_and_alignment_size'] == 1496
    assert native_inventory['local_branches'] == target_inventory['local_branches']
    assert native_inventory['table_local_offsets'] == target_inventory['table_local_offsets']
    image = PEImage(directory / 'source.exe')
    comparator = runpy.run_path(str(ROOT / 'scripts/compare-linked-function.py'))
    target = comparator['verified_target']()
    functions = linked_functions(directory / 'source.exe', directory / 'source.map',
                                 directory / 'source.pdb')['functions']
    publics = map_publics(directory / 'source.map')
    all_symbols = publics + map_statics(directory / 'source.map')
    owner, = [f for f in functions if f['symbol'] == '?Run@EndingTextVmView@@QAGHXZ']
    address = int(owner['address'], 0)
    mask = set()
    for field in target_inventory['fields']:
        mask.update(range(field['offset'], field['offset'] + field['width']))
    assert [(f['offset'], f['width'], f['type']) for f in native_inventory['field_identities']] == [
        (f['offset'], f['width'], f['type']) for f in target_inventory['fields']]
    raw = image.read_address(address, 1568)
    retail = target.read_address(0x40BD80, 1568)
    differences = [{'offset': offset, 'candidate': left, 'target': right}
                   for offset, (left, right) in enumerate(zip(raw, retail))
                   if offset not in mask and left != right]
    assert len(mask) == 368 and len(differences) == 54
    result['run_ordinary_comparison'] = {
        'result': 'mismatch', 'acceptance_authority': 'none',
        'compared_bytes': 1200, 'matched_bytes': 1146, 'differences': differences}
    manifest = tomllib.loads((ROOT / 'config/match-units.toml').read_text())
    unit = manifest['units']['enemy-mark-pending-interrupt']
    producer = unit.get('pdb_source', unit['source'])
    unit.update(copy.deepcopy(graph))
    unit['pdb_source'] = producer
    unit['output_dir'] = str(directory)
    bound = comparator['compare_unit']('enemy-mark-pending-interrupt', _unit_manifest=manifest)
    assert bound['result'] == 'exact' and bound['matched_bytes'] == 65
    result['strict_comparisons']['enemy-mark-pending-interrupt'] = {
        'result': bound['result'], 'matched_bytes': 65, 'field_count': len(bound['linkages'])}
    assert sum(r['matched_bytes'] for r in result['strict_comparisons'].values()) == 860
    path_address, = {s['address'] for s in publics if s['symbol'].startswith('?g_StageFilePath@@')}
    table_address, = {s['address'] for s in publics if s['symbol'].startswith('?g_EndingScriptPaths@@')}
    samples = {}
    for key, symbol, target_address, target_size, table in (
        ('initialize', '?Initialize@EndingView@@QAEHXZ', 0x40B560, 584, True),
        ('load_animation', '?LoadAnimation@EndingTextVmView@@QAEHXZ', 0x40C3C0, 126, False)):
        function, = [f for f in functions if f['symbol'] == symbol]
        assert function['map_object'] == 'source.ltcg.obj'
        body_address, body_size = int(function['address'], 0), function['size']
        fields = linked_code_fields(image, all_symbols, body_address, body_size)
        assert fields['normalization_complete']
        body = bytearray(image.read_address(body_address, body_size))
        for field in fields['fields']:
            body[field['offset']:field['offset'] + field['width']] = b'\0' * field['width']
        samples[key] = {
            'candidate_size': function['size'], 'target_size': target_size,
            'result': 'nonexact',
            'native_field_count': len(fields['fields']),
            'normalized_native_sha256': hashlib.sha256(body).hexdigest(),
            'native': path_order(image, int(function['address'], 0), function['size'],
                                 path_address, table_address if table else None),
            'target': path_order(target, target_address, target_size, 0x497C38,
                                 0x474440 if table else None)}
    result['path_sampling'] = samples
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--artifact-dir', type=Path, required=True)
    args = parser.parse_args()
    directory = args.artifact_dir.resolve()
    directory.relative_to(ROOT / 'build')
    proof = json.loads(Path(__file__).with_name('results.json').read_text())
    assert hashlib.sha256((ROOT / 'resources/th10.exe').read_bytes()).hexdigest() == proof['target_sha256']
    actual = review(directory, proof['graph'])
    assert actual == proof['review'], 'complete candidate receipt differs'
    compare = runpy.run_path(str(ROOT / 'scripts/compare-linked-function.py'))['compare_unit']
    total = 0
    for name in proof['affected_canonical_selectors']:
        bound = compare(name)
        assert bound['result'] == 'exact'
        total += bound['matched_bytes']
    assert total == 871
    print(json.dumps({'result': 'ending-source-progress-run-nonexact',
                      'complete_run_bytes': 1568, 'ordinary_differences': 54,
                      'fields': 92, 'fixed_locals': 53, 'table_words': 18,
                      'affected_canonical_bytes': 871, 'new_exact_credit': 0}))


if __name__ == '__main__':
    main()
