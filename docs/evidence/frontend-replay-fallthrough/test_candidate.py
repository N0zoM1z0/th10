"""Read-only source tests plus abstract model checks; nothing executes on import."""
from pathlib import Path
import argparse
import difflib
import unittest
from unittest.mock import patch
import importlib.util
from reconstruct import reconstruct, OLD, NEW, REMOVED, sha256
from model import step

BASELINE = None
PACKAGE = Path(__file__).resolve().parent
VALUES = (-(2**31), -1, 0, 1, 5, 6, 7, 8, 2**31 - 1)

class CandidateTests(unittest.TestCase):
    def test_source_exact_and_inverse(self):
        generated = reconstruct(BASELINE)
        original = (BASELINE / 'src/FrontEnd.cpp').read_bytes()
        candidate = generated['src/FrontEnd.cpp']
        expected_hashes = {
            'src/FrontEnd.cpp': 'd50c95e092e8eba62f77ee25f2abf16a812ac1e5f21708edadcb1cb54c2681f7',
            'src/FrontEnd.hpp': '8b4e0e13312bce0b9bab71e16daff79cfa402d55a61e55cca53bd359edc7832b',
        }
        for name, data in generated.items():
            self.assertEqual(sha256(data), expected_hashes[name])
        self.assertEqual(candidate.replace(NEW, OLD, 1), original)
        offset = original.index(OLD) + OLD.index(REMOVED)
        self.assertEqual(candidate, original[:offset] + original[offset + len(REMOVED):])
        self.assertEqual(len(original) - len(candidate), 16)
        self.assertNotIn(b'\n', candidate.replace(b'\r\n', b''))
        raw_diff = b''.join(difflib.diff_bytes(difflib.unified_diff,
            original.splitlines(keepends=True), candidate.splitlines(keepends=True),
            fromfile=b'baseline/src/FrontEnd.cpp', tofile=b'candidate/src/FrontEnd.cpp'))
        self.assertEqual(raw_diff, (PACKAGE / 'candidate.patch').read_bytes())

    def test_pin_mismatch_and_relative_root(self):
        with self.assertRaises(ValueError):
            reconstruct(Path('.'))
        with patch.object(Path, 'read_bytes', side_effect=[b'wrong cpp', b'wrong hpp']):
            with self.assertRaisesRegex(ValueError, 'pin mismatch'):
                reconstruct(BASELINE)

    def test_import_has_no_source_io(self):
        # Loading module definitions must not touch candidate/baseline files.
        with patch.object(Path, 'read_bytes', side_effect=AssertionError('import read')), \
             patch.object(Path, 'write_bytes', side_effect=AssertionError('import write')), \
             patch.object(Path, 'read_text', side_effect=AssertionError('import read')), \
             patch.object(Path, 'write_text', side_effect=AssertionError('import write')):
            for name in ('reconstruct', 'model', 'test_candidate'):
                spec = importlib.util.spec_from_file_location(name, PACKAGE / (name + '.py'))
                module = importlib.util.module_from_spec(spec)
                spec.loader.exec_module(module)

    def test_reset_without_mutation(self):
        for initial in VALUES:
            a = step('INITIALIZE', initial, False)
            b = step('INITIALIZE', initial, True)
            self.assertEqual((a.phase, a.timer, a.offset), ('OPENING', 0, 0))
            self.assertEqual((b.phase, b.timer, b.offset), ('OPENING', 0, 0))
            self.assertEqual(b.events, a.events + ['opening_predicate'])

    def test_mock_mutation_before_predicate(self):
        for value in VALUES:
            def mutation(s):
                self.assertEqual(s.events[-1], 'chdir:parent')
                self.assertNotIn('opening_predicate', s.events)
                s.timer = value
                s.events.append('mock_timer_mutation')
            a = step('INITIALIZE', -1, False, mutation)
            b = step('INITIALIZE', -1, True, mutation)
            self.assertEqual((a.phase, a.timer), ('OPENING', value))
            expected = ('SELECT_FILE', 0) if value > 6 else ('OPENING', value)
            self.assertEqual((b.phase, b.timer), expected)
            suffix = ['opening_predicate'] + (['SetScreenState:SELECT_FILE'] if value > 6 else [])
            self.assertEqual(b.events, a.events + suffix)
            self.assertEqual(a.events[-2:], ['mock_timer_mutation', 'replayListOffset=0'])

    def test_init_event_order_and_counts(self):
        for missing in (False, True):
            for count in (0, 1, 24, 25):
                a = step('INITIALIZE', 7, False, vm_missing=missing, user_count=count)
                b = step('INITIALIZE', 7, True, vm_missing=missing, user_count=count)
                expected = ['cursor.count=25', 'cursor.SetCurrent', 'saved_cursor=0', 'ResolveVm']
                if missing:
                    expected += ['CreateVm:5e', 'CreateVmVariant0:8']
                expected += ['CreateVm:65', 'SetScreenState:OPENING', 'clear_replay_owners']
                expected += ['numbered_load:%d' % n for n in range(1, 26)]
                expected += ['mkdir:replay', 'chdir:replay', 'FindFirstFile']
                for n in range(count):
                    expected += ['chdir:parent', 'user_load:%d' % (25+n), 'chdir:replay']
                    if n < 24:
                        expected += ['FindNextFile']
                expected += ['FindClose', 'chdir:parent', 'replayListOffset=0']
                self.assertEqual(a.events, expected)
                self.assertEqual(b.events, expected + ['opening_predicate'])
                self.assertEqual(a.events.count('FindClose'), 1)
                self.assertEqual(a.events.count('SetScreenState:OPENING'), 1)
                self.assertEqual(a.events.count('FindNextFile'), min(count, 24))

    def test_other_states_unchanged(self):
        for phase in ('OPENING', 'SELECT_FILE', 'SELECT_STAGE', 'STARTING', 'CLOSING', 'unknown'):
            for timer in VALUES:
                self.assertEqual(step(phase, timer, False), step(phase, timer, True))
                if phase == 'OPENING':
                    expected = ('SELECT_FILE', 0) if timer > 6 else ('OPENING', timer)
                    result = step(phase, timer, True)
                    self.assertEqual((result.phase, result.timer), expected)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--baseline-root', required=True, type=Path)
    args = parser.parse_args()
    BASELINE = args.baseline_root
    unittest.main(argv=['test_candidate.py'], verbosity=1)
