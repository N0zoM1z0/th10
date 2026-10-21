#!/usr/bin/env python3
"""Execute extracted actual Hint owner lifecycle source with audited dependencies.

Copies Hint.hpp, Chain.hpp, and AnmVmId.hpp unchanged. The entire lexical region
from // Target418D40. up to // Target419120. executes, including constructor,
Initialize, CleanupHintOwner, factory, and both callbacks. Actual entry constructor,
ClearHintLists, ANM FindVm, and MarkVmForDeletion bodies execute as dependencies.
The actual vector/node declarations are extracted; large ANM/Main dependencies
use exact called-signature/field-type seams audited against canonical headers.

Documented substitutions, applied AFTER source mutation: factory Initialize call
uses a seam which executes real Initialize then optionally injects a failed result;
this tests the real factory failure branch but does not claim Initialize can fail.
Owner/entry delete boundaries observe live objects then perform matching delete.
The ClearHintLists and ANM MarkVmForDeletion definitions are renamed to allow
observing their calls before running their unchanged bodies. There is no failed
allocation, repeated cleanup, destroyed-owner read, or invalid lifetime traversal.

These are bounded host source contracts, not target execution, x86 layout/ABI,
original source identity, runtime allocator identity, or instruction-byte proof.
Every mutant must COMPILE and exit exactly 1 with exactly one well-formed CHECK
failure at its intended fixture expression. Signals, sanitizer diagnostics, compile
errors, timeouts, extra output, and generic failures are harness errors, not kills.
"""
from pathlib import Path
import argparse
import os
import re
import shutil
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[1]
START = '// Target418D40.'
END = '// Target419120.'
DIAGNOSTIC = re.compile(r'HINT_LIFECYCLE_CHECK_FAILED line ([1-9][0-9]*): ([^\r\n]+)\n\Z')


def require(condition, message):
    if not condition:
        raise RuntimeError(message)


def balanced(source, start, semicolon=False):
    require(source.count(start) == 1, 'source extraction anchor changed: ' + start)
    offset = source.index(start)
    brace = source.index('{', offset)
    depth, end = 1, brace + 1
    while depth:
        require(end < len(source), 'unbalanced source: ' + start)
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    if semicolon:
        require(source[end] == ';', 'missing declaration semicolon: ' + start)
        end += 1
    return source[offset:end]


def normalized(source):
    return re.sub(r'\s+', '', source)


def audit_integration(root):
    """Lexical integration gates are separate from behavioral mutation kills."""
    hint = (root / 'src/Hint.hpp').read_text()
    game = (root / 'src/GameManager.cpp').read_text()
    declaration = 'extern HintOwnerView *g_GameOwner477814;'
    require(len(re.findall(r'\bextern\s+HintOwnerView\s*\*\s*g_GameOwner477814\s*;', hint)) == 1, 'Hint.hpp needs exactly one typed canonical 477814 declaration')
    require(not re.search(r'extern\s+GameChainOwnerView\s*\*\s*g_GameOwner477814\b', game),
            'GameManager retained an incompatible generic 477814 declaration')
    require('GameCreateStageOwner' not in game, 'stale GameCreateStageOwner boundary remains')
    require('#include "Hint.hpp"' in game, 'GameManager must use the canonical Hint declaration')
    require('CreateHintOwner() == NULL' in game, 'GameManager startup does not call CreateHintOwner')
    for field in ['updateChain', 'drawChain']:
        require('if (g_GameOwner477814->%s != NULL)' % field in game, 'missing typed chain null check: ' + field)
        require('g_GameOwner477814->%s->flags |= 2;' % field in game, 'missing typed chain mark: ' + field)
    for line in game.splitlines():
        if 'g_GameOwner477814' in line:
            require('reinterpret_cast' not in line and 'static_cast' not in line and 'GameChainOwnerView' not in line,
                    'unrelated prefix cast/declaration at canonical Hint owner: ' + line)
    source = (root / 'src/Hint.cpp').read_text()
    factory = balanced(source, 'HintOwnerView *CreateHintOwner()')
    require('if (owner->Initialize() != 0)' in factory, 'factory must preserve its unconditional initialize call')
    require(re.search(r'if \(owner != NULL\)\s*\{\s*CleanupHintOwner\(owner\);\s*delete owner;', factory) is not None,
            'factory failure arm must retain its native owner-null cleanup/delete guard')
    print('PASS source integration audit (separate from behavioral controls)', flush=True)


