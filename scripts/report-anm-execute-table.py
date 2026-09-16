#!/usr/bin/env python3
"""Report and validate the TH10 ANM ExecuteScript dispatch table."""

from __future__ import annotations

import argparse
import json
from pathlib import Path
import re
import struct
import sys

from linked_image import LinkedImageError, PEImage, verify_capstone
from target_identity import pe_bytes_at, resolve_target, verify_target


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_SOURCE = ROOT / "src" / "AnmManager.cpp"
DEFAULT_HEADER = ROOT / "src" / "AnmManager.hpp"
FUNCTION_ADDRESS = 0x0043EE30
CODE_END_ADDRESS = 0x004413A2
ALIGNMENT_ADDRESS = 0x004413A3
JUMP_TABLE_ADDRESS = 0x004413A4
JUMP_TABLE_ENTRIES = 94
TABLE_END_ADDRESS = JUMP_TABLE_ADDRESS + JUMP_TABLE_ENTRIES * 4 - 1
NEXT_FUNCTION_ADDRESS = 0x00441520
FIRST_OPCODE = -1
LAST_OPCODE = 92

ENUM_PATTERN = re.compile(
    r"^\s*(ANM_OP_[A-Z0-9_]+)\s*=\s*(-?(?:0x[0-9A-Fa-f]+|[0-9]+)),?\s*$",
    re.MULTILINE,
)
CASE_PATTERN = re.compile(r"^\s*case\s+(ANM_OP_[A-Z0-9_]+):", re.MULTILINE)


def parse_source(
    source_path: Path, header_path: Path
) -> tuple[dict[str, int], list[str], list[str]]:
    header = header_path.read_text(encoding="utf-8")
    source = source_path.read_text(encoding="utf-8")
    function_start = source.index(
        "int __stdcall AnmRenderManagerView::ExecuteScript(AnmVmView *vm)"
    )
    function_end = source.index(
        "\nvoid AnmFloat3View::FromAngleMagnitude", function_start
    )
    function = source[function_start:function_end]
    enum_pairs = ENUM_PATTERN.findall(header)
    enum_values = {name: int(value, 0) for name, value in enum_pairs}
    case_names = CASE_PATTERN.findall(function)
    unique_case_names = list(dict.fromkeys(case_names))
    problems: list[str] = []
    if len(enum_pairs) != len(enum_values):
        problems.append("source has duplicate ANM opcode enum names")
    if len(enum_values.values()) != len(set(enum_values.values())):
        problems.append("source has duplicate ANM opcode enum values")
    expected_values = list(range(FIRST_OPCODE, LAST_OPCODE + 1))
    if sorted(enum_values.values()) != expected_values:
        problems.append("ANM opcode enum does not cover every value from -1 through 92")
    unknown_cases = sorted(set(unique_case_names) - set(enum_values))
    if unknown_cases:
        problems.append("cases without enum entries: " + ", ".join(unknown_cases))
    return enum_values, unique_case_names, problems


def physical_groups(table: tuple[int, ...]) -> list[dict[str, object]]:
    grouped: dict[int, list[int]] = {}
    for opcode, destination in zip(
        range(FIRST_OPCODE, LAST_OPCODE + 1), table
    ):
        grouped.setdefault(destination, []).append(opcode)
    return [
        {
            "destination": f"0x{destination:08X}",
            "opcodes": opcodes,
        }
        for destination, opcodes in sorted(grouped.items())
    ]


def candidate_table_address(
    image: PEImage, function_address: int, contribution_size: int
) -> int:
    from capstone import CS_ARCH_X86, CS_MODE_32, Cs
    from capstone.x86_const import X86_OP_MEM

    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    code = image.read_address(function_address, contribution_size)
    hits: list[int] = []
    for instruction in decoder.disasm(code, function_address):
        if instruction.mnemonic != "jmp" or not instruction.operands:
            continue
        operand = instruction.operands[0]
        if operand.type != X86_OP_MEM or operand.mem.scale != 4:
            continue
        displacement = int(operand.mem.disp) & 0xFFFFFFFF
        try:
            table = struct.unpack(
                f"<{JUMP_TABLE_ENTRIES}I",
                image.read_address(displacement, JUMP_TABLE_ENTRIES * 4),
            )
        except (LinkedImageError, struct.error):
            continue
        if all(function_address <= value < displacement for value in table):
            hits.append(displacement)
    if len(set(hits)) != 1:
        raise ValueError(
            "candidate must contain exactly one 94-entry indexed jump table, "
            f"found {len(set(hits))}"
        )
    return hits[0]


