# TH10 reconstruction — next-agent handoff

**Updated 2026-11-20.** Start with [AGENTS.md](../AGENTS.md), this brief, and the relevant source/ledger rows. The current phase is natural C/C++ reconstruction with targeted VC7.1 codegen comparison. Product closure, semantic runtime validation and portability are later phases.

## Checkpoint and worktree

- Code baseline: `7c93f291` — `gpt-web: use state opcode for active flag`. Latest metadata checkpoint: `a0ca57dc` — `gpt-web: align bullet pattern owner mapping`. Continue on `main`; do not push.
- The committed state-8 action publishes `this->activeTransformFlags |= kind`. It has been rebuilt in the current source graph; the complete owner remains nonexact at the PDB-extent gate below.
- The previous random-mode experiment in `src/BulletManager.cpp` was reviewed against the retail x87 sequence and reverted. Explicit float temporaries introduced `fstp`/`fmul` stack spills absent from the target. `BulletManager.cpp` is back at its committed source state; do not reintroduce that rewrite without new target evidence.
- The current worktree has documentation/handoff edits only; C/C++ sources and comparison recipes are clean. No original target or toolchain inputs were modified.
- The original target remains the private, ignored `resources/th10.exe`, Japanese TH10 v1.00a, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`. Keep it read-only and uncommitted.
- `config/match-units.toml` contains linked-PE recipes; `config/matches.csv` is the canonical exact ledger; `config/frontiers.toml` is diagnostic only. A local strict comparison is not a substitute for a selected canonical cold replay.

## Main owner: `AdvanceTransformProgram`

`BulletRuntimeView::AdvanceTransformProgram`, target `0x00406D90`, is the substantial nonexact owner in `src/BulletTransform.cpp`.

- Target code extent: `0x5AE` / 1,454 bytes.
- Full code-and-local-data comparison extent: `0x644` / 1,604 bytes.
- Unit: `bullet-transform-program`, rooted at `?AdvanceTransformProgram@BulletRuntimeView@@QAEXXZ`, built with VC7.1 SP1 LTCG `/GL` and `/GS` in `build/match-linked/BulletTransformProgramNormal`.
- The suffix includes five local switch-table destinations at `+0x5B0..+0x5C0` and the 128-byte selector span at `+0x5C4`.
- Runtime layout: up to 18 records, stride `0x18`, first record at `+0x464`, kind at record `+0x10`, allow-while-active at `+0x14`, active flags at `+0x43C`, cursor at `+0x45C`, and nine extension-state slots beginning at `+0x614`.

A fresh targeted build from the current committed C++ body produces a PDB contribution of `0x640` / 1,600 bytes. The strict comparator stops with `PDB contribution size 0x640 differs from manifest 0x644`; it therefore has not compared the complete owner. Do not lower `compare_size`, shrink the selector span, or claim exactness from a partial prefix.

The target's first indexed loop load is:

```asm
mov edx, [ebp+45Ch]
lea eax, [edx+edx*2]
mov ecx, [ebp+eax*8+474h]
lea esi, [ebp+eax*8+464h]
```

The current natural C++ candidate still allocates the scaled index and loaded opcode to different registers/order than the target. Repeated targeted experiments with raw byte-offset arithmetic, direct array indexing, volatile opcode loads, local declaration ordering and register hints did not resolve this. Keep those failed experiments reverted. Next, explore C++ expression/lifetime shapes that recover EAX as the scaled index and ECX as the opcode without inline assembly or fake ABI tricks. Then continue through child descriptor initialization, the distinct loop tails, local jump tables and selector bytes.

The child-pattern call at `+0x498` is now bound to the descriptor-owned symbol `?SpawnBulletPattern@BulletSpawnDescriptorView@@QAEHPAUBulletManagerView@@@Z`; the earlier `BulletManagerView::SpawnBulletPattern` mapping was stale.

Do not use inline assembly, naked functions, fake ABI declarations, copied target bytes, padding, inert locals or comparator cropping. A matching leaf never makes this parent exact.

## Relevant leaf owners

| Target owner | Fresh/current result | Canonical status |
| --- | --- | --- |
| `0x004073E0` `BulletSpawnDescriptorView::SpawnBulletPattern` | Fresh targeted build: 194/194 bytes, 6/6 target-bound fields | Accepted 194-byte row in `config/matches.csv`; source mapping and linked symbol now consistently use the descriptor-owned method. |
| `0x00404F30` `AnmVmView::StartScript` | Independent selected cold replay: 202/202 bytes, 2/2 REL32 fields | Accepted 202-byte exact row in `config/matches.csv`; maintained source and reccmp mapping now point to the native member method, while the global wrapper remains an adapter. |
| `0x004067D0` `BulletSpawnDescriptorView::SpawnSingleBullet` | Fresh selected link: 1,472-byte complete extent; all 44 target fields bind; 1,418/1,472 bytes match; 54 ordinary-byte differences remain | Large nonexact leaf frontier. The `StartScript` resource/context, state-2/backstep branch order, all three `4.0f` relocations and collision x87 store order now match. Continue with effect-switch register allocation and the post-spawn cursor/return tail; do not claim exactness. |
| `0x00408030` `EnemyCancelBulletRecord` | Existing strict linked result: 199/199 bytes, 4 target-bound fields | Accepted historical row in `config/matches.csv`; no new replay was needed for this session. |

For `0x00404F30`, the maintained owner is `AnmVmView::StartScript(int, AnmLoadedView *, void *)` in `src/AnmVmStartScript.cpp`; the global `StartEntityScript_00404f30` wrapper remains for ordinary source-order callers. The selected independent cold replay now reproduces the full 202-byte owner, including the two declared REL32 calls, and the canonical exact/source mappings use the member method name. The target-private entry ABI is VM in ESI, script index in EAX, loaded resource on the stack, and `RET 4`; this exact codegen result does not claim original-TU or product/runtime closure.

## Bounded workflow

```sh
# Main nonexact owner
scripts/repo-python scripts/build-match-unit.py --unit bullet-transform-program
scripts/repo-python scripts/compare-linked-function.py --unit bullet-transform-program --json