def audit_dependencies(root):
    canonical = root / 'src'
    anm = (canonical / 'AnmManager.hpp').read_text()
    main = (canonical / 'Main.hpp').read_text()
    fixture = (root / 'scripts/fixtures/hint-lifecycle.cpp').read_text()
    signatures = ['AnmVmView *FindVm(AnmVmIdView id);', 'void MarkVmForDeletion(AnmVmIdView id);']
    compact = normalized(anm.replace('TH10_ANM_NOINLINE ', ''))
    for signature in signatures:
        require(normalized(signature) in compact, 'audited ANM signature changed: ' + signature)
        require(normalized(signature) in normalized(fixture), 'fixture signature changed: ' + signature)
    for field in ['int id;', 'unsigned int flags35C;', 'AnmVmLayerNodeView layerNode;',
                  'AnmVmLayerNodeView *primaryVmListHead;', 'AnmVmLayerNodeView *secondaryVmListHead;']:
        require(normalized(field) in compact, 'audited ANM field changed: ' + field)
    for field in ['signed char hintMode;', 'CRITICAL_SECTION criticalSections[7];',
                  'unsigned char criticalSectionLockCounts[7];',
                  'extern MainSupervisorView g_MainSupervisorView;']:
        require(normalized(field) in normalized(main), 'audited Main dependency changed: ' + field)
    require('externAnmRenderManagerView*g_AnmRenderManagerView;' in compact, 'audited ANM global changed')
    fixture = fixture.replace('@ANM_FLOAT3@', balanced(anm, 'struct AnmFloat3View\n', True))
    # Compile real scheduler constructors for the copied complete declarations.
    # Registration/locking boundaries remain the explicit host seams below.
    chain = (canonical / 'Chain.cpp').read_text()
    constructors = '\nnamespace th10 {\n' + '\n'.join(
        balanced(chain, anchor) for anchor in
        ['ChainElem::ChainElem()', 'Chain::Chain()']) + '\n}\n'
    require(fixture.count('#include "Hint.hpp"') == 1, 'Hint header insertion changed')
    fixture = fixture.replace('#include "Hint.hpp"', '#include "Hint.hpp"\n' + constructors)
    return fixture.replace('@ANM_LAYER_NODE@', balanced(anm, 'struct AnmVmLayerNodeView\n', True))


def extract_sources(root):
    hint = (root / 'src/Hint.cpp').read_text()
    require(hint.count(START) == 1 and hint.count(END) == 1, 'lifecycle source marker changed')
    lifecycle = hint[hint.index(START):hint.index(END)]
    require(lifecycle.count('HintOwnerView::HintOwnerView()') == 1, 'constructor not in extracted region')
    anm = (root / 'src/AnmManager.cpp').read_text()
    support = '\n\n'.join([
        balanced(hint, 'HintEntryView::HintEntryView()'),
        balanced(hint, 'void ClearHintLists(HintOwnerView *owner)'),
        balanced(anm, 'AnmVmView *AnmRenderManagerView::FindVm(AnmVmIdView id)'),
        balanced(anm, 'void AnmRenderManagerView::MarkVmForDeletion(AnmVmIdView id)'),
    ])
    return lifecycle, support


