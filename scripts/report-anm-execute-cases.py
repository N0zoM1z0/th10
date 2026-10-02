#!/usr/bin/env python3
"""Compare aligned ANM ExecuteScript case spans in a linked LTCG probe.

This is a diagnostic only. Case boundaries help distinguish local source changes
from shifted branch displacements; they do not establish whole-function exactness.
"""

from __future__ import annotations

import argparse
import importlib.util
import json
from pathlib import Path
import struct
import sys

from linked_image import PEImage, map_publics
from target_identity import pe_bytes_at


ROOT = Path(__file__).resolve().parents[1]


def load_module(filename: str, module_name: str):
    path = ROOT / "scripts" / filename
    spec = importlib.util.spec_from_file_location(module_name, path)
    if spec is None or spec.loader is None:
        raise ValueError(f"cannot load linked-image comparator: {path}")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def resolve_artifact(value: str) -> Path:
    path = Path(value)
    return path if path.is_absolute() else ROOT / path


def selected_function(probe_path: Path) -> tuple[dict, dict]:
    report = json.loads(probe_path.read_text(encoding="utf-8"))
    found = [
        (source, function)
        for source in report["sources"]
        for function in source["functions"]
        if function["address"] == "0x0043EE30"
    ]
    if len(found) != 1:
        raise ValueError(f"expected one ExecuteScript owner, found {len(found)}")
    return found[0]


def compare(probe_path: Path) -> dict:
    probe = load_module("probe-ltcg-backlog.py", "probe_ltcg_backlog")
    table_report = load_module("report-anm-execute-table.py", "report_anm_execute_table")
    target = probe.verified_target()
    source, function = selected_function(probe_path)
    image_path = resolve_artifact(source["image"])
    map_path = resolve_artifact(source["map"])
    image = PEImage(image_path)
    publics = map_publics(map_path)
    address = int(function["candidate_address"], 0)
    contribution_size = int(function["candidate_size"])

    target_table = struct.unpack(
        f"<{table_report.JUMP_TABLE_ENTRIES}I",
        pe_bytes_at(
            target.data,
            table_report.JUMP_TABLE_ADDRESS,
            table_report.JUMP_TABLE_ENTRIES * 4,
        ),
    )
    enum_values, _, source_problems = table_report.parse_source(
        table_report.DEFAULT_SOURCE, table_report.DEFAULT_HEADER
    )
    if source_problems:
        raise ValueError("; ".join(source_problems))
    layout = table_report.candidate_case_layout(
        image_path,
        address,
        contribution_size,
        target_table,
        {value: name for name, value in enum_values.items()},
    )
    if not layout["physical_order_matches"]:
        raise ValueError("candidate physical case order differs from target")
    if (
        int(layout["jump_table_address"], 0)
        + table_report.JUMP_TABLE_ENTRIES * 4
        > address + contribution_size
    ):
        raise ValueError("candidate jump table exceeds the declared contribution")

    target_groups = layout["target_physical_groups"]
    candidate_groups = layout["candidate_physical_groups"]
    cases = []
    for index, (left, right) in enumerate(zip(target_groups, candidate_groups)):
        target_address = int(left["destination"], 0)
        candidate_address = int(right["destination"], 0)
        target_end = (
            int(target_groups[index + 1]["destination"], 0)
            if index + 1 < len(target_groups)
            else table_report.JUMP_TABLE_ADDRESS
        )
        candidate_end = (
            int(candidate_groups[index + 1]["destination"], 0)
            if index + 1 < len(candidate_groups)
            else int(layout["jump_table_address"], 0)
        )
        target_size = target_end - target_address
        candidate_size = candidate_end - candidate_address
        comparison = probe.structural_compare(
            image,
            publics,
            candidate_address,
            candidate_size,
            target,
            target_address,
            target_size,
        )
        cases.append(
            {
                "opcodes": left["opcodes"],
                "target_address": f"0x{target_address:08X}",
                "candidate_address": f"0x{candidate_address:08X}",
                "target_size": target_size,
                "candidate_size": candidate_size,
                "gap_delta": candidate_size - target_size,
                "matched_comparable_bytes": comparison["matched_comparable_bytes"],
                "comparable_bytes": comparison["comparable_bytes"],
                "normalization_complete": comparison["normalization_complete"],
            }
        )
    return {
        "acceptance_authority": "none",
        "target_sha256": target.sha256,
        "candidate_image": str(image_path),
        "candidate_image_sha256": image.sha256,
        "candidate_map": str(map_path),
        "candidate_address": f"0x{address:08X}",
        "candidate_contribution_size": contribution_size,
        "physical_order_matches": True,
        "pre_table_span_delta": layout["pre_table_span_delta"],
        "case_aligned": {
            "matched_comparable_bytes": sum(
                case["matched_comparable_bytes"] for case in cases
            ),
            "comparable_bytes": sum(case["comparable_bytes"] for case in cases),
            "absolute_case_gap_delta": sum(abs(case["gap_delta"]) for case in cases),
            "same_sized_cases": sum(case["gap_delta"] == 0 for case in cases),
            "normalization_complete": all(
                case["normalization_complete"] for case in cases
            ),
        },
        "cases": cases,
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("probe_json", type=Path)
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args()
    try:
        result = compare(args.probe_json)
    except (KeyError, IndexError, OSError, ValueError) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 2
    if args.json:
        print(json.dumps(result, indent=2))
    else:
        aligned = result["case_aligned"]
        print(
            "ANM ExecuteScript case diagnostic: "
            f"{aligned['matched_comparable_bytes']}/"
            f"{aligned['comparable_bytes']} comparable bytes; "
            f"{aligned['same_sized_cases']}/92 same-sized cases; "
            f"absolute gap {aligned['absolute_case_gap_delta']} bytes"
        )
        print("acceptance authority: none")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
