#!/usr/bin/env python3
"""Local host-only caller contract for the actual complete 0x417C80 source.

The lexical region beginning // Target417C80. through EOF is executed, including
any real helper in that region. This fixture does NOT execute native dependencies,
prove x86 layout/ABI, prove byte exactness, or establish Hint exporter retention.
Platform and subsystem seams are mocks used ONLY by this host test. Every delete
expression is observed through HostDelete after mutations have been applied;
HostDelete preserves destruction followed by matching allocation release.
Mutation success requires compilation and exactly the intended CHECK diagnostic;
compiler failure, signals, sanitizer reports, and timeouts are harness errors.
"""
from pathlib import Path
import argparse
import hashlib
import os
import re
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
START = '// Target417C80.'
DIAGNOSTIC = re.compile(r'GAME_TEARDOWN_CHECK_FAILED line ([1-9][0-9]*): ([^\r\n]+)\n\Z')


def require(condition, message):
    if not condition:
        raise RuntimeError(message)


def normalized(text):
    return re.sub(r'\s+', '', text)


def balanced(source, anchor, semicolon=False):
    require(source.count(anchor) == 1, 'source extraction anchor changed: ' + anchor)
    start = source.index(anchor)
    brace = source.index('{', start)
    depth, end = 1, brace + 1
    while depth:
        require(end < len(source), 'unbalanced source: ' + anchor)
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    if semicolon:
        require(source[end] == ';', 'missing declaration semicolon: ' + anchor)
        end += 1
    return source[start:end]


def canonical(root, filename):
    path = root / 'src' / filename
    if not path.exists():
        path = root / 'baseline/src' / filename
    require(path.exists(), 'missing canonical seam source: ' + str(path))
    return path.read_text()