# Each tuple records source, lexical mutation, and its exact intended CHECK.
# Expected diagnostics never come from the actual mutant output.
MUTATIONS = [
 ('constructor-wrong-flags', 'lifecycle', 'flags |= 2;', 'flags |= 4;', 'owner->flags == 2'),
 ('constructor-short-zero', 'lifecycle', 'memset(this, 0, sizeof(*this));', 'memset(this, 0, sizeof(*this) - 8);', 'owner->currentBase == 0 && owner->nextVmSlot == 0'),
 ('constructor-loaded-payload', 'lifecycle', 'loadedStageLists[stage].payload = this;', 'loadedStageLists[stage].payload = 0;', 'owner->loadedStageLists[i].payload == owner'),
 ('constructor-export-payload', 'lifecycle', 'exportStageLists[stage].payload = this;', 'exportStageLists[stage].payload = 0;', 'owner->exportStageLists[i].payload == owner'),
 ('constructor-no-publication', 'lifecycle', 'g_GameOwner477814 = this;', 'g_GameOwner477814 = 0;', 'g_GameOwner477814 == owner'),
 ('update-callback-address', 'lifecycle', 'AllocElem(OnUpdate)', 'AllocElem(OnDraw)', 'callback == (allocationCount == 0 ? HintOwnerView::OnUpdate : HintOwnerView::OnDraw)'),
 ('draw-callback-address', 'lifecycle', 'AllocElem(OnDraw)', 'AllocElem(OnUpdate)', 'callback == (allocationCount == 0 ? HintOwnerView::OnUpdate : HintOwnerView::OnDraw)'),
 ('registration-owner-argument', 'lifecycle', 'element->arg = this;', 'element->arg = 0;', 'element->arg == activeOwner', 2),
 ('registration-wrong-bit', 'lifecycle', 'element->flags &= ~2u;', 'element->flags &= ~4u;', 'element->flags == (allocatedFlags[0] & ~2u)', 2),
 ('draw-registration-owner-argument', 'lifecycle', 'element = th10::Chain::AllocElem(OnDraw);\n    element->arg = this;', 'element = th10::Chain::AllocElem(OnDraw);\n    element->arg = 0;', 'element->arg == activeOwner'),
 ('draw-registration-wrong-bit', 'lifecycle', 'element = th10::Chain::AllocElem(OnDraw);\n    element->arg = this;\n    element->flags &= ~2u;', 'element = th10::Chain::AllocElem(OnDraw);\n    element->arg = this;\n    element->flags &= ~4u;', 'element->flags == (allocatedFlags[1] & ~2u)'),
 ('calc-result-propagated', 'lifecycle', 'th10::Chain::AddToCalcChain(element, 25, th10::g_Chain);', 'if (th10::Chain::AddToCalcChain(element, 25, th10::g_Chain) != 0) return -11;', 'status == 0'),
 ('draw-result-propagated', 'lifecycle', 'th10::Chain::AddToDrawChain(element, 44, th10::g_Chain);', 'if (th10::Chain::AddToDrawChain(element, 44, th10::g_Chain) != 0) return -12;', 'status == 0'),
 ('calc-priority', 'lifecycle', 'AddToCalcChain(element, 25,', 'AddToCalcChain(element, 26,', 'priority == 25'),
 ('draw-priority', 'lifecycle', 'AddToDrawChain(element, 44,', 'AddToDrawChain(element, 45,', 'priority == 44'),
 ('update-chain-not-stored', 'lifecycle', 'updateChain = element;', 'updateChain = 0;', 'activeOwner->updateChain == &elements[0]'),
 ('draw-chain-not-stored', 'lifecycle', 'drawChain = element;', 'drawChain = 0;', 'owner.updateChain == &elements[0] && owner.drawChain == &elements[1]'),
 ('counter-not-reset', 'lifecycle', 'counter10 = 0;', 'counter10 += 0;', 'owner.counter10 == 0 && owner.counter14 == 81'),
 ('mode-one-not-loaded', 'lifecycle', 'hintMode != 0', 'hintMode == 2', 'Count(Load) == (mode == 0 ? 0 : 2)'),
 ('mode-zero-loaded', 'lifecycle', 'hintMode != 0', 'hintMode >= 0', 'Count(Load) == (mode == 0 ? 0 : 2)'),
 ('auto-loader-path', 'lifecycle', 'LoadHintFile(this, "hint/hint_auto.txt", 0);', 'LoadHintFile(this, "hint/wrong_auto.txt", 0);', 'std::strcmp(path, index == 0 ? "hint/hint_auto.txt" : "hint/hint_user.txt") == 0'),
 ('user-loader-path', 'lifecycle', 'LoadHintFile(this, "hint/hint_user.txt", 1);', 'LoadHintFile(this, "hint/wrong_user.txt", 1);', 'std::strcmp(path, index == 0 ? "hint/hint_auto.txt" : "hint/hint_user.txt") == 0'),
 ('auto-loader-mode', 'lifecycle', 'LoadHintFile(this, "hint/hint_auto.txt", 0);', 'LoadHintFile(this, "hint/hint_auto.txt", 1);', 'mode == index'),
 ('user-loader-mode', 'lifecycle', 'LoadHintFile(this, "hint/hint_user.txt", 1);', 'LoadHintFile(this, "hint/hint_user.txt", 0);', 'mode == index'),
 ('user-load-skipped', 'lifecycle', 'LoadHintFile(this, "hint/hint_user.txt", 1);', ';', 'Count(Load) == (mode == 0 ? 0 : 2)'),
 ('loader-result-propagated', 'lifecycle', 'LoadHintFile(this, "hint/hint_auto.txt", 0);', 'return LoadHintFile(this, "hint/hint_auto.txt", 0);', 'status == 0'),
 ('user-loader-result-propagated', 'lifecycle', 'LoadHintFile(this, "hint/hint_user.txt", 1);', 'return LoadHintFile(this, "hint/hint_user.txt", 1);', 'status == 0'),
 ('update-result-not-forwarded', 'lifecycle', 'return static_cast<HintOwnerView *>(owner)->Update();', 'static_cast<HintOwnerView *>(owner)->Update(); return 0;', 'elements[0].callback(elements[0].arg) == result'),
 ('draw-not-one', 'lifecycle', 'int __fastcall HintOwnerView::OnDraw(void *)\n{\n    return 1;', 'int __fastcall HintOwnerView::OnDraw(void *)\n{\n    return 2;', 'elements[1].callback(elements[1].arg) == 1'),
 ('update-unregister-skipped', 'lifecycle', 'th10::Chain *chain = th10::g_Chain;\n    if (element != NULL)', 'th10::Chain *chain = th10::g_Chain;\n    if (false)', 'element == expectedUnregister[unregisterCalls]'),
 ('draw-unregister-skipped', 'lifecycle', 'element = owner->drawChain;\n    chain = th10::g_Chain;\n    if (element != NULL)', 'element = owner->drawChain;\n    chain = th10::g_Chain;\n    if (false)', 'unregisterCalls == expectedUnregisterCount'),
 ('cleanup-lock-omitted', 'lifecycle', 'EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);', ';', 'criticalHeld', 2),
 ('cleanup-increment-omitted', 'lifecycle', '++g_MainSupervisorView.criticalSectionLockCounts[0];', ';', 'g_MainSupervisorView.criticalSectionLockCounts[0] == originalDepth + 1', 2),
 ('cleanup-unlock-omitted', 'lifecycle', 'LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);', ';', '!criticalHeld', 2),
 ('cleanup-decrement-omitted', 'lifecycle', '--g_MainSupervisorView.criticalSectionLockCounts[0];', ';', 'g_MainSupervisorView.criticalSectionLockCounts[0] == originalDepth', 2),
 ('decrement-before-unlock', 'lifecycle', 'LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);\n        --g_MainSupervisorView.criticalSectionLockCounts[0];', '--g_MainSupervisorView.criticalSectionLockCounts[0];\n        LeaveCriticalSection(&g_MainSupervisorView.criticalSections[0]);', 'g_MainSupervisorView.criticalSectionLockCounts[0] == originalDepth + 1', 2),
 ('second-chain-not-refreshed', 'lifecycle', 'element = owner->drawChain;\n    chain = th10::g_Chain;', 'element = owner->drawChain;', 'chain == chainBeforeEnter'),
 ('chain-captured-after-lock', 'lifecycle', 'EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);', 'EnterCriticalSection(&g_MainSupervisorView.criticalSections[0]);\n        chain = th10::g_Chain;', 'chain == chainBeforeEnter', 2),
 ('zero-id-marked', 'lifecycle', 'if (owner->vmIds[index].value != 0)', 'if (true)', 'markCalls < expectedMarkCount'),
 ('last-vm-slot-skipped', 'lifecycle', 'index < 10;', 'index < 9;', 'markCalls == expectedMarkCount'),
 ('ids-not-cleared', 'lifecycle', 'owner->vmIds[index].value = 0;', ';', 'activeOwner->vmIds[i].value == 0'),
 ('id-cleared-before-mark', 'lifecycle', 'if (owner->vmIds[index].value != 0)\n            manager->MarkVmForDeletion(owner->vmIds[index]);\n        owner->vmIds[index].value = 0;', 'AnmVmIdView saved = owner->vmIds[index];\n        owner->vmIds[index].value = 0;\n        if (saved.value != 0) manager->MarkVmForDeletion(saved);', 'activeOwner->vmIds[i].value == initialIds[i]'),
 ('vm-mark-reversed', 'lifecycle', 'int index = 0; index < 10; ++index', 'int index = 9; index >= 0; --index', 'id.value == expectedMarkIds[markCalls]'),
 ('vm-mark-omitted', 'lifecycle', 'manager->MarkVmForDeletion(owner->vmIds[index]);', ';', 'markCalls == expectedMarkCount'),
 ('manager-incorrectly-refreshed', 'lifecycle', 'manager->MarkVmForDeletion(owner->vmIds[index]);', 'g_AnmRenderManagerView->MarkVmForDeletion(owner->vmIds[index]);', 'this == expectedManager'),
 ('mode-one-exported', 'lifecycle', 'hintMode == 2', 'hintMode != 0', 'g_MainSupervisorView.hintMode == 2'),
 ('export-omitted', 'lifecycle', 'ExportAutomaticHints(owner, "hint/hint_auto.txt");', ';', 'exportCalls == (g_MainSupervisorView.hintMode == 2 ? 1 : 0)'),
 ('export-wrong-path', 'lifecycle', 'ExportAutomaticHints(owner, "hint/hint_auto.txt");', 'ExportAutomaticHints(owner, "hint/hint_user.txt");', 'std::strcmp(path, "hint/hint_auto.txt") == 0'),
 ('global-cleared-before-free', 'lifecycle', 'ClearHintLists(owner);\n    g_GameOwner477814 = NULL;', 'g_GameOwner477814 = NULL;\n    ClearHintLists(owner);', 'g_GameOwner477814 == owner'),
 ('global-not-cleared', 'lifecycle', 'g_GameOwner477814 = NULL;', ';', 'g_GameOwner477814 == 0'),
 ('cleanup-resets-chain-pointer', 'lifecycle', 'element = owner->drawChain;', 'owner->updateChain = NULL;\n    element = owner->drawChain;', 'owner.updateChain == update && owner.drawChain == draw'),
 ('factory-delete-omitted', 'lifecycle', 'delete owner;', ';', 'ownerDeleted && ownerDeleteCalls == 1'),
 ('factory-cleanup-omitted', 'lifecycle', '        CleanupHintOwner(owner);', ';', 'clearCalls == 1'),
 ('factory-delete-before-cleanup', 'lifecycle', 'CleanupHintOwner(owner);\n            delete owner;', 'delete owner;\n            CleanupHintOwner(owner);', 'clearCalls == 1'),
 ('factory-success-null', 'lifecycle', '    return owner;\n}', '    return NULL;\n}', 'result != 0 && result == activeOwner && g_GameOwner477814 == result'),
 ('lookup-zero-accepted', 'support', 'if (id.value == 0)', 'if (false)', 'manager.FindVm(AnmVmIdView(0)) == 0'),
 ('lookup-secondary-skipped', 'support', 'node = secondaryVmListHead;', 'node = NULL;', 'manager.FindVm(AnmVmIdView(202)) == &secondChild'),
 ('deletion-overwrites-flags', 'support', 'vm->flags35C |= deletionFlag;', 'vm->flags35C = deletionFlag;', 'vms[i].flags35C == (original | (deleted[i] ? 0x04000000u : 0u))'),
 ('deletion-no-root-children', 'support', 'if (vm->layerNode.previous != NULL)', 'if (true)', 'vms[i].flags35C == (original | (deleted[i] ? 0x04000000u : 0u))'),
 ('deletion-propagates-from-child', 'support', 'if (vm->layerNode.previous != NULL)', 'if (false)', 'vms[i].flags35C == (original | (deleted[i] ? 0x04000000u : 0u))'),
 ('deletion-wrong-flag', 'support', '0x04000000u', '0x02000000u', 'vms[i].flags35C == (original | (deleted[i] ? 0x04000000u : 0u))'),
]


