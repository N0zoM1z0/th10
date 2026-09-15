#!/usr/bin/env python3
"""Report and validate the reconstructed TH10 key-configuration core."""

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

UPDATE_START = 0x0042F540
UPDATE_CODE_END = 0x0042F89A
UPDATE_ALIGNMENT = 0x0042F89B
UPDATE_TABLE = 0x0042F89C
UPDATE_TABLE_DESTINATIONS = (
    0x0042F556,
    0x0042F5DC,
    0x0042F62B,
    0x0042F892,
    0x0042F875,
)
UPDATE_OWNER_END = 0x0042F8AF

DISPLAY_START = 0x0042F8B0
DISPLAY_END = 0x00430243
ASSIGN_START = 0x00430250
ASSIGN_END = 0x00430317
NEXT_FUNCTION = 0x00430320

CONTROLLER_MAPPING = 0x00474E88
DEFAULT_CONTROLLER_MAPPING = (0, 1, 2, 3, -1, -1, -1, -1, 4)

FUNCTIONS = (
    (UPDATE_START, UPDATE_OWNER_END, "FrontEndControllerView::UpdateKeyConfig"),
    (
        DISPLAY_START,
        DISPLAY_END,
        "FrontEndControllerView::RefreshKeyConfigDisplay",
    ),
    (
        ASSIGN_START,
        ASSIGN_END,
        "FrontEndControllerView::AssignKeyConfigBinding",
    ),
)

EXPECTED_CALLS = {
    UPDATE_START: {
        0x0042C5C0: 1,
        0x0042C620: 3,
        0x0042C670: 1,
        0x0042C770: 3,
        0x0042F8B0: 3,
        0x00430250: 1,
        0x0043DC90: 3,
        0x00449250: 2,
        0x0044A4E0: 1,
        0x0044BE70: 1,
        0x0044BEA0: 2,
    },
    DISPLAY_START: {
        0x0043E5A0: 19,
        0x004491C0: 40,
    },
    ASSIGN_START: {
        0x0042F8B0: 1,
        0x0043DC90: 1,
    },
}

MARKER_PATTERN = re.compile(
    r"^// TH10_FRONTEND_FUNCTION: (0x[0-9A-Fa-f]{8}) (.+)$",
    re.MULTILINE,
)
STATE_PATTERN = re.compile(
    r"^\s*(FRONT_END_KEY_CONFIG_[A-Z]+)\s*=\s*"
    r"(0x[0-9A-Fa-f]+|[0-9]+)\s*[,}]?",
    re.MULTILINE,
)


