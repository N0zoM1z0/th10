#!/usr/bin/env python3
"""Bounded actual-source contracts for Player state2 and its automatic hint chain.

Extracts the three complete maintained bodies, real Hint entry constructor/link
append, timer SetCurrent, faith/rank helpers and VM delete propagation. The host
fixture projects Player/Hint/panel owners onto the fields used by these bodies;
it does not model target offsets, private ABI, allocator identity or native runtime.
Allocation and ANM/GUI boundaries are observable seams. Tests use live objects and
defined integer domains; signals, sanitizer errors and generic failures never count
as functional mutation detections. Native layout/bytes have separate Oracles.
"""
from pathlib import Path
import argparse
import hashlib
import importlib.util
import os
import re
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('hint_source_extract', ROOT / 'scripts/test-hint-lifecycle.py')
helpers = importlib.util.module_from_spec(spec)
spec.loader.exec_module(helpers)
balanced = helpers.balanced


def extract():
    src = ROOT / 'src'
    player = (src / 'Player.cpp').read_text()
    hint = (src / 'Hint.cpp').read_text()
    header = (src / 'Player.hpp').read_text()
    score = (src / 'GameScoreState.hpp').read_text()
    item = (src / 'ItemManager.cpp').read_text()
    hint_header = (src / 'Hint.hpp').read_text()
    panel = (src / 'EnemyGameState.hpp').read_text()
    for text in ('HintLinkView exportStageLists[8];', 'int count;', 'int remaining;', 'char text[68];'):
        assert text in hint_header, text
    for text in ('PlayerTimerView updateTimer1;', 'PlayerTimerView highlightTimer;', 'PlayerOptionRuntime options[4];'):
        assert text in header, text
    for text in ('AnmVmView frontVms[8];', 'AnmVmTimerView spellTimer;', 'unsigned int flags378C;'):
        assert text in panel, text
    declarations = '\n'.join([
        balanced(header, 'struct PlayerFloat3\n{', True),
        balanced((src / 'AnmManager.hpp').read_text(), 'struct AnmFloat3View\n{', True),
        balanced(score, 'struct GameScoreTimerView\n{', True),
        balanced(score, 'struct GameScoreStateView\n{', True),
        balanced(hint_header, 'struct HintLinkView\n{', True),
        balanced(hint_header, 'struct HintEntryView\n{', True),
    ])
    timer = balanced(header, 'void SetCurrent(int value)\n    {')
    support = '\n'.join([
        balanced(hint, 'HintEntryView::HintEntryView()'),
        balanced(hint, 'void HintLinkView::Append(HintLinkView *node)'),
        balanced(item, 'void GameScoreStateView::DecayFaith(int amount)'),
        balanced(item, 'void GameScoreStateView::AddRank(int amount)'),
        balanced(player, 'static void MarkPlayerVmDeleteState(unsigned int id, unsigned short state)'),
    ])
    bodies = '\n'.join([
        balanced(hint, 'HintEntryView *HintOwnerView::AddAutomaticEntry('),
        balanced(hint, 'void HintOwnerView::AddCaution('),
        balanced(player, 'void PlayerTransitionToState2(Player *player)\n{'),
    ])
    fixture = (ROOT / 'scripts/fixtures/player-state2.cpp').read_text()
    return fixture.replace('@DECLARATIONS@', declarations).replace('@TIMER@', timer).replace('@SUPPORT@', support).replace('@BODIES@', bodies), hashlib.sha256(bodies.encode()).hexdigest()


MUTATIONS = [
    ('third-vm', 'if (slot != 2)', 'if (slot != 3)', 'panel->frontVms[i].flags35C == expected'),
    ('remaining-zero', 'if (entry->remaining > 0)', 'if (entry->remaining >= 0)', 'old[1].remaining == 0'),
    ('text-bound', 'strncpy(entry->text, text, 64);', 'strncpy(entry->text, text, 65);', 'entry->text[64] == 0'),
    ('caution-count', 'entry->count -= 60;', 'entry->count -= 61;', 'entry->count == (base == 0 ? 60 : 1)'),
    ('faith-fraction', 'faith - 5000) / 3', 'faith - 5000) / 2', 'g_GameScoreState.faith == expectedFaith'),
    ('life-zero', 'if (lives >= 0)', 'if (lives > 0)', 'g_PlayerLivesDisplayCount==lives-1 && guiCalls==(lives>0)'),
    ('rank-step', 'AddRank(-0x400)', 'AddRank(-0x3ff)', 'g_GameScoreState.rank == expectedRank'),
]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--baseline-only', action='store_true')
    args = parser.parse_args()
    source, digest = extract()
    tests = [('baseline', source, None)]
    if not args.baseline_only:
        for label, before, after, expression in MUTATIONS:
            assert source.count(before) == 1, label
            tests.append((label, source.replace(before, after), expression))
    with tempfile.TemporaryDirectory(prefix='th10-player-state2-') as output:
        root = Path(output)
        for label, code, expected in tests:
            path = root / (label + '.cpp')
            binary = root / label
            path.write_text(code)
            flags = ['-O1', '-g', '-fsanitize=address,undefined', '-fno-omit-frame-pointer', '-no-pie'] if expected is None else ['-O2']
            compiled = subprocess.run(['c++', '-std=c++17', *flags, str(path), '-o', str(binary)], capture_output=True, text=True)
            assert compiled.returncode == 0, (label, compiled.stderr)
            result = subprocess.run([str(binary)], capture_output=True, text=True, timeout=30,
                env={**os.environ, 'ASAN_OPTIONS': 'detect_leaks=1:halt_on_error=1', 'UBSAN_OPTIONS': 'halt_on_error=1'})
            if expected is None:
                assert result.returncode == 0 and result.stdout == 'PASS player-state2 source contracts\n' and not result.stderr, (label, result.returncode, result.stdout, result.stderr)
            else:
                match = re.fullmatch(r'PLAYER_STATE2_CHECK_FAILED line [1-9][0-9]*: ([^\r\n]+)\n', result.stderr)
                assert result.returncode == 1 and not result.stdout and match and match[1] == expected, (label, result.returncode, result.stdout, result.stderr)
    print('PASS actual Player state2/hint contracts; ASan/UBSan/leaks; ' + str(len(tests)-1) + ' strict functional controls; bodies SHA256 ' + digest)


if __name__ == '__main__':
    main()
