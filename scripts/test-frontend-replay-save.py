#!/usr/bin/env python3
"""Actual replay-save, replay factory/FPS and cursor/input helpers with host roles.

Canonical FrontEnd/Replay/Input headers are copied with native-layout assertions
omitted and timer prefill defined. Only owner time_t casts use a host int32 seam.
VMs, replay I/O/lifecycle, state transitions, time/localtime and ASCII formatting
are callback-sensitive roles. The actual sound queue has an entry observation. No target/runtime, ABI, layout or exactness claim.
"""
import argparse, hashlib, json, os, re, subprocess, tempfile
from pathlib import Path
ROOT = Path(__file__).resolve().parents[1]


def body(text, signature):
    starts = list(re.finditer(re.escape(signature), text))
    if len(starts) != 1: raise ValueError('nonunique definition: ' + signature)
    start = starts[0].start(); opening = text.index('{', start); depth = 0
    for i in range(opening, len(text)):
        depth += (text[i] == '{') - (text[i] == '}')
        if depth == 0: return text[start:i + 1]
    raise ValueError('unclosed definition')


def run(args, output):
    output.mkdir(parents=True, exist_ok=True)
    hashes = {}; pending = ['FrontEnd.hpp', 'ReplayManager.hpp', 'InputState.hpp', 'EnemySoundQueue.hpp']; seen = set()
    while pending:
        name = pending.pop()
        if name in seen: continue
        seen.add(name); p = ROOT/'src'/name; text = p.read_text()
        hashes['src/' + name] = hashlib.sha256(p.read_bytes()).hexdigest()
        pending.extend(re.findall(r'^\s*#\s*include\s*"([^"]+)"', text, re.M))
        text = re.sub(r'typedef\s+char\s+\w+\s*\[[\s\S]*?\]\s*;', '// HOST layout assertion omitted.', text)
        if name == 'AnmManager.hpp':
            old = 'AnmVmTimerView() { flags &= ~1u; }'; assert text.count(old) == 1
            text = text.replace(old, 'AnmVmTimerView() : flags(0) {} // HOST defined prefill.')
        (output/name).write_text(text)
    p = ROOT/'src/FrontEnd.cpp'; text = p.read_text(); hashes['src/FrontEnd.cpp'] = hashlib.sha256(p.read_bytes()).hexdigest()
    owners = '\n\n'.join(body(text, sig) for sig in [
        'int __stdcall FrontEndControllerView::UpdateReplaySave(',
        'int __stdcall FrontEndControllerView::DrawReplaySave('])
    # The native timestamp is int32; Linux time_t is wider. Explicit host seam.
    owners = owners.replace('reinterpret_cast<time_t *>', 'reinterpret_cast<std::int32_t *>')
    helpers = '\n\n'.join(body(text, sig) for sig in [
        'int __fastcall FrontEndCursorSetCurrent(', 'void FrontEndCursorView::Push()',
        'void FrontEndCursorView::Pop()', 'int FrontEndCursorView::Move(',
        'int FrontEndInputView::IsRepeated(', 'static int InputRepeated('])
    p = ROOT/'src/ReplayManager.cpp'; replay = p.read_text(); hashes['src/ReplayManager.cpp'] = hashlib.sha256(p.read_bytes()).hexdigest()
    (output/'replay-delete.inc').write_text(body(replay, 'void ReplayManager::Destroy('))
    owners += '\n\n' + body(replay, 'ReplayManager *__stdcall ReplayManager::Load(') + '\n\n' + body(replay, 'int ReplayManager::DrawFps()')
    p = ROOT/'src/EnemyEclDispatcher.cpp'; enemy = p.read_text(); hashes['src/EnemyEclDispatcher.cpp'] = hashlib.sha256(p.read_bytes()).hexdigest()
    sound = body(enemy, 'struct EnemySoundCueMetadataView') + ';\nEnemySoundCueMetadataView g_EnemySoundCueMetadata[134];\n'
    queue = body(enemy, 'void EnemySoundQueueView::QueueSoundSample(')
    opening = queue.index('{'); queue = queue[:opening+1] + '\n    HostObserveQueue(this, soundId, sample); // HOST entry observation.\n' + queue[opening+1:]
    (output/'sound.inc').write_text(sound + queue)
    constants = body(text, 'enum\n{\n    FRONT_END_FLAG_DEMO_TRANSITION') + ';'
    (output/'helpers.inc').write_text(constants + '\n' + helpers)
    variants = [('positive', owners, None)]
    mutations = [
        ('wrong-load-mode', 'manager->mode = REPLAY_MANAGER_LOAD_ONLY;', 'manager->mode = REPLAY_MANAGER_RECORD;', 'factory-load-mode'),
        ('wrong-fps-threshold', 'fps < 30.0f', 'fps < 31.0f', 'fps-color'),
        ('wrong-fps-mode', 'case REPLAY_MANAGER_PLAYBACK:', 'case REPLAY_MANAGER_LOAD_ONLY:', 'fps-gate'),
        ('stale-fps-ascii', 'g_AsciiManagerView->AddFormatText(&position, "%3d", replayFps);\n            g_AsciiManagerView->color = 0xffffffffu;', 'AsciiManagerView *oldAscii = g_AsciiManagerView;\n            oldAscii->AddFormatText(&position, "%3d", replayFps);\n            oldAscii->color = 0xffffffffu;', 'fps-current-ascii'),
        ('wrong-sound-owner', 'g_MainSoundOwner', 'g_AlternateSoundOwner', 'queue-owner'),
        ('wrong-sound-sample', 'FRONT_END_SOUND_UNLOCK, 0', 'FRONT_END_SOUND_UNLOCK, 1', 'queue-sample'),
        ('early-opening', 'stateTimer.current > 6', 'stateTimer.current >= 6', 'opening-seven'),
        ('stale-header', 'recorded->replayData->completionState = 8;', 'staleHeader->completionState = 8;', 'time-live-header'),
        ('rebound-time-owner', 'recorded->replayData->completionState = 8;', 'g_ReplayManager->replayData->completionState = 8;', 'time-stable-owner'),
        ('late-load-slot', 'controller->replayFiles[slot] = ReplayManager::Load(replayPath);', 'ReplayManager *loaded = ReplayManager::Load(replayPath);\n                controller->replayFiles[controller->cursor.current] = loaded;', 'load-sampled-slot'),
        ('short-clear', 'memset(controller->replayFiles, 0, sizeof(controller->replayFiles));', 'memset(controller->replayFiles, 0, 25 * sizeof(controller->replayFiles[0]));', 'close-clear-fifty'),
        ('empty-backspace', 'if (controller->scoreEntryNameCursor == 0)\n                    break;', 'if (controller->scoreEntryNameCursor == 0)\n                    controller->SetScreenStateTarget(FRONT_END_REPLAY_SAVE_SELECT_SLOT);\n                else', 'empty-backspace-exit'),
        ('stale-draw-timer', 'if (controller->stateTimer.current >= 10)', 'if (false)', 'draw-live-timer'),
        ('wrong-preview-stage', 'g_ReplayCompletionNames[8]', 'g_ReplayCompletionNames[data->completionState]', 'preview-completion'),
    ]
    if not args.no_mutations:
        for name, old, new, label in mutations:
            assert owners.count(old) == (7 if name == 'wrong-sound-owner' else 1), (name, owners.count(old))
            variant = owners
            if name == 'stale-header':
                old_decl = 'ReplayManager *recorded = g_ReplayManager;'
                assert variant.count(old_decl) == 2
                variant = variant.replace(old_decl, old_decl + '\n            ReplayDataHeader *staleHeader = recorded->replayData;', 1)
            variants.append((name, variant.replace(old, new), label))
    for name, variant, expected in variants:
        (output/'owner.inc').write_text(variant)
        command = ['nice','-n','10','g++','-std=c++17','-O1','-g','-Wall','-Wextra','-Wno-unused-function','-Wno-class-memaccess','-I',str(output),str(ROOT/'scripts/fixtures/frontend-replay-save.cpp'),'-o',str(output/'fixture')]
        if args.sanitize: command += ['-fsanitize=address,undefined','-fno-sanitize-recover=all','-fno-pie','-no-pie']
        built = subprocess.run(command, capture_output=True, text=True)
        if built.returncode: raise RuntimeError(name + '\n' + built.stdout + built.stderr)
        result = subprocess.run([str(output/'fixture')], capture_output=True, text=True, env={**os.environ,'ASAN_OPTIONS':'detect_leaks=1'})
        want = (0,'','') if expected is None else (1,'','CHECK ' + expected + '\n')
        if (result.returncode,result.stdout,result.stderr) != want:
            raise RuntimeError(f'{name}: {result.returncode}\n{result.stdout}{result.stderr}')
        print('PASS ' + name, flush=True)
    (output/'manifest.json').write_text(json.dumps({'scope':__doc__,'inputs':hashes,'variants':[x[0] for x in variants],'sanitize':args.sanitize},indent=2)+'\n')
    print(f'Replay-save actual source and {len(variants)-1} strict mutants pass')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sanitize',action='store_true'); parser.add_argument('--no-mutations',action='store_true'); parser.add_argument('--out',type=Path)
    args = parser.parse_args()
    if args.out: run(args, args.out)
    else:
        root = ROOT/'build/host-tests'; root.mkdir(parents=True,exist_ok=True)
        with tempfile.TemporaryDirectory(prefix='frontend-replay-save-',dir=root) as temp: run(args,Path(temp))

if __name__ == '__main__': main()
