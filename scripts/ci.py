#!/usr/bin/env python3
"""Run the small target-independent validation used by public CI."""

from __future__ import annotations

import os
from pathlib import Path
import subprocess
import sys


ROOT = Path(__file__).resolve().parents[1]


def tracked(pattern: str) -> list[str]:
    output = subprocess.run(
        ["git", "ls-files", "--cached", "--others", "--exclude-standard", "-z", pattern],
        cwd=ROOT,
        check=True,
        stdout=subprocess.PIPE,
    ).stdout
    return [raw.decode() for raw in output.split(b"\0") if raw and (ROOT / raw.decode()).is_file()]


def run(label: str, command: list[str]) -> None:
    print(f"\n==> {label}", flush=True)
    environment = os.environ.copy()
    environment["PYTHONPYCACHEPREFIX"] = str(ROOT / "build" / "pycache")
    subprocess.run(command, cwd=ROOT, env=environment, check=True)


def validate_public_tree() -> None:
    forbidden = {".exe", ".dll", ".dat", ".gpr", ".i64", ".id0", ".id1", ".id2", ".nam", ".til"}
    for relative in tracked("*"):
        if Path(relative).suffix.lower() in forbidden:
            raise ValueError(f"private binary or database is trackable: {relative}")


def main() -> int:
    try:
        validate_public_tree()
        run("Compile workflow Python", [sys.executable, "-m", "py_compile", *tracked("scripts/*.py")])
        run("Validate LTCG entry identity guards", [sys.executable, "scripts/test-ltcg-entry.py"])
        run("Validate replay payload order", [sys.executable, "scripts/test-replay-payload-order.py"])
        run("Validate lexical HUD message phase", [sys.executable, "scripts/test-gui-message-phase.py"])
        run("Validate lexical HUD boss-gauge phase", [sys.executable, "scripts/test-gui-boss-gauge-phase.py"])
        run("Validate HUD boss-gauge selection", [sys.executable, "scripts/test-gui-boss-gauge-selector.py"])
        run("Validate HUD lifetimes and countdown sounds", [sys.executable, "scripts/test-gui-hud-lifetimes.py"])
        run("Validate automatic hint export", [sys.executable, "scripts/test-hint-export.py", "--skip-mutations"])
        run("Validate hint loading and parsing", [sys.executable, "scripts/test-hint-load.py", "--skip-mutations"])
        run("Validate hint frame contracts", [sys.executable, "scripts/test-hint-frame.py", "--skip-mutations"])
        run("Validate conditional hint arithmetic", [sys.executable, "scripts/test-hint-frame-arithmetic.py"])
        run("Validate hint lifecycle contracts", [sys.executable, "scripts/test-hint-lifecycle.py", "--skip-mutations"])
        run("Validate GUI reset event order", [sys.executable, "scripts/test-gui-reset-order.py"])
        run("Validate music-room state and parsers", [sys.executable, "scripts/test-music-room-state.py"])
        run("Validate stage script state and callback", [sys.executable, "scripts/test-stage-script-state.py"])
        run("Validate stage projected bounds and draw", [sys.executable, "scripts/test-stage-draw.py"])
        run("Validate stage camera setup", [sys.executable, "scripts/test-stage-camera.py"])
        run("Validate stage draw passes", [sys.executable, "scripts/test-stage-draw-passes.py"])
        run("Validate stage lifecycle ownership", [sys.executable, "scripts/test-stage-lifecycle.py"])
        run("Validate enemy callback scale reload", [sys.executable, "scripts/test-enemy-runtime-scale.py"])
        run("Validate enemy motion and interpolation", [sys.executable, "scripts/test-enemy-motion.py"])
        run("Validate enemy runtime tail", [sys.executable, "scripts/test-enemy-runtime-tail.py"])
        run("Validate item sound owner", [sys.executable, "scripts/test-item-sound-owner.py"])
        run("Validate player damage lifetimes", [sys.executable, "scripts/test-player-damage-lifetime.py"])
        run("Validate player damage prerequisites", [sys.executable, "scripts/test-player-damage-prerequisites.py"])
        run("Validate auxiliary player damage", [sys.executable, "scripts/test-player-auxiliary-damage.py"])
        run("Validate player shot update lifetimes", [sys.executable, "scripts/test-player-shot-update.py"])
        run("Validate player movement ANM identities", [sys.executable, "scripts/test-player-movement-anm.py"])
        run("Validate result/replay-name controller", [sys.executable, "scripts/test-result-menu-state.py"])
        run("Validate pause/capture controller", [sys.executable, "scripts/test-pause-menu-state.py"])
        run("Validate practice controller state", [sys.executable, "scripts/test-frontend-practice-state.py", "--skip-mutations"])
        run("Validate key-config display lifetimes", [sys.executable, "scripts/test-keyconfig-display-state.py"])
        run("Validate options-refresh lifetimes", [sys.executable, "scripts/test-options-refresh-state.py"])
        run("Validate options controller state", [sys.executable, "scripts/test-options-controller-state.py"])
        run("Validate options topology recognizer", [sys.executable, "scripts/test-options-owner-topology.py"])
        run("Validate options timer predicate", [sys.executable, "scripts/test-options-timer-predicate.py"])
        run("Validate supervisor binding identity", [sys.executable, "scripts/test-main-supervisor-bindings.py"])
        run("Validate score insertion and publication", [sys.executable, "scripts/test-score-insertion.py"])
        run("Validate Player power-state transitions", [sys.executable, "scripts/test-player-update-state.py"])
        run("Validate documentation references", [sys.executable, "scripts/check-doc-hygiene.py"])
        run("Validate ledgers", [sys.executable, "scripts/validate-tracking.py"])
        run(
            "Validate ANM/ECL core source completeness",
            [
                sys.executable,
                "scripts/report-source-completeness.py",
                "--module",
                "AnmManager",
                "--module",
                "EclVm",
                "--module",
                "Enemy",
                "--require-complete",
            ],
        )
        run("Validate toolchain declarations", [sys.executable, "scripts/verify-toolchain.py", "--check"])
        run("Validate canonical match graph", [sys.executable, "scripts/build-match-unit.py", "--check"])
        run("Validate open whole-build graph", [sys.executable, "scripts/build.py", "--check"])
        run("Check generated progress", [sys.executable, "scripts/progress.py", "--check"])
        run("Check whitespace", ["git", "diff", "--check"])
    except (OSError, ValueError, subprocess.CalledProcessError) as exc:
        print(f"error: CI validation failed: {exc}", file=sys.stderr)
        return 1
    print("\nTH10 target-independent checks passed")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
