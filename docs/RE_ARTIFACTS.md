# TH10 private inputs and storage handoff

**Updated 2026-11-17.** Preserve the original target and the small set of
private build/analysis inputs needed to continue the reconstruction.

## Protected inputs

- `resources/th10.exe`: original Japanese TH10 v1.00a, SHA-256
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
  Verify with `scripts/repo-python scripts/verify-target.py`.
- `.tools/`, including `.tools/wineprefix-msvc710-sp1/`, and the pinned
  VC7.1 compiler/linker setup.
- `ghidra-project/`, `_reference/`, maintained `src/`, `scripts/`, `config/`,
  Git history and build-input files.
- All manifest-backed `build/match-linked/` contexts and object-backed
  `build/match/` support objects. There are 271 referenced linked output
  directories and no orphan directories; these are reusable comparison inputs.
- `build/anm-caller-abi-test/caller.cpp`; generated object outputs are absent.
- `docs/evidence/anm-loaded-start-script-wave11-20261109/compiler-context.cpp`
  and `docs/evidence/anm-child-request-record/binding-contract.json`.
- Unrelated `.analysis/d3d-*` work and its private evidence.
- `.analysis/gpt-5.6-sol/20260917-origin-review/runtime-origin-review.json`
  (the independent target-hash-pinned origin review).

## Five retained noncanonical diagnostic contexts

Keep these narrow contexts for unresolved behavior:

| Owner | Directory under `build/gpt-web/` |
| --- | --- |
| Enemy ECL dispatcher rank-tail | `20261108-enemy-dispatch-rank-tail-final/` |
| Enemy runtime aggregate | `20261108-enemy-runtime-aggregate-sum/` |
| ANM `ExecuteScript`, opcode 91 | `20261109/anm-execute-child91-aggregate/` |
| ECL `Run`, pointer order | `20261109/ecl-run-length-pointer-order/pointer-y-first/` |
| Player HUD natural loader | `20261109/playerhud-restored-natural-loader/` |

Four optional trial drivers remain under `.analysis/gpt-web/`:

- `20261110-anm-child91-aggregate-initializers.py`
- `20261110-bullet-absolute-speed-liveness.py`
- `20261110-ecl-format-cursor-allocation-schedule.py`
- `20261110-seek-tail-control-flow-trials.py`

Other per-variant images, copied source snapshots, temporary patches and
one-off scripts can be recreated from the current source and unit manifest.
The Ghidra project, Factory runtime state, D3D adapter evidence and independent
origin review are retained separately and are not scratch cleanup targets.

## 2026-11-17 cleanup

Removed the superseded `build/gpt-web/20261110/anm-child91-aggregate/` variant
matrix (about 3.31 MB) and 77 obsolete files from `.analysis/gpt-web/`
(about 1.66 MB). The combined reclaimed space is approximately 4.97 MB.
Their useful negative results and current owner measurements are summarized in
`docs/KNOWLEDGE_BASE.md` and `docs/RE_HANDOFF.md`. The five diagnostic contexts,
all manifest-backed `build/match-linked/` outputs, `build/match/` inputs,
original target, toolchain, Ghidra project, D3D work and origin review were
preserved.

Keep new session scratch below `.analysis/` and generated builds below `build/`.
Delete superseded experiment outputs after recording the useful finding; do not
create replacement archives, deletion manifests or recurring cleanup reports.
