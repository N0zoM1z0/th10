# TH10 private inputs and reclaimable storage

**Updated 2026-11-17.** This is the storage handoff for the next reconstruction agent. The short task and unit-selection brief is [RE_HANDOFF.md](RE_HANDOFF.md). Preserve active reconstruction inputs; only discard reproducible scratch after recording useful conclusions.

## Protect these inputs and contexts

- `resources/th10.exe` — original Japanese v1.00a target, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`; keep ignored and read-only.
- `.tools/`, especially `.tools/wineprefix-msvc710-sp1/`, shared-runtime links, pinned VC7.1 compiler/linker and related Wine setup. The approximately 585 MiB tool tree is **not** disposable scratch.
- `ghidra-project/`, `_reference/`, maintained `src/`, `scripts/`, `config/`, original resources, Git history and its reflogs.
- `build/match-linked/`: **270 distinct configured output-directory names**, of which **243 directories are currently present** and 27 have not been built. No existing directory is orphaned. Keep these reusable linked-PE/MAP/PDB and support-object contexts, plus `build/match/` inputs.
- `build/anm-caller-abi-test/caller.cpp`, `docs/evidence/anm-loaded-start-script-wave11-20261109/compiler-context.cpp` and `docs/evidence/anm-child-request-record/binding-contract.json`.
- D3D adapter work under `.analysis/d3d-*` and its evidence, the original target origin review at `.analysis/gpt-5.6-sol/20260917-origin-review/runtime-origin-review.json`, and the private analyzer runtime. Some older D3D evidence pages reference historical private paths that are no longer present; those paths are archival provenance, not current build inputs.
- The handful of root-level ECL Ghidra exports: `dispatch-ecl-0040e770.c`, `dispatch-ecl-0040e770-listing.txt`, `dispatch-ecl-full-listing.txt` and `ecl-read-int-0044fdb0.c`, all below `.analysis/`.

Five noncanonical, small diagnostic workspaces remain under `build/gpt-web/` because they cover unresolved native scheduling:

| Problem | Retained relative path under `build/gpt-web/` |
| --- | --- |
| Enemy dispatcher rank-tail | `20261108-enemy-dispatch-rank-tail-final/` |
| Enemy runtime aggregate | `20261108-enemy-runtime-aggregate-sum/` |
| ANM opcode 91 | `20261109/anm-execute-child91-aggregate/` |
| ECL pointer ordering | `20261109/ecl-run-length-pointer-order/pointer-y-first/` |
| Player HUD loader | `20261109/playerhud-restored-natural-loader/` |

Four narrowly scoped optional trial scripts remain in `.analysis/gpt-web/`: `20261110-anm-child91-aggregate-initializers.py`, `20261110-bullet-absolute-speed-liveness.py`, `20261110-ecl-format-cursor-allocation-schedule.py` and `20261110-seek-tail-control-flow-trials.py`. Their presence does not make their unsuccessful results current evidence.

## Space reclaimed on 2026-11-17

The previous documented cleanup removed approximately **4.97 MiB** of obsolete ANM trial variants and reports. The **additional handoff cleanup** performed now:

- Deleted **430 top-level generated files** from `.analysis/` (old build logs, duplicate strict/selected comparison JSON, temporary host executables and discarded trial scripts), reclaiming **1,988,757 bytes (~1.90 MiB)**. Protected subdirectories, the Ghidra exports and the lockfile were left alone.
- Repacked Git **without pruning any object or expiring any reflog** (`git -c gc.reflogExpire=never -c gc.reflogExpireUnreachable=never gc --prune=never`). `.git/` shrank from **130,440 KiB** to **20,072 KiB**, saving **110,368 KiB (~107.78 MiB)**. All 22,588 Git objects are preserved in two packs; the connectivity check completed successfully.
- **Additional savings this sweep: approximately 109.68 MiB**. The complete checked-out repository now occupies about **881 MiB**, including protected toolchain, reference and build contexts. No tracked game source, protected private input, referenced build context or acceptance record was deleted.

Continue to keep scratch below `.analysis/` and builds below `build/`. Old one-off reports and duplicate source snapshots may be removed after their findings are preserved in [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md). Do not remove the retained contexts above or manufacture replacement archives or cleanup manifests.
