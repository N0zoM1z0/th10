# TH10 private inputs and retained artifacts

[RE_HANDOFF.md](RE_HANDOFF.md) identifies the current comparison lanes.
A Git checkout supplies the source, recipes and ledgers. The original
executable and local compiler and analyzer state are private inputs.

- Keep the read-only Japanese TH10 v1.00a executable at
  `resources/th10.exe`. Verify the target with
  `scripts/repo-python scripts/verify-target.py` before target-dependent work.
- Keep `.tools/`, including the TH10 Wine prefix and shared runtime links,
  `ghidra-project/`, the game's analyzer runtime state and `_reference/`.
  See [SHARED_TOOLS.md](SHARED_TOOLS.md) for runtime setup.
- Keep `build/match-linked/` (about 199 MiB), `build/match/` and
  `build/probe-backlog/`. The linked comparator uses the PE, map and PDB
  together. The support objects let the next probe reuse the reviewed
  compiler graph.
- Keep `docs/evidence/`, private D3D evidence, original origin-review
  inputs and the working notes, selected traces and source probes under
  `.analysis/`. The open ANM, ECL, MIDI, music room, spell and statistics
  lanes retain their selected compiler contexts under `build/`.
  The stage loader context retains the real-initializer comparison.

The 2026-11-23 handoff cleanup reclaimed about 88 MiB from `.analysis/` and
`build/`. The exact Result menu, GUI reset, Item update and score-display
owners now use their canonical outputs. Superseded candidate links,
duplicate trial source and generated comparison reports were removed after
the relevant findings were recorded. The retained working directories use
about 21 MiB for `.analysis/` and 206 MiB for `build/`.

Check the worktree and manifest before deleting a trial. Keep the source,
objects and comparison inputs needed by current probes. Git keeps tracked
history. Avoid replacement archives, reflog expiry and Git GC during routine
artifact cleanup.
