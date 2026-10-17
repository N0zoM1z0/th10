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
            if (g_GuiStage == 6)
            {
                switch (g_GuiCharacter)
                {
                case 0:
                {
                    AnmLoadedView *loaded = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                    message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value = loaded->CreateVmVariant0(0x20, GUI_VM_RENDER_LAYER).value;
                    break;
                }
                case 1:
                {
                    AnmLoadedView *loaded = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                    message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value = loaded->CreateVmVariant0(0x20, GUI_VM_RENDER_LAYER).value;
                    break;
                }
                }
            }
            else if (g_GuiStage == 7 && message->messageIndex < 1)
            {
                switch (g_GuiCharacter)
                {
                case 0:
                {
                    AnmLoadedView *loaded = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                    message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value = loaded->CreateVmVariant0(0x25, GUI_VM_RENDER_LAYER).value;
                    break;
                }
                case 1:
                {
                    AnmLoadedView *loaded = static_cast<AnmLoadedView *>(g_EnemyManager->effectResources[2]);
                    message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value = loaded->CreateVmVariant0(0x25, GUI_VM_RENDER_LAYER).value;
                    break;
                }
                }
            }
            else
            {
                switch (g_GuiCharacter)
                {
                case 0:
                {
                    AnmLoadedView *loaded = static_cast<AnmLoadedView *>(g_Player->resource);
                    message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value = loaded->CreateVmVariant0(0x1e, GUI_VM_RENDER_LAYER).value;
                    break;
                }
                case 1:
                {
                    AnmLoadedView *loaded = static_cast<AnmLoadedView *>(g_Player->resource);
                    message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value = loaded->CreateVmVariant0(0x1e, GUI_VM_RENDER_LAYER).value;
                    break;
                }
                }
            }
            break;

'''

COLLAPSED = '''        case GUI_MSG_SHOW_ENEMY_PORTRAIT:
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
    # Exact source-shape guard, not native CFG proof or a semantic oracle.
    # Equivalent collapsed source and null guards are structural rejections.
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
    try:
        structure(text[:a] + COLLAPSED + text[b:])
    except RuntimeError as error:
        if str(error) != 'opcode 2 structural contract mismatch':
            raise
    else:
        raise RuntimeError('collapsed source escaped structural rejection')
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
                    structure='pass', original_guard_negative_control='structurally rejected only',
                    collapsed_source_structure='rejected; not a runtime counterexample',
                    portrait_cases=768,
                    asan_options='detect_leaks=0; no leak claim')
    environment = dict(os.environ, ASAN_OPTIONS='detect_leaks=0')

    def build_and_run(folder, selected_fixture=fixture):
        command = driver.compile_command(folder, selected_fixture, args.sanitize)
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
    # Synthetic callback witnesses only: no claim that the actual callee mutates
    # these globals. Keep the checked-in 768-case fixture byte-identical, and
    # deterministically add six cases to a separate generated host fixture.
    fixture_text = fixture.read_text()
    callback_anchor = 'static void Portraits(){\n'
    if fixture_text.count(callback_anchor) != 1:
        raise RuntimeError('portrait callback fixture anchor changed')
    callback_fixture = out / 'callback-fixture.cpp'
    callback_fixture.write_text(fixture_text.replace(
        callback_anchor, callback_anchor + CALLBACK_WITNESSES))
    callback_folder = out / 'synthetic-callbacks'
    callback_folder.mkdir()
    for path in baseline.iterdir():
        if path.suffix in ('.h', '.hpp', '.inc'):
            shutil.copy2(path, callback_folder / path.name)
    callback_result = build_and_run(callback_folder, callback_fixture)
    if callback_result.returncode != 0 or callback_result.stdout or callback_result.stderr:
        raise RuntimeError('synthetic portrait callback positive outcome mismatch')
    manifest['synthetic_callbacks'] = dict(
        cases=6, positive='pass', compile_returncode=0,
        returncode=callback_result.returncode, stdout=callback_result.stdout,
        stderr=callback_result.stderr, fixture_sha256=sha(callback_fixture),
        caveat='Synthetic host callbacks; not evidence the actual callee mutates these globals.')
    raw = (baseline / 'owner.inc').read_text()
    a, b, body = region(raw)
    # The previously accepted collapsed form is semantically equivalent on the
    # live-owner fixture. Compile/run it positively, separate from strict mutants.
    collapsed_folder = out / 'collapsed-equivalence'
    collapsed_folder.mkdir()
    for path in baseline.iterdir():
        if path.suffix in ('.h', '.hpp', '.inc'):
            shutil.copy2(path, collapsed_folder / path.name)
    (collapsed_folder / 'owner.inc').write_text(raw[:a] + COLLAPSED + raw[b:])
    equivalent = build_and_run(collapsed_folder)
    if equivalent.returncode != 0 or equivalent.stdout or equivalent.stderr:
        raise RuntimeError('collapsed equivalence positive outcome mismatch')
    manifest['collapsed_source_runtime'] = 'positive fixture passes; not counted as mutant detection'
    results = []
    if not args.no_mutations:
        if len(MUTATIONS) != 15 or len({row[0] for row in MUTATIONS}) != 15:
            raise RuntimeError('portrait mutation catalog must have 15 unique controls')
        for name, old, new, expected, expected_count in MUTATIONS:
            folder = out / 'mutations' / name
            folder.mkdir(parents=True)
            for path in baseline.iterdir():
                if path.suffix in ('.h', '.hpp', '.inc'):
                    shutil.copy2(path, folder / path.name)
            if body.count(old) != expected_count:
                raise RuntimeError('mutation anchor count changed: ' + name)
            (folder / 'owner.inc').write_text(raw[:a] + body.replace(old, new) + raw[b:])
            result = build_and_run(folder)
            accepted = driver.intended_failure(result, expected)
            results.append(dict(name=name, expected_check=expected,
                                compile_returncode=0, returncode=result.returncode, stdout=result.stdout,
                                stderr=result.stderr, accepted=accepted))
            if not accepted:
                raise RuntimeError('incorrect mutation outcome: ' + name)
    manifest['mutations'] = results
    manifest['source_hashes_still_current'] = all(
        sha(Path(path)) == digest for path, digest in manifest['source_inputs_sha256'].items())
    if not manifest['source_hashes_still_current'] or sha(fixture) != manifest['fixture_sha256']:
        raise RuntimeError('source or fixture changed during test')
    (out / 'manifest.json').write_text(json.dumps(manifest, indent=2) + '\n')
    print('Portrait creation verified: structure, guard negative control, 768 cases, six synthetic callbacks; '
          + str(len(results)) + ' strict mutants detected.')


