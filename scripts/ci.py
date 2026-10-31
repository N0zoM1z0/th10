#!/usr/bin/env python3
"""Run lightweight exact-stage repository checks; select Oracle self-tests explicitly."""

from __future__ import annotations

import argparse
import os
from pathlib import Path
import subprocess
import sys


ROOT = Path(__file__).resolve().parents[1]


def tracked(pattern: str) -> list[str]:
    output = subprocess.run(
        ["git", "ls-files", "--cached", "--others", "--exclude-standard", "-z", pattern],
        cwd=ROOT,
        check=True,
        stdout=subprocess.PIPE,
    ).stdout
    return [raw.decode() for raw in output.split(b"\0") if raw and (ROOT / raw.decode()).is_file()]


def run(label: str, command: list[str]) -> None:
    print(f"\n==> {label}", flush=True)
    environment = os.environ.copy()
    environment["PYTHONPYCACHEPREFIX"] = str(ROOT / "build" / "pycache")
    subprocess.run(command, cwd=ROOT, env=environment, check=True)


def validate_public_tree() -> None:
    forbidden = {".exe", ".dll", ".dat", ".gpr", ".i64", ".id0", ".id1", ".id2", ".nam", ".til"}
    for relative in tracked("*"):
        if Path(relative).suffix.lower() in forbidden:
            raise ValueError(f"private binary or database is trackable: {relative}")


DEFAULT_CHECKS = {
    'Compile workflow Python': [sys.executable, "-m", "py_compile", *tracked("scripts/*.py")],
    'Validate documentation references': [sys.executable, "scripts/check-doc-hygiene.py"],
    'Validate ledgers': [sys.executable, "scripts/validate-tracking.py"],
    'Validate toolchain declarations': [sys.executable, "scripts/verify-toolchain.py", "--check"],
    'Validate canonical match graph': [sys.executable, "scripts/build-match-unit.py", "--check"],
    'Check generated progress': [sys.executable, "scripts/progress.py", "--check"],
    'Check whitespace': ["git", "diff", "--check"],
}

# Comparator self-tests are relevant when changing that Oracle, not for every owner.
ORACLE_CHECKS = {
    'Validate LTCG entry identity guards': [sys.executable, "scripts/test-ltcg-entry.py"],
    'Validate qualified static owners': [sys.executable, "scripts/test-static-owner-selection.py"],
    'Validate referenced static callee units': [sys.executable, "scripts/test-linked-static-callee-unit.py"],
    'Validate linked profiles and static callees': [sys.executable, "scripts/test-linked-context.py"],
    'Validate internal linked destinations': [sys.executable, "scripts/test-linked-local-fields.py"],
    'Validate linked table and selector coverage': [sys.executable, "scripts/test-linked-data-coverage.py"],
    'Validate ECL diagnostic source routing': [sys.executable, "scripts/test-ecl-run-source-selection.py"],
    'Validate allocation EH address binding': [sys.executable, "scripts/test-linked-allocation-eh.py"],
    'Validate stack-file EH address binding': [sys.executable, "scripts/test-linked-stack-file-eh.py"],
    'Validate worker-member EH address binding': [sys.executable, "scripts/test-linked-worker-member-eh.py"],
    'Validate chain-member EH address binding': [sys.executable, "scripts/test-linked-chain-member-eh.py"],
    'Validate Stage array EH address binding': [sys.executable, "scripts/test-linked-stage-array-eh.py"],
    'Validate Bullet array EH address binding': [sys.executable, "scripts/test-linked-bullet-array-eh.py"],
    'Validate Ending complete address inventory': [sys.executable, "scripts/test-ending-address-inventory.py"],
}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--only", action="append", metavar="STAGE",
                        help="run a named repository or Oracle check (repeatable); default: seven lightweight checks")
    args = parser.parse_args()
    available = {**DEFAULT_CHECKS, **ORACLE_CHECKS}
    labels = list(dict.fromkeys(args.only)) if args.only is not None else list(DEFAULT_CHECKS)
    try:
        unknown = set(labels) - available.keys()
        if unknown:
            raise ValueError(f"unknown CI stage(s): {sorted(unknown)}")
        validate_public_tree()
        for label in labels:
            run(label, available[label])
    except (OSError, ValueError, subprocess.CalledProcessError) as exc:
        print(f"error: CI validation failed: {exc}", file=sys.stderr)
        return 1
    print(f"\nTH10 exact-stage repository checks passed ({len(labels)} stages)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
