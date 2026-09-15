#!/usr/bin/env python3
"""Report and validate the reconstructed TH10 front-end/options core."""

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
DEFAULT_MAIN_HEADER = ROOT / "src" / "Main.hpp"
DEFAULT_GUI_SOURCE = ROOT / "src" / "Gui.cpp"

TITLE_START = 0x0042CDF0
TITLE_CODE_END = 0x0042D212
TITLE_ALIGNMENT = 0x0042D213
TITLE_TABLE = 0x0042D214
TITLE_TABLE_DESTINATIONS = (
    0x0042CF36, 0x0042D065, 0x0042D0B6, 0x0042D14D,
    0x0042D13E, 0x0042D146, 0x0042D169, 0x0042D171,
    0x0042D179, 0x0042D181, 0x0042D162, 0x0042D191,
    0x0042D136, 0x0042D162, 0x0042D189, 0x0042CFFF,
    0x0042D199,
)
TITLE_OWNER_END = 0x0042D257
TITLE_NEXT_FUNCTION = 0x0042D260

OPTIONS_START = 0x0042D920
OPTIONS_CODE_END = 0x0042E57F
OPTIONS_TABLE = 0x0042E580
OPTIONS_TABLE_DESTINATIONS = (
    0x0042D942, 0x0042D97B, 0x0042DBCC, 0x0042E574, 0x0042E511,
)
OPTIONS_OWNER_END = 0x0042E593
OPTIONS_NEXT_FUNCTION = 0x0042E5A0

REFRESH_START = 0x0042E5A0
REFRESH_END = 0x0042F53E
REFRESH_NEXT_FUNCTION = 0x0042F540

FUNCTIONS = (
    (TITLE_START, TITLE_OWNER_END, "FrontEndControllerView::Update"),
    (OPTIONS_START, OPTIONS_OWNER_END, "FrontEndControllerView::UpdateOptions"),
    (
        REFRESH_START,
        REFRESH_END,
        "FrontEndControllerView::RefreshOptionsDisplay",
    ),
)

EXPECTED_CALLS = {
    TITLE_START: {
        0x0042D920: 1,
        0x004294A0: 1,
        0x004296F0: 1,
        0x004493E0: 5,
        0x00463B2C: 1,
    },
    OPTIONS_START: {
        0x0040ACE0: 2,
        0x00427C70: 1,
        0x0042E5A0: 8,
        0x004491C0: 24,
        0x00449670: 16,
        0x0044BEA0: 2,
    },
    REFRESH_START: {
        0x0043E460: 1,
        0x0043E5A0: 12,
        0x004491C0: 70,
        0x00463B2C: 1,
    },
}

MARKER_PATTERN = re.compile(
    r"^// TH10_FRONTEND_FUNCTION: (0x[0-9A-Fa-f]{8}) (.+)$",
    re.MULTILINE,
)
ENUM_PATTERN = re.compile(
    r"^\s*(FRONT_END_SCREEN_[A-Z0-9_]+)\s*=\s*"
    r"(0x[0-9A-Fa-f]+|[0-9]+)\s*[,}]?",
    re.MULTILINE,
)
STATE_PATTERN = re.compile(
    r"^\s*(FRONT_END_OPTIONS_[A-Z0-9_]+)\s*=\s*"
    r"(0x[0-9A-Fa-f]+|[0-9]+)\s*[,}]?",
    re.MULTILINE,
)


def read_pointers(image: bytes, address: int, count: int) -> tuple[int, ...]:
    return struct.unpack(
        f"<{count}I", pe_bytes_at(image, address, count * 4)
    )


def direct_call_counts(image: bytes, start: int, end: int) -> Counter[int]:
    from capstone import CS_ARCH_X86, CS_MODE_32, Cs
    from capstone.x86_const import X86_OP_IMM

    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    result: Counter[int] = Counter()
    body = pe_bytes_at(image, start, end - start + 1)
    consumed = 0
    for instruction in decoder.disasm(body, start):
        if instruction.address != start + consumed:
            break
        consumed += instruction.size
        if (
            instruction.mnemonic == "call"
            and instruction.operands
            and instruction.operands[0].type == X86_OP_IMM
        ):
            result[int(instruction.operands[0].imm) & 0xFFFFFFFF] += 1
    if consumed != len(body):
        raise ValueError(
            f"code at 0x{start:08X} decoded {consumed}/{len(body)} bytes"
        )
    return result


def tracking_extents() -> dict[int, int]:
    with (ROOT / "config" / "function-boundaries.csv").open(
        newline="", encoding="utf-8"
    ) as stream:
        return {
            int(row["address"], 0): int(row["span_end"], 0)
            for row in csv.DictReader(stream)
        }


