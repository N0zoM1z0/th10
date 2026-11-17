# TH10 reconstruction — agent handoff

**Updated 2026-11-17.** The source baseline for the measurements below is
`82bc4ab1` (`gpt-web: restore exact enemy runtime callback in C++`). This
handoff/artifact cleanup is a documentation-only follow-up. Continue on
`main`; keep work local and never push. [AGENTS.md](../AGENTS.md) is the
operational authority. [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) contains the
reverse-engineering details and rejected experiments worth retaining.

## First actions

1. Run `git status --short --branch`, `git diff --check` and `git log -8
   --oneline`. Preserve any work already present.
2. Before target-dependent work, run
   `scripts/repo-python scripts/verify-target.py`. The only valid target is
   the Japanese TH10 v1.00a executable with SHA-256
   `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
3. `config/functions.csv`, `config/matches.csv`, `config/match-units.toml` and
   `config/frontiers.toml` are the current ledgers. `docs/PROGRESS.md` is
   generated; do not edit it manually.
4. Use `scripts/repo-python` for repository Python. Use the pinned VC7.1 SP1
   profiles and LTCG `/GL`; do not run the historical full replay cohort for
   routine work.

## Accepted exact owners

These owners have complete, selected-unit cold-replay evidence in the current
ledgers. Their exactness is independent of the still-open larger owners.

| Owner | Extent | Current evidence |
| --- | ---: | --- |
| `EnemyResolveRuntimeCallback @0x4127A0` | 593 | Pure C++; 593/593 bytes, 147 instructions and seven target-bound DIR32 fields. Canonical 17-support-TU VC7.1 SP1 `/GL /GS` replay. The timeout flag uses a size-checked 32-bit bit-field plus `_ReadWriteBarrier()`; no inline assembly. |
| `PlayerHandleCollisionHit @0x426CF0` | 627 | 627/627 bytes and 21 fields in the real replay-init/load-script graph. |
| `PlayerLoadOptionData @0x426520` | 229 | 229/229 bytes and six fields in the real `PlayerInitialize` / `EnemyReadIntOperand` graph (20 support TUs). |
| `EnemyKillAll @0x40E6A0` | 182 | 182/182 bytes and six fields in the real `AnmLoadedView::InitializeVm` caller graph. |
| `TransitionShakeEnvelope @0x43C710` | 349 | 349/349 bytes and all 16 bound fields in two independent selected `/GL` Transition/GUI colds; the associated 396-byte Transition initializer and other same-image exact guards remain exact. |
| `GuiView::DrawStageElements @0x415800` | 724 | 724/724 bytes and 33 bound fields in the selected 17-input `/GL` GUI draw graph. Native score rectangle compound assignments restore exact x87 instruction scheduling. |
| `EnemyReadIntOperand @0x412350` | 820 | 820/820 bytes and 95 fields; retained as the exact dependency guard for the Player loader. |
| `FrontEndControllerView::RefreshOptionsDisplay` | 3,999 | 3,999/3,999 bytes in the refreshed FrontEnd options graph. |

The ECL stack leaves also remain exact in the current 36-support-TU graph:
`Pop` 122/122, `Push` 96/96, `EnterFrame` 64/64 and `LeaveFrame` 41/41.

## Current open owners

All measurements below were refreshed from the current source on 2026-11-17.
`OK` means the recorded frontier did not regress; it does not mean exact.
`REGRESSED` means the current residual offsets moved relative to the stored
frontier, even when the raw mismatch count is unchanged or lower.

| Owner | Current measurement | Next useful action |
| --- | --- | --- |
| `EnemyRuntimeView::DispatchEclInstruction @0x40E770` | In the real 36-support-TU graph: candidate 14,492 vs target 14,416 bytes, 10,568 differing bytes; `REGRESSED` vs the stored 10,416-byte residual set. Separate 12-TU `/GS` diagnostic: 14,496 bytes, 10,574 differences; its frontier is `OK`, but it is not a close or exact match. | Continue rank-five lower bands and operand/register scheduling. Keep the 12-TU result diagnostic-only and reconcile every change against the 36-TU graph. Do not refresh a frontier merely to accept moved bytes. |
| `AnmRenderManagerView::ExecuteScript @0x43EE30` | 9,964/9,964 bytes; two differences at +0x112/+0x115. Retail loads `[ebp+0x20]` into EDX and pushes EDX; candidate uses ECX. | Isolate opcode 91's call-site register lifetime without changing adjacent opcode bodies. The current source uses `AnmChildScriptLayer` with `script` and `layer` fields. Register, volatile, direct-argument and field-order trials did not improve it; all were reverted. |
| `EclVmContext::Run @0x44E1A0` | 7,020/7,020 bytes, 23 differences; frontier `REGRESSED` because the residual offsets moved. Main clusters: +0x0EB/+0x0F1 register choice, +0x129/+0x12B pointer register, +0x0403..+0x04CD spill homes, +0x0E6F/+0x0E7B and +0x1586 operand/address ordering, and the late +0x19E6/+0x19E8 register choice. | Reconcile the current residual set before changing the VM. The two float opcodes still have opposite spill-home preferences; previous declaration/result-local and case-order variants either moved differences or regressed the owner. Preserve the exact ECL stack leaves. |
| `GameResetGui @0x413BC0` | 1,965/1,965 bytes, 47 differences; frontier `OK`. | Continue the life-loop register schedule and stage-VM creation ordering. The previous manual stage-VM expansion worsened the owner and was reverted. |
| `BulletRuntimeView::UpdateAbsoluteDirectionChange @0x4078E0` | 335/335 bytes, two differences at +0xD7/+0xDD; target passes the angle in EDX while the candidate uses EAX. Frontier `OK`. | Revisit only with a new, falsifiable register-liveness hypothesis. `register` hints were neutral; direct angle passing and volatile forms regressed. |
| `ReplayPlaybackFrameControlCallback @0x42A3E0` | 68/68 bytes, four conditional-branch destination bytes at +0x13/+0x18/+0x22/+0x37. The adjacent 3,999-byte FrontEnd options owner is exact. Frontier `OK`. | Keep the natural runtime/flag/control-flow split; investigate shared return-tail layout without disturbing the exact FrontEnd owner. It remains nonexact. |
| `ResultMenuUpdateResult @0x4236F0` | Latest retained measurement: 3,216 vs 3,227 bytes, 2,285 differences; frontier `OK`. | Rebuild its graph before trusting the measurement; then continue the score receiver/frame and state-body layout. |

`GuiMessageVmView::Run @0x415E90` has an older 2026-11-14 measurement
(4,463 vs 4,480 bytes). It has not been refreshed in this handoff; rebuild its
current graph before using that number.

## Rebuild only the affected graph

The following bounded commands refresh the shared inputs and the relevant
comparison outputs. Pass every changed source TU after `--dirty`; omitted
sources can silently reuse stale LTCG objects.

```sh
# Main 36-support-TU graph: ANM, ECL, dispatcher and GUI owners.
scripts/repo-python scripts/diagnostics/triage/quick-relink.py player-handle-collision-hit --dirty src/AnmManager.cpp src/EclVm.cpp src/EnemyEclDispatcher.cpp src/Gui.cpp src/ReplayManager.cpp
scripts/repo-python scripts/check-frontier.py --no-build --show anm-execute-script ecl-run enemy-dispatch-ecl game-reset-gui