def audit_seams(root, source, fixture):
    anm = canonical(root, 'AnmManager.hpp')
    main = canonical(root, 'Main.hpp')
    replay = canonical(root, 'ReplayManager.hpp')
    replay_chain = canonical(root, 'ReplayChain.hpp')
    item = canonical(root, 'ItemManager.hpp')
    hint = canonical(root, 'Hint.hpp')

    for signature in [
        'int GameFinalizeStatistics42B1E0(GameStatisticsRowView *statistics);',
        'void __stdcall GameCleanupController422220(GameChainOwnerView *owner);',
        'void __stdcall GameCleanupGui4145F0(GameGuiUpdateView *owner);',
        'void __stdcall GameCleanupAnimation405F70(GameChainOwnerView *owner);',
        'void __stdcall GameCleanupLinked41C1C0(GameChainOwnerView *owner);',
        'void __stdcall GameCleanupOwner42B570(GameChainOwnerView *owner);',
        'void GamePrepareGuiReuse414570(GameGuiUpdateView *owner);',
        'void __stdcall GameCleanupOwner40AF00(GameChainOwnerView *owner);',
        'void __stdcall GameCleanupLoader405620(GameChainOwnerView *owner);',
        'void __stdcall GameCleanupOwner408AF0(GameAdditionalChainView *owner);',
        'void __stdcall GameClearEnemyVms(EnemyManagerView *manager);',
        'void FrontEndQueueSoundCommand(const char *path, int opcode, int argument);',
        'unsigned char *Fn00418A90(unsigned char *owner);']:
        require(normalized(signature) in normalized(source), 'called boundary declaration drift: ' + signature)
    for signature in ['AnmVmIdView CreateVmAtScreenVariant0(int scriptIndex, const AnmFloat3View *position);',
                      'int EnsureSelectionVm(float x, float y);', 'AnmLoadedView *asciiAnm;', 'int auxiliaryVm89A4;']:
        require(normalized(signature) in normalized(anm), 'ANM called type/field drift: ' + signature)
        require(normalized(signature) in normalized(fixture), 'host ANM seam drift: ' + signature)
    for field in ['unsigned int options;', 'signed char hintMode;',
                  'CRITICAL_SECTION criticalSections[7];', 'unsigned char criticalSectionLockCounts[7];']:
        require(normalized(field) in normalized(main), 'Main called field drift: ' + field)
        require(normalized(field) in normalized(fixture), 'host Main seam drift: ' + field)
    for field in ['ReplayChainElement *playbackChain;', 'ReplayChainElement *drawChain;', 'ReplayChainElement *updateChain;']:
        require(normalized(field) in normalized(replay), 'Replay called field drift: ' + field)
    require('ItemRecordViewitems[0x896];' in normalized(item), 'Item array type/count drift')
    require('sizeof(ItemRecordView)==0x3f0' in normalized(item), 'Item record extent drift')
    require('offsetof(ItemManagerView,items)==0x14' in normalized(item), 'Item byte-range start drift')
    require('void__stdcallCleanupHintOwner(HintOwnerView*owner);' in normalized(hint), 'Hint boundary drift')
    for filename, destructor in [('ReplayManager.hpp', '~ReplayManager();'), ('StageScript.hpp', '~StageScriptView();'),
                                 ('Player.hpp', '~Player();'), ('Enemy.hpp', '~EnemyManagerView();'),
                                 ('ItemManager.hpp', '~ItemManagerView();')]:
        require(destructor in canonical(root, filename), 'genuine destructor boundary absent: ' + filename)
    game_types = ['GameManager', 'GameChainElementView', 'GameChainOwnerView', 'GameMenuView',
                  'GameGuiUpdateView', 'GameAdditionalChainView', 'GameStatisticsRowView']
    fixture = fixture.replace('@GAME_DECLARATIONS@', 'struct GameChainElementView;\n' + '\n'.join(
        balanced(source, 'struct ' + name + '\n', True) for name in game_types))
    fixture = fixture.replace('@CHAIN_DECLARATIONS@', canonical(root, 'Chain.hpp'))
    fixture = fixture.replace('@VM_ID_DECLARATIONS@', canonical(root, 'AnmVmId.hpp').replace('#pragma once', '/* amalgamated header: pragma once removed */'))
    fixture = fixture.replace('@FLOAT3_DECLARATION@', balanced(anm, 'struct AnmFloat3View\n', True))
    fixture = fixture.replace('@REPLAY_DECLARATIONS@', '\n'.join([
        'class ReplayManager;', balanced(replay, 'struct ReplayListNode\n', True),
        'typedef int (*ReplayChainCallback)(ReplayManager *manager);',
        balanced(replay_chain, 'struct ReplayChainElement\n', True),
        'class ReplayManager { public: ReplayChainElement *updateChain; ReplayChainElement *drawChain; ReplayChainElement *playbackChain; ~ReplayManager(); };']))
    for marker, name in [('@GLOBALS_AND_BOUNDARIES@', 'game-teardown-globals.inc'),
                         ('@BOUNDARY_DEFINITIONS@', 'game-teardown-boundaries.inc'),
                         ('@FIXTURE_SCENARIOS@', 'game-teardown-scenarios.inc')]:
        fixture = fixture.replace(marker, (root / 'scripts/fixtures' / name).read_text())
    print('PASS canonical called-signature/field audits; actual small declarations copied', flush=True)
    return fixture


def extract(root):
    source = (root / 'src/GameManager.cpp').read_text()
    require(source.count(START) == 1, 'complete-owner source marker changed')
    region = source[source.index(START):]
    require(region.count('void __stdcall GameManagerCleanup(GameManager *manager)') == 1,
            'missing/duplicate cleanup body')
    body = balanced(region, 'void __stdcall GameManagerCleanup(GameManager *manager)')
    require(region.rstrip().endswith(body), 'cleanup must be complete and final in extracted region')
    return source, region


def instrument(region):
    # Applied only after mutation. Observe all candidate delete operands, including
    # an intentionally wrong global operand, without replacing caller control flow.
    region = re.sub(r'::operator delete\(([A-Za-z_]\w*)\);', r'fixture::HostFree(\1);', region)
    return re.sub(r'\bdelete\s+([A-Za-z_]\w*)\s*;', r'fixture::HostDelete(\1);', region)


