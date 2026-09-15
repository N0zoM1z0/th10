#!/usr/bin/env python3
"""Report and validate the reconstructed TH10 selection-screen corridor."""

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

FUNCTIONS = (
    {
        "start": 0x00430320,
        "code_end": 0x00430680,
        "table": 0x00430684,
        "owner_end": 0x00430697,
        "next": 0x004306A0,
        "name": "FrontEndControllerView::UpdateDifficulty",
        "table_destinations": (
            0x00430351, 0x004303FD, 0x00430423, 0x0043059A, 0x004305FA,
        ),
    },
    {
        "start": 0x004306A0,
        "code_end": 0x00430A47,
        "table": 0x00430A48,
        "owner_end": 0x00430A5B,
        "next": 0x00430A60,
        "name": "FrontEndControllerView::UpdateCharacter",
        "table_destinations": (
            0x004306BB, 0x004307D4, 0x004307F9, 0x00430966, 0x00430A00,
        ),
    },
    {
        "start": 0x00430A60,
        "code_end": 0x00430FDA,
        "table": 0x00430FDC,
        "owner_end": 0x00430FEF,
        "next": 0x00430FF0,
        "name": "FrontEndControllerView::UpdateShotType",
        "table_destinations": (
            0x00430A7B, 0x00430D3E, 0x00430D63, 0x00430E7D, 0x00430F95,
        ),
    },
)

EXPECTED_CALLS = {
    0x00430320: {
        0x00409E50: 1, 0x0040ACE0: 2, 0x0040AD20: 1,
        0x0042C5C0: 2, 0x0042C620: 4, 0x0042C670: 3,
        0x0042C6D0: 5, 0x0042C750: 1, 0x0042C770: 5,
        0x0043DC90: 3, 0x00448D00: 1, 0x004491C0: 2,
        0x00449250: 1, 0x00449470: 1, 0x004497D0: 1,
        0x0044BE20: 1, 0x0044BE70: 1, 0x0044BEA0: 2,
    },
    0x004306A0: {
        0x0040AD20: 1, 0x0042C5C0: 2, 0x0042C620: 4,
        0x0042C670: 2, 0x0042C6D0: 4, 0x0042C750: 1,
        0x0042C770: 2, 0x0043DC90: 3, 0x00449250: 1,
        0x00449470: 4, 0x004497D0: 4, 0x0044BE20: 1,
        0x0044BE70: 1, 0x0044BEA0: 2,
    },
    0x00430A60: {
        0x0040C540: 1, 0x00420C30: 1, 0x0042C5C0: 4,
        0x0042C620: 4, 0x0042C670: 2, 0x0042C6D0: 5,
        0x0042C750: 1, 0x0042C770: 2, 0x0043C8B0: 1,
        0x0043DC90: 3, 0x00449250: 1, 0x00449470: 1,
        0x004495E0: 3, 0x004497D0: 4, 0x0044BE20: 2,
        0x0044BE70: 1, 0x0044BEA0: 2,
    },
}

