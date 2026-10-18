#!/usr/bin/env python3
"""Source-only checkpoint verifier and canonical command planner. Never builds."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import re
import sys
import tomllib

MANIFEST_SHA256 = '7fa1ac981796b4b4a559f731e229fd5c9dc4a7f9404e4fb740628503402396c5'
MANIFEST_NAME = 'radial_reproduction.json'
RECIPE_KEYS = ('artifact_kind', 'source', 'support_sources', 'normal_support_sources',
               'object', 'output_dir', 'profile', 'link_profile', 'harness', 'entry_symbol')
HEX = re.compile(r'[0-9a-f]{64}\Z')
# Plan DATA only. A separately authorized, reviewed executor must enforce the
# durable claims/resources/pre-post checks before invoking any generated argv.
LINK_CODE = '''import os,sys,tomllib
from pathlib import Path
root=Path.cwd().resolve()
sys.path.insert(0,str(root/'scripts'))
import ltcg_link as l
u=tomllib.loads((root/'config/match-units.toml').read_text())['units'][sys.argv[1]]
assert u['artifact_kind']=='linked-pe'
assert u['link_profile']==l.LINK_PROFILE and u['harness']==l.HARNESS_KIND
linker,env=l.tool_environment()
os.environ.update(env)
l.cold_link(root/u['source'],root/u['output_dir'],u['entry_symbol'],linker,env,
 list(u['profile']),[root/p for p in u.get('support_sources',[])],
 [root/p for p in u.get('normal_support_sources',[])])
'''


class VerificationError(ValueError):
    """Opaque failure; untrusted input and native diagnostics are never echoed."""


def require(condition):
    if not condition:
        raise VerificationError('verification-failed')


def digest(data):
    return hashlib.sha256(data).hexdigest()


def canonical(value):
    return json.dumps(value, sort_keys=True, separators=(',', ':'), allow_nan=False).encode()


def exact(left, right):
    return canonical(left) == canonical(right)


def unique_object(pairs):
    result = {}
    for key, value in pairs:
        require(key not in result)
        result[key] = value
    return result


def read_file(root, relative, limit=4 * 1024 * 1024):
    """Only bounded regular, non-symlink files within the supplied tree."""
    require(type(relative) is str and '\\' not in relative)
    parts = PurePosixPath(relative).parts
    require(parts and not PurePosixPath(relative).is_absolute())
    require(all(p not in ('.', '..', '') for p in parts))
    path = Path(root).absolute()
    require(not any(p.is_symlink() for p in (path, *path.parents)))
    for part in parts:
        path = path / part
        require(not path.is_symlink())
    require(path.is_file() and path.stat().st_size <= limit)
    with path.open('rb') as stream:
        data = stream.read(limit + 1)
    require(len(data) <= limit)
    return data


def load_manifest(directory):
    raw = read_file(directory, MANIFEST_NAME, 65536)
    require(digest(raw) == MANIFEST_SHA256)
    manifest = json.loads(raw, object_pairs_hook=unique_object)
    require(manifest['schema'] == 'radial-reproduction-source-v1')
    require(len(manifest['source_pins']) == 83 and len(manifest['units']) == 29)
    require(len(manifest['recipes']) == 12)
    return manifest


def recipe_of(unit):
    return {k: unit.get(k, [] if k.endswith('sources') else None) for k in RECIPE_KEYS}


def verify_config(raw, manifest):
    new = manifest['new_unit']
    n = new['serialized_bytes']
    require(len(raw) > n + 1 and raw[-n - 1:-n] == b'\n')
    unit_raw, baseline_raw = raw[-n:], raw[:-n - 1]
    require(unit_raw.endswith(b'\n') and baseline_raw.endswith(b'\n'))
    require(digest(unit_raw) == new['serialized_sha256'])
    require(digest(baseline_raw) == manifest['baseline_config_sha256'])
    before, after = (tomllib.loads(x.decode()) for x in (baseline_raw, raw))
    old, units = before['units'], after['units']
    require(set(units) == set(old) | {new['name']} and new['name'] not in old)
    require(exact({k: v for k, v in after.items() if k != 'units'},
                  {k: v for k, v in before.items() if k != 'units'}))
    require(all(exact(units[k], v) for k, v in old.items()))
    for name, expected in manifest['units'].items():
        require(digest(canonical(units[name])) == expected['sha256'])
    seen = []
    for phase in manifest['recipes']:
        require(phase['build_unit'] in phase['units'])
        for name in phase['units']:
            require(exact(recipe_of(units[name]), phase['recipe']))
            seen.append(name)
    require(len(seen) == 30 and len(set(seen)) == 30)
    require(set(seen) == set(manifest['units']) | {new['name']})
    unit = units[new['name']]
    require(type(unit['size']) is int and unit['size'] == new['size'] == 803)
    require(unit.get('compare_size', unit['size']) == 803)
    fields = unit['linkages']
    require(len(fields) == new['field_count'] == 43 and new['field_bytes'] == 172)
    occupied = set()
    for field in fields:
        offset = field['offset']
        require(type(offset) is int and 0 <= offset <= 799)
        require(field['type'] in ('DIR32', 'REL32'))
        span = set(range(offset, offset + 4))
        require(not occupied & span)
        occupied.update(span)
    require(len(occupied) == 172)
    return {'config_sha256': digest(raw), 'unit_sha256': digest(unit_raw)}


def verify_source(repo, manifest, helpers):
    for table, root in ((manifest['source_pins'], repo), (manifest['tool_pins'], repo),
                        (manifest['helper_pins'], helpers)):
        for relative, expected in table.items():
            require(digest(read_file(root, relative)) == expected)
    result = verify_config(read_file(repo, 'config/match-units.toml'), manifest)
    result.update({'schema': 'radial-source-verification-v1', 'source_verified': True,
                   'source_count': 83, 'guard_count': 29, 'recipe_count': 12,
                   'manifest_sha256': MANIFEST_SHA256,
                   'source_pins_sha256': digest(canonical(manifest['source_pins'])),
                   'target_expected_sha256': manifest['target_sha256'],
                   'target_read': False, 'native_executed': False,
                   'factory_acceptance': 'pending'})
    return result


def native_plan(manifest, verified):
    phases = []
    for phase in manifest['recipes']:
        recipe = phase['recipe']
        coff = recipe['artifact_kind'] == 'coff'
        build = (['bash', 'scripts/compile-probe.sh', recipe['source'], recipe['object'],
                  *recipe['profile']] if coff else
                 ['python3', '-B', '-c', LINK_CODE, phase['build_unit']])
        comparator = 'compare-coff-function.py' if coff else 'compare-linked-function.py'
        phases.append({'id': phase['id'], 'build_argv': build,
                       'compare_argv': [['python3', '-B', 'scripts/' + comparator,
                                         '--unit', name, '--json'] for name in phase['units']],
                       'artifact_kind': recipe['artifact_kind'],
                       'artifact_path': recipe['object'] if coff else recipe['output_dir']})
    return {'schema': 'radial-native-command-plan-v1', 'cwd': 'repository-root',
            'source_verification': verified, 'limits': manifest['native_limits'],
            'target_expected_sha256': manifest['target_sha256'],
            'phases': phases, 'executor_implemented': False, 'execution_authorized': False,
            'requires': ['separate-native-authorization', 'reviewed-durable-claim-executor',
                         'target-and-toolchain-preflight', 'source-tool-config-pre-post-pins',
                         'frozen-artifact-hashes-before-and-after-all-phase-comparisons',
                         'phase-timeouts-process-cleanup-and-stop-on-first-failure'],
            'fresh_private_binding_derivation_implemented': False}


def is_hash(value):
    return type(value) is str and HEX.fullmatch(value) is not None


def verify_receipt(receipt, manifest, verified):
    """Validate a saved summary's consistency, never authenticate an execution."""
    plan = native_plan(manifest, verified)
    require(set(receipt) == {'schema', 'plan_sha256', 'target_sha256', 'phases'})
    require(receipt['schema'] == 'radial-native-summary-v1')
    require(receipt['plan_sha256'] == digest(canonical(plan)))
    require(receipt['target_sha256'] == manifest['target_sha256'])
    phases = receipt['phases']
    require(type(phases) is list and len(phases) == len(manifest['recipes']))
    claims = set()
    for row, phase in zip(phases, manifest['recipes']):
        require(set(row) == {'id', 'claim_sha256', 'build_calls', 'exit_code', 'limits',
                             'artifacts_before', 'artifacts_after', 'comparisons'})
        require(row['id'] == phase['id'] and type(row['build_calls']) is int
                and row['build_calls'] == 1 and type(row['exit_code']) is int
                and row['exit_code'] == 0)
        require(is_hash(row['claim_sha256']) and row['claim_sha256'] not in claims)
        claims.add(row['claim_sha256'])
        require(exact(row['limits'], manifest['native_limits']))
        coff = phase['recipe']['artifact_kind'] == 'coff'
        artifacts = row['artifacts_before']
        require(set(artifacts) == ({'object'} if coff else {'image', 'map', 'pdb'}))
        require(all(is_hash(v) for v in artifacts.values()))
        require(exact(artifacts, row['artifacts_after']))
        comparisons = row['comparisons']
        require(type(comparisons) is list and len(comparisons) == len(phase['units']))
        for report, name in zip(comparisons, phase['units']):
            spec = manifest['units'].get(name, {'size': 803, 'compared_size': 803})
            expected = {'unit': name, 'result': 'exact',
                        'artifact_kind': 'coff' if coff else 'linked-pe',
                        'size': spec['size'], 'matched_bytes': spec['size'],
                        'compared_size': spec['compared_size'],
                        'matched_compared_bytes': spec['compared_size'],
                        'artifact_sha256': artifacts['object' if coff else 'image']}
            if not coff:
                expected.update(normalization_complete=True, decoded_bytes=spec['size'])
            require(exact(report, expected))
    return {'schema': 'radial-receipt-consistency-v1', 'receipt_consistent': True,
            'receipt_sha256': digest(canonical(receipt)), 'plan_sha256': digest(canonical(plan)),
            'recipe_count': 12, 'guard_count': 29, 'radial_compared_bytes': 803,
            'execution_authenticated': False, 'native_executed_by_verifier': False,
            'factory_acceptance': 'pending'}


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('mode', choices=('verify-source', 'native-plan', 'verify-receipt',
                                         'reproduce-native'))
    parser.add_argument('--repo', type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument('--receipt', type=Path)
    args = parser.parse_args(argv)
    if args.mode == 'reproduce-native':
        print(json.dumps({'ok': False, 'error': 'native-executor-not-implemented'}))
        return 2
    try:
        helpers = Path(__file__).resolve().parent
        manifest = load_manifest(helpers)
        verified = verify_source(args.repo, manifest, helpers)
        if args.mode == 'verify-source':
            require(args.receipt is None)
            result = verified
        elif args.mode == 'native-plan':
            require(args.receipt is None)
            result = native_plan(manifest, verified)
        else:
            require(args.receipt is not None)
            raw = read_file(args.receipt.parent, args.receipt.name, 262144)
            receipt = json.loads(raw, object_pairs_hook=unique_object)
            result = verify_receipt(receipt, manifest, verified)
        print(json.dumps(result, sort_keys=True, separators=(',', ':'), allow_nan=False))
        return 0
    except Exception:  # Opaque failure for malformed bounded input, including deep JSON.
        print(json.dumps({'ok': False, 'error': 'verification-failed'}))
        return 1


if __name__ == '__main__':
    sys.exit(main())
