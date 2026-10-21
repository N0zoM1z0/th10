#!/usr/bin/env python3
"""Execute actual Hint.cpp owner source with audited, instrumented dependencies.

This host fixture proves bounded source contracts, not target execution, original
class layout on this host, native FP control word, registration ABI, or codegen
identity. Hint.hpp, Chain.hpp, AnmVmId.hpp, GameEffectUpdate.hpp and the source
from // Target419120. through Update's closing brace are copied into an isolated build, together with
the source-extracted genuine entry and owner constructors. The only Hint source
substitution maps the single delete entry; dependency to a live observer that
checks/snapshots the entry before invoking the matching real delete. Mutations
apply first, so relocated deletion is observed at its relocated site. The
actual AnmFloat3View and AnmVmLayerNodeView declarations are extracted from the
audited ANM header. FindVm is extracted from the maintained implementation with
one lookup-entry observation inserted; its primary/secondary searches are real.
Large ANM/Player headers are replaced by audited signature/field seams.

No draw format is expanded; no failed-VM/null dereference, negative array index,
overflowing count/3*A, or invalid float-to-integer conversion is executed. Every
mutation must compile and produce exactly exit 1 plus one well-formed CHECK
failure line. A compiler failure, signal, timeout, sanitizer report, generic exit
1 or forged diagnostic with extra output is a harness error, never a kill.
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
MARKER = '// Target419120.'
DIAGNOSTIC = re.compile(r'HINT_FRAME_CHECK_FAILED line ([1-9][0-9]*): ([^\r\n]+)\n\Z')


# Each mutation must fail its intended contract, not merely any assertion.
EXPECTED_FAILURES = {
 'old-vm-self-flag-omitted': 'watchedVms[i]->flags35C == watchedFlags[i]',
 'old-vm-nonroot-children-marked': 'watchedVms[i]->flags35C == watchedFlags[i]',
 'old-vm-root-gate-reversed': 'watchedVms[i]->flags35C == watchedFlags[i]',
 'old-vm-child-flag-omitted': 'watchedVms[i]->flags35C == watchedFlags[i]',
 'old-vm-only-first-child': 'watchedVms[i]->flags35C == watchedFlags[i]',
 'old-vm-clobbered-flags': 'watchedVms[i]->flags35C == watchedFlags[i]',
'zero-id-mark-not-skipped': 'Count(Lookup) == (scenario == 0 ? 1 : 2)',
 'zero-id-lookup-not-skipped': 'Count(Lookup) == 0',
 'delete-before-unlink': 'entry->link.next == 0',
 'unsigned-slot-modulo': 'owner.nextVmSlot == -1',
 'cached-slot-after-create': 'owner.vmIds[1].value == 0 && owner.vmIds[2].value == 100',
 'cached-slot-after-scale': 'owner.cachedWidths[2] == -4002 && owner.cachedWidths[3] == 40',
 'cached-slot-after-text': 'owner.nextVmSlot == 5',
 'medium-recomputed-after-create': 'Vm(100)->glyphWidth == expectedGlyphs[i]',
 'manager-not-refreshed-after-create': 'Nth(Lookup, 1).receiver == &alternateManagers[0]',
 'manager-always-refreshed-after-scale': 'Nth(TextCenter).receiver == (i == 1 ? &alternateManagers[1] : '
                                         '&manager)',
 'manager-not-refreshed-after-scale': 'Nth(TextCenter).receiver == (i == 1 ? &alternateManagers[1] : '
                                      '&manager)',
 'manager-not-refreshed-after-text': 'Nth(Lookup, 2).receiver == &alternateManagers[2]',
 'manager-not-refreshed-after-delete': 'Nth(Lookup, 2).receiver == &alternateManagers[3]',
 'cached-next-not-used': 'held->count == 1',
 'old-id-not-cleared': 'owner->vmIds[mustBeClearedSlot].value == 0',
 'fade-rounded-instead-of-truncated': 'Vm(61)->primaryColor.alpha == test.expected255',
 'interior-alpha-shift-before-byte': 'Vm(61)->primaryColor.alpha == expected',
 'unsigned-fade-quotient': 'Vm(61)->primaryColor.alpha == expected',
 'reverse-scale-vectors': 'EqualFloat(Nth(Scale).initial.x, initial)',
 'wrong-small-script-base': 'Nth(Create).value == expectedScript',
 'wrong-large-script-base': 'Nth(Create).value == expectedScript',
 'safe-format-substitution': 'Nth(kind).pointer == reinterpret_cast<std::uintptr_t>(entry->text)',
 'invalid-alignment-centered': 'Count(TextLeft) + Count(TextRight) + Count(TextCenter) == 0',
 'base-gated-emission': 'Count(Create) == 3',
 'unsigned-count': 'Count(Create) == 3',
 'remaining-decrement': 'due->remaining == 29 && already->remaining == 29 && negative->remaining == 29',
 'unconditional-free': 'allowDelete',
 'copied-final-alpha': 'vm->primaryColor.alpha == 0',
 'inclusive-proximity': 'Vm(61)->primaryColor.alpha == test.expected255',
 'wrong-position-cache': 'owner.cachedPositions[9][0] == (alignment == 0 ? 101 : alignment == 1 ? 121 : 81)',
 'wrong-width-cache': 'owner.cachedWidths[9] == 40',
 'skip-y-mutation': 'EqualFloat(Nth(Create).position.y, expectedY)',
 'wrong-small-glyph': 'vm->glyphWidth == glyph && vm->glyphHeight == glyph',
 'wrong-scale-duration': 'Nth(Scale).duration == 8 && Nth(Scale).mode == 4',
 'wrong-scale-mode': 'Nth(Scale).duration == 8 && Nth(Scale).mode == 4',
 'wrong-time': 'vm->intVar0 == 120 && vm->intVar1 == 255',
 'signed-entry-alpha': 'vm->intVar0 == 120 && vm->intVar1 == 255',
 'missing-id-not-cleared': 'owner.vmIds[3].value == 0 && owner.vmIds[8].value == 0',
 'interpolation-not-skipped': 'Vm(200 + i)->primaryColor.alpha == ((i == 4 || i == 5) ? 171 : i)',
 'negative-interpolation-not-skipped': 'Vm(200 + i)->primaryColor.alpha == ((i == 4 || i == 5) ? 171 : i)',
 'wrong-stage-lists': 'selectedRecord.deleted',
 'no-counter10-increment': 'owner.counter10 == 1 && owner.counter14 == 1',
 'no-counter14-reset': 'owner.counter10 == 41 && owner.counter14 == 1',
 'current-base-written': 'owner.currentBase == 6',
 'wrong-slot-count': 'Vm(200 + i)->primaryColor.alpha == ((i == 4 || i == 5) ? 171 : i)',
 'dy-store-omitted': 'Vm(61)->primaryColor.alpha == 64',
 'adjusted-player-stored': 'Vm(61)->primaryColor.alpha == 64',
 'height-rounded': 'Vm(61)->primaryColor.alpha == 63',
 'outer-width-store-omitted': 'Vm(61)->primaryColor.alpha == 255'}

def require(condition, message):
    if not condition:
        raise RuntimeError(message)


def extract_function(source, signature):
    require(source.count(signature) == 1, 'Missing or duplicate function: ' + signature)
    start = source.index(signature)
    opening = source.find('{', start)
    require(opening != -1, 'Missing function body: ' + signature)
    depth, cursor = 1, opening + 1
    while depth:
        require(cursor < len(source), 'Unclosed function body: ' + signature)
        depth += (source[cursor] == '{') - (source[cursor] == '}')
        cursor += 1
    return source[start:cursor]


def declaration(source, start):
    offset = source.index(start)
    brace = source.index('{', offset)
    depth, end = 1, brace + 1
    while depth:
        require(end < len(source), 'unbalanced declaration: ' + start)
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    require(source[end] == ';', 'declaration does not end in semicolon')
    return source[offset:end + 1]


def normalize(text):
    return re.sub(r'\s+', '', text)


def audit_dependencies(root):
    canonical = root / 'src'
    anm = (canonical / 'AnmManager.hpp').read_text()
    player = (canonical / 'Player.hpp').read_text()
    expected = [
        'void StartScaleInterpolation(int duration, unsigned char mode, const AnmFloat2View *initial, const AnmFloat2View *final);',
        'AnmVmIdView CreateVmAtWorldVariant0(int scriptIndex, const AnmFloat3View *position);',
        'AnmVmView *FindVm(AnmVmIdView id);',

        'void DrawTextLeft(AnmVmView *vm, unsigned int color, const char *format, ...);',
        'void DrawTextRight(AnmVmView *vm, unsigned int color, const char *format, ...);',
        'void DrawTextCentered(AnmVmView *vm, unsigned int color, const char *format, ...);',
    ]
    compact = normalize(anm.replace('TH10_ANM_NOINLINE ', ''))
    fixture = (root / 'scripts/fixtures/hint-frame.cpp').read_text()
    for signature in expected:
        require(normalize(signature) in compact, 'audited ANM signature changed: ' + signature)
        require(normalize(signature) in normalize(fixture), 'fixture signature changed: ' + signature)
    for field in ['float scaleY;', 'float spriteHeight;', 'AnmVmAlphaInterpolationView primaryAlphaInterpolation;',
                  'AnmColorView primaryColor;', 'int intVar0;', 'int intVar1;', 'AnmFloat3View position;',
                  'AnmFloat3View positionOffset;', 'unsigned int flags35C;',
                  'unsigned char glyphWidth;', 'unsigned char glyphHeight;']:
        require(normalize(field) in compact, 'audited ANM field changed: ' + field)
    require('PlayerFloat3drawPosition;' in normalize(player), 'audited Player.drawPosition type changed')
    require('externPlayer*g_Player;' in normalize(player), 'audited Player global changed')
    require('externAnmRenderManagerView*g_AnmRenderManagerView;' in compact, 'audited ANM global changed')
    require('intid;' in normalize(declaration(anm, 'struct AnmVmView\n')), 'audited VM id changed')
    require('AnmVmLayerNodeViewlayerNode;' in compact, 'audited layer node changed')
    require('AnmVmLayerNodeView*primaryVmListHead;' in compact, 'audited primary list changed')
    require('AnmVmLayerNodeView*secondaryVmListHead;' in compact, 'audited secondary list changed')
    find = extract_function((canonical / 'AnmManager.cpp').read_text(),
        'AnmVmView *AnmRenderManagerView::FindVm(AnmVmIdView id)')
    find = find.replace('{', '{\n    fixture::Record(fixture::Lookup, id.value, 0, this);', 1)
    return (fixture.replace('@ANM_FLOAT3@', declaration(anm, 'struct AnmFloat3View\n'))
            .replace('@ANM_LAYER_NODE@', declaration(anm, 'struct AnmVmLayerNodeView\n'))
            .replace('@ANM_FIND_VM@', find))


def checked_mutant(name, source, needle, replacement, expected=1):
    found = source.count(needle)
    require(found == expected, '%s: mutation anchor count %d, expected %d: %r' % (name, found, expected, needle))
    return name, source.replace(needle, replacement)


def mutations(source):
    # This list is intentionally lexical and fail-closed. Source evolution must
    # update a mutation explicitly, not silently turn it into an ineffective test.
    specs = [
        ('old-vm-self-flag-omitted', 'oldVm->flags35C |= 0x04000000u;', 'oldVm->flags35C |= 0u;'),
        ('old-vm-nonroot-children-marked', 'if (oldVm->layerNode.previous == NULL)', 'if (true)'),
        ('old-vm-root-gate-reversed', 'oldVm->layerNode.previous == NULL', 'oldVm->layerNode.previous != NULL'),
        ('old-vm-child-flag-omitted', 'static_cast<AnmVmView *>(child->owner)->flags35C |= 0x04000000u;', 'static_cast<AnmVmView *>(child->owner)->flags35C |= 0u;'),
        ('old-vm-only-first-child', 'child = child->next;', 'child = NULL;'),
        ('old-vm-clobbered-flags', 'oldVm->flags35C |= 0x04000000u;', 'oldVm->flags35C = 0x04000000u;'),
        ('zero-id-mark-not-skipped', 'if (vmIds[slot].value != 0)', 'if (true)'),
        ('zero-id-lookup-not-skipped', 'if (vmIds[slot].value == 0)\n            continue;', 'if (false)\n            continue;'),
        ('unsigned-slot-modulo', '(nextVmSlot + 1) % 10', 'static_cast<unsigned int>(nextVmSlot + 1) % 10'),
        ('cached-slot-after-create', 'slot = nextVmSlot;\n            vmIds[slot] = created;', 'vmIds[slot] = created;', 3),
        ('cached-slot-after-scale', 'slot = nextVmSlot;\n        if (entry->alignment', 'if (entry->alignment'),
        ('cached-slot-after-text', '(nextVmSlot + 1) % 10', '(slot + 1) % 10'),
        ('medium-recomputed-after-create', 'unsigned char glyphSize = static_cast<unsigned char>(\n                static_cast<int>(static_cast<double>(entry->scale) * 15.0));', 'unsigned char glyphSize = entry->scale <= 2.0f ? static_cast<unsigned char>(\n                static_cast<int>(static_cast<double>(entry->scale) * 15.0)) : 30;'),
        ('manager-not-refreshed-after-create', 'manager = g_AnmRenderManagerView;\n            vm = manager->FindVm', 'vm = manager->FindVm', 3),
        ('manager-always-refreshed-after-scale', '        slot = nextVmSlot;\n        if (entry->alignment', '        manager = g_AnmRenderManagerView;\n        slot = nextVmSlot;\n        if (entry->alignment'),
        ('manager-not-refreshed-after-scale', 'vm->StartScaleInterpolation(8, 4, &initial, &final);\n            manager = g_AnmRenderManagerView;', 'vm->StartScaleInterpolation(8, 4, &initial, &final);'),
        ('manager-not-refreshed-after-text', '            manager = g_AnmRenderManagerView;\n        }\n        vm->primaryColor', '        }\n        vm->primaryColor'),
        ('manager-not-refreshed-after-delete', 'delete entry;\n            manager = g_AnmRenderManagerView;', 'delete entry;'),
        ('cached-next-not-used', '        node = next;\n    }\n    return 0;', '        node = entry->link.next;\n    }\n    return 0;'),
        ('old-id-not-cleared', '        vmIds[slot].value = 0;\n        entry->position.y', '        entry->position.y'),
        ('fade-rounded-instead-of-truncated', 'static_cast<unsigned char>(static_cast<int>(\n                alpha -', 'static_cast<unsigned char>(std::lround(\n                alpha -', 2),
        ('interior-alpha-shift-before-byte', 'static_cast<unsigned char>(alpha) >> 2', 'static_cast<unsigned char>(alpha >> 2)'),
        ('unsigned-fade-quotient', '(3 * alpha) / 4', 'static_cast<unsigned int>(3 * alpha) / 4', 2),
        ('reverse-scale-vectors', 'StartScaleInterpolation(8, 4, &initial, &final)', 'StartScaleInterpolation(8, 4, &final, &initial)', 3),
        ('wrong-small-script-base', 'nextVmSlot + 2, &entry->position', 'nextVmSlot + 3, &entry->position'),
        ('wrong-large-script-base', 'nextVmSlot + 12, &entry->position', 'nextVmSlot + 13, &entry->position', 2),
        ('safe-format-substitution', 'vm, 0x00FFFFFF, entry->text)', 'vm, 0x00FFFFFF, "%s", entry->text)', 3),
        ('invalid-alignment-centered', 'entry->alignment >= 0 && entry->alignment <= 2', 'true'),
        ('base-gated-emission', 'if (entry->count > 0)', 'if (entry->count > 0 || (entry->base != 0 && entry->base != g_GameFrameRestorePending))'),
        ('unsigned-count', 'if (entry->count > 0)', 'if (static_cast<unsigned int>(entry->count) > 0)'),
        ('remaining-decrement', 'vm->intVar0 = entry->time;', '--entry->remaining;\n        vm->intVar0 = entry->time;'),
        ('unconditional-free', 'if (freeEntries != 0)', 'if (true)'),
        ('copied-final-alpha', 'vm->primaryColor.alpha = 0;', 'vm->primaryColor.alpha = entry->alpha;'),
        ('inclusive-proximity', 'dx < w && dy < h)', 'dx <= w && dy <= h)'),
        ('wrong-position-cache', 'vm->positionOffset.', 'vm->position.', 3),
        ('wrong-width-cache', 'cachedWidths[slot] = entry->textWidth;', 'cachedWidths[slot] = entry->scale;'),
        ('skip-y-mutation', 'static_cast<double>(entry->scale) * 8.0 + entry->position.y', 'entry->position.y'),
        ('wrong-small-glyph', 'vm->glyphWidth = vm->glyphHeight = 15;', 'vm->glyphWidth = vm->glyphHeight = 14;'),
        ('wrong-scale-duration', 'StartScaleInterpolation(8, 4, &initial, &final)', 'StartScaleInterpolation(7, 4, &initial, &final)', 3),
        ('wrong-scale-mode', 'StartScaleInterpolation(8, 4, &initial, &final)', 'StartScaleInterpolation(8, 3, &initial, &final)', 3),
        ('wrong-time', 'vm->intVar0 = entry->time;', 'vm->intVar0 = entry->remaining;'),
        ('signed-entry-alpha', 'vm->intVar1 = entry->alpha;', 'vm->intVar1 = static_cast<signed char>(entry->alpha);'),
        ('missing-id-not-cleared', 'if (vm == 0)\n        {\n            vmIds[slot].value = 0;\n            continue;', 'if (vm == 0)\n        {\n            continue;'),
        ('interpolation-not-skipped', 'if (vm->primaryAlphaInterpolation.duration != 0)', 'if (false)'),
        ('negative-interpolation-not-skipped', 'vm->primaryAlphaInterpolation.duration != 0', 'vm->primaryAlphaInterpolation.duration > 0'),
        ('wrong-stage-lists', 'ConsumeEntries(loadedStageLists[g_GameStartupMode].next, 1)', 'ConsumeEntries(exportStageLists[g_GameStartupMode].next, 1)'),
        ('no-counter10-increment', '++counter10;', 'counter10 += 0;'),
        ('no-counter14-reset', 'counter14 = 0;', 'counter14 += 0;'),
        ('current-base-written', 'counter14 = 0;', 'counter14 = 0, currentBase = g_GameFrameRestorePending;'),
        ('wrong-slot-count', 'slot < 10;', 'slot < 9;'),
        ('dy-store-omitted', 'const float dy = static_cast<float>(fabs(playerY - cachedPositions[slot][1]));', 'const double dy = fabs(playerY - cachedPositions[slot][1]);'),
        ('adjusted-player-stored', 'const double playerX =', 'const float playerX ='),
        ('height-rounded', 'const double h =', 'const float h ='),
        ('outer-width-store-omitted', 'const float outerW = static_cast<float>(static_cast<double>(w) + 32.0);', 'const double outerW = static_cast<double>(w) + 32.0;'),
    ]
    # Relocate the actual delete expression before unlink, rather than accepting a
    # crash from freed traversal. The observation seam fails while the object lives.
    require(source.count('            delete entry;') == 1, 'delete relocation anchor changed')
    early_delete = source.replace('            delete entry;', '')
    early_delete = early_delete.replace('        if (entry->link.next != 0)',
        '        if (freeEntries != 0) delete entry;\n        if (entry->link.next != 0)')
    yield 'delete-before-unlink', early_delete
    for spec in specs:
        yield checked_mutant(spec[0], source, *spec[1:])


def classify_mutation(result, expected_expression=None, fixture=None):
    require(result.returncode == 1, 'mutation did not exit 1 (got %r)' % result.returncode)
    require(result.stdout == '', 'mutation emitted unexpected stdout: %r' % result.stdout)
    match = DIAGNOSTIC.fullmatch(result.stderr)
    require(match is not None, 'mutation did not produce exactly one CHECK diagnostic: %r' % result.stderr)
    if expected_expression is not None:
        require(match.group(2) == expected_expression,
                'mutation failed the wrong contract: %r (expected %r)' % (match.group(2), expected_expression))
    if fixture is not None:
        line = int(match.group(1))
        lines = fixture.splitlines()
        require(line <= len(lines) and ('CHECK(' + match.group(2) + ')') in lines[line - 1],
                'diagnostic does not identify its actual fixture CHECK line')


def verify_classifier():
    good = subprocess.CompletedProcess([], 1, '', 'HINT_FRAME_CHECK_FAILED line 12: x == 3\n')
    classify_mutation(good)
    invalid = [
        (-11, '', ''), (0, 'HINT_FRAME_OK\n', ''), (1, '', ''),
        (1, '', 'generic assertion failed\n'),
        (1, '', good.stderr + 'AddressSanitizer: error\n'),
        (1, 'extra output\n', good.stderr),
        (1, '', 'HINT_FRAME_CHECK_FAILED line 0: x\n'),
        (1, '', good.stderr + good.stderr),
    ]
    for code, out, err in invalid:
        try:
            classify_mutation(subprocess.CompletedProcess([], code, out, err))
        except RuntimeError:
            continue
        raise RuntimeError('classifier accepted invalid control %r' % ((code, out, err),))


def run(root, compiler, selected, sanitize, skip_mutations=False, optimization='O1'):
    source = (root / 'src/Hint.cpp').read_text()
    require(source.count(MARKER) == 1, 'expected exactly one Hint owner source marker')
    update = extract_function(source, 'int HintOwnerView::Update()')
    frame_end = source.index(update) + len(update)
    require(frame_end > source.index(MARKER), 'Hint frame source order changed')
    source = '\n\n'.join(extract_function(source, signature) for signature in [
        'HintEntryView::HintEntryView()', 'HintOwnerView::HintOwnerView()'
    ]) + '\n\n' + source[source.index(MARKER):frame_end]
    fixture = audit_dependencies(root)
    verify_classifier()
    env = dict(os.environ)
    # LeakSanitizer cannot run under this executor's ptrace supervision.
    # Address/undefined/float-cast checks remain enabled; no leak-proof claim.
    env['ASAN_OPTIONS'] = 'detect_leaks=0:halt_on_error=1'
    env['UBSAN_OPTIONS'] = 'halt_on_error=1:print_stacktrace=1'
    with tempfile.TemporaryDirectory(prefix='hint-frame-') as temporary:
        work = Path(temporary)
        for name in ['Hint.hpp', 'Chain.hpp', 'GameEffectUpdate.hpp']:
            shutil.copyfile(root / 'src' / name, work / name)
        shutil.copyfile(root / 'src/AnmVmId.hpp', work / 'AnmVmId.hpp')
        (work / 'AnmManager.hpp').write_text('#pragma once\n// Audited declaration seams live in fixture.cpp before Hint.hpp.\n')
        (work / 'Player.hpp').write_text('#pragma once\n// Audited Player.drawPosition seam lives in fixture.cpp.\n')
        (work / 'fixture.cpp').write_text(fixture)
        flags = ['-std=c++17', '-' + optimization, '-g', '-fno-omit-frame-pointer', '-fno-fast-math', '-ffp-contract=off', '-no-pie']
        if sanitize:
            flags += ['-fsanitize=address,undefined,float-cast-overflow', '-fno-sanitize-recover=all']
        candidates = [('baseline', source)]
        if not skip_mutations:
            controls = list(mutations(source))
            require(set(name for name, _ in controls) == set(EXPECTED_FAILURES), 'mutation/expected-contract registry differs')
            candidates += controls
        if selected:
            candidates = [item for item in candidates if item[0] == 'baseline' or item[0] in selected]
            require(all(any(name == item for name, _ in candidates) for item in selected), 'unknown requested mutation')
        killed = []
        for name, candidate in candidates:
            require(candidate.count('delete entry;') == 1, name + ': expected one delete dependency seam')
            prepared = candidate.replace('delete entry;', 'fixture::DeleteEntryObserved(entry);')
            (work / 'hint-frame-source.inc').write_text(prepared)
            build = subprocess.run([compiler, *flags, str(work / 'fixture.cpp'), '-o', str(work / 'fixture')],
                                   text=True, capture_output=True, timeout=90)
            require(build.returncode == 0, '%s COMPILE ERROR, not a mutation kill:\n%s' % (name, build.stderr))
            result = subprocess.run([str(work / 'fixture')], text=True, capture_output=True, timeout=15, env=env)
            if name == 'baseline':
                require(result.returncode == 0 and result.stdout == 'HINT_FRAME_OK\n' and result.stderr == '',
                        'baseline failed: exit %r stdout %r stderr %r' % (result.returncode, result.stdout, result.stderr))
                print('PASS baseline actual-source fixture' + (' (ASan/UBSan/float-cast-overflow)' if sanitize else ''), flush=True)
            else:
                try:
                    classify_mutation(result, EXPECTED_FAILURES[name], fixture)
                except RuntimeError as error:
                    raise RuntimeError('%s: %s' % (name, error)) from error
                killed.append(name)
                print('KILLED %s: %s' % (name, result.stderr.rstrip()), flush=True)
        print('PASS %d strict functional mutation controls; classifier rejects signals, sanitizer/extra output, and generic failures' % len(killed))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source-root', type=Path, default=ROOT)
    parser.add_argument('--compiler', default=shutil.which('g++') or 'g++')
    selection = parser.add_mutually_exclusive_group()
    selection.add_argument('--mutation', action='append', help='run only named mutation(s), plus baseline')
    selection.add_argument('--skip-mutations', action='store_true', help='run only the actual-source baseline fixture')
    parser.add_argument('--no-sanitize', action='store_true', help='omit sanitizers (all strict CHECK requirements remain)')
    parser.add_argument('--optimization', choices=['O0', 'O1', 'O2'], default='O1')
    args = parser.parse_args()
    try:
        run(args.source_root.resolve(), args.compiler, args.mutation, not args.no_sanitize, args.skip_mutations, args.optimization)
    except (RuntimeError, OSError, subprocess.TimeoutExpired) as error:
        print('HINT_FRAME_HARNESS_ERROR: %s' % error, file=sys.stderr)
        return 2
    return 0


if __name__ == '__main__':
    sys.exit(main())
