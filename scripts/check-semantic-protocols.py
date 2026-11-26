#!/usr/bin/env python3
"""Regression guard for completed TH10 semantic protocols.

Enforces completed protocol invariants so semantic readability does not
silently regress back to bare numbers, duplicate ODR definitions, or
unverified enum values:
1. ProductGlobals.cpp uses canonical GameErrorContext.hpp and
   ScriptViewerWorker.hpp (no local duplicate struct definitions).
2. SupervisorLockSlot defines the exact evidenced set {CHAIN=0, LOG=3, MODE=5}
   with no extra/duplicate entries, and NO src/*.cpp file uses bare [0], [3],
   or [5] on criticalSections / criticalSectionLockCounts / g_CriticalSections.
3. ChainCallbackResult matches the exact 0..7 name/value mapping, and both
   Chain.cpp dispatchers cover their expected enum cases with no numeric cases.
4. SoundEffectId matches the exact 0..36 + SE_COUNT=37 mapping, Sound.cpp
   g_SoundFileNames[SE_COUNT] has 37 matching entries, and Sound.cpp uses SE_COUNT.
5. SupervisorGameMode includes SUPERVISOR_MODE_UNINITIALIZED = -2.
6. GameInputButtonMask matches the exact evidenced bitmask mapping, and
   completed consumers (InputPolling.cpp MergeJoystickInput, Main.cpp screenshot)
   use named masks.
7. src/resource.h defines 201/202/203 and is included by both th10.rc & Main.cpp.
8. docs/*.md contain no local machine file:///home/ links.
"""

from __future__ import annotations

from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[1]


def parse_enum(text: str, enum_name: str) -> tuple[dict[str, int], list[str]]:
    """Parse a C++ enum { NAME = VAL, ... }; block and detect duplicates."""
    m = re.search(rf"\benum\s+{re.escape(enum_name)}\s*\{{(.*?)\}}\s*;", text, re.S)
    if not m:
        return {}, [f"enum {enum_name} not found"]
    body = re.sub(r"//.*", "", m.group(1))
    body = re.sub(r"/\*.*?\*/", "", body, flags=re.S)
    entries: dict[str, int] = {}
    errs: list[str] = []
    next_val = 0
    for raw_item in body.split(","):
        item = raw_item.strip()
        if not item:
            continue
        em = re.match(r"^([A-Za-z_]\w*)(?:\s*=\s*(-?(?:0x[0-9A-Fa-f]+|[0-9]+)))?$", item)
        if not em:
            errs.append(f"enum {enum_name}: unparseable item {item!r}")
            continue
        name = em.group(1)
        val = int(em.group(2), 0) if em.group(2) is not None else next_val
        if name in entries:
            errs.append(f"enum {enum_name}: duplicate name {name}")
        if val in entries.values():
            errs.append(f"enum {enum_name}: duplicate value {val} on {name}")
        entries[name] = val
        next_val = val + 1
    return entries, errs


def check_exact_enum(
    text: str, enum_name: str, expected: dict[str, int], source_label: str, failures: list[str]
) -> None:
    actual, errs = parse_enum(text, enum_name)
    for e in errs:
        failures.append(f"{source_label}: {e}")
    if actual != expected:
        missing = {k: v for k, v in expected.items() if actual.get(k) != v}
        extra = {k: v for k, v in actual.items() if k not in expected}
        failures.append(
            f"{source_label}: enum {enum_name} mismatch (missing/changed={missing}, extra={extra})"
        )


