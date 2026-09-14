#!/usr/bin/env python3
"""Compile, link, and structurally probe TH10 source backlog under VC7.1 LTCG."""

from __future__ import annotations

import argparse
from collections import defaultdict
import json
from pathlib import Path
import struct
import sys
import tomllib

from linked_image import (
    LinkedImageError,
    PEImage,
    linked_code_fields,
    linked_functions,
    map_publics,
    verify_capstone,
)
from ltcg_link import (
    BUILD,
    HARNESS_KIND,
    LTCG_PROFILE,
    NORMAL_PROFILE,
    cold_link,
    compile_source,
    run,
    sha256,
    tool_environment,
)


ROOT = Path(__file__).resolve().parents[1]
TARGET = ROOT / "resources" / "th10.exe"
TARGET_CONFIG = ROOT / "config" / "target.toml"
TOOLS_LOCK = ROOT / "config" / "tools.lock.toml"


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--source",
        action="append",
        default=[],
        help="probe one exact source path (repeatable); default: all authored backlog",
    )
    parser.add_argument(
        "--show",
        choices=("all", "structural-exact", "mismatch", "unresolved"),
        default="all",
        help="filter text output; JSON always contains every result",
    )
    parser.add_argument("--json", action="store_true", help="emit one JSON report")
    return parser.parse_args()


def json_command(command: list[str], label: str) -> dict[str, object]:
    completed = run(command)
    try:
        report = json.loads(completed.stdout)
    except json.JSONDecodeError as exc:
        raise ValueError(f"{label} did not return JSON: {completed.stdout!r}") from exc
    if completed.returncode != 0 or report.get("result") == "error":
        raise ValueError(f"{label} failed: {json.dumps(report, sort_keys=True)}")
    return report


def verified_target() -> PEImage:
    with TARGET_CONFIG.open("rb") as stream:
        expected = tomllib.load(stream)["target"]
    if TARGET.stat().st_size != int(expected["size"]) or sha256(TARGET) != str(
        expected["sha256"]
    ).lower():
        raise ValueError("target identity mismatch")
    return PEImage(TARGET)


def load_backlog(sources: list[str]) -> list[dict[str, object]]:
    command = [sys.executable, "scripts/report-exact-backlog.py", "--json"]
    for source in sources:
        command.extend(["--source", source])
    report = json_command(command, "exact backlog report")
    items = report.get("items")
    if not isinstance(items, list) or not all(isinstance(item, dict) for item in items):
        raise ValueError("exact backlog report has invalid items")
    return items


def normal_inventory(source: Path, output: Path) -> list[dict[str, object]]:
    compile_source(source, output, NORMAL_PROFILE)
    report = json_command(
        [
            sys.executable,
            "scripts/compare-coff-function.py",
            str(output),
            "--list-functions",
            "--json",
        ],
        f"normal COFF inventory for {source.relative_to(ROOT)}",
    )
    functions = report.get("functions")
    if not isinstance(functions, list) or not all(
        isinstance(function, dict) for function in functions
    ):
        raise ValueError("normal COFF inventory has invalid functions")
    return functions


def target_field_value(
    target: bytes,
    target_address: int,
    offset: int,
    width: int,
    kind: str,
    instruction_end: int | None,
) -> tuple[str | None, str | None]:
    if offset < 0 or offset + width > len(target):
        return None, None
    raw = target[offset : offset + width]
    encoded = "0x" + raw[::-1].hex().upper()
    if kind == "DIR32":
        candidate = int.from_bytes(raw, "little")
    else:
        assert instruction_end is not None
        displacement = int.from_bytes(raw, "little", signed=True)
        candidate = target_address + instruction_end + displacement
    return encoded, f"0x{candidate & 0xFFFFFFFF:08X}"