def mutate(region, spec):
    name, old, new, expected, *rest = spec
    count = rest[0] if rest else 1
    require(region.count(old) == count,
            '%s: expected %d source anchors, got %d: %r' % (name, count, region.count(old), old))
    candidate = region.replace(old, new)
    if name == 'secondary-stage-cached-too-early':
        needle = 'stage = reinterpret_cast<StageScriptView *>(g_GameMenu);'
        require(candidate.count(needle) == 2, 'secondary stage cache mutation anchor drift')
        candidate = candidate.replace(needle, 'stage = reinterpret_cast<StageScriptView *>(cachedMenu);', 1)
    if name == 'merge-chain-locks':
        candidate = candidate.replace('LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);\n        --g_MainSupervisorView.criticalSectionLockCounts[0];', '', 1)
    if name == 'sound-flags-cached-before-call':
        require(candidate.count('unsigned int finalFlags = g_GameRuntimeFlags;') == 2, 'sound snapshot mutant drift')
        point = candidate.rindex('unsigned int finalFlags = g_GameRuntimeFlags;')
        candidate = candidate[:point] + candidate[point:].replace('unsigned int finalFlags = g_GameRuntimeFlags;', '', 1)
    return candidate


# Filled with actual-source mutation anchors. Each exact expected CHECK expression
# is declared here before execution, never learned from a mutant's output.
MUTATIONS = [
 ('selection-flags-cached', 'g_AsciiManagerView->EnsureSelectionVm(224.0f, 416.0f);\n        g_GameRuntimeFlags |= 2u;',
  'unsigned int cachedFlags = g_GameRuntimeFlags;\n        g_AsciiManagerView->EnsureSelectionVm(224.0f, 416.0f);\n        g_GameRuntimeFlags = cachedFlags | 2u;', 'g_GameRuntimeFlags == expectedFinalFlags'),
 ('statistics-order', 'GameFinalizeStatistics42B1E0(g_GameStatisticsRows);\n    g_GameRuntimeFlags &= ~3u;',
  'g_GameRuntimeFlags &= ~3u;\n    GameFinalizeStatistics42B1E0(g_GameStatisticsRows);', 'g_GameRuntimeFlags == expectedInitialFlags'),
 ('selection-transition-reloads-omitted', '\n        transition = g_GameTransitionState;', '\n        transition = transition;', 'events == expected', 3),
 ('merge-chain-locks', 'element = manager->drawChain;\n    chain = th10::g_Chain;\n    if (element != NULL)\n    {\n        EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);\n        ++g_MainSupervisorView.criticalSectionLockCounts[0];',
  'element = manager->drawChain;\n    chain = th10::g_Chain;\n    if (element != NULL)\n    {', 'events == expected'),
 ('sound-flags-cached-before-call', 'g_GameManager = NULL;', 'unsigned int finalFlags = g_GameRuntimeFlags;\n    g_GameManager = NULL;',
  'static_cast<unsigned int>(g_StageScriptGlobal4923A8) == ((flags & 1u) ? 0u : 0xff000000u)'),
 ('cleanup-order', '''GameChainOwnerView *controller = g_GameOwner477830;
        if (controller != NULL)
        {
            GameCleanupController422220(controller);
            ::operator delete(controller);
        }
        GameGuiUpdateView *gui = g_GameGuiUpdate;
        if (gui != NULL)
        {
            GameCleanupGui4145F0(gui);
            ::operator delete(gui);
        }''', '''GameGuiUpdateView *gui = g_GameGuiUpdate;
        if (gui != NULL)
        {
            GameCleanupGui4145F0(gui);
            ::operator delete(gui);
        }
        GameChainOwnerView *controller = g_GameOwner477830;
        if (controller != NULL)
        {
            GameCleanupController422220(controller);
            ::operator delete(controller);
        }''', 'events == expected'),

 ('statistics-result-propagated', 'GameFinalizeStatistics42B1E0(g_GameStatisticsRows);',
  'if (GameFinalizeStatistics42B1E0(g_GameStatisticsRows) != 0) return;', 'released[Hint] == ((!reuse && s.missing != Hint) ? 1 : 0)'),
 ('low-bit-not-cleared', 'g_GameRuntimeFlags &= ~3u;', 'g_GameRuntimeFlags &= ~2u;', 'events == expected'),
 ('high-flags-dropped', 'g_GameRuntimeFlags &= ~3u;', 'g_GameRuntimeFlags = 0;', 'g_GameRuntimeFlags == expectedFinalFlags'),
 ('timer-not-restored', 'g_PlayerTimerScale = 1.0f;', ';', 'g_PlayerTimerScale == 1.0f'),
 ('transition10-script', 'CreateVmAtScreenVariant0(6, &position)', 'CreateVmAtScreenVariant0(7, &position)', 'scriptIndex == 6'),
 ('transition10-position', 'AnmFloat3View position(480.0f, 392.0f, 0.0f);',
  'AnmFloat3View position(480.0f, 393.0f, 0.0f);', 'position->x == 480.0f && position->y == 392.0f && position->z == 0.0f'),
 ('transition10-existing-vm-recreated', 'if (ascii->auxiliaryVm89A4 == 0)', 'if (true)', 'ascii.auxiliaryVm89A4 == 0'),
 ('transition10-publication-wrong-owner', 'ascii->auxiliaryVm89A4 =\n                ascii->asciiAnm',
  'g_AsciiManagerView->auxiliaryVm89A4 =\n                ascii->asciiAnm', 'ascii.auxiliaryVm89A4 == (s.existingVm ? 712 : 1907)'),
 ('transition-reloads-omitted', '            transition = g_GameTransitionState;', '            transition = transition;', 'events == expected'),
 ('transition11-coordinates', 'EnsureSelectionVm(224.0f, 416.0f)', 'EnsureSelectionVm(480.0f, 392.0f)', 'x == expectedX && y == expectedY'),
 ('transition11-reuse-flag-omitted', 'g_GameRuntimeFlags |= 2u;', ';', 'released[Hint] == ((!reuse && s.missing != Hint) ? 1 : 0)'),
 ('transition13-leaf-omitted', 'th10::leaf::Fn00418A90(g_GameStageState);', ';', 'events == expected'),
 ('transition13-bit8-omitted', 'g_GameRuntimeFlags |= 8u;', ';', 'g_GameRuntimeFlags == expectedFinalFlags'),
 ('replay-retention-condition', 'transition != 14 && transition != 15', 'transition != 14 || transition != 15', 'events == expected'),
 ('primary-stage-delete-omitted', 'if (stage != NULL)\n            delete stage;\n        stage =',
  'if (stage != NULL) {}\n        stage =', 'events == expected'),
 ('secondary-stage-cached-too-early', 'StageScriptView *stage = reinterpret_cast<StageScriptView *>(g_GameAnimationSlots);',
  'GameMenuView *cachedMenu = g_GameMenu;\n        StageScriptView *stage = reinterpret_cast<StageScriptView *>(g_GameAnimationSlots);', 'events == expected'),
 ('hint-cleanup-omitted', 'CleanupHintOwner(hint);', ';', 'NaturalDestructor<T> || allocation.cleaned'),
 ('hint-release-omitted', 'delete hint;', ';', 'released[Hint] == ((!reuse && s.missing != Hint) ? 1 : 0)'),
 ('hint-release-before-cleanup', 'CleanupHintOwner(hint);\n            delete hint;',
  'delete hint;\n            CleanupHintOwner(hint);', 'NaturalDestructor<T> || allocation.cleaned'),
 ('hint-released-via-cleared-global', 'delete hint;', 'delete g_GameOwner477814;', 'released[Hint] == ((!reuse && s.missing != Hint) ? 1 : 0)'),
 ('raw-release-omitted', '::operator delete(controller);', ';', 'events == expected'),
 ('raw-release-before-cleanup', 'GameCleanupController422220(controller);\n            ::operator delete(controller);',
  '::operator delete(controller);\n            GameCleanupController422220(controller);', 'allocation.live && allocation.cleaned'),
 ('reuse-stage-publication-omitted', 'g_GameMenu = reinterpret_cast<GameMenuView *>(g_GameAnimationSlots);', ';',
  'g_GameMenu == reinterpret_cast<GameMenuView *>(savedPrimary)'),
 ('reuse-controller-bit-wrong', 'controller->updateChain->flags &= ~2u;', 'controller->updateChain->flags &= ~4u;', 'ownerElements[i].flags == expectedFlags'),
 ('reuse-replay-update-instead-of-playback', 'replay->playbackChain', 'replay->updateChain', 'replayElements[0].unknownFlag1 == 1', 2),
 ('reuse-replay-draw-omitted', 'replay->drawChain->unknownFlag1 = 0;', '{}', 'replayElements[2].unknownFlag1 == ((s.callbackMask & 32u) ? 0u : 1u)'),
 ('wipe-short', 'memset(items->items, 0, 0x21cea0);', 'memset(items->items, 0, 0x21ce9c);', 'bytes[i] == 0'),
 ('wipe-long', 'memset(items->items, 0, 0x21cea0);', 'memset(items->items, 0, 0x21cea4);', 'byte == 0xa5'),
 ('wipe-prefix', 'memset(items->items, 0, 0x21cea0);',
  'memset(reinterpret_cast<unsigned char *>(items->items) - 4, 0, 0x21cea0);', 'byte == 0xa5'),
 ('enemy-mask-wrong', '(g_GameRuntimeFlags & 9u) == 0', '(g_GameRuntimeFlags & 1u) == 0', 'events == expected'),
 ('manager-global-not-cleared', 'g_GameManager = NULL;', ';', 'g_GameManager == NULL'),
 ('first-chain-not-captured', 'reinterpret_cast<th10::ChainElem *>(element), chain)',
  'reinterpret_cast<th10::ChainElem *>(element), ((void)chain, th10::g_Chain))', 'chain == chainBeforeEnter', 2),
 ('second-chain-not-reloaded', 'element = manager->drawChain;\n    chain = th10::g_Chain;',
  'element = manager->drawChain;', 'chain == chainBeforeEnter'),
 ('second-element-not-reloaded', 'element = manager->drawChain;', 'element = manager->updateChain;',
  'element == reinterpret_cast<th10::ChainElem *>(elementBeforeEnter)'),
 ('chain-increment-omitted', '++g_MainSupervisorView.criticalSectionLockCounts[0];', ';',
  'g_MainSupervisorView.criticalSectionLockCounts[0] == static_cast<unsigned char>(initialDepth + 1)', 2),
 ('chain-decrement-before-leave', 'LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);\n        --g_MainSupervisorView.criticalSectionLockCounts[0];',
  '--g_MainSupervisorView.criticalSectionLockCounts[0];\n        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);',
  'g_MainSupervisorView.criticalSectionLockCounts[0] == static_cast<unsigned char>(initialDepth + 1)', 2),
 ('chain-lock-omitted', 'EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);', ';', 'criticalHeld', 2),
 ('chain-unlock-omitted', 'LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);', ';', '!criticalHeld', 2),
 ('sound-mask-bit2-omitted', '(g_GameRuntimeFlags & 0x22u) == 0', '(g_GameRuntimeFlags & 0x20u) == 0', 'events == expected'),
 ('sound-mask-bit20-omitted', '(g_GameRuntimeFlags & 0x22u) == 0', '(g_GameRuntimeFlags & 2u) == 0', 'events == expected'),
 ('sound-opcode-wrong', '!= 0 ? 4 : 3, 0)', '!= 0 ? 3 : 4, 0)', 'opcode == expectedOpcode && argument == 0'),
 ('sound-flags-cached', 'unsigned int finalFlags = g_GameRuntimeFlags;', 'unsigned int finalFlags = 0;',
  'static_cast<unsigned int>(g_StageScriptGlobal4923A8) == ((flags & 1u) ? 0u : 0xff000000u)'),
 ('final-counter-omitted', 'g_GameStartupCounter = 1;', ';', 'g_GameStartupCounter == 1'),
 ('final-color-boolean-alpha', 'static_cast<int>(0xff000000u)', '255',
  'static_cast<unsigned int>(g_StageScriptGlobal4923A8) == ((flags & 1u) ? 0u : 0xff000000u)'),
]


