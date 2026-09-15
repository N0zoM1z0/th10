#!/usr/bin/env python3
"""Report and validate the reconstructed TH10 score-entry owners."""

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

UPDATE_START = 0x00432CB0
UPDATE_CODE_END = 0x00433215
UPDATE_ALIGNMENT = 0x00433216
UPDATE_TABLE = 0x00433218
UPDATE_END = 0x00433227
DRAW_START = 0x00433230
DRAW_END = 0x00433569
NEXT_START = 0x00433570

STATE_DESTINATIONS = (
    0x00432CCC,
    0x00432EDA,
    0x00432F00,
    0x004331AC,
)
EXPECTED_CALLS = {
    UPDATE_START: {
        0x00405410: 1,
        0x0040ACE0: 4,
        0x0040AD20: 2,
        0x00420A90: 1,
        0x00420B10: 1,
        0x00421FA0: 1,
        0x0042C5C0: 1,
        0x0042C620: 4,
        0x0042C670: 5,
        0x0042C6D0: 4,
        0x0043DC90: 4,
        0x00448D00: 1,
        0x004491C0: 1,
        0x0044BEA0: 5,
    },
    DRAW_START: {
        0x00401630: 5,
        0x00452BAA: 1,
    },
}
ALPHABET = (
    b"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
    b"+-=.,!?@:;[]()_/{}|~^#$%&*   \0"
)

