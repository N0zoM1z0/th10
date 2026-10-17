#!/usr/bin/env python3
"""Portable local opcode 1/3 contract checks using maintained complete host types.
Only live owners/resources/messages are executed. Null fallback removal and the
pre-access grouped character gate are source-shape checks, not runtime or native
exactness claims. Existing fixture/catalog stay unchanged; output must be fresh.
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

LABELS = {
    1: ('GUI_MSG_SHOW_PLAYER_PORTRAIT', 'GUI_MSG_SHOW_ENEMY_PORTRAIT'),
    3: ('GUI_MSG_SHOW_TEXT_BOX', 'GUI_MSG_HIDE_PLAYER_PORTRAIT'),
}
EXPECTED = {
    1: '''case GUI_MSG_SHOW_PLAYER_PORTRAIT:
            if (g_GuiCharacter == 0 || g_GuiCharacter == 1)
            {
                {
                    AnmLoadedView *loaded = static_cast<AnmLoadedView *>(g_Player->resource);
                    message->vmIds[GUI_MESSAGE_PLAYER_PORTRAIT].value = loaded->CreateVmVariant0(0x1d, GUI_VM_RENDER_LAYER).value;
                }
            }
            break;''',
    3: '''case GUI_MSG_SHOW_TEXT_BOX:
            {
                {
                    AnmLoadedView *loaded = g_GuiView->frontAnm;
                    message->vmIds[GUI_MESSAGE_TEXT_BOX].value = loaded->CreateVmVariant0(0x5a, GUI_VM_RENDER_LAYER).value;
                }
            }
            break;''',
}
GATE = 'if (g_GuiCharacter == 0 || g_GuiCharacter == 1)'
STORE = {
    1: 'message->vmIds[GUI_MESSAGE_PLAYER_PORTRAIT].value = loaded->CreateVmVariant0(0x1d, GUI_VM_RENDER_LAYER).value;',
    3: 'message->vmIds[GUI_MESSAGE_TEXT_BOX].value = loaded->CreateVmVariant0(0x5a, GUI_VM_RENDER_LAYER).value;',
}


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def region(text, op):
    first, last = ('        case ' + name + ':' for name in LABELS[op])
    if text.count(first) != 1 or text.count(last) != 1:
        raise RuntimeError('opcode boundaries changed: ' + str(op))
    a = text.index(first)
    b = text.index(last, a)
    return a, b, text[a:b]


def structure(text):
    # Whitespace-only normalization: the maintained scopes/grouped gate are exact.
    # This also establishes one source publication and no pre-gate owner access.
    for op, expected in EXPECTED.items():
        if re.sub(r'\s+', '', region(text, op)[2]) != re.sub(r'\s+', '', expected):
            raise RuntimeError('opcode ' + str(op) + ' structural contract mismatch')


def replace_once(text, old, new):
    if text.count(old) != 1:
        raise RuntimeError('expected exactly one mutation anchor: ' + old)
    return text.replace(old, new)


def mutate(text, op, old, new):
    a, b, body = region(text, op)
    return text[:a] + replace_once(body, old, new) + text[b:]


def guard_controls(text):
    controls = [
        ('player-owner-guard', 1, GATE,
         'if (g_Player != NULL &&\n                (g_GuiCharacter == 0 || g_GuiCharacter == 1))'),
        ('player-resource-zero', 1, '= loaded->CreateVmVariant0(', '= loaded == NULL ? 0 : loaded->CreateVmVariant0('),
        ('gui-owner-guard', 3, 'case GUI_MSG_SHOW_TEXT_BOX:\n',
         'case GUI_MSG_SHOW_TEXT_BOX:\n            if (g_GuiView != NULL)\n'),
        ('gui-resource-zero', 3, '= loaded->CreateVmVariant0(', '= loaded == NULL ? 0 : loaded->CreateVmVariant0('),
    ]
    old_guarded = text
    results = []
    for name, op, old, new in controls:
        changed = mutate(text, op, old, new)
        try:
            structure(changed)
        except RuntimeError as error:
            if str(error) != 'opcode ' + str(op) + ' structural contract mismatch':
                raise
        else:
            raise RuntimeError('structural negative escaped: ' + name)
        results.append(dict(name=name, outcome='structurally rejected only'))
        old_guarded = mutate(old_guarded, op, old, new)
    return old_guarded, results


def intended_failure(result, check):
    return result.returncode == 1 and result.stdout == '' and result.stderr == 'CHECK ' + check + '\n'


def classifier():
    good = subprocess.CompletedProcess([], 1, '', 'CHECK declared\n')
    if not intended_failure(good, 'declared'):
        raise RuntimeError('classifier rejected exact declared CHECK')
    invalid = [(-11, '', 'CHECK declared\n'), (139, '', 'CHECK declared\n'),
               (0, '', 'CHECK declared\n'), (2, '', 'CHECK declared\n'),
               (1, 'extra', 'CHECK declared\n'), (1, '', 'CHECK other\n'),
               (1, '', 'compiler error'), (1, '', 'CHECK declared\nextra\n'),
               (1, '', 'CHECK declared'), (1, '', ''),
               (1, '', 'CHECK declared\nAddressSanitizer: error\n')]
    if any(intended_failure(subprocess.CompletedProcess([], *row), 'declared') for row in invalid):
        raise RuntimeError('classifier accepted a wrong, crashing, noisy or incomplete outcome')
    return dict(exact_accept=1, strict_rejections=len(invalid))


# Generated addition only. Existing creation seam logs this/script/layer, invokes
# the synthetic callback, then returns host.id; no production callee is executed.
CASES = r'''
// DIRECT-CREATION GENERATED CASES: all owners/resources/messages remain live.
// Static storage safely precedes unchanged Player raw-flag member constructors.
static Player directReplacementPlayer;
static void DirectCreation(int id){
 for(int op:{1,3})for(int c:{INT_MIN,-123,-1,0,1,2,123,INT_MAX})
 for(int stage:{INT_MIN,-1,0,6,7,8,INT_MAX})for(int index:{INT_MIN,-1,0,1,INT_MAX})
 {
  Fixture f;g_GuiCharacter=c;g_GuiStage=stage;f.message.messageIndex=index;
  f.message.unknown58=0x1357;f.message.flags=0x40;f.message.activeSide=1;f.message.alternatingLine=2;
  f.gui->message=&f.message;f.Program(static_cast<unsigned char>(op));host.id=id;auto profile=f.profile;
  Normal(f);const int slot=op==1?0:2;const bool selected=op==3||c==0||c==1;
  if(selected){
   Check(host.creations.size()==1&&host.order==std::vector<std::string>{"create"},"direct.one_create");
   Check(host.creations[0].resource==(op==1?&f.playerResource:&f.otherResource),"direct.resource");
   Check(host.creations[0].script==(op==1?0x1d:0x5a)&&host.creations[0].layer==15,"direct.script_layer");
   Check(f.message.vmIds[slot].value==id,"direct.returned_id");
  }else{
   Check(host.creations.empty()&&host.order.empty(),"direct.invalid_no_create");
   for(int i=0;i<6;++i)Check(f.message.vmIds[i].value==101+i,"direct.invalid_ids");
  }
  for(int i=0;i<6;++i)if(i!=slot)Check(f.message.vmIds[i].value==101+i,"direct.other_ids");
  Check(f.message.messageIndex==index&&f.message.unknown58==0x1357&&f.message.flags==0x40&&f.message.activeSide==1&&f.message.alternatingLine==2&&f.profile==profile&&f.gui->guiFlags==0&&f.gui->endingCounter==47,"direct.sentinels");
  for(auto&v:f.vms)Check(v.pendingInterrupt==-12&&v.flags35C==0&&Same(v.positionOffset,AnmFloat3View(-7,-8,-9)),"direct.vm_sentinels");
 }
}
static void DirectCallbacks(){
 for(int op:{1,3})for(int c:{INT_MIN,-123,-1,0,1,2,123,INT_MAX})for(int id:{0,-1,INT_MIN,INT_MAX,137}){
  if(op==1&&c!=0&&c!=1)continue;
  Fixture f;GuiMessageVmView replacementMessage(nullptr);std::unique_ptr<GuiView>replacementGui(new GuiView);
  for(int i=0;i<6;++i)replacementMessage.vmIds[i].value=901+i;
  replacementGui->frontAnm=&f.enemyResource;replacementGui->message=&replacementMessage;
  directReplacementPlayer.resource=&f.enemyResource;
  f.gui->message=&f.message;g_GuiCharacter=c;g_GuiStage=7;f.Program(static_cast<unsigned char>(op));
  const int slot=op==1?0:2;AnmLoadedView*selected=op==1?&f.playerResource:&f.otherResource;
  int callbacks=0;host.id=117;
  host.afterCreate=[&](){
   ++callbacks;Check(f.message.vmIds[slot].value==101+slot,"direct.callback_before_publication");
   g_Player=&directReplacementPlayer;g_GuiView=replacementGui.get();
   playerOwner.resource=&f.enemyResource;f.gui->frontAnm=&f.enemyResource;f.gui->message=&replacementMessage;
   g_GuiCharacter=2;g_GuiStage=INT_MIN;f.message.vmIds[slot].value=-31337;host.id=id;
  };
  Normal(f);
  Check(callbacks==1&&host.creations.size()==1&&host.order==std::vector<std::string>{"create"},"direct.callback_once");
  Check(host.creations[0].resource==selected&&host.creations[0].script==(op==1?0x1d:0x5a)&&host.creations[0].layer==15,"direct.callback_capture");
  Check(f.message.vmIds[slot].value==id,"direct.callback_original_message");
  for(int i=0;i<6;++i){
   Check(replacementMessage.vmIds[i].value==901+i,"direct.callback_replacement_unchanged");
   if(i!=slot)Check(f.message.vmIds[i].value==101+i,"direct.callback_other_ids");
  }
  host.afterCreate={};g_Player=&playerOwner;g_GuiView=f.gui.get();
 }
}
'''

# All outcomes are declared before execution. Compilation, signals, timeouts,
# sanitizer diagnostics and a different CHECK are failures of this campaign.
MUTATIONS = [
    ('player-script', 1, '0x1d, GUI_VM_RENDER_LAYER', '0x1e, GUI_VM_RENDER_LAYER', 'direct-negative', 'direct.script_layer'),
    ('box-script', 3, '0x5a, GUI_VM_RENDER_LAYER', '0x59, GUI_VM_RENDER_LAYER', 'direct-negative', 'direct.script_layer'),
    ('player-layer', 1, 'GUI_VM_RENDER_LAYER', '14', 'direct-negative', 'direct.script_layer'),
    ('box-layer', 3, 'GUI_VM_RENDER_LAYER', '14', 'direct-negative', 'direct.script_layer'),
    ('player-resource', 1, 'g_Player->resource', 'g_GuiView->frontAnm', 'direct-negative', 'direct.resource'),
    ('box-resource', 3, 'g_GuiView->frontAnm', 'static_cast<AnmLoadedView *>(g_Player->resource)', 'direct-negative', 'direct.resource'),
    ('player-character-zero', 1, GATE, 'if (g_GuiCharacter == 1)', 'direct-negative', 'direct.one_create'),
    ('player-character-one', 1, GATE, 'if (g_GuiCharacter == 0)', 'direct-negative', 'direct.one_create'),
    ('player-character-extra', 1, GATE, 'if (g_GuiCharacter == 0 || g_GuiCharacter == 1 || g_GuiCharacter == -1)', 'direct-negative', 'direct.invalid_no_create'),
    ('player-invalid-publication', 1, GATE, 'message->vmIds[0].value = 0;\n            ' + GATE, 'direct-negative', 'direct.invalid_ids'),
    ('player-stage-gate', 1, GATE, 'if (g_GuiStage == 6 && (g_GuiCharacter == 0 || g_GuiCharacter == 1))', 'direct-negative', 'direct.one_create'),
    ('player-index-gate', 1, GATE, 'if (message->messageIndex < 1 && (g_GuiCharacter == 0 || g_GuiCharacter == 1))', 'direct-negative', 'direct.one_create'),
    ('box-character-gate', 3, STORE[3], 'if (g_GuiCharacter == 0) ' + STORE[3], 'direct-negative', 'direct.one_create'),
    ('box-stage-gate', 3, STORE[3], 'if (g_GuiStage == 6) ' + STORE[3], 'direct-negative', 'direct.one_create'),
    ('box-index-gate', 3, STORE[3], 'if (message->messageIndex < 1) ' + STORE[3], 'direct-negative', 'direct.one_create'),
]
for _op, _name in [(1, 'player'), (3, 'box')]:
    _store = STORE[_op]
    MUTATIONS.extend([
        (_name + '-wrong-slot', _op, _store, _store.replace('GUI_MESSAGE_PLAYER_PORTRAIT' if _op == 1 else 'GUI_MESSAGE_TEXT_BOX', 'GUI_MESSAGE_ENEMY_NAME'), 'direct-negative', 'direct.returned_id'),
        (_name + '-duplicate-create', _op, _store, 'loaded->CreateVmVariant0(0, GUI_VM_RENDER_LAYER);\n                    ' + _store, 'direct-negative', 'direct.one_create'),
        (_name + '-zero-id', _op, _store, _store + '\n                    message->vmIds[' + ('0' if _op == 1 else '2') + '].value = 0;', 'direct-negative', 'direct.returned_id'),
        (_name + '-other-id', _op, _store, _store + '\n                    message->vmIds[5].value = 0;', 'direct-negative', 'direct.other_ids'),
        (_name + '-sentinel', _op, _store, _store + '\n                    message->unknown58 = 0;', 'direct-negative', 'direct.sentinels'),
        (_name + '-early-publication', _op, _store, 'message->vmIds[' + ('0' if _op == 1 else '2') + '].value = 0;\n                    ' + _store, 'direct-callbacks', 'direct.callback_before_publication'),
        (_name + '-owner-message-reload', _op, _store,
         'int createdId = loaded->CreateVmVariant0(' + ('0x1d' if _op == 1 else '0x5a') + ', GUI_VM_RENDER_LAYER).value;\n                    g_GuiView->message->vmIds[' + ('0' if _op == 1 else '2') + '].value = createdId;', 'direct-callbacks', 'direct.callback_original_message'),
        (_name + '-postcallback-character-gate', _op, _store,
         'int createdId = loaded->CreateVmVariant0(' + ('0x1d' if _op == 1 else '0x5a') + ', GUI_VM_RENDER_LAYER).value;\n                    if (g_GuiCharacter == 0 || g_GuiCharacter == 1) message->vmIds[' + ('0' if _op == 1 else '2') + '].value = createdId;', 'direct-callbacks', 'direct.callback_original_message'),
    ])
EXPECTED_MUTATIONS = 31


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument('--out', type=Path, required=True)
    parser.add_argument('--fallback-root', type=Path)
    parser.add_argument('--sanitize', action='store_true')
    parser.add_argument('--no-mutations', action='store_true')
    args = parser.parse_args()
    if os.path.lexists(args.out):
        parser.error('--out must be a fresh, nonexistent directory (including symlinks)')
    root, out = args.root.resolve(), args.out.resolve()
    if out.exists():
        parser.error('--out must be a fresh, nonexistent directory')
    if len(MUTATIONS) != EXPECTED_MUTATIONS or len({row[0] for row in MUTATIONS}) != EXPECTED_MUTATIONS:
        raise RuntimeError('direct mutation catalog must have 31 unique controls')
    classifier_results = classifier()
    source = root / 'src/Gui.cpp'
    source_text = source.read_text()
    structure(source_text)
    guarded_source, structural_controls = guard_controls(source_text)
    driver_path = root / 'scripts/test-gui-message-run.py'
    fixture = root / 'scripts/fixtures/gui-message-run.cpp'
    catalog = root / 'scripts/gui_message_run_mutations.py'
    input_before = {str(p.resolve()): sha(p) for p in (source, driver_path, fixture, catalog, Path(__file__))}
    sys.dont_write_bytecode = True
    sys.path.insert(0, str(root / 'scripts'))
    spec = importlib.util.spec_from_file_location('direct_run_driver', driver_path)
    driver = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(driver)
    out.mkdir(parents=True, exist_ok=False)
    baseline = out / 'positive'
    manifest = driver.prepare(root, args.fallback_root, baseline, False)
    for path, digest in manifest['source_inputs_sha256'].items():
        if path in input_before and input_before[path] != digest:
            raise RuntimeError('input changed during preparation: ' + path)
        input_before[path] = digest
    generated_fixture = out / 'direct-fixture.cpp'
    fixture_text = replace_once(fixture.read_text(), 'int main(int argc,char**argv){', CASES + '\nint main(int argc,char**argv){')
    fixture_text = replace_once(fixture_text, 'if(s=="replay")', 'if(s=="direct-zero")DirectCreation(0);else if(s=="direct-negative")DirectCreation(-1);else if(s=="direct-min")DirectCreation(INT_MIN);else if(s=="direct-max")DirectCreation(INT_MAX);else if(s=="direct-positive")DirectCreation(137);else if(s=="direct-callbacks")DirectCallbacks();else if(s=="replay")')
    generated_fixture.write_text(fixture_text)
    manifest.update(input_hashes_before=input_before, sanitize=args.sanitize,
                    fixture_generated_sha256=sha(generated_fixture), classifier_self_controls=classifier_results,
                    structural_controls=structural_controls, structure='pass; exact maintained grouped source blocks',
                    source_scope='Only opcodes 1 and 3; grouped opcode 1 gate before owner access',
                    positive_cases=2800, positive_cases_per_process=560, synthetic_callback_cases=50,
                    compile_timeout_seconds=120, run_timeout_seconds=15,
                    caveats=['Live objects/resources/messages only; no null member calls or invalid lifetimes.',
                             'Unsupported-character runtime checks observe no creation and unchanged IDs; no-access follows from source structure.',
                             'One source publication is structural; runtime checks final ID and before/after callback ordering.',
                             'Callbacks are synthetic seam witnesses, not actual callee behavior.',
                             'No native ABI, target execution, physical storage, runtime-bug or all-input exactness claim.',
                             'ASAN_OPTIONS=detect_leaks=0; no leak claim.'])
    (out / 'declared-mutations.json').write_text(json.dumps(MUTATIONS, indent=2) + '\n')
    environment = dict(os.environ, ASAN_OPTIONS='detect_leaks=0')

    def compile_case(folder):
        command = driver.compile_command(folder, generated_fixture, args.sanitize)
        result = subprocess.run(command, text=True, capture_output=True, timeout=120)
        (folder / 'compile.stdout').write_text(result.stdout)
        (folder / 'compile.stderr').write_text(result.stderr)
        if result.returncode:
            raise RuntimeError('compile failure is not mutation evidence: ' + str(folder))
        return command

    def run_case(folder, selection):
        result = subprocess.run([str(folder / 'run-test'), selection], text=True,
                                capture_output=True, timeout=15, env=environment)
        (folder / (selection + '.stdout')).write_text(result.stdout)
        (folder / (selection + '.stderr')).write_text(result.stderr)
        return result

    def copy_case(folder):
        folder.mkdir(parents=True, exist_ok=False)
        for path in baseline.iterdir():
            if path.suffix in ('.h', '.hpp', '.inc'):
                shutil.copy2(path, folder / path.name)

    def positives(folder):
        command = compile_case(folder)
        results = []
        for selection in ('direct-zero', 'direct-negative', 'direct-min', 'direct-max', 'direct-positive', 'direct-callbacks'):
            result = run_case(folder, selection)
            accepted = result.returncode == 0 and result.stdout == result.stderr == ''
            results.append(dict(selection=selection, returncode=result.returncode,
                                stdout=result.stdout, stderr=result.stderr, accepted=accepted))
            if not accepted:
                raise RuntimeError('live-object positive failure: ' + str(folder) + '/' + selection)
        return dict(compile_command=command, compile_returncode=0, runs=results)

    manifest['positive'] = positives(baseline)
    print('Direct positives pass: 2800 live-object cases and 50 synthetic live-owner callbacks.', flush=True)
    # Reconstruct precisely the four removed guards in extracted actual Run. This
    # old form must pass every live-object positive and is never a strict mutant.
    raw = (baseline / 'owner.inc').read_text()
    guarded_raw, _ = guard_controls(raw)
    old_folder = out / 'old-guarded-equivalence'
    copy_case(old_folder)
    (old_folder / 'owner.inc').write_text(guarded_raw)
    manifest['old_guarded_live_object_equivalence'] = positives(old_folder)
    manifest['old_guarded_classification'] = 'All live-object positives pass; structurally rejected only, not a runtime counterexample.'
    manifest['old_guarded_source_sha256'] = hashlib.sha256(guarded_source.encode()).hexdigest()
    print('Old guarded form also passes all live-object positives; structural rejection only.', flush=True)
    results = []
    if not args.no_mutations:
        for name, op, before, after, selection, expected in MUTATIONS:
            folder = out / 'mutations' / name
            copy_case(folder)
            (folder / 'owner.inc').write_text(mutate(raw, op, before, after))
            compile_case(folder)
            result = run_case(folder, selection)
            accepted = intended_failure(result, expected)
            results.append(dict(name=name, selection=selection, expected_check=expected,
                                compile_returncode=0, returncode=result.returncode,
                                stdout=result.stdout, stderr=result.stderr, accepted=accepted))
            (out / 'mutations.json').write_text(json.dumps(results, indent=2) + '\n')
            if not accepted:
                raise RuntimeError('wrong mutation outcome: ' + name)
    manifest['mutations'] = results
    manifest['input_hashes_after'] = {path: sha(Path(path)) for path in input_before}
    manifest['input_hashes_unchanged'] = manifest['input_hashes_after'] == input_before
    (out / 'manifest.json').write_text(json.dumps(manifest, indent=2) + '\n')
    if not manifest['input_hashes_unchanged']:
        raise RuntimeError('source/fixture/catalog/driver changed during test; rerun required')
    print('Direct creation verified: four structural guard controls; ' + str(len(results)) + ' strict mutants detected.')


if __name__ == '__main__':
    main()
