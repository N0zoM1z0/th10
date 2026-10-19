#!/usr/bin/env python3
"""Fresh local-only actual constructor/factory/Run/destructor integration.
Complete canonical headers, three explicit timer constructor seams, live byte
records, actual linked-list lookup/deletion, and strict negative controls.
No native build, remote action, ABI/layout/runtime closure or exactness claim.
"""
from pathlib import Path
import argparse
import hashlib
import json
import os
import re
import subprocess
import sys

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
SOURCE_ROOTS = []
SEAMS = {
    'AnmManager.hpp': ('AnmVmTimerView() { flags &= ~1u; }', 'AnmVmTimerView() : flags(0) {}'),
    'Player.hpp': ('PlayerTimerView() { flags &= ~1u; }', 'PlayerTimerView() : flags(0) {}'),
    'Enemy.hpp': ('EnemyInterpolationTimerView() { flags &= ~1u; }', 'EnemyInterpolationTimerView() : flags(0) {}'),
}
MACROS = '''#define __stdcall
#define __fastcall
#define __cdecl
#define __thiscall
#define __declspec(x)
#define __forceinline inline
#define __int64 long long
'''
MARK = '_ZN20AnmRenderManagerView17MarkVmForDeletionE11AnmVmIdView'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def owner(text, signature):
    definitions = []
    for match in re.finditer(re.escape(signature), text):
        opening = text.find('{', match.end())
        if opening >= 0 and ';' not in text[match.end():opening]:
            definitions.append((match.start(), opening))
    if len(definitions) != 1:
        raise RuntimeError('expected unique complete definition: ' + signature)
    start, opening = definitions[0]
    token = re.compile(r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|[{}]')
    depth = 0
    for match in token.finditer(text, opening):
        if match.group() == '{':
            depth += 1
        elif match.group() == '}':
            depth -= 1
            if depth == 0:
                return text[start:match.end()]
    raise RuntimeError('unclosed owner: ' + signature)


def source_path(name):
    for directory in SOURCE_ROOTS:
        path = directory / name
        if path.exists():
            return path
    raise RuntimeError('missing canonical source: ' + name)


def strip_asserts(text):
    def remove(match):
        if 'GuiMessageScalarWidths' in match.group():
            return match.group()
        if re.search(r'\?\s*1\s*:\s*-1', match.group()):
            return '// HOST ONLY: target layout assertion omitted; no layout claim.'
        return match.group()
    return re.sub(r'typedef\s+char\s+\w+\s*\[[\s\S]*?\]\s*;', remove, text)


def prepare(out):
    out.mkdir(parents=True, exist_ok=True)
    inputs, owners = {}, {}
    pending = ['Gui.hpp', 'GuiScore.hpp', 'EnemySoundQueue.hpp', 'Main.hpp',
               'Chain.hpp', 'Player.hpp', 'Enemy.hpp', 'EnemyLaser.hpp',
               'BulletManager.hpp', 'GameStageDescriptor.hpp', 'ResultMenu.hpp', 'ReplayManager.hpp']
    copied, applied = set(), set()
    while pending:
        name = pending.pop()
        if name in copied:
            continue
        path = source_path(name)
        text = path.read_text()
        inputs[str(path)] = sha(path)
        pending.extend(re.findall(r'^\s*#include\s+"([^"]+)"', text, re.M))
        text = strip_asserts(text)
        if name in SEAMS:
            before, after = SEAMS[name]
            if text.count(before) != 1:
                raise RuntimeError('timer seam inventory changed: ' + name)
            text = text.replace(before, '// HOST LIFETIME SEAM: raw-integer construction unproved.\n    ' + after)
            applied.add(name)
        (out / name).write_text(text)
        copied.add(name)
    if applied != set(SEAMS):
        raise RuntimeError('not all three timer seams applied')
    (out / 'windows.h').write_text('''// HOST SDK-ONLY TYPE PLACEHOLDERS. No game declarations replaced.
#pragma once
#include <cstdint>
typedef void *HWND;
typedef void *HINSTANCE;
typedef void *HANDLE;
typedef unsigned int UINT;
typedef std::uintptr_t WPARAM;
typedef std::intptr_t LPARAM;
typedef std::intptr_t LRESULT;
union LARGE_INTEGER { struct { std::uint32_t LowPart; std::int32_t HighPart; }; std::int64_t QuadPart; };
struct CRITICAL_SECTION { void *DebugInfo; std::int32_t LockCount; std::int32_t RecursionCount; void *OwningThread; void *LockSemaphore; std::uintptr_t SpinCount; };
''')

    def extract(name, signature):
        path = source_path(name)
        inputs[str(path)] = sha(path)
        body = owner(path.read_text(), signature)
        owners[signature] = {'path': str(path), 'sha256': hashlib.sha256(body.encode()).hexdigest()}
        return body

    gui = source_path('Gui.cpp').read_text()
    run = extract('Gui.cpp', 'int __stdcall GuiMessageVmView::Run(GuiMessageVmView *message)')
    (out / 'owner.inc').write_text(strip_asserts(gui[:gui.index(run) + len(run)]) + '\n')
    construction = [
        extract('GuiMessage.cpp', 'void __stdcall EnemyKillAll(EnemyManagerView *manager)'),
        extract('GuiMessage.cpp', 'GuiMessageVmView::GuiMessageVmView(const unsigned char *record)'),
        extract('EnemyEclDispatcher.cpp', 'static void EnemyReadDialog(GuiView *gui, int index)'),
        extract('Gui.cpp', 'GuiMessageVmView::~GuiMessageVmView()'),
    ]
    (out / 'construction.inc').write_text('\n\n'.join(construction) + '\n')
    dependencies = []
    for name, signatures in [
        ('AnmManager.cpp', ['AnmVmView *AnmRenderManagerView::FindVm(AnmVmIdView id)',
                           'void __stdcall AnmRenderManagerView::SetVmPosition(',
                           'void AnmRenderManagerView::MarkVmForDeletion(AnmVmIdView id)',
                           'void AnmVmTimerView::Add(float value)']),
        ('AnmVmId.cpp', ['void AnmVmIdView::SetInterrupt(short interrupt)',
                        'void AnmVmIdView::SetSprite(int spriteIndex)',
                        'void AnmVmIdView::SetSpriteWithAnm(AnmLoadedView *loaded, int spriteIndex)']),
        ('FrontEnd.cpp', ['AnmVmIdView ResultMenuFindScript(AnmVmIdView *id, int scriptIndex)',
                         'void ResultMenuPendingThree(AnmVmIdView *id)',
                         'int __stdcall FrontEndPlayMusic(int slot, int trackIndex)']),
        ('Player.cpp', ['void PlayerSetShotVmInterrupt2(unsigned int *vmId)']),
        ('EnemyEclDispatcher.cpp', ['__declspec(noinline) void EnemyMarkPendingInterrupt(int *id)',
                                   'void EnemySoundQueueView::QueueSoundSample(int soundId, int sample)']),
        ('Main.cpp', ['void MainSupervisorView::SetNextGameMode(int mode)']),
    ]:
        dependencies.extend(extract(name, signature) for signature in signatures)
    metadata = extract('EnemyEclDispatcher.cpp', 'struct EnemySoundCueMetadataView')
    (out / 'shared-declarations.inc').write_text(metadata + ';\n' +
        'extern EnemySoundCueMetadataView g_EnemySoundCueMetadata[];\n' +
        'extern unsigned int g_FrontEndSupervisorFlags;\n' +
        'extern unsigned char *g_FrontEndProfileData;\n' +
        'void FrontEndQueueSoundCommand(const char *, int, int);\n')
    # Actual deletion implementation is unchanged and in another translation
    # unit so the GNU host linker wrapper observes all calls, then forwards.
    (out / 'dependencies.cpp').write_text(MACROS +
        '#include "AnmManager.hpp"\n#include "Main.hpp"\n#include "EnemySoundQueue.hpp"\n' +
        '#include "shared-declarations.inc"\n' + '\n\n'.join(dependencies) + '\n')
    return {
        'source_search_roots': [str(path) for path in SOURCE_ROOTS],
        'source_inputs_sha256': inputs,
        'actual_owners': owners,
        'canonical_headers': sorted(copied),
        'record_accessor_seams': [],
        'timer_constructor_seams': {name: {'original': before, 'host': after}
                                    for name, (before, after) in SEAMS.items()},
        'deletion_observation': {
            'linker_symbol': MARK,
            'mechanism': 'GNU ld --wrap, host only',
            'contract': 'Wrapper observes receiver/count/ID order/publication, changes the published manager to a different complete live canonical owner to test receiver caching, and forwards every call to unchanged maintained MarkVmForDeletion. It never substitutes deletion behavior.'},
        'explicit_host_seams': {
            'dependency_lifecycle': ['AnmVmView ctor/dtor', 'AnmLoadedView ctor', 'AnmRenderManagerView ctor/dtor',
                                     'AsciiManagerView ctor/dtor/GetSize', 'GuiView dtor', 'EnemyManagerView ctor/dtor',
                                     'BulletManagerView dtor'],
            'constructor_callbacks': ['AllocateVm', 'InitializeVm', 'AddVmVariant0'],
            'rejecting_unused_boundaries': ['EnemyCancelBulletRecord', 'CreateVmAtWorldVariant0',
                'EnemyEffectWaitNode UnknownVirtual0/1/2/3/4/6 and ApplyPatternCancel/ApplyClear',
                'CreateVmVariant0', 'SetSprite', 'DrawTextLeft', 'FrontEndQueueSoundCommand',
                'GameCompleteStageTransition', 'GameQueueScaledSoundCommand', 'StageCreateTransitionEffect',
                'th10::leaf::Fn00417010/Fn004175E0'],
            'observation': ['malloc-backed global ordinary new/delete count message allocation and free',
                            'GNU deletion wrapper, always forwarding to real maintained body; explicit live-manager publication mutation tests cached receiver'],
            'storage': ['complete canonical fixture-owned objects and globals initialized before consumption',
                        'canonical sound owner placement-constructed in aligned declared sound byte storage',
                        'four nontext IDs explicitly seeded after construction into live canonical VMs'],
        },
        'limitations': [
            'Local host semantic evidence only; no target ABI/layout, native runtime closure, binary identity or exactness claim.',
            'Three explicit timer flags(0) seams; unchanged raw-integer default-construction validity remains unproved.',
            'Complete canonical headers retained; target-layout assertions omitted but scalar-width assertion retained.',
            'Runtime width and little-endian checks; actual byte accessors over live valid bounded unsigned-char records.',
            'Two text IDs are constructor outputs; four other IDs are explicit setup for the all-six-ID deletion scenario.',
            'Actual full Run graph compiles, but only WAIT/END are executed here.',
            'Explicit driver delete/unpublication, not the complete production GUI updater.',
            'Actual sound queue and maintained helper implementations extracted from their canonical source units.',
            'LeakSanitizer excluded in managed ptrace environment; no LSan/general leak-freedom claim.',
        ],
    }


def intended_failure(result, check):
    return result.returncode == 1 and result.stdout == '' and result.stderr == 'CHECK ' + check + '\n'


def classifier():
    if not intended_failure(subprocess.CompletedProcess([], 1, '', 'CHECK x\n'), 'x'):
        raise RuntimeError('strict classifier positive self-test failed')
    for code, out, err in [(-11, '', 'CHECK x\n'), (139, '', 'CHECK x\n'), (0, '', 'CHECK x\n'),
                           (2, '', 'CHECK x\n'), (1, 'x', 'CHECK x\n'), (1, '', 'CHECK wrong\n'),
                           (1, '', 'compiler error'), (1, '', 'CHECK x\nextra\n')]:
        if intended_failure(subprocess.CompletedProcess([], code, out, err), 'x'):
            raise RuntimeError('strict classifier rejection self-test failed')


def save(path, result):
    (path.parent / (path.name + '.stdout')).write_text(result.stdout)
    (path.parent / (path.name + '.stderr')).write_text(result.stderr)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, default=ROOT, help='Maintained repository/proposal root; input files from root/src.')
    parser.add_argument('--fallback-root', type=Path, action='append', help='Optional explicit source-directory fallback; repeat for layered snapshots. No implicit fallback.')
    parser.add_argument('--fixture', type=Path, default=HERE / 'fixtures/gui-message-lifecycle.cpp')
    parser.add_argument('--out', type=Path, default=ROOT / 'lifecycle-tests/evidence')
    parser.add_argument('--sanitize', action='store_true')
    parser.add_argument('--no-mutations', action='store_true')
    args = parser.parse_args()
    classifier()
    SOURCE_ROOTS[:] = [args.root.resolve() / 'src'] + [path.resolve() for path in (args.fallback_root or [])]
    out, fixture = args.out.resolve(), args.fixture.resolve()
    manifest = prepare(out)
    binary = out / 'gui-message-lifecycle'
    command = ['flock', '/tmp/gui-message-host-tests.lock', 'nice', '-n', '10', 'g++',
               '-std=c++17', '-O1', '-g', '-Wall', '-Wextra', '-Wno-unused-function', '-Wno-class-memaccess',
               '-I', str(out), str(fixture), str(out / 'dependencies.cpp'), '-Wl,--wrap=' + MARK, '-o', str(binary)]
    if args.sanitize:
        command[6:6] = ['-fsanitize=address,undefined', '-fno-sanitize-recover=all', '-fno-pie', '-no-pie']
    environment = dict(os.environ)
    if args.sanitize:
        environment['ASAN_OPTIONS'] = 'detect_leaks=0'
    manifest.update(compile_command=command, fixture_sha256=sha(fixture), driver_sha256=sha(Path(__file__)),
                    sanitize=args.sanitize, sanitizer_environment={'ASAN_OPTIONS': environment.get('ASAN_OPTIONS', '')},
                    scalar_and_endian_contract='Width assertion retained; runtime 8-bit byte/16-bit ushort/32-bit int and little-endian checks.',
                    strict_classifier_self_test='passed', negative_controls=[])
    def checkpoint():
        (out / 'manifest.json').write_text(json.dumps(manifest, indent=2) + '\n')
    result = subprocess.run(command, text=True, capture_output=True, timeout=240)
    save(out / 'compile', result)
    manifest['compile_returncode'] = result.returncode
    checkpoint()
    if result.returncode:
        raise RuntimeError('compile failed, never accepted: ' + str(out / 'compile.stderr'))
    result = subprocess.run([str(binary)], text=True, capture_output=True, timeout=40, env=environment)
    save(out / 'run', result)
    manifest['run'] = dict(returncode=result.returncode, stdout=result.stdout, stderr=result.stderr,
                          accepted=result.returncode == 0 and result.stdout == 'actual constructor/run/destructor lifecycle passed\n' and not result.stderr)
    checkpoint()
    if not manifest['run']['accepted']:
        raise RuntimeError('runtime did not pass: ' + str(out / 'manifest.json'))
    if not args.no_mutations:
        mutation = out / 'negative-control-omit-sixth-delete'
        mutation.mkdir(exist_ok=True)
        source = (out / 'construction.inc').read_text()
        anchor = 'manager->MarkVmForDeletion(vmIds[GUI_MESSAGE_ENEMY_NAME]);'
        if source.count(anchor) != 1:
            raise RuntimeError('negative-control anchor changed')
        (mutation / 'construction.inc').write_text(source.replace(anchor, '/* HOST NEGATIVE CONTROL: omit sixth deletion. */'))
        mutant_command = list(command)
        include = mutant_command.index('-I')
        mutant_command[include:include] = ['-I', str(mutation)]
        mutant_command[-1] = str(mutation / 'gui-message-lifecycle-mutant')
        result = subprocess.run(mutant_command, text=True, capture_output=True, timeout=240)
        save(mutation / 'compile', result)
        record = dict(name='omit_sixth_destructor_delete', compile_command=mutant_command, compile_returncode=result.returncode, accepted=False)
        manifest['negative_controls'].append(record)
        checkpoint()
        if result.returncode:
            raise RuntimeError('negative-control compiler failure, never accepted')
        result = subprocess.run([mutant_command[-1]], text=True, capture_output=True, timeout=40, env=environment)
        save(mutation / 'run', result)
        check = 'destructor.six_calls_and_one_free'
        record.update(returncode=result.returncode, stdout=result.stdout, stderr=result.stderr,
                      assigned_check=check, accepted=intended_failure(result, check))
        checkpoint()
        if not record['accepted']:
            raise RuntimeError('negative-control wrong CHECK/signal/output; never accepted')
    manifest['source_hashes_still_current'] = all(sha(Path(path)) == digest for path, digest in manifest['source_inputs_sha256'].items())
    checkpoint()
    if not manifest['source_hashes_still_current']:
        raise RuntimeError('source changed during test; rerun required')
    print('PASS actual constructor/factory -> WAIT pending -> END -> real six-ID destructor; ' + ('0 strict controls executed (--no-mutations).' if args.no_mutations else '1 strict control accepted.'))
    return 0

if __name__ == '__main__':
    try:
        sys.exit(main())
    except (OSError, RuntimeError, subprocess.TimeoutExpired) as error:
        print('FAIL ' + str(error), file=sys.stderr)
        sys.exit(1)
