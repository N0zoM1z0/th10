#!/usr/bin/env python3
"""One fixed cold native graph. Parent-run only; never executes its output image."""
import argparse
import hashlib
import json
from pathlib import Path
import sys
import tomllib


def require(ok, message):
    if not ok:
        raise RuntimeError(message)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, default=Path.cwd())
    parser.add_argument('--check-only', action='store_true')
    args = parser.parse_args()
    root = args.root.resolve()
    sys.path.insert(0, str(root / 'scripts'))
    import ltcg_link
    require(ltcg_link.ROOT.resolve() == root, 'ltcg_link belongs to a different repository')
    with (root / 'config/match-units.toml').open('rb') as stream:
        unit = tomllib.load(stream)['units']['enemy-mark-pending-interrupt']
    source = 'src/EnemyEclDispatcher.cpp'
    entry = '?DispatchEclInstruction@EnemyRuntimeView@@QAEHXZ'
    supports = ['src/EclVm.cpp', 'src/AnmManager.cpp', 'src/AnmVmCreate.cpp']
    profile = ['/TP', '/MT', '/O2', '/Gy', '/GF', '/Oi', '/DNDEBUG', '/Isrc', '/GL', '/GS']
    link_profile = ['/ltcg', '/nodefaultlib', '/subsystem:console', '/incremental:no',
                    '/fixed:no', '/base:0x00400000', '/debug', '/debugtype:cv',
                    '/opt:noref', '/opt:noicf', '/safeseh:no']
    require(unit['source'] == source, 'canonical primary source changed')
    require(unit['entry_symbol'] == entry, 'canonical entry changed')
    require(unit['support_sources'] == supports, 'canonical ordered support sources changed')
    require(unit['profile'] == profile, 'canonical compile profile changed')
    require(unit['link_profile'] == link_profile, 'canonical link profile changed')
    require(ltcg_link.LINK_PROFILE == unit['link_profile'], 'cold_link hardcoded link profile differs')
    require(unit['harness'] == ltcg_link.HARNESS_KIND == 'unresolved-symbol-data-anchors-v1', 'harness changed')
    require(unit['artifact_kind'] == 'linked-pe', 'canonical artifact kind changed')
    require(not unit.get('normal_support_sources'), 'unexpected normal support sources')
    selected = [*unit['support_sources'], 'src/EnemyLaser.cpp']
    require(len(set(selected)) == 4 and source not in selected, 'duplicate primary/support')
    files = [root / unit['source'], *(root / name for name in selected)]
    require(all(path.is_file() for path in files), 'missing real production source')
    directory = root / 'build/diagnostic-laser-owner-binding'
    require(not directory.is_symlink(), 'output directory is a symlink')
    require(directory.resolve().parent == (root / 'build').resolve(), 'output escapes dedicated build directory')
    require(not directory.exists() or not any(directory.iterdir()), 'dedicated diagnostic output is not empty; preserve prior artifacts')
    settings = {
        'canonical_unit': 'enemy-mark-pending-interrupt',
        'source': unit['source'], 'entry_symbol': unit['entry_symbol'],
        'support_sources': selected, 'normal_support_sources': [],
        'profile': unit['profile'], 'link_profile': unit['link_profile'],
        'harness': unit['harness'], 'output_dir': str(directory),
        'source_sha256': {str(path.relative_to(root)): hashlib.sha256(path.read_bytes()).hexdigest() for path in files},
    }
    if args.check_only:
        print(json.dumps({'result': 'settings-verified-no-build', **settings}, indent=2))
        return
    # Parent must perform the ordinary target/toolchain preflight first. This
    # redundant input check keeps this standalone diagnostic target-bound.
    target = root / 'resources/th10.exe'
    require(target.is_file() and hashlib.sha256(target.read_bytes()).hexdigest() ==
            '2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040',
            'pinned target identity mismatch')
    linker, environment = ltcg_link.tool_environment()
    result = ltcg_link.cold_link(
        root / unit['source'], directory, unit['entry_symbol'], linker, environment,
        profile=list(unit['profile']), support_sources=[root / name for name in selected],
        normal_support_sources=[])
    require(result['entry_identity']['symbol'] == unit['entry_symbol'], 'returned entry identity mismatch')
    require(result['support_sources'] == [root / name for name in selected], 'returned support order mismatch')
    report = {'result': 'linked-diagnostic-only', 'acceptance_authority': 'none',
              **settings, 'artifacts': result}
    (directory / 'binding-build.json').write_text(json.dumps(report, indent=2, default=str) + '\n')
    print(json.dumps(report, indent=2, default=str))


if __name__ == '__main__':
    main()
