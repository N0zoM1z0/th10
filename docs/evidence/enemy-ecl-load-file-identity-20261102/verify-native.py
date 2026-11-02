"""Strict nonexact 95-byte ECL resource loader comparison in selected Enemy image.

Requires: scripts/repo-python scripts/build-match-unit.py --unit enemy-manager-constructor
This is a diagnostic-only unit; do not add it to the exact-unit manifest.
"""
from __future__ import annotations

import copy
import json
import runpy
import sys
import tomllib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / 'scripts'))
with (ROOT / 'config/match-units.toml').open('rb') as stream:
    manifest = tomllib.load(stream)
unit = copy.deepcopy(manifest['units']['enemy-manager-constructor'])
unit.update(
    functions=['EnemyEclResourceView::LoadFile'],
    symbol='?LoadFile@EnemyEclResourceView@@UAEHPBD@Z',
    target_address=0x0040CD20,
    size=95,
    linkages=[
        {'offset':0x0B, 'type':'DIR32','symbol':'?g_EnemyEclFilenameBuffer@@3PADA','target':0x00497C38},
        {'offset':0x1A, 'type':'DIR32','symbol':'?g_EnemyEclFilenameBuffer@@3PADA','target':0x00497C38},
        {'offset':0x3C, 'type':'DIR32','symbol':'?g_EnemyEclFilenameBuffer@@3PADA','target':0x00497C38},
        {'offset':0x43, 'type':'REL32','symbol':'?OpenFile@FileSystem@@YGPAEPBDPAHH@Z','target':0x0044B360},
        {'offset':0x4B, 'type':'REL32','symbol':'?AddScriptData@EclVmScriptDatabase@@UAEHPAX@Z','target':0x00450220},
    ],
)
manifest['units']['enemy-loader-diagnostic'] = unit
compare = runpy.run_path(str(ROOT / 'scripts/compare-linked-function.py'))['compare_unit']
r = compare('enemy-loader-diagnostic', _unit_manifest=manifest)
print(json.dumps({k:r.get(k) for k in (
    'result','target_address','symbol','size','matched_bytes','decoded_bytes',
    'normalization_complete','extent_source','linkages','first_differences',
)}, indent=2))
assert r['result'] == 'mismatch', r
assert r['size'] == 95 and r['matched_bytes'] == 85, r
assert r['decoded_bytes'] == 95 and r['normalization_complete'], r
assert len(r['linkages']) == 5, r
assert [v['offset'] for v in r['first_differences']] == [
    f'0x{i:02X}' for i in range(0x32,0x3c)
], r
