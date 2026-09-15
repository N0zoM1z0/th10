#!/usr/bin/env python3
"""Report and validate the reconstructed TH10 practice-record owners."""

from __future__ import annotations

import argparse
from collections import Counter
import csv
import json
from pathlib import Path
import re
import struct
import sys
import tomllib

from linked_image import LinkedImageError, verify_capstone
from target_identity import pe_bytes_at, resolve_target, verify_target


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_SOURCE = ROOT / "src" / "FrontEnd.cpp"
DEFAULT_HEADER = ROOT / "src" / "FrontEnd.hpp"

UPDATE_START = 0x00431EE0
UPDATE_CODE_END = 0x0043267C
UPDATE_ALIGNMENT = 0x0043267D
UPDATE_TABLE = 0x00432680
UPDATE_END = 0x0043268F
REFRESH_START = 0x00432690
REFRESH_END = 0x004329E1
DRAW_PRACTICE_START = 0x004329F0
DRAW_PRACTICE_END = 0x00432CA0
NEXT_START = 0x00432CB0

DRAW_DISPATCH_START = 0x0042D260
DRAW_DISPATCH_CODE_END = 0x0042D2B3
DRAW_DISPATCH_TABLE = 0x0042D2B4
DRAW_DISPATCH_END = 0x0042D2D3

STATE_DESTINATIONS = (
    0x00431F06,
    0x00432095,
    0x004320BA,
    0x00432615,
)
DRAW_DESTINATIONS = (
    0x0042D2A8,
    0x0042D2AD,
    0x0042D282,
    0x0042D275,
    0x0042D2AD,
    0x0042D2AD,
    0x0042D28E,
    0x0042D29B,
)

EXPECTED_CALLS = {
    DRAW_DISPATCH_START: {
        0x00431410: 1,
        0x00431BA0: 1,
        0x004329F0: 1,
        0x00433230: 1,
        0x00433B30: 1,
    },
    UPDATE_START: {
        0x004088C0: 2,
        0x00409E50: 2,
        0x0042C5C0: 1,
        0x0042C620: 3,
        0x0042C670: 16,
        0x0042C6D0: 16,
        0x0042C770: 2,
        0x0042C8C0: 1,
        0x00432690: 3,
        0x0043DC90: 5,
        0x00448D00: 2,
        0x004491C0: 1,
        0x00449250: 4,
        0x0044B010: 1,
        0x0044BE70: 1,
        0x0044BEA0: 5,
    },
    REFRESH_START: {
        0x00447BB0: 3,
        0x00458EA5: 1,
    },
    DRAW_PRACTICE_START: {
        0x00401630: 5,
        0x00452BAA: 1,
    },
}

SECRET_KEYS = (
    0x17, 0x20, 0x18, 0x31, 0x14, 0x20, 0x13, 0x17, 0x31, 0x25, 0x30,
    0x12, 0x12, 0x13, 0x14, 0x18, 0x12, 0x31, 0x20, 0x17, 0x31, 0x22,
)
PRACTICE_DIFFICULTIES = bytes(
    [2, 3, 0, 1] * 12 + [2, 3] +
    [1, 2, 3, 0] * 11 + [1, 2, 3] +
    [4] * 13
)

MARKER_PATTERN = re.compile(
    r"^// TH10_FRONTEND_FUNCTION: (0x[0-9A-Fa-f]{8}) (.+)$",
    re.MULTILINE,
)
STATE_PATTERN = re.compile(
    r"^\s*(FRONT_END_PRACTICE_[A-Z_]+)\s*=\s*"
    r"(0x[0-9A-Fa-f]+|[0-9]+)\s*[,}]?",
    re.MULTILINE,
)


