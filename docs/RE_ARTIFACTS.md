# TH10 private inputs and storage handoff

**Updated 2026-11-12.** A Git clone has source and canonical manifests, but
not the original Japanese executable, private toolchain/analysis setup, or
all temporary native compiler products. Keep the following local inputs.

## Protected — do not delete

- `resources/th10.exe`: original Japanese TH10 v1.00a, SHA-256
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
  Check with `scripts/repo-python scripts/verify-target.py`.
- `.tools/`, including `.tools/wineprefix-msvc710-sp1/`, and the pinned
  VC7.1 compiler/linker setup; `ghidra-project/` and `_reference/`.
- All **existing** `build/match-linked/` native comparison contexts,
  including PDB/MAP/PE files and support objects. An absent individual unit
  output is rebuilt by its canonical manifest recipe; do not assume every
  possible `build/match-linked/<unit>/source.exe` exists already.
- All maintained `src/`, `scripts/`, `config/`, Git, and the build-input
  files `docs/evidence/anm-loaded-start-script-wave11-20261109/compiler-context.cpp`
  and `docs/evidence/anm-child-request-record/binding-contract.json`.
- Unrelated `.analysis/d3d-*` work and associated private evidence.

## Five retained noncanonical diagnostic contexts

Under `build/gpt-web/`, retain exactly these narrow contexts for currently
unresolved function behavior:

| Owner | Directory under `build/gpt-web/` |
| --- | --- |
| ANM `ExecuteScript`, opcode 91 | `20261109/anm-execute-child91-aggregate/` |
| ECL `Run`, x87 stack ordering | `20261109/ecl-run-length-pointer-order/pointer-y-first/` |
| Enemy runtime/motion | `20261108-enemy-runtime-aggregate-sum/` |
| Enemy ECL dispatch | `20261108-enemy-dispatch-rank-tail-final/` |
| Player HUD loader | `20261109/playerhud-restored-natural-loader/` |

Four small reproducible trial drivers remain under `.analysis/gpt-web/`:
`20261110-anm-child91-aggregate-initializers.py`,
`20261110-ecl-format-cursor-allocation-schedule.py`,
`20261110-bullet-absolute-speed-liveness.py`, and
`20261110-seek-tail-control-flow-trials.py`.
These are optional diagnostics, not canonical exact evidence.

## Reclaimed storage

On **2026-11-12**, cleaned only superseded **ignored** outputs from
`build/gpt-web/` and `.analysis/gpt-web/`, preserving the five contexts and
four drivers above. The deletion comprised **8,181 files / 377,422,607 bytes
(359.94 MiB)**: 275.53 MiB of old linked images, objects, PDB/MAP files and
logs, and 84.41 MiB of duplicate trial source snapshots/results.
The retained `build/gpt-web/` contents use roughly 4.5 MiB on disk;
`.analysis/gpt-web/` about 120 KiB. The existing `build/match-linked/`
contexts remain roughly 142 MiB on disk. Earlier cleanup history is in Git.

Dated paths in [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) are historical
experiment references, **not live artifact dependencies**. Rebuild a new
hypothesis from current maintained source and `config/match-units.toml`;
replay only its selected exact unit(s). Never recreate a full historical
cache, and do not delete the protected inputs to recover additional space.