MARKER_PATTERN = re.compile(
    r"^// TH10_FRONTEND_FUNCTION: (0x[0-9A-Fa-f]{8}) (.+)$",
    re.MULTILINE,
)
STATE_PATTERN = re.compile(
    r"^\s*(FRONT_END_SCORE_ENTRY_[A-Z_]+)\s*=\s*"
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


def source_report(
    source: Path, header: Path
) -> tuple[dict[str, object], list[str]]:
    text = source.read_text(encoding="utf-8")
    header_text = header.read_text(encoding="utf-8")
    problems: list[str] = []
    starts = {UPDATE_START, DRAW_START}
    markers = [
        (int(address, 0), name)
        for address, name in MARKER_PATTERN.findall(text)
        if int(address, 0) in starts
    ]
    expected_markers = [
        (UPDATE_START, "FrontEndControllerView::UpdateScoreEntry"),
        (DRAW_START, "FrontEndControllerView::DrawScoreEntry"),
    ]
    if markers != expected_markers:
        problems.append("source markers do not match reviewed owner order")

    states = {
        name: int(value, 0)
        for name, value in STATE_PATTERN.findall(header_text)
    }
    if set(states.values()) != set(range(4)):
        problems.append("score-entry state enum does not cover exactly 0..3")

    required_source = (
        "case FRONT_END_SCREEN_SCORE_ENTRY:",
        "UpdateScoreEntry(this)",
        "DrawScoreEntry(this)",
        'FrontEndLoadMusic(0, "bgm/th10_17.wav")',
        "FrontEndPlayMusic(0, 17)",
        "CreateVm(controller, 0x68)",
        "CreateVm(controller, 0x98 + g_ReplayCharacter)",
        "g_FrontEndSelectedStage = FRONT_END_SCORE_ENTRY_NAME_LENGTH",
        "g_FrontEndSelectedStageMirror = FRONT_END_SCORE_ENTRY_NAME_LENGTH",
        "g_FrontEndScoreFormatTable = g_FrontEndScoreEntryFormatTable",
        "FrontEndInsertScore(GetFrontEndScoreTable(shot))",
        "controller->cursor.SetCurrent(insertedRow)",
        "keyboard->count = strlen(g_FrontEndScoreEntryAlphabet)",
        'strcmp(controller->scoreEntryName, "        ")',
        "keyboard->Move(-FRONT_END_SCORE_ENTRY_COLUMNS)",
        "keyboard->Move(FRONT_END_SCORE_ENTRY_COLUMNS)",
        "key == alphabetLength - 3",
        "key == alphabetLength - 2",
        "key == alphabetLength - 1",
        "g_FrontEndProfileData + 0x1d878",
        "SetScreen(controller, FRONT_END_SCREEN_RESULT)",
        "GetPracticeScoreRecord(shot, g_ReplayDifficulty, row)",
        'ascii->AddFormatText(&position, "%s", controller->scoreEntryName)',
        'ascii->AddFormatText(&position, "_")',
        "position.x = 212.0f",
        "position.y = 360.0f",
        "glyph = 0x81",
        "glyph = 0x7f",
        "glyph = 0x80",
    )
    for token in required_source:
        if token not in text:
            problems.append(f"source is missing reviewed behavior token: {token}")

    required_header = (
        "FRONT_END_SCREEN_SCORE_ENTRY = 15",
        "static int __stdcall UpdateScoreEntry",
        "static int __stdcall DrawScoreEntry",
        "char scoreEntryName[9]",
        "int scoreEntryNameCursor",
        "int scoreEntryUnavailable",
        "FrontEndCursorView scoreEntryKeyboardCursor",
        "offsetof(FrontEndControllerView, scoreEntryName) == 0x58dc",
        "offsetof(FrontEndControllerView, scoreEntryNameCursor) == 0x58e8",
        "offsetof(FrontEndControllerView, scoreEntryUnavailable) == 0x58ec",
        "offsetof(FrontEndControllerView, scoreEntryKeyboardCursor) == 0x58f4",
    )
    for token in required_header:
        if token not in header_text:
            problems.append(f"front-end header is missing declaration: {token}")

    if "FrontEndUpdateSpecial" in text or "FrontEndDrawSpecial" in text:
        problems.append("dispatcher still uses a source-absent score-entry stub")

    return {
        "markers": [f"0x{address:08X} {name}" for address, name in markers],
        "states": states,
        "controller_offsets": {
            "name": "0x58DC",
            "name_cursor": "0x58E8",
            "unavailable": "0x58EC",
            "keyboard_cursor": "0x58F4",
            "keyboard_wraps": "0x59C4",
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
            UPDATE_START: direct_call_counts(
                image, UPDATE_START, UPDATE_CODE_END
            ),
            DRAW_START: direct_call_counts(image, DRAW_START, DRAW_END),
        }
        table = struct.unpack("<4I", pe_bytes_at(image, UPDATE_TABLE, 16))
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
        problems.append("score-entry update code does not end in RET 4")
    if pe_bytes_at(image, UPDATE_ALIGNMENT, 2) != b"\x8b\xff":
        problems.append("score-entry update alignment differs from MOV EDI,EDI")
    if table != STATE_DESTINATIONS:
        problems.append("score-entry state table differs from review")
    if pe_bytes_at(image, DRAW_END - 2, 3) != b"\xc2\x04\x00":
        problems.append("score-entry draw does not end in RET 4")
    if pe_bytes_at(image, DRAW_END + 1, NEXT_START - DRAW_END - 1) != b"\xcc" * 6:
        problems.append("padding after score-entry draw differs")
    for start, expected in EXPECTED_CALLS.items():
        if calls[start] != Counter(expected):
            problems.append(f"direct-call multiset differs at 0x{start:08X}")
    if pe_bytes_at(image, 0x0042CFFF, 6) != b"\x53\xe8\xab\x5c\x00\x00":
        problems.append("front-end update dispatcher call site differs")
    if pe_bytes_at(image, 0x0042D28E, 6) != b"\x57\xe8\x9c\x5f\x00\x00":
        problems.append("front-end draw dispatcher call site differs")
    if pe_bytes_at(image, 0x0046E2F8, len(ALPHABET)) != ALPHABET:
        problems.append("score-entry alphabet differs from review")
    if pe_bytes_at(image, 0x0046E354, 9) != b"        \0":
        problems.append("blank score-entry name differs from review")
    if struct.unpack("<I", pe_bytes_at(image, 0x004746D8, 4))[0] != 0x0046E2F8:
        problems.append("score-entry alphabet pointer differs from review")
    if pe_bytes_at(image, 0x00432DAF, 10) != (
        b"\xc7\x05\x48\x78\x47\x00\x08\x49\x47\x00"
    ):
        problems.append("score-entry format table selection differs")
    if pe_bytes_at(image, 0x00432DC2, 10) != (
        b"\xc7\x05\x48\x78\x47\x00\x88\x47\x47\x00"
    ):
        problems.append("default score format table restoration differs")
    if extents.get(UPDATE_START) != UPDATE_END:
        problems.append("score-entry update boundary ledger omits its table")
    if extents.get(DRAW_START) != DRAW_END:
        problems.append("score-entry draw boundary ledger differs from review")

    report = {
        "ok": not problems,
        "target": str(target),
        "target_sha256": observed["sha256"],
        "decoder": {"name": "capstone", **decoder_identity},
        "owners": [
            {
                "start": f"0x{UPDATE_START:08X}",
                "code_end": f"0x{UPDATE_CODE_END:08X}",
                "end": f"0x{UPDATE_END:08X}",
                "size": UPDATE_END - UPDATE_START + 1,
                "name": "FrontEndControllerView::UpdateScoreEntry",
                "state_table": [f"0x{value:08X}" for value in table],
            },
            {
                "start": f"0x{DRAW_START:08X}",
                "end": f"0x{DRAW_END:08X}",
                "size": DRAW_END - DRAW_START + 1,
                "name": "FrontEndControllerView::DrawScoreEntry",
            },
        ],
        "reviewed_call_counts": {
            f"0x{start:08X}": {
                f"0x{destination:08X}": count
                for destination, count in sorted(flow.items())
            }
            for start, flow in calls.items()
        },
        "keyboard": {
            "alphabet_length": len(ALPHABET) - 1,
            "columns": 13,
            "name_length": 8,
            "special_entries": ["space", "backspace", "accept"],
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
        print("keyboard: 91 entries; 13 columns; final 3 are controls")
        print(f"status: {'PASS' if report['ok'] else 'FAIL'}")
        for problem in problems:
            print(f"problem: {problem}")

    return 0 if (not args.check or report["ok"]) else 1


if __name__ == "__main__":
    raise SystemExit(main())
