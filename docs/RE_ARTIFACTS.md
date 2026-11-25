# TH10 private inputs and retained artifacts

[RE_HANDOFF.md](RE_HANDOFF.md) identifies the comparison lanes. The checkout
supplies the source, recipes and ledgers. Keep these private inputs locally.

- Keep the original Japanese TH10 v1.00a executable at the read-only path
  `resources/th10.exe`. Verify it with
  `scripts/repo-python scripts/verify-target.py` before target-dependent work.
- Keep `.tools/`, the TH10 Wine prefix, the Ghidra project and runtime state,
  and `_reference/`. [SHARED_TOOLS.md](SHARED_TOOLS.md) describes the runtime
  setup.
- Keep `build/match-linked/`, `build/match/` and `build/probe-backlog/`.
  The linked comparator needs the matching PE, map and PDB. Support objects
  are useful for warm probes in the same graph.
- Keep `docs/evidence/`, private D3D evidence, origin-review inputs, and
  the current MIDI and statistics diagnostic contexts. Local receipts for
  the GUI destructor, GUI reset and replay stage snapshot are under
  `.analysis/gui-destructor/`, `.analysis/game-reset-gui/` and
  `.analysis/replay-begin-stage/`. Each can be reproduced from its
  manifest recipe.
- For `StartSubroutine`, keep `.analysis/ecl-start-subroutine/` and the
  warm relink helper at `.analysis/update-replay/relink.py`. The ECL
  directory contains the baseline instruction diff, target listing and
  trial generators. Run a `make-*.py` generator from the repository root
  to recreate its source variants. The current link is
  `build/match-linked/EclVmHostRunEnemy/`.
- The current ASCII GUI draw receipt and same-image guards are under
  `.analysis/ascii-gui-draw/`. Their canonical link is
  `build/match-linked/StageViewport/`.
- The popup Draw cold receipt is under `.analysis/script-viewer-popup-draw/`.
  Its canonical link is `build/match-linked/ScriptViewerPopupCreate/`.
  Completed popup trials reclaimed about 3.5 MiB.

The November 25 handoff reclaimed about 126 MiB from `.analysis/` and
`build/`. The latest pass removed about 10 MiB, including the obsolete
`AsciiGuiDraw` link and reproducible source variants. Four worktrees
under `.analysis/` carry uncommitted work. Use `git worktree list` to
locate them before touching their parent directories.

Check `git status` and `git worktree list` before removing a trial. Keep
uncommitted worktrees and live comparison inputs. Use regular worktree
removal for clean snapshots. Git retains tracked history.
