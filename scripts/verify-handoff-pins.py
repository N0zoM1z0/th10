#!/usr/bin/env python3
"""Fast live-source and target identity check for the next-agent handoff.

Read-only, no compiler calls, no full historical replay. Run from the repo
root with `scripts/repo-python scripts/verify-handoff-pins.py`.
"""
from __future__ import annotations

import hashlib
from pathlib import Path
import re
import tomllib

ROOT = Path(__file__).resolve().parent.parent
HANDOFF = ROOT / "docs/RE_HANDOFF.md"
PROGRESS = ROOT / "docs/PROGRESS.md"
TARGET_MANIFEST = ROOT / "config/target.toml"
TARGET = ROOT / "resources/th10.exe"


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def main() -> int:
    document = HANDOFF.read_text(encoding="utf-8")
    if "## Current-source proofs and candidate epoch pins" not in document:
        raise SystemExit("handoff lost its stable source pins section")
    table = document.split("## Current-source proofs and candidate epoch pins", 1)[1]
    table = table.split("\n## ", 1)[0]
    pins = re.findall(r"^\| `(src/[^`]+)` \| `([0-9a-f]{64})` \|$", table, re.M)
    if len(pins) < 20 or len({name for name, _ in pins}) != len(pins):
        raise SystemExit(f"missing/duplicate SHA pins: found {len(pins)}, expected >=20")
    changed = []
    for name, expected in pins:
        file_path = ROOT / name
        value = sha256(file_path) if file_path.is_file() else "MISSING"
        if value != expected:
            changed.append(f"{name}: handoff={expected}, current={value}")
    with TARGET_MANIFEST.open("rb") as stream:
        target_config = tomllib.load(stream)["target"]
    target_value = sha256(TARGET)
    if target_value != target_config["sha256"] or target_value not in document:
        changed.append("original Japanese executable SHA-256 mismatch")
    counts = PROGRESS.read_text(encoding="utf-8")
    measures = {}
    for label in ("Canonical exact functions", "Canonical exact codegen bytes"):
        match = re.search(r"^\| " + re.escape(label) + r" \| ([\d,]+) \|$", counts, re.M)
        if match is None:
            changed.append(f"generated progress is missing {label}")
        else:
            measures[label] = match.group(1)
    if len(measures) == 2:
        total = (f"{measures['Canonical exact functions']} complete exact functions / "
                 f"{measures['Canonical exact codegen bytes']} original code bytes")
        if total not in document:
            changed.append("handoff exact total differs from generated progress")
    if changed:
        raise SystemExit("Handoff source-epoch checks FAILED:\n" + "\n".join(changed))
    print(f"Handoff OK: {len(pins)} installed source SHA pins, original target, exact ledger")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
