#!/usr/bin/env python3
"""Report and validate the TH10 generic ECL VM selector tables."""

from __future__ import annotations

import argparse
import json
from pathlib import Path
import re
import struct
import sys
import tomllib

from linked_image import LinkedImageError, PEImage, verify_capstone
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


def candidate_case_layout(
    path: Path,
    function_address: int,
    selectors: bytes,
    target_jump_table: tuple[int, ...],
    enum_values: dict[str, int],
) -> dict[str, object]:
    image = PEImage(path)
    raw_hits = []
    cursor = 0
    while True:
        hit = image.data.find(selectors, cursor)
        if hit < 0:
            break
        raw_hits.append(hit)
        cursor = hit + 1
    if len(raw_hits) != 1:
        raise ValueError(
            "candidate must contain exactly one target selector table, found "
            f"{len(raw_hits)}"
        )

    selector_raw = raw_hits[0]
    selector_address = None
    for section in image.sections:
        relative = selector_raw - section.raw_offset
        if 0 <= relative and relative + len(selectors) <= section.raw_size:
            selector_address = image.image_base + section.rva + relative
            break
    if selector_address is None:
        raise ValueError("candidate selector table is not mapped by one PE section")

    jump_table_address = selector_address - JUMP_TABLE_ENTRIES * 4
    jump_table = struct.unpack(
        f"<{JUMP_TABLE_ENTRIES}I",
        image.read_address(jump_table_address, JUMP_TABLE_ENTRIES * 4),
    )
    if len(set(jump_table)) != JUMP_TABLE_ENTRIES:
        raise ValueError("candidate jump table does not have 60 unique destinations")
    invalid_destinations = [
        destination
        for destination in jump_table
        if not function_address <= destination < jump_table_address
    ]
    if invalid_destinations:
        raise ValueError(
            "candidate jump-table destination leaves the function code extent: "
            + ", ".join(f"0x{value:08X}" for value in invalid_destinations[:4])
        )

    def destination_gaps(
        table: tuple[int, ...], limit: int
    ) -> dict[int, int]:
        destinations = sorted(set(table))
        following = destinations[1:] + [limit]
        return {
            destination: next_destination - destination
            for destination, next_destination in zip(destinations, following)
        }

    target_gaps = destination_gaps(target_jump_table, ALIGNMENT_ADDRESS)
    candidate_gaps = destination_gaps(jump_table, jump_table_address)
    names_by_opcode = {value: name for name, value in enum_values.items()}
    rows = []
    for opcode, selector in enumerate(selectors):
        target_destination = target_jump_table[selector]
        if target_destination == DEFAULT_DESTINATION:
            continue
        candidate_destination = jump_table[selector]
        target_relative = target_destination - FUNCTION_ADDRESS
        candidate_relative = candidate_destination - function_address
        target_gap = target_gaps[target_destination]
        candidate_gap = candidate_gaps[candidate_destination]
        rows.append(
            {
                "opcode": f"0x{opcode:02X}",
                "name": names_by_opcode.get(opcode, "unknown"),
                "target_destination": f"0x{target_destination:08X}",
                "candidate_destination": f"0x{candidate_destination:08X}",
                "target_relative": target_relative,
                "candidate_relative": candidate_relative,
                "relative_delta": candidate_relative - target_relative,
                "target_next_destination_gap": target_gap,
                "candidate_next_destination_gap": candidate_gap,
                "gap_delta": candidate_gap - target_gap,
            }
        )
    target_physical_order = [
        row["opcode"]
        for row in sorted(
            rows, key=lambda row: int(row["target_destination"], 16)
        )
    ]
    candidate_physical_order = [
        row["opcode"]
        for row in sorted(
            rows, key=lambda row: int(row["candidate_destination"], 16)
        )
    ]
    return {
        "image": str(path),
        "image_sha256": image.sha256,
        "function_address": f"0x{function_address:08X}",
        "target_code_size": CODE_END_ADDRESS - FUNCTION_ADDRESS + 1,
        "target_pre_table_size": JUMP_TABLE_ADDRESS - FUNCTION_ADDRESS,
        "candidate_pre_table_size": jump_table_address - function_address,
        "pre_table_size_delta": (
            jump_table_address - function_address
            - (JUMP_TABLE_ADDRESS - FUNCTION_ADDRESS)
        ),
        "jump_table_address": f"0x{jump_table_address:08X}",
        "selector_table_address": f"0x{selector_address:08X}",
        "physical_order_matches": (
            candidate_physical_order == target_physical_order
        ),
        "target_physical_order": target_physical_order,
        "candidate_physical_order": candidate_physical_order,
        "absolute_case_gap_delta": sum(abs(row["gap_delta"]) for row in rows),
        "equal_case_span_count": sum(row["gap_delta"] == 0 for row in rows),
        "aligned_destination_count": sum(
            row["relative_delta"] == 0 for row in rows
        ),
        "case_rows": rows,
    }


