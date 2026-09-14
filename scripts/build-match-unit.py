#!/usr/bin/env python3
"""Build one target-bound TH10 exact-comparison artifact."""

from __future__ import annotations

import argparse
from collections import Counter
from pathlib import Path
import subprocess
import sys
import tomllib

from ltcg_link import HARNESS_KIND, LINK_PROFILE, cold_link, tool_environment


ROOT = Path(__file__).resolve().parents[1]
MANIFEST = ROOT / "config" / "match-units.toml"
TARGET = ROOT / "config" / "target.toml"
SUPPORTED_ARTIFACTS = {"coff", "linked-pe"}
SUPPORTED_FIELDS = {"DIR32": 4, "REL8": 1, "REL16": 2, "REL32": 4}


def build_path(raw: object, label: str) -> Path:
    path = (ROOT / str(raw)).resolve()
    try:
        path.relative_to((ROOT / "build").resolve())
    except ValueError as exc:
        raise ValueError(f"{label} must stay below build/") from exc
    return path


def validate_fields(name: str, unit: dict[str, object], compare_size: int) -> None:
    fields = unit.get("linkages", [])
    if not isinstance(fields, list):
        raise ValueError(f"unit {name!r} linkages must be a list")
    occupied: set[int] = set()
    for field in fields:
        if not isinstance(field, dict):
            raise ValueError(f"unit {name!r} has an invalid linkage row")
        kind = str(field.get("type", ""))
        width = int(field.get("width", SUPPORTED_FIELDS.get(kind, 0)))
        if kind not in SUPPORTED_FIELDS or width != SUPPORTED_FIELDS[kind]:
            raise ValueError(f"unit {name!r} has an unsupported linkage type/width")
        offset = int(field["offset"])
        extent = set(range(offset, offset + width))
        if offset < 0 or offset + width > compare_size or occupied & extent:
            raise ValueError(f"unit {name!r} has an invalid linkage extent")
        occupied.update(extent)
        symbol = field.get("symbol")
        if (
            not isinstance(symbol, str)
            or not symbol
            or symbol.startswith("_th10_ltcg_probe_anchor_")
        ):
            raise ValueError(f"unit {name!r} has an invalid linkage symbol")
        target = int(field["target"])
        addend = int(field.get("addend", 0))
        if not 0 <= target <= 0xFFFFFFFF or not -0x80000000 <= addend <= 0x7FFFFFFF:
            raise ValueError(f"unit {name!r} has an invalid linkage target/addend")


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

    compile_groups: dict[tuple[object, ...], Path] = {}
    artifact_groups: dict[Path, tuple[object, ...]] = {}
    target_extents: list[tuple[int, int, str]] = []
    text_start = int(pe["text_start"], 0)
    text_end = int(pe["text_end"], 0)
    for name, unit in units.items():
        if not isinstance(unit, dict):
            raise ValueError(f"unit {name!r} must be a table")
        required = (
            "artifact_kind",
            "source",
            "profile",
            "functions",
            "symbol",
            "target_address",
            "size",
        )
        missing = [field for field in required if field not in unit]
        if missing:
            raise ValueError(f"unit {name!r} lacks {', '.join(missing)}")
        kind = str(unit["artifact_kind"])
        if kind not in SUPPORTED_ARTIFACTS:
            raise ValueError(f"unit {name!r} has unsupported artifact_kind {kind!r}")
        profile = unit["profile"]
        if not isinstance(profile, list) or not profile or not all(
            isinstance(flag, str) and flag for flag in profile
        ):
            raise ValueError(f"unit {name!r} has an invalid compiler profile")
        has_gl = any(flag.lower() == "/gl" for flag in profile)
        source = (ROOT / str(unit["source"])).resolve()
        try:
            source.relative_to(ROOT.resolve())
        except ValueError as exc:
            raise ValueError(f"unit {name!r} source leaves the repository") from exc
        if not source.is_file():
            raise ValueError(f"unit {name!r} source does not exist")
        functions = unit["functions"]
        if (
            not isinstance(functions, list)
            or not functions
            or not all(isinstance(function, str) and function for function in functions)
        ):
            raise ValueError(f"unit {name!r} must contain functions")
        if not isinstance(unit["symbol"], str) or not unit["symbol"]:
            raise ValueError(f"unit {name!r} has an invalid symbol")
        address = int(unit["target_address"])
        size = int(unit["size"])
        compare_size = int(unit.get("compare_size", size))
        if size <= 0 or compare_size < size:
            raise ValueError(f"unit {name!r} has an invalid comparison extent")
        if address < text_start or address + compare_size - 1 > text_end:
            raise ValueError(f"unit {name!r} comparison extent leaves target .text")
        target_extents.append((address, address + compare_size, name))

        if kind == "coff":
            if has_gl:
                raise ValueError(f"unit {name!r} requests LTCG for a COFF artifact")
            output = build_path(unit.get("object", ""), f"unit {name!r} object")
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
                target_value = int(relocation["target"])
                if not 0 <= target_value <= 0xFFFFFFFF:
                    raise ValueError(f"unit {name!r} has an invalid relocation target")
            if unit.get("linkages"):
                raise ValueError(f"unit {name!r} mixes COFF relocations and linkages")
            group = (kind, source, tuple(profile))
        else:
            if not has_gl:
                raise ValueError(f"unit {name!r} linked PE profile lacks /GL")
            output = build_path(
                unit.get("output_dir", ""), f"unit {name!r} output_dir"
            )
            if output.exists() and not output.is_dir():
                raise ValueError(f"unit {name!r} output_dir is not a directory")
            entry = unit.get("entry_symbol")
            if not isinstance(entry, str) or not entry:
                raise ValueError(f"unit {name!r} lacks a linked entry symbol")
            link_profile = unit.get("link_profile")
            if link_profile != LINK_PROFILE:
                raise ValueError(f"unit {name!r} linked profile differs from the driver")
            if unit.get("harness") != HARNESS_KIND:
                raise ValueError(f"unit {name!r} linked harness differs from the driver")
            if unit.get("relocations"):
                raise ValueError(f"unit {name!r} mixes COFF relocations and linkages")
            if "linkages" not in unit:
                raise ValueError(f"unit {name!r} lacks an exhaustive linkage list")
            validate_fields(name, unit, compare_size)
            group = (
                kind,
                source,
                tuple(profile),
                entry,
                tuple(link_profile),
                HARNESS_KIND,
            )
        previous_output = compile_groups.setdefault(group, output)
        if previous_output != output:
            raise ValueError(f"units with build group {group!r} must share one output")
        previous_group = artifact_groups.setdefault(output, group)
        if previous_group != group:
            raise ValueError(f"build artifact {output.relative_to(ROOT)!s} has two owners")

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
            counts = Counter(str(unit["artifact_kind"]) for unit in units.values())
            detail = ", ".join(
                f"{kind}={count}" for kind, count in sorted(counts.items())
            )
            print(f"match-unit graph OK: {len(units)} configured units ({detail})")
            return 0
        if args.unit not in units:
            raise ValueError(f"unknown match unit: {args.unit}")
        unit = units[args.unit]
        source = ROOT / str(unit["source"])
        if unit["artifact_kind"] == "coff":
            output = ROOT / str(unit["object"])
            subprocess.run(
                [
                    str(ROOT / "scripts" / "compile-probe.sh"),
                    str(source),
                    str(output),
                    *unit["profile"],
                ],
                cwd=ROOT,
                check=True,
            )
            if not output.is_file() or output.read_bytes()[:2] != b"L\x01":
                raise ValueError(f"unit {args.unit!r} did not produce normal i386 COFF")
            print(f"built {args.unit}: {output.relative_to(ROOT)}")
        else:
            linker, environment = tool_environment()
            linked = cold_link(
                source,
                ROOT / str(unit["output_dir"]),
                str(unit["entry_symbol"]),
                linker,
                environment,
                list(unit["profile"]),
            )
            if Path(linked["object"]).read_bytes()[:8] != bytes.fromhex(
                "0000ffff01004c01"
            ):
                raise ValueError(f"unit {args.unit!r} did not produce an LTCG object")
            print(
                f"built {args.unit}: "
                f"{Path(linked['image']).relative_to(ROOT)} + map/PDB"
            )
        return 0
    except (
        OSError,
        KeyError,
        TypeError,
        ValueError,
        subprocess.CalledProcessError,
        tomllib.TOMLDecodeError,
    ) as error:
        print(f"error: match-unit build failed: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