def mutations(lifecycle, support):
    for spec in MUTATIONS:
        name, target, needle, replacement, expression, *counts = spec
        expected = counts[0] if counts else 1
        source = lifecycle if target == 'lifecycle' else support
        require(source.count(needle) == expected, '%s: anchor count %d, expected %d: %r' % (name, source.count(needle), expected, needle))
        candidate = source.replace(needle, replacement)
        yield name, candidate if target == 'lifecycle' else lifecycle, candidate if target == 'support' else support, expression
    load_pair = '        LoadHintFile(this, "hint/hint_auto.txt", 0);\n        LoadHintFile(this, "hint/hint_user.txt", 1);'
    require(lifecycle.count(load_pair) == 1, 'loader order mutation anchor changed')
    reversed_loads = '\n'.join(reversed(load_pair.splitlines()))
    yield 'loader-order-reversed', lifecycle.replace(load_pair, reversed_loads), support, 'std::strcmp(path, index == 0 ? "hint/hint_auto.txt" : "hint/hint_user.txt") == 0'
    swapped_lists = support.replace('node = primaryVmListHead;', 'node = secondaryVmListHead;', 1)
    original_second = swapped_lists.rfind('node = secondaryVmListHead;')
    require(original_second >= 0, 'lookup list order mutation anchor changed')
    swapped_lists = swapped_lists[:original_second] + swapped_lists[original_second:].replace('node = secondaryVmListHead;', 'node = primaryVmListHead;', 1)
    yield 'lookup-list-order-reversed', lifecycle, swapped_lists, 'manager.FindVm(AnmVmIdView(101)) == &root'
    manager_declaration = '    AnmRenderManagerView *manager = g_AnmRenderManagerView;\n'
    require(lifecycle.count(manager_declaration) == 1, 'manager capture mutation anchor changed')
    moved_manager = lifecycle.replace(manager_declaration, '')
    start = 'void __stdcall CleanupHintOwner(HintOwnerView *owner)\n{\n'
    require(moved_manager.count(start) == 1, 'cleanup start mutation anchor changed')
    yield 'manager-captured-before-chains', moved_manager.replace(start, start + manager_declaration), support, 'this == expectedManager'
    # Reorder complete call statements without inventing alternative cleanup code.
    anchor = '    if (g_MainSupervisorView.hintMode == 2)\n        ExportAutomaticHints(owner, "hint/hint_auto.txt");\n    ClearHintLists(owner);'
    require(lifecycle.count(anchor) == 1, 'export/free order mutation anchor changed')
    yield 'free-before-export', lifecycle.replace(anchor,
        '    ClearHintLists(owner);\n    if (g_MainSupervisorView.hintMode == 2)\n        ExportAutomaticHints(owner, "hint/hint_auto.txt");'), support, 'exportCalls == (g_MainSupervisorView.hintMode == 2 ? 1 : 0)'


