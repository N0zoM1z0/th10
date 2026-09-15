#!/usr/bin/env python3
"""Report and validate the reconstructed TH10 replay-selection owners."""

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

UPDATE_START = 0x004315C0
UPDATE_CODE_END = 0x00431B79
UPDATE_ALIGNMENT = 0x00431B7A
UPDATE_TABLE = 0x00431B7C
UPDATE_END = 0x00431B93
DISPLAY_START = 0x00431BA0
DISPLAY_END = 0x00431ED8
NEXT_START = 0x00431EE0

STATE_DESTINATIONS = (
    0x004315F2,
    0x0043174D,
    0x004317B1,
    0x004319B4,
    0x004318FD,
    0x00431A9E,
)

EXPECTED_CALLS = {
    UPDATE_START: {
        0x00409E50: 1,
        0x0040ACE0: 2,
        0x0040AD20: 1,
        0x0040C540: 1,
        0x00420C30: 1,
        0x004294A0: 1,
        0x004296F0: 2,
        0x0042C5C0: 2,
        0x0042C620: 4,
        0x0042C670: 2,
        0x0042C6D0: 2,
        0x0042C770: 2,
        0x0043C8B0: 1,
        0x0043DC90: 4,
        0x00448D00: 1,
        0x004491C0: 1,
        0x0044BE20: 1,
        0x0044BE70: 1,
        0x0044BEA0: 6,
        0x004524A1: 1,
        0x00452ABC: 1,
        0x00452AE8: 1,
        0x00453111: 4,
        0x00458EA5: 1,
    },
    DISPLAY_START: {0x00401630: 5, 0x00452BAA: 2},
}

CHARACTER_NAMES = (
    b"ReimuA \0", b"ReimuB \0", b"ReimuC \0",
    b"MarisaA\0", b"MarisaB\0", b"MarisaC\0",
)
DIFFICULTY_NAMES = (
    b"Easy   \0", b"Normal \0", b"Hard   \0", b"Lunatic\0", b"Extra  \0",
)
COMPLETION_NAMES = (
    b"tst\0", b"St1\0", b"St2\0", b"St3\0", b"St4\0",
    b"St5\0", b"St6\0", b"Ex \0", b"All\0", b"ExA\0",
)