def direct_call_counts(image: bytes, start: int, end: int) -> Counter[int]:
    from capstone import CS_ARCH_X86, CS_MODE_32, Cs
    from capstone.x86_const import X86_OP_IMM

    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    body = pe_bytes_at(image, start, end - start + 1)
    calls: Counter[int] = Counter()
    consumed = 0
    for instruction in decoder.disasm(body, start):
        if instruction.address != start + consumed:
            break
        consumed += instruction.size
        if (instruction.mnemonic == "call" and instruction.operands and
                instruction.operands[0].type == X86_OP_IMM):
            calls[int(instruction.operands[0].imm) & 0xFFFFFFFF] += 1
    if consumed != len(body):
        raise ValueError(
            f"code at 0x{start:08X} decoded {consumed}/{len(body)} bytes"
        )
    return calls


def tracking_extents() -> dict[int, int]:
    with (ROOT / "config" / "function-boundaries.csv").open(
        newline="", encoding="utf-8"
    ) as stream:
        return {
            int(row["address"], 0): int(row["span_end"], 0)
            for row in csv.DictReader(stream)
        }


def source_report(source: Path, header: Path) -> tuple[dict[str, object], list[str]]:
    text = source.read_text(encoding="utf-8")
    header_text = header.read_text(encoding="utf-8")
    problems: list[str] = []
    starts = {
        DRAW_DISPATCH_START, UPDATE_START, REFRESH_START,
        DRAW_PRACTICE_START,
    }
    markers = [
        (int(address, 0), name)
        for address, name in MARKER_PATTERN.findall(text)
        if int(address, 0) in starts
    ]
    expected_markers = [
        (DRAW_DISPATCH_START, "FrontEndControllerView::Draw"),
        (UPDATE_START, "FrontEndControllerView::UpdatePractice"),
        (REFRESH_START, "FrontEndControllerView::RefreshPracticeRecords"),
        (DRAW_PRACTICE_START, "FrontEndControllerView::DrawPractice"),
    ]
    if markers != expected_markers:
        problems.append("source markers do not match reviewed owner order")

    states = {
        name: int(value, 0)
        for name, value in STATE_PATTERN.findall(header_text)
    }
    if set(states.values()) != set(range(4)):
        problems.append("practice state enum does not cover exactly 0..3")

    required_source = (
        "case FRONT_END_SCREEN_PRACTICE:",
        "DrawPractice();",
        "case FRONT_END_SCREEN_REPLAY:",
        "DrawReplay(this);",
        "UpdatePractice(this)",
        "controller->practiceDifficultyCursor.count = 5",
        "controller->practicePageCursor.count =",
        "CountPracticeRecordsForDifficulty(",
        "CreateVm(controller, 0x66)",
        "row + 0x17, FRONT_END_RENDER_LAYER",
        "controller->RefreshPracticeRecords()",
        "g_FrontEndPracticeSecretKeys[",
        "g_FrontEndPracticeSecretIdleFrames > 300",
        "FrontEndUnlockPracticeRecords()",
        "InterruptPracticeRows(controller)",
        "g_FrontEndProfileData + 0x19a8c + record * 0x90",
        "g_FrontEndProfileData + shot * 0x437c + 0x624 + record * 0x90",
        '"No.%3d %s %4d/%4d"',
        "? 0x00ffff80u : 0x00efefefu",
        "g_FrontEndPracticeUnavailableFormat",
        "memset(name + length, ' ', 42 - length)",
        "GetPracticeScoreRecord(cursor.current, difficulty, row)",
        '"%2d  %s  %9ld%d  ----/--/-- --:--  Stage -  ---%%"',
        '"%3d:%.2d:%.2d"',
        "playTimeFrames / 216000",
    )
    for token in required_source:
        if token not in text:
            problems.append(f"source is missing reviewed behavior token: {token}")

    required_header = (
        "FRONT_END_SCREEN_PRACTICE = 11",
        "FRONT_END_SCREEN_REPLAY = 12",
        "int Draw()",
        "static int __stdcall UpdatePractice(FrontEndControllerView *controller)",
        "int RefreshPracticeRecords()",
        "int DrawPractice()",
        "FrontEndCursorView practiceDifficultyCursor",
        "FrontEndCursorView practicePageCursor",
        "int practiceDisplayedEntries",
        "AnmVmIdView practiceRowVmIds[10]",
        "offsetof(FrontEndControllerView, practiceDifficultyCursor) == 0x0fc",
        "offsetof(FrontEndControllerView, practicePageCursor) == 0x1d4",
        "offsetof(FrontEndControllerView, practiceDisplayedEntries) == 0x2ac",
        "offsetof(FrontEndControllerView, practiceRowVmIds) == 0x5d4",
    )
    for token in required_header:
        if token not in header_text:
            problems.append(f"front-end header is missing declaration: {token}")

    if "extern int FrontEndUpdatePractice" in text:
        problems.append("parent dispatcher still uses the source-absent practice stub")

    return {
        "markers": [f"0x{address:08X} {name}" for address, name in markers],
        "states": states,
        "controller_offsets": {
            "difficulty_cursor": "0x0FC",
            "page_cursor": "0x1D4",
            "displayed_entries": "0x2AC",
            "row_vm_ids": "0x5D4",
            "row_vm_ids_end": "0x5FC",
        },
    }, problems


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("executable", nargs="?", type=Path)
    parser.add_argument("--source", type=Path, default=DEFAULT_SOURCE)
    parser.add_argument("--header", type=Path, default=DEFAULT_HEADER)
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
        with (ROOT / "config" / "tools.lock.toml").open("rb") as stream:
            decoder_identity = verify_capstone(tomllib.load(stream)["capstone"])
        image = target.read_bytes()
        calls = {
            DRAW_DISPATCH_START: direct_call_counts(
                image, DRAW_DISPATCH_START, DRAW_DISPATCH_CODE_END
            ),
            UPDATE_START: direct_call_counts(
                image, UPDATE_START, UPDATE_CODE_END
            ),
            REFRESH_START: direct_call_counts(
                image, REFRESH_START, REFRESH_END
            ),
            DRAW_PRACTICE_START: direct_call_counts(
                image, DRAW_PRACTICE_START, DRAW_PRACTICE_END
            ),
        }
        table = struct.unpack("<4I", pe_bytes_at(image, UPDATE_TABLE, 16))
        draw_table = struct.unpack(
            "<8I", pe_bytes_at(image, DRAW_DISPATCH_TABLE, 32)
        )
        secret_keys = struct.unpack(
            "<22I", pe_bytes_at(image, 0x0046EF10, 88)
        )
        difficulties = pe_bytes_at(image, 0x004743C0, 110)
        source, source_problems = source_report(args.source, args.header)
        extents = tracking_extents()
    except (
        OSError, UnicodeError, ValueError, KeyError, struct.error,
        LinkedImageError, tomllib.TOMLDecodeError,
    ) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 2

    problems = list(source_problems)
    if pe_bytes_at(image, UPDATE_CODE_END - 2, 3) != b"\xc2\x04\x00":
        problems.append("practice update code does not end in RET 4")
    if pe_bytes_at(image, UPDATE_ALIGNMENT, 3) != b"\x8d\x49\x00":
        problems.append("practice update alignment differs from LEA ECX,[ECX]")
    if table != STATE_DESTINATIONS:
        problems.append("practice update state table differs from review")
    if pe_bytes_at(image, REFRESH_END, 1) != b"\xc3":
        problems.append("practice refresh does not end in RET")
    if pe_bytes_at(image, REFRESH_END + 1, DRAW_PRACTICE_START - REFRESH_END - 1) != b"\xcc" * 14:
        problems.append("padding before practice draw differs")
    if pe_bytes_at(image, DRAW_DISPATCH_CODE_END, 1) != b"\xc3":
        problems.append("front-end draw dispatcher does not end in RET")
    if draw_table != DRAW_DESTINATIONS:
        problems.append("front-end draw dispatcher table differs from review")
    if pe_bytes_at(image, DRAW_PRACTICE_END, 1) != b"\xc3":
        problems.append("practice draw does not end in RET")
    if pe_bytes_at(
        image, DRAW_PRACTICE_END + 1, NEXT_START - DRAW_PRACTICE_END - 1
    ) != b"\xcc" * 15:
        problems.append("padding after practice draw differs")
    for start, expected in EXPECTED_CALLS.items():
        if calls[start] != Counter(expected):
            problems.append(f"direct-call multiset differs at 0x{start:08X}")
    if pe_bytes_at(image, 0x0042D191, 6) != b"\x53\xe8\x49\x4d\x00\x00":
        problems.append("front-end update callback no longer calls practice update")
    if secret_keys != SECRET_KEYS:
        problems.append("practice secret-key table differs from review")
    if difficulties != PRACTICE_DIFFICULTIES:
        problems.append("practice difficulty map differs from review")
    expected_unavailable = (
        b"No.%3d " + b"\x81\x48" * 21 + b" %4d/%4d\0"
    )
    if pe_bytes_at(image, 0x0046EEBC, len(expected_unavailable)) != expected_unavailable:
        problems.append("practice unavailable-row format differs from review")
    if pe_bytes_at(image, 0x0046EEF8, 18) != b"No.%3d %s %4d/%4d\0":
        problems.append("practice active-row format differs from review")
    if extents.get(UPDATE_START) != UPDATE_END:
        problems.append("practice update boundary ledger omits its state table")
    if extents.get(REFRESH_START) != REFRESH_END:
        problems.append("practice refresh boundary ledger differs from review")
    if extents.get(DRAW_DISPATCH_START) != DRAW_DISPATCH_END:
        problems.append("front-end draw dispatcher boundary differs from review")
    if extents.get(DRAW_PRACTICE_START) != DRAW_PRACTICE_END:
        problems.append("practice draw boundary differs from review")

    report = {
        "ok": not problems,
        "target": str(target),
        "target_sha256": observed["sha256"],
        "decoder": {"name": "capstone", **decoder_identity},
        "owners": [
            {
                "start": f"0x{DRAW_DISPATCH_START:08X}",
                "code_end": f"0x{DRAW_DISPATCH_CODE_END:08X}",
                "end": f"0x{DRAW_DISPATCH_END:08X}",
                "size": DRAW_DISPATCH_END - DRAW_DISPATCH_START + 1,
                "name": "FrontEndControllerView::Draw",
                "screen_table": [
                    f"0x{value:08X}" for value in draw_table
                ],
            },
            {
                "start": f"0x{UPDATE_START:08X}",
                "code_end": f"0x{UPDATE_CODE_END:08X}",
                "end": f"0x{UPDATE_END:08X}",
                "size": UPDATE_END - UPDATE_START + 1,
                "name": "FrontEndControllerView::UpdatePractice",
                "state_table": [f"0x{value:08X}" for value in table],
            },
            {
                "start": f"0x{REFRESH_START:08X}",
                "end": f"0x{REFRESH_END:08X}",
                "size": REFRESH_END - REFRESH_START + 1,
                "name": "FrontEndControllerView::RefreshPracticeRecords",
            },
            {
                "start": f"0x{DRAW_PRACTICE_START:08X}",
                "end": f"0x{DRAW_PRACTICE_END:08X}",
                "size": DRAW_PRACTICE_END - DRAW_PRACTICE_START + 1,
                "name": "FrontEndControllerView::DrawPractice",
            },
        ],
        "reviewed_call_counts": {
            f"0x{start:08X}": {
                f"0x{destination:08X}": count
                for destination, count in sorted(flow.items())
            }
            for start, flow in calls.items()
        },
        "practice_records": {
            "record_count": 110,
            "rows_per_page": 10,
            "secret_key_count": 22,
            "secret_timeout_frames": 300,
        },
        "source": source,
        "problems": problems,
    }

    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print(f"target OK: {target}")
        for owner in report["owners"]:
            print(
                f"{owner['start']}-{owner['end']} {owner['size']:4d} bytes "
                f"{owner['name']}"
            )
        print("practice rows: 110 records; 10 rows per page")
        print("practice secret: 22 keys; 300-frame timeout")
        print(f"status: {'PASS' if report['ok'] else 'FAIL'}")
        for problem in problems:
            print(f"problem: {problem}")

    return 0 if (not args.check or report["ok"]) else 1


if __name__ == "__main__":
    raise SystemExit(main())
