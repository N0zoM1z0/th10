#!/usr/bin/env python3
"""Rank source-absent TH10 candidates for semantic core reconstruction."""

from __future__ import annotations

import argparse
import csv
import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
CONFIG = ROOT / "config"
DEFAULT_ARCHITECTURE = ROOT / ".analysis" / "architecture" / "function-metrics.csv"


def rows(path: Path) -> list[dict[str, str]]:
    with path.open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream))


def address(value: str) -> int:
    return int(value, 0)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--limit", type=int, default=40)
    parser.add_argument("--after", type=lambda value: int(value, 0))
    parser.add_argument("--before", type=lambda value: int(value, 0))
    parser.add_argument(
        "--state",
        action="append",
        choices=("reviewed", "provisional", "needs_review"),
        help="boundary state to include; default: reviewed",
    )
    parser.add_argument(
        "--disposition",
        action="append",
        choices=("review", "authored", "exclude"),
        help="origin disposition to include; default: review and authored",
    )
    parser.add_argument(
        "--architecture",
        type=Path,
        default=DEFAULT_ARCHITECTURE,
        help="optional Ghidra metrics from `scripts/repo-python scripts/ghidra.py architecture`",
    )
    parser.add_argument("--json", action="store_true")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    if args.limit <= 0:
        raise SystemExit("error: --limit must be positive")
    function_rows = rows(CONFIG / "functions.csv")
    origins = {
        address(row["address"]): row
        for row in rows(CONFIG / "function-origins.csv")
    }
    boundaries = {
        address(row["address"]): row
        for row in rows(CONFIG / "function-boundaries.csv")
    }
    mapped = {
        address(row["address"])
        for row in rows(CONFIG / "reccmp-functions.csv")
    }
    metrics = {}
    if args.architecture.is_file():
        metrics = {
            address(row["address"]): row for row in rows(args.architecture)
        }
    states = set(args.state or ("reviewed",))
    dispositions = set(args.disposition or ("review", "authored"))
    ranked = []
    for row in function_rows:
        candidate = address(row["address"])
        if candidate in mapped:
            continue
        if args.after is not None and candidate < args.after:
            continue
        if args.before is not None and candidate >= args.before:
            continue
        origin = origins[candidate]
        boundary = boundaries[candidate]
        if origin["disposition"] not in dispositions or boundary["state"] not in states:
            continue
        metric = metrics.get(candidate, {})
        size = int(row["size"], 0)
        in_degree = int(metric.get("in_degree", 0))
        call_sites = int(metric.get("call_sites", 0))
        global_refs = int(metric.get("global_refs", 0))
        string_refs = int(metric.get("string_refs", 0))
        score = (
            size
            + min(in_degree, 20) * 30
            + min(call_sites, 50) * 8
            + min(global_refs, 40) * 5
            + string_refs * 20
        )
        ranked.append(
            {
                "address": f"0x{candidate:08X}",
                "span_end": f"0x{address(row['span_end']):08X}",
                "size": size,
                "score": score,
                "boundary": boundary["state"],
                "origin": origin["origin"],
                "disposition": origin["disposition"],
                "in_degree": in_degree,
                "call_sites": call_sites,
                "global_refs": global_refs,
                "string_refs": string_refs,
                "current_name": row["current_name"],
            }
        )
    ranked.sort(key=lambda item: (-int(item["score"]), -int(item["size"]), item["address"]))
    selected = ranked[: args.limit]
    report = {
        "states": sorted(states),
        "dispositions": sorted(dispositions),
        "architecture_metrics": str(args.architecture) if metrics else None,
        "score_note": (
            "Triage only: size plus capped call, entry, global, and string-reference "
            "weights; it grants no boundary, origin, source, or exactness credit."
        ),
        "eligible": len(ranked),
        "items": selected,
    }
    if args.json:
        print(json.dumps(report, indent=2))
        return 0
    print(report["score_note"])
    print(f"eligible: {len(ranked)}; showing: {len(selected)}")
    print("score  size address    boundary    in calls globals strings name")
    for item in selected:
        print(
            f"{item['score']:5d} {item['size']:5d} {item['address']} "
            f"{item['boundary']:<11} {item['in_degree']:2d} "
            f"{item['call_sites']:5d} {item['global_refs']:7d} "
            f"{item['string_refs']:7d} {item['current_name']}"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
