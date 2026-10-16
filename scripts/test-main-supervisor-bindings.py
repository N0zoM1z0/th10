#!/usr/bin/env python3
"""Cross-unit semantic binding check, not native ABI or runtime proof.

The constructor-attested supervisor base is 0x491C28. Native compilation of
Main.hpp offsetof assertions, separately, establishes actual member offsets.
"""
import argparse
from collections import Counter
from copy import deepcopy
from pathlib import Path
import tomllib

SYMBOL = '?g_MainSupervisorView@@3UMainSupervisorView@@A'
BASE = 0x491C28
UNIT = 'frontend-options-refresh'
ROOT = Path(__file__).resolve().parents[1]


def bindings(data):
    for name, unit in data['units'].items():
        for kind in ('linkages', 'relocations'):
            for row in unit.get(kind, []):
                if row.get('symbol') == SYMBOL:
                    yield name, kind, row


def check_named_global_bases(data):
    owners = {}
    for name, unit in data['units'].items():
        for kind in ('linkages', 'relocations'):
            for row in unit.get(kind, []):
                symbol = row.get('symbol', '')
                if symbol.startswith(('?g_', '_g_')) and 'target' in row:
                    owners.setdefault(symbol, {}).setdefault(row['target'], []).append(name)
    for symbol, bases in owners.items():
        if len(bases) != 1:
            raise ValueError(f'Inconsistent named-global base for {symbol}: {bases}')
    return len(owners)


def check(data):
    check_named_global_bases(data)
    rows = list(bindings(data))
    if not rows:
        raise ValueError('No exact MainSupervisorView symbol bindings found')
    for name, kind, row in rows:
        if row.get('target') != BASE:
            raise ValueError(f'{name} {kind} offset={row.get("offset")}: '
                             f'expected supervisor base {BASE:#x}, got {row.get("target")!r}')
    refresh = [row for name, _, row in rows if name == UNIT]
    counts = Counter(row.get('addend') for row in refresh)
    if len(refresh) != 16 or counts != Counter({0x140: 8, 0x141: 8}):
        raise ValueError(f'{UNIT}: expected 16 references, 8 each at +0x140/+0x141; got {dict(counts)}')
    return len(rows)


def self_test(data):
    bad = deepcopy(data)
    bad['units']['global-control-a'] = {'linkages': [{'symbol': '?g_TestIdentity@@3HA', 'target': 0x400010}]}
    bad['units']['global-control-b'] = {'relocations': [{'symbol': '?g_TestIdentity@@3HA', 'target': 0x400020}]}
    expect_failure(bad, 'inconsistent-named-global-base')
    # Exercise each record category even if this manifest currently uses only one.
    for kind in ('linkages', 'relocations'):
        bad = deepcopy(data)
        bad['units']['failure-control'] = {kind: [{'symbol': SYMBOL, 'target': BASE + 8}]}
        expect_failure(bad, 'wrong-base-' + kind)
    bad = deepcopy(data)
    row = next(r for n, _, r in bindings(bad) if n == UNIT)
    row.pop('addend')
    expect_failure(bad, 'missing-field-addend')
    bad = deepcopy(data)
    next(r for n, _, r in bindings(bad) if n == UNIT)['addend'] = 0x138
    expect_failure(bad, 'old-field-addend')


def expect_failure(data, label):
    try:
        check(data)
    except ValueError:
        print('DETECTED ' + label)
    else:
        raise ValueError('Failure control escaped: ' + label)


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--manifest', type=Path, default=ROOT / 'config/match-units.toml')
    ap.add_argument('--self-test', action='store_true')
    args = ap.parse_args()
    data = tomllib.loads(args.manifest.read_text())
    try:
        count = check(data)
        if args.self_test:
            self_test(data)
    except ValueError as error:
        raise SystemExit(str(error))
    print(f'PASS {count} exact-symbol bindings at {BASE:#x}; {UNIT}: 8 +0x140, 8 +0x141')
    print(f'PASS {check_named_global_bases(data)} named-global symbols with consistent bases')
    print('Scope: manifest cross-unit semantic binding only; no ABI/runtime claim.')


if __name__ == '__main__':
    main()
