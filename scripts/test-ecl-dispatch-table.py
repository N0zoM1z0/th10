#!/usr/bin/env python3
"""Target-independent scope controls for the Enemy selector-table reporter."""

import importlib.util
from pathlib import Path
import sys
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    'th10_dispatch_table', ROOT / 'scripts/report-ecl-dispatch-table.py')
REPORT = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = REPORT
SPEC.loader.exec_module(REPORT)

ENUM = '''enum Opcode {
    ENEMY_ECL_A = 0x100,
    ENEMY_ECL_B = 0x101,
};
'''


class CaseScopeTests(unittest.TestCase):
    def parse(self, body):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / 'dispatcher.cpp'
            path.write_text(ENUM + body)
            return REPORT.parse_source(path)

    def method(self, cases):
        return ('int EnemyRuntimeView::DispatchEclInstruction() {\n'
                'switch (opcode) {\n' + cases + '\n} return 0;\n}')

    def test_current_source_has_124_outer_labels(self):
        values, names, problems = REPORT.parse_source(ROOT / 'src/EnemyEclDispatcher.cpp')
        self.assertEqual(problems, [])
        self.assertEqual(len(values), 124)
        self.assertEqual(len(names), 124)
        self.assertEqual(set(names), set(values))

    def test_nested_labels_do_not_duplicate_outer_labels(self):
        _, names, problems = self.parse(self.method('''
case ENEMY_ECL_A:
    switch (other) {
    case ENEMY_ECL_A: break;
    case ENEMY_ECL_B: break;
    }
    break;
case ENEMY_ECL_B: break;
'''))
        self.assertEqual(names, ['ENEMY_ECL_A', 'ENEMY_ECL_B'])
        self.assertEqual(problems, [])

    def test_nested_label_cannot_hide_missing_outer_label(self):
        _, names, problems = self.parse(self.method('''
case ENEMY_ECL_A:
    switch (other) { case ENEMY_ECL_B: break; }
    break;
'''))
        self.assertEqual(names, ['ENEMY_ECL_A'])
        self.assertIn('enum entries without cases: ENEMY_ECL_B', problems)

    def test_real_duplicate_and_unknown_outer_labels_fail(self):
        _, _, problems = self.parse(self.method('''
case ENEMY_ECL_A: break;
case ENEMY_ECL_A: break;
case ENEMY_ECL_B: break;
case ENEMY_ECL_UNKNOWN: break;
'''))
        self.assertIn('source has duplicate ECL case labels', problems)
        self.assertIn('cases without enum entries: ENEMY_ECL_UNKNOWN', problems)

    def test_non_switch_blocks_and_quoted_braces(self):
        _, names, problems = self.parse(self.method('''
case ENEMY_ECL_A:
    // switch (fake) { case ENEMY_ECL_A:
    const char *text = "} case ENEMY_ECL_A: {";
    char brace = '}';
    /* } switch (fake) { case ENEMY_ECL_B: */
    if (flag) { case ENEMY_ECL_B: break; }
'''))
        self.assertEqual(names, ['ENEMY_ECL_A', 'ENEMY_ECL_B'])
        self.assertEqual(problems, [])

    def test_missing_ambiguous_and_unbalanced_definitions_fail(self):
        for body in ['', self.method('') * 2, self.method('')[:-1]]:
            with self.subTest(body=body):
                _, _, problems = self.parse(body)
                self.assertTrue(problems)

    def test_multiple_outer_or_unbraced_switches_fail(self):
        bodies = [
            ('int EnemyRuntimeView::DispatchEclInstruction() {'
             'switch (opcode) { case ENEMY_ECL_A: break; }'
             'switch (other) { case ENEMY_ECL_B: break; }}'),
            ('int EnemyRuntimeView::DispatchEclInstruction() {'
             'switch (opcode) case ENEMY_ECL_A: break; }'),
        ]
        for body in bodies:
            with self.subTest(body=body):
                _, _, problems = self.parse(body)
                self.assertTrue(problems)


if __name__ == '__main__':
    unittest.main()
