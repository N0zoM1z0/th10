#!/usr/bin/env python3
"""Strictly reject the not-yet-exact complete 74-byte Chain::AllocElem owner."""
from __future__ import annotations
import copy
import hashlib
import json
from pathlib import Path
import runpy
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / 'scripts'))

def main() -> None:
    with (ROOT / 'config/target.toml').open('rb') as f:
        target_sha256 = tomllib.load(f)['target']['sha256']
    assert hashlib.sha256((ROOT / 'resources/th10.exe').read_bytes()).hexdigest() == target_sha256
    with (ROOT / 'config/match-units.toml').open('rb') as f:
        manifest = tomllib.load(f)
    unit = copy.deepcopy(manifest['units']['game-owner-40af90-create'])
    unit.update(
        pdb_source='src/Chain.cpp', functions=['th10::Chain::AllocElem'],
        symbol='?AllocElem@Chain@th10@@SGPAUChainElem@2@P6IHPAX@Z@Z',
        target_address=0x00449ED0, size=74,
        linkages=[dict(offset=3, type='REL32', symbol='??2@YAPAXI@Z', target=0x00452493)],
    )
    manifest['units']['chain-alloc-nonexact-check'] = unit
    compare = runpy.run_path(str(ROOT / 'scripts/compare-linked-function.py'))['compare_unit']
    result = compare('chain-alloc-nonexact-check', _unit_manifest=manifest)
    assert result['result'] == 'mismatch', result
    assert result['size'] == 74 and result['matched_bytes'] == 52, result
    assert result['normalization_complete'] is True, result
    assert len(result['linkages']) == 1, result
    assert [int(d['offset'],16) for d in result['first_differences']] == [
        0x08, 0x0D, 0x11, 0x14, 0x17, 0x1A, 0x1D, 0x1F, 0x22, 0x28, 0x2B,
        0x32, 0x37, 0x38, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x43,
    ], result
    print(json.dumps(dict(
        result='full-owner-nonexact', target_address='0x00449ED0',
        target_bytes=74, native_bytes=result['size'],
        matched_bytes=result['matched_bytes'],
        bound_fields=len(result['linkages']),
        ordinary_differences=result['first_differences'],
        exact_credit=0,
    ), indent=2))

if __name__ == '__main__':
    main()