# Relevant leaves
scripts/repo-python scripts/build-match-unit.py --unit bullet-spawn-pattern-exact
scripts/repo-python scripts/compare-linked-function.py --unit bullet-spawn-pattern-exact --json
scripts/repo-python scripts/build-match-unit.py --unit entity-start-script
scripts/repo-python scripts/compare-linked-function.py --unit entity-start-script --json

# Only for a deliberate canonical promotion, not ordinary iteration
scripts/repo-python scripts/replay-exact-units.py --unit NAME --json

git diff --check
```

`entity-start-script` and `bullet-transform-program` share `build/match-linked/BulletTransformProgramNormal`; rebuild the selected recipe before trusting that directory's PDB/PE/MAP artifacts. Do not run the full historical cold replay for ordinary bounded work.

## Retained context

Keep `resources/th10.exe`, `.tools/` and the TH10 Wine prefix, `ghidra-project/`, `_reference/`, canonical ledgers, `docs/evidence/`, selected original Ghidra exports in `.analysis/`, configured `build/match-linked/` outputs, and unresolved `build/gpt-web/` contexts. See [RE_ARTIFACTS.md](RE_ARTIFACTS.md) for the retained artifact inventory. The adjacent `_reference/th10-decomp` project is a separate historical reference, not this repository's build or status authority.

## 2026-11-21 bullet-runtime follow-up

This section supersedes stale status statements above where they conflict with the two newer commits. Latest source checkpoint is `1e5630b9` (`gpt-web: use explicit planar bullet bounds`), preceded by `3eadb55b` (`gpt-web: reconstruct bullet cancel bounds and VM lookup`). The branch is `main`, two commits ahead of `origin/main`; no full cold replay was run in this iteration.

### `BulletManagerView::CancelBulletsInArea`

- Target: `0x00408210`, manifest extent `0x249` (585 bytes).
- Current source: `src/BulletCancelArea.cpp`; it iterates 2,000 slots, skips states 0 and 3, computes explicit XY lower/upper bullet bounds, sets the cancel flag, optionally spawns an item, creates the ANM cancellation VM, resolves it through `AnmRenderManagerView::FindVm`, selects the size-based palette, and clears the trailing state word.
- Current targeted linked contribution: `0x23B` (571 bytes), so strict comparison stops at the extent gate. This is a semantic/source checkpoint, not exact.
- Target x87 order is known: width half -> lower X -> height half -> lower Y -> upper X -> upper Y, followed by the four overlap comparisons. Keep the XY math as natural C++; do not use volatile locals just to force spills. The compiler still schedules/reuses some temporaries differently from retail.

### `BulletRuntimeView::UpdateAbsoluteDirectionChange`

- Target: `0x004078E0`, extent `0x14F` (335 bytes).
- Current targeted result: candidate extent is also 335 bytes; 333/335 bytes match. The only differences are at offsets `0xD7` and `0xDD`: retail loads the angle word into EDX and pushes EDX, while the candidate uses EAX. The current bit-preserving `memcpy` source is retained; direct union reads and local-union/register variants did not improve the result.
- Do not call this exact until the two-byte register-selection difference is resolved by a natural C++ expression.

### `BulletRuntimeView::AdvanceTransformProgram`

- Target: `0x00406D90`, full extent `0x644` (1,604 bytes).
- The committed natural C++ body currently contributes `0x640` (1,600 bytes); the strict comparator therefore stops before comparing the full owner.
- A temporary `const unsigned int&` opcode-field variant reached the `0x644` extent but failed the target-bound relocation-field check: the indexed kind load and record pointer were formed in the wrong order/registers. A typed enum reference produced only `0x638`. These experiments are retained under `.analysis/` and are not part of the source tree.
- Keep the current source reverted to its natural enum local. Continue with the indexed-load/register-allocation and jump-table structure; do not use inline assembly, copied bytes, padding, or comparator cropping.

### `SpawnSingleBullet` status correction

The fresh `bullet-spawn-single-exact` build from the current committed `src/BulletManager.cpp` contributes `0x585` (1,413 bytes) against the `0x5C0` (1,472-byte) manifest extent, with 1,359/1,418 bytes matching in the compared prefix. This supersedes the older 54-byte-difference note in the earlier leaf table; re-establish that frontier from current source before resuming it.