# Duplicate expressions must select an explicit intended function or guard line.
# Every other expression must resolve to exactly one generated CHECK site.
CHECK_SCOPES = {
    'timer-not-restored': 'void Run(const Scenario &s) {',
    'manager-global-not-cleared': 'void FrontEndQueueSoundCommand(const char *path, int opcode, int argument) {',
    'chain-increment-omitted': 'void th10::Chain::UnregisterElem(th10::ChainElem *element, th10::Chain *chain) {',
    'chain-decrement-before-leave': 'void LeaveCriticalSection(CRITICAL_SECTION *section) {',
    'chain-lock-omitted': 'void th10::Chain::UnregisterElem(th10::ChainElem *element, th10::Chain *chain) {',
    'chain-unlock-omitted': 'void EnterCriticalSection(CRITICAL_SECTION *section) {',
}
CHECK_LINES = {
    'wipe-long': 'for (unsigned char byte : savedItem->suffix) CHECK(byte == 0xa5);',
    'wipe-prefix': 'for (unsigned char byte : savedItem->prefix) CHECK(byte == 0xa5);',
}


def intended_check_line(cpp, spec):
    name, expression = spec[0], spec[3]
    start, end = 0, len(cpp)
    if name in CHECK_SCOPES:
        anchor = CHECK_SCOPES[name]
        region = balanced(cpp, anchor)
        start = cpp.index(anchor)
        end = start + len(region)
    exact = CHECK_LINES.get(name)
    matches = []
    offset = 0
    for number, line in enumerate(cpp.splitlines(keepends=True), 1):
        if start <= offset < end and 'CHECK(' + expression + ');' in line:
            if exact is None or exact in line:
                matches.append(number)
        offset += len(line)
    require(len(matches) == 1, '%s: intended CHECK source location ambiguous/missing: %r' % (name, matches))
    return matches[0]


