#!/usr/bin/env python3
"""Audit tracked TH10 function extents against the pinned target image."""

from __future__ import annotations

import argparse
from bisect import bisect_right
from collections import Counter
import csv
import json
from pathlib import Path
import re
import struct
import sys
import tomllib

from linked_image import LinkedImageError, verify_capstone
from target_identity import load_manifest, pe_bytes_at, resolve_target, verify_target


ROOT = Path(__file__).resolve().parents[1]
CONFIG = ROOT / "config"
BOUNDARIES = CONFIG / "function-boundaries.csv"
DEFAULT_GHIDRA_RANGES = ROOT / ".analysis" / "boundaries" / "function-body-ranges.csv"
EVIDENCE_ID = "target-boundary-audit-2026-09-15"
ALIGNMENT_BYTES = frozenset((0x90, 0xCC))
LEDGER_FIELDS = (
    "address",
    "span_end",
    "state",
    "confidence",
    "evidence_id",
    "notes",
)


def rows(name: str) -> list[dict[str, str]]:
    with (CONFIG / name).open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream))


def address(value: str) -> int:
    return int(value, 0)


def format_address(value: int) -> str:
    return f"0x{value:08X}"


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--json", action="store_true", help="emit the full JSON report")
    parser.add_argument(
        "--write-ledger",
        action="store_true",
        help="replace the boundary ledger with this target-bound audit",
    )
    parser.add_argument(
        "--check-ledger",
        action="store_true",
        help="require every recorded automatic result to agree with this audit",
    )
    parser.add_argument(
        "--ghidra-ranges",
        type=Path,
        default=DEFAULT_GHIDRA_RANGES,
        help=(
            "body-range CSV from `scripts/repo-python scripts/ghidra.py architecture`; the "
            "default is used when present"
        ),
    )
    return parser.parse_args()


def overlapping_addresses(
    extents: list[tuple[int, int]],
) -> tuple[set[int], list[dict[str, str]]]:
    active: list[tuple[int, int]] = []
    participants: set[int] = set()
    pairs = []
    for start, end in extents:
        active = [(left, right) for left, right in active if right >= start]
        for left, right in active:
            participants.update((left, start))
            pairs.append(
                {
                    "left": format_address(left),
                    "left_end": format_address(right),
                    "right": format_address(start),
                    "right_end": format_address(end),
                }
            )
        active.append((start, end))
    return participants, pairs


def containing_extent(
    starts: list[int], ends: dict[int, int], destination: int
) -> tuple[int, int] | None:
    index = bisect_right(starts, destination) - 1
    while index >= 0:
        start = starts[index]
        if ends[start] >= destination:
            return start, ends[start]
        index -= 1
    return None


def load_ghidra_ranges(path: Path | None) -> dict[int, list[dict[str, str]]]:
    if path is None or not path.is_file():
        return {}
    grouped: dict[int, list[dict[str, str]]] = {}
    with path.open(newline="", encoding="utf-8") as stream:
        for row in csv.DictReader(stream):
            grouped.setdefault(address(row["address"]), []).append(row)
    for start, group in grouped.items():
        expected = int(group[0]["range_count"])
        if len(group) != expected:
            raise ValueError(
                f"incomplete Ghidra body-range group at {format_address(start)}"
            )
        indices = [int(row["range_index"]) for row in group]
        if indices != list(range(expected)):
            raise ValueError(
                f"unordered Ghidra body ranges at {format_address(start)}"
            )
    return grouped


def ghidra_range_owner(
    grouped: dict[int, list[dict[str, str]]], destination: int
) -> int | None:
    for owner, ranges in grouped.items():
        for row in ranges:
            if address(row["range_start"]) <= destination <= address(row["range_end"]):
                return owner
    return None


def merge_extents(extents: list[tuple[int, int]]) -> list[tuple[int, int]]:
    merged: list[list[int]] = []
    for start, end in extents:
        if not merged or start > merged[-1][1] + 1:
            merged.append([start, end])
        else:
            merged[-1][1] = max(merged[-1][1], end)
    return [(start, end) for start, end in merged]