def prepare(lifecycle, support):
    # Source mutation is already complete. Substitutions affect dependencies only.
    require(lifecycle.count('owner->Initialize()') == 1, 'factory Initialize dependency anchor changed')
    lifecycle = lifecycle.replace('owner->Initialize()', 'fixture::InitializeForFactory(owner)')
    # A deletion-omitted control has no delete seam; otherwise exactly one.
    require(lifecycle.count('delete owner;') <= 1, 'unexpected owner delete count')
    lifecycle = lifecycle.replace('delete owner;', 'fixture::DeleteOwnerObserved(owner);')
    require(support.count('void ClearHintLists(HintOwnerView *owner)') == 1, 'clear dependency anchor changed')
    support = support.replace('void ClearHintLists(HintOwnerView *owner)', 'void ClearHintListsActual(HintOwnerView *owner)')
    require(support.count('void AnmRenderManagerView::MarkVmForDeletion(AnmVmIdView id)') == 1, 'ANM mark dependency anchor changed')
    support = support.replace('void AnmRenderManagerView::MarkVmForDeletion(AnmVmIdView id)', 'void AnmRenderManagerView::MarkVmForDeletionActual(AnmVmIdView id)')
    deletion = 'delete static_cast<HintEntryView *>(node->payload);'
    require(support.count(deletion) == 2, 'entry delete dependency anchors changed')
    support = support.replace(deletion, 'fixture::DeleteEntryObserved(static_cast<HintEntryView *>(node->payload));')
    return lifecycle, support


