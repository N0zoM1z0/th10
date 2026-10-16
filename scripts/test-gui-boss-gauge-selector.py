#!/usr/bin/env python3
"""Test the maintained HUD boss-gauge selector against target-derived cases.

Synthetic host execution only: no game execution or exact-codegen claim.
Target 414DFD selects stage-1 with unsigned range check <=6; table 4157E0
maps stages 1..7 to 414E14/24/38/4C/63/6A/71. Signed CMP state,24
controls stages 1..4 and 7. Stage 7 adds 0x8C to SETGE, not 0x8D.
"""
from pathlib import Path
import argparse
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / 'src/Gui.cpp'


def function_source():
    source = SOURCE.read_text()
    marker = '// HUD boss-gauge stage selector begin.'
    if marker in source:
        start = source.index(marker) + len(marker)
        end = source.index('// HUD boss-gauge stage selector end.', start)
        return ('static int BossGaugeScript() {\n' + source[start:end] +
                '\nreturn script;\n}')
    start = source.index('static int BossGaugeScript()')
    brace = source.index('{', start)
    depth, end = 1, brace + 1
    while depth:
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[start:end]


# Expectations are literal native branch results, independent of source math.
PROGRAM = r'''
#include <climits>
#include <cstdio>
static int g_GuiStage, g_GuiMessageState;
@HELPER@
int main() {
    const int stages[] = {INT_MIN, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, INT_MAX};
    const int states[] = {INT_MIN, -1, 0, 22, 23, 24, 25, INT_MAX};
    const int low[] = {0x85,0x85,0x85,0x85,-1,-1,0x89,0x8B,0x8C,0x8C,0x85,0x85};
    const int high[] = {0x85,0x85,0x85,0x86,0x87,0x88,0x8A,0x8B,0x8C,0x8D,0x85,0x85};
    for (unsigned i=0;i<sizeof(stages)/sizeof(*stages);++i) {
        for (unsigned j=0;j<sizeof(states)/sizeof(*states);++j) {
            g_GuiStage=stages[i]; g_GuiMessageState=states[j];
            const int expected = j<5 ? low[i] : high[i];
            const int actual = BossGaugeScript();
            if (actual != expected) {
                std::fprintf(stderr,"stage=%d state=%d expected=%d actual=%d\n",
                             g_GuiStage,g_GuiMessageState,expected,actual);
                return 1;
            }
        }
    }
    return 0;
}
'''


class BossGaugeSelector(unittest.TestCase):
    def run_helper(self, helper):
        compiler = shutil.which('c++')
        if compiler is None:
            self.skipTest('host C++ compiler unavailable')
        with tempfile.TemporaryDirectory(prefix='th10-gui-selector-') as folder:
            source = Path(folder) / 'selector.cpp'
            executable = Path(folder) / 'selector'
            source.write_text(PROGRAM.replace('@HELPER@', helper))
            built = subprocess.run([compiler, '-std=c++11', '-O0', '-fsanitize=undefined',
                                    '-fno-sanitize-recover=undefined', str(source),
                                    '-o', str(executable)], capture_output=True, text=True)
            self.assertEqual(built.returncode, 0, built.stdout + built.stderr)
            return subprocess.run([str(executable)], capture_output=True, text=True)

    def test_actual_source_all_stages_and_signed_boundaries(self):
        result = self.run_helper(function_source())
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

    def test_mutation_controls(self):
        helper = function_source()
        replacements = [
            ('stage7-base', '0x8d : 0x8c', '0x8e : 0x8d'),
            ('threshold', '> 0x17', '> 0x18'),
            ('signedness', 'g_GuiMessageState', 'static_cast<unsigned int>(g_GuiMessageState)'),
        ]
        for name, old, new in replacements:
            with self.subTest(mutation=name):
                self.assertIn(old, helper, 'mutation no longer applies; revise the control')
                result = self.run_helper(helper.replace(old, new))
                self.assertEqual(result.returncode, 1, result.stdout + result.stderr)
                self.assertNotIn('runtime error:', result.stderr)
                self.assertIn('expected=', result.stderr)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, default=SOURCE)
    args, rest = parser.parse_known_args()
    SOURCE = args.source
    unittest.main(argv=[__file__] + rest)
