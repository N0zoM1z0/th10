#!/usr/bin/env python3
"""Strict full-owner Replay frame allocation and sibling guard in a scoped PE."""
from __future__ import annotations

import argparse
import copy
import json
from pathlib import Path
import runpy
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / 'scripts'))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--artifact-dir', type=Path,
                        default=Path('build/match-linked/ReplayFrameAllocate'))
    args = parser.parse_args()
    path = (ROOT / args.artifact_dir).resolve()
    path.relative_to((ROOT / 'build').resolve())
    with (ROOT / 'config/match-units.toml').open('rb') as handle:
        config = tomllib.load(handle)
    base = config['units']['replay-allocate-frame-data']
    unit = copy.deepcopy(base)
    unit['output_dir'] = str(path.relative_to(ROOT))
    config['units']['diagnostic-replay-allocation'] = unit
    compare = runpy.run_path(str(ROOT / 'scripts/compare-linked-function.py'))['compare_unit']
    result = compare('diagnostic-replay-allocation', _unit_manifest=config)
    assert result['result'] == 'exact' and result['size'] == 139
    assert result['matched_bytes'] == 139 and len(result['linkages']) == 1
    assert result['linkages'][0]['offset'] == '0x6'
    assert result['linkages'][0]['target'] == '0x00452493'

    guards = {}
    for name, size, count in [
        ('replay-clear-stage-frame-data', 93, 1),
        ('replay-restore-stage-position', 83, 2),
    ]:
        unit = copy.deepcopy(config['units'][name])
        unit.update(source=base['source'],
                    support_sources=base['support_sources'],
                    pdb_source='src/ReplayStageHelpers.cpp',
                    entry_symbol=base['entry_symbol'],
                    output_dir=str(path.relative_to(ROOT)),
                    profile=base['profile'], link_profile=base['link_profile'])
        unit.pop('support_profiles', None)
        config['units']['diagnostic-replay-sibling'] = unit
        guard = compare('diagnostic-replay-sibling', _unit_manifest=config)
        assert (guard['result'], guard['size'], guard['matched_bytes'], len(guard['linkages'])) == ('exact', size, size, count), guard
        guards[name] = {'bytes': size, 'fields': count, 'result': 'exact'}
    print(json.dumps({'result': 'exact', 'complete_owner': 'ReplayAllocateFrameData',
                      'size': 139, 'matched_bytes': 139, 'linkages': 1,
                      'guarded_existing_siblings': guards, 'new_exact_credit': 139}, indent=2))


if __name__ == '__main__':
    main()
