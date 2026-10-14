# TH10 reconstruction handoff

Updated 2026-10-14. This is a concise resume brief, not an experiment journal.
Keep live totals in the generated [`PROGRESS.md`](PROGRESS.md), durable target
facts and negative results in [`KNOWLEDGE_BASE.md`](KNOWLEDGE_BASE.md), and
experiment chronology in Git history. Only explicitly dated, scoped
diagnostics belong in the checkpoint table; do not copy unpinned probe scores,
scratch paths, or build hashes into this file.

## Authority and recovery

- Target only the original Japanese TH10 v1.00a executable at the ignored
  `resources/th10.exe`, SHA-256
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
- Use `scripts/repo-python` for repository Python commands and keep target
  identity, boundary, origin, source presence, exact codegen, whole-product
  closure, runtime validation, semantics, and portability as separate claims.
- Work from the live worktree, including dirty changes. Inspect and continue,
  deliberately supersede, or reject existing work; do not create a clean side
  tree merely to avoid understanding it.
- Operator-requested commits use `gpt-web: ...`. Do not push from this worktree.
- Treat Ghidra, decompiler output, adjacent games, compiler diagnostics, and
  similarity scores as evidence or hypotheses according to their Oracle
  boundary. Only canonical replay can grant exact-codegen credit.
- Read current repository-wide counts from `docs/PROGRESS.md` or regenerate
  them. Do not reuse counts copied from an older commit or experiment.

## Validated large-owner checkpoints (not exact claims)

These are **dated, compiler-context-specific diagnostics**, not live exact
ledger entries. Rebuild each probe after changing a supporting translation unit.

| Owner | Last checked source context | Observed candidate / target | Open work |
| --- | --- | --- | --- |
| ECL `EclVmContext::Run` | `7d77b95`, EclVmStack linked /GL Run probe | 7,020 / 7,020 owned bytes; 59 / 59 dispatch destinations aligned; historical 6,209 raw matches must not be divided by the 6,264 comparable-byte denominator | Remaining register, stack-home, and opcode byte differences; non-exact |
| ANM `ExecuteScript` | `d2cddda`, ANM /GL /GS probe | 9,964 / 9,964 bytes, 0xFC target frame, 92 physical groups in order, 84 of 92 spans target-sized; 2,986 / 8,572 raw comparable bytes | Eight non-equal group spans and aggregate/RNG private ABI; non-exact |
| Enemy `DispatchEclInstruction` | `d2cddda`, selected ECL + ANM support graph | 14,384 / 14,416 bytes; target-sized 181-byte selector, all 108 group destinations in physical order; 0x2BC vs 0x2C4 stack allocation | ECL argument adapters, spawn/laser group lifetimes, stack allocation; non-exact |

The 2026-10-14 bounded Run refinement nests `ReadIntValue` in the
`SpawnThread` call, restoring target argument-push scheduling. Two cold
`EclVmHost::Run` entry probes reproduce 7,020 owned bytes, all 59 aligned
dispatch destinations, 6,223 / 7,020 raw matches and 6,115 / 6,264
matched-comparable bytes (baseline 6,210 and 6,102 respectively).
`normalization_complete=false`: the final 88-byte selector table is neither
decoded code nor DIR32 relocation data. The current checker deliberately
rejects that suffix; no checker or policy change was made. Register, stack-home
and FORMAT differences also remain. This is zero new exact credit.
Only Run was modified and cold-probed; prior helper units were not replayed.

A later isolated FORMAT store-before-null-check move is also reproduced by
two cold real-host probes: 6,233 / 7,020 raw matches and 6,125 / 6,264
matched-comparable bytes, with every dispatch offset still aligned. The
percent/scratch stack homes remain swapped and normalization remains
incomplete. This scheduling improvement adds zero exact credit.

The later `62b0980` change is restricted to the non-exact
`EnemyCancelBulletRecord` timer-flag dataflow (200 / 199 candidate/target
bytes; 131 / 183 comparable bytes), with all 14 source-scope canonical exact
units passing. The dispatcher-size checkpoint above predates this adjacent
source change and must be regenerated before making a dispatcher-size claim.

## Bounded ANM closure (2026-10-14)

`AnmLoadedView::Release` (242 bytes) and `ServicePreloadedAnms` (135 bytes)
pass two focused cold canonical linked-PE replays with all 11 fields bound.
The source change preserves one texture-entry pointer across COM Release/free,
matching target side effects and recovering both private-register allocations.
The local ledger gains 377 authored bytes; these local passes are not Factory
receipt acceptance. The broader historical ANM regression was not run.

## Bounded Bullet closure (2026-10-14)

`UpdatePolarAcceleration` at `0x004076A0` passes two focused canonical
cold /GL /GS replays: 209 bytes and all eight linkage fields, complete
normalization. The retained changes restore the expired-duration-first branch
and scale-before-subframe x87 load order, including FST before conversion.
Only this owner was modified and replayed. `UpdateVerticalWrap` remains
non-exact; the duplicated-branch control was reverted. Factory acceptance
for this unit passed at commit `b6fadac1f786801dbc4cdbd9b5e4558844e02b17`:
receipt `6c364031e1979cc5647b8d4b4b39e98c425d783307df6c4f1dc60a45c29ce364`,
verdict pass and decision accepted, bound to snapshot
`8d6643829b54e3d8744f18c81decaef505a876130cbf656d4ed93172b91d27f3`.

