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
        target_document = tomllib.load(stream)
    target = target_document["target"]
    pe = target_document["pe"]
    with MANIFEST.open("rb") as stream:
        manifest = tomllib.load(stream)
    if manifest.get("schema_version") != 1:
        raise ValueError("match-units.toml must use schema_version = 1")
    if str(manifest.get("target_sha256", "")).lower() != str(target["sha256"]).lower():
        raise ValueError("match-unit target hash differs from config/target.toml")
    units = manifest.get("units", {})
    if not isinstance(units, dict):
        raise ValueError("match-units.toml [units] must be a table")
    compile_groups: dict[tuple[Path, tuple[str, ...]], Path] = {}
    object_groups: dict[Path, tuple[Path, tuple[str, ...]]] = {}
    target_extents: list[tuple[int, int, str]] = []
    text_start = int(pe["text_start"], 0)
    text_end = int(pe["text_end"], 0)
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
        source = (ROOT / str(unit["source"])).resolve()
        source.relative_to(ROOT.resolve())
        if not source.is_file():
            raise ValueError(f"unit {name!r} source does not exist")
        output = (ROOT / str(unit["object"])).resolve()
        output.relative_to((ROOT / "build").resolve())
        functions = unit["functions"]
        if (
            not isinstance(functions, list)
            or not functions
            or not all(isinstance(function, str) and function for function in functions)
        ):
            raise ValueError(f"unit {name!r} must contain functions")
        address = int(unit["target_address"])
        size = int(unit["size"])
        compare_size = int(unit.get("compare_size", size))
        if size <= 0 or compare_size < size:
            raise ValueError(f"unit {name!r} has an invalid comparison extent")
        if address < text_start or address + compare_size - 1 > text_end:
            raise ValueError(f"unit {name!r} comparison extent leaves target .text")
        target_extents.append((address, address + compare_size, name))
        relocations = unit.get("relocations", [])
        if not isinstance(relocations, list):
            raise ValueError(f"unit {name!r} relocations must be a list")
        relocation_offsets: set[int] = set()
        for relocation in relocations:
            if not isinstance(relocation, dict):
                raise ValueError(f"unit {name!r} has an invalid relocation row")
            offset = int(relocation["offset"])
            if offset in relocation_offsets or not 0 <= offset <= compare_size - 4:
                raise ValueError(f"unit {name!r} has an invalid relocation offset")
            relocation_offsets.add(offset)
            if relocation.get("type") not in {"DIR32", "REL32"}:
                raise ValueError(f"unit {name!r} has an unsupported relocation type")
            if not isinstance(relocation.get("symbol"), str) or not relocation["symbol"]:
                raise ValueError(f"unit {name!r} has an invalid relocation symbol")
            relocation_target = int(relocation["target"])
            if not 0 <= relocation_target <= 0xFFFFFFFF:
                raise ValueError(f"unit {name!r} has an invalid relocation target")
        group = (source, tuple(profile))
        previous_output = compile_groups.setdefault(group, output)
        if previous_output != output:
            raise ValueError(
                f"units with source/profile {group!r} must share one object path"
            )
        previous_group = object_groups.setdefault(output, group)
        if previous_group != group:
            raise ValueError(
                f"object {output.relative_to(ROOT)!s} is shared by different compile profiles"
            )
    target_extents.sort()
    for previous, current in zip(target_extents, target_extents[1:]):
        if current[0] < previous[1]:
            raise ValueError(
                f"units {previous[2]!r} and {current[2]!r} overlap in target .text"
            )
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
