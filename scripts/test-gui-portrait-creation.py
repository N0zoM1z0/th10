#!/usr/bin/env python3
"""Opcode 2 host contract: live resources only; no native ABI or null-callee claim.
Uses the maintained Run extraction/fixture and its explicit host seams. Output
must not exist. Mutation crashes, compiler errors and unrelated failures fail.
"""
from pathlib import Path
import argparse
import hashlib
import importlib.util
import json
import os
import re
import shutil
import subprocess
import sys

START = '        case GUI_MSG_SHOW_ENEMY_PORTRAIT:'
END = '        case GUI_MSG_SHOW_TEXT_BOX:'
EXPECTED = '''case GUI_MSG_SHOW_ENEMY_PORTRAIT:
if (g_GuiCharacter == 0 || g_GuiCharacter == 1) {
 AnmLoadedView *anm;
 int script = 0x1e;
 if (g_GuiStage == 6) {
  anm = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
  script = 0x20;
 } else if (g_GuiStage == 7 && message->messageIndex < 1) {
  anm = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
  script = 0x25;
 } else anm = static_cast<AnmLoadedView *>(g_Player->resource);
 {
  AnmLoadedView *loaded = anm;
  message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value = loaded->CreateVmVariant0(script, GUI_VM_RENDER_LAYER).value;
 }
}
break;'''


def region(text):
    if text.count(START) != 1 or text.count(END) != 1:
        raise RuntimeError('opcode 2 source boundaries changed')
    a = text.index(START)
    b = text.index(END, a)
    return a, b, text[a:b]


def structure(text):
    _, _, body = region(text)
    # Exact non-whitespace source contract: positive live-owner cases cannot
    # detect restoration of a null-resource skip-create/zero-ID alternative.
    if re.sub(r'\s+', '', body) != re.sub(r'\s+', '', EXPECTED):
        raise RuntimeError('opcode 2 structural contract mismatch')


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--fallback-root', type=Path)
    parser.add_argument('--sanitize', action='store_true')
    parser.add_argument('--no-mutations', action='store_true')
    args = parser.parse_args()
    root, out = args.root.resolve(), args.out.resolve()
    if out.exists():
        parser.error('--out must be a fresh, nonexistent directory')
    source = root / 'src/Gui.cpp'
    text = source.read_text()
    structure(text)
    a, b, body = region(text)
    guarded = body.replace('= loaded->CreateVmVariant0(', '= loaded == NULL ? 0 : loaded->CreateVmVariant0(')
    if guarded == body:
        raise RuntimeError('guard negative-control anchor missing')
    try:
        structure(text[:a] + guarded + text[b:])
    except RuntimeError as error:
        if str(error) != 'opcode 2 structural contract mismatch':
            raise
    else:
        raise RuntimeError('null guard negative control escaped rejection')
    sys.path.insert(0, str(root / 'scripts'))
    driver_path = root / 'scripts/test-gui-message-run.py'
    spec = importlib.util.spec_from_file_location('portrait_run_driver', driver_path)
    driver = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(driver)
    driver.classifier()
    fixture = root / 'scripts/fixtures/gui-message-run.cpp'
    baseline = out / 'positive'
    manifest = driver.prepare(root, args.fallback_root, baseline, False)
    manifest.update(driver_sha256=sha(driver_path), fixture_sha256=sha(fixture),
                    portrait_driver_sha256=sha(Path(__file__)), sanitize=args.sanitize,
                    structure='pass', original_guard_negative_control='rejected',
                    asan_options='detect_leaks=0; no leak claim')
    environment = dict(os.environ, ASAN_OPTIONS='detect_leaks=0')

    def build_and_run(folder):
        command = driver.compile_command(folder, fixture, args.sanitize)
        compiled = subprocess.run(command, text=True, capture_output=True, timeout=180)
        (folder / 'compile.stdout').write_text(compiled.stdout)
        (folder / 'compile.stderr').write_text(compiled.stderr)
        if compiled.returncode:
            raise RuntimeError('compile failure is not mutation evidence: ' + str(folder))
        result = subprocess.run([str(folder / 'run-test'), 'portraits'], text=True,
                                capture_output=True, timeout=30, env=environment)
        (folder / 'run.stdout').write_text(result.stdout)
        (folder / 'run.stderr').write_text(result.stderr)
        return result

    result = build_and_run(baseline)
    if result.returncode != 0 or result.stdout or result.stderr:
        raise RuntimeError('portrait positive outcome mismatch')
    manifest['positive'] = 'pass'
    raw = (baseline / 'owner.inc').read_text()
    a, b, body = region(raw)
    results = []
    if not args.no_mutations:
        if len(MUTATIONS) != 15 or len({row[0] for row in MUTATIONS}) != 15:
            raise RuntimeError('portrait mutation catalog must have 15 unique controls')
        for name, old, new, expected in MUTATIONS:
            folder = out / 'mutations' / name
            folder.mkdir(parents=True)
            for path in baseline.iterdir():
                if path.suffix in ('.h', '.hpp', '.inc'):
                    shutil.copy2(path, folder / path.name)
            expected_count = 2 if name == 'enemy-resource' else 1
            if body.count(old) != expected_count:
                raise RuntimeError('mutation anchor count changed: ' + name)
            (folder / 'owner.inc').write_text(raw[:a] + body.replace(old, new) + raw[b:])
            result = build_and_run(folder)
            accepted = driver.intended_failure(result, expected)
            results.append(dict(name=name, expected_check=expected,
                                returncode=result.returncode, stdout=result.stdout,
                                stderr=result.stderr, accepted=accepted))
            if not accepted:
                raise RuntimeError('incorrect mutation outcome: ' + name)
    manifest['mutations'] = results
    manifest['source_hashes_still_current'] = all(
        sha(Path(path)) == digest for path, digest in manifest['source_inputs_sha256'].items())
    if not manifest['source_hashes_still_current'] or sha(fixture) != manifest['fixture_sha256']:
        raise RuntimeError('source or fixture changed during test')
    (out / 'manifest.json').write_text(json.dumps(manifest, indent=2) + '\n')
    print('Portrait creation verified: structure, guard negative control, positive fixture; '
          + str(len(results)) + ' strict mutants detected.')