def stack_displacement_diagnostics(
    target: bytes, candidate_path: Path, rows: list[dict[str, object]]
) -> dict[str, object]:
    """Compare encoded ESP offsets, without identifying locals or dataflow."""
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    from capstone.x86 import X86_OP_MEM, X86_REG_ESP

    lock = tomllib.loads((ROOT / "config" / "tools.lock.toml").read_text())
    decoder_identity = verify_capstone(lock["capstone"])
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    candidate = PEImage(candidate_path)
    cases = []
    for row in rows:
        target_address = int(row["target_destination"], 16)
        candidate_address = int(row["candidate_destination"], 16)
        target_size = row["target_next_destination_gap"]
        candidate_size = row["candidate_next_destination_gap"]
        target_instructions = list(decoder.disasm(
            pe_bytes_at(target, target_address, target_size), target_address
        ))
        candidate_instructions = list(decoder.disasm(
            candidate.read_address(candidate_address, candidate_size),
            candidate_address,
        ))

        def shape(instructions, start):
            return [
                (instruction.address - start, instruction.size, instruction.id,
                 tuple((operand.type, operand.size)
                       for operand in instruction.operands))
                for instruction in instructions
            ]

        layout_matches = (
            sum(instruction.size for instruction in target_instructions)
            == target_size
            and sum(instruction.size for instruction in candidate_instructions)
            == candidate_size
            and shape(target_instructions, target_address)
            == shape(candidate_instructions, candidate_address)
        )
        compared = 0
        differences = []
        if layout_matches:
            for original, rebuilt in zip(
                target_instructions, candidate_instructions
            ):
                for index, (left, right) in enumerate(zip(
                    original.operands, rebuilt.operands
                )):
                    if not (
                        left.type == right.type == X86_OP_MEM
                        and left.mem.base == right.mem.base == X86_REG_ESP
                        and left.mem.index == right.mem.index == 0
                    ):
                        continue
                    compared += 1
                    if left.mem.disp != right.mem.disp:
                        differences.append({
                            "instruction_offset": (
                                f"0x{original.address - target_address:X}"
                            ),
                            "mnemonic": original.mnemonic,
                            "operand_index": index,
                            "target_displacement": left.mem.disp,
                            "candidate_displacement": right.mem.disp,
                        })
        cases.append({
            "opcode": row["opcode"],
            "name": row["name"],
            "instruction_layout_matches": layout_matches,
            "stack_operands_compared": compared,
            "differences": differences,
        })
    return {
        "acceptance_authority": "none",
        "method": "encoded ESP offsets within identical instruction layouts",
        "decoder": decoder_identity,
        "aligned_case_count": sum(
            case["instruction_layout_matches"] for case in cases
        ),
        "stack_operands_compared": sum(
            case["stack_operands_compared"] for case in cases
        ),
        "difference_count": sum(len(case["differences"]) for case in cases),
        "cases": cases,
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("executable", nargs="?", type=Path)
    parser.add_argument("--source", type=Path, default=DEFAULT_SOURCE)
    parser.add_argument("--candidate", type=Path)
    parser.add_argument(
        "--candidate-function-address", type=lambda value: int(value, 0)
    )
    parser.add_argument("--json", action="store_true")
    parser.add_argument("--check", action="store_true")
    parser.add_argument(
        "--stack-displacements", action="store_true",
        help="diagnose ESP offsets in cases with identical instruction layouts",
    )
    args = parser.parse_args()
    if (args.candidate is None) != (args.candidate_function_address is None):
        parser.error(
            "--candidate and --candidate-function-address must be supplied together"
        )
    if args.stack_displacements and args.candidate is None:
        parser.error("--stack-displacements requires --candidate")

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
        candidate_layout = None
        stack_diagnostics = None
        if args.candidate is not None:
            candidate_layout = candidate_case_layout(
                args.candidate,
                args.candidate_function_address,
                selectors,
                jump_table,
                enum_values,
            )
            if args.stack_displacements:
                stack_diagnostics = stack_displacement_diagnostics(
                    image, args.candidate, candidate_layout["case_rows"]
                )
    except (
        OSError,
        UnicodeError,
        ValueError,
        struct.error,
        LinkedImageError,
    ) as exc:
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
        "candidate_case_layout": candidate_layout,
        "stack_displacement_diagnostics": stack_diagnostics,
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
        if candidate_layout is not None:
            if stack_diagnostics is not None:
                print(
                    "ESP displacement diagnostic (no exactness credit): "
                    f"{stack_diagnostics['difference_count']} differences / "
                    f"{stack_diagnostics['stack_operands_compared']} operands / "
                    f"{stack_diagnostics['aligned_case_count']} aligned cases"
                )
            print(
                "candidate tables: "
                f"{candidate_layout['jump_table_address']} / "
                f"{candidate_layout['selector_table_address']}"
            )
            print(
                "candidate pre-table span: "
                f"{candidate_layout['candidate_pre_table_size']} bytes / "
                f"target {candidate_layout['target_pre_table_size']} / "
                f"delta {candidate_layout['pre_table_size_delta']:+d}; "
                "physical order "
                + (
                    "matches"
                    if candidate_layout["physical_order_matches"]
                    else "differs"
                )
            )
            print(
                "opcode  target+  candidate+  delta  next-gap target/candidate"
            )
            for row in candidate_layout["case_rows"]:
                print(
                    f"{row['opcode']:>6}  {row['target_relative']:7d}  "
                    f"{row['candidate_relative']:10d}  "
                    f"{row['relative_delta']:5d}  "
                    f"{row['target_next_destination_gap']:4d}/"
                    f"{row['candidate_next_destination_gap']:<4d}  "
                    f"{row['name']}"
                )
        if problems:
            for problem in problems:
                print(f"problem: {problem}", file=sys.stderr)
        else:
            print("generic ECL VM table coverage OK")
    return 1 if args.check and problems else 0


if __name__ == "__main__":
    raise SystemExit(main())
