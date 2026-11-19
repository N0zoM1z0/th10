# TH10 reconstruction — next-agent handoff

**Updated 2026-11-19.** Read [AGENTS.md](../AGENTS.md), this brief, and the relevant topic in [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md). Keep commits local; do not push.

## Repository and target

- Continue on `main`. Local checkpoints are ahead of `origin/main`; do not push. Check `git status --short --branch` before starting.
- The only valid target is the original Japanese TH10 v1.00a executable at `resources/th10.exe`, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`. Keep it read-only and uncommitted.
- `docs/PROGRESS.md` is generated from the canonical ledgers. Current totals: 1,556 exact functions / 214,824 codegen bytes; 640 exact functions / 144,495 bytes have confirmed authored origin. The complete inventory and authored denominator remain provisional.

## Earlier source checkpoint: typed supervisor cameras

`MainSupervisorView::cameraRecords[2]` is now an explicit pair of `MainCameraRecordView` records at +0x154 and +0x26C, each 0x118 bytes. Each record contains an 0xE4-byte `AnmViewportOwnerView` prefix, `unknown0E4`, screen-shake X/Y at +0xE8/+0xEC, and 0x28 trailing bytes. Slot 1 ends exactly at `activeViewport` +0x384.

The first slot is the published camera (0x491D7C); the second is the secondary camera (0x491E94). `MainConfigureGameplayViewport` and `EnemyPrepareProjection` index the records directly. ANM layer 3/14 use slot 0; layers 15/16/19 and `GameWindowView::Render` use slot 1.

All 15 complete affected owners compare exact after relinking the changed translation units:

- Main supervisor constructor/destructor, `SetNextGameMode`, `OnUpdate`, `ReleaseGame`, `StartModeThread`, and `LeaveLock` (71, 22, 37, 113, 293, 118 and 25 bytes).
- `GameWindowView::Render` (425), `MainConfigureGameplayViewport` (60), and `EnemyPrepareProjection` (60).
- ANM `DrawLayer3/14/15/16/19` (115, 48, 72, 72 and 72 bytes).

## Latest source checkpoint: typed Enemy ANM script state

`EnemyRuntimeView` runtime +0xE8..+0xF8 is now represented by `EnemyAnmScriptStateView`, a five-DWORD record with asserted size 0x14 and asserted offsets for selected resource, primary VM resource/script, base script and direction. The ECL dispatcher, `EnemyRuntimeUpdate` and `EnemySpawn` now share these semantic fields. The separate `StageCameraStateView::value0E8/value0EC` fields remain untouched; they are camera shake values, not Enemy ANM selectors.

Targeted linked exact replays after the change pass for `EnemyRuntimeUpdate` (2,412/2,412) and `EnemySpawn` (577/577). The primary 36-support-TU dispatcher remains nonexact: current local comparison is 14,452/14,416 bytes with 10,610 differing bytes, and its residual set has moved. Do not refresh the stored frontier to accept it.

## Exact owners to preserve

- `EnemyMotionView::AdvanceMotion` — 171/171 bytes, seven bound REL32/DIR32 fields; selected 17-source VC7.1 SP1 `/GL` cold replay passes.
- `EclVmScriptDatabase::AddScriptData` — 589/589 bytes; SCPT payload/table bases derive from `sizeof(EclVmScriptFileHeader)`.
- `PlayerHandleCollisionHit` 627/627; `EnemyApplyBulletClear` 44/44; `GuiSetLivesDisplayCount` 77/77.
- `EnemyRuntimeUpdate` 2,412/2,412 and `EnemySpawn` 577/577; targeted exact replays after the ANM state record migration remain byte-identical.
- ECL stack `Pop`, `Push`, `EnterFrame`, `LeaveFrame` — 122/122, 96/96, 64/64, 41/41.
- `Chain::AllocElem` 74/74 and `Chain::~Chain` 103/103.
- The 15 camera-related owners listed above are exact in their complete linked extents.

The unit definitions and relocation bindings are in `config/match-units.toml`; accepted exact rows are in `config/matches.csv`. A matching leaf does not make its larger caller exact.

## Current open owners

These are bounded local linked comparisons, not cold-replay receipts. `REGRESSED` means the residual offsets moved relative to the stored frontier; it does not necessarily mean the raw difference count increased. Do not update `config/frontiers.toml` just to bless moved offsets.

| Owner | Candidate / target | Differing bytes | Current state / focus |
| --- | ---: | ---: | --- |
| Enemy ECL dispatcher, primary 36-TU `/GL` graph | 14,452 / 14,416 | 10,610 | `REGRESSED`; typed pattern/VM access and firing-pattern receiver are retained. Continue with dispatch scheduling and the 36-TU graph. |
| Enemy ECL dispatcher, secondary 12-TU `/GS` graph | 14,456 / 14,416 | 10,240 | `REGRESSED`; diagnostic only, not a replacement for the primary graph. |
| `EclVmContext::Run` | 7,020 / 7,020 | 23 | `REGRESSED` because residual positions moved. The four stack leaves remain exact. |
| `AnmRenderManagerView::ExecuteScript` | 9,964 / 9,964 | 2 | Opcode 91 argument register: target EDX, candidate ECX at +0x112/+0x115. |
| `GameResetGui` | 1,965 / 1,965 | 47 | Lives-display helper remains exact at 77/77. |
| `TransitionShakeLinear` | 439 / 439 | 27 | Separate `TransitionShakeEnvelope` 349/349 is exact; naive RNG/division reordering regresses. |
| `BulletRuntimeView::UpdateAbsoluteDirectionChange` | 335 / 335 | 2 | Remaining angle argument register differs (target EDX, candidate EAX). |
| `ReplayPlaybackFrameControlCallback` | 68 / 68 | 4 | Branch destinations differ; FrontEnd options owner remains exact. |
| `Chain::Chain` | 59 / 59 | 8 | Keep separate from the exact allocator/destructor. |
| `EnemyFullObjectView::ReadFloatOperand` | 876 / 884 | 262 | Eight-byte extent short. |
| `InitializeGameStatisticsRecord` | 274 / 274 | 25 | Full extent; register allocation for zero/name/group loop remains. |
| `EnemyGameStateView::StartSpell` | 2,404 / 2,432 | 1,129 | Nonexact. |
| `GameManagerUpdateBody` | 1,538 / 1,544 | 1,022 | Nonexact. |
| `ItemManagerUpdateBody` | 2,220 / 2,320 | 1,639 | Nonexact. |

## Enemy/ECL decisions already in source

- `FIRE_BULLET_PATTERN` retains one `EnemyBulletPatternView*` across XYZ stores, the protection-radius check and `SpawnBulletPattern`. This reduced the primary dispatcher from 10,495 to 10,414 differing bytes and the secondary graph from 10,531 to 10,240; both residual sets moved and remain nonexact.
- Paired bullet sprite/offset/angle/count/speed/sound and rank-interpolation writes retain typed record pointers after the first store. Preserve unchecked modulo-32-bit script indexing and operand-read order; hoisting pointers earlier changes register lifetimes.
- `dispatch_copy_player_to_motion` uses a 12-byte `memcpy` to preserve NaN payloads and signed zero. Pattern initialization clears the complete 0x210-byte record before explicit defaults; pattern copy uses typed assignment because indices may be equal. Opcode 0x199 has 22 overlapping 0x18-byte extra records per pattern.
- Type-0/type-1 laser request layouts remain distinct (0x1DC/0x1F8). `EnemyManagedVmIdAt` retains unchecked 32-bit arithmetic for ten four-byte VM handles; only flag loops are bounded to 0..9.
- Enemy ANM script state is the asserted five-DWORD `EnemyAnmScriptStateView` at runtime +0xE8. Keep the selected resource separate from the primary VM resource/script pair and from the base script/direction consumed by runtime update.
- `EclVmScriptDatabase::AddScriptData` uses the verified 0x24-byte header size for both subroutine-table and include-payload bases.

See the Enemy, ECL VM, ANM and Main sections of [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) for the supporting offsets and failed experiments.

## Short validation loop

```sh
# Primary Enemy/ECL graph and its complete exact leaf guard
scripts/repo-python scripts/diagnostics/triage/quick-relink.py player-handle-collision-hit --dirty src/EnemyEclDispatcher.cpp
scripts/repo-python scripts/check-frontier.py --no-build enemy-dispatch-ecl ecl-run anm-execute-script game-reset-gui
scripts/repo-python scripts/compare-linked-function.py --unit player-handle-collision-hit --json