def structural_compare(
    candidate_image: PEImage,
    publics: list[dict[str, object]],
    candidate_address: int,
    candidate_size: int,
    target_image: PEImage,
    target_address: int,
    target_size: int,
) -> dict[str, object]:
    candidate = candidate_image.read_address(candidate_address, candidate_size)
    target = target_image.read_address(target_address, target_size)
    field_report = linked_code_fields(
        candidate_image, publics, candidate_address, candidate_size
    )
    decoded = int(field_report["decoded_bytes"])
    normalization_complete = bool(field_report["normalization_complete"])
    fields = []
    for raw_field in field_report["fields"]:
        field = dict(raw_field)
        offset = int(field["offset"])
        width = int(field["width"])
        kind = str(field["type"])
        encoded, target_candidate = target_field_value(
            target,
            target_address,
            offset,
            width,
            kind,
            (
                int(field["instruction_end"])
                if field["instruction_end"] is not None
                else None
            ),
        )
        field["candidate_target"] = f"0x{int(field['candidate_target']):08X}"
        field["target_encoded_value"] = encoded
        field["target_candidate"] = target_candidate
        fields.append(field)
    occupied: set[int] = set()
    for field in fields:
        candidate_extent = set(
            range(
                int(field["offset"]),
                int(field["offset"]) + int(field["width"]),
            )
        )
        occupied.update(offset for offset in candidate_extent if offset < target_size)
    differences = []
    raw_matched = 0
    for offset, right in enumerate(target):
        left = candidate[offset] if offset < len(candidate) else None
        if left == right:
            raw_matched += 1
        if offset not in occupied and left != right:
            differences.append({"offset": f"0x{offset:X}", "candidate": left, "target": right})
    comparable = target_size - len(occupied)
    return {
        "result": (
            "structural-exact"
            if normalization_complete
            and candidate_size == target_size
            and not differences
            else "mismatch"
        ),
        "candidate_address": f"0x{candidate_address:08X}",
        "candidate_size": candidate_size,
        "target_address": f"0x{target_address:08X}",
        "target_size": target_size,
        "decoded_candidate_bytes": decoded,
        "normalization_complete": normalization_complete,
        "raw_matched_bytes": raw_matched,
        "comparable_bytes": comparable,
        "matched_comparable_bytes": comparable - len(differences),
        "linkage_fields": fields,
        "first_differences": differences[:32],
        "acceptance_authority": "none",
    }


def probe_source(
    source_name: str,
    backlog: list[dict[str, object]],
    target: PEImage,
    linker: Path,
    environment: dict[str, str],
) -> dict[str, object]:
    source = (ROOT / source_name).resolve()
    source.relative_to(ROOT.resolve())
    if not source.is_file():
        raise ValueError(f"missing source: {source_name}")
    directory = BUILD / source_name.replace("/", "_").replace("\\", "_")
    directory.mkdir(parents=True, exist_ok=True)
    inventory = normal_inventory(source, directory / "source.normal.obj")
    symbols_by_hint: dict[str, list[str]] = defaultdict(list)
    for function in inventory:
        hint = function.get("source_name_hint")
        if isinstance(hint, str) and hint:
            symbols_by_hint[hint].append(str(function["symbol"]))
    entry_candidates = [
        symbols_by_hint[str(item["name"])][0]
        for item in backlog
        if len(symbols_by_hint.get(str(item["name"]), [])) == 1
    ]
    if not entry_candidates:
        raise ValueError(f"no external entry symbol can be derived for {source_name}")
    linked = cold_link(source, directory, entry_candidates[0], linker, environment)
    linked_report = linked_functions(linked["image"], linked["map"], linked["pdb"])
    publics = map_publics(linked["map"])
    candidate_image = PEImage(linked["image"])
    linked_by_symbol: dict[str, list[dict[str, object]]] = defaultdict(list)
    for function in linked_report["functions"]:
        pdb_object_name = str(function["pdb_object"]).replace("\\", "/").rsplit("/", 1)[-1]
        if pdb_object_name.lower() == "source.ltcg.obj":
            linked_by_symbol[str(function["symbol"])].append(function)

    reports = []
    for item in backlog:
        name = str(item["name"])
        normal_matches = symbols_by_hint.get(name, [])
        base = {
            "name": name,
            "source": source_name,
            "address": item["address"],
            "size": item["size"],
        }
        if len(normal_matches) != 1:
            reports.append(
                {
                    **base,
                    "result": "unresolved",
                    "reason": "no unique normal-COFF decorated-symbol bridge",
                    "symbols": normal_matches,
                    "acceptance_authority": "none",
                }
            )
            continue
        symbol = normal_matches[0]
        linked_matches = linked_by_symbol.get(symbol, [])
        if len(linked_matches) != 1:
            reports.append(
                {
                    **base,
                    "result": "unresolved",
                    "reason": "no unique PDB-owned linked contribution",
                    "symbols": [symbol],
                    "linked_match_count": len(linked_matches),
                    "acceptance_authority": "none",
                }
            )
            continue
        function = linked_matches[0]
        comparison = structural_compare(
            candidate_image,
            publics,
            int(str(function["address"]), 0),
            int(function["size"]),
            target,
            int(str(item["address"]), 0),
            int(item["size"]),
        )
        reports.append(
            {
                **base,
                **comparison,
                "symbol": symbol,
                "extent_source": function["extent_source"],
                "pdb_module_index": function["pdb_module_index"],
            }
        )
    counts: dict[str, int] = defaultdict(int)
    for report in reports:
        counts[str(report["result"])] += 1
    return {
        "source": source_name,
        "entry_symbol": entry_candidates[0],
        "normal_profile": NORMAL_PROFILE,
        "ltcg_profile": LTCG_PROFILE,
        "link_harness": HARNESS_KIND,
        "physical_ownership": "unknown",
        "object": str(Path(linked["object"]).relative_to(ROOT)),
        "image": str(Path(linked["image"]).relative_to(ROOT)),
        "map": str(Path(linked["map"]).relative_to(ROOT)),
        "pdb": str(Path(linked["pdb"]).relative_to(ROOT)),
        "unresolved_symbol_count": len(linked["unresolved_symbols"]),
        "anchor_bindings": linked["anchor_bindings"],
        "unowned_code_contribution_count": linked_report[
            "unowned_code_contribution_count"
        ],
        "function_count": len(reports),
        "counts": dict(sorted(counts.items())),
        "functions": reports,
    }


