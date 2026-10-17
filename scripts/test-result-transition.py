#!/usr/bin/env python3
"""Actual-body host contracts for the result-transition source hypothesis.

Extracts proposal bodies and actual unchanged dependencies on each run. This is
not native ABI/storage, layout, x87, rendering, or target-machine equivalence.
Run --mutations for isolated, exactly-once, assertion-identified mutation checks.
See docs/RESULT_TRANSITION_CONTRACTS.md for the dependency seams and test domain.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import tempfile


def lexical_mask(text):
    pattern = r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\''
    return re.sub(pattern, lambda m: ''.join('\n' if c == '\n' else ' ' for c in m[0]), text)


def extract(path, name, struct=False):
    text = path.read_text()
    mask = lexical_mask(text)
    if struct:
        pat = re.compile(r'^\s*struct\s+' + re.escape(name) + r'\s*\{', re.M)
    else:
        pat = re.compile(r'^\s*(?:static\s+)?(?:int|void)\s+(?:(?:__stdcall|__fastcall)\s+)?'
                         + re.escape(name) + r'\s*\([^;{}]*\)\s*\{', re.M)
    found = []
    for match in pat.finditer(mask):
        opening = mask.index('{', match.start(), match.end())
        depth, end = 1, opening + 1
        while end < len(mask) and depth:
            depth += (mask[end] == '{') - (mask[end] == '}')
            end += 1
        if depth:
            raise RuntimeError(f'Unclosed definition: {name} in {path}')
        found.append(text[match.start():end] + (';' if struct else ''))
    if len(found) != 1:
        raise RuntimeError(f'Expected exactly one definition: {name} in {path}; got {len(found)}')
    return found[0]


# Every mutation is applied to exactly one occurrence in one extracted body.
# No mutation creates a null dereference, out-of-bounds access, zero division,
# nonfinite conversion, or dependence on an unrelated object lifetime.
MUTATIONS = [
 ('mode-nonzero', 'GameCompleteStageTransition', 'game->initializationMode == 1', 'game->initializationMode != 0'),
 ('wrong-config-owner', 'GameCompleteStageTransition', 'g_GameConfigurationFlags & 0x1000', 'g_GameRuntimeFlags & 0x1000'),
 ('config-mask', 'GameCompleteStageTransition', 'g_GameConfigurationFlags & 0x1000', 'g_GameConfigurationFlags & 0x0800'),
 ('early-mode-value', 'GameCompleteStageTransition', '? 2 : 4', '? 4 : 2'),
 ('result-state', 'GameCompleteStageTransition', 'menu->state = 6;', 'menu->state = 7;'),
 ('timer-value', 'GameCompleteStageTransition', 'menu->timer.SetCurrent(0);', 'menu->timer.SetCurrent(1);'),
 ('flags-preservation', 'GameCompleteStageTransition', 'game->flags58 |= 0x10;', 'game->flags58 = 0x10;'),
 ('capture-script', 'GameCompleteStageTransition', 'resource->InitializeVm(vm, 0);', 'resource->InitializeVm(vm, 1);'),
 ('front-script', 'GameCompleteStageTransition', 'resource->InitializeVm(vm, 129);', 'resource->InitializeVm(vm, 128);'),
 ('capture-rectangle', 'GameCompleteStageTransition', 'menu->secondaryVm.value, 32, 16, 384, 448', 'menu->secondaryVm.value, 33, 16, 384, 448'),
 ('capture-receiver', 'GameCompleteStageTransition', 'manager->RequestCaptureRectangle(', 'g_AnmRenderManagerView->RequestCaptureRectangle('),
 ('manager-reload', 'GameCompleteStageTransition', 'manager = g_AnmRenderManagerView;\n    menu->secondaryVm', 'manager = manager;\n    menu->secondaryVm'),
 ('second-allocation-manager', 'GameCompleteStageTransition', '    vm = manager->AllocateVm();', '    vm = g_AnmRenderManagerView->AllocateVm();'),
 ('music-path', 'GameCompleteStageTransition', 'bgm/th10_17.wav', 'bgm/th10_16.wav'),
 ('supervisor-bit', 'GameCompleteStageTransition', 'g_FrontEndSupervisorFlags & 0x10', 'g_FrontEndSupervisorFlags & 0x20'),
 ('play-opcode', 'GameCompleteStageTransition', '"dummy", 2, 0', '"dummy", 3, 0'),
 ('profile-index', 'GameCompleteStageTransition', '[0x1d8a3] = 1', '[0x1d8a2] = 1'),
 ('completed-value', 'GameCompleteStageTransition', 'menu->completed = 1;', 'menu->completed = 2;'),
 ('unrelated-field', 'GameCompleteStageTransition', 'menu->completed = 1;', 'menu->completed = 1; menu->unknown000 = 0;'),
 ('saved-speed', 'GameCompleteStageTransition', 'menu->savedSpeed = g_AnmGameSpeed;', 'menu->savedSpeed = 1.0f;'),
 ('speed-reset', 'GameCompleteStageTransition', 'g_AnmGameSpeed = 1.0f;', 'g_AnmGameSpeed = 0.5f;'),
 ('scaled-positive-only', 'GameQueueScaledSoundCommand', 'g_AnmGameSpeed != 0.0f', 'g_AnmGameSpeed > 0.0f'),
 ('scaled-threshold', 'GameQueueScaledSoundCommand', 'g_AnmGameSpeed <= 1.0f', 'g_AnmGameSpeed <= 0.5f'),
 ('scaled-rounding', 'GameQueueScaledSoundCommand', 'double value = magnitude;', 'float value = magnitude;'),
 ('scaled-opcode', 'GameQueueScaledSoundCommand', '"", 5, static_cast<int>(value)', '"", 6, static_cast<int>(value)'),
 ('scaled-path', 'GameQueueScaledSoundCommand', '"", 5, static_cast<int>(value)', '"dummy", 5, static_cast<int>(value)'),
 ('scaled-result', 'GameQueueScaledSoundCommand', 'return 0;', 'return 1;'),
 ('capture-global-lookup', 'AnmRenderManagerView::RequestCaptureRectangle', 'g_AnmRenderManagerView->FindVm(id)', 'FindVm(id)'),
 ('capture-busy', 'AnmRenderManagerView::RequestCaptureRectangle', 'captureSurfaceIndex >= 0', 'captureSurfaceIndex > 0'),
 ('queue-capacity', 'SoundPlayerView::QueueCommand', 'index >= 31', 'index >= 32'),
 ('queue-step', 'SoundPlayerView::QueueCommand', 'command->step = 0;', 'command->step = 1;'),
 ('gui-early-condition', 'CompleteStageFromMessage', 'g_GuiGameFlags & 0x10', 'g_GuiGameFlags & 0x20'),
]

# Explicit, predeclared failure IDs. A different check or additional diagnostic
# does not count as a kill, even when execution returns the contract exit code.
EXPECTED = {
 'mode-nonzero':'normal-dependency-order-and-reloads',
 'wrong-config-owner':'early-config-flag-selects-mode',
 'config-mask':'early-config-flag-selects-mode',
 'early-mode-value':'early-config-flag-selects-mode',
 'result-state':'state-timer-flags-before-allocation',
 'timer-value':'state-timer-flags-before-allocation',
 'flags-preservation':'state-timer-flags-before-allocation',
 'capture-script':'main-id-before-music-load',
 'front-script':'normal-dependency-order-and-reloads',
 'capture-rectangle':'capture-receiver-and-destination',
 'capture-receiver':'capture-receiver-and-destination',
 'manager-reload':'main-id-before-music-load',
 'second-allocation-manager':'normal-dependency-order-and-reloads',
 'music-path':'normal-dependency-order-and-reloads',
 'supervisor-bit':'normal-dependency-order-and-reloads',
 'play-opcode':'normal-dependency-order-and-reloads',
 'profile-index':'profile-reload-and-exact-index',
 'completed-value':'result-completed-one',
 'unrelated-field':'unlisted-menu-bytes-unchanged',
 'saved-speed':'save-latest-speed-after-queue',
 'speed-reset':'game-speed-reset-one',
 'scaled-positive-only':'scaled-sound-value-opcode-empty-path',
 'scaled-threshold':'scaled-sound-value-opcode-empty-path',
 'scaled-rounding':'scaled-sound-value-opcode-empty-path',
 'scaled-opcode':'scaled-sound-value-opcode-empty-path',
 'scaled-path':'scaled-sound-value-opcode-empty-path',
 'scaled-result':'scaled-sound-return-zero',
 'capture-global-lookup':'normal-dependency-order-and-reloads',
 'capture-busy':'busy-capture-return',
 'queue-capacity':'reserved-queue-slot31-unchanged',
 'queue-step':'load-command',
 'gui-early-condition':'result-state-six',
}


def is_assertion_kill(build_code, run_code, stdout, stderr, expected):
    return (build_code == 0 and run_code == 86 and stdout == ''
            and stderr == 'CONTRACT_FAILURE: ' + expected + '\n')


def check_classifier():
    marker = 'CONTRACT_FAILURE: expected\n'
    good = (0,86,'',marker,'expected')
    if not is_assertion_kill(*good): raise RuntimeError('Classifier rejects its positive control')
    bad = [
        (1,86,'',marker,'expected'),  # compiler failure is never a kill
        (0,-6,'',marker,'expected'), (0,-11,'',marker,'expected'),
        (0,0,'',marker,'expected'), (0,1,'',marker,'expected'),
        (0,86,'unexpected\n',marker,'expected'),
        (0,86,'',marker+'extra\n','expected'),
        (0,86,'','extra\n'+marker,'expected'),
        (0,86,'',marker+marker,'expected'),
        (0,86,'','CONTRACT_FAILURE: wrong\n','expected'),
        (0,86,'',marker.rstrip(),'expected'), (0,86,'','','expected'),
    ]
    for case in bad:
        if is_assertion_kill(*case): raise RuntimeError('Classifier accepted a negative control: '+repr(case))
    print(f'PASS: strict classifier positive control and {len(bad)} rejected negative controls',flush=True)
    return len(bad)



def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--root', type=Path, default=Path(__file__).resolve().parents[1])
    p.add_argument('--baseline-root', type=Path, default=None)
    p.add_argument('--fixture', type=Path, default=Path(__file__).resolve().parent/'fixtures/result-transition.cpp')
    p.add_argument('--cxx', default=os.environ.get('CXX', 'c++'))
    p.add_argument('--mutations', action='store_true')
    p.add_argument('--manifest', type=Path, help='Write final provenance and results JSON')
    p.add_argument('--emit', type=Path, help='Write assembled baseline harness for inspection')
    args = p.parse_args()
    negatives = check_classifier()
    compiler = subprocess.run([args.cxx,'--version'],capture_output=True,text=True,check=True).stdout.splitlines()[0]
    print('COMPILER: '+compiler,flush=True)
    results = []
    inputs = set()
    def source(name):
        path = args.root/'src'/name
        if not path.exists() and args.baseline_root is not None: path = args.baseline_root/'src'/name
        inputs.add(path)
        return path
    structs = [
        ('AnmManager.hpp', 'AnmVmTimerView'), ('AnmVmId.hpp', 'AnmVmIdView'),
        ('FrontEnd.hpp', 'FrontEndCursorView'), ('ResultMenu.hpp', 'ResultMenuView'),
        ('FrontEnd.cpp', 'ResultGameChainView'), ('FrontEnd.cpp', 'ResultGameManagerView'),
        ('Sound.cpp', 'SoundPlayerCommand'), ('Sound.cpp', 'SoundPlayerView')]
    structure_parts = []
    for file,name in structs:
        body = extract(source(file),name,True)
        if name == 'AnmVmTimerView':
            # Actual constructor is retained. All its fixture instances have
            # static zero-initialization; silence only the generic declaration
            # warning, not runtime findings or other compilation diagnostics.
            body = ('#pragma GCC diagnostic push\n'
                    '#pragma GCC diagnostic ignored "-Wuninitialized"\n'
                    + body + '\n#pragma GCC diagnostic pop\n')
        structure_parts.append(body)
    structures = '\n'.join(structure_parts)
    names = [
        ('FrontEnd.cpp', 'AnmRenderManagerView::RequestCaptureRectangle'),
        ('FrontEnd.cpp', 'FrontEndLoadMusic'), ('FrontEnd.cpp', 'FrontEndPlayMusic'),
        ('FrontEnd.cpp', 'GameQueueScaledSoundCommand'),
        ('FrontEnd.cpp', 'GameCompleteStageTransition'),
        ('Sound.cpp', 'SoundPlayerView::QueueCommand'),
        ('Gui.cpp', 'CompleteStageFromMessage')]
    bodies = {name:extract(source(file),name) for file,name in names}
    gui = source('Gui.cpp').read_text()
    if len(re.findall(r'extern\s+GameChainOwnerView\s*\*\s*g_GameOwner477830\s*;',gui)) != 1:
        raise RuntimeError('Canonical g_GameOwner477830 declaration is absent or changed')
    fixture = args.fixture.read_text()
    def assemble(parts):
        return fixture.replace('// @ACTUAL_STRUCTURES@',structures).replace('// @ACTUAL_BODIES@','\n'.join(parts.values()))
    base = assemble(bodies)
    if args.emit: args.emit.write_text(base)
    with tempfile.TemporaryDirectory(prefix='th10-result-transition-') as tmp:
        directory = Path(tmp)
        def run(label, code, mutant=False):
            cpp,binary = directory/(label+'.cpp'),directory/label
            cpp.write_text(code)
            build = subprocess.run([args.cxx,'-std=c++17','-Wall','-Wextra','-Werror','-O0',str(cpp),'-o',str(binary)],capture_output=True,text=True)
            if build.returncode:
                raise RuntimeError(f'COMPILE_ERROR (not a mutation kill): {label}\n{build.stdout}{build.stderr}')
            result = subprocess.run([str(binary)],capture_output=True,text=True,timeout=15)
            if mutant:
                expected = EXPECTED[label]
                if not is_assertion_kill(build.returncode,result.returncode,result.stdout,result.stderr,expected):
                    raise RuntimeError(f'MUTATION_NOT_KILLED_BY_CONTRACT: {label}; exit={result.returncode}\n{result.stdout}{result.stderr}')
                results.append({'mutation':label,'expected_assertion':expected,'exit_code':result.returncode,'stdout':result.stdout,'stderr':result.stderr})
                print(f'KILLED {label}: {expected}',flush=True)
            elif result.returncode or result.stderr or result.stdout != 'PASS: actual-body result-transition/scaled-sound host contracts; native ABI/storage/x87 remain unresolved\n':
                raise RuntimeError(f'BASELINE_FAILED: exit={result.returncode}\n{result.stdout}{result.stderr}')
            else:
                print(result.stdout.strip(),flush=True)
        run('baseline',base)
        if args.mutations:
            for label,owner,old,new in MUTATIONS:
                if bodies[owner].count(old) != 1:
                    raise RuntimeError(f'MUTATION_REPLACEMENT_ERROR: {label}: expected 1, got {bodies[owner].count(old)}')
                changed = dict(bodies)
                changed[owner] = bodies[owner].replace(old,new,1)
                run(label,assemble(changed),True)
            print(f'PASS: baseline plus {len(MUTATIONS)} assertion-identified safe mutations')
    if args.manifest:
        tracked = inputs | {Path(__file__).resolve(),args.fixture}
        args.manifest.write_text(json.dumps({
            'status':'pass','compiler':compiler,'compiler_options':['-std=c++17','-Wall','-Wextra','-Werror','-O0'],
            'baseline':'pass','classifier_negative_controls':negatives,'mutation_count':len(results),
            'mutation_results':results,'native_abi_storage_x87_proof':False,
            'inputs':[{'path':str(path),'sha256':hashlib.sha256(path.read_bytes()).hexdigest()} for path in sorted(tracked)],
        },indent=2)+'\n')
    print('EXTRACTED INPUTS:')
    for path in sorted(inputs):
        print(hashlib.sha256(path.read_bytes()).hexdigest(),path)


if __name__ == '__main__':
    main()
