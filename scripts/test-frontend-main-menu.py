#!/usr/bin/env python3
"""Execute maintained title/menu bodies with explicit host VM/state roles.

Canonical headers and actual cursor, input, progress-query and sound-queue
bodies are consumed. Native layout assertions and timer prefill are adapted
for the host. This is finite source validation, not target/runtime/ABI proof.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import runpy
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
body = runpy.run_path(str(ROOT/'scripts/test-frontend-replay-save.py'))['body']


def run(args, output):
    output.mkdir(parents=True, exist_ok=True)
    hashes = {}; pending = ['FrontEnd.hpp', 'InputState.hpp', 'EnemySoundQueue.hpp']; seen = set()
    while pending:
        name = pending.pop()
        if name in seen: continue
        seen.add(name); path = ROOT/'src'/name; text = path.read_text()
        hashes['src/'+name] = hashlib.sha256(path.read_bytes()).hexdigest()
        pending.extend(re.findall(r'^\s*#\s*include\s*"([^"]+)"', text, re.M))
        text = re.sub(r'typedef\s+char\s+\w+\s*\[[\s\S]*?\]\s*;', '// HOST layout assertion omitted.', text)
        text = text.replace('AnmVmTimerView() { flags &= ~1u; }', 'AnmVmTimerView() : flags(0) {} // HOST defined prefill.')
        (output/name).write_text(text)
    path = ROOT/'src/FrontEnd.cpp'; text = path.read_text()
    hashes['src/FrontEnd.cpp'] = hashlib.sha256(path.read_bytes()).hexdigest()
    owners = '\n\n'.join(body(text, s) for s in [
        'int FrontEndHasAnyClear(', 'int __stdcall FrontEndUpdateMainMenu(',
        'int __stdcall FrontEndUpdateMainMenuReturn('])
    helpers = body(text, 'enum\n{\n    FRONT_END_FLAG_DEMO_TRANSITION') + ';\n'
    helpers += '\n\n'.join(body(text, s) for s in [
        'int __fastcall FrontEndCursorSetCurrent(', 'void FrontEndCursorView::Push()',
        'void FrontEndCursorView::Pop()', 'int FrontEndCursorView::Move(',
        'int FrontEndInputView::IsRepeated('])
    (output/'helpers.inc').write_text(helpers)
    path = ROOT/'src/EnemyEclDispatcher.cpp'; text = path.read_text()
    hashes['src/EnemyEclDispatcher.cpp'] = hashlib.sha256(path.read_bytes()).hexdigest()
    sound = body(text, 'struct EnemySoundCueMetadataView') + ';\nEnemySoundCueMetadataView g_EnemySoundCueMetadata[134];\n'
    queue = body(text, 'void EnemySoundQueueView::QueueSoundSample(')
    opening = queue.index('{')
    queue = queue[:opening+1] + '\n    HostObserveQueue(this, soundId, sample); // HOST entry observation.\n' + queue[opening+1:]
    (output/'sound.inc').write_text(sound + queue)
    variants = [('positive', owners, None)]
    mutations = [
        ('miss-last-clear', 'profile[0x1d88d] != 0', 'false', 'six-clear-flags'),
        ('early-open', 'stateTimer.current > 10', 'stateTimer.current >= 10', 'open-eleven'),
        ('early-close', 'stateTimer.current >= 20', 'stateTimer.current >= 19', 'close-twenty'),
        ('held-confirm', 'g_FrontEndInput.pressed & FRONT_END_INPUT_CONFIRM', 'g_FrontEndInput.current & FRONT_END_INPUT_CONFIRM', 'cancel-then-confirm'),
        ('quit-fallthrough', 'goto quit;', ';', 'quit-cancel-priority'),
        ('lost-music-sound', 'case 5:\n                reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundSample(', 'case 5:\n                if (false) reinterpret_cast<EnemySoundQueueView *>(g_MainSoundOwner)->QueueSoundSample(', 'music-double-sound'),
        ('stale-extra-difficulty', 'controller->savedDifficulty = g_ReplayDifficulty;', 'controller->savedDifficulty = 4;', 'extra-saved-difficulty'),
        ('wrong-intro-aux', 'EnemyMarkPendingInterrupt(&controller->titleAuxVmId.value);', 'EnemyMarkPendingInterrupt(&controller->difficultyAuxVmId.value);', 'intro-auxiliary'),
        ('late-intro-close', 'stateTimer.current >= 30', 'stateTimer.current > 30', 'intro-thirty'),
    ]
    if not args.no_mutations:
        for name, old, new, label in mutations:
            assert owners.count(old) == (2 if name == 'early-open' else 1), (name, owners.count(old))
            variants.append((name, owners.replace(old, new), label))
    for name, variant, expected in variants:
        (output/'owner.inc').write_text(variant)
        command = ['nice','-n','10','g++','-std=c++17','-O1','-g','-Wall','-Wextra',
                   '-Wno-unused-function','-Wno-class-memaccess','-Wno-implicit-fallthrough',
                   '-I',str(output),str(ROOT/'scripts/fixtures/frontend-main-menu.cpp'),'-o',str(output/'fixture')]
        if args.sanitize: command += ['-fsanitize=address,undefined','-fno-sanitize-recover=all','-fno-pie','-no-pie']
        built = subprocess.run(command, capture_output=True, text=True)
        if built.returncode: raise RuntimeError(name+'\n'+built.stdout+built.stderr)
        result = subprocess.run([str(output/'fixture')], capture_output=True, text=True,
                                env={**os.environ,'ASAN_OPTIONS':'detect_leaks=1'})
        want = (0,'','') if expected is None else (1,'','CHECK '+expected+'\n')
        if (result.returncode,result.stdout,result.stderr) != want:
            raise RuntimeError(f'{name}: {result.returncode}\n{result.stdout}{result.stderr}')
        print('PASS '+name, flush=True)
    (output/'manifest.json').write_text(json.dumps({'scope':__doc__,'inputs':hashes,
        'variants':[v[0] for v in variants],'sanitize':args.sanitize},indent=2)+'\n')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sanitize',action='store_true')
    parser.add_argument('--no-mutations',action='store_true')
    parser.add_argument('--out',type=Path)
    args = parser.parse_args()
    if args.out: run(args,args.out)
    else:
        root=ROOT/'build/host-tests'; root.mkdir(parents=True,exist_ok=True)
        with tempfile.TemporaryDirectory(prefix='frontend-main-menu-',dir=root) as temp: run(args,Path(temp))


if __name__ == '__main__': main()