def source_report(
    source: Path, header: Path, main_header: Path, gui_source: Path
) -> tuple[dict[str, object], list[str]]:
    text = source.read_text(encoding="utf-8")
    header_text = header.read_text(encoding="utf-8")
    main_text = main_header.read_text(encoding="utf-8")
    gui_text = gui_source.read_text(encoding="utf-8")
    problems: list[str] = []

    markers = [
        (int(address, 0), name)
        for address, name in MARKER_PATTERN.findall(text)
    ]
    expected_markers = [(start, name) for start, _, name in FUNCTIONS]
    if markers != expected_markers:
        problems.append("source markers do not match reviewed owner order")

    screens = {name: int(value, 0) for name, value in ENUM_PATTERN.findall(header_text)}
    states = {name: int(value, 0) for name, value in STATE_PATTERN.findall(header_text)}
    if set(screens.values()) != set(range(17)):
        problems.append("front-end screen enum does not cover exactly 0..16")
    if set(states.values()) != set(range(5)):
        problems.append("options-state enum does not cover exactly 0..4")

    required_source = (
        '"bgm/th10_02.wav"',
        '"SetVol"',
        "g_FrontEndDemoFrames < 900",
        "ReplayManager::Load(g_FrontEndDemoPath)",
        "controller->RefreshOptionsDisplay()",
        "value / 100 + 0x33",
        "value / 100 + 0x3d",
        "-5000 - static_cast<int>(-5000.0f * volume)",
        "TimerReachedMultiple(&controller->stateTimer, 60)",
    )
    for token in required_source:
        if token not in text:
            problems.append(f"source is missing reviewed behavior token: {token}")

    required_layout = (
        "sizeof(FrontEndCursorView) == 0xd8",
        "offsetof(FrontEndControllerView, screen) == 0x1c",
        "offsetof(FrontEndControllerView, stateTimer) == 0x2b0",
        "offsetof(FrontEndControllerView, vmIds) == 0x2c4",
    )
    for token in required_layout:
        if token not in header_text:
            problems.append(f"front-end header is missing layout token: {token}")

    for token in (
        "signed char bgmVolume;              // +0x138",
        "signed char sfxVolume;              // +0x139",
        "signed char colorMode;              // +0x13A",
    ):
        if token not in main_text:
            problems.append(f"Main header is missing options field: {token}")

    if "vm->scriptIndex == scriptIndex" not in text:
        problems.append("front-end child lookup does not use target field +0x38A")
    if "vm->scriptIndex == scriptIndex" not in gui_text:
        problems.append("GUI child lookup does not use target field +0x38A")

    return {
        "markers": [f"0x{address:08X} {name}" for address, name in markers],
        "screen_values": sorted(screens.values()),
        "options_state_values": sorted(states.values()),
    }, problems


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("executable", nargs="?", type=Path)
    parser.add_argument("--source", type=Path, default=DEFAULT_SOURCE)
    parser.add_argument("--header", type=Path, default=DEFAULT_HEADER)
    parser.add_argument("--main-header", type=Path, default=DEFAULT_MAIN_HEADER)
    parser.add_argument("--gui-source", type=Path, default=DEFAULT_GUI_SOURCE)
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
            decoder_identity = verify_capstone(
                tomllib.load(stream)["capstone"]
            )
        image = target.read_bytes()
        title_table = read_pointers(
            image, TITLE_TABLE, len(TITLE_TABLE_DESTINATIONS)
        )
        options_table = read_pointers(
            image, OPTIONS_TABLE, len(OPTIONS_TABLE_DESTINATIONS)
        )
        calls = {
            TITLE_START: direct_call_counts(image, TITLE_START, TITLE_CODE_END),
            OPTIONS_START: direct_call_counts(
                image, OPTIONS_START, OPTIONS_CODE_END
            ),
            REFRESH_START: direct_call_counts(
                image, REFRESH_START, REFRESH_END
            ),
        }
        source, source_problems = source_report(
            args.source, args.header, args.main_header, args.gui_source
        )
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
    if pe_bytes_at(image, TITLE_CODE_END, 2) != b"\xc3\x90":
        problems.append("title code/alignment tail differs from RET/NOP")
    if title_table != TITLE_TABLE_DESTINATIONS:
        problems.append("title 17-entry screen table differs from review")
    if pe_bytes_at(image, TITLE_OWNER_END + 1, 8) != b"\xcc" * 8:
        problems.append("title owner is not followed by eight CC bytes")
    if pe_bytes_at(image, OPTIONS_CODE_END - 2, 3) != b"\xc2\x04\x00":
        problems.append("options code does not end in RET 4")
    if options_table != OPTIONS_TABLE_DESTINATIONS:
        problems.append("options five-entry state table differs from review")
    if pe_bytes_at(image, OPTIONS_OWNER_END + 1, 12) != b"\xcc" * 12:
        problems.append("options owner is not followed by twelve CC bytes")
    if pe_bytes_at(image, REFRESH_END - 4, 5)[0] != 0xE9:
        problems.append("options-refresh owner does not end in a tail jump")
    if pe_bytes_at(image, REFRESH_END + 1, 1) != b"\xcc":
        problems.append("options-refresh owner is not followed by CC padding")

    for source_start, expected in EXPECTED_CALLS.items():
        for destination, count in expected.items():
            actual = calls[source_start][destination]
            if actual != count:
                problems.append(
                    f"0x{source_start:08X} calls 0x{destination:08X} "
                    f"{actual} times, expected {count}"
                )

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
        "title_table": [f"0x{value:08X}" for value in title_table],
        "options_table": [f"0x{value:08X}" for value in options_table],
        "reviewed_call_counts": {
            f"0x{start:08X}": {
                f"0x{destination:08X}": calls[start][destination]
                for destination in EXPECTED_CALLS[start]
            }
            for start in EXPECTED_CALLS
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
        print(
            f"screen/state tables: {len(title_table)} + {len(options_table)} entries"
        )
        print("reviewed calls: 42D920->42E5A0 x8; 42E5A0->4491C0 x70")
        if problems:
            for problem in problems:
                print(f"problem: {problem}", file=sys.stderr)
        else:
            print("front-end core target and source coverage OK")
    return 1 if args.check and problems else 0


if __name__ == "__main__":
    raise SystemExit(main())