### Prior ANM receipt bindings

Both prior ANM units received pass/accepted Factory receipts at clean commit
`11a9ab346a81341ce37ca7d823630e1f467a461d`, source snapshot
`850fff77f0604b27e516465989888b857232331f3aa18baa079a8bf8ba7e5f28`:

- Service: `receipt:5caf3240ab78421312f231b5fc2101b80520b8dd0108eff9cc3fde582e5bd30c`.
- Release: `receipt:134b6af1da960c7d23a71b3f14e6333609b57a1b2a7f328a693536368a0447e3`.

These attest that source snapshot only; imported ledger totals are not a
fresh replay of every historical function. No unchanged historical unit was
replayed in the Bullet batch.

## Recent bounded negative controls (2026-10-14)

- The ECL percent-qualifier-only and conversion-switch-only controls each
  grew the owner to 7,024 bytes and worsened matching. Both were reverted;
  keep the isolated store-before-check refinement and stop that FORMAT branch.
- Radial initializer `0x004452F0` is a stronger medium-owner follow-up:
  the existing ExecuteScript /GL /GS graph with RandomMath, AnmVmCreate and
  AnmVmId yields 803 / 803 bytes, complete normalization and 623 / 631
  comparable matches. Three targeted source controls regressed and were
  reverted. Eight instruction/scheduling bytes remain; no exact credit.
  Do not reuse the unrelated service-entry 571-byte probe as its baseline.

## Active reconstruction order

1. **ECL Run — `EclVmContext::Run @ 0x0044E1A0`**
   - Keep the full target-owned 7,020-byte function in scope; do not reduce the
     problem to nearby leaf functions.
   - Rebuild diagnostics from the current source before judging a change.
   - The FORMAT/shared-tail placement and all 59 dispatch destinations were
     aligned in the selected checkpoint. Concentrate on remaining raw-byte
     differences, stack homes, and private register allocation; do not treat
     target-sized layout as exactness.
   - Replay only the canonical exact units actually affected by a bounded edit.
   - The six operand/read/resolve helpers at `0x0044FDB0-0x004500CC`, plus `SpawnThread`, `FindThread`, `StopAllThreads`, `Push`, `Pop`, `EnterFrame` and `LeaveFrame`, are canonical exact constraints; do not trade them away for a runner score.

2. **ANM ExecuteScript — `AnmRenderManagerView::ExecuteScript @ 0x0043EE30`**
   - Preserve the reviewed 92 physical dispatch groups in target order.
   - **Correction:** target and present candidate both restore EDI at the
     shared loop tail (the missing-EDI claim was stale).
   - Retain the recovered 0xFC frame and Y-before-X SCALE_TIME dataflow;
     investigate the remaining eight case spans and aggregate/RNG lifetimes
     instead of reshuffling the 92 dispatch groups.
   - Rebuild the linked diagnostic after support-source changes.

3. **Enemy dispatcher — `EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770`**
   - Always regenerate the diagnostic baseline from current source first.
   - Preserve the current 181/181 selector and 108-group ordering;
     prioritize private ECL argument calling conventions, laser construction,
     0x2C4 target stack allocation, rank arithmetic and x87 homes.

4. **Batch closure**
   - `BulletRuntimeView::UpdateHorizontalWrap @ 0x00407DA0` is canonical exact
     (148 bytes); `UpdateVerticalWrap @ 0x00407E40` is target-sized but remains
     non-exact. Preserve the full `BulletManager.cpp` exact gate when refining it.
   - Interleave smaller non-exact Bullet/ECL functions when they constrain the
     large owners, but do not turn the project into a leaf-only queue.
   - After a coherent batch, run the broader tracking/progress/CI checks and the
     required focused exact replays.

## Recovery and verification

Start every resumed session by recovering the real worktree:

```sh
git status --short --branch
git diff --check
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --execute
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/report-reconstruction-status.py
```

For a bounded source change, use the narrowest relevant diagnostic first.
Before committing a coherent batch, run:

```sh
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py
scripts/repo-python scripts/progress.py
scripts/repo-python scripts/ci.py
git diff --check
```

Factory receipt acceptance is separate from a local replay or Git commit.
Never report a diagnostic size, similarity score, successful compile, or
checkpoint commit as exactness.

## Scratch and cleanup policy

- `.analysis/` and `build/` are disposable working state, not durable records.
  Keep scratch only while an unresolved claim needs it.
- Before pruning scratch, preserve retained source edits in Git and move durable
  conclusions into source, ledgers, reusable scripts, or `KNOWLEDGE_BASE.md`.
- Remove superseded probes, decompiler dumps, logs, generated PE/PDB/OBJ files,
  stale build trees, and Python caches after a bounded batch.
- Live knowledge entries must not depend on deleted scratch files.
  Historical experiment paths and superseded scores belong in Git history,
  not in the maintained knowledge index.
- Point-in-time audit snapshots belong in Git history once their durable accepted
  conclusions have been absorbed into source, ledgers, and `KNOWLEDGE_BASE.md`;
  do not keep obsolete snapshot reports on the live documentation surface.
- Keep `ghidra-project/`, `.tools/`, `_reference/`, and `resources/th10.exe`;
  they are private analysis state, pinned tools/reference material, and the
  canonical private target rather than generic build debris.
