#!/usr/bin/env python3
"""Report and validate the reconstructed TH10 Main execution corridor."""

from __future__ import annotations

import argparse
import json
from pathlib import Path
import re
import struct
import sys

from target_identity import pe_bytes_at, resolve_target, verify_target


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_SOURCE = ROOT / "src" / "Main.cpp"
DEFAULT_HEADER = ROOT / "src" / "Main.hpp"

FUNCTIONS = (
    (0x00438AD0, 0x0043903C, 0x00439040, b"\xc2\x10\x00", "WinMain"),
    (
        0x004390E0,
        0x004391E8,
        0x004391F0,
        b"\xc2\x10\x00",
        "GameWindowView::WindowProc",
    ),
    (0x004391F0, 0x004392DB, 0x004392E0, b"\xc3", "GameWindowView::Present"),
    (
        0x00439390,
        0x00439538,
        0x00439540,
        b"\xc2\x04\x00",
        "GameWindowView::Render",
    ),
    (
        0x00439540,
        0x00439659,
        0x00439660,
        b"\xc3",
        "GameWindowView::GetTimestamp",
    ),
    (
        0x00439730,
        0x00439884,
        0x00439890,
        b"\xc3",
        "GameWindowView::CreateGameWindow",
    ),
    (
        0x00439890,
        0x00439D16,
        0x00439D20,
        b"\xc3",
        "GameWindowView::InitD3DRendering",
    ),
    (
        0x00439D20,
        0x00439FE5,
        0x00439FF0,
        b"\xc3",
        "GameWindowView::ResetRenderState",
    ),
    (
        0x00439FF0,
        0x0043A1A9,
        0x0043A1B0,
        b"\xc3",
        "GameWindowView::CheckForRunningGameInstance",
    ),
    (
        0x0043A1C0,
        0x0043A28E,
        0x0043A290,
        b"\xc3",
        "GameWindowView::CalcExecutableChecksum",
    ),
)

RENDER_STATES = (
    (0x07, 0x00000001),
    (0x89, 0x00000000),
    (0x16, 0x00000001),
    (0x1B, 0x00000001),
    (0x09, 0x00000002),
    (0x13, 0x00000005),
    (0x14, 0x00000006),
    (0x17, 0x00000008),
    (0x0F, 0x00000001),
    (0x18, 0x00000001),
    (0x19, 0x00000007),
    (0x1C, 0x00000001),
    (0x26, 0x3F800000),
    (0x23, 0x00000000),
    (0x8C, 0x00000003),
    (0x22, 0xFFA0A0A0),
    (0x24, 0x447A0000),
    (0x25, 0x459C4000),
    (0xA1, 0x00000000),
)

TEXTURE_STAGE_STATES = (
    (0x04, 0x00000004),
    (0x05, 0x00000002),
    (0x06, 0x00000003),
    (0x01, 0x00000004),
    (0x02, 0x00000002),
    (0x03, 0x00000003),
    (0x18, 0x00000002),
    (0x0B, 0x00000000),
)

SAMPLER_STATES = (
    (0x07, 0x00000000),
    (0x05, 0x00000002),
    (0x06, 0x00000002),
    (0x03, 0x00000003),
    (0x01, 0x00000001),
    (0x02, 0x00000001),
)

MARKER_PATTERN = re.compile(
    r"^// TH10_MAIN_FUNCTION: (0x[0-9A-Fa-f]{8}) (.+)$", re.MULTILINE
)


def parse_state_calls(body: str, macro: str) -> tuple[tuple[int, str], ...]:
    pattern = re.compile(
        rf"^\s{{4}}{re.escape(macro)}\(\s*"
        r"(0x[0-9A-Fa-f]+),\s*(.*?)\);$",
        re.MULTILINE | re.DOTALL,
    )
    return tuple(
        (int(state, 0), " ".join(value.split()))
        for state, value in pattern.findall(body)
    )


def validate_state_calls(
    name: str,
    calls: tuple[tuple[int, str], ...],
    expected: tuple[tuple[int, int], ...],
) -> list[str]:
    problems: list[str] = []
    if tuple(state for state, _ in calls) != tuple(state for state, _ in expected):
        return [f"source {name} state order differs from target review"]

    float_render_states = {0x26, 0x24, 0x25} if name == "render" else set()
    for (state, expression), (_, value) in zip(calls, expected):
        if state in float_render_states:
            if expression != "*reinterpret_cast<unsigned int *>(&stateValue)":
                problems.append(
                    f"source {name} state 0x{state:X} does not use the reviewed float scratch"
                )
            continue
        try:
            source_value = int(expression, 0)
        except ValueError:
            problems.append(
                f"source {name} state 0x{state:X} has a non-literal value"
            )
        else:
            if source_value != value:
                problems.append(
                    f"source {name} state 0x{state:X} value differs from target review"
                )
    return problems