# Separate 12-TU dispatcher diagnostic; never treat this as canonical acceptance.
scripts/repo-python scripts/diagnostics/triage/quick-relink.py enemy-apply-bullet-clear --dirty src/AnmManager.cpp src/EclVm.cpp src/EnemyEclDispatcher.cpp
scripts/repo-python scripts/check-frontier.py --no-build --show enemy-dispatch-ecl-12tu

# FrontEnd callback and its exact 3,999-byte owner.
scripts/repo-python scripts/diagnostics/triage/quick-relink.py frontend-options-refresh --dirty src/ReplayManager.cpp src/AnmManager.cpp src/EnemyEclDispatcher.cpp
scripts/repo-python scripts/compare-linked-function.py --unit frontend-options-refresh --json

# Bullet angle leaf.
scripts/repo-python scripts/diagnostics/triage/quick-relink.py bullet-update-absolute-direction --dirty src/BulletManager.cpp
scripts/repo-python scripts/check-frontier.py --no-build --show bullet-update-absolute-direction

# Strict owner comparison and selected cold replay for a completed candidate.
scripts/repo-python scripts/compare-linked-function.py --unit UNIT --json
scripts/repo-python scripts/replay-exact-units.py --unit UNIT --json
```

Do not run the full historical cohort for ordinary iterations. A frontier is a
regression guard only; exact acceptance requires the complete owner, all
resolved fields/relocations and the selected canonical cold replay.

## Storage and retained diagnostics

`docs/RE_ARTIFACTS.md` lists the protected inputs and the five retained
noncanonical diagnostic contexts. `build/match-linked/` has 271 manifest-backed
output directories and no orphan directories; preserve it and the object-backed
`build/match/` inputs. The 2026-11-17 cleanup removed superseded ANM aggregate
trial outputs and scratch reports while retaining the original target, toolchain,
Ghidra project, D3D evidence, origin review and four optional trial drivers.
