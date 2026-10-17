#!/usr/bin/env python3
"""Opcode-19 structural checks and safe valid-owner Names mutation controls.
Uses the maintained interpreter fixture and host extraction/compile seams.
Never invokes a member through a null object; not native-equivalence evidence.
"""
import argparse
import importlib.util
import json
from pathlib import Path
import shutil
import subprocess
import sys


def region(text):
    start = text.index('        case GUI_MSG_ENEMY_NAME:')
    end = text.index('        case GUI_MSG_COMPLETE_STAGE:', start)
    return start, end, text[start:end]


def structure(text):
    import re
    body = region(text)[2]
    assert body.count('switch (g_GuiStage)') == 1
    assert not re.search(r'\bif\s*\(|\?|\bNULL\b|\bdefault\s*:', body)
    assert re.findall(r'case (\d+):', body) == list('1234567')
    assert body.count('g_EnemyManager->effectResources[2]') == 7
    assert re.findall(r'loaded->CreateVmVariant0\((0x[0-9a-f]+), GUI_VM_RENDER_LAYER\)', body) == [
        '0xd', '0x10', '0x14', '0x15', '0xe', '0x23', '0x1d']
    assert body.count('message->vmIds[GUI_MESSAGE_ENEMY_NAME].value = loaded->CreateVmVariant0(') == 7


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument('--fallback-root', type=Path, help='Explicit optional source-directory fallback, as in the maintained driver')
    parser.add_argument('--out', type=Path, required=True, help='Fresh output directory')
    args = parser.parse_args()
    root, out = args.root.resolve(), args.out.resolve()
    if out.exists() and any(out.iterdir()):
        parser.error('--out must be absent or empty')
    source = (root/'src/Gui.cpp').read_text()
    structure(source)
    guarded = source.replace('        case GUI_MSG_ENEMY_NAME:',
                             '        case GUI_MSG_ENEMY_NAME:\n            if (g_EnemyManager != NULL)', 1)
    try:
        structure(guarded)
    except AssertionError:
        pass
    else:
        raise RuntimeError('structural guard negative control escaped')
    sys.path.insert(0, str(root/'scripts'))
    spec = importlib.util.spec_from_file_location('maintained_gui_run', root/'scripts/test-gui-message-run.py')
    driver = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(driver)
    baseline = out/'baseline'
    provenance = driver.prepare(root, args.fallback_root, baseline, False)
    fixture = root/'scripts/fixtures/gui-message-run.cpp'
    text = (baseline/'owner.inc').read_text()
    start, end, body = region(text)
    mutations = [('script-'+value, 'CreateVmVariant0('+value+',', 'CreateVmVariant0(0x7f,', 'names.table')
                 for value in ['0xd', '0x10', '0x14', '0x15', '0xe', '0x23', '0x1d']]
    mutations += [
        ('layer', 'GUI_VM_RENDER_LAYER', '14', 'names.layer_one_create'),
        ('resource', 'g_EnemyManager->effectResources[2]', 'g_GuiView->stageUiAnm', 'names.table'),
        ('publication', 'vmIds[GUI_MESSAGE_ENEMY_NAME]', 'vmIds[GUI_MESSAGE_PLAYER_PORTRAIT]', 'names.table'),
        ('unsupported-id', 'switch (g_GuiStage)', 'message->vmIds[GUI_MESSAGE_ENEMY_NAME].value = 0;\n            switch (g_GuiStage)', 'names.stage_guard')]
    results = []
    for name, old, new, expected in [('baseline', '', '', '')] + mutations:
        destination = baseline if name == 'baseline' else out/name
        if name != 'baseline':
            destination.mkdir()
            for path in baseline.iterdir():
                if path.suffix in ('.h', '.hpp', '.inc'):
                    shutil.copy2(path, destination/path.name)
            assert old in body, name
            (destination/'owner.inc').write_text(text[:start] + body.replace(old, new) + text[end:])
        build = subprocess.run(driver.compile_command(destination, fixture, False), text=True, capture_output=True, timeout=180)
        (destination/'compile.stderr').write_text(build.stderr)
        if build.returncode:
            raise RuntimeError(name+': compile failure is not mutation evidence')
        run = subprocess.run([str(destination/'run-test'), 'names'], text=True, capture_output=True, timeout=30)
        accepted = (run.returncode == 0 and run.stdout == run.stderr == '') if name == 'baseline' else driver.intended_failure(run, expected)
        results.append(dict(name=name, accepted=accepted, expected=expected, returncode=run.returncode, stdout=run.stdout, stderr=run.stderr))
        (out/'results.json').write_text(json.dumps(dict(structure='pass', guard_negative_control='rejected', provenance=provenance, results=results), indent=2)+'\n')
        if not accepted:
            raise RuntimeError(name+': incorrect test outcome')
    print('PASS: opcode-19 structure, guard negative control, Names baseline and 11 strict mutants')


if __name__ == '__main__':
    main()
