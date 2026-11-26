# TH10 private inputs and retained artifacts

[RE_HANDOFF.md](RE_HANDOFF.md) identifies the next owner and its current
source/artifact distinction. The checkout supplies source, recipes and ledgers;
the following inputs remain private and local.

## Protected inputs

- Keep the read-only Japanese TH10 v1.00a executable at `resources/th10.exe`.
  Run `scripts/repo-python scripts/verify-target.py` before target work.
- Keep `.tools/`, this game's Wine prefix, `ghidra-project/`, analyzer runtime
  state and `_reference/`. See [SHARED_TOOLS.md](SHARED_TOOLS.md).
- Keep `build/match-linked/`, `build/match/` and `build/probe-backlog/`.
  The linked comparator needs the corresponding PE, MAP and PDB; support
  objects are useful for probes in the same graph. Find each owner's link
  through [match-units.toml](../config/match-units.toml).
- Keep D3D adapter evidence, `docs/evidence/`, origin-review inputs and the
  MIDI/statistics diagnostic contexts. The current cleanup leaves these intact.

## Current unresolved context

`.analysis/ecl-start-subroutine/` retains the 542/550 cold baseline report,
build log and exit codes, plus reports/logs from the two latest byte-neutral
controls. Three completed read-only trace families retain their PE/MAP/PDB,
JSONL observations and debugger scripts/logs:

- `terra-float-costs2`: float-word register costs and preferences.
- `terra-latch-tie-write`: latch candidate ordering; the write watchpoint
  observes the compiler's counter assignment.
- `terra-latch-count-scores`: the older volatile-count control's allocation.

The corresponding launchers remain alongside them. They depend on
`_reference/th10-decomp/matching2/reference/compiler_traces/regalloc/trace_temporary_cursor.py`.
Use a fresh label and current link inputs for new observations; saved debugger
scripts contain historical proxy ports. Keep the warm relink helper at
`.analysis/update-replay/relink.py`. Rebuild the restored source before using
the live `EclVmHostRunEnemy` objects for another probe.

The small reusable ABI probe remains at
`.analysis/enemy-full-destructor/compiler-abi/run.py`. Other accepted-owner
receipts and useful diagnostics remain in their existing `.analysis/` folders;
their presence does not make an older source epoch current.

## Dirty worktrees and reclaimed storage

Four historical worktrees contain uncommitted changes and remain intact:

- `.analysis/bullet-transform/pending-abi.ptm2BF/tree`
- `.analysis/ecl-run-accept.Ic4vb6/tree`
- `.analysis/start-spell-accept.b8K9HV/tree`
- `.analysis/start-spell/draw-varargs-tree`

The handoff cleanup removed superseded ECL source variants, generators and
debug images, plus obsolete Enemy destructor trial builds and snapshots.
This reclaimed about 47 MiB. Git object repacking saved another 153 MiB
without pruning objects, refs or reflogs: about 200 MiB total.
The ECL context now occupies about 1.2 MiB.

Before further cleanup, inspect `git status` and `git worktree list`. Preserve
dirty trees and active comparison inputs. Record useful findings in the
knowledge base, then remove superseded experiments without replacement
archives; Git retains tracked history.