MARKER_PATTERN = re.compile(
    r"^// TH10_FRONTEND_FUNCTION: (0x[0-9A-Fa-f]{8}) (.+)$",
    re.MULTILINE,
)
STATE_PATTERN = re.compile(
    r"^\s*(FRONT_END_REPLAY_[A-Z_]+)\s*=\s*"
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


def pointer_strings(
    image: bytes, table: int, count: int
) -> tuple[tuple[int, ...], tuple[bytes, ...]]:
    pointers = struct.unpack(
        f"<{count}I", pe_bytes_at(image, table, count * 4)
    )
    strings = []
    for pointer in pointers:
        raw = bytearray()
        address = pointer
        while True:
            value = pe_bytes_at(image, address, 1)
            raw += value
            address += 1
            if value == b"\0":
                break
        strings.append(bytes(raw))
    return pointers, tuple(strings)


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
        (UPDATE_START, "FrontEndControllerView::UpdateReplay"),
        (DISPLAY_START, "FrontEndControllerView::DrawReplay"),
    ]
    if markers != expected_markers:
        problems.append("source markers do not match reviewed owner order")

    states = {
        name: int(value, 0)
        for name, value in STATE_PATTERN.findall(header_text)
    }
    if set(states.values()) != set(range(6)):
        problems.append("replay state enum does not cover exactly 0..5")

    required_source = (
        "UpdateReplay(this)",
        "controller->cursor.SetCurrent(g_FrontEndReplayCursor)",
        'sprintf(replayPath, "th10_%.2d.rpy", replayNumber)',
        'FindFirstFileA("th10_ud????.rpy", &findData)',
        "FindClose(search)",
        "controller->replayFiles[replayIndex]",
        "replay->stageStates[stage].header",
        "controller->cursor.DisableEntry(stage - 1)",
        "controller->selectedReplayStage + 1",
        "g_FrontEndNextGameMode = 12",
        "g_FrontEndMode = 2",
        "ReplayManager::Destroy(controller->replayFiles[replayIndex])",
        "DrawReplaySummary(",
        '"No.%.2d -------- --/--/-- --:-- ------- ------- --- ---%%"',
        '"No.%.2d %s %.2d/%.2d/%.2d %.2d:%.2d %s %s %s %2.1f%%"',
        '"%s  %.8d%d"',
        "position.y += 15.0f",
        "position.y += 18.0f",
    )
    for token in required_source:
        if token not in text:
            problems.append(f"source is missing reviewed behavior token: {token}")

    required_header = (
        "static int __stdcall UpdateReplay(FrontEndControllerView *controller)",
        "static int __stdcall DrawReplay(FrontEndControllerView *controller)",
        "ReplayManager *replayFiles[50]",
        "offsetof(FrontEndControllerView, replayListOffset) == 0x59d8",
        "offsetof(FrontEndControllerView, selectedReplay) == 0x59dc",
        "offsetof(FrontEndControllerView, selectedReplayStage) == 0x59e0",
        "offsetof(FrontEndControllerView, replayFiles) == 0x59e4",
    )
    for token in required_header:
        if token not in header_text:
            problems.append(f"front-end header is missing declaration: {token}")

    if "extern int FrontEndUpdateReplay" in text:
        problems.append("parent dispatcher still uses the source-absent replay stub")

    return {
        "markers": [f"0x{address:08X} {name}" for address, name in markers],
        "states": states,
        "controller_offsets": {
            "replay_list_offset": "0x59D8",
            "selected_replay": "0x59DC",
            "selected_stage": "0x59E0",
            "replay_files": "0x59E4",
            "replay_files_end": "0x5AAC",
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
            UPDATE_START: direct_call_counts(image, UPDATE_START, UPDATE_CODE_END),
            DISPLAY_START: direct_call_counts(image, DISPLAY_START, DISPLAY_END),
        }
        table = struct.unpack("<6I", pe_bytes_at(image, UPDATE_TABLE, 24))
        _, character_names = pointer_strings(image, 0x004746DC, 6)
        _, difficulty_names = pointer_strings(image, 0x004746F4, 5)
        _, completion_names = pointer_strings(image, 0x00474744, 10)
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
        problems.append("replay update code does not end in RET 4")
    if pe_bytes_at(image, UPDATE_ALIGNMENT, 2) != b"\x8b\xff":
        problems.append("replay update alignment differs from MOV EDI,EDI")
    if table != STATE_DESTINATIONS:
        problems.append("replay update state table differs from review")
    if pe_bytes_at(image, UPDATE_END + 1, DISPLAY_START - UPDATE_END - 1) != b"\xcc" * 12:
        problems.append("padding between replay update and display differs")
    if pe_bytes_at(image, DISPLAY_END - 2, 3) != b"\xc2\x04\x00":
        problems.append("replay display does not end in RET 4")
    if pe_bytes_at(image, DISPLAY_END + 1, NEXT_START - DISPLAY_END - 1) != b"\xcc" * 7:
        problems.append("padding after replay display differs")
    for start, expected in EXPECTED_CALLS.items():
        if calls[start] != Counter(expected):
            problems.append(f"direct-call multiset differs at 0x{start:08X}")
    if pe_bytes_at(image, 0x0042D136, 6) != b"\x53\xe8\x84\x44\x00\x00":
        problems.append("front-end update callback no longer calls replay update")
    if pe_bytes_at(image, 0x0042D275, 6) != b"\x57\xe8\x25\x49\x00\x00":
        problems.append("front-end draw callback no longer calls replay display")
    if character_names != CHARACTER_NAMES:
        problems.append("replay character-name table differs from review")
    if difficulty_names != DIFFICULTY_NAMES:
        problems.append("replay difficulty-name table differs from review")
    if completion_names != COMPLETION_NAMES:
        problems.append("replay completion-name table differs from review")
    expected_floats = {
        0x00470C08: 15.0,
        0x00470C0C: 18.0,
        0x00470C18: 0.1,
        0x00470C1C: 10.0,
        0x00470C28: 80.0,
    }
    for address, value in expected_floats.items():
        if pe_bytes_at(image, address, 4) != struct.pack("<f", value):
            problems.append(f"replay display float at 0x{address:08X} differs")
    if extents.get(UPDATE_START) != UPDATE_END:
        problems.append("replay update boundary ledger omits its state table")
    if extents.get(DISPLAY_START) != DISPLAY_END:
        problems.append("replay display boundary ledger differs from review")

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
                "name": "FrontEndControllerView::UpdateReplay",
                "state_table": [f"0x{value:08X}" for value in table],
            },
            {
                "start": f"0x{DISPLAY_START:08X}",
                "end": f"0x{DISPLAY_END:08X}",
                "size": DISPLAY_END - DISPLAY_START + 1,
                "name": "FrontEndControllerView::DrawReplay",
            },
        ],
        "reviewed_call_counts": {
            f"0x{start:08X}": {
                f"0x{destination:08X}": count
                for destination, count in sorted(flow.items())
            }
            for start, flow in calls.items()
        },
        "display": {
            "fixed_rows": 25,
            "stage_rows": 7,
            "fixed_row_spacing": 15.0,
            "stage_row_spacing": 18.0,
            "character_names": [value[:-1].decode("ascii") for value in character_names],
            "difficulty_names": [value[:-1].decode("ascii") for value in difficulty_names],
            "completion_names": [value[:-1].decode("ascii") for value in completion_names],
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
        print("replay rows: 25 fixed slots; 7 stage choices")
        print(f"status: {'PASS' if report['ok'] else 'FAIL'}")
        for problem in problems:
            print(f"problem: {problem}")

    return 0 if (not args.check or report["ok"]) else 1


if __name__ == "__main__":
    raise SystemExit(main())