# Secondary Enemy graph
scripts/repo-python scripts/diagnostics/triage/quick-relink.py enemy-apply-bullet-clear --dirty src/EnemyEclDispatcher.cpp
scripts/repo-python scripts/check-frontier.py --no-build enemy-dispatch-ecl-12tu
scripts/repo-python scripts/compare-linked-function.py --unit enemy-apply-bullet-clear --json

# Enemy runtime and spawn state
scripts/repo-python scripts/replay-exact-units.py --unit enemy-runtime-update --unit enemy-spawn --json

# ECL VM
scripts/repo-python scripts/diagnostics/triage/quick-relink.py player-handle-collision-hit --dirty src/EclVm.cpp
scripts/repo-python scripts/check-frontier.py --no-build ecl-run
scripts/repo-python scripts/compare-linked-function.py --unit ecl-vm-stack-pop --json
```

Pass every changed translation unit after `--dirty`. Do not run the full cold replay for ordinary iterations. Keep commits local with the `gpt-web: ...` prefix and the required co-author trailer.

## Storage

See [RE_ARTIFACTS.md](RE_ARTIFACTS.md). Preserve `resources/`, `.tools/`, `ghidra-project/`, `_reference/`, `build/match-linked/`, `build/match/`, `build/probe-backlog/`, the five unresolved `build/gpt-web/` diagnostic workspaces, D3D evidence, the origin-review input and the private analyzer runtime.