def section_pointer_counts(
    image: bytes, observed: dict[str, object], text_start: int, text_end: int
) -> Counter[int]:
    pointers: Counter[int] = Counter()
    pe = observed["pe"]
    assert isinstance(pe, dict)
    sections = pe["sections"]
    assert isinstance(sections, list)
    for raw_section in sections:
        assert isinstance(raw_section, dict)
        if raw_section["name"] == ".text":
            continue
        raw_offset = int(raw_section["raw_offset"])
        raw_size = int(raw_section["raw_size"])
        data = image[raw_offset : raw_offset + raw_size]
        for offset in range(0, len(data) - 3, 4):
            candidate = struct.unpack_from("<I", data, offset)[0]
            if text_start <= candidate <= text_end:
                pointers[candidate] += 1
    return pointers


def audit(ghidra_ranges_path: Path | None = DEFAULT_GHIDRA_RANGES) -> dict[str, object]:
    try:
        from capstone import (
            CS_ARCH_X86,
            CS_GRP_CALL,
            CS_GRP_JUMP,
            CS_MODE_32,
            Cs,
        )
        from capstone.x86_const import X86_OP_IMM, X86_OP_MEM
    except ImportError as exc:
        raise LinkedImageError("Capstone is required for boundary review") from exc

    with (CONFIG / "tools.lock.toml").open("rb") as stream:
        decoder_identity = verify_capstone(tomllib.load(stream)["capstone"])
    manifest = load_manifest()
    target_path = resolve_target()
    observed, problems = verify_target(target_path)
    if problems:
        raise ValueError("target identity mismatch: " + "; ".join(problems))
    image = target_path.read_bytes()
    function_rows = rows("functions.csv")
    exact_starts = {
        address(row["address"]) for row in rows("matches.csv")
    }
    text_start = address(str(manifest["pe"]["text_start"]))
    text_end = address(str(manifest["pe"]["text_end"]))
    extents = [
        (address(row["address"]), address(row["span_end"]))
        for row in function_rows
    ]
    starts = [start for start, _ in extents]
    ends = dict(extents)
    overlap_addresses, overlap_pairs = overlapping_addresses(extents)
    ghidra_ranges = load_ghidra_ranges(ghidra_ranges_path)
    sparse_ghidra_functions = {
        start for start, group in ghidra_ranges.items() if len(group) > 1
    }
    remote_ghidra_functions = {
        start
        for start, group in ghidra_ranges.items()
        if any(address(row["range_start"]) < start for row in group)
    }

    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    decoded: dict[int, dict[str, object]] = {}
    raw_direct_references: list[tuple[int, int, int]] = []
    raw_absolute_references: list[tuple[int, int, int]] = []
    raw_indirect_control_references: list[tuple[int, int, int]] = []
    for start, end in extents:
        data = pe_bytes_at(image, start, end - start + 1)
        consumed = 0
        instructions = []
        for instruction in decoder.disasm(data, start):
            if instruction.address != start + consumed:
                break
            consumed += instruction.size
            instructions.append(instruction)
            is_control = instruction.group(CS_GRP_CALL) or instruction.group(
                CS_GRP_JUMP
            )
            if is_control and instruction.operands:
                operand = instruction.operands[0]
                if operand.type == X86_OP_IMM:
                    destination = int(operand.imm) & 0xFFFFFFFF
                    if text_start <= destination <= text_end:
                        raw_direct_references.append(
                            (start, instruction.address, destination)
                        )
                elif (
                    operand.type == X86_OP_MEM
                    and instruction.mnemonic == "jmp"
                    and operand.mem.index != 0
                    and operand.mem.scale == 4
                ):
                    destination = int(operand.mem.disp) & 0xFFFFFFFF
                    if text_start <= destination <= text_end:
                        raw_indirect_control_references.append(
                            (start, instruction.address, destination)
                        )
            for operand in instruction.operands:
                candidate = None
                if (
                    operand.type == X86_OP_IMM
                    and not is_control
                    and instruction.mnemonic in {"mov", "push"}
                ):
                    candidate = int(operand.imm) & 0xFFFFFFFF
                if candidate is not None and text_start <= candidate <= text_end:
                    raw_absolute_references.append(
                        (start, instruction.address, candidate)
                    )
        decoded[start] = {
            "bytes": consumed,
            "last_mnemonic": instructions[-1].mnemonic if instructions else "",
            # A one- or two-byte C2/CA suffix is not data or padding: it is a
            # near/far RET whose imm16 operand was cut off by the recorded end.
            # Keep this explicit because Capstone quite correctly stops before
            # the incomplete instruction, while a retained manual review used
            # to override that evidence and leave the owner ending mid-opcode.
            "truncated_ret_immediate": (
                0 < len(data) - consumed < 3
                and data[consumed] in (0xC2, 0xCA)
            ),
        }

    # Do not promote references decoded from overlapping extents. Those spans are
    # exactly the cases where a linear pass can interpret another entry as a body.
    direct_references = [
        item for item in raw_direct_references if item[0] not in overlap_addresses
    ]
    absolute_references = [
        item for item in raw_absolute_references if item[0] not in overlap_addresses
    ]
    indirect_control_references = [
        item
        for item in raw_indirect_control_references
        if item[0] not in overlap_addresses
    ]
    owned_indirect_control_bases: dict[int, int] = {}
    for source, _, destination in indirect_control_references:
        if source <= destination <= ends[source]:
            previous = owned_indirect_control_bases.get(source)
            if previous is None or destination < previous:
                owned_indirect_control_bases[source] = destination
    entry_references: Counter[int] = Counter()
    absolute_entry_references: Counter[int] = Counter()
    interior_references: dict[int, list[dict[str, str]]] = {}
    for source, instruction, destination in direct_references:
        if destination in ends and destination != source:
            entry_references[destination] += 1
            continue
        containing = containing_extent(starts, ends, destination)
        if containing is None or containing[0] == source:
            continue
        owner_start, owner_end = containing
        interior_references.setdefault(owner_start, []).append(
            {
                "source": format_address(source),
                "instruction": format_address(instruction),
                "destination": format_address(destination),
                "owner_end": format_address(owner_end),
            }
        )
    for source, _, destination in absolute_references:
        if destination in ends and destination != source:
            absolute_entry_references[destination] += 1

    pointer_references = section_pointer_counts(
        image, observed, text_start, text_end
    )
    entry_point = int(observed["pe"]["entry_point"])
    boundary_rows = []
    state_counts: Counter[str] = Counter()
    for row, (start, end) in zip(function_rows, extents):
        size = end - start + 1
        result = decoded[start]
        decoded_bytes = int(result["bytes"])
        last_mnemonic = str(result["last_mnemonic"])
        truncated_ret_immediate = bool(result["truncated_ret_immediate"])
        table_base = owned_indirect_control_bases.get(start)
        complete = decoded_bytes == size
        terminal = last_mnemonic.startswith("ret") or last_mnemonic == "jmp"
        historical_text = row["evidence"] + " " + row["notes"]
        manual_boundary_review = bool(
            "boundary-reviewed" in historical_text.lower()
            or "cc-delimited" in historical_text.lower()
            or re.search(
                r"\breviewed\b[^.;]{0,96}\b"
                r"(?:target\s+)?(?:extent|span|body|boundary|owner|contribution)\b",
                historical_text,
                re.I,
            )
            or re.search(
                r"\b(?:extent|span|owner|contribution|body)\b[^.;]{0,96}"
                r"0x[0-9a-f]{8}-0x[0-9a-f]{8}",
                historical_text,
                re.I,
            )
        )
        prior_review = start in exact_starts or manual_boundary_review
        conflicts = []
        if start in overlap_addresses:
            conflicts.append("overlapping-extents")
        if start in interior_references:
            conflicts.append("external-interior-branch")
        if start in remote_ghidra_functions:
            conflicts.append("remote-ghidra-body")
        code_refs = entry_references[start]
        data_refs = pointer_references[start]
        absolute_refs = absolute_entry_references[start]
        entry_parts = []
        if start == entry_point:
            entry_parts.append("pe-entry")
        if code_refs:
            entry_parts.append(f"direct-code-refs={code_refs}")
        if absolute_refs:
            entry_parts.append(f"absolute-code-refs={absolute_refs}")
        if data_refs:
            entry_parts.append(f"aligned-data-refs={data_refs}")
        if table_base is not None:
            entry_parts.append(
                f"owned-indirect-control-table={format_address(table_base)}"
            )
        if truncated_ret_immediate:
            state, confidence = "needs_review", "low"
            evidence = EVIDENCE_ID
            notes = (
                f"Recorded span truncates a RET imm16 instruction after "
                f"{decoded_bytes}/{size} bytes"
            )
            if entry_parts:
                notes += "; " + ", ".join(entry_parts)
        elif prior_review:
            state, confidence = "reviewed", "high"
            evidence = f"prior-target-boundary-review+{EVIDENCE_ID}"
            if table_base is None:
                notes = (
                    f"Prior boundary evidence retained; dense decode "
                    f"{decoded_bytes}/{size}, tail {last_mnemonic or 'none'}"
                )
            else:
                notes = (
                    f"Prior boundary evidence retained; dense decode "
                    f"{decoded_bytes}/{size}; owned indirect control table at "
                    f"{format_address(table_base)}"
                )
            if conflicts:
                notes += "; automated diagnostic: " + "+".join(conflicts)
        elif complete and terminal and not conflicts and entry_parts:
            state, confidence = "reviewed", "high"
            evidence = EVIDENCE_ID
            notes = (
                f"Complete dense decode ends in {last_mnemonic}; "
                + ", ".join(entry_parts)
                + "; no overlap or external interior branch"
            )
        elif complete and terminal and not conflicts:
            state, confidence = "provisional", "medium"
            evidence = EVIDENCE_ID
            notes = (
                f"Complete dense decode ends in {last_mnemonic}, but no independent "
                "entry reference was found"
            )
        else:
            state, confidence = "needs_review", "low"
            evidence = EVIDENCE_ID
            diagnostic = []
            if not complete:
                diagnostic.append(f"dense-decode={decoded_bytes}/{size}")
            if not terminal:
                diagnostic.append(f"tail={last_mnemonic or 'none'}")
            diagnostic.extend(conflicts)
            notes = "; ".join(diagnostic)
            if entry_parts:
                notes += "; " + ", ".join(entry_parts)
        state_counts[state] += 1
        boundary_rows.append(
            {
                "address": format_address(start),
                "span_end": format_address(end),
                "state": state,
                "confidence": confidence,
                "evidence_id": evidence,
                "notes": notes,
            }
        )

    merged = merge_extents(extents)
    gaps = []
    cursor = text_start
    for start, end in merged:
        if cursor < start:
            gaps.append((cursor, start - 1))
        cursor = max(cursor, end + 1)
    if cursor <= text_end:
        gaps.append((cursor, text_end))
    gap_rows = []
    pure_alignment_gaps = 0
    pure_alignment_bytes = 0
    for start, end in gaps:
        data = pe_bytes_at(image, start, end - start + 1)
        prefix = 0
        while prefix < len(data) and data[prefix] in ALIGNMENT_BYTES:
            prefix += 1
        suffix = 0
        while suffix < len(data) - prefix and data[len(data) - 1 - suffix] in ALIGNMENT_BYTES:
            suffix += 1
        pure = prefix == len(data)
        if pure:
            pure_alignment_gaps += 1
            pure_alignment_bytes += len(data)
        gap_rows.append(
            {
                "start": format_address(start),
                "end": format_address(end),
                "size": len(data),
                "kind": "alignment" if pure else "unresolved",
                "alignment_prefix": prefix,
                "alignment_suffix": suffix,
            }
        )

    untracked_code_entries: Counter[int] = Counter()
    untracked_code_sources: dict[int, list[dict[str, str]]] = {}
    remote_body_targets: Counter[tuple[int, int]] = Counter()
    for source, instruction, destination in direct_references:
        if containing_extent(starts, ends, destination) is None:
            remote_owner = ghidra_range_owner(ghidra_ranges, destination)
            if remote_owner is not None:
                remote_body_targets[(destination, remote_owner)] += 1
                continue
            untracked_code_entries[destination] += 1
            untracked_code_sources.setdefault(destination, []).append(
                {
                    "source": format_address(source),
                    "instruction": format_address(instruction),
                }
            )
    untracked_data_entries = {
        candidate: count
        for candidate, count in pointer_references.items()
        if containing_extent(starts, ends, candidate) is None
        and ghidra_range_owner(ghidra_ranges, candidate) is None
    }
    untracked_entries = []
    for candidate in sorted(
        set(untracked_code_entries) | set(untracked_data_entries)
    ):
        untracked_entries.append(
            {
                "address": format_address(candidate),
                "direct_code_references": untracked_code_entries[candidate],
                "aligned_data_references": untracked_data_entries.get(candidate, 0),
                "code_reference_sites": untracked_code_sources.get(candidate, []),
            }
        )

    remote_body_rows = [
        {
            "address": format_address(candidate),
            "owner": format_address(owner),
            "direct_code_references": count,
        }
        for (candidate, owner), count in sorted(remote_body_targets.items())
    ]

    indirect_control_rows = []
    for source, instruction, destination in indirect_control_references:
        owner = containing_extent(starts, ends, destination)
        indirect_control_rows.append(
            {
                "source": format_address(source),
                "instruction": format_address(instruction),
                "destination": format_address(destination),
                "owner": format_address(owner[0]) if owner is not None else None,
                "owned_by_source": owner is not None and owner[0] == source,
            }
        )
    unowned_indirect_control_rows = [
        row for row in indirect_control_rows if not row["owned_by_source"]
    ]

    text_size = text_end - text_start + 1
    union_bytes = sum(end - start + 1 for start, end in merged)
    total_gap_bytes = sum(row["size"] for row in gap_rows)
    unresolved_gaps = [row for row in gap_rows if row["kind"] == "unresolved"]
    return {
        "target": {
            "path": str(target_path),
            "sha256": observed["sha256"],
            "text_start": format_address(text_start),
            "text_end": format_address(text_end),
            "text_bytes": text_size,
        },
        "decoder": {"name": "capstone", **decoder_identity},
        "summary": {
            "tracked_candidates": len(extents),
            "reviewed": state_counts["reviewed"],
            "provisional": state_counts["provisional"],
            "needs_review": state_counts["needs_review"],
            "overlap_pairs": len(overlap_pairs),
            "overlap_candidates": len(overlap_addresses),
            "tracked_union_bytes": union_bytes,
            "untracked_gap_bytes": total_gap_bytes,
            "pure_alignment_gaps": pure_alignment_gaps,
            "pure_alignment_bytes": pure_alignment_bytes,
            "unresolved_gaps": len(unresolved_gaps),
            "unresolved_gap_bytes": total_gap_bytes - pure_alignment_bytes,
            "untracked_direct_targets": len(untracked_code_entries),
            "untracked_data_targets": len(untracked_data_entries),
            "ghidra_body_functions": len(ghidra_ranges),
            "ghidra_sparse_body_functions": len(sparse_ghidra_functions),
            "ghidra_remote_body_functions": len(remote_ghidra_functions),
            "remote_body_direct_targets": len(remote_body_targets),
            "indirect_control_tables": len(indirect_control_rows),
            "unowned_indirect_control_tables": len(unowned_indirect_control_rows),
        },
        "boundaries": boundary_rows,
        "overlaps": overlap_pairs,
        "gaps": gap_rows,
        "untracked_entries": untracked_entries,
        "remote_body_targets": remote_body_rows,
        "indirect_control_tables": indirect_control_rows,
        "ghidra_body_ranges": {
            "path": str(ghidra_ranges_path) if ghidra_ranges_path else None,
            "available": bool(ghidra_ranges),
            "function_count": len(ghidra_ranges),
            "range_count": sum(len(group) for group in ghidra_ranges.values()),
            "sparse_function_count": len(sparse_ghidra_functions),
            "remote_function_count": len(remote_ghidra_functions),
        },
    }