# Two accepted characters crossed with the three outer resource/script branches.
CALLBACK_WITNESSES = ' for(int c:{0,1})for(int branch:{0,1,2}){\n  Fixture f;g_GuiCharacter=c;g_GuiStage=branch==0?6:7;f.message.messageIndex=branch==2?1:0;f.Program(2);\n  AnmLoadedView *selected=branch==2?&f.playerResource:&f.enemyResource;\n  host.afterCreate=[&](){g_GuiCharacter=-1;g_GuiStage=8;f.enemy.effectResources[2]=&f.otherResource;g_Player->resource=&f.otherResource;f.message.vmIds[1].value=-9;host.id=137;};\n  Normal(f);\n  Check(host.creations.size()==1&&host.creations[0].resource==selected&&host.creations[0].script==(branch==0?0x20:branch==1?0x25:0x1e)&&host.creations[0].layer==15,"portrait.callback_selected_call");\n  Check(f.message.vmIds[1].value==137&&host.order==std::vector<std::string>{"create"},"portrait.callback_publication");\n }\n'

MUTATIONS = [('script-default', 'CreateVmVariant0(0x1e', 'CreateVmVariant0(0x7f', 'portrait.enemy_script_layer', 2), ('script-stage6', 'CreateVmVariant0(0x20', 'CreateVmVariant0(0x7f', 'portrait.enemy_script_layer', 2), ('script-stage7', 'CreateVmVariant0(0x25', 'CreateVmVariant0(0x7f', 'portrait.enemy_script_layer', 2), ('index-boundary', 'message->messageIndex < 1', 'message->messageIndex < 0', 'portrait.enemy_resource', 1), ('index-unsigned', 'message->messageIndex < 1', 'static_cast<unsigned>(message->messageIndex) < 1', 'portrait.enemy_resource', 1), ('stage6', 'g_GuiStage == 6', 'g_GuiStage == 5', 'portrait.enemy_resource', 1), ('enemy-resource', 'g_EnemyManager->effectResources[2]', 'g_GuiView->stageUiAnm', 'portrait.enemy_resource', 4), ('player-resource', 'g_Player->resource', 'g_GuiView->stageUiAnm', 'portrait.enemy_resource', 2), ('layer', 'GUI_VM_RENDER_LAYER', '14', 'portrait.enemy_script_layer', 6), ('publication', 'vmIds[GUI_MESSAGE_ENEMY_PORTRAIT]', 'vmIds[GUI_MESSAGE_PLAYER_PORTRAIT]', 'portrait.enemy_id', 6), ('character-guard', 'case 0:', 'case -1: case 0:', 'portrait.character_guard', 3), ('invalid-id', '            if (g_GuiStage', '            message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value = 0;\n            if (g_GuiStage', 'portrait.character_guard', 1), ('duplicate-create', '                    message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value = loaded->CreateVmVariant0(', '                    loaded->CreateVmVariant0(0, GUI_VM_RENDER_LAYER);\n                    message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value = loaded->CreateVmVariant0(', 'portrait.enemy_resource', 6), ('other-id', '                    message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value = loaded->CreateVmVariant0(', '                    message->vmIds[0].value = 0;\n                    message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value = loaded->CreateVmVariant0(', 'portrait.other_ids', 6), ('sentinel', '                    message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value = loaded->CreateVmVariant0(', '                    message->unknown58 = 0;\n                    message->vmIds[GUI_MESSAGE_ENEMY_PORTRAIT].value = loaded->CreateVmVariant0(', 'portrait.sentinels', 6)]

if __name__ == "__main__":
    main()
