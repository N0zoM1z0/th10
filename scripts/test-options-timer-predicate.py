#!/usr/bin/env python3
"""Test the extracted production timer predicate on a host, under UBSan.

No binary, snapshot, controller mock, native layout or calling-convention proof.
Only the named helper's exact definition is extracted from --source. The tiny
field carrier supplies its inputs; expected results are independently computed
using Python's unbounded magnitude arithmetic (no remainder operator).
"""
import argparse
from pathlib import Path
import re
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
NAME = 'TimerReachedMultiple'
LOW, HIGH = -(2**31), 2**31 - 1


def extract(source):
    # Mask comments/literals without changing positions; match definitions only.
    masked = re.sub(r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',
                    lambda m: ''.join('\n' if c == '\n' else ' ' for c in m[0]), source)
    pattern = r'\bstatic\s+int\s+' + NAME + r'\s*\([^;{}]*\)\s*\{'
    hits = list(re.finditer(pattern, masked))
    if len(hits) != 1:
        raise ValueError(f'{NAME}: expected one definition, found {len(hits)}')
    start = hits[0].start()
    end, depth = hits[0].end(), 1
    while depth and end < len(masked):
        depth += (masked[end] == '{') - (masked[end] == '}')
        end += 1
    if depth:
        raise ValueError(f'{NAME}: unclosed definition')
    return source[start:end]


def cases():
    currents = sorted({LOW, LOW + 1, LOW + 3, HIGH - 3, HIGH - 1, HIGH,
                       *range(-65, 66), -120, 120})
    rows = []
    for current in currents:
        previous_values = {LOW, HIGH, -1, 0, 1, current}
        previous_values.update(p for p in (current - 1, current + 1) if LOW <= p <= HIGH)
        for previous in sorted(previous_values):
            for divisor in (1, -1, 4, -4, 60, -60, 0):
                changed = current != previous
                if changed and (divisor == 0 or (current == LOW and divisor == -1)):
                    continue  # Undefined C++ division is outside the tested domain.
                expected = 0
                if changed:
                    magnitude, step = abs(current), abs(divisor)
                    expected = int((magnitude // step) * step == magnitude)
                rows.append((previous, current, divisor, expected))
    return rows


def mutate(helper, pattern, replacement, name):
    result, count = re.subn(pattern, replacement, helper)
    if count != 1:
        raise ValueError(f'{name}: expected one mutation site, found {count}')
    return result.replace(NAME, name, 1)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, default=ROOT / 'src/FrontEnd.cpp')
    parser.add_argument('--cxx', default='g++')
    parser.add_argument('--mutations', action='store_true', help='also require three bad variants to fail')
    args = parser.parse_args()
    helper = extract(args.source.read_text())
    functions = [(NAME, helper)]
    if args.mutations:
        guard = r'timer\s*->\s*current\s*!=\s*timer\s*->\s*previous'
        functions += [
            ('DroppedGuard', mutate(helper, guard + r'\s*&&\s*', '', 'DroppedGuard')),
            ('ForwardOnly', mutate(helper, guard, 'timer->current > timer->previous', 'ForwardOnly')),
            ('InvertedRemainder', mutate(helper, r'==\s*0', '!= 0', 'InvertedRemainder')),
        ]
    rows = cases()
    program = '''#include <cstdio>
#include <cstdlib>
#include <climits>
static_assert(INT_MIN == (-2147483647-1) && INT_MAX == 2147483647, "requires 32-bit int");
struct AnmVmTimerView { int previous; int current; };
'''
    program += '\n'.join(body for _, body in functions)
    program += '\nstruct Case { int previous, current, divisor, expected; };\nCase cases[] = {\n'
    program += ',\n'.join('{' + ','.join(map(str, row)) + '}' for row in rows) + '\n};\n'
    program += 'using Fn = int (*)(const AnmVmTimerView*, int);\nFn functions[] = {'
    program += ','.join(name for name, _ in functions) + '};\n'
    program += '''int main(int argc, char** argv) {
    int index = argc > 1 ? std::atoi(argv[1]) : 0;
    if (index < 0 || index >= int(sizeof(functions)/sizeof(functions[0]))) return 2;
    for (const auto& c : cases) {
        AnmVmTimerView timer{c.previous, c.current};
        int actual = functions[index](&timer, c.divisor);
        if (actual != c.expected) {
            std::fprintf(stderr, "FAIL previous=%d current=%d divisor=%d expected=%d actual=%d\\n",
                         c.previous, c.current, c.divisor, c.expected, actual);
            return 1;
        }
    }
    return 0;
}
'''
    with tempfile.TemporaryDirectory(prefix='th10-timer-') as directory:
        cpp, executable = Path(directory) / 'test.cpp', Path(directory) / 'test'
        cpp.write_text(program)
        subprocess.run([args.cxx, '-std=c++17', '-O2', '-fsanitize=undefined',
                        '-fno-sanitize-recover=all', str(cpp), '-o', str(executable)], check=True)
        subprocess.run([str(executable), '0'], check=True)
        print(f'PASS extracted {NAME}: {len(rows)} defined cases under UBSan', flush=True)
        for index, (name, _) in enumerate(functions[1:], 1):
            result = subprocess.run([str(executable), str(index)], capture_output=True, text=True)
            if result.returncode != 1 or 'FAIL previous=' not in result.stderr:
                raise RuntimeError(f'{name}: expected explicit mismatch, got {result.returncode}: {result.stderr}')
            print(f'PASS mutation rejected: {name}: {result.stderr.strip()}', flush=True)


if __name__ == '__main__':
    main()
