#!/usr/bin/env python3
"""One fixed /EHsc member-construction diagnostic. Parent-run only; never runs its image."""
import argparse
import hashlib
import json
from pathlib import Path
import sys
import tomllib


EXPECTED_INPUT_SHA256 = {
    "src/EnemyEclDispatcher.cpp": "9fae2594e13b7e399e7a7b27a3117495f4a54afd08907df9dcc668d5b00a1e30",
    "src/EclVm.cpp": "8f236b6d2bdfa73bbf8747b16e934eb62264d2d54a811ac5f5cec789a1457efb",
    "src/AnmManager.cpp": "26bf39d6a7446311f373fd972477207c8ef950d54c59c0b68f85f0ccc64669ab",
    "src/AnmVmCreate.cpp": "49f873ff06835a86826c4d2647b33fd60957c4cf476033f7b25dc7afa0c50b83",
    "src/EnemyLaser.cpp": "d3ba99945c0ef3776796cbff86cfb6f506a699b0d4cdb013becc90c6ff7d59dd",
    "src/GuiMessage.cpp": "c53ed5ef974a296ecd83e949a50ad48d9f1d7e12b621680849dd465965bde4c6",
    "src/Gui.hpp": "21c98062547c469a649adeb065408cc8c1402c84f91be6ad1b580ba87716bceb",
    "src/Enemy.hpp": "b2ef2f155861082af19c81b186f45c1bcf45496f887c96ca0d3dd5b47f8d5d4d",
    "src/EnemyLaser.hpp": "b37486fc9421707645588b8b239d8c24a10f5d71d4b05fd34f6e8aa228be3743",
    "src/BulletManager.hpp": "3eb4b14e48456c4cec49a851e7c4ede0b7c38ef71c40a36ccd5321da622701c1",
    "src/AnmManager.hpp": "961a01f8267f9974838aea4d4102a296548ced89dbc0415ebc471af8f5a549c9",
    "src/AnmVmId.hpp": "b700f281acff5d909d85d2bd9bbf456237eee89f8d3a1ce36df871a48172ef1d",
    "src/Player.hpp": "8969b24cfd14cfcfcdbaa1504968b0e9f56ea8b4972748de1e6bf65fada362ce",
    "src/Enemy.cpp": "6f1bea5905ae2f9871f8ae5a0703e3b7309d58689c402614a2f14db4e7a45ee5",
    "src/EnemyCallback.cpp": "8d96765576c94e631db6c76178afa4c556b9a8d40cadb5f108f9cd5297103e4d"
}
RAW_MEMBER_PACKET_SHA256 = {
    "src/Enemy.cpp": "cd65fc31e7915753b26c74b3f30291b86887e98acc16beb67f3a87f64bcca757",
    "src/Enemy.hpp": "b2ef2f155861082af19c81b186f45c1bcf45496f887c96ca0d3dd5b47f8d5d4d",
    "src/EnemyCallback.cpp": "062509a99d5274c47b899412d33b9b7d50a2aaa6e1e5df13c6cfd1baf3f368e4",
    "src/EnemyEclDispatcher.cpp": "9fae2594e13b7e399e7a7b27a3117495f4a54afd08907df9dcc668d5b00a1e30",
    "src/Gui.hpp": "21c98062547c469a649adeb065408cc8c1402c84f91be6ad1b580ba87716bceb",
    "src/GuiMessage.cpp": "c53ed5ef974a296ecd83e949a50ad48d9f1d7e12b621680849dd465965bde4c6"
}
EXPECTED_CONSTRUCTOR = "??0GuiMessageVmView@@QAE@PBE@Z"
EXPECTED_CALLER = "?EnemyReadDialog@@YAXPAUGuiView@@H@Z"

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
    diagnostic_profile = [*unit['profile'], '/EHsc']
    require(diagnostic_profile == [*profile, '/EHsc'], 'unexpected isolated profile delta')
    selected = [*unit['support_sources'], 'src/EnemyLaser.cpp', 'src/GuiMessage.cpp']
    require(len(set(selected)) == 5 and source not in selected, 'duplicate primary/support')
    files = [root / unit['source'], *(root / name for name in selected)]
    require(all(path.is_file() for path in files), 'missing real production source')
    input_hashes = {}
    for name, expected in EXPECTED_INPUT_SHA256.items():
        path = root / name
        require(path.is_file(), 'missing reviewed input: ' + name)
        observed = hashlib.sha256(path.read_bytes()).hexdigest()
        require(observed == expected, 'reviewed input hash changed: ' + name)
        input_hashes[name] = observed
    primary_text = (root / source).read_text()
    require('static void EnemyReadDialog(GuiView *gui, int index)' in primary_text,
            'typed natural static caller changed')
    for name in ('src/EnemyEclDispatcher.cpp', 'src/GuiMessage.cpp',
                 'src/Enemy.cpp', 'src/EnemyCallback.cpp', 'src/Enemy.hpp'):
        contents = (root / name).read_text()
        require('g_EnemyVisualState' not in contents, 'legacy GUI alias remains: ' + name)
        require('EnemyInitializeDialog' not in contents, 'free initializer remains: ' + name)
    directory = root / 'build/diagnostic-gui-message-member-ehsc'
    require(not directory.is_symlink(), 'output directory is a symlink')
    require(directory.resolve().parent == (root / 'build').resolve(), 'output escapes dedicated build directory')
    require(not directory.exists() or not any(directory.iterdir()), 'dedicated diagnostic output is not empty; preserve prior artifacts')
    header_names = ['Gui.hpp', 'Enemy.hpp', 'EnemyLaser.hpp', 'BulletManager.hpp',
                    'AnmManager.hpp', 'AnmVmId.hpp', 'Player.hpp']
    settings = {
        'header_sha256': {name: hashlib.sha256((root / 'src' / name).read_bytes()).hexdigest()
                          for name in header_names},
        'canonical_unit': 'enemy-mark-pending-interrupt',
        'source': unit['source'], 'entry_symbol': unit['entry_symbol'],
        'support_sources': selected, 'normal_support_sources': [],
        'canonical_profile': unit['profile'], 'profile_delta': ['/EHsc'],
        'profile': diagnostic_profile, 'link_profile': unit['link_profile'],
        'constructor_source_symbol': EXPECTED_CONSTRUCTOR,
        'caller_source_symbol': EXPECTED_CALLER,
        'reviewed_input_sha256': input_hashes,
        'raw_member_packet_sha256': RAW_MEMBER_PACKET_SHA256,
        'input_hash_policy': 'Applied CRLF bytes for touched C++ inputs; original bytes for untouched baseline inputs',
        'reviewed_member_patch_sha256': 'a5083ce9e128fe6ca46f6af865f56cf6b11df25be1f46e315db88eaffae3ca22',
        'non_graph_packet_sources': ['src/Enemy.cpp', 'src/EnemyCallback.cpp'],
        'manifest_sha256': hashlib.sha256((root / 'config/match-units.toml').read_bytes()).hexdigest(),
        'link_helper_sha256': hashlib.sha256((root / 'scripts/ltcg_link.py').read_bytes()).hexdigest(),
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
        profile=list(diagnostic_profile), support_sources=[root / name for name in selected],
        normal_support_sources=[])
    require(result['entry_identity']['symbol'] == unit['entry_symbol'], 'returned entry identity mismatch')
    require(result['support_sources'] == [root / name for name in selected], 'returned support order mismatch')
    require(not result['normal_support_sources'], 'returned unexpected normal support')
    report = {'result': 'linked-diagnostic-only', 'acceptance_authority': 'none',
              **settings, 'artifacts': result}
    (directory / 'member-build.json').write_text(json.dumps(report, indent=2, default=str) + '\n')
    print(json.dumps(report, indent=2, default=str))
    required_names = ('??0GuiMessageVmView@@', 'EnemyReadDialog', 'EnemyKillAll', 'EnemyCancelBulletRecord',
                      'AllocateVm@AnmRenderManagerView', 'InitializeVm@AnmLoadedView',
                      'AddVmVariant0@AnmRenderManagerView', 'FindVm@AnmRenderManagerView',
                      'CreateVmAtWorldVariant0@AnmLoadedView')
    bad = [symbol for symbol in result['unresolved_symbols']
           if any(name in symbol for name in required_names)]
    require(not bad, 'required real owner/dependency anchored: ' + repr(bad))
    forbidden = [symbol for symbol in result['unresolved_symbols']
                 if 'g_EnemyVisualState' in symbol or 'EnemyInitializeDialog' in symbol]
    require(not forbidden, 'retired alias/interface anchored: ' + repr(forbidden))
    # Complete map/PDB contributions and call flow still require independent review.



if __name__ == '__main__':
    main()
