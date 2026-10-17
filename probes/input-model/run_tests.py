#!/usr/bin/env python3
"""Self-contained finite C++03 model tests; does not rerun target proofs."""
import argparse
import hashlib
import json
import os
import shutil
import sys
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parent
FLAGS = ['-std=c++03', '-pedantic-errors', '-Wall', '-Wextra', '-Werror', '-O2']

def require(condition, message):
    if not condition:
        raise RuntimeError(message)

def canonical_text(path):
    text = path.read_bytes().decode('utf-8', errors='strict')
    require('\x00' not in text, 'NUL byte in text source: ' + str(path))
    text = text.replace('\r\n', '\n')
    require('\r' not in text, 'Unsupported bare CR in text source: ' + str(path))
    return text

def packaging_regression():
    # No source-tree changes: test a disposable, independently relocated copy.
    with tempfile.TemporaryDirectory(prefix='input-model-packaging-') as temporary:
        copied = Path(temporary) / 'input-model'
        shutil.copytree(ROOT, copied)
        converted = 0
        for path in copied.rglob('*'):
            if path.is_file() and path.suffix in ('.cpp', '.hpp', '.h'):
                path.write_bytes(canonical_text(path).replace('\n', '\r\n').encode('utf-8'))
                converted += 1
        command = [sys.executable, '-O', str(copied / 'run_tests.py')]
        baseline = subprocess.run(command, text=True, capture_output=True, cwd=temporary, timeout=300)
        require(baseline.returncode == 0, 'CRLF checkout suite failed: ' + baseline.stderr + baseline.stdout)
        result = json.loads(baseline.stdout)
        require(result['all_passed'] and result['baseline_passes'] == 6 and result['semantic_kills'] == 64,
                'CRLF checkout did not run the complete suite')
        source = copied / 'components' / 'key_masks.cpp'
        original = source.read_bytes()
        changed = original.replace(b'mask |= 0x0001;', b'mask |= 0x0002;', 1)
        require(changed != original, 'Packaging mutation anchor absent')
        source.write_bytes(changed)
        rejected = subprocess.run(command, text=True, capture_output=True, cwd=temporary, timeout=20)
        require(rejected.returncode != 0 and 'Checkpoint source hash mismatch: components/key_masks.cpp' in rejected.stderr,
                'Real source-byte change was not rejected by integrity gate')
        return {'scope': 'canonical_LF_packaging_regression', 'converted_cpp_files': converted,
                'crlf_baseline_passes': 6, 'crlf_semantic_kills': 64,
                'real_source_mutation_rejected': True, 'all_passed': True}

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--report', type=Path, help='Optional JSON result file; no file is required to run.')
    parser.add_argument('--check-packaging', action='store_true',
                        help='Run full suite in a CRLF copy, then verify source mutation rejection.')
    args = parser.parse_args()
    # Best-effort lower priority using Python alone; no external nice dependency.
    if hasattr(os, 'nice'):
        try:
            os.nice(10)
        except OSError:
            pass
    manifest_document = json.loads(canonical_text(ROOT / 'SOURCE_SHA256.json'))
    require(manifest_document['schema'] == 1 and manifest_document['hash_format'] == 'sha256_utf8_canonical_lf',
            'Unsupported source manifest format')
    manifest = manifest_document['files']
    for name, expected in manifest.items():
        path = ROOT / name
        require(path.is_file() and hashlib.sha256(canonical_text(path).encode('utf-8')).hexdigest() == expected,
                'Checkpoint source hash mismatch: ' + name)
    if args.check_packaging:
        report = packaging_regression()
        if args.report:
            args.report.write_text(json.dumps(report, indent=2) + '\n')
        print(json.dumps(report, indent=2))
        return 0
    suites = json.loads(canonical_text(ROOT / 'test_cases.json'))
    require(len(suites) == 6, 'Expected all six suites')
    compiler = subprocess.run(['g++', '--version'], text=True, capture_output=True, timeout=10)
    require(compiler.returncode == 0, 'g++ unavailable')
    report = {'scope': 'finite_host_model_tests_not_target_proofs_or_native_exactness',
              'compiler': compiler.stdout.splitlines()[0], 'flags': FLAGS,
              'source_hash_format': 'sha256_utf8_canonical_lf', 'source_sha256': manifest, 'results': []}
    owner_dependencies = ['components/key_masks.cpp', 'components/input_transition.cpp',
                          'components/button_masks.cpp', 'di_axes.cpp', 'winmm_axes.cpp']
    with tempfile.TemporaryDirectory(prefix='input-model-tests-') as temporary:
        build = Path(temporary)
        def run(suite, label, source, mutant):
            name = suite['name'] + '__' + label
            exe = build / name
            sources = [str(source)]
            if suite['name'] == 'owner':
                sources += [str(ROOT / p) for p in owner_dependencies]
            command = ['g++'] + FLAGS + ['-I', str(ROOT), '-I', str(ROOT / 'components'),
                       '-I', str(ROOT / 'tests')] + sources + [str(ROOT / suite['test']), '-o', str(exe)]
            try:
                compiled = subprocess.run(command, capture_output=True, text=True, timeout=30)
                if compiled.returncode:
                    return {'name': name, 'status': 'COMPILE_FAILURE_NOT_KILL', 'stderr': compiled.stderr}
                result = subprocess.run([str(exe)], capture_output=True, text=True, timeout=20)
            except subprocess.TimeoutExpired:
                return {'name': name, 'status': 'TIMEOUT_NOT_KILL'}
            if mutant:
                good = result.returncode == suite['mismatch_exit'] and result.stdout.startswith(suite['mismatch_prefix'])
                status = 'SEMANTIC_KILL' if good else 'SURVIVED_OR_CRASHED_NOT_KILL'
            else:
                expected = 'PASS cases=%d\n' % suite['cases']
                if suite['name'] == 'owner':
                    expected = 'PASS orchestration_cases=%d all_axes=IMPLEMENTED\n' % suite['cases']
                status = 'PASS' if result.returncode == 0 and result.stdout == expected else 'FAIL'
            return {'name': name, 'status': status, 'exit_code': result.returncode,
                    'stdout': result.stdout, 'stderr': result.stderr}
        for suite in suites:
            source = ROOT / suite['source']
            original = canonical_text(source)
            report['results'].append(run(suite, 'baseline', source, False))
            for label, old, new, occurrence_count, replacement_count in suite['mutations']:
                require(original.count(old) == occurrence_count, 'Mutation anchor: ' + suite['name'] + '/' + label)
                require(replacement_count > 0 and replacement_count <= occurrence_count, 'Mutation count')
                mutated = build / (suite['name'] + '__' + label + '.cpp')
                mutated.write_text(original.replace(old, new, replacement_count))
                report['results'].append(run(suite, label, mutated, True))
    report['baseline_passes'] = sum(x['status'] == 'PASS' for x in report['results'])
    report['semantic_kills'] = sum(x['status'] == 'SEMANTIC_KILL' for x in report['results'])
    report['all_passed'] = (report['baseline_passes'] == 6 and report['semantic_kills'] == 64
                            and len(report['results']) == 70)
    if args.report:
        args.report.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report, indent=2))
    return 0 if report['all_passed'] else 1

if __name__ == '__main__':
    raise SystemExit(main())