MUTATIONS = [('script-default','int script = 0x1e','int script = 0x7f','portrait.enemy_script_layer'),('script-stage6','script = 0x20','script = 0x7f','portrait.enemy_script_layer'),('script-stage7','script = 0x25','script = 0x7f','portrait.enemy_script_layer'),('index-boundary','message->messageIndex < 1','message->messageIndex < 0','portrait.enemy_resource'),('index-unsigned','message->messageIndex < 1','static_cast<unsigned>(message->messageIndex) < 1','portrait.enemy_resource'),('stage6','g_GuiStage == 6','g_GuiStage == 5','portrait.enemy_resource'),('enemy-resource','g_EnemyManager->effectResources[2]','g_GuiView->stageUiAnm','portrait.enemy_resource'),('player-resource','g_Player->resource','g_GuiView->stageUiAnm','portrait.enemy_resource'),('layer','GUI_VM_RENDER_LAYER','14','portrait.enemy_script_layer'),('publication','vmIds[GUI_MESSAGE_ENEMY_PORTRAIT]','vmIds[GUI_MESSAGE_PLAYER_PORTRAIT]','portrait.enemy_id'),('character-guard','g_GuiCharacter == 0 || g_GuiCharacter == 1','g_GuiCharacter <= 1','portrait.character_guard'),('invalid-id','            if (g_GuiCharacter','            message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value = 0;\n            if (g_GuiCharacter','portrait.character_guard'),('duplicate-create','AnmLoadedView *loaded = anm;','AnmLoadedView *loaded = anm; loaded->CreateVmVariant0(script, GUI_VM_RENDER_LAYER);','portrait.enemy_resource'),('other-id','AnmLoadedView *loaded = anm;','AnmLoadedView *loaded = anm; message->vmIds[0].value = 0;','portrait.other_ids'),('sentinel','AnmLoadedView *loaded = anm;','AnmLoadedView *loaded = anm; message->unknown58 = 0;','portrait.sentinels')]

if __name__ == "__main__":
    main()