def build_run(cpp, directory, name, optimize, sanitize):
    source = directory / (name + '.cpp')
    binary = directory / name
    source.write_text(cpp)
    command = [os.environ.get('CXX', 'g++'), '-std=c++17', '-' + optimize,
               '-Wall', '-Wextra', '-Werror', '-Wno-unused-function',
               '-Wno-unused-parameter', str(source), '-o', str(binary)]
    if sanitize:
        command[1:1] = ['-fsanitize=address,undefined', '-fno-omit-frame-pointer', '-fno-pie', '-no-pie']
    compiled = subprocess.run(command, text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=90)
    require(compiled.returncode == 0, 'HARNESS compile failure for %s:\n%s%s' % (name, compiled.stdout, compiled.stderr))
    result = subprocess.run([str(binary)], text=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                            timeout=45, env={**os.environ, 'ASAN_OPTIONS': 'detect_leaks=0:halt_on_error=1',
                                             'UBSAN_OPTIONS': 'halt_on_error=1:print_stacktrace=1'})
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, default=ROOT)
    parser.add_argument('--opt', choices=('O0', 'O1', 'O2'), default='O2')
    parser.add_argument('--sanitize', action='store_true')
    parser.add_argument('--baseline-only', action='store_true')
    args = parser.parse_args()
    source, region = extract(args.root)
    template = (args.root / 'scripts/fixtures/game-manager-teardown.cpp').read_text()
    template = audit_seams(args.root, source, template)
    require(template.count('@ACTUAL_OWNER_SOURCE@') == 1, 'fixture insertion point changed')
    print('Actual owner region SHA256 ' + hashlib.sha256(region.encode()).hexdigest(), flush=True)
    build_root = args.root / 'build'
    build_root.mkdir(exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='game-teardown-', dir=build_root) as tmp:
        directory = Path(tmp)
        baseline = build_run(template.replace('@ACTUAL_OWNER_SOURCE@', instrument(region)),
                             directory, 'baseline', args.opt, args.sanitize)
        require(baseline.returncode == 0 and baseline.stderr == '' and
                baseline.stdout.startswith('PASS game teardown caller contract: '),
                'HARNESS baseline failure:\n%s%s' % (baseline.stdout, baseline.stderr))
        print(baseline.stdout.rstrip(), flush=True)
        if not args.baseline_only:
            for spec in MUTATIONS:
                candidate = mutate(region, spec)
                cpp = template.replace('@ACTUAL_OWNER_SOURCE@', instrument(candidate))
                expected_line = intended_check_line(cpp, spec)
                result = build_run(cpp, directory, spec[0], args.opt, args.sanitize)
                diagnostic = DIAGNOSTIC.fullmatch(result.stderr)
                require(result.returncode == 1 and result.stdout == '' and diagnostic is not None
                        and diagnostic.group(2) == spec[3] and int(diagnostic.group(1)) == expected_line,
                        'HARNESS invalid mutant result %s: code=%s; expected=%r; stdout=%r; stderr=%r' %
                        (spec[0], result.returncode, spec[3], result.stdout, result.stderr))
                print('PASS intended CHECK: %s -> line %d: %s' % (spec[0], expected_line, spec[3]), flush=True)
            print('PASS %d compiled mutations with exact intended CHECK classifications' % len(MUTATIONS), flush=True)


if __name__ == '__main__':
    try:
        main()
    except (RuntimeError, subprocess.TimeoutExpired) as error:
        raise SystemExit(str(error))
