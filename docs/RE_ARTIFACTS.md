# TH10 private inputs and storage handoff

**Updated 2026-11-16.** A Git clone has source and canonical manifests, but
not the original Japanese executable, private toolchain/analysis setup, or all
temporary native compiler products. Preserve the following local inputs.

## Protected — do not delete

- `resources/th10.exe`: original Japanese TH10 v1.00a, SHA-256
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
  Check with `scripts/repo-python scripts/verify-target.py`.
- `.tools/`, including `.tools/wineprefix-msvc710-sp1/`, and the pinned
  VC7.1 compiler/linker setup; `ghidra-project/` and `_reference/`.
- Canonical manifest-backed `build/match-linked/` native comparison contexts,
  including PDB/MAP/PE files and support objects. An absent unit output is
  rebuilt from its `config/match-units.toml` recipe.
- `build/match/*.obj`: support COFF objects explicitly referenced by object-backed
  `config/match-units.toml` recipes. These are build inputs, not disposable cache.
- Maintained `src/`, `scripts/`, `config/`, Git and build-input files, including
  `docs/evidence/anm-loaded-start-script-wave11-20261109/compiler-context.cpp`
  and `docs/evidence/anm-child-request-record/binding-contract.json`.
- Unrelated `.analysis/d3d-*` work and associated private evidence.
- The independent target-hash-pinned origin review at
  `.analysis/gpt-5.6-sol/20260917-origin-review/runtime-origin-review.json`
  (334 CRT matches and 961 resolved runtime references).

## Five retained noncanonical diagnostic contexts

Keep exactly these narrow contexts for unresolved behavior:

| Owner | Directory under `build/gpt-web/` |
| --- | --- |
| ANM `ExecuteScript`, opcode 91 | `20261109/anm-execute-child91-aggregate/` |
| ECL `Run`, x87 stack ordering | `20261109/ecl-run-length-pointer-order/pointer-y-first/` |
| Enemy runtime/motion | `20261108-enemy-runtime-aggregate-sum/` |
| Enemy ECL dispatch | `20261108-enemy-dispatch-rank-tail-final/` |
| Player HUD loader | `20261109/playerhud-restored-natural-loader/` |

Four optional trial drivers remain under `.analysis/gpt-web/`:

- `20261110-anm-child91-aggregate-initializers.py`
- `20261110-ecl-format-cursor-allocation-schedule.py`
- `20261110-bullet-absolute-speed-liveness.py`
- `20261110-seek-tail-control-flow-trials.py`

These are small diagnostics for current open owners, not canonical exact
receipts. Other source snapshots, per-variant linked images, logs and scratch
results can be recreated from current source and the unit manifest.

## Scratch policy

Session scratch belongs under `.analysis/` or an agent-owned subdirectory
and is deleted once its findings are recorded in `KNOWLEDGE_BASE.md` or
`RE_HANDOFF.md`. `build/probe-ltcg/` and `build/probe-backlog/` are generated
probe outputs; recreate them with their scripts when needed. Preserve the
manifest-backed `build/match-linked/` contexts and object-backed `build/match/`
inputs.

2026-11-16 cleanup: removed the superseded `.analysis/gpt-web/` experiment
reports, candidate images, object snapshots, patches and temporary relink
helper (about 54 MB), retaining only the four trial drivers listed above.
Removed the generated `build/probe-ltcg/` directory (about 2.5 MB) and the
failed, unreferenced `build/gpt-web/enemy-callback-pointer-zero/` probe.
The five diagnostic contexts above, canonical `build/match-linked/` outputs,
`build/match/` objects, D3D evidence and Factory runtime state were preserved.

2026-11-16 second-pass cleanup: removed 397 superseded top-level `.analysis/`
scratch files (logs, frontier snapshots, transient JSON/patches, decompiler
text and one-off helper scripts), about 1.03 MB. Kept the active Ghidra lock,
the four current `.analysis/gpt-web/` trial drivers, the independent origin
review, D3D evidence and Factory runtime. Removed the generated
`caller.obj`/`caller.normal.obj` from `build/anm-caller-abi-test/` while keeping
its small `caller.cpp` ABI probe source. The five retained `build/gpt-web/`
contexts and canonical build caches were not touched.
