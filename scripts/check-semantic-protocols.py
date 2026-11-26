#!/usr/bin/env python3
"""Lightweight regression guard for completed TH10 semantic protocols.

Enforces only completed protocol invariants so semantic readability does not
silently regress back to bare numbers or duplicate ODR definitions:
1. src/ProductGlobals.cpp uses canonical GameErrorContext.hpp and
   ScriptViewerWorker.hpp (no local duplicate struct definitions).
2. src/Chain.cpp MainRunCalcChain / MainRunDrawChain switches use
   th10::ChainCallbackResult (no bare numeric case labels) and
   SUPERVISOR_LOCK_CHAIN (no bare criticalSections[0]).
3. src/Sound.cpp uses SE_COUNT (no bare 37 in g_SoundFileNames or loops).
4. src/th10.rc and src/Main.cpp both share src/resource.h for Dialog 203 IDs.
5. docs/*.md contain no local machine file:///home/ links.
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

    # 2. Chain.cpp protocol closure
    chain = (ROOT / "src" / "Chain.cpp").read_text(errors="replace")
    if re.search(r"\bcase\s+[0-9]+\s*:", chain):
        failures.append("src/Chain.cpp contains bare numeric switch case labels instead of th10::ChainCallbackResult")
    if "g_MainSupervisorView.criticalSections[0]" in chain or "g_MainSupervisorView.criticalSectionLockCounts[0]" in chain:
        failures.append("src/Chain.cpp uses bare criticalSections[0] instead of SUPERVISOR_LOCK_CHAIN")

    # 3. Sound.cpp SE_COUNT closure
    sound = (ROOT / "src" / "Sound.cpp").read_text(errors="replace")
    if re.search(r"g_SoundFileNames\[37\]|i\s*<\s*37", sound):
        failures.append("src/Sound.cpp uses bare 37 instead of SE_COUNT")

    # 4. Shared Win32 dialog resource IDs
    rc = (ROOT / "src" / "th10.rc").read_text(errors="replace")
    main_cpp = (ROOT / "src" / "Main.cpp").read_text(errors="replace")
    if '#include "resource.h"' not in rc or '#include "resource.h"' not in main_cpp:
        failures.append("both src/th10.rc and src/Main.cpp must include resource.h")

    # 5. No local machine absolute links in docs/
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
