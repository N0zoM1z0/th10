#!/usr/bin/env python3
"""Report and validate the TH10 Enemy ECL dispatcher selector tables."""

from __future__ import annotations

import argparse
import json
from pathlib import Path
import re
import struct
import sys

from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone.x86 import X86_OP_MEM

from linked_image import PEImage
from target_identity import pe_bytes_at, resolve_target, verify_target


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_SOURCE = ROOT / "src" / "EnemyEclDispatcher.cpp"
JUMP_TABLE_ADDRESS = 0x00411D30
JUMP_TABLE_ENTRIES = 108
SELECTOR_TABLE_ADDRESS = 0x00411EE0
SELECTOR_TABLE_ENTRIES = 181
FIRST_OPCODE = 0x100
DEFAULT_DESTINATION = 0x00411D18

ENUM_PATTERN = re.compile(
    r"^\s*(ENEMY_ECL_[A-Z0-9_]+)\s*=\s*(0x[0-9A-Fa-f]+),", re.MULTILINE
)
CASE_PATTERN = re.compile(
    r"^\s*case\s+(ENEMY_ECL_[A-Z0-9_]+):", re.MULTILINE
)


def parse_source(path: Path) -> tuple[dict[str, int], list[str], list[str]]:
    text = path.read_text(encoding="utf-8")
    enum_pairs = ENUM_PATTERN.findall(text)
    enum_values = {name: int(value, 16) for name, value in enum_pairs}
    case_names = CASE_PATTERN.findall(text)
    problems: list[str] = []
    if len(enum_pairs) != len(enum_values):
        problems.append("source has duplicate ECL enum names")
    if len(enum_values.values()) != len(set(enum_values.values())):
        problems.append("source has duplicate ECL enum values")
    if len(case_names) != len(set(case_names)):
        problems.append("source has duplicate ECL case labels")
    missing_cases = sorted(set(enum_values) - set(case_names))
    unknown_cases = sorted(set(case_names) - set(enum_values))
    if missing_cases:
        problems.append("enum entries without cases: " + ", ".join(missing_cases))
    if unknown_cases:
        problems.append("cases without enum entries: " + ", ".join(unknown_cases))
    return enum_values, case_names, problems