def source_report(source: Path, header: Path) -> tuple[dict[str, object], list[str]]:
    text = source.read_text(encoding="utf-8")
    header_text = header.read_text(encoding="utf-8")
    markers = [(int(address, 0), name) for address, name in MARKER_PATTERN.findall(text)]
    expected_markers = [(start, name) for start, _, _, _, name in FUNCTIONS]
    problems: list[str] = []
    if markers != expected_markers:
        problems.append("source markers do not match the reviewed Main owner order")

    reset_marker = "// TH10_MAIN_FUNCTION: 0x00439D20"
    next_marker = "// TH10_MAIN_FUNCTION: 0x00439FF0"
    if reset_marker not in text or next_marker not in text:
        reset_body = ""
        problems.append("source is missing the ResetRenderState marker interval")
    else:
        reset_body = text.split(reset_marker, 1)[1].split(next_marker, 1)[0]

    source_states = {
        "render": parse_state_calls(reset_body, "MAIN_SET_RENDER_STATE"),
        "texture_stage": parse_state_calls(
            reset_body, "MAIN_SET_TEXTURE_STAGE_STATE"
        ),
        "sampler": parse_state_calls(reset_body, "MAIN_SET_SAMPLER_STATE"),
    }
    expected_states = {
        "render": RENDER_STATES,
        "texture_stage": TEXTURE_STAGE_STATES,
        "sampler": SAMPLER_STATES,
    }
    for name, expected in expected_states.items():
        problems.extend(validate_state_calls(name, source_states[name], expected))

    for state, value in ((0x26, "1.0f"), (0x24, "1000.0f"), (0x25, "5000.0f")):
        pattern = re.compile(
            rf"stateValue\s*=\s*{re.escape(value)};\s*"
            rf"MAIN_SET_RENDER_STATE\(\s*0x{state:x},",
            re.IGNORECASE,
        )
        if not pattern.search(reset_body):
            problems.append(
                f"source render state 0x{state:X} is missing float value {value}"
            )

    required_layout_tokens = (
        "sizeof(GameWindowView) == 0x58",
        "offsetof(GameWindowView, performanceFrequency) == 0x18",
        "offsetof(GameWindowView, usesRelativePath) == 0x28",
        "offsetof(GameWindowView, currentTimestamp) == 0x38",
        "offsetof(MainSupervisorView, presentParameters) == 0x0e4",
        "offsetof(MainSupervisorView, gameplayViewport) == 0x26c",
        "offsetof(MainSupervisorView, criticalSections) == 0x64c",
        "offsetof(MainSupervisorView, lastFrameDuration) == 0x778",
        "sizeof(MainAllocationRegistryView) == 0xa004",
    )
    missing_layout_tokens = [
        token for token in required_layout_tokens if token not in header_text
    ]
    if missing_layout_tokens:
        problems.append(
            "header is missing layout assertions: " + ", ".join(missing_layout_tokens)
        )

    return (
        {
            "markers": [
                {"address": f"0x{address:08X}", "name": name}
                for address, name in markers
            ],
            "state_write_counts": {
                name: len(values) for name, values in source_states.items()
            },
            "layout_assertions": len(required_layout_tokens) - len(missing_layout_tokens),
        },
        problems,
    )


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
        image = target.read_bytes()
        source, source_problems = source_report(args.source, args.header)
    except (OSError, UnicodeError, ValueError) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 2

    problems = list(source_problems)
    functions: list[dict[str, object]] = []
    for start, end, next_start, epilogue, name in FUNCTIONS:
        body = pe_bytes_at(image, start, end - start + 1)
        padding = pe_bytes_at(image, end + 1, next_start - end - 1)
        epilogue_ok = body.endswith(epilogue)
        padding_ok = padding == b"\xcc" * len(padding)
        if not epilogue_ok:
            problems.append(f"{name} has an unexpected target epilogue")
        if not padding_ok:
            problems.append(f"{name} has unexpected target boundary padding")
        functions.append(
            {
                "address": f"0x{start:08X}",
                "end": f"0x{end:08X}",
                "size": end - start + 1,
                "name": name,
                "epilogue": epilogue.hex(),
                "epilogue_ok": epilogue_ok,
                "padding_bytes": len(padding),
                "padding_ok": padding_ok,
            }
        )

    create_window = pe_bytes_at(image, 0x00439730, 0x00439885 - 0x00439730)
    callback_pointer_ok = struct.pack("<I", 0x004390E0) in create_window
    if not callback_pointer_ok:
        problems.append("CreateGameWindow does not contain the reviewed WindowProc pointer")

    report = {
        "ok": not problems,
        "target": str(target),
        "target_sha256": observed["sha256"],
        "corridor": {
            "start": "0x00438AD0",
            "end": "0x0043A28E",
            "mapped_owner_count": len(FUNCTIONS),
            "mapped_owner_bytes": sum(end - start + 1 for start, end, *_ in FUNCTIONS),
        },
        "functions": functions,
        "window_proc_pointer_in_create_window": callback_pointer_ok,
        "source": source,
        "problems": problems,
    }

    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print(f"target OK: {target}")
        print(
            f"Main corridor: {report['corridor']['mapped_owner_count']} owners / "
            f"{report['corridor']['mapped_owner_bytes']} bytes"
        )
        print(
            "source state writes: "
            f"{source['state_write_counts']['render']} render, "
            f"{source['state_write_counts']['texture_stage']} texture-stage, "
            f"{source['state_write_counts']['sampler']} sampler"
        )
        if problems:
            for problem in problems:
                print(f"problem: {problem}", file=sys.stderr)
        else:
            print("Main corridor boundaries, ABI tails, layout and source coverage OK")
    return 1 if args.check and problems else 0


if __name__ == "__main__":
    raise SystemExit(main())
