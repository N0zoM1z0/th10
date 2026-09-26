#!/usr/bin/env python3
"""Validate the small, target-bound TH10 reconstruction control plane."""

from __future__ import annotations

import argparse
import csv
from pathlib import Path
import subprocess
import sys
import tomllib


ROOT = Path(__file__).resolve().parents[1]
CONFIG = ROOT / "config"


def rows(name: str) -> list[dict[str, str]]:
    with (CONFIG / name).open(newline="", encoding="utf-8") as stream:
        reader = csv.DictReader(stream)
        if not reader.fieldnames:
            raise ValueError(f"{name}: missing CSV header")
        if len(reader.fieldnames) != len(set(reader.fieldnames)):
            raise ValueError(f"{name}: duplicate CSV column name")
        result = []
        for row in reader:
            if None in row or any(value is None for value in row.values()):
                raise ValueError(f"{name}:{reader.line_num}: wrong CSV column count")
            result.append(row)
        return result


def one_column(name: str) -> list[str]:
    with (CONFIG / name).open(newline="", encoding="utf-8") as stream:
        result = []
        for line_number, row in enumerate(csv.reader(stream), 1):
            if len(row) != 1:
                raise ValueError(f"{name}:{line_number}: expected one CSV column")
            if row[0]:
                result.append(row[0])
        return result


