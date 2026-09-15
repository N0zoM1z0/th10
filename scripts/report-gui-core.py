#!/usr/bin/env python3
"""Report and validate the TH10 stage-GUI core selector tables."""

from __future__ import annotations

import argparse
import json
from pathlib import Path
import re
import struct
import sys

from target_identity import pe_bytes_at, resolve_target, verify_target


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_SOURCE = ROOT / "src" / "Gui.cpp"

HUD_START = 0x00414900
HUD_CODE_END = 0x004157DC
HUD_ALIGNMENT = 0x004157DD
HUD_TABLE = 0x004157E0
HUD_TABLE_ENTRIES = 7
HUD_OWNER_END = 0x004157FB
HUD_NEXT_FUNCTION = 0x00415800

MESSAGE_START = 0x00415E90
MESSAGE_CODE_END = 0x00416F61
MESSAGE_ALIGNMENT = 0x00416F62
MESSAGE_TABLE = 0x00416F64
MESSAGE_OWNER_END = 0x0041700F
MESSAGE_NEXT_FUNCTION = 0x00417010

MESSAGE_TABLE_PARTS = (
    ("opcode", 24),
    ("enemy_portrait_stage", 7),
    ("enemy_name_stage", 7),
    ("stage_clear_difficulty", 5),
)

ENUM_PATTERN = re.compile(
    r"^\s*(GUI_MSG_[A-Z0-9_]+)\s*=\s*(0x[0-9A-Fa-f]+|[0-9]+)\s*[,}]?",
    re.MULTILINE,
)
CASE_PATTERN = re.compile(
    r"^\s*case\s+(GUI_MSG_[A-Z0-9_]+):", re.MULTILINE
)


def read_pointers(image: bytes, address: int, count: int) -> tuple[int, ...]:
    return struct.unpack(
        f"<{count}I", pe_bytes_at(image, address, count * 4)
    )


def table_report(
    values: tuple[int, ...], code_start: int, code_end: int
) -> dict[str, object]:
    return {
        "entries": len(values),
        "unique_destinations": len(set(values)),
        "destinations": [f"0x{value:08X}" for value in values],
        "all_destinations_in_code": all(
            code_start <= value <= code_end for value in values
        ),
    }


def source_coverage(path: Path) -> tuple[dict[str, object], list[str]]:
    text = path.read_text(encoding="utf-8")
    pairs = ENUM_PATTERN.findall(text)
    values = {name: int(value, 0) for name, value in pairs}
    cases = CASE_PATTERN.findall(text)
    problems: list[str] = []
    if len(pairs) != len(values):
        problems.append("source has duplicate GUI message enum names")
    if len(values) != len(set(values.values())):
        problems.append("source has duplicate GUI message enum values")
    if len(cases) != len(set(cases)):
        problems.append("source has duplicate GUI message case labels")
    if set(values.values()) != set(range(24)):
        problems.append("source GUI message enum does not cover exactly opcodes 0..23")
    missing_cases = sorted(set(values) - set(cases))
    extra_cases = sorted(set(cases) - set(values))
    if missing_cases:
        problems.append("enum entries without cases: " + ", ".join(missing_cases))
    if extra_cases:
        problems.append("cases without enum entries: " + ", ".join(extra_cases))
    return {
        "enum_entries": len(values),
        "case_labels": len(cases),
        "opcode_values": sorted(values.values()),
    }, problems


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
        observed, identity_problems = verify_target(target)
        if identity_problems:
            raise ValueError("; ".join(identity_problems))
        image = target.read_bytes()
        hud_alignment = pe_bytes_at(image, HUD_ALIGNMENT, 3)
        hud_values = read_pointers(image, HUD_TABLE, HUD_TABLE_ENTRIES)
        message_alignment = pe_bytes_at(image, MESSAGE_ALIGNMENT, 2)
        message_count = sum(count for _, count in MESSAGE_TABLE_PARTS)
        message_values = read_pointers(image, MESSAGE_TABLE, message_count)
        source, source_problems = source_coverage(args.source)
    except (OSError, UnicodeError, ValueError, struct.error) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 2

    problems = list(source_problems)
    if hud_alignment != b"\x8d\x49\x00":
        problems.append(f"unexpected HUD alignment: {hud_alignment.hex()}")
    if message_alignment != b"\x8b\xff":
        problems.append(
            f"unexpected message/table alignment: {message_alignment.hex()}"
        )
    hud_table = table_report(hud_values, HUD_START, HUD_CODE_END)
    if not hud_table["all_destinations_in_code"]:
        problems.append("HUD stage-selector destination outside reviewed code")

    parts: dict[str, dict[str, object]] = {}
    offset = 0
    for name, count in MESSAGE_TABLE_PARTS:
        values = message_values[offset : offset + count]
        part = table_report(values, MESSAGE_START, MESSAGE_CODE_END)
        parts[name] = part
        if not part["all_destinations_in_code"]:
            problems.append(
                f"message {name} selector destination outside reviewed code"
            )
        if part["unique_destinations"] != count:
            problems.append(f"message {name} selector has duplicate destinations")
        offset += count

    report = {
        "ok": not problems,
        "target": str(target),
        "target_sha256": observed["sha256"],
        "source": str(args.source),
        "hud_owner": {
            "start": f"0x{HUD_START:08X}",
            "code_end": f"0x{HUD_CODE_END:08X}",
            "owner_end": f"0x{HUD_OWNER_END:08X}",
            "size": HUD_OWNER_END - HUD_START + 1,
            "next_function": f"0x{HUD_NEXT_FUNCTION:08X}",
            "stage_selector": hud_table,
        },
        "message_owner": {
            "start": f"0x{MESSAGE_START:08X}",
            "code_end": f"0x{MESSAGE_CODE_END:08X}",
            "owner_end": f"0x{MESSAGE_OWNER_END:08X}",
            "size": MESSAGE_OWNER_END - MESSAGE_START + 1,
            "next_function": f"0x{MESSAGE_NEXT_FUNCTION:08X}",
            "selectors": parts,
        },
        "source_coverage": source,
        "problems": problems,
    }

    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print(f"target OK: {target}")
        print(
            f"HUD owner: 0x{HUD_START:08X}-0x{HUD_OWNER_END:08X} "
            f"({HUD_OWNER_END - HUD_START + 1} bytes), "
            f"{HUD_TABLE_ENTRIES}-entry stage selector"
        )
        print(
            f"message owner: 0x{MESSAGE_START:08X}-0x{MESSAGE_OWNER_END:08X} "
            f"({MESSAGE_OWNER_END - MESSAGE_START + 1} bytes), "
            f"{sum(count for _, count in MESSAGE_TABLE_PARTS)} selector entries"
        )
        print(
            f"source: {source['enum_entries']} enum entries / "
            f"{source['case_labels']} case labels"
        )
        if problems:
            for problem in problems:
                print(f"problem: {problem}", file=sys.stderr)
        else:
            print("GUI core table and source coverage OK")
    return 1 if args.check and problems else 0


if __name__ == "__main__":
    raise SystemExit(main())
