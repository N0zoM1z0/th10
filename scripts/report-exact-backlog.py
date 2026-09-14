#!/usr/bin/env python3
"""Report source-present TH10 functions that do not have canonical exact units."""

from __future__ import annotations

import argparse
import csv
from itertools import groupby
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
        "--source",
        action="append",
        default=[],
        help="select an exact source path (repeatable)",
    )
    parser.add_argument(
        "--module",
        action="append",
        default=[],
        help="select an exact module/subsystem name (repeatable)",
    )
    parser.add_argument(
        "--state",
        choices=("authored", "origin-review", "excluded", "all"),
        default="authored",
        help="select one origin state or all non-exact source mappings",
    )
    parser.add_argument("--json", action="store_true", help="emit one JSON report")
    return parser.parse_args()


def load_backlog() -> tuple[list[dict[str, object]], dict[str, int]]:
    functions = {
        int(row["address"], 0): row for row in rows("functions.csv")
    }
    origins = {
        int(row["address"], 0): row for row in rows("function-origins.csv")
    }
    mappings = rows("reccmp-functions.csv")
    exact_addresses = {int(row["address"], 0) for row in rows("matches.csv")}

    result: list[dict[str, object]] = []
    state_counts = {"authored": 0, "origin-review": 0, "excluded": 0}
    state_bytes = {"authored": 0, "origin-review": 0, "excluded": 0}
    for mapping in mappings:
        address = int(mapping["address"], 0)
        if address in exact_addresses:
            continue
        function = functions[address]
        origin = origins[address]
        disposition = origin["disposition"]
        if disposition == "authored":
            state = "authored"
        elif disposition == "review":
            state = "origin-review"
        else:
            state = "excluded"
        size = int(function["size"], 0)
        state_counts[state] += 1
        state_bytes[state] += size
        source = function["source_file"]
        if not source:
            raise ValueError(f"source mapping at {mapping['address']} has no source file")
        source_path = (ROOT / source).resolve()
        source_path.relative_to(ROOT.resolve())
        if not source_path.is_file():
            raise ValueError(f"source mapping at {mapping['address']} is missing {source}")
        module = function["module"] or origin["subsystem"]
        result.append(
            {
                "address": f"0x{address:08X}",
                "size": size,
                "name": mapping["name"],
                "source": source,
                "module": module,
                "queue_state": state,
                "origin": origin["origin"],
                "origin_confidence": origin["confidence"],
                "origin_evidence_id": origin["evidence_id"],
                "current_name": function["current_name"],
                "calling_convention": function["calling_convention"],
                "signature": function["signature"],
                "boundary_evidence": function["evidence"],
                "reconstruction_notes": function["notes"],
            }
        )
    result.sort(key=lambda row: (str(row["source"]), int(str(row["address"]), 0)))
    totals = {
        "mapped": len(mappings),
        "exact": len(exact_addresses),
        "authored_backlog": state_counts["authored"],
        "authored_backlog_bytes": state_bytes["authored"],
        "origin_review": state_counts["origin-review"],
        "origin_review_bytes": state_bytes["origin-review"],
        "excluded": state_counts["excluded"],
        "excluded_bytes": state_bytes["excluded"],
    }
    return result, totals


def select(
    backlog: list[dict[str, object]], args: argparse.Namespace
) -> list[dict[str, object]]:
    available_sources = {str(row["source"]) for row in backlog}
    available_modules = {str(row["module"]) for row in backlog if row["module"]}
    unknown_sources = sorted(set(args.source) - available_sources)
    unknown_modules = sorted(set(args.module) - available_modules)
    if unknown_sources:
        raise ValueError(f"unknown backlog source(s): {unknown_sources}")
    if unknown_modules:
        raise ValueError(f"unknown backlog module(s): {unknown_modules}")

    result = backlog
    if args.state != "all":
        result = [row for row in result if row["queue_state"] == args.state]
    if args.source:
        wanted_sources = set(args.source)
        result = [row for row in result if row["source"] in wanted_sources]
    if args.module:
        wanted_modules = set(args.module)
        result = [row for row in result if row["module"] in wanted_modules]
    return result


def render_text(selected: list[dict[str, object]], totals: dict[str, int]) -> None:
    for source, group_rows in groupby(selected, key=lambda row: str(row["source"])):
        group = list(group_rows)
        byte_count = sum(int(row["size"]) for row in group)
        print(f"{source}: {len(group)} function(s), {byte_count} bytes")
        for row in group:
            state = "" if row["queue_state"] == "authored" else f" [{row['queue_state']}]"
            module = f" [{row['module']}]" if row["module"] else ""
            print(
                f"  {row['address']}  {int(row['size']):5d}  "
                f"{row['name']}{module}{state}"
            )
    selected_bytes = sum(int(row["size"]) for row in selected)
    print(
        f"selected non-exact source backlog: {len(selected)} function(s), "
        f"{selected_bytes} bytes"
    )
    print(
        f"authored queue: {totals['authored_backlog']} function(s), "
        f"{totals['authored_backlog_bytes']} bytes; "
        f"origin review: {totals['origin_review']} function(s), "
        f"{totals['origin_review_bytes']} bytes; "
        f"canonical exact: {totals['exact']}"
    )
    print("queue membership is triage only and grants no exactness credit")


def main() -> int:
    args = parse_args()
    try:
        subprocess.run(
            [sys.executable, "scripts/validate-tracking.py"],
            cwd=ROOT,
            check=True,
            stdout=subprocess.DEVNULL,
        )
        backlog, totals = load_backlog()
        selected = select(backlog, args)
    except (OSError, KeyError, TypeError, ValueError, subprocess.CalledProcessError) as exc:
        if args.json:
            print(json.dumps({"result": "error", "error": str(exc)}, indent=2))
        else:
            print(f"error: exact backlog report failed: {exc}", file=sys.stderr)
        return 1

    if args.json:
        print(
            json.dumps(
                {
                    "schema_version": 1,
                    "claim": "triage-only; no exactness credit",
                    "selected_count": len(selected),
                    "selected_bytes": sum(int(row["size"]) for row in selected),
                    "totals": totals,
                    "items": selected,
                },
                indent=2,
                sort_keys=True,
            )
        )
    else:
        render_text(selected, totals)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
