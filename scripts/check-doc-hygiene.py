#!/usr/bin/env python3
"""Protect the maintained TH10 docs from stale scratch links and broken indexes."""

from __future__ import annotations

from collections import Counter
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[1]
KB = ROOT / "docs" / "KNOWLEDGE_BASE.md"
ID = re.compile(r"^\|\s*([A-Z]+-[0-9]+[A-Z]?)\s*\|")
SCRATCH = re.compile(r"(?<![A-Za-z0-9_])(?:\.analysis|build)/[^\s`|]+")
LINK = re.compile(r"\]\(([^)]+)\)")


def validate() -> list[str]:
    errors = []
    rows = [line for line in KB.read_text().splitlines() if ID.match(line)]
    ids = [ID.match(line).group(1) for line in rows]
    duplicates = sorted(key for key, count in Counter(ids).items() if count > 1)
    if duplicates:
        errors.append(f"duplicate knowledge IDs: {duplicates}")
    for line in rows:
        if SCRATCH.search(line):
            errors.append(f"{ID.match(line).group(1)} cites disposable scratch")
    defined = set(ids)
    prefixes = {name.split("-", 1)[0] for name in defined}
    for line in rows:
        owner = ID.match(line).group(1)
        for name in set(re.findall(r"\b[A-Z]+-[0-9]{2,3}[A-Z]?\b", line)):
            if name != owner and name not in defined and name.split("-", 1)[0] in prefixes:
                errors.append(f"{owner}: missing knowledge cross-reference {name}")
    for relative in ("README.md", "docs/RE_HANDOFF.md", "docs/KNOWLEDGE_BASE.md"):
        path = ROOT / relative
        for dest in LINK.findall(path.read_text()):
            if dest.startswith(("http:", "https:", "#", "mailto:")):
                continue
            if not (path.parent / dest.split("#", 1)[0]).exists():
                errors.append(f"{relative}: missing link {dest}")
    return errors


if __name__ == "__main__":
    problems = validate()
    for error in problems:
        print("error:", error, file=sys.stderr)
    if problems:
        raise SystemExit(1)
    print("TH10 documentation IDs, scratch references and local links OK")