def main() -> int:
    failures: list[str] = []

    # 1. ProductGlobals.cpp ODR check
    pg = (ROOT / "src" / "ProductGlobals.cpp").read_text(errors="replace")
    if '#include "GameErrorContext.hpp"' not in pg or '#include "ScriptViewerWorker.hpp"' not in pg:
        failures.append("src/ProductGlobals.cpp must include GameErrorContext.hpp and ScriptViewerWorker.hpp")
    if re.search(r"\bstruct\s+(GameErrorContext|ScriptViewerWorkerView)\s*\{", pg):
        failures.append("src/ProductGlobals.cpp contains a duplicate local struct GameErrorContext/ScriptViewerWorkerView")

    # 2. SupervisorLockSlot exact domain + repo-wide src/*.cpp lock-index scan
    lock_hpp = (ROOT / "src" / "SupervisorLockSlot.hpp").read_text(errors="replace")
    check_exact_enum(
        lock_hpp,
        "SupervisorLockSlot",
        {
            "SUPERVISOR_LOCK_CHAIN": 0,
            "SUPERVISOR_LOCK_LOG": 3,
            "SUPERVISOR_LOCK_MODE": 5,
        },
        "src/SupervisorLockSlot.hpp",
        failures,
    )

    lock_index_pat = re.compile(
        r"\b(criticalSections|criticalSectionLockCounts|g_CriticalSections)\s*\[\s*(?:0x0*[035]|[035])\s*\]"
    )
    for cpp in sorted((ROOT / "src").glob("*.cpp")):
        for lineno, line in enumerate(cpp.read_text(errors="replace").splitlines(), 1):
            m = lock_index_pat.search(line)
            if m:
                failures.append(
                    f"{cpp.relative_to(ROOT)}:{lineno} uses bare lock slot '{m.group(0)}' instead of SupervisorLockSlot"
                )

    # 3. ChainCallbackResult exact 0..7 mapping + Chain.cpp switch coverage
    chain_hpp = (ROOT / "src" / "Chain.hpp").read_text(errors="replace")
    check_exact_enum(
        chain_hpp,
        "ChainCallbackResult",
        {
            "CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_NO_DELETE": 0,
            "CHAIN_CALLBACK_RESULT_CONTINUE": 1,
            "CHAIN_CALLBACK_RESULT_EXECUTE_AGAIN": 2,
            "CHAIN_CALLBACK_RESULT_BREAK": 3,
            "CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS": 4,
            "CHAIN_CALLBACK_RESULT_EXIT_GAME_ERROR": 5,
            "CHAIN_CALLBACK_RESULT_RESTART_FROM_FIRST_JOB": 6,
            "CHAIN_CALLBACK_RESULT_INVOKE_DELETED_CALLBACK": 7,
        },
        "src/Chain.hpp",
        failures,
    )
    chain_cpp = (ROOT / "src" / "Chain.cpp").read_text(errors="replace")
    if re.search(r"\bcase\s+(?:0x[0-9A-Fa-f]+|[0-9]+)\s*:", chain_cpp):
        failures.append("src/Chain.cpp contains bare numeric (decimal/hex) switch case labels")

    calc_m = re.search(r"MainRunCalcChain\b(.*?)MainRunDrawChain\b", chain_cpp, re.S)
    draw_m = re.search(r"MainRunDrawChain\b(.*)", chain_cpp, re.S)
    if not calc_m or not draw_m:
        failures.append("src/Chain.cpp missing MainRunCalcChain or MainRunDrawChain")
    else:
        calc_cases = set(re.findall(r"\bcase\s+(?:th10::)?(CHAIN_CALLBACK_RESULT_\w+)\s*:", calc_m.group(1)))
        draw_cases = set(re.findall(r"\bcase\s+(?:th10::)?(CHAIN_CALLBACK_RESULT_\w+)\s*:", draw_m.group(1)))
        exp_calc = {
            "CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_NO_DELETE",
            "CHAIN_CALLBACK_RESULT_EXECUTE_AGAIN",
            "CHAIN_CALLBACK_RESULT_BREAK",
            "CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS",
            "CHAIN_CALLBACK_RESULT_EXIT_GAME_ERROR",
            "CHAIN_CALLBACK_RESULT_RESTART_FROM_FIRST_JOB",
            "CHAIN_CALLBACK_RESULT_INVOKE_DELETED_CALLBACK",
        }
        exp_draw = {
            "CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_NO_DELETE",
            "CHAIN_CALLBACK_RESULT_EXECUTE_AGAIN",
            "CHAIN_CALLBACK_RESULT_BREAK",
            "CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS",
            "CHAIN_CALLBACK_RESULT_EXIT_GAME_ERROR",
        }
        if calc_cases != exp_calc:
            failures.append(f"src/Chain.cpp MainRunCalcChain cases={sorted(calc_cases)} != expected={sorted(exp_calc)}")
        if draw_cases != exp_draw:
            failures.append(f"src/Chain.cpp MainRunDrawChain cases={sorted(draw_cases)} != expected={sorted(exp_draw)}")

    # 4. SoundEffectId exact 0..36 + SE_COUNT=37 mapping & Sound.cpp table/loop closure
    expected_se_names = [
        "SE_PLST00", "SE_ENEP00", "SE_PLDEAD00", "SE_POWER0", "SE_POWER1",
        "SE_TAN00", "SE_TAN01", "SE_TAN02", "SE_OK00", "SE_CANCEL00",
        "SE_SELECT00", "SE_GUN00", "SE_CAT00", "SE_LAZER00", "SE_LAZER01",
        "SE_ENEP01", "SE_DAMAGE00", "SE_ITEM00", "SE_KIRA00", "SE_KIRA01",
        "SE_KIRA02", "SE_TIMEOUT", "SE_GRAZE", "SE_POWERUP", "SE_PAUSE",
        "SE_CARDGET", "SE_OPTION", "SE_DAMAGE01", "SE_TIMEOUT2", "SE_INVALID",
        "SE_SLASH", "SE_CH00", "SE_CH01", "SE_HINT00", "SE_EXTEND",
        "SE_BONUS3", "SE_WATER",
    ]
    expected_se = {name: i for i, name in enumerate(expected_se_names)}
    expected_se["SE_COUNT"] = 37
    sound_hpp = (ROOT / "src" / "Sound.hpp").read_text(errors="replace")
    check_exact_enum(sound_hpp, "SoundEffectId", expected_se, "src/Sound.hpp", failures)

    sound_cpp = (ROOT / "src" / "Sound.cpp").read_text(errors="replace")
    if re.search(r"g_SoundFileNames\[37\]|i\s*<\s*37", sound_cpp):
        failures.append("src/Sound.cpp uses bare 37 instead of SE_COUNT")
    se_init_m = re.search(r"g_SoundFileNames\[SE_COUNT\]\s*=\s*\{(.*?)\};", sound_cpp, re.S)
    if not se_init_m:
        failures.append("src/Sound.cpp missing g_SoundFileNames[SE_COUNT] initializer")
    else:
        se_files = re.findall(r'"([^"]+)"', se_init_m.group(1))
        expected_files = [f"{name.lower()}.wav" for name in expected_se_names]
        if se_files != expected_files:
            failures.append(
                f"src/Sound.cpp g_SoundFileNames initializer mismatch (count={len(se_files)}, expected={len(expected_files)})"
            )

    # 5. SupervisorGameMode sentinel check
    main_hpp = (ROOT / "src" / "Main.hpp").read_text(errors="replace")
    if not re.search(r"SUPERVISOR_MODE_UNINITIALIZED\s*=\s*-2\b", main_hpp):
        failures.append("src/Main.hpp missing SUPERVISOR_MODE_UNINITIALIZED = -2")

    # 6. GameInputButtonMask exact mapping + completed consumer regression checks
    input_hpp = (ROOT / "src" / "InputPolling.hpp").read_text(errors="replace")
    check_exact_enum(
        input_hpp,
        "GameInputButtonMask",
        {
            "INPUT_BUTTON_SHOOT": 0x0001,
            "INPUT_BUTTON_BOMB": 0x0002,
            "INPUT_BUTTON_PAUSE": 0x0008,
            "INPUT_BUTTON_UP": 0x0010,
            "INPUT_BUTTON_DOWN": 0x0020,
            "INPUT_BUTTON_LEFT": 0x0040,
            "INPUT_BUTTON_RIGHT": 0x0080,
            "INPUT_BUTTON_SKIP": 0x0100,
            "INPUT_BUTTON_FOCUS": 0x0200,
            "INPUT_BUTTON_SCREENSHOT": 0x0800,
        },
        "src/InputPolling.hpp",
        failures,
    )
    input_cpp = (ROOT / "src" / "InputPolling.cpp").read_text(errors="replace")
    merge_m = re.search(r"MergeJoystickInput\b(.*?)FrontEndGetControllerState\b", input_cpp, re.S)
    if not merge_m:
        failures.append("src/InputPolling.cpp missing MergeJoystickInput")
    else:
        body = merge_m.group(1)
        # Check for bare named button masks (excluding 4 / 0x04 which is intentionally unnamed)
        bare_input_pat = re.compile(
            r"(?:\|=|\?\s*|,)\s*(?:0x0*(?:1|2|8|10|20|40|80|100|200|800)|(?:1|2|8|16|32|64|128|256|512|2048))\s*(?:[:;,)])"
        )
        # Only check MergeWinMmButton/MergeDirectButton action args and ternary bitmask outputs
        if re.search(
            r"(?:MergeWinMmButton|MergeDirectButton)\([^,]+,[^,]+,\s*(?:0x0*(?:1|2|8|100)|(?:1|2|8|256))\s*,",
            body,
        ) or re.search(
            r"\?\s*(?:0x0*(?:10|20|40|80)|(?:16|32|64|128))\s*:\s*0",
            body,
        ):
            failures.append("src/InputPolling.cpp MergeJoystickInput regressed to bare numeric button mask")
        for req in ("INPUT_BUTTON_SHOOT", "INPUT_BUTTON_BOMB", "INPUT_BUTTON_PAUSE", "INPUT_BUTTON_UP", "INPUT_BUTTON_DOWN", "INPUT_BUTTON_LEFT", "INPUT_BUTTON_RIGHT", "INPUT_BUTTON_SKIP"):
            if req not in body:
                failures.append(f"src/InputPolling.cpp MergeJoystickInput missing {req}")

    main_cpp = (ROOT / "src" / "Main.cpp").read_text(errors="replace")
    if re.search(r"g_MainInputEdges\s*&\s*0x0*800\b", main_cpp):
        failures.append("src/Main.cpp uses bare 0x0800 instead of INPUT_BUTTON_SCREENSHOT")
    if "INPUT_BUTTON_SCREENSHOT" not in main_cpp:
        failures.append("src/Main.cpp missing INPUT_BUTTON_SCREENSHOT")

    # 7. Shared Win32 dialog resource IDs
    res_h = (ROOT / "src" / "resource.h").read_text(errors="replace")
    for macro, val in (
        ("IDD_MAIN_STARTUP_MODE", "203"),
        ("IDC_MAIN_FULLSCREEN", "201"),
        ("IDC_MAIN_ASK_ALWAYS", "202"),
        ("IDC_MAIN_WINDOWED", "203"),
    ):
        if not re.search(rf"#define\s+{macro}\s+{val}\b", res_h):
            failures.append(f"src/resource.h missing #define {macro} {val}")
    rc = (ROOT / "src" / "th10.rc").read_text(errors="replace")
    if '#include "resource.h"' not in rc or '#include "resource.h"' not in main_cpp:
        failures.append("both src/th10.rc and src/Main.cpp must include resource.h")

    # 8. SoundPlayerCommandOpcode exact 0..8 mapping + zero bare opcodes across src/*.cpp
    cmd_hpp = (ROOT / "src" / "SoundCommandOpcode.hpp").read_text(errors="replace")
    check_exact_enum(
        cmd_hpp,
        "SoundPlayerCommandOpcode",
        {
            "SOUND_COMMAND_NONE": 0,
            "SOUND_COMMAND_PRELOAD_BGM": 1,
            "SOUND_COMMAND_LOAD_BGM": 2,
            "SOUND_COMMAND_STOP_BGM": 3,
            "SOUND_COMMAND_RELEASE_BGM": 4,
            "SOUND_COMMAND_FADE_BGM": 5,
            "SOUND_COMMAND_PAUSE_BGM": 6,
            "SOUND_COMMAND_UNPAUSE_BGM": 7,
            "SOUND_COMMAND_SET_BGM_VOLUME": 8,
        },
        "src/SoundCommandOpcode.hpp",
        failures,
    )
    bare_cmd_pat = re.compile(
        r"\b(?:QueueCommand|FrontEndQueueSoundCommand)\s*\([^;]*?,\s*(?:0x[0-9A-Fa-f]+|[0-9]+)\s*,",
        re.S,
    )
    for cpp in sorted((ROOT / "src").glob("*.cpp")):
        txt = cpp.read_text(errors="replace")
        if bare_cmd_pat.search(txt):
            failures.append(f"{cpp.relative_to(ROOT)} passes bare numeric opcode to QueueCommand/FrontEndQueueSoundCommand")
    if "commandQueue[index].opcode == 0" in sound_cpp:
        failures.append("src/Sound.cpp uses bare 0 instead of SOUND_COMMAND_NONE")

    # 9. No local machine absolute links in docs/
    for doc in sorted((ROOT / "docs").glob("*.md")):
        if "file:///home/" in doc.read_text(errors="replace"):
            failures.append(f"{doc.relative_to(ROOT)} contains a local file:///home/ link")

    if failures:
        for f in failures:
            print(f"semantic-guard FAIL: {f}", file=sys.stderr)
        return 1
    print("semantic-guard OK: all completed protocol invariants passed.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
