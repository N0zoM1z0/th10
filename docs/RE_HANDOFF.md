# TH10 reconstruction — next-agent handoff

**Updated 2026-11-17.** Continue on the repository's local `main` branch; do not push. Start with [AGENTS.md](../AGENTS.md). This brief tracks the current typed ECL and Chain allocator source, including the `chain-alloc-elem`, `main-create-fps-counter`, and `main-fps-callback` exact owners. Git history is the provenance record; do not assume a clean tree without checking it.

## First five minutes

Run `git status --short --branch`, `git diff --check` and `git log -8 --oneline`. Preserve any dirty changes and resolve them before switching owners. Verify the only valid target with `scripts/repo-python scripts/verify-target.py`: original Japanese TH10 v1.00a, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`. Use `scripts/repo-python scripts/verify-toolchain.py --check` for compiler declarations, not a repeated full toolchain smoke.

The count authority is generated [PROGRESS.md](PROGRESS.md), not an old narrative or an aggregate match-size guess. As of this handoff, the 1,832 reviewed candidates include **1,555 exact functions / 214,653 exact codegen bytes**; among 751 reviewed authored functions, 639 functions / 144,324 bytes are exact and 112 mapped authored functions remain. Both seven-byte GUI callback entries are exact **with indeterminate original authorship**. Separate source mapping, original authorship, compilation and whole-game runtime closure.

## Complete exact owners to protect

| Owner / target | Strict current evidence |
| --- | --- |
| `GuiView::UpdateStageElements @0x414900` | 3,836/3,836 bytes, 72 bound fields; the separate update callback `0x415AE0` is 7/7 with one call field. |
| `GuiView::DrawStageElements @0x415800` | 724/724 bytes, 33 fields; the separate draw callback `0x415AF0` is 7/7 with one call field. |
| `TransitionShakeEnvelope @0x43C710` | 349/349 bytes, 16 fields; adjacent Transition initializer 396/396, 37 fields. |
| `ReplayManager::ProcessFrame @0x4297D0` | 596/596 bytes, 33 fields; `ReplayProcessFrameCallback` 10/10, one field. |
| `EnemyResolveRuntimeCallback @0x4127A0` | 593/593 bytes, seven fields, natural C++ with retained bit-field logic. |
| `PlayerHandleCollisionHit @0x426CF0` | 627/627 bytes, 21 fields; `EnemyApplyBulletClear` in its own 12-TU graph is 44/44. |
| `EnemyKillAll @0x40E6A0`, `EnemyReadDialog @0x415DB0` | 182/182 and 124/124 bytes, respectively, in their affected owner graphs. |
| `PlayerLoadOptionData @0x426520`, `EnemyReadIntOperand @0x412350` | 229/229 and 820/820 bytes, respectively; preserve the existing caller bindings. |
| `FrontEndControllerView::RefreshOptionsDisplay` | 3,999/3,999 bytes in `frontend-options-refresh`. |
| `th10::Chain::AllocElem @0x449ED0` | 74/74 bytes and one target-bound `operator new` REL32 in four-TU ScriptViewer popup graph; targeted cold replay exact. |
| ECL stack `Pop` / `Push` / `EnterFrame` / `LeaveFrame` | 122 / 96 / 64 / 41 exact bytes, respectively; the complete `Run` owner is **not** exact. |

All exact entries are tracked in `config/matches.csv`. Source-presence and original-source origin are separate in `config/reccmp-functions.csv`, `config/implemented.csv` and `config/function-origins.csv`. Do not promote a partial byte count or nonexact parent because an exact leaf survived.

## Current nonexact work — measured source epoch

All sizes below are **candidate / target**. `OK` and `REGRESSED` are comparisons against *older recorded residual-offset frontiers*, not exactness labels. A lower mismatch count can still fail the frontier if offsets moved.

| Owner | Latest bounded measurement | Useful next attack |
| --- | --- | --- |
| `EnemyRuntimeView::DispatchEclInstruction @0x40E770` | Main 36-support-TU graph: **14,480 / 14,416**, 10,564 differing bytes, frontier `REGRESSED`. Separate 12-TU `/GS` diagnostic: **14,424 / 14,416**, 10,412 differences, also `REGRESSED`. | Rank-5 lower-band control flow and operand scheduling. The latest typed `EnemyLaserType0/1RequestView::position` stores, typed `memset` initialization, and `EnemyRuntimeView*`-based unchecked pattern/offset indexing are retained; both 36-TU and 12-TU byte-difference sets are unchanged from HEAD. Never confuse the smaller 12-TU diagnostic with canonical acceptance. |
| `AnmRenderManagerView::ExecuteScript @0x43EE30` | **9,964 / 9,964**, two differing bytes at +0x112/+0x115. | Opcode 91: original passes the child layer in EDX, current compiler selects ECX. Most obvious direct arguments, local/volatile qualifiers and field-order variants were tried without improvement. |
| `EclVmContext::Run @0x44E1A0` | **7,020 / 7,020**, 23 differing bytes; frontier `REGRESSED` on moved offsets. | Focus the thread spawn/stop register choices, integer ADD/SUB stack slots and the handful of addressing forms. Changing stack-leaf spelling may preserve leaf exactness but badly regress the parent; compare both. |
| `GuiMessageVmView::Run @0x415E90` | **4,464 / 4,480**, 1,935 differences, versus older frontier's 1,910 offset set. | Portrait operand register lifetimes and resource-read ordering, inline clear-count increments, stage 6/7 tail sharing. Do not collapse the eight apparently duplicate character branches: that shrinks code to 3,796 bytes and loses target layout. All 36 portrait script-ID additions now have explicit native 32-bit wrap semantics. |
| `ResultMenuUpdateResult @0x4236F0` | Last retained check: **3,216 / 3,227**, 2,285 differences. | Rebuild the graph before using this older measurement; inspect state 7/8 and 10–13, score receiver and stack-home layout. |
| `GameResetGui @0x413BC0` | **1,965 / 1,965**, 47 differences, `OK`. | Life-count loop register schedule, two stage VM-creation call sites. Wrapped DWORD stage script IDs are already retained; manual loop expansion regressed. |
| `GuiMessageVmView` constructor `@0x415B00` | **684 / 683**, 81 ordinary byte mismatches. | Preserve scoped VM ID return values and storage layout; the extra byte is near the final enemy-manager register reload. A 683-byte spelling had 227 mismatches and is worse. |
| `ScriptViewerPopupOwnerView::Draw @0x42B780` | **537 / 540**, 266 non-linkage differences; draw callback 10/10 length but nonexact. | Owner/digit-cursor register allocation and alpha lifetime. The advancing digit address is now native DWORD arithmetic; Update stays 134/134 exact. |
| `TransitionShakeLinear @0x43C550` | **439 / 439**, 27 differences. | The floating divide is scheduled before the second RNG state update; naive source movement breaks the entire function. Preserve the separate exact 349-byte envelope. |
| `Chain::Chain @0x449AA0` | **59/59**, eight residual bytes; `Chain::AllocElem` is now separately canonical exact. | First self-link/second sentinel register scheduling remains nonexact; preserve the new allocator exactness and three exact destructor/popup guards. |
| `BulletRuntimeView::UpdateAbsoluteDirectionChange @0x4078E0` | **335 / 335**, two register bytes at +0xD7/+0xDD. | Angle parameter EDX versus EAX; multiple earlier type/`register` hints were neutral or worse. |
| `ReplayPlaybackFrameControlCallback @0x42A3E0` | **68 / 68**, four conditional-branch destinations. | Shared return tail; preserve the exact 3,999-byte FrontEnd owner. Explicit `goto` variants severely regressed. |

Detailed target observations, meaningful failed trials and source-layout notes live in [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md). **Do not refresh** `config/frontiers.toml` simply to make a changed residual offset set pass; compare the full original and candidate.

## Small, repeatable validation commands

Every command below builds only its named graph. Pass the **actual changed translation units** after `--dirty`; missing one silently reuses its previous LTCG object. Rebuild the graph after experimenting with code, even if the source was subsequently restored.

```sh
# Main large-owner graph; specify additional changed TUs when relevant.
scripts/repo-python scripts/diagnostics/triage/quick-relink.py player-handle-collision-hit --dirty src/EnemyEclDispatcher.cpp
scripts/repo-python scripts/check-frontier.py --no-build enemy-dispatch-ecl anm-execute-script ecl-run game-reset-gui
scripts/repo-python scripts/compare-linked-function.py --unit player-handle-collision-hit --json