def candidate_layout(path: Path, entry: int, size: int | None,
                     target_selectors: bytes, target_jumps: tuple[int, ...],
                     enum_values: dict[str, int]) -> dict[str, object]:
    image = PEImage(path)
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    selector_sites = []
    jump_sites = []
    for instruction in decoder.disasm(image.read_address(entry, 0x100), entry):
        operands = instruction.operands
        if instruction.mnemonic == 'movzx' and len(operands) == 2:
            if operands[1].type == X86_OP_MEM and operands[1].size == 1:
                selector_sites.append(operands[1].mem.disp)
        if instruction.mnemonic == 'jmp' and len(operands) == 1:
            if operands[0].type == X86_OP_MEM and operands[0].size == 4:
                jump_sites.append(operands[0].mem.disp)
    if len(selector_sites) != 1 or len(jump_sites) != 1:
        raise ValueError('candidate entry lacks a unique byte selector and indexed jump')
    selector_address, jump_address = selector_sites[0], jump_sites[0]
    if jump_address + JUMP_TABLE_ENTRIES * 4 != selector_address:
        raise ValueError('candidate jump and selector tables are not adjacent')
    selectors = image.read_address(selector_address, SELECTOR_TABLE_ENTRIES)
    jumps = struct.unpack(f'<{JUMP_TABLE_ENTRIES}I',
                          image.read_address(jump_address, JUMP_TABLE_ENTRIES * 4))
    if any(index >= JUMP_TABLE_ENTRIES for index in selectors):
        raise ValueError('candidate selector exceeds jump table')
    if len(set(jumps)) != JUMP_TABLE_ENTRIES:
        raise ValueError('candidate jump table contains duplicate destinations')
    if any(destination < entry or destination >= jump_address for destination in jumps):
        raise ValueError('candidate jump destination leaves pre-table code')
    if size is not None and size < selector_address + SELECTOR_TABLE_ENTRIES - entry:
        raise ValueError('candidate contribution does not cover selector table')
    target_order = sorted(range(JUMP_TABLE_ENTRIES), key=lambda i: target_jumps[i])
    candidate_order = sorted(range(JUMP_TABLE_ENTRIES), key=lambda i: jumps[i])
    first_order_difference = next((index for index, (a, b) in
                                   enumerate(zip(target_order, candidate_order))
                                   if a != b), None)
    by_opcode = {value: name for name, value in enum_values.items()}
    case_rows = []
    for opcode in sorted(by_opcode):
        index = opcode - FIRST_OPCODE
        if not 0 <= index < len(selectors):
            raise ValueError(f'source opcode leaves selector range: {opcode:#x}')
        target_slot = target_selectors[index]
        candidate_slot = selectors[index]
        case_rows.append({
            'opcode': f'0x{opcode:03X}', 'name': by_opcode[opcode],
            'target_slot': target_slot, 'candidate_slot': candidate_slot,
            'target_relative': target_jumps[target_slot] - 0x0040E770,
            'candidate_relative': jumps[candidate_slot] - entry,
        })
    return {
        'image': str(path), 'function_address': f'0x{entry:08X}',
        'contribution_size': size,
        'jump_table_address': f'0x{jump_address:08X}',
        'selector_table_address': f'0x{selector_address:08X}',
        'selector_equal': selectors == target_selectors,
        'selector_equal_bytes': sum(a == b for a, b in zip(selectors, target_selectors)),
        'target_pre_table_span': JUMP_TABLE_ADDRESS - 0x0040E770,
        'candidate_pre_table_span': jump_address - entry,
        'pre_table_span_delta': (jump_address - entry) - (JUMP_TABLE_ADDRESS - 0x0040E770),
        'target_suffix_bytes': 0x00411FC0 - (SELECTOR_TABLE_ADDRESS + SELECTOR_TABLE_ENTRIES),
        'candidate_suffix_bytes': (entry + size - selector_address - SELECTOR_TABLE_ENTRIES)
                                  if size is not None else None,
        'physical_order_matches': target_order == candidate_order,
        'first_physical_order_difference': first_order_difference,
        'case_rows': case_rows,
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("executable", nargs="?", type=Path)
    parser.add_argument("--source", type=Path, default=DEFAULT_SOURCE)
    parser.add_argument("--candidate", type=Path)
    parser.add_argument("--candidate-function-address", type=lambda value: int(value, 0))
    parser.add_argument("--candidate-contribution-size", type=lambda value: int(value, 0))
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
        selectors = pe_bytes_at(
            image, SELECTOR_TABLE_ADDRESS, SELECTOR_TABLE_ENTRIES
        )
        jump_table = struct.unpack(
            f"<{JUMP_TABLE_ENTRIES}I",
            pe_bytes_at(
                image, JUMP_TABLE_ADDRESS, JUMP_TABLE_ENTRIES * 4
            ),
        )
        enum_values, case_names, source_problems = parse_source(args.source)
        if args.candidate:
            if args.candidate_function_address is None:
                raise ValueError('--candidate-function-address is required with --candidate')
            candidate = candidate_layout(
                args.candidate, args.candidate_function_address,
                args.candidate_contribution_size, selectors, jump_table, enum_values)
        else:
            candidate = None
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
    if len(default_slots) != 1:
        problems.append(
            f"expected one default jump slot, observed {len(default_slots)}"
        )
    active_opcodes = [
        FIRST_OPCODE + index
        for index, selector in enumerate(selectors)
        if selector < len(jump_table)
        and jump_table[selector] != DEFAULT_DESTINATION
    ]
    source_opcodes = sorted(enum_values.values())
    missing_source = sorted(set(active_opcodes) - set(source_opcodes))
    extra_source = sorted(set(source_opcodes) - set(active_opcodes))
    if missing_source:
        problems.append(
            "target opcodes missing from source: "
            + ", ".join(f"0x{value:03X}" for value in missing_source)
        )
    if extra_source:
        problems.append(
            "source opcodes routed to target default: "
            + ", ".join(f"0x{value:03X}" for value in extra_source)
        )

    report = {
        "ok": not problems,
        "target": str(target),
        "target_sha256": observed["sha256"],
        "source": str(args.source),
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
        "active_opcode_values": [f"0x{value:03X}" for value in active_opcodes],
        "candidate_case_layout": candidate,
        "problems": problems,
    }
    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print(f"target OK: {target}")
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
            print("ECL dispatcher coverage OK")
        if candidate:
            print(f"candidate pre-table delta: {candidate['pre_table_span_delta']:+d} bytes; "
                  f"selector equal: {candidate['selector_equal']}; "
                  f"physical order equal: {candidate['physical_order_matches']}")
            print('candidate comparison is diagnostic only; no exactness credit')
    return 1 if args.check and problems else 0


if __name__ == "__main__":
    raise SystemExit(main())
