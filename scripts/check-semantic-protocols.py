#!/usr/bin/env python3
"""Lightweight regression guard for completed TH10 semantic protocols.

Enforces only completed protocol invariants so semantic readability does not
silently regress back to bare numbers, duplicate ODR definitions, or
unverified enum values:
1. ProductGlobals.cpp uses canonical GameErrorContext.hpp and
   ScriptViewerWorker.hpp (no local duplicate struct definitions).
2. SupervisorLockSlot defines only evidenced slots 0, 3, 5 (excludes
   unverified slot 2) and all closed consumers (Chain.cpp, ScriptViewer.cpp,
   ReplayManager.cpp, GameErrorContext.cpp) use named lock slots.
3. ChainCallbackResult covers 0..7 and both Chain.cpp dispatchers use named
   enum cases (no decimal or hex numeric case labels).
4. SoundEffectId defines SE_COUNT = 37 and Sound.cpp uses SE_COUNT (no bare 37).
5. SupervisorGameMode includes SUPERVISOR_MODE_UNINITIALIZED = -2.
6. GameInputButtonMask is used in Main.cpp (no bare 0x0800 on g_MainInputEdges).
7. src/resource.h defines 201/202/203 and is included by both th10.rc & Main.cpp.
8. docs/*.md contain no local machine file:///home/ links.
"""

from __future__ import annotations

from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[1]


def main() -> int:
    failures: list[str] = []

    # 1. ProductGlobals.cpp ODR check
    pg = (ROOT / "src" / "ProductGlobals.cpp").read_text(errors="replace")
    if '#include "GameErrorContext.hpp"' not in pg or '#include "ScriptViewerWorker.hpp"' not in pg:
        failures.append("src/ProductGlobals.cpp must include GameErrorContext.hpp and ScriptViewerWorker.hpp")
    if re.search(r"\bstruct\s+(GameErrorContext|ScriptViewerWorkerView)\s*\{", pg):
        failures.append("src/ProductGlobals.cpp contains a duplicate local struct GameErrorContext/ScriptViewerWorkerView")

    # 2. SupervisorLockSlot domain & consumer closure
    lock_hpp = (ROOT / "src" / "SupervisorLockSlot.hpp").read_text(errors="replace")
    for expected in ("SUPERVISOR_LOCK_CHAIN = 0", "SUPERVISOR_LOCK_LOG   = 3", "SUPERVISOR_LOCK_MODE  = 5"):
        if expected not in lock_hpp:
            failures.append(f"src/SupervisorLockSlot.hpp missing '{expected}'")
    if "SUPERVISOR_LOCK_FILE_IO" in lock_hpp:
        failures.append("src/SupervisorLockSlot.hpp must not define unverified SUPERVISOR_LOCK_FILE_IO")

    for rel in ("src/Chain.cpp", "src/ScriptViewer.cpp", "src/ReplayManager.cpp"):
        txt = (ROOT / rel).read_text(errors="replace")
        if "criticalSections[0]" in txt or "criticalSectionLockCounts[0]" in txt:
            failures.append(f"{rel} uses bare criticalSections[0] instead of SUPERVISOR_LOCK_CHAIN")

    gec = (ROOT / "src" / "GameErrorContext.cpp").read_text(errors="replace")
    if "g_CriticalSections[3]" in gec:
        failures.append("src/GameErrorContext.cpp uses bare g_CriticalSections[3] instead of SUPERVISOR_LOCK_LOG")

    # 3. ChainCallbackResult domain (0..7) & Chain.cpp switch closure (decimal + hex)
    chain_hpp = (ROOT / "src" / "Chain.hpp").read_text(errors="replace")
    for val in range(8):
        if not re.search(rf"CHAIN_CALLBACK_RESULT_\w+\s*=\s*{val}\b", chain_hpp):
            failures.append(f"src/Chain.hpp missing ChainCallbackResult value {val}")
    chain_cpp = (ROOT / "src" / "Chain.cpp").read_text(errors="replace")
    if re.search(r"\bcase\s+(?:0x[0-9A-Fa-f]+|[0-9]+)\s*:", chain_cpp):
        failures.append("src/Chain.cpp contains bare numeric (decimal/hex) switch case labels")

    # 4. SoundEffectId SE_COUNT = 37 & Sound.cpp closure
    sound_hpp = (ROOT / "src" / "Sound.hpp").read_text(errors="replace")
    if not re.search(r"\bSE_COUNT\s*=\s*37\b", sound_hpp):
        failures.append("src/Sound.hpp missing SE_COUNT = 37")
    sound_cpp = (ROOT / "src" / "Sound.cpp").read_text(errors="replace")
    if re.search(r"g_SoundFileNames\[37\]|i\s*<\s*37", sound_cpp):
        failures.append("src/Sound.cpp uses bare 37 instead of SE_COUNT")

    # 5. SupervisorGameMode sentinel check
    main_hpp = (ROOT / "src" / "Main.hpp").read_text(errors="replace")
    if not re.search(r"SUPERVISOR_MODE_UNINITIALIZED\s*=\s*-2\b", main_hpp):
        failures.append("src/Main.hpp missing SUPERVISOR_MODE_UNINITIALIZED = -2")

    # 6. Input mask check in Main.cpp
    main_cpp = (ROOT / "src" / "Main.cpp").read_text(errors="replace")
    if re.search(r"g_MainInputEdges\s*&\s*0x0*800\b", main_cpp):
        failures.append("src/Main.cpp uses bare 0x0800 instead of INPUT_BUTTON_SCREENSHOT")

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

    # 8. No local machine absolute links in docs/
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