def candidate_case_layout(
    path: Path,
    function_address: int,
    contribution_size: int,
    target_table: tuple[int, ...],
    names_by_opcode: dict[int, str],
) -> dict[str, object]:
    image = PEImage(path)
    table_address = candidate_table_address(
        image, function_address, contribution_size
    )
    table = struct.unpack(
        f"<{JUMP_TABLE_ENTRIES}I",
        image.read_address(table_address, JUMP_TABLE_ENTRIES * 4),
    )

    target_groups = physical_groups(target_table)
    candidate_groups = physical_groups(table)
    rows = []
    for opcode, target_destination, candidate_destination in zip(
        range(FIRST_OPCODE, LAST_OPCODE + 1), target_table, table
    ):
        rows.append(
            {
                "opcode": opcode,
                "name": names_by_opcode.get(opcode, "unknown"),
                "target_destination": f"0x{target_destination:08X}",
                "candidate_destination": f"0x{candidate_destination:08X}",
                "target_relative": target_destination - FUNCTION_ADDRESS,
                "candidate_relative": candidate_destination - function_address,
                "relative_delta": (
                    candidate_destination
                    - function_address
                    - (target_destination - FUNCTION_ADDRESS)
                ),
            }
        )
    target_opcode_groups = [group["opcodes"] for group in target_groups]
    candidate_opcode_groups = [group["opcodes"] for group in candidate_groups]
    return {
        "image": str(path),
        "image_sha256": image.sha256,
        "function_address": f"0x{function_address:08X}",
        "contribution_size": contribution_size,
        "jump_table_address": f"0x{table_address:08X}",
        "jump_table_entries": len(table),
        "jump_table_unique_destinations": len(set(table)),
        "candidate_pre_table_span": table_address - function_address,
        "target_pre_table_span": JUMP_TABLE_ADDRESS - FUNCTION_ADDRESS,
        "pre_table_span_delta": (
            table_address
            - function_address
            - (JUMP_TABLE_ADDRESS - FUNCTION_ADDRESS)
        ),
        "physical_order_matches": candidate_opcode_groups == target_opcode_groups,
        "target_physical_groups": target_groups,
        "candidate_physical_groups": candidate_groups,
        "case_rows": rows,
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("executable", nargs="?", type=Path)
    parser.add_argument("--source", type=Path, default=DEFAULT_SOURCE)
    parser.add_argument("--header", type=Path, default=DEFAULT_HEADER)
    parser.add_argument("--candidate", type=Path)
    parser.add_argument(
        "--candidate-function-address", type=lambda value: int(value, 0)
    )
    parser.add_argument(
        "--candidate-contribution-size", type=lambda value: int(value, 0)
    )
    parser.add_argument("--json", action="store_true")
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    candidate_args = (
        args.candidate,
        args.candidate_function_address,
        args.candidate_contribution_size,
    )
    if any(value is not None for value in candidate_args) and not all(
        value is not None for value in candidate_args
    ):
        parser.error(
            "--candidate, --candidate-function-address and "
            "--candidate-contribution-size must be supplied together"
        )

    target = resolve_target(args.executable)
    if not target.is_file():
        print(f"missing target: {target}", file=sys.stderr)
        return 1
    try:
        observed, problems = verify_target(target)
        if problems:
            raise ValueError("; ".join(problems))
        target_bytes = target.read_bytes()
        table = struct.unpack(
            f"<{JUMP_TABLE_ENTRIES}I",
            pe_bytes_at(target_bytes, JUMP_TABLE_ADDRESS, JUMP_TABLE_ENTRIES * 4),
        )
        alignment = pe_bytes_at(target_bytes, ALIGNMENT_ADDRESS, 1)
        following_padding = pe_bytes_at(target_bytes, TABLE_END_ADDRESS + 1, 4)
        enum_values, case_names, source_problems = parse_source(
            args.source, args.header
        )
        names_by_opcode = {value: name for name, value in enum_values.items()}
        candidate_layout = None
        if args.candidate is not None:
            candidate_layout = candidate_case_layout(
                args.candidate,
                args.candidate_function_address,
                args.candidate_contribution_size,
                table,
                names_by_opcode,
            )
        with (ROOT / "config" / "tools.lock.toml").open("rb") as stream:
            import tomllib

            decoder_identity = verify_capstone(tomllib.load(stream)["capstone"])
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
    invalid_destinations = [
        destination
        for destination in table
        if not FUNCTION_ADDRESS <= destination <= CODE_END_ADDRESS
    ]
    if invalid_destinations:
        problems.append(
            "target table destinations leave ExecuteScript: "
            + ", ".join(f"0x{value:08X}" for value in invalid_destinations[:4])
        )
    if alignment != b"\x90":
        problems.append(f"unexpected function/table alignment byte: {alignment.hex()}")
    if following_padding != b"\xCC\xCC\xCC\xCC":
        problems.append("expected four CC bytes after the target jump table")

    source_case_values = sorted(
        enum_values[name] for name in case_names if name in enum_values
    )
    implicit_values = sorted(
        set(range(FIRST_OPCODE, LAST_OPCODE + 1)) - set(source_case_values)
    )
    allowed_implicit = {
        enum_values.get("ANM_OP_NOP"),
        enum_values.get("ANM_OP_INTERRUPT_LABEL"),
    }
    if set(implicit_values) - allowed_implicit:
        problems.append(
            "target opcodes missing from source cases: "
            + ", ".join(str(value) for value in implicit_values)
        )

    report = {
        "ok": not problems,
        "target": str(target),
        "target_sha256": observed["sha256"],
        "decoder": {"name": "capstone", **decoder_identity},
        "source": str(args.source),
        "header": str(args.header),
        "physical_owner": {
            "start": f"0x{FUNCTION_ADDRESS:08X}",
            "code_end": f"0x{CODE_END_ADDRESS:08X}",
            "code_size": CODE_END_ADDRESS - FUNCTION_ADDRESS + 1,
            "jump_table_start": f"0x{JUMP_TABLE_ADDRESS:08X}",
            "jump_table_end": f"0x{TABLE_END_ADDRESS:08X}",
            "next_function": f"0x{NEXT_FUNCTION_ADDRESS:08X}",
        },
        "jump_table": {
            "entries": len(table),
            "unique_destinations": len(set(table)),
            "physical_groups": physical_groups(table),
        },
        "source_enum_entries": len(enum_values),
        "source_unique_case_labels": len(case_names),
        "source_implicit_opcode_values": implicit_values,
        "candidate_case_layout": candidate_layout,
        "problems": problems,
    }
    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print(f"target OK: {target}")
        print(
            f"ExecuteScript code: 0x{FUNCTION_ADDRESS:08X}-"
            f"0x{CODE_END_ADDRESS:08X} "
            f"({CODE_END_ADDRESS - FUNCTION_ADDRESS + 1} bytes)"
        )
        print(
            f"jump table: {len(table)} entries / {len(set(table))} "
            "unique destinations"
        )
        print(
            f"source: {len(enum_values)} enum entries / "
            f"{len(case_names)} explicit opcode cases"
        )
        if implicit_values:
            print("implicit/default opcodes: " + ", ".join(map(str, implicit_values)))
        if candidate_layout is not None:
            print(
                "candidate table: "
                f"{candidate_layout['jump_table_address']}; pre-table span "
                f"{candidate_layout['candidate_pre_table_span']} bytes / "
                f"target {candidate_layout['target_pre_table_span']} / delta "
                f"{candidate_layout['pre_table_span_delta']:+d}; physical order "
                + (
                    "matches"
                    if candidate_layout["physical_order_matches"]
                    else "differs"
                )
            )
            print("opcode  target+  candidate+  delta  name")
            for row in candidate_layout["case_rows"]:
                print(
                    f"{row['opcode']:>6}  {row['target_relative']:7d}  "
                    f"{row['candidate_relative']:10d}  "
                    f"{row['relative_delta']:5d}  {row['name']}"
                )
        if problems:
            for problem in problems:
                print(f"problem: {problem}", file=sys.stderr)
        else:
            print("ANM ExecuteScript table coverage OK")
    return 1 if args.check and problems else 0


if __name__ == "__main__":
    raise SystemExit(main())
