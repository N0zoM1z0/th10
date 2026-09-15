#!/usr/bin/env python3
"""Report and validate the reconstructed TH10 stage-selection owners."""

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

UPDATE_START = 0x00430FF0
UPDATE_CODE_END = 0x004313F6
UPDATE_TABLE = 0x004313F8
UPDATE_END = 0x0043140B
DISPLAY_START = 0x00431410
DISPLAY_END = 0x004315B2
NEXT_START = 0x004315C0

STATE_DESTINATIONS = (
    0x0043100B,
    0x00431060,
    0x00431085,
    0x00431320,
    0x004313B4,
)

EXPECTED_CALLS = {
    UPDATE_START: {
        0x0040C540: 1,
        0x00420C30: 1,
        0x0042C5C0: 2,
        0x0042C620: 4,
        0x0042C670: 2,
        0x0042C6D0: 2,
        0x0043C8B0: 1,
        0x0043DC90: 3,
        0x0044B010: 1,
        0x0044BE20: 1,
        0x0044BE70: 1,
        0x0044BEA0: 2,
    },
    DISPLAY_START: {0x00401630: 2},
}

STAGE_NAME_POINTERS = (
    0x0046E284,
    0x0046E27C,
    0x0046E274,
    0x0046E26C,
    0x0046E264,
    0x0046E25C,
    0x0046E254,
    0x0046E2A0,
)
STAGE_NAMES = (
    b"test   \0",
    b"Stage 1\0",
    b"Stage 2\0",
    b"Stage 3\0",
    b"Stage 4\0",
    b"Stage 5\0",
    b"Stage 6\0",
    b"Extra  \0",
)

MARKER_PATTERN = re.compile(
    r"^// TH10_FRONTEND_FUNCTION: (0x[0-9A-Fa-f]{8}) (.+)$",
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
    starts = {UPDATE_START, DISPLAY_START}
    markers = [
        (int(address, 0), name)
        for address, name in MARKER_PATTERN.findall(text)
        if int(address, 0) in starts
    ]
    expected_markers = [
        (UPDATE_START, "FrontEndControllerView::UpdateStage"),
        (DISPLAY_START, "FrontEndControllerView::DrawStageScores"),
    ]
    if markers != expected_markers:
        problems.append("source markers do not match reviewed owner order")

    required_source = (
        "UpdateStage(this)",
        "controller->cursor.SetCurrent(g_FrontEndStageCursor)",
        "GetFrontEndStageScoreRecord(controller->cursor.current + 1)",
        "directInput != 0 ? 2 : '1'",
        "g_FrontEndStageRecords[controller->cursor.current]",
        "g_FrontEndNextGameMode = 7",
        "screenState >= FRONT_END_SELECTION_ACTIVE",
        "stateTimer.current % 4 >= 2",
        '"%s  ---------"',
        '"%s  %.8d0"',
        "position.y += 18.0f",
        "FrontEndStageScoreRecordSizeIs8",
    )
    for token in required_source:
        if token not in text:
            problems.append(f"source is missing reviewed behavior token: {token}")

    required_header = (
        "static int __stdcall UpdateStage(FrontEndControllerView *controller)",
        "int DrawStageScores()",
    )
    for token in required_header:
        if token not in header_text:
            problems.append(f"front-end header is missing declaration: {token}")

    if "extern int FrontEndUpdateStage" in text:
        problems.append("parent dispatcher still uses the source-absent stage stub")

    return {
        "markers": [f"0x{address:08X} {name}" for address, name in markers],
        "stage_score_record_size": 8,
        "stage_score_offset": 0,
        "availability_offset": 5,
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
            UPDATE_START: direct_call_counts(image, UPDATE_START, UPDATE_CODE_END),
            DISPLAY_START: direct_call_counts(image, DISPLAY_START, DISPLAY_END),
        }
        table = struct.unpack("<5I", pe_bytes_at(image, UPDATE_TABLE, 20))
        stage_name_pointers = struct.unpack(
            "<8I", pe_bytes_at(image, 0x0047471C, 32)
        )
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
        problems.append("stage update code does not end in RET 4")
    if pe_bytes_at(image, UPDATE_CODE_END + 1, 1) != b"\x90":
        problems.append("stage update alignment byte is not the reviewed NOP")
    if table != STATE_DESTINATIONS:
        problems.append("stage update state table differs from review")
    if pe_bytes_at(image, UPDATE_END + 1, DISPLAY_START - UPDATE_END - 1) != b"\xcc" * 4:
        problems.append("padding between stage update and display differs")
    if pe_bytes_at(image, DISPLAY_END + 1, NEXT_START - DISPLAY_END - 1) != b"\xcc" * 13:
        problems.append("padding after stage display differs")
    if calls[UPDATE_START] != Counter(EXPECTED_CALLS[UPDATE_START]):
        problems.append("stage update direct-call multiset differs")
    if calls[DISPLAY_START] != Counter(EXPECTED_CALLS[DISPLAY_START]):
        problems.append("stage display direct-call multiset differs")
    if pe_bytes_at(image, 0x0042D2A8, 5) != b"\xe8\x63\x41\x00\x00":
        problems.append("front-end draw callback no longer calls stage display")
    if pe_bytes_at(image, 0x00470C0C, 4) != struct.pack("<f", 18.0):
        problems.append("stage row spacing constant differs from 18.0")
    if stage_name_pointers != STAGE_NAME_POINTERS:
        problems.append("stage-name pointer table differs from review")
    for address, expected in zip(STAGE_NAME_POINTERS, STAGE_NAMES, strict=True):
        if pe_bytes_at(image, address, len(expected)) != expected:
            problems.append(f"stage name at 0x{address:08X} differs")
    if extents.get(UPDATE_START) != UPDATE_END:
        problems.append("stage update boundary ledger omits its state table")
    if extents.get(DISPLAY_START) != DISPLAY_END:
        problems.append("stage display boundary ledger differs from review")

    report = {
        "ok": not problems,
        "target": str(target),
        "target_sha256": observed["sha256"],
        "decoder": {"name": "capstone", **decoder_identity},
        "owners": [
            {
                "start": f"0x{UPDATE_START:08X}",
                "end": f"0x{UPDATE_END:08X}",
                "size": UPDATE_END - UPDATE_START + 1,
                "name": "FrontEndControllerView::UpdateStage",
                "state_table": [f"0x{value:08X}" for value in table],
            },
            {
                "start": f"0x{DISPLAY_START:08X}",
                "end": f"0x{DISPLAY_END:08X}",
                "size": DISPLAY_END - DISPLAY_START + 1,
                "name": "FrontEndControllerView::DrawStageScores",
            },
        ],
        "reviewed_call_counts": {
            f"0x{start:08X}": {
                f"0x{destination:08X}": count
                for destination, count in sorted(flow.items())
            }
            for start, flow in calls.items()
        },
        "stage_names": [item[:-1].decode("ascii") for item in STAGE_NAMES],
        "row_spacing": 18.0,
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
        if problems:
            for problem in problems:
                print(f"problem: {problem}", file=sys.stderr)
        else:
            print("front-end stage target and source coverage OK")
    return 1 if args.check and problems else 0


if __name__ == "__main__":
    raise SystemExit(main())
