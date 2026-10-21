#!/usr/bin/env python3
"""Actual-source contracts for opcode55 raw-word NEG, including negative-zero bits.

The exact case and union are extracted. The host stack seam checks tags/order and
projects the first dword of initialized host storage; its unused pointer member
may make that storage wider than i386. This does not prove native union layout,
VMStack conversion/underflow, private ABI or TH10 runtime. Native compilation has
separate complete-byte/field checks. Sanitizer/crash/compile failures never count
as functional mutation detections.
"""
from pathlib import Path
import argparse
import hashlib
import importlib.util
import os
import re
import subprocess
import tempfile

if not __debug__:
    raise SystemExit('Refusing optimized Python: assertion gates are required')

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('ecl_neg_source_extract', ROOT / 'scripts/test-hint-lifecycle.py')
helpers = importlib.util.module_from_spec(spec)
spec.loader.exec_module(helpers)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, default=ROOT / 'src/EclVm.cpp')
    parser.add_argument('--baseline-only', action='store_true')
    args = parser.parse_args()
    source = args.source.resolve()
    source.relative_to(ROOT)
    fixture = ROOT / 'scripts/fixtures/ecl-raw-negation.cpp'
    pins = {path: hashlib.sha256(path.read_bytes()).hexdigest() for path in (source, fixture)}
    text = source.read_text()
    union = helpers.balanced(text, 'union EclVmScalar\n{', True)
    arm = helpers.balanced(text, 'case ECL_VM_NEGATE_FLOAT_STORAGE:\n            {')
    generated = fixture.read_text().replace('@UNION@', union).replace('@ARM@', arm)
    tests = [('baseline', generated, None)]
    if not args.baseline_only:
        for label, before, after, failure in [
            ('sign-bit', 'value.bits = 0u - value.bits;', 'value.bits ^= 0x80000000u;', 'word_negate'),
            ('complement', 'value.bits = 0u - value.bits;', 'value.bits = ~value.bits;', 'word_negate'),
            ('integer-tag', "stack.Push('f', sizeof(value), &value);", "stack.Push('i', sizeof(value), &value);", 'push_float_tag'),
        ]:
            assert arm.count(before) == 1, label
            tests.append((label, generated.replace(before, after), failure))
    output = ROOT / 'build/host-tests'
    output.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='ecl-raw-negation-', dir=output) as temporary:
        directory = Path(temporary)
        for label, code, expected in tests:
            path = directory / (label + '.cpp')
            binary = directory / label
            path.write_text(code)
            flags = ['-O1', '-g', '-fsanitize=address,undefined', '-fno-sanitize-recover=all', '-fno-omit-frame-pointer', '-no-pie'] if expected is None else ['-O2']
            built = subprocess.run(['c++', '-std=c++17', *flags, str(path), '-o', str(binary)], capture_output=True, text=True, timeout=60)
            assert built.returncode == 0, (label, built.stderr)
            result = subprocess.run([str(binary)], capture_output=True, text=True, timeout=30,
                env={**os.environ, 'ASAN_OPTIONS': 'detect_leaks=1:halt_on_error=1', 'UBSAN_OPTIONS': 'halt_on_error=1'})
            if expected is None:
                assert result.returncode == 0 and result.stdout == 'PASS ECL raw negation\n' and not result.stderr, (label, result.returncode, result.stdout, result.stderr)
            else:
                match = re.fullmatch(r'ECL_RAW_NEG_CHECK_FAILED ([a-z_]+)\n', result.stderr)
                assert result.returncode == 1 and not result.stdout and match and match[1] == expected, (label, result.returncode, result.stdout, result.stderr)
    assert all(hashlib.sha256(path.read_bytes()).hexdigest() == digest for path, digest in pins.items()), 'source or fixture changed during test'
    print('PASS ECL raw NEG actual-source arm; 17 edge payloads/34 invocations; ASan/UBSan/leaks; ' + str(len(tests)-1) + ' strict functional controls; arm SHA256 ' + hashlib.sha256(arm.encode()).hexdigest())


if __name__ == '__main__':
    main()
