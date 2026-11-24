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
- Keep `docs/evidence/`, private D3D evidence, origin-review inputs and
  working files for open owners. The music-room, ECL, ANM, spell-start and
  stage-loader owners use their canonical comparison recipes. The MIDI
  loader and statistics initializer retain their open diagnostic contexts.

The November 24 handoff cleanup reclaimed about 316 MiB of local disk space.
It removed superseded trial builds and duplicate snapshots for closed owners.
The dispatcher artifact from the `58e9e8eb` replay moved from a clean temporary
worktree to `build/match-linked/EnemyEclDispatcher/`. Its compiler inputs belong
to that checkpoint. Rebuild the selected unit to verify later source changes.

Check `git status` and `git worktree list` before removing a trial. Preserve
uncommitted worktrees and any live comparison inputs. Git retains tracked
history. Use regular worktree removal for clean snapshots and leave Git
garbage collection outside routine artifact cleanup.
