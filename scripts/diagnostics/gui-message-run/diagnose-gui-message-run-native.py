#!/usr/bin/env python3
"""One fixed parent-run updater-to-Run codegen diagnostic. Never execute its image."""
import argparse
import hashlib
import json
from pathlib import Path
import sys
import tomllib


def require(ok, message):
    if not ok:
        raise RuntimeError(message)


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, default=Path.cwd())
    parser.add_argument('--check-only', action='store_true')
    args = parser.parse_args()
    root = args.root.resolve()
    plan_path = Path(__file__).with_name('native-inputs.json')
    plan = json.loads(plan_path.read_text())
    require(plan['schema'] == 'gui-run-fixed-native-v2', 'wrong plan schema')
    require(plan['review_status'] in ('awaiting-independent-review', 'frozen-inputs-reviewed'), 'unknown review status')
    require(args.check_only or plan['review_status'] == 'frozen-inputs-reviewed', 'plan is not ready to build')
    for name, expected in plan['input_sha256'].items():
        path = root/name
        require(not Path(name).is_absolute() and path.resolve().is_relative_to(root), 'input escapes root: '+name)
        require(path.is_file() and sha(path) == expected, 'reviewed raw input hash changed: '+name)
    config = root/'config/match-units.toml'
    require(sha(config) == plan['canonical_manifest_sha256'], 'canonical manifest changed; explicit review required')
    unit = tomllib.loads(config.read_text())['units']['gui-score-add']
    require(unit == plan['canonical_unit'], 'canonical GUI producer changed')
    require(plan['source'] == unit['source'] == 'src/Gui.cpp', 'wrong primary source')
    names = [plan['source'], *plan['support_sources']]
    require(all(n in plan['input_sha256'] for n in names), 'source missing hash gate')
    files = [(root/n).resolve() for n in names]
    require(len(set(files)) == len(files) and all(p.is_relative_to(root) for p in files), 'duplicate or escaping source')
    source, supports = files[0], files[1:]
    sys.path.insert(0, str(root/'scripts'))
    import ltcg_link
    require(ltcg_link.ROOT.resolve() == root, 'wrong repository link helper')
    require(unit['profile'] == plan['profile'], 'compile profile changed')
    require(unit['link_profile'] == ltcg_link.LINK_PROFILE == plan['link_profile'], 'link profile changed')
    require(unit['harness'] == ltcg_link.HARNESS_KIND == 'unresolved-symbol-data-anchors-v1', 'harness changed')
    require(not unit.get('support_sources') and not unit.get('normal_support_sources'), 'canonical supports changed')
    text = source.read_text()
    require('int __stdcall GuiView::UpdateStageElements(GuiView *gui)' in text and
            'GuiMessageVmView::Run(message)' in text and
            'int __stdcall GuiMessageVmView::Run(GuiMessageVmView *message)' in text,
            'actual root/Run definition or edge changed')
    directory = root/plan['output_dir']
    require(not directory.is_symlink() and directory.resolve().parent == (root/'build').resolve(), 'unsafe output directory')
    require(not directory.exists() or not any(directory.iterdir()), 'output not empty; preserve prior artifacts')
    settings = {**plan, 'plan_sha256': sha(plan_path), 'driver_sha256': sha(Path(__file__)),
                'root': str(root), 'acceptance_authority': 'none'}
    if args.check_only:
        print(json.dumps({'result': 'verified-no-build', **settings}, indent=2))
        return
    require(sha(root/'resources/th10.exe') == plan['target_sha256'], 'target identity changed')
    directory.mkdir(parents=True, exist_ok=True)
    report_path = directory/'run-build.json'
    report = {'result': 'native-attempt-started', **settings}
    report_path.write_text(json.dumps(report, indent=2, default=str)+'\n')
    try:
        linker, environment = ltcg_link.tool_environment()
        result = ltcg_link.cold_link(source, directory, plan['entry_symbol'], linker, environment,
                    profile=list(plan['profile']), support_sources=supports, normal_support_sources=[])
        report['artifacts'] = result
        require(result['entry_identity']['symbol'] == plan['entry_symbol'], 'entry identity changed')
        require(result['support_sources'] == supports and not result['normal_support_sources'], 'support graph changed')
        bad = [s for s in result['unresolved_symbols'] if any(t in s for t in plan['required_real_symbol_fragments'])]
        require(not bad, 'required real owner/dependency became an anchor: '+repr(bad))
        retired = [s for s in result['unresolved_symbols'] if any(t in s for t in plan['retired_interface_fragments'])]
        require(not retired, 'obsolete interpreter interface anchored: '+repr(retired))
    except Exception as exc:
        # Failed compiler stdout is included in cold_link's exception. Preserve it,
        # along with its normal link-attempt logs and partial objects.
        report.update(result='failed-native-attempt', error_type=type(exc).__name__, error=str(exc))
        report_path.write_text(json.dumps(report, indent=2, default=str)+'\n')
        print(json.dumps(report, indent=2, default=str))
        raise
    report['result'] = 'linked-codegen-only-manual-review-pending'
    report_path.write_text(json.dumps(report, indent=2, default=str)+'\n')
    print(json.dumps(report, indent=2, default=str))
    # Complete owner/static-helper/table evidence needs independent manual review.
    # Natural Run inlining or disappearance is inconclusive; never force retention.


if __name__ == '__main__':
    main()
