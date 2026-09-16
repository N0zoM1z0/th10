#!/usr/bin/env python3
"""Report whether reviewed authored TH10 owners have maintained source mappings."""

from __future__ import annotations

import argparse
import csv
import json
from pathlib import Path
import subprocess
import sys


ROOT = Path(__file__).resolve().parents[1]
CONFIG = ROOT / "config"


def rows(name: str) -> list[dict[str, str]]:
    with (CONFIG / name).open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream))


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--module",
        action="append",
        default=[],
        help="select a module/subsystem name (repeatable); default: all modules",
    )
    parser.add_argument(
        "--require-complete",
        action="store_true",
        help="fail when a selected authored owner has no maintained source mapping",
    )
    parser.add_argument("--json", action="store_true", help="emit one JSON report")
    return parser.parse_args()


def build_report(selected_modules: list[str]) -> dict[str, object]:
    functions = {int(row["address"], 0): row for row in rows("functions.csv")}
    origins = {int(row["address"], 0): row for row in rows("function-origins.csv")}
    mappings = {int(row["address"], 0): row for row in rows("reccmp-functions.csv")}
    exact_addresses = {int(row["address"], 0) for row in rows("matches.csv")}

    authored: list[dict[str, object]] = []
    available_modules: set[str] = set()
    for address, origin in origins.items():
        if origin["disposition"] != "authored":
            continue
        function = functions[address]
        module = function["module"] or origin["subsystem"]
        if module:
            available_modules.add(module)
        authored.append(
            {
                "address": f"0x{address:08X}",
                "size": int(function["size"], 0),
                "name": (
                    mappings[address]["name"]
                    if address in mappings
                    else function["proposed_name"] or function["current_name"]
                ),
                "module": module,
                "source": function["source_file"] or None,
                "source_present": address in mappings,
                "exact": address in exact_addresses,
                "calling_convention": function["calling_convention"],
                "boundary_evidence": function["evidence"],
            }
        )

    unknown_modules = sorted(set(selected_modules) - available_modules)
    if unknown_modules:
        raise ValueError(f"unknown authored module(s): {unknown_modules}")
    if selected_modules:
        wanted = set(selected_modules)
        authored = [row for row in authored if row["module"] in wanted]

    authored.sort(key=lambda row: int(str(row["address"]), 0))
    source_present = [row for row in authored if row["source_present"]]
    source_absent = [row for row in authored if not row["source_present"]]
    exact = [row for row in authored if row["exact"]]
    nonexact = [row for row in source_present if not row["exact"]]

    return {
        "schema_version": 1,
        "claim": "source-presence inventory only; no exactness credit",
        "selected_modules": selected_modules or sorted(available_modules),
        "authored_count": len(authored),
        "authored_bytes": sum(int(row["size"]) for row in authored),
        "source_present_count": len(source_present),
        "source_present_bytes": sum(int(row["size"]) for row in source_present),
        "source_absent_count": len(source_absent),
        "source_absent_bytes": sum(int(row["size"]) for row in source_absent),
        "exact_count": len(exact),
        "nonexact_source_count": len(nonexact),
        "nonexact_source_bytes": sum(int(row["size"]) for row in nonexact),
        "source_absent": source_absent,
    }


def render_text(report: dict[str, object]) -> None:
    modules = ", ".join(str(value) for value in report["selected_modules"])
    print(f"selected modules: {modules}")
    print(
        f"authored owners: {report['authored_count']} / {report['authored_bytes']} bytes"
    )
    print(
        "source-present: "
        f"{report['source_present_count']} / {report['source_present_bytes']} bytes; "
        "source-absent: "
        f"{report['source_absent_count']} / {report['source_absent_bytes']} bytes"
    )
    print(
        f"canonical exact: {report['exact_count']}; "
        "source-present non-exact: "
        f"{report['nonexact_source_count']} / {report['nonexact_source_bytes']} bytes"
    )
    for row in report["source_absent"]:
        print(
            f"  {row['address']}  {int(row['size']):5d}  "
            f"{row['name']} [{row['module']}]"
        )
    print("source presence and exactness remain independent")


def main() -> int:
    args = parse_args()
    try:
        subprocess.run(
            [sys.executable, "scripts/validate-tracking.py"],
            cwd=ROOT,
            check=True,
            stdout=subprocess.DEVNULL,
        )
        report = build_report(args.module)
    except (OSError, KeyError, TypeError, ValueError, subprocess.CalledProcessError) as exc:
        if args.json:
            print(json.dumps({"result": "error", "error": str(exc)}, indent=2))
        else:
            print(f"error: source-completeness report failed: {exc}", file=sys.stderr)
        return 1

    if args.json:
        print(json.dumps(report, indent=2, sort_keys=True))
    else:
        render_text(report)
    if args.require_complete and report["source_absent_count"]:
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
