#!/usr/bin/env python3
"""Full, target-bound strict rejection proof for TH10 ANM ExecuteScript."""

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


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--artifact-dir', type=Path, required=True)
    args = parser.parse_args()
    artifact = args.artifact_dir.resolve()
    artifact.relative_to((ROOT / 'build').resolve())
    contract = json.loads((ROOT / 'docs/evidence/anm-child-request-record/binding-contract.json').read_text())
    expected = contract['target_sha256']
    assert hashlib.sha256((ROOT / 'resources/th10.exe').read_bytes()).hexdigest() == expected
    assert len(contract['linkages']) == 350
    assert sum(bool(x.get('local')) for x in contract['linkages']) == 95

    graph = dict(
        source='src/AnmManager.cpp',
        support_sources=['src/RandomMath.cpp', 'src/AnmVmCreate.cpp',
                         'src/AnmVmId.cpp', 'src/AnmInt3Scale.cpp'],
        profile=['/TP', '/MT', '/O2', '/Gy', '/GF', '/Oi', '/DNDEBUG',
                 '/Isrc', '/GL', '/GS'],
        support_profiles={'src/AnmInt3Scale.cpp': ['/TP', '/MT', '/O2',
                         '/Gy', '/GF', '/Oi', '/DNDEBUG', '/Isrc',
                         '/GL', '/GS', '/Ob1']},
        entry_symbol='?ExecuteScript@AnmRenderManagerView@@SGHPAUAnmVmView@@@Z',
        artifact_kind='linked-pe',
        output_dir=str(artifact.relative_to(ROOT)),
        symbol='?ExecuteScript@AnmRenderManagerView@@SGHPAUAnmVmView@@@Z',
        functions=['AnmRenderManagerView::ExecuteScript'],
        target_address=0x0043EE30,
        size=9964,
        linkages=[{k: v for k, v in x.items() if k != 'evidence'}
                  for x in contract['linkages']],
    )
    with (ROOT / 'config/match-units.toml').open('rb') as stream:
        unit_manifest = tomllib.load(stream)
    unit_manifest['units']['diagnostic-anm-executor'] = graph
    compare = runpy.run_path(str(ROOT / 'scripts/compare-linked-function.py'))['compare_unit']
    result = compare('diagnostic-anm-executor', _unit_manifest=unit_manifest)
    assert result['result'] == 'mismatch', result.get('result')
    assert result['size'] == 9964 and result['matched_bytes'] == 9962
    assert len(result['linkages']) == 350
    assert len([x for x in result['linkages'] if x['local']]) == 95
    diffs = result['first_differences']
    assert [int(x['offset'], 16) for x in diffs] == [0x113, 0x115], diffs
    assert all(x['linked'] != x['target'] for x in diffs)
    print(json.dumps(dict(result='complete-nonexact', target_size=9964,
                          native_size=9964, matched_bytes=9962,
                          independently_bound_fields=350,
                          local_destinations=95,
                          ordinary_differences=diffs,
                          exact_credit=0), indent=2))


if __name__ == '__main__':
    main()
