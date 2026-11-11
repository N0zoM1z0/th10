# TH10 retained private inputs and reclaimable storage

**Updated 2026-11-11.** This describes what a successor must preserve when
moving the reconstruction to another environment. A Git clone alone does
**not** contain the original executable or all private compiler contexts.

## Keep — do not prune

- The original ignored `resources/th10.exe` (Japanese v1.00a), its SHA-256
  verified through `scripts/verify-target.py`, and legitimate resource inputs.
- `.tools/` and `.tools/wineprefix-msvc710-sp1/`, the pinned VC7.1 SP1
  compiler/linker environment. The Wine prefix is the largest retained
  private asset; deleting it would require reinstalling/reinitializing tools.
- `ghidra-project/`, `_reference/`, source, scripts, canonical ledgers and Git.
- **All** of `build/match-linked/` — current canonical native comparison graphs,
  PE/MAP/PDB files and support objects used by scoped cold reconstruction.
- Unrelated D3D adapter analysis and retained inputs, including their private
  `.analysis/d3d-*` directories.
- Actual build inputs stored under `docs/evidence/`:
  `anm-loaded-start-script-wave11-20261109/compiler-context.cpp` and
  `anm-child-request-record/binding-contract.json`. These are **not**
  disposable just because their path contains `evidence`.

## Kept unresolved contexts

Only five hand-selected private diagnostic contexts remain under
`build/gpt-web/`:

| Problem | Retained directory |
| --- | --- |
| ANM `ExecuteScript`, opcode 91 | `20261109/anm-execute-child91-aggregate/` |
| ECL `Run`, floating-point/FORMAT ordering | `20261109/ecl-run-length-pointer-order/pointer-y-first/` |
| Enemy runtime/motion | `20261108-enemy-runtime-aggregate-sum/` |
| Enemy ECL dispatcher | `20261108-enemy-dispatch-rank-tail-final/` |
| Player HUD loader | `20261109/playerhud-restored-natural-loader/` |

Four focused, small Python trial drivers remain in `.analysis/gpt-web/`:
`20261110-anm-child91-aggregate-initializers.py`,
`20261110-ecl-format-cursor-allocation-schedule.py`,
`20261110-bullet-absolute-speed-liveness.py`, and
`20261110-seek-tail-control-flow-trials.py`.

The accepted 134-byte `bullet-manager-draw-bucket` and 124-byte
`enemy-read-dialog` functions have canonical replay recipes in
`config/match-units.toml`. Their deleted date-stamped trial outputs are
**not** required for acceptance; rebuild only the chosen unit using
`scripts/repo-python scripts/replay-exact-units.py --unit UNIT --json`.

## Storage cleanup and future policy

On **2026-11-11**, removed superseded generated compiler images, objects,
PDB/MAPs, source duplicates and logs from `build/gpt-web/` plus historical
`.analysis/gpt-web/` scratch snapshots. **306,678,113 bytes (about
292.47 MiB) of file content** were reclaimed. The five contexts and four
scripts above remain (about 4.24 MiB together). The total repository
working directory measured approximately 836 MiB afterward, mostly protected
Wine/compiler inputs and canonical comparison builds. A prior 2026-11-10
cleanup had already reclaimed about 515 MiB from other obsolete archives.

Old paths mentioned by dated knowledge notes may therefore no longer exist;
those descriptions are historical source-epoch observations, not current
live file links. Reconstruct an experiment using the tracked source,
canonical unit manifest and pinned toolchain if its hypothesis becomes useful
again. Do not rebuild all historical units simply to replace deleted caches.
Do not create new archives, per-file deletion manifests or repeated cleanup
reports. Preserve dirty or unrelated work before any future pruning.