# Smaller ECL dispatch diagnostic and its strict leaf guard.
scripts/repo-python scripts/diagnostics/triage/quick-relink.py enemy-apply-bullet-clear --dirty src/EnemyEclDispatcher.cpp
scripts/repo-python scripts/check-frontier.py --no-build enemy-dispatch-ecl-12tu
scripts/repo-python scripts/compare-linked-function.py --unit enemy-apply-bullet-clear --json

# Alternative individual owner graphs.
scripts/repo-python scripts/diagnostics/triage/quick-relink.py gui-message-run-candidate --dirty src/Gui.cpp
scripts/repo-python scripts/diagnostics/triage/quick-relink.py enemy-read-dialog --dirty src/GuiMessage.cpp
scripts/repo-python scripts/diagnostics/triage/quick-relink.py transition-shake-linear --dirty src/TransitionEffect.cpp
scripts/repo-python scripts/diagnostics/triage/quick-relink.py script-viewer-popup-update --dirty src/ScriptViewerPopupCreate.cpp
scripts/repo-python scripts/diagnostics/triage/quick-relink.py frontend-options-refresh --dirty src/ReplayManager.cpp

# Only once a new or modified exact owner passes strict whole-owner checks:
scripts/repo-python scripts/compare-linked-function.py --unit UNIT --json
scripts/repo-python scripts/replay-exact-units.py --unit UNIT --json
```

The canonical target executable, Wine/VC7.1 setup, local Ghidra project and current manifest-backed `build/match-linked/` contexts are kept. See [RE_ARTIFACTS.md](RE_ARTIFACTS.md) for the retained scratch scope and storage cleanup. Documentation-only changes do **not** need game builds: run `scripts/repo-python scripts/ci.py`, `scripts/repo-python scripts/progress.py --check` when ledgers change, and `git diff --check`.
