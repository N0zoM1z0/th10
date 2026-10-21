#!/usr/bin/env python3
"""Actual Chain dispatcher host contracts and strict faulty-code controls.

Uses the existing transition host builder without relaxing its canonical pins.
Mutations affect only generated harness copies. Native layout, private ABI,
Windows concurrency and whole-game runtime remain outside this host contract.
"""
from pathlib import Path
import argparse
import hashlib
import json
import subprocess
import tempfile
from transition_effect_host_test import ROOT, block, build, once, sha
from transition_effect_mutations import classify, self_checks


def controls():
    return [
        ('calc_restart', 'Calc', [('                case 6:\n                    goto restart;',
          '                case 6:\n                    break;')], 'CHAIN_RETRY_RESTART_VISITS'),
        ('draw_restart', 'Draw', [('    updatedCount = 0;\n    node = chain->DrawList()->next;',
          'restart_draw:\n    updatedCount = 0;\n    node = chain->DrawList()->next;'),
          ('                case 0:', '                case 6:\n                    goto restart_draw;\n                case 0:')],
          'CHAIN_RETRY_RESTART_VISITS'),
        ('calc_delete_missing', 'Calc', [('                    if (elem->deletedCallback != NULL)\n                        elem->deletedCallback(elem->arg);',
          '                    ; // Fault: omit the observed delete callback.')], 'CHAIN_CALC7_DRAW_DEFAULT_DELETE'),
        ('draw_delete_added', 'Draw', [('                case 0:',
          '                case 7:\n                    if (elem->deletedCallback != NULL)\n                        elem->deletedCallback(elem->arg);\n                    break;\n                case 0:')],
          'CHAIN_CALC7_DRAW_DEFAULT_DELETE'),
        ('retry_guard_bypassed', 'Calc', [('execute_again:\n            if (elem->flags & 2)\n            {',
          '            if (elem->flags & 2)\n            {\nexecute_again:')], 'CHAIN_ENABLED_AND_RETRY_RECHECK'),
        ('next_refreshed_after_callback', 'Calc', [('        node = next;\n    }\nloop_exit:',
          '        next = node->next;\n        node = next;\n    }\nloop_exit:')], 'CHAIN_CACHED_NEXT_SKIPS_NEW_NODE'),
        ('null_callback_counted', 'Calc', [('            ++updatedCount;\n        }\n        node = next;',
          '            ++updatedCount;\n        }\n        else\n            ++updatedCount;\n        node = next;')], 'CHAIN_NULL_DISABLED_COUNTS'),
        ('exit_keeps_previous_count', 'Calc', [('                case 4:\n                    updatedCount = 0;',
          '                case 4:\n                    updatedCount = updatedCount;')], 'CHAIN_FORCED_RESULT_REPLACES_COUNT'),
    ]


def run_suite(out):
    out.mkdir(parents=True, exist_ok=True)
    guards = self_checks()
    baseline = out / 'baseline'
    exe = build(ROOT / 'src/TransitionEffect.cpp', baseline)
    result = subprocess.run([str(exe), 'dispatch_statuses'], capture_output=True, text=True, timeout=15)
    if result.returncode != 0 or result.stderr or 'PASS:dispatch_statuses\n' not in result.stdout:
        raise RuntimeError('Unchanged actual-source dispatcher baseline failed: ' + result.stdout + result.stderr)
    manifest = json.loads((baseline / 'manifest.json').read_text())
    original = (baseline / 'harness.cpp').read_text()
    records = []
    for name, owner, edits, expected in controls():
        marker = 'int __stdcall MainRun' + owner + 'Chain(void *owner)'
        before = block(original, marker)
        changed = before
        for old, new in edits:
            changed = once(changed, old, new)
        if changed == before:
            raise RuntimeError('Unchanged control: ' + name)
        folder = out / name
        folder.mkdir(exist_ok=True)
        cpp = folder / 'harness.cpp'
        cpp.write_text(once(original, before, changed))
        command = list(manifest['command'])
        command[command.index(str(baseline / 'harness.cpp'))] = str(cpp)
        executable = folder / 'harness'
        command[command.index('-o') + 1] = str(executable)
        compiled = subprocess.run(command, capture_output=True, text=True, timeout=120)
        (folder / 'compile.stderr').write_text(compiled.stderr)
        if compiled.returncode:
            raise RuntimeError('Control compilation failed, not a detection: ' + name + '\n' + compiled.stderr)
        observed = subprocess.run([str(executable), 'dispatch_statuses'], capture_output=True, text=True, timeout=15)
        status = classify(True, observed.returncode, observed.stdout, observed.stderr, expected)
        records.append({'name': name, 'owner': marker, 'expected_assertion': expected,
                        'status': status, 'returncode': observed.returncode,
                        'stdout': observed.stdout, 'stderr': observed.stderr,
                        'before_sha256': hashlib.sha256(before.encode()).hexdigest(),
                        'after_sha256': hashlib.sha256(changed.encode()).hexdigest(),
                        'generated_source_sha256': sha(cpp)})
        if status != 'EXPECTED_ASSERTION':
            raise RuntimeError('Wrong failure or surviving control: ' + json.dumps(records[-1]))
    for name, digest in manifest['inputs'].items():
        if sha(Path(name)) != digest:
            raise RuntimeError('Maintained input changed during controls: ' + name)
    report = {'scope': __doc__, 'canonical_inputs': manifest['inputs'],
              'strict_classifier_guards': guards, 'baseline_returncode': result.returncode,
              'controls': records, 'runner_sha256': sha(Path(__file__))}
    (out / 'report.json').write_text(json.dumps(report, indent=2) + '\n')
    print('Actual-source Calc/Draw status contracts and eight strict faulty-code controls pass')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', type=Path)
    args = parser.parse_args()
    if args.out is not None:
        run_suite(args.out.resolve())
    else:
        parent = ROOT / 'build/host-tests'
        parent.mkdir(parents=True, exist_ok=True)
        with tempfile.TemporaryDirectory(prefix='chain-dispatch-controls-', dir=parent) as folder:
            run_suite(Path(folder))
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