def classify_mutation(result, expected_expression=None, fixture=None):
    require(result.returncode == 1, 'mutation did not exit 1 (got %r)' % result.returncode)
    require(result.stdout == '', 'mutation emitted unexpected stdout: %r' % result.stdout)
    match = DIAGNOSTIC.fullmatch(result.stderr)
    require(match is not None, 'mutation did not produce exactly one CHECK diagnostic: %r' % result.stderr)
    if expected_expression is not None:
        require(match.group(2) == expected_expression, 'wrong contract: %r (expected %r)' % (match.group(2), expected_expression))
    if fixture is not None:
        line = int(match.group(1)); lines = fixture.splitlines()
        require(line <= len(lines) and ('CHECK(' + match.group(2) + ')') in lines[line - 1],
                'diagnostic does not identify its actual fixture CHECK line')


def verify_classifier():
    good = subprocess.CompletedProcess([], 1, '', 'HINT_LIFECYCLE_CHECK_FAILED line 12: x == 3\n')
    classify_mutation(good)
    invalid = [(-11, '', ''), (0, 'HINT_LIFECYCLE_OK\n', ''), (1, '', ''),
        (1, '', 'generic assertion failed\n'), (1, '', good.stderr + 'AddressSanitizer: error\n'),
        (1, 'extra output\n', good.stderr), (1, '', good.stderr + good.stderr),
        (1, '', 'HINT_LIFECYCLE_CHECK_FAILED line 0: x\n')]
    for code, out, err in invalid:
        try: classify_mutation(subprocess.CompletedProcess([], code, out, err))
        except RuntimeError: continue
        raise RuntimeError('classifier accepted invalid control %r' % ((code, out, err),))
    for expected, fixture in [('wrong expression', None), ('x == 3', '\n' * 11 + 'CHECK(wrong);')]:
        try: classify_mutation(good, expected, fixture)
        except RuntimeError: continue
        raise RuntimeError('classifier accepted wrong expression/line control')