def validate() -> dict[str, int]:
    with (CONFIG / "target.toml").open("rb") as stream:
        target_manifest = tomllib.load(stream)
    target = target_manifest["target"]
    pe = target_manifest["pe"]
    with (CONFIG / "match-units.toml").open("rb") as stream:
        units = tomllib.load(stream)
    with (CONFIG / "build.toml").open("rb") as stream:
        build = tomllib.load(stream)
    for label, digest in (
        ("match-units", units.get("target_sha256")),
        ("build", build.get("target_sha256")),
    ):
        if digest != target["sha256"]:
            raise ValueError(f"{label} target SHA-256 mismatch")
    functions = rows("functions.csv")
    origins = rows("function-origins.csv")
    boundaries = rows("function-boundaries.csv")
    mappings = rows("reccmp-functions.csv")
    matches = rows("matches.csv")
    implemented = set(one_column("implemented.csv"))
    starts: list[int] = []
    text_start, text_end = int(pe["text_start"], 0), int(pe["text_end"], 0)
    for row in functions:
        address, size, end = int(row["address"], 0), int(row["size"], 0), int(row["span_end"], 0)
        if size <= 0 or end != address + size - 1:
            raise ValueError(f"invalid function extent at {row['address']}")
        if address < text_start or end > text_end:
            raise ValueError(f"function extent leaves .text at {row['address']}")
        starts.append(address)
    if starts != sorted(set(starts)):
        raise ValueError("function addresses must be unique and sorted")
    functions_by_address = {int(row["address"], 0): row for row in functions}
    origins_by_address = {int(row["address"], 0): row for row in origins}
    boundaries_by_address = {int(row["address"], 0): row for row in boundaries}
    function_addresses = set(functions_by_address)
    origin_addresses = set(origins_by_address)
    if origin_addresses != function_addresses or len(origins) != len(functions):
        raise ValueError("origin ledger must cover each function exactly once")
    boundary_addresses = set(boundaries_by_address)
    if boundary_addresses != function_addresses or len(boundaries) != len(functions):
        raise ValueError("boundary ledger must cover each function exactly once")
    if [int(row["address"], 0) for row in boundaries] != starts:
        raise ValueError("boundary ledger rows must follow the sorted function inventory")
    allowed_boundary_states = {
        "reviewed": {"high", "medium"},
        "provisional": {"medium", "low"},
        "needs_review": {"low"},
    }
    for row in boundaries:
        candidate = functions_by_address[int(row["address"], 0)]
        if int(row["span_end"], 0) != int(candidate["span_end"], 0):
            raise ValueError(f"boundary extent differs at {row['address']}")
        state, confidence = row["state"], row["confidence"]
        if state not in allowed_boundary_states or confidence not in allowed_boundary_states[state]:
            raise ValueError(f"invalid boundary state at {row['address']}")
        if not row["evidence_id"] or not row["notes"]:
            raise ValueError(f"boundary evidence is incomplete at {row['address']}")
        if state == "reviewed" and "boundary unreviewed" in candidate["evidence"].lower():
            raise ValueError(f"stale boundary state in function inventory at {row['address']}")
    allowed_origins = {"unknown", "authored", "authored_game", "compiler", "compiler_generated", "library", "third_party", "import_thunk", "data", "padding"}
    allowed_dispositions = {"review", "authored", "exclude", "indeterminate"}
    for row in origins:
        if row["origin"] not in allowed_origins or row["disposition"] not in allowed_dispositions:
            raise ValueError(f"invalid origin state at {row['address']}")
        function = functions_by_address[int(row["address"], 0)]
        if (row["disposition"] == "exclude") != (function["status"] == "excluded"):
            raise ValueError(f"exclusion state differs from function inventory at {row['address']}")
        if row["disposition"] == "indeterminate":
            if (row["origin"] != "unknown" or row["confidence"] != "unknown"
                    or not row["evidence_id"]
                    or "origin indeterminate:" not in function["notes"]):
                raise ValueError(f"indeterminate origin lacks individual reason at {row['address']}")
    mapping_addresses = {int(row["address"], 0) for row in mappings}
    match_addresses = {int(row["address"], 0) for row in matches}
    if not mapping_addresses.issubset(function_addresses) or not match_addresses.issubset(function_addresses):
        raise ValueError("mapping or match ledger references an unknown address")
    if len(mapping_addresses) != len(mappings):
        raise ValueError("source mapping addresses must be unique")
    mappings_by_address = {int(row["address"], 0): row for row in mappings}
    for address, mapping in mappings_by_address.items():
        function = functions_by_address[address]
        if not function["source_file"] or function["proposed_name"] != mapping["name"]:
            raise ValueError(f"source mapping differs from function inventory at {function['address']}")
    for address, function in functions_by_address.items():
        if function["source_file"] and address not in mappings_by_address:
            raise ValueError(f"source-present function lacks mapping at {function['address']}")
    mapped_names = {row["name"] for row in mappings}
    if len(mapped_names) != len(mappings):
        raise ValueError("source mapping names must be unique")
    if not implemented.issubset(mapped_names):
        raise ValueError("implemented.csv contains an unmapped source name")
    if implemented != mapped_names:
        raise ValueError("implemented.csv must cover every mapped source name")
    configured_units = units.get("units", {})
    if not isinstance(configured_units, dict):
        raise ValueError("match units must be a table")
    matched_units: set[str] = set()
    if len(match_addresses) != len(matches):
        raise ValueError("exact match addresses must be unique")
    if [int(row["address"], 0) for row in matches] != sorted(match_addresses):
        raise ValueError("exact match rows must be sorted by address")
    for row in matches:
        unit_name = row["unit"]
        if unit_name not in configured_units or row["match_percent"] != "100.00":
            raise ValueError(f"unreplayable exact claim at {row['address']}")
        if row["status"] != "exact" or unit_name in matched_units:
            raise ValueError(f"invalid or duplicate exact unit {unit_name!r}")
        matched_units.add(unit_name)
        unit = configured_units[unit_name]
        address = int(row["address"], 0)
        size = int(row["size"], 0)
        if address != int(unit["target_address"]) or size != int(unit["size"]):
            raise ValueError(f"exact ledger extent differs from unit {unit_name!r}")
        if row["name"] not in unit["functions"]:
            raise ValueError(f"exact ledger name differs from unit {unit_name!r}")
        mapping = mappings_by_address.get(address)
        if mapping is None or mapping["name"] != row["name"]:
            raise ValueError(f"exact unit {unit_name!r} lacks its source mapping")
        function = functions_by_address[address]
        if (function["status"] != "exact" or function["match_percent"] != "100.00"
                or function["proposed_name"] != row["name"]):
            raise ValueError(f"exact match differs from function inventory at {row['address']}")
        expected_source = unit.get("pdb_source", unit["source"])
        if function["source_file"] != expected_source:
            raise ValueError(f"exact unit {unit_name!r} differs from source ledger")
        if boundaries_by_address[address]["state"] != "reviewed":
            raise ValueError(f"exact unit {unit_name!r} lacks a reviewed boundary")
    if matched_units != set(configured_units):
        missing = sorted(set(configured_units) - matched_units)
        extra = sorted(matched_units - set(configured_units))
        raise ValueError(
            f"canonical units and exact ledger differ: missing={missing!r} extra={extra!r}"
        )
    for address, function in functions_by_address.items():
        if function["status"] == "exact" and address not in match_addresses:
            raise ValueError(f"function inventory has unregistered exact claim at {function['address']}")
    if build.get("schema_version") != 1:
        raise ValueError("unsupported build manifest schema")
    acceptance = build.get("acceptance", {})
    if acceptance.get("whole_build_closed") and not build.get("graph", {}).get("sources"):
        raise ValueError("whole-build closure cannot be true for an empty graph")
    return {
        "functions": len(functions),
        "boundaries": len(boundaries),
        "mappings": len(mappings),
        "matches": len(matches),
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--require-target", action="store_true")
    args = parser.parse_args()
    try:
        counts = validate()
        if args.require_target:
            subprocess.run([sys.executable, "scripts/verify-target.py"], cwd=ROOT, check=True)
    except (OSError, KeyError, TypeError, ValueError, tomllib.TOMLDecodeError, subprocess.CalledProcessError) as exc:
        print(f"error: tracking validation failed: {exc}", file=sys.stderr)
        return 1
    print(
        f"tracking OK: {counts['functions']} candidates/boundaries, "
        f"{counts['mappings']} mappings, {counts['matches']} exact"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