def direct_flow_counts(
    image: bytes, start: int, end: int
) -> tuple[Counter[int], Counter[int]]:
    from capstone import CS_ARCH_X86, CS_MODE_32, Cs
    from capstone.x86_const import X86_OP_IMM

    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    calls: Counter[int] = Counter()
    jumps: Counter[int] = Counter()
    body = pe_bytes_at(image, start, end - start + 1)
    consumed = 0
    for instruction in decoder.disasm(body, start):
        if instruction.address != start + consumed:
            break
        consumed += instruction.size
        if not instruction.operands or instruction.operands[0].type != X86_OP_IMM:
            continue
        destination = int(instruction.operands[0].imm) & 0xFFFFFFFF
        if instruction.mnemonic == "call":
            calls[destination] += 1
        elif instruction.mnemonic == "jmp":
            jumps[destination] += 1
    if consumed != len(body):
        raise ValueError(
            f"code at 0x{start:08X} decoded {consumed}/{len(body)} bytes"
        )
    return calls, jumps


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

    reviewed_starts = {start for start, _, _ in FUNCTIONS}
    markers = [
        (int(address, 0), name)
        for address, name in MARKER_PATTERN.findall(text)
        if int(address, 0) in reviewed_starts
    ]
    expected_markers = [(start, name) for start, _, name in FUNCTIONS]
    if markers != expected_markers:
        problems.append("source markers do not match reviewed owner order")

    states = {
        name: int(value, 0)
        for name, value in STATE_PATTERN.findall(header_text)
    }
    if set(states.values()) != set(range(5)):
        problems.append("key-configuration state enum does not cover exactly 0..4")

    required_source = (
        "controller->cursor.SetCurrent(0)",
        "FRONT_END_KEY_CONFIG_ROOT_VM = 2",
        "FRONT_END_CONTROLLER_BUTTON_COUNT = 31",
        "controllerState[controllerButton] < 0",
        "controller->cursor.current == 5",
        "controller->cursor.current == 6",
        "CommitKeyConfigBindings(controller)",
        "g_FrontEndSavedControllerMapping = g_FrontEndControllerMapping",
        "binding / 10 + 0x33",
        "binding % 10 + 0x33",
        "0x43, 0x4d",
        "0x45, 0x4f",
        "0x47, 0x51",
        "0x49, 0x53",
        "0x4b, 0x55",
        "UpdateKeyConfig(this)",
    )
    for token in required_source:
        if token not in text:
            problems.append(f"source is missing reviewed behavior token: {token}")

    required_layout = (
        "sizeof(FrontEndControllerMappingView) == 0x12",
        "offsetof(FrontEndControllerView, keyConfigBindings) == 0x59cc",
        "short shotButton;",
        "short bombButton;",
        "short focusButton;",
        "short menuButton;",
        "short upButton;",
        "short downButton;",
        "short leftButton;",
        "short rightButton;",
        "short skipButton;",
    )
    for token in required_layout:
        if token not in header_text:
            problems.append(f"front-end header is missing layout token: {token}")

    if "extern int FrontEndUpdateKeyConfig" in text:
        problems.append("parent dispatcher still uses an unreconstructed key-config stub")

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
        state_table = struct.unpack(
            "<5I", pe_bytes_at(image, UPDATE_TABLE, 5 * 4)
        )
        default_mapping = struct.unpack(
            "<9h", pe_bytes_at(image, CONTROLLER_MAPPING, 9 * 2)
        )
        flows = {
            UPDATE_START: direct_flow_counts(image, UPDATE_START, UPDATE_CODE_END),
            DISPLAY_START: direct_flow_counts(image, DISPLAY_START, DISPLAY_END),
            ASSIGN_START: direct_flow_counts(image, ASSIGN_START, ASSIGN_END),
        }
        source, source_problems = source_report(args.source, args.header)
        extents = tracking_extents()
    except (
        OSError,
        UnicodeError,
        ValueError,
        KeyError,
        struct.error,
        LinkedImageError,
        tomllib.TOMLDecodeError,
    ) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 2

    problems = list(source_problems)
    if pe_bytes_at(image, UPDATE_CODE_END, 2) != b"\xc3\x90":
        problems.append("key-config update code/alignment tail differs from RET/NOP")
    if state_table != UPDATE_TABLE_DESTINATIONS:
        problems.append("key-config five-entry state table differs from review")
    if pe_bytes_at(image, DISPLAY_START, 1) != b"\x53":
        problems.append("display owner does not begin immediately after the state table")
    if pe_bytes_at(image, DISPLAY_END, 13) != b"\xc3" + b"\xcc" * 12:
        problems.append("display owner does not end in RET plus twelve CC bytes")
    if pe_bytes_at(image, ASSIGN_END, 9) != b"\xc3" + b"\xcc" * 8:
        problems.append("assignment owner does not end in RET plus eight CC bytes")
    if pe_bytes_at(image, NEXT_FUNCTION, 1) == b"\xcc":
        problems.append("next function does not begin at the reviewed address")
    if default_mapping != DEFAULT_CONTROLLER_MAPPING:
        problems.append("target default nine-short controller mapping differs from review")

    for source_start, expected in EXPECTED_CALLS.items():
        calls, _ = flows[source_start]
        if calls != Counter(expected):
            problems.append(
                f"direct-call multiset for 0x{source_start:08X} differs from review"
            )
    if flows[DISPLAY_START][1][0x0043E5A0] != 1:
        problems.append("display owner lacks the reviewed final SetSprite tail jump")

    for start, end, _ in FUNCTIONS:
        if extents.get(start) != end:
            problems.append(
                f"boundary ledger extent for 0x{start:08X} is not 0x{end:08X}"
            )

    report = {
        "ok": not problems,
        "target": str(target),
        "target_sha256": observed["sha256"],
        "decoder": {"name": "capstone", **decoder_identity},
        "owners": [
            {
                "start": f"0x{start:08X}",
                "end": f"0x{end:08X}",
                "size": end - start + 1,
                "name": name,
            }
            for start, end, name in FUNCTIONS
        ],
        "state_table": [f"0x{value:08X}" for value in state_table],
        "default_controller_mapping": list(default_mapping),
        "reviewed_call_counts": {
            f"0x{start:08X}": {
                f"0x{destination:08X}": count
                for destination, count in sorted(flows[start][0].items())
            }
            for start in flows
        },
        "display_set_sprite_tail_jumps": flows[DISPLAY_START][1][0x0043E5A0],
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
        print("state table: 5 entries; controller mapping: 9 signed shorts")
        print("display transfers: FindVm x40; SetSprite calls x19 + tail jump x1")
        if problems:
            for problem in problems:
                print(f"problem: {problem}", file=sys.stderr)
        else:
            print("front-end key-configuration target and source coverage OK")
    return 1 if args.check and problems else 0


if __name__ == "__main__":
    raise SystemExit(main())