MARKER_PATTERN = re.compile(
    r"^// TH10_FRONTEND_FUNCTION: (0x[0-9A-Fa-f]{8}) (.+)$",
    re.MULTILINE,
)
STATE_PATTERN = re.compile(
    r"^\s*(FRONT_END_SELECTION_[A-Z]+)\s*=\s*"
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
    reviewed_starts = {int(item["start"]) for item in FUNCTIONS}
    markers = [
        (int(address, 0), name)
        for address, name in MARKER_PATTERN.findall(text)
        if int(address, 0) in reviewed_starts
    ]
    expected_markers = [
        (int(item["start"]), str(item["name"])) for item in FUNCTIONS
    ]
    if markers != expected_markers:
        problems.append("source markers do not match reviewed owner order")

    states = {
        name: int(value, 0)
        for name, value in STATE_PATTERN.findall(header_text)
    }
    if set(states.values()) != set(range(5)):
        problems.append("selection state enum does not cover exactly 0..4")

    required_source = (
        "UpdateDifficulty(this)",
        "UpdateCharacter(this)",
        "UpdateShotType(this)",
        "0x77 + (g_ReplayDifficulty >= 4)",
        "controller->difficultyAuxVmId",
        "controller->savedDifficulty",
        "g_FrontEndProfileData[0x1d888]",
        "g_FrontEndProfileData + 0x1d888 + g_ReplayCharacter * 3",
        "g_ReplayCharacter * 0x329d + g_ReplayDifficulty",
        "g_ReplayCharacter * 3 + 0x3f",
        "EnsureAsciiSelectionVm(480.0f, 392.0f)",
        "FrontEndBeginSelectionTransition(5, 0x20, 0, 0, 0, 0x2b)",
        "g_FrontEndStageRecords[6]",
        "FrontEndFinalizeGameSelection(6.0f)",
    )
    for token in required_source:
        if token not in text:
            problems.append(f"source is missing reviewed behavior token: {token}")

    required_layout = (
        "AnmVmIdView vmIds[0x98]",
        "offsetof(FrontEndControllerView, difficultyAuxVmId) == 0x5d0",
        "offsetof(FrontEndControllerView, savedDifficulty) == 0x58f0",
        "offsetof(FrontEndControllerView, keyConfigBindings) == 0x59cc",
        "void DisableEntry(int value)",
    )
    for token in required_layout:
        if token not in header_text:
            problems.append(f"front-end header is missing layout token: {token}")

    for stale in (
        "extern int FrontEndUpdateDifficulty",
        "extern int FrontEndUpdateCharacter",
        "extern int FrontEndUpdateShotType",
    ):
        if stale in text:
            problems.append(f"parent dispatcher still uses source-absent stub: {stale}")

    return {
        "markers": [f"0x{address:08X} {name}" for address, name in markers],
        "state_values": sorted(states.values()),
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
            int(item["start"]): direct_call_counts(
                image, int(item["start"]), int(item["code_end"])
            )
            for item in FUNCTIONS
        }
        tables = {
            int(item["start"]): struct.unpack(
                "<5I", pe_bytes_at(image, int(item["table"]), 20)
            )
            for item in FUNCTIONS
        }
        source, source_problems = source_report(args.source, args.header)
        extents = tracking_extents()
    except (
        OSError, UnicodeError, ValueError, KeyError, struct.error,
        LinkedImageError, tomllib.TOMLDecodeError,
    ) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 2

    problems = list(source_problems)
    expected_tails = {
        0x00430320: b"\x8d\x49\x00",
        0x004306A0: b"",
        0x00430A60: b"\x90",
    }
    for item in FUNCTIONS:
        start = int(item["start"])
        code_end = int(item["code_end"])
        table = int(item["table"])
        if pe_bytes_at(image, code_end - 2, 3) != b"\xc2\x04\x00":
            problems.append(f"owner 0x{start:08X} code does not end in RET 4")
        if pe_bytes_at(image, code_end + 1, table - code_end - 1) != expected_tails[start]:
            problems.append(f"owner 0x{start:08X} alignment before table differs")
        if tables[start] != item["table_destinations"]:
            problems.append(f"owner 0x{start:08X} state table differs from review")
        if calls[start] != Counter(EXPECTED_CALLS[start]):
            problems.append(f"owner 0x{start:08X} direct-call multiset differs")
        if extents.get(start) != int(item["owner_end"]):
            problems.append(f"owner 0x{start:08X} boundary ledger omits its state table")
        if pe_bytes_at(image, int(item["next"]), 1) == b"\xcc":
            problems.append(f"next owner after 0x{start:08X} does not begin as reviewed")

    report = {
        "ok": not problems,
        "target": str(target),
        "target_sha256": observed["sha256"],
        "decoder": {"name": "capstone", **decoder_identity},
        "owners": [
            {
                "start": f"0x{int(item['start']):08X}",
                "end": f"0x{int(item['owner_end']):08X}",
                "size": int(item["owner_end"]) - int(item["start"]) + 1,
                "name": item["name"],
                "state_table": [f"0x{value:08X}" for value in tables[int(item["start"])]],
            }
            for item in FUNCTIONS
        ],
        "reviewed_call_counts": {
            f"0x{start:08X}": {
                f"0x{destination:08X}": count
                for destination, count in sorted(flow.items())
            }
            for start, flow in calls.items()
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
        if problems:
            for problem in problems:
                print(f"problem: {problem}", file=sys.stderr)
        else:
            print("front-end selection target and source coverage OK")
    return 1 if args.check and problems else 0


if __name__ == "__main__":
    raise SystemExit(main())
