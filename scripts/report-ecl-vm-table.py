#!/usr/bin/env python3
"""Report and validate the TH10 generic ECL VM selector tables."""

from __future__ import annotations

import argparse
import json
from pathlib import Path
import re
import struct
import sys

from target_identity import pe_bytes_at, resolve_target, verify_target


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_SOURCE = ROOT / "src" / "EclVm.cpp"
FUNCTION_ADDRESS = 0x0044E1A0
CODE_END_ADDRESS = 0x0044FBC1
ALIGNMENT_ADDRESS = 0x0044FBC2
JUMP_TABLE_ADDRESS = 0x0044FBC4
JUMP_TABLE_ENTRIES = 60
SELECTOR_TABLE_ADDRESS = 0x0044FCB4
SELECTOR_TABLE_ENTRIES = 88
OWNER_END_ADDRESS = 0x0044FD0B
NEXT_FUNCTION_ADDRESS = 0x0044FD10
DEFAULT_DESTINATION = 0x0044FB68

ENUM_PATTERN = re.compile(
    r"^\s*(ECL_VM_[A-Z0-9_]+)\s*=\s*(0x[0-9A-Fa-f]+),", re.MULTILINE
)
CASE_PATTERN = re.compile(
    r"^\s*case\s+(ECL_VM_[A-Z0-9_]+):", re.MULTILINE
)


def parse_source(path: Path) -> tuple[dict[str, int], list[str], list[str]]:
    text = path.read_text(encoding="utf-8")
    enum_pairs = ENUM_PATTERN.findall(text)
    enum_values = {name: int(value, 16) for name, value in enum_pairs}
    case_names = CASE_PATTERN.findall(text)
    problems: list[str] = []
    if len(enum_pairs) != len(enum_values):
        problems.append("source has duplicate ECL VM enum names")
    if len(enum_values.values()) != len(set(enum_values.values())):
        problems.append("source has duplicate ECL VM enum values")
    if len(case_names) != len(set(case_names)):
        problems.append("source has duplicate ECL VM case labels")
    missing_cases = sorted(set(enum_values) - set(case_names))
    unknown_cases = sorted(set(case_names) - set(enum_values))
    if missing_cases:
        problems.append("enum entries without cases: " + ", ".join(missing_cases))
    if unknown_cases:
        problems.append("cases without enum entries: " + ", ".join(unknown_cases))
    return enum_values, case_names, problems


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("executable", nargs="?", type=Path)
    parser.add_argument("--source", type=Path, default=DEFAULT_SOURCE)
    parser.add_argument("--json", action="store_true")
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()

    target = resolve_target(args.executable)
    if not target.is_file():
        print(f"missing target: {target}", file=sys.stderr)
        return 1
    try:
        observed, problems = verify_target(target)
        if problems:
            raise ValueError("; ".join(problems))
        image = target.read_bytes()
        jump_table = struct.unpack(
            f"<{JUMP_TABLE_ENTRIES}I",
            pe_bytes_at(image, JUMP_TABLE_ADDRESS, JUMP_TABLE_ENTRIES * 4),
        )
        selectors = pe_bytes_at(
            image, SELECTOR_TABLE_ADDRESS, SELECTOR_TABLE_ENTRIES
        )
        alignment = pe_bytes_at(image, ALIGNMENT_ADDRESS, 2)
        following_padding = pe_bytes_at(image, OWNER_END_ADDRESS + 1, 4)
        enum_values, case_names, source_problems = parse_source(args.source)
    except (OSError, UnicodeError, ValueError, struct.error) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 2

    problems = list(source_problems)
    invalid_selectors = sorted(set(selectors) - set(range(JUMP_TABLE_ENTRIES)))
    if invalid_selectors:
        problems.append(
            "selector bytes outside the jump table: "
            + ", ".join(str(value) for value in invalid_selectors)
        )
    default_slots = [
        index for index, destination in enumerate(jump_table)
        if destination == DEFAULT_DESTINATION
    ]
    if default_slots != [JUMP_TABLE_ENTRIES - 1]:
        problems.append(
            "expected the final jump-table slot to be the sole default, observed "
            + repr(default_slots)
        )
    if alignment != b"\x8b\xff":
        problems.append(f"unexpected function/table alignment bytes: {alignment.hex()}")
    if following_padding != b"\xcc\xcc\xcc\xcc":
        problems.append(
            "expected four CC bytes between the selector table and next function"
        )

    active_opcodes = [
        opcode
        for opcode, selector in enumerate(selectors)
        if selector < len(jump_table)
        and jump_table[selector] != DEFAULT_DESTINATION
    ]
    source_opcodes = sorted(enum_values.values())
    missing_source = sorted(set(active_opcodes) - set(source_opcodes))
    extra_source = sorted(set(source_opcodes) - set(active_opcodes))
    if missing_source:
        problems.append(
            "target opcodes missing from source: "
            + ", ".join(f"0x{value:02X}" for value in missing_source)
        )
    if extra_source:
        problems.append(
            "source opcodes routed to target default: "
            + ", ".join(f"0x{value:02X}" for value in extra_source)
        )

    report = {
        "ok": not problems,
        "target": str(target),
        "target_sha256": observed["sha256"],
        "source": str(args.source),
        "physical_owner": {
            "start": f"0x{FUNCTION_ADDRESS:08X}",
            "code_end": f"0x{CODE_END_ADDRESS:08X}",
            "owner_end": f"0x{OWNER_END_ADDRESS:08X}",
            "size": OWNER_END_ADDRESS - FUNCTION_ADDRESS + 1,
            "next_function": f"0x{NEXT_FUNCTION_ADDRESS:08X}",
        },
        "jump_table": {
            "address": f"0x{JUMP_TABLE_ADDRESS:08X}",
            "entries": len(jump_table),
            "unique_destinations": len(set(jump_table)),
            "default_destination": f"0x{DEFAULT_DESTINATION:08X}",
            "default_slots": default_slots,
        },
        "selector_table": {
            "address": f"0x{SELECTOR_TABLE_ADDRESS:08X}",
            "entries": len(selectors),
            "unique_slots": len(set(selectors)),
            "active_opcodes": len(active_opcodes),
            "default_opcodes": len(selectors) - len(active_opcodes),
        },
        "source_enum_entries": len(enum_values),
        "source_case_labels": len(case_names),
        "active_opcode_values": [f"0x{value:02X}" for value in active_opcodes],
        "problems": problems,
    }
    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print(f"target OK: {target}")
        print(
            f"physical owner: 0x{FUNCTION_ADDRESS:08X}-0x{OWNER_END_ADDRESS:08X} "
            f"({OWNER_END_ADDRESS - FUNCTION_ADDRESS + 1} bytes)"
        )
        print(
            f"jump table: {len(jump_table)} slots / "
            f"{len(set(jump_table))} unique destinations"
        )
        print(
            f"selector table: {len(selectors)} opcodes / "
            f"{len(active_opcodes)} active / "
            f"{len(selectors) - len(active_opcodes)} default"
        )
        print(
            f"source: {len(enum_values)} enum entries / "
            f"{len(case_names)} case labels"
        )
        if problems:
            for problem in problems:
                print(f"problem: {problem}", file=sys.stderr)
        else:
            print("generic ECL VM table coverage OK")
    return 1 if args.check and problems else 0


if __name__ == "__main__":
    raise SystemExit(main())
