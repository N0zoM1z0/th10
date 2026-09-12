#!/usr/bin/env python3
"""Build one target-bound TH10 normal-COFF exact-comparison unit."""

from __future__ import annotations

import argparse
from pathlib import Path
import subprocess
import sys
import tomllib


ROOT = Path(__file__).resolve().parents[1]
MANIFEST = ROOT / "config" / "match-units.toml"
TARGET = ROOT / "config" / "target.toml"


def load() -> dict[str, object]:
    with TARGET.open("rb") as stream:
        target = tomllib.load(stream)["target"]
    with MANIFEST.open("rb") as stream:
        manifest = tomllib.load(stream)
    if manifest.get("schema_version") != 1:
        raise ValueError("match-units.toml must use schema_version = 1")
    if str(manifest.get("target_sha256", "")).lower() != str(target["sha256"]).lower():
        raise ValueError("match-unit target hash differs from config/target.toml")
    units = manifest.get("units", {})
    if not isinstance(units, dict):
        raise ValueError("match-units.toml [units] must be a table")
    for name, unit in units.items():
        if not isinstance(unit, dict):
            raise ValueError(f"unit {name!r} must be a table")
        required = ("source", "object", "profile", "functions", "symbol", "target_address", "size")
        missing = [field for field in required if field not in unit]
        if missing:
            raise ValueError(f"unit {name!r} lacks {', '.join(missing)}")
        if unit.get("artifact_kind") != "coff":
            raise ValueError(
                f"unit {name!r} must explicitly declare artifact_kind = 'coff'"
            )
        profile = unit["profile"]
        if not isinstance(profile, list) or not profile or not all(isinstance(flag, str) for flag in profile):
            raise ValueError(f"unit {name!r} has an invalid compiler profile")
        if any(flag.lower() == "/gl" for flag in profile):
            raise ValueError(f"unit {name!r} requests LTCG; use a linked-image Oracle")
        if not (ROOT / str(unit["source"])).is_file():
            raise ValueError(f"unit {name!r} source does not exist")
        output = (ROOT / str(unit["object"])).resolve()
        output.relative_to((ROOT / "build").resolve())
        if not isinstance(unit["functions"], list) or not unit["functions"]:
            raise ValueError(f"unit {name!r} must contain functions")
    return manifest


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true")
    parser.add_argument("--unit")
    args = parser.parse_args()
    if not args.check and args.unit is None:
        parser.error("one of --check or --unit is required")
    try:
        manifest = load()
        units = manifest.get("units", {})
        if args.check:
            print(f"normal-COFF match-unit graph OK: {len(units)} configured units")
            return 0
        if args.unit not in units:
            raise ValueError(f"unknown match unit: {args.unit}")
        unit = units[args.unit]
        output = ROOT / str(unit["object"])
        subprocess.run(
            [
                str(ROOT / "scripts" / "compile-probe.sh"),
                str(ROOT / str(unit["source"])),
                str(output),
                *unit["profile"],
            ],
            cwd=ROOT,
            check=True,
        )
        if not output.is_file() or output.read_bytes()[:2] != b"L\x01":
            raise ValueError(f"unit {args.unit!r} did not produce normal i386 COFF")
        print(f"built {args.unit}: {output.relative_to(ROOT)}")
        return 0
    except (OSError, KeyError, TypeError, ValueError, subprocess.CalledProcessError, tomllib.TOMLDecodeError) as error:
        print(f"error: match-unit build failed: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
