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
- GUI cleanup cold and direct-caller receipts are under
  `.analysis/gui-cleanup-resources/`.
- The Bomb power transition replay and direct-caller comparisons are under
  `.analysis/player-start-power-transition/`. Its canonical link is
  `build/match-linked/PlayerStartPowerTransition/`.

The absolute-direction owner retains its canonical comparison and selected
cold replay under `.analysis/bullet-absolute/`, with the live link in
`build/match-linked/BulletHorizontalWrap/`. Removing its superseded source
trials, debugger contexts and experiment links reclaimed about 53 MiB.

The surface-copy owners retain their canonical comparisons and selected cold
replays under `.analysis/anm-copy-backbuffer/` and
`.analysis/anm-copy-backbuffer2/`, in the same `BulletHorizontalWrap` link.
Their superseded warm trials and source snapshots reclaimed about 2 MiB.

The Player destructor retains its canonical comparison, selected consumer
replays and focused EH controls under `.analysis/player-destructor/`.
Its declared live link is `build/match-linked/ScriptViewerDestructSmall/`;
superseded source snapshots and warm probe links have been removed.

The score insertion owner retains its complete baseline, canonical comparison
and selected cold replay under `.analysis/frontend-insert-score/`. Its live
links are `ResultMenuClose` and the score-entry caller's `FrontEndReplay`.
Superseded disassembly and diagnostic helper files have been removed.

The type-1 laser initializer retains its initial build log, canonical comparison
and independent cold replay under `.analysis/laser-type1-initialize/`. Keep
`build/match-linked/LaserType1InitCore/`; the temporary decoder output and
one-off diagnostic helper have been removed.

The GUI stage-data loader retains its initial diagnosis, complete strict
comparison, selected cold replay and one useful read-only C2 allocation trace
under `.analysis/gui-load-data/`. Keep its live `GuiCreation-cold-20261025`
and `GameStartupRestore` links. Superseded source snapshots, warm dumps,
private relink helpers and debugger-link images have been removed (about
2.2 MiB).

The main-menu entry owner retains its baseline, complete strict comparison
and dispatcher cold replay under `.analysis/frontend-main-menu/`, with the
live graph in `build/match-linked/FrontEndReplay/`. Superseded warm dumps
and the private relink helper have been removed.

The default camera owner retains its baseline, first complete strict comparison,
selected cold replay and useful read-only stack-colour trace under
`.analysis/main-configure-camera/`. Its live links are `StageViewport`,
`AnmDrawCallbacks` and `ChainLifetime`. The trace describes the preceding
six-displacement candidate at commit `201b2dfa`; Git retains that source.
Superseded trial dumps, source snapshots, warm helpers and the debugger image
have been removed (about 0.3 MiB).

The Bomb power transition cleanup reclaimed about 26 MiB in source
snapshots, trial links and superseded GUI and text-rendering outputs.

Check `git status` and `git worktree list` before removing a trial. Keep
uncommitted worktrees and live comparison inputs. Use regular worktree
removal for clean snapshots. Git retains tracked history.
