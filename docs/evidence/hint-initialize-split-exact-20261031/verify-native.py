#!/usr/bin/env python3
"""Strict complete native PE/PDB owner checks across the scoped Hint source split.

Uses existing canonical linked artifacts; no build, no normalized byte waivers.
"""
from __future__ import annotations

import json
from pathlib import Path
import runpy
import sys

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / 'scripts'))
compare = runpy.run_path(str(ROOT / 'scripts/compare-linked-function.py'))['compare_unit']

GROUPS = {
    'lifecycle': [
        ('hint-initialize', 131, 13),
        ('hint-create-owner', 67, 5),
        ('hint-update-callback', 10, 1),
        ('hint-draw-callback', 6, 0),
        ('hint-frame-update', 571, 17),
    ],
    'loader-gl': [
        ('hint-clear-lists', 79, 2),
        ('hint-trim-whitespace', 114, 0),
        ('hint-split-line', 75, 4),
        ('hint-lookup-value', 98, 0),
    ],
    'loader-gs': [
        ('hint-read-line', 260, 4),
        ('hint-append-link', 52, 0),
    ],
}


def main() -> None:
    summary = {}
    for group, units in GROUPS.items():
        total = 0
        fields = 0
        for name, size, expected_fields in units:
            result = compare(name)
            assert result['result'] == 'exact', (name, result.get('error'))
            assert result['size'] == size and result['matched_bytes'] == size, (name, result)
            assert len(result['linkages']) == expected_fields, (name, result)
            total += size
            fields += expected_fields
        summary[group] = {'status': 'exact', 'bytes': total, 'bound_fields': fields,
                          'owners': len(units)}
    assert [summary[x]['bytes'] for x in GROUPS] == [785, 366, 312]
    assert sum(x['bound_fields'] for x in summary.values()) == 46
    print(json.dumps({'result': 'exact', 'selected_native_owners': summary,
                      'new_exact_credit_only': 131,
                      'parser_full_owner_exact_credit': 0}, indent=2))


if __name__ == '__main__':
    main()
