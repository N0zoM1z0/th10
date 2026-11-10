#!/usr/bin/env python3
"""Check changed input syntax and whitespace with Python's standard library."""

import argparse
import ast
import csv
import json
import os
from pathlib import Path
import subprocess
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[1]


def check_file(path: Path) -> bool:
    if path.suffix == ".py":
        ast.parse(path.read_bytes(), filename=str(path.relative_to(ROOT)))
    elif path.suffix == ".json":
        json.loads(path.read_bytes())
    elif path.suffix == ".toml":
        with path.open("rb") as stream:
            tomllib.load(stream)
    elif path.suffix == ".csv":
        with path.open(encoding="utf-8", newline="") as stream:
            rows = csv.reader(stream, strict=True)
            width = len(next(rows, []))
            for row in rows:
                if row and len(row) != width:
                    raise ValueError(f"CSV line {rows.line_num}: expected {width} columns, got {len(row)}")
    elif path.suffix in ("", ".sh"):
        with path.open("rb") as stream:
            shebang = stream.readline()
        if path.suffix != ".sh" and shebang.strip() not in (
            b"#!/bin/bash", b"#!/usr/bin/env bash", b"#!/bin/sh", b"#!/usr/bin/env sh"
        ):
            return False
        shell = "sh" if shebang.strip() in (b"#!/bin/sh", b"#!/usr/bin/env sh") else "bash"
        subprocess.run([shell, "-n", str(path)], check=True)
    else:
        return False
    return True


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--base", metavar="REV", help="also check changes from REV to the current checkout (for CI)")
    args = parser.parse_args()
    try:
        base = args.base or "HEAD"
        if args.base and set(args.base) == {"0"}:
            # GitHub uses an all-zero before SHA for the first push of a branch.
            base = subprocess.check_output(
                ["git", "hash-object", "-t", "tree", "--stdin"], input=b"", cwd=ROOT
            ).decode().strip()
        names = set()
        for command in (
            ["git", "diff", "--name-only", "--diff-filter=ACMR", "-z", base, "--"],
            ["git", "ls-files", "--others", "--exclude-standard", "-z"],
        ):
            names.update(os.fsdecode(name) for name in subprocess.check_output(command, cwd=ROOT).split(b"\0") if name)
        checked = 0
        for name in sorted(names):
            path = ROOT / name
            if not path.is_file() or path.is_symlink():
                continue
            try:
                checked += check_file(path)
            except (OSError, UnicodeError, SyntaxError, ValueError, csv.Error) as error:
                print(f"error: {name}: {error}", file=sys.stderr)
                return 1
        # Check each local layer so unstaged edits cannot hide a staged error.
        for command in (
            ["git", "diff", "--check"],
            ["git", "diff", "--cached", "--check"],
            ["git", "diff", "--check", base, "--"],
        ):
            subprocess.run(command, cwd=ROOT, check=True)
    except (OSError, subprocess.CalledProcessError) as error:
        print(f"error: basic checks failed: {error}", file=sys.stderr)
        return 1
    print(f"Basic checks passed ({checked} changed machine-input files; whitespace OK).")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
