#!/usr/bin/env python3
"""Local post-construction semantic tests of source-extracted GUI owners.

No remote calls, production edits/builds, target execution, i386 layout claim,
raw-byte comparison, or exactness credit. Complete maintained headers are copied
into a temporary directory. Target-layout assertions are removed, and EXACTLY
THREE explicitly enumerated timer constructors are replaced with flags(0) host
lifetime seams. All fields and actual SetCurrent/Add/Tick methods are retained.
Complete helper, member-constructor, and typed caller bodies are extracted unchanged.
The actual member constructor is exercised through ordinary new, including
synchronous callback throws, automatic allocation cleanup and publication order.
External dependency implementations are explicit host seams in the fixture.

Maintained raw-integer default-construction model validity is UNPROVED and is excluded from this
runtime evidence. A real unchanged-constructor GCC -O1 probe failed despite raw
storage prefill. No -fno-lifetime-dse, opaque/volatile workaround, or production
source edit is used here. Source ordering is checked separately.

Mutation success means compilation succeeded and the executable returned exactly
1, with empty stdout and exactly the assigned CHECK line on stderr. Compiler
errors, signals, crashes, timeouts, other CHECKs, and null-pointer outcomes never
count as a detected mutation. No mutation creates a null lookup/allocation.
"""
from __future__ import annotations
import argparse
from dataclasses import dataclass
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile

# Exhaustive constructor seams for types instantiated by this fixture. Other
# header constructors listed below are neither changed nor run: the fixture
# never constructs PlayerOptionRuntime, PlayerConstructorFlagView, or PlayerView.
CONSTRUCTOR_SEAMS = {
    'AnmManager.hpp': ('AnmVmTimerView() { flags &= ~1u; }',
                       'AnmVmTimerView() : flags(0) {}'),
    'Player.hpp': ('PlayerTimerView() { flags &= ~1u; }',
                   'PlayerTimerView() : flags(0) {}'),
    'Enemy.hpp': ('EnemyInterpolationTimerView() { flags &= ~1u; }',
                  'EnemyInterpolationTimerView() : flags(0) {}'),
}

UNEXECUTED_CONSTRUCTORS = {
    'Player.hpp': ('PlayerOptionRuntime() { flags80 &= ~1u; }',
                   'PlayerConstructorFlagView() { flags &= ~1u; }'),
}

OWNERS = (
    'void __stdcall EnemyKillAll(EnemyManagerView *manager)',
    'GuiMessageVmView::GuiMessageVmView(const unsigned char *record)',
)


