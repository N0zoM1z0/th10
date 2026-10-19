"""Read-only checks; main guard keeps import free of execution."""
import argparse
from pathlib import Path
import difflib
import unittest
from unittest.mock import patch
import importlib.util
from reconstruct import reconstruct, OLD, NEW, CPP, sha
from model import index, confirm
BASELINE = None
PACKAGE = Path(__file__).resolve().parent

class Checks(unittest.TestCase):
    def test_raw_inverse_and_patch(self):
        generated = reconstruct(BASELINE)
        original = (BASELINE / 'src/FrontEnd.cpp').read_bytes()
        candidate = generated['src/FrontEnd.cpp']
        expected = {'src/FrontEnd.cpp': 'dfa7d15b5bc1f11605385c1c31e9f883751ae5162c4a8dc2aa4826aa5b6daa58',
                    'src/FrontEnd.hpp': '8b4e0e13312bce0b9bab71e16daff79cfa402d55a61e55cca53bd359edc7832b'}
        for path, data in generated.items():
            self.assertEqual(sha(data), expected[path])
        inverse = candidate.replace(NEW, OLD, 1)
        self.assertEqual(inverse, original)
        self.assertEqual(sha(inverse), CPP)
        old_expr = b'g_ReplayCharacter * 3 + 0x3f +'
        new_expr = b'(g_ReplayCharacter * 3 + 0x3f) * 2 +'
        self.assertEqual(candidate, original.replace(old_expr, new_expr, 1))
        self.assertEqual(len(candidate) - len(original), 6)
        self.assertNotIn(b'\n', candidate.replace(b'\r\n', b''))
        self.assertEqual(b''.join(difflib.diff_bytes(difflib.unified_diff,
            original.splitlines(keepends=True), candidate.splitlines(keepends=True),
            fromfile=b'baseline/src/FrontEnd.cpp', tofile=b'candidate/src/FrontEnd.cpp')),
            (PACKAGE / 'candidate.patch').read_bytes())
        replay = b'        controller->replayListOffset = 0;\r\n        break;\r\n'
        self.assertIn(replay, candidate)  # No earlier Replay overlay.

    def test_bad_input(self):
        with self.assertRaises(ValueError):
            reconstruct('.')
        with patch.object(Path, 'read_bytes', side_effect=[b'bad cpp', b'bad header']):
            with self.assertRaisesRegex(ValueError, 'pin mismatch'):
                reconstruct(BASELINE)
        for c, s in ((-1, 0), (2, 0), (0, -1), (0, 3), (2**31, 0)):
            with self.assertRaises(ValueError):
                index(c, s, True)

    def test_six_arguments(self):
        rows = [(c,s,index(c,s,False),index(c,s,True)) for c in (0,1) for s in (0,1,2)]
        self.assertEqual(rows, [(0,0,63,126),(0,1,64,127),(0,2,65,128),
                                (1,0,66,132),(1,1,67,133),(1,2,68,134)])

    def test_wrong_parenthesization(self):
        for c in (0,1):
            for s in (0,1,2):
                right = index(c,s,True)
                self.assertNotEqual(right, 2*c*3+0x3f+s)
                if s:
                    self.assertNotEqual(right, 2*(c*3+0x3f+s))
                else:
                    self.assertEqual(right, 2*(c*3+0x3f+s))

    def test_mutating_callbacks_keep_captures(self):
        for c in (0,1):
            for s in (0,1,2):
                for proposed in (False,True):
                    callbacks = []
                    def on_state(state):
                        callbacks.append('state')
                        state.update(character=1-c, selection=(s+1)%3)
                    def on_sound(state):
                        callbacks.append('sound')
                        state.update(character=1-c, selection=(s+2)%3)
                    state, events = confirm(c,s,proposed,on_state,on_sound)
                    arg = index(c,s,proposed)
                    self.assertEqual(callbacks, ['state','sound'])
                    self.assertEqual(events, [('capture_root',c+0x96),('capture_short_argument',arg),
                        ('SetScreenState','CONFIRMED'),('FrontEndPlaySound','SELECT'),
                        ('InterruptChildVm',c+0x96,arg,6)])
                    self.assertNotEqual(arg, index(state['character'],state['selection'],proposed))
                    self.assertNotEqual(c+0x96,state['character']+0x96)

    def test_import_no_source_io(self):
        with patch.object(Path,'read_bytes',side_effect=AssertionError('import read')), \
             patch.object(Path,'write_bytes',side_effect=AssertionError('import write')):
            for name in ('reconstruct','model','test_candidate'):
                spec = importlib.util.spec_from_file_location(name,PACKAGE/(name+'.py'))
                spec.loader.exec_module(importlib.util.module_from_spec(spec))

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--baseline-root',type=Path,required=True)
    BASELINE = parser.parse_args().baseline_root
    unittest.main(argv=['test_candidate.py'],verbosity=1)