def write_ledger(report: dict[str, object]) -> None:
    raw_rows = report["boundaries"]
    assert isinstance(raw_rows, list)
    with BOUNDARIES.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(stream, fieldnames=LEDGER_FIELDS, lineterminator="\n")
        writer.writeheader()
        writer.writerows(raw_rows)
    print(f"wrote {BOUNDARIES.relative_to(ROOT)}", file=sys.stderr)


def check_ledger(report: dict[str, object]) -> None:
    expected = report["boundaries"]
    if not BOUNDARIES.is_file():
        raise ValueError("boundary ledger is missing")
    actual = rows("function-boundaries.csv")
    if actual != expected:
        for index, (left, right) in enumerate(zip(actual, expected)):
            if left != right:
                raise ValueError(
                    f"boundary ledger differs from target audit at row {index + 2}: "
                    f"recorded={left!r} observed={right!r}"
                )
        raise ValueError(
            f"boundary ledger row count differs: recorded={len(actual)} "
            f"observed={len(expected)}"
        )


def print_report(report: dict[str, object]) -> None:
    summary = report["summary"]
    assert isinstance(summary, dict)
    print(f"target:                     {report['target']['sha256']}")
    print(f"tracked candidates:         {summary['tracked_candidates']}")
    print(
        "boundary states:           "
        f"{summary['reviewed']} reviewed / {summary['provisional']} provisional / "
        f"{summary['needs_review']} needs review"
    )
    print(
        "overlapping extents:       "
        f"{summary['overlap_pairs']} pairs / {summary['overlap_candidates']} candidates"
    )
    print(f"tracked .text union bytes:  {summary['tracked_union_bytes']}")
    print(f"untracked .text gap bytes:  {summary['untracked_gap_bytes']}")
    print(
        "alignment-only gaps:       "
        f"{summary['pure_alignment_gaps']} / {summary['pure_alignment_bytes']} bytes"
    )
    print(
        "unresolved gaps:           "
        f"{summary['unresolved_gaps']} / {summary['unresolved_gap_bytes']} bytes"
    )
    print(
        "untracked target evidence: "
        f"{summary['untracked_direct_targets']} direct / "
        f"{summary['untracked_data_targets']} data-pointer"
    )
    print(
        "indirect control tables:    "
        f"{summary['indirect_control_tables']} referenced / "
        f"{summary['unowned_indirect_control_tables']} outside their owners"
    )
    if summary["ghidra_body_functions"]:
        print(
            "Ghidra body ranges:         "
            f"{summary['ghidra_body_functions']} functions / "
            f"{summary['ghidra_sparse_body_functions']} sparse / "
            f"{summary['ghidra_remote_body_functions']} remote"
        )
    unresolved = sorted(
        (row for row in report["gaps"] if row["kind"] == "unresolved"),
        key=lambda row: (-int(row["size"]), row["start"]),
    )
    if unresolved:
        print("largest unresolved gaps:")
        for row in unresolved[:12]:
            print(
                f"  {row['start']}-{row['end']} {row['size']:5d} bytes "
                f"(edge alignment {row['alignment_prefix']}+{row['alignment_suffix']})"
            )
    code_entries = [
        row
        for row in report["untracked_entries"]
        if row["direct_code_references"]
    ]
    if code_entries:
        print("untracked code-target candidates:")
        for row in code_entries:
            print(
                f"  {row['address']} code refs={row['direct_code_references']} "
                f"data refs={row['aligned_data_references']}"
            )
    unowned_tables = [
        row
        for row in report["indirect_control_tables"]
        if not row["owned_by_source"]
    ]
    if unowned_tables:
        print("indirect control tables outside their source owners:")
        for row in unowned_tables:
            print(
                f"  {row['source']} at {row['instruction']} -> "
                f"{row['destination']} (recorded owner {row['owner'] or 'none'})"
            )


def main() -> int:
    args = parse_args()
    try:
        if (args.write_ledger or args.check_ledger) and not args.ghidra_ranges.is_file():
            raise ValueError(
                "Ghidra body ranges are required for ledger writes/checks; run "
                "`scripts/repo-python scripts/ghidra.py architecture` first"
            )
        report = audit(args.ghidra_ranges)
        if args.write_ledger:
            write_ledger(report)
        if args.check_ledger:
            check_ledger(report)
    except (OSError, KeyError, TypeError, ValueError, LinkedImageError, tomllib.TOMLDecodeError) as exc:
        print(f"error: boundary inventory failed: {exc}", file=sys.stderr)
        return 1
    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print_report(report)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