def extract_owner(text: str, signature: str) -> str:
    if text.count(signature) != 1:
        raise RuntimeError(f'expected one complete owner signature: {signature}')
    start = text.index(signature)
    opening = text.index('{', start)
    # Ignore braces in comments and strings without modifying the returned body.
    token = re.compile(r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|[{}]')
    depth = 0
    for match in token.finditer(text, opening):
        value = match.group()
        if value == '{':
            depth += 1
        elif value == '}':
            depth -= 1
            if depth == 0:
                return text[start:match.end()]
    raise RuntimeError(f'unclosed owner: {signature}')


def host_headers(source: Path, output: Path, fallback: Path) -> int:
    pending = ['Gui.hpp', 'Enemy.hpp', 'EnemyLaser.hpp', 'BulletManager.hpp']
    visited: set[str] = set()
    removed = 0
    applied_seams: set[str] = set()
    while pending:
        name = pending.pop()
        if name in visited:
            continue
        visited.add(name)
        selected = source / name
        if not selected.exists():
            selected = fallback / name
        text = selected.read_text()
        pending.extend(re.findall(r'^\s*#include\s+"([^"]+)"', text, re.M))
        def remove_assert(match: re.Match[str]) -> str:
            nonlocal removed
            # Do not suppress ordinary declarations or any source data member.
            if not re.search(r'\?\s*1\s*:\s*-1', match.group()):
                return match.group()
            removed += 1
            return '// HOST ONLY: target layout assertion omitted; no layout claim.'
        text = re.sub(r'typedef\s+char\s+\w+\s*\[[\s\S]*?\]\s*;', remove_assert, text)
        for unexecuted in UNEXECUTED_CONSTRUCTORS.get(name, ()):
            if text.count(unexecuted) != 1:
                raise RuntimeError(f'{name}: unexecuted constructor inventory changed')
        if name in CONSTRUCTOR_SEAMS:
            before, after = CONSTRUCTOR_SEAMS[name]
            if text.count(before) != 1:
                raise RuntimeError(f'{name}: expected exactly one reviewed constructor seam; source changed')
            text = text.replace(before, '// HOST LIFETIME SEAM: maintained raw-integer default construction is unproved.\n    ' + after)
            applied_seams.add(name)
        (output / name).write_text(text)
    if applied_seams != set(CONSTRUCTOR_SEAMS):
        raise RuntimeError('not all reviewed host constructor seams were applied')
    return removed


def extract_case(text: str, label: str) -> str:
    anchor = '  case ' + label + ':'
    if text.count(anchor) != 1:
        raise RuntimeError(f'expected one ECL snippet: {label}')
    start = text.index(anchor) + len(anchor)
    end = re.search(r'^  case ', text[start:], re.M)
    if end is None:
        raise RuntimeError(f'unbounded ECL snippet: {label}')
    return text[start:start + end.start()]


def gui_field_snippets(dispatcher: str, callback: str) -> str:
    # These are BOUNDED SNIPPETS, not complete-owner runtime coverage. Existing
    # 32-bit integer pointer arithmetic cannot run on the 64-bit host. Exactly
    # the listed runtime scalar accesses are mapped to actual header fields.
    marker = extract_case(dispatcher, 'ENEMY_ECL_SET_LIFE_MARKER')
    clear = extract_case(dispatcher, 'ENEMY_ECL_SET_LIFE')
    wait = extract_case(dispatcher, 'ENEMY_ECL_WAIT_DIALOG')
    segment = extract_case(dispatcher, 'ENEMY_ECL_SET_SPELL_TIMEOUT')
    mappings = {
        '*(int *)((int)runtimeAddress + 0x13c4)': 'enemy->unknown13C4',
        '*(int *)((int)runtimeAddress + 0x13c0)': 'enemy->life',
        '*(unsigned int *)((int)runtimeAddress + 0x13c8) = *(unsigned int *)((int)runtimeAddress + 0x13c0)': 'enemy->value13C8 = enemy->life',
    }
    if marker.count(next(iter(mappings))) != 1:
        raise RuntimeError('marker runtime field mapping changed')
    for before, after in mappings.items():
        expected = 1
        if clear.count(before) != expected:
            raise RuntimeError(f'clear runtime field mapping changed: {before}')
        clear = clear.replace(before, after)
        marker = marker.replace(before, after)
    if wait.count('    break;') != 1:
        raise RuntimeError('wait snippet control-flow boundary changed')
    wait = wait.replace('    break;', '    return 0; // HOST: break exits the extracted switch arm')
    first = callback.index('            int seconds =')
    last_anchor = '            g_GuiView->spellSeconds = seconds;'
    last = callback.index(last_anchor, first) + len(last_anchor)
    countdown = callback[first:last]
    color = marker.index('].color =')
    value = marker.index('].value =')
    return ('// HOST BOUNDED SNIPPETS: explicit scalar access mappings; no target ABI claim.\n'
        + 'static const bool host_marker_color_before_value = ' + str(color < value).lower() + ';\n'
        + '#define ENEMY_READ_INT_DIRECT(index) HostReadInt(index)\n'
        + '#define ReadFloatArgument(index) HostReadFloat(index)\n'
        + '#define ReadIntArgument(index) HostReadInt(index)\n'
        + 'static int HostMarkerSnippet(EnemyRuntimeView *enemy) { unsigned int uVar22, uVar23; double fVar19; float local_2a8; int iVar26;\n' + marker + '}\n'
        + 'static int HostClearSnippet(EnemyRuntimeView *enemy) { unsigned int uVar22;\n' + clear + '}\n'
        + 'static unsigned int HostWaitSnippet() {\n' + wait + '}\n'
        + 'static int HostSegmentsSnippet() { unsigned int uVar22;\n' + segment + '}\n'
        + 'static void HostCountdownSnippet(EnemyRuntimeView *enemy, unsigned int callbackIndex) {\n' + countdown + '\n}\n'
        + '#undef ENEMY_READ_INT_DIRECT\n#undef ReadFloatArgument\n#undef ReadIntArgument\n')


@dataclass(frozen=True)
class Mutation:
    name: str
    before: str
    after: str
    check: str
    occurrence: int = 0

    def apply(self, text: str) -> str:
        positions = [m.start() for m in re.finditer(re.escape(self.before), text)]
        if len(positions) <= self.occurrence:
            raise RuntimeError(f'{self.name}: source anchor missing; review this negative control')
        at = positions[self.occurrence]
        return text[:at] + self.after + text[at + len(self.before):]


MUTATIONS = [
    Mutation('caller_manual_allocation', 'GuiMessageVmView *dialog = new GuiMessageVmView(', 'GuiMessageVmView *dialog = new (::operator new(sizeof(GuiMessageVmView))) GuiMessageVmView(', 'exception.free_once'),
    Mutation('message_lifecycle_order', 'memset(message, 0, sizeof(*message));\n    message->lifetimeTimer.SetCurrent(0);', 'message->lifetimeTimer.SetCurrent(0);\n    memset(message, 0, sizeof(*message));', 'message.lifecycle_order'),
    Mutation('bullet_1999', 'i < 2000', 'i < 1999', 'bullet.indices'),
    Mutation('bullet_2001', 'i < 2000', 'i < 2001', 'bullet.indices'),
    Mutation('bullet_state_filter', 'bullet->state != 0 && bullet->state != 3', 'bullet->state == 1 || bullet->state == 2', 'bullet.indices'),
    Mutation('bullet_state_three', 'bullet->state != 0 && bullet->state != 3', 'bullet->state != 0 && bullet->state != 2', 'bullet.indices'),
    Mutation('vm_layer', 'vm->renderLayer = 15;', 'vm->renderLayer = 14;', 'vm.layer'),
    Mutation('vm_flags_preserve', 'vm->flags35C |= 0x40000000u;', 'vm->flags35C = 0x40000000u;', 'vm.flags'),
    Mutation('vm_second_script', 'text->InitializeVm(vm, 1);', 'text->InitializeVm(vm, 0);', 'vm.scripts'),
    Mutation('vm_reload_text', 'text = g_AsciiManagerView->textAnm;', 'text = text;', 'vm.resources', 1),
    Mutation('vm_cached_second_allocate', 'vm = anm->AllocateVm();', 'vm = g_AnmRenderManagerView->AllocateVm();', 'vm.allocation_managers', 1),
    Mutation('vm_reload_first_insert', 'anm = g_AnmRenderManagerView;', 'anm = anm;', 'vm.insertion_managers', 1),
    Mutation('vm_reload_second_insert', 'anm = g_AnmRenderManagerView;', 'anm = anm;', 'vm.insertion_managers', 2),
    Mutation('vm_cached_lookup_manager', 'vm = anm->FindVm(', 'vm = g_AnmRenderManagerView->FindVm(', 'vm.lookup_managers'),
    Mutation('vm_separate_lookup', 'vm = anm->FindVm(message->vmIds[GUI_MESSAGE_TEXT_LINE_0]);', '/* reuse the prior LIVE VM, never null */', 'vm.lookup_count', 1),
    Mutation('vm_lookup_id', 'vm = anm->FindVm(message->vmIds[GUI_MESSAGE_TEXT_LINE_1]);', 'vm = anm->FindVm(message->vmIds[GUI_MESSAGE_TEXT_LINE_0]);', 'vm.lookup_ids'),
    Mutation('vm_glyph_destination', 'vm->glyphHeight = 16;', 'vm->glyphWidth = 16;', 'vm.glyph_destinations'),
    Mutation('timer_lower_inclusive', '*manager->timer.scale > 0.99f', '*manager->timer.scale >= 0.99f', 'timer.strict_interval'),
    Mutation('timer_upper_inclusive', '*manager->timer.scale < 1.01f', '*manager->timer.scale <= 1.01f', 'timer.strict_interval'),
    Mutation('timer_exact_one', '*manager->timer.scale > 0.99f && *manager->timer.scale < 1.01f', '*manager->timer.scale == 1.0f', 'timer.strict_interval'),
    Mutation('timer_collapse_current', '++manager->timer.current;', 'manager->timer.current = static_cast<int>(manager->timer.subframe + 1.0f);', 'timer.strict_interval'),
    Mutation('timer_previous', 'manager->timer.previous = manager->timer.current;', 'manager->timer.previous = -111;', 'timer.previous'),
    Mutation('timer_empty_list', 'EnemyListNodeView *node = manager->enemyListHead;', 'EnemyListNodeView *node = manager->enemyListHead; if (node == 0) return;', 'timer.previous'),
    Mutation('enemy_mask', 'enemy->flags & 0xc050u', 'enemy->flags & 0xc040u', 'enemy.flags'),
    Mutation('enemy_override', '(enemy->flags & 0x80u) != 0', '(enemy->flags & 0x80u) == 0', 'enemy.flags'),
    Mutation('enemy_negative_script', 'enemy->deathEffectScript >= 0', 'enemy->deathEffectScript > 0', 'enemy.effect_records'),
    Mutation('enemy_global_resource', 'g_EnemyManager->effectResources[enemy->deathEffectResourceIndex]', 'manager->effectResources[enemy->deathEffectResourceIndex]', 'enemy.global_resources'),
    Mutation('enemy_mark', 'enemy->flags |= 0x20000u;', 'enemy->flags |= 0x10000u;', 'enemy.flags'),
    Mutation('enemy_saved_next', 'node = next;', 'node = node->next;', 'enemy.flags'),
    Mutation('laser_state_skip', 'laser->state != 1', 'laser->state != 0', 'laser.nodes'),
    Mutation('laser_mode', 'laser->ApplyClear(0);', 'laser->ApplyClear(1);', 'laser.mode'),
    Mutation('laser_saved_next', 'laser = next;', 'laser = laser->next;', 'laser.nodes'),
    Mutation('laser_distinct_owner', 'g_EnemyLaserManager)->head', 'g_HostDecoyLaserManager)->head', 'laser.nodes'),
    Mutation('message_enemy_call', 'EnemyKillAll(g_EnemyManager);', '/* omit helper */', 'message.enemy_helper'),
    Mutation('message_color', '0x00f8f08fu', '0x00f8f080u', 'message.colors'),
    Mutation('message_position', 'AnmFloat3View(24.0f, 0.0f, 0.0f)', 'AnmFloat3View(23.0f, 0.0f, 0.0f)', 'message.positions'),
    Mutation('message_timer', 'message->waitTimer.SetCurrent(0);', 'message->waitTimer.SetCurrent(1);', 'message.timers'),
]


def run(command: list[str], *, timeout: int = 90) -> subprocess.CompletedProcess[str]:
    return subprocess.run(command, text=True, capture_output=True, timeout=timeout)


def intended_failure(result: subprocess.CompletedProcess[str], check: str) -> bool:
    return result.returncode == 1 and result.stdout == '' and result.stderr == f'CHECK {check}\n'


def test_classifier() -> None:
    correct = subprocess.CompletedProcess([], 1, '', 'CHECK intended\n')
    rejected = [
        subprocess.CompletedProcess([], -11, '', 'CHECK intended\n'),
        subprocess.CompletedProcess([], 139, '', 'CHECK intended\n'),
        subprocess.CompletedProcess([], 0, '', 'CHECK intended\n'),
        subprocess.CompletedProcess([], 2, '', 'CHECK intended\n'),
        subprocess.CompletedProcess([], 1, '', 'CHECK other\n'),
        subprocess.CompletedProcess([], 1, '', 'CHECK intended\nextra\n'),
        subprocess.CompletedProcess([], 1, 'unexpected stdout', 'CHECK intended\n'),
        subprocess.CompletedProcess([], 1, '', 'compiler error'),
    ]
    if not intended_failure(correct, 'intended') or any(intended_failure(result, 'intended') for result in rejected):
        raise RuntimeError('strict mutation classifier self-test failed')


def main() -> int:
    root = Path(__file__).resolve().parents[1]
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, default=root, help='repository/proposal root; headers fall back to baseline/src locally')
    parser.add_argument('--source', type=Path)
    parser.add_argument('--headers', type=Path)
    parser.add_argument('--caller-source', type=Path)
    parser.add_argument('--compiler', default='g++')
    parser.add_argument('--no-mutations', action='store_true')
    parser.add_argument('--keep-dir', type=Path, help='retain generated host inputs and executables in this local directory')
    args = parser.parse_args()
    test_classifier()
    root = args.root.resolve()
    args.source = args.source or root / 'src/GuiMessage.cpp'
    args.headers = args.headers or (root / 'src' if (root / 'src/Gui.hpp').exists() else root / 'baseline/src')
    source = args.source.read_text()
    caller_source = args.caller_source or root / 'src/EnemyEclDispatcher.cpp'
    dispatcher = caller_source.read_text()
    caller = extract_owner(dispatcher, 'static void EnemyReadDialog(GuiView *gui, int index)')
    snippets = gui_field_snippets(dispatcher, (root / 'src/EnemyCallback.cpp').read_text())
    extracted = '\n\n'.join([*(extract_owner(source, signature) for signature in OWNERS), caller]) + '\n'
    fixture = root / 'scripts/fixtures/gui-message-constructor.cpp'
    temporary = tempfile.TemporaryDirectory(prefix='gui-message-host-') if args.keep_dir is None else None
    directory = Path(temporary.name) if temporary else args.keep_dir
    directory.mkdir(parents=True, exist_ok=True)
    omitted = host_headers(args.headers, directory, root / 'baseline/src')
    shutil.copyfile(fixture, directory / 'fixture.cpp')
    (directory / 'gui-field-snippets.inc').write_text(snippets)
    # Source declarations are copied separately; source OWNER bodies are unchanged.
    declarations = 'extern EnemyManagerView *g_EnemyManager;\nunsigned int EnemyCancelBulletRecord(unsigned char *bullet);\n'
    selected = [] if args.no_mutations else MUTATIONS
    for mutation in [None, *selected]:
        name = mutation.name if mutation else 'baseline'
        body = mutation.apply(extracted) if mutation else extracted
        lifecycle = [body.find(anchor) for anchor in ('GuiMessageVmView *message = this;', 'memset(message, 0, sizeof(*message));', 'message->lifetimeTimer.SetCurrent(0);', 'message->instructionTimer.SetCurrent(0);', 'message->waitTimer.SetCurrent(')]
        lifecycle_order = all(position >= 0 for position in lifecycle) and lifecycle == sorted(lifecycle)
        (directory / 'gui-message-extracted.inc').write_text(declarations + 'static const bool host_source_lifecycle_order = ' + str(lifecycle_order).lower() + ';\n' + body)
        executable = directory / name
        compile_result = run([args.compiler, '-std=c++17', '-O1', '-fno-strict-aliasing', '-Wall', '-Wextra', '-Wno-class-memaccess', '-I', str(directory), str(directory / 'fixture.cpp'), '-o', str(executable)])
        if compile_result.returncode != 0:
            raise RuntimeError(f'{name}: COMPILE FAILURE, never a mutation success\n{compile_result.stdout}{compile_result.stderr}')
        result = run([str(executable)])
        if mutation:
            expected = f'CHECK {mutation.check}\n'
            if not intended_failure(result, mutation.check):
                raise RuntimeError(f'{name}: wrong mutation outcome; expected exit 1 and {expected!r}; got exit {result.returncode}, stdout={result.stdout!r}, stderr={result.stderr!r}')
            category = 'STATIC source-order control' if name == 'message_lifecycle_order' else 'behavioral mutation'
            print(f'PASS {category} {name}: {mutation.check}')
        elif result.returncode != 0 or result.stdout != 'host semantic fixture passed\n' or result.stderr:
            raise RuntimeError(f'baseline failed: exit {result.returncode}, stdout={result.stdout!r}, stderr={result.stderr!r}')
        else:
            print('PASS source-extracted member constructor/helper/caller with explicit timer seams (body semantics and synchronous exception lifecycle)')
    # Retained source must represent the baseline, not the last negative control.
    (directory / 'gui-message-extracted.inc').write_text(declarations + 'static const bool host_source_lifecycle_order = true;\n' + extracted)
    static_count = sum(m.name == 'message_lifecycle_order' for m in selected)
    print(f'PASS {len(selected) - static_count} behavioral mutations + {static_count} STATIC source-order controls, all strictly classified; {omitted} target-only assertions omitted in temporary host headers')
    print('PASS bounded actual-header GUI field snippets: marker, clear, wait, segments, signed countdown; explicit host scalar mappings and separate STATIC store-order check.')
    print('PASS ordinary-new lifecycle: synchronous callback throws free once, skip the complete-message destructor, retain earlier effects, and do not publish/update GUI state.')
    print('PASS strict classifier self-test (signals, crashes, compile errors, other CHECKs and accidental passes rejected).')
    print('Host constructor seams: AnmVmTimerView, PlayerTimerView, EnemyInterpolationTimerView; maintained raw-integer default-construction model validity UNPROVED.')
    print('Dependency lifecycle stubs: AnmVmView ctor/dtor; AnmLoadedView ctor; AnmRenderManagerView ctor/dtor; AsciiManagerView ctor/dtor; GuiView dtor; GuiMessageVmView counting dtor; EnemyManagerView ctor/dtor; BulletManagerView dtor; EnemyEclContextView ctor; EnemyBulletPatternView ctor; EnemyEclHostBaseView ctor/dtor; EnemyFullObjectView ctor/dtor. Other dependency methods/virtuals are explicit fixture seams.')
    print('All instantiated GuiView, AnmVmView and message objects have real host C++ lifetimes. Ordinary global new/delete observation is host-only; no production cleanup is stubbed into the member constructor or caller.')
    print('No target ABI/layout, binary equivalence, target runtime, or product-closure claim.')
    if temporary:
        temporary.cleanup()
    return 0


if __name__ == '__main__':
    try:
        sys.exit(main())
    except (OSError, RuntimeError, subprocess.TimeoutExpired) as error:
        print(f'FAIL {error}', file=sys.stderr)
        sys.exit(1)