def render_text(report: dict[str, object], show: str) -> None:
    for source in report["sources"]:
        print(
            f"{source['source']}: {source['counts']} "
            f"anchors={source['unresolved_symbol_count']}"
        )
        for function in source["functions"]:
            result = str(function["result"])
            if show != "all" and result != show:
                continue
            if result == "unresolved":
                detail = function["reason"]
            else:
                detail = (
                    f"linked={function['candidate_size']} target={function['target_size']} "
                    f"comparable={function['matched_comparable_bytes']}/"
                    f"{function['comparable_bytes']} fields={len(function['linkage_fields'])}"
                )
            print(
                f"  {result:16s} {function['address']} "
                f"{function['name']}  {detail}"
            )
    print(
        f"LTCG backlog probe complete: {report['function_count']} function(s), "
        f"{report['counts']}"
    )
    print("anchored linked-image diagnostics grant no exactness credit")


def main() -> int:
    args = parse_args()
    try:
        backlog = load_backlog(args.source)
        by_source: dict[str, list[dict[str, object]]] = defaultdict(list)
        for item in backlog:
            by_source[str(item["source"])].append(item)
        target = verified_target()
        with TOOLS_LOCK.open("rb") as stream:
            decoder_identity = verify_capstone(tomllib.load(stream)["capstone"])
        linker, environment = tool_environment()
        source_reports = [
            probe_source(source, items, target, linker, environment)
            for source, items in sorted(by_source.items())
        ]
        counts: dict[str, int] = defaultdict(int)
        for source in source_reports:
            for result, count in source["counts"].items():
                counts[result] += int(count)
        report: dict[str, object] = {
            "schema_version": 1,
            "result": "ok",
            "acceptance_authority": "none",
            "artifact_kind": "anchored-linked-pe-diagnostic",
            "decoder": {"name": "capstone", **decoder_identity},
            "source_count": len(source_reports),
            "function_count": len(backlog),
            "counts": dict(sorted(counts.items())),
            "sources": source_reports,
        }
    except (
        KeyError,
        LinkedImageError,
        OSError,
        struct.error,
        TypeError,
        ValueError,
        tomllib.TOMLDecodeError,
    ) as exc:
        report = {"result": "error", "error": str(exc)}
    if args.json:
        print(json.dumps(report, indent=2, sort_keys=True))
    elif report["result"] == "error":
        print(f"error: LTCG backlog probe failed: {report['error']}", file=sys.stderr)
    else:
        render_text(report, args.show)
    return 0 if report["result"] == "ok" else 1


if __name__ == "__main__":
    raise SystemExit(main())
