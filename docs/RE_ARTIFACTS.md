# TH10 private inputs and storage handoff

**Updated 2026-11-13.** A Git clone has source and canonical manifests, but
not the original Japanese executable, private toolchain/analysis setup, or all
temporary native compiler products. Preserve the following local inputs.

## Protected — do not delete

- `resources/th10.exe`: original Japanese TH10 v1.00a, SHA-256
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
  Check with `scripts/repo-python scripts/verify-target.py`.
- `.tools/`, including `.tools/wineprefix-msvc710-sp1/`, and the pinned
  VC7.1 compiler/linker setup; `ghidra-project/` and `_reference/`.
- All existing `build/match-linked/` native comparison contexts, including
  PDB/MAP/PE files and support objects. An absent unit output is rebuilt from
  its canonical `config/match-units.toml` recipe.
- Maintained `src/`, `scripts/`, `config/`, Git and build-input files, including
  `docs/evidence/anm-loaded-start-script-wave11-20261109/compiler-context.cpp`
  and `docs/evidence/anm-child-request-record/binding-contract.json`.
- Unrelated `.analysis/d3d-*` work and associated private evidence.

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

## 2026-11-13 storage cleanup

Removed superseded `build/gpt-web/20261112/` and `build/gpt-web/20261113/`
outputs, plus all `.analysis/gpt-web/` scratch except the four drivers above.
This removed **2,220 build files** and **545 scratch files**, approximately
**118.6 MB (113.1 MiB)**. The five retained compiler contexts occupy about
**4.5 MiB**; the four trial drivers are about **20 KB of source** (roughly
**120 KiB allocated**). The existing `build/match-linked/` contexts (about
**142 MiB**) were preserved intact.

Dated paths in `KNOWLEDGE_BASE.md` are historical experiment references, not
live artifact dependencies. Rebuild only the affected graph for a new
hypothesis; never recreate a full historical cache or replay cohort.