def run(root, compiler, selected, sanitize, skip_mutations, optimization='O1'):
    audit_integration(root)
    fixture = audit_dependencies(root)
    lifecycle, support = extract_sources(root)
    verify_classifier()
    env = dict(os.environ)
    # LeakSanitizer is unsupported under executor ptrace; no leak-proof claim.
    env['ASAN_OPTIONS'] = 'detect_leaks=0:halt_on_error=1'
    env['UBSAN_OPTIONS'] = 'halt_on_error=1:print_stacktrace=1'
    candidates = [('baseline', lifecycle, support, None)]
    if not skip_mutations: candidates += list(mutations(lifecycle, support))
    require(len({item[0] for item in candidates}) == len(candidates), 'duplicate mutation name')
    if selected:
        require(set(selected) <= {item[0] for item in candidates}, 'unknown requested mutation')
        candidates = [item for item in candidates if item[0] == 'baseline' or item[0] in selected]
    with tempfile.TemporaryDirectory(prefix='hint-lifecycle-') as temporary:
        work = Path(temporary)
        for name in ['Hint.hpp', 'Chain.hpp', 'AnmVmId.hpp']:
            shutil.copyfile(root / 'src' / name, work / name)
        (work / 'AnmManager.hpp').write_text('#pragma once\n// Audited dependency declarations precede Hint.hpp in fixture.cpp.\n')
        (work / 'fixture.cpp').write_text(fixture)
        flags = ['-std=c++17', '-' + optimization, '-g', '-fno-omit-frame-pointer', '-no-pie']
        if sanitize: flags += ['-fsanitize=address,undefined', '-fno-sanitize-recover=all']
        killed = 0
        for name, candidate, dependency, expression in candidates:
            candidate, dependency = prepare(candidate, dependency)
            (work / 'hint-lifecycle-source.inc').write_text(candidate)
            (work / 'hint-support-source.inc').write_text(dependency)
            build = subprocess.run([compiler, *flags, str(work / 'fixture.cpp'), '-o', str(work / 'fixture')],
                                   text=True, capture_output=True, timeout=90)
            require(build.returncode == 0, '%s COMPILE ERROR, not a kill:\n%s' % (name, build.stderr))
            result = subprocess.run([str(work / 'fixture')], text=True, capture_output=True, timeout=15, env=env)
            if name == 'baseline':
                require(result.returncode == 0 and result.stdout == 'HINT_LIFECYCLE_OK\n' and result.stderr == '',
                        'baseline failed: exit %r stdout %r stderr %r' % (result.returncode, result.stdout, result.stderr))
                print('PASS baseline actual-source lifecycle + ANM + list traversal' + (' (ASan/UBSan)' if sanitize else ''), flush=True)
            else:
                try: classify_mutation(result, expression, fixture)
                except RuntimeError as error: raise RuntimeError(name + ': ' + str(error)) from error
                killed += 1
                print('KILLED %s: %s' % (name, result.stderr.rstrip()), flush=True)
        print('PASS %d strict intended-CHECK controls; compile failures, signals, sanitizer and extra output are rejected' % killed)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source-root', type=Path, default=ROOT)
    parser.add_argument('--compiler', default=shutil.which('g++') or 'g++')
    choice = parser.add_mutually_exclusive_group()
    choice.add_argument('--mutation', action='append', help='run named mutation(s), plus baseline')
    choice.add_argument('--skip-mutations', action='store_true')
    parser.add_argument('--no-sanitize', action='store_true')
    parser.add_argument('--optimization', choices=['O0', 'O1', 'O2', 'O3'], default='O1')
    args = parser.parse_args()
    try:
        run(args.source_root.resolve(), args.compiler, args.mutation, not args.no_sanitize, args.skip_mutations, args.optimization)
    except (RuntimeError, OSError, subprocess.TimeoutExpired) as error:
        print('HINT_LIFECYCLE_HARNESS_ERROR: ' + str(error), file=sys.stderr)
        return 2
    return 0


if __name__ == '__main__':
    sys.exit(main())
