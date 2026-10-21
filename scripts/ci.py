#!/usr/bin/env python3
"""Run the small target-independent validation used by public CI."""

from __future__ import annotations

import os
from pathlib import Path
import subprocess
import sys
import tempfile


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
        run("Validate qualified static owners", [sys.executable, "scripts/test-static-owner-selection.py"])
        run("Validate linked profiles and static callees", [sys.executable, "scripts/test-linked-context.py"])
        run("Validate internal linked destinations", [sys.executable, "scripts/test-linked-local-fields.py"])
        run("Validate linked table and selector coverage", [sys.executable, "scripts/test-linked-data-coverage.py"])
        run("Validate allocation EH address binding", [sys.executable, "scripts/test-linked-allocation-eh.py"])
        run("Validate replay payload order", [sys.executable, "scripts/test-replay-payload-order.py"])
        run("Validate Enemy dispatch case scopes", [sys.executable, "scripts/test-ecl-dispatch-table.py"])
        run("Validate ECL FORMAT argument consumption", [sys.executable, "scripts/test-ecl-format.py", "--sanitize"])
        run("Validate lexical HUD message phase", [sys.executable, "scripts/test-gui-message-phase.py"])
        run("Validate lexical HUD boss-gauge phase", [sys.executable, "scripts/test-gui-boss-gauge-phase.py"])
        run("Validate HUD boss-gauge selection", [sys.executable, "scripts/test-gui-boss-gauge-selector.py"])
        run("Validate HUD lifetimes and countdown sounds", [sys.executable, "scripts/test-gui-hud-lifetimes.py"])
        run("Validate automatic hint export", [sys.executable, "scripts/test-hint-export.py", "--skip-mutations"])
        run("Validate hint loading and parsing", [sys.executable, "scripts/test-hint-load.py", "--skip-mutations"])
        run("Validate hint frame contracts", [sys.executable, "scripts/test-hint-frame.py", "--skip-mutations"])
        run("Validate conditional hint arithmetic", [sys.executable, "scripts/test-hint-frame-arithmetic.py"])
        run("Validate hint lifecycle contracts", [sys.executable, "scripts/test-hint-lifecycle.py", "--skip-mutations"])
        run("Validate GameManager cleanup contract", [sys.executable, "scripts/test-game-manager-teardown.py", "--baseline-only"])
        run("Validate statistics save contract", [sys.executable, "scripts/test-game-statistics-save.py", "--baseline-only"])
        run("Validate GUI lifecycle contract", [sys.executable, "scripts/test-gui-lifecycle.py", "--baseline-only"])
        run("Validate GUI message construction and unwind contracts", [sys.executable, "scripts/test-gui-message-constructor.py", "--no-mutations"])
        run("Validate GUI message Run baseline", [sys.executable, "scripts/test-gui-message-run.py", "--no-mutations", "--out", "build/host-tests/gui-message-run-ci"])
        with tempfile.TemporaryDirectory(prefix="gui-enemy-name-ci-") as output:
            run("Validate enemy-name dispatch controls", [sys.executable, "scripts/test-gui-enemy-name.py", "--out", output])
        with tempfile.TemporaryDirectory(prefix="gui-replay-mode-ci-") as output:
            run("Validate canonical replay mode controls", [sys.executable, "scripts/test-gui-replay-mode.py", "--root", str(ROOT), "--out", str(Path(output) / "run")])
        with tempfile.TemporaryDirectory(prefix="gui-enemy-portrait-sprites-ci-") as output:
            run("Validate enemy portrait sprite controls", [sys.executable, "scripts/test-gui-enemy-portrait-sprites.py", "--root", str(ROOT), "--out", str(Path(output) / "run")])
        with tempfile.TemporaryDirectory(prefix="gui-stage-complete20-ci-") as output:
            run("Validate stage complete20 controls", [sys.executable, "scripts/test-gui-stage-complete20.py", "--root", str(ROOT), "--out", str(Path(output) / "run")])
        with tempfile.TemporaryDirectory(prefix="gui-stage-event18-ci-") as output:
            run("Validate stage event18 controls", [sys.executable, "scripts/test-gui-stage-event18.py", "--root", str(ROOT), "--out", str(Path(output) / "run")])
        with tempfile.TemporaryDirectory(prefix="gui-direct-creation-ci-") as output:
            run("Validate direct creation caller controls", [sys.executable, "scripts/test-gui-direct-creation.py", "--root", str(ROOT), "--out", str(Path(output) / "run")])
        with tempfile.TemporaryDirectory(prefix="gui-portrait-creation-ci-") as output:
            run("Validate portrait creation controls", [sys.executable, "scripts/test-gui-portrait-creation.py", "--out", str(Path(output) / "run")])
        run("Validate GUI message construction-Run-destruction baseline", [sys.executable, "scripts/test-gui-message-lifecycle.py", "--no-mutations", "--out", "build/host-tests/gui-message-lifecycle-ci"])
        run("Validate GUI draw contracts", [sys.executable, "scripts/test-gui-draw.py"])
        run("Validate laser owner bindings", [sys.executable, "scripts/test-laser-owner-binding.py", "--opt", "O2", "--no-mutations"])
        run("Validate rectangle contracts and strict mutations", [sys.executable, "scripts/test-anm-rectangle.py"])
        run("Validate alpha interpolation and strict mutations", [sys.executable, "scripts/test-anm-alpha-interpolation.py"])
        run("Validate RGB interpolation and strict mutations", [sys.executable, "scripts/test-anm-rgb-interpolation.py"])
        run("Validate loaded-ANM slot release", [sys.executable, "scripts/test-anm-release-slot.py"])
        run("Validate GUI draw arithmetic witnesses", [sys.executable, "scripts/test-gui-draw-arithmetic.py"])
        run("Validate GUI draw complete-header layout", [sys.executable, "scripts/test-gui-draw-layout.py"])
        run("Validate GUI reset event order", [sys.executable, "scripts/test-gui-reset-order.py"])
        run("Validate music-room state and parsers", [sys.executable, "scripts/test-music-room-state.py"])
        run("Validate stage script state and callback", [sys.executable, "scripts/test-stage-script-state.py"])
        run("Validate stage projected bounds and draw", [sys.executable, "scripts/test-stage-draw.py"])
        run("Validate stage camera setup", [sys.executable, "scripts/test-stage-camera.py"])
        run("Validate ASCII viewport transitions", [sys.executable, "scripts/test-ascii-viewport.py", "--sanitize"])
        run("Validate ASCII glyph paths", [sys.executable, "scripts/test-ascii-glyphs.py", "--sanitize"])
        run("Validate stage draw passes", [sys.executable, "scripts/test-stage-draw-passes.py"])
        run("Validate stage lifecycle ownership", [sys.executable, "scripts/test-stage-lifecycle.py"])
        run("Validate enemy callback scale reload", [sys.executable, "scripts/test-enemy-runtime-scale.py"])
        run("Validate enemy motion and interpolation", [sys.executable, "scripts/test-enemy-motion.py"])
        run("Validate conditional enemy polar storage", [sys.executable, "scripts/check-enemy-polar-rounding-model.py"])
        run("Validate enemy runtime tail", [sys.executable, "scripts/test-enemy-runtime-tail.py"])
        run("Validate item sound owner", [sys.executable, "scripts/test-item-sound-owner.py"])
        run("Validate enemy item scatter numeric sequence", [sys.executable, "scripts/test-enemy-item-scatter.py"])
        run("Validate enemy death service order and live storage", [sys.executable, "scripts/test-enemy-finalize-death.py"])
        run("Validate bullet cancel transition and timer order", [sys.executable, "scripts/test-enemy-cancel-record.py"])
        run("Validate player damage lifetimes", [sys.executable, "scripts/test-player-damage-lifetime.py"])
        run("Validate player damage prerequisites", [sys.executable, "scripts/test-player-damage-prerequisites.py"])
        run("Validate auxiliary player damage", [sys.executable, "scripts/test-player-auxiliary-damage.py"])
        run("Validate player shot update lifetimes", [sys.executable, "scripts/test-player-shot-update.py"])
        run("Validate player movement ANM identities", [sys.executable, "scripts/test-player-movement-anm.py"])
        run("Validate result/replay-name controller", [sys.executable, "scripts/test-result-menu-state.py"])
        run("Validate front-end replay save", [sys.executable, "scripts/test-frontend-replay-save.py", "--no-mutations"])
        run("Validate title and main-menu controls", [sys.executable, "scripts/test-frontend-main-menu.py", "--no-mutations"])
        run("Validate MIDI event and timer controls", [sys.executable, "scripts/test-midi-events.py", "--no-mutations"])
        run("Validate spell-panel state and sprite binding", [sys.executable, "scripts/test-spell-panel.py", "--no-mutations"])
        run("Validate result rows and name-entry ASCII queue", [sys.executable, "scripts/test-result-rows.py", "--no-mutations"])
        run("Validate laser motion and Player contact", [sys.executable, "scripts/test-laser-line.py", "--no-mutations"])
        run("Validate laser command programs and live fields", [sys.executable, "scripts/test-laser-script.py", "--no-mutations"])
        run("Validate laser initialization and turning motion", [sys.executable, "scripts/test-laser-init.py", "--no-mutations"])
        run("Validate laser clearing, VM lists and item spawning", [sys.executable, "scripts/test-laser-clear.py", "--no-mutations"])
        run("Validate type1 laser phases and runtime interfaces", [sys.executable, "scripts/test-laser-type1.py", "--no-mutations"])
        run("Validate type1 laser clearing and offscreen counts", [sys.executable, "scripts/test-laser-type1-clear.py", "--no-mutations"])
        run("Validate input polling, device failures and gameplay repeat", [sys.executable, "scripts/test-input-polling.py", "--no-mutations"])
        run("Validate script browser state and event order", [sys.executable, "scripts/test-script-viewer-update.py"])
        run("Validate script browser display and renderer calls", [sys.executable, "scripts/test-script-viewer-draw.py"])
        run("Validate worker wait and restart order", [sys.executable, "scripts/test-script-viewer-worker.py"])
        run("Validate Ending owner and script state", [sys.executable, "scripts/test-ending-state.py", "--no-mutations"])
        run("Validate browser and manager destruction order", [sys.executable, "scripts/test-script-viewer-lifecycle.py"])
        run("Validate manager pool reset order", [sys.executable, "scripts/test-manager-pool-reset.py"])
        run("Validate debug input repeat edges", [sys.executable, "scripts/test-input-debug-repeat.py"])
        run("Validate result-transition and scaled-sound baseline", [sys.executable, "scripts/test-result-transition.py"])
        run("Validate transition-effect logical baseline", [sys.executable, "scripts/transition_effect_host_test.py", "--out", "build/host-tests/transition-effect-ci"])
        run("Validate transition-effect pin portability", [sys.executable, "scripts/transition_effect_eol_test.py", "--out", "build/host-tests/transition-effect-eol-ci"])
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
