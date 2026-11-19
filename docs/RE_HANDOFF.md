# TH10 reconstruction — next-agent handoff

**Updated 2026-11-19.** Start with [AGENTS.md](../AGENTS.md), then this brief and the relevant sections of [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md). Keep work local; do not push.

## Repository checkpoint

- Continue from the local `main` HEAD; the branch has local-only checkpoints and must not be pushed.
- On 2026-11-19, the natural Enemy motion source was restored to a complete
  171/171-byte, seven-field exact linked PDB contribution using a shared
  inlined quantization tail with a volatile post-write X reload. Its targeted
  `enemy-advance-motion` cold replay passes; the `EnemyRuntimeUpdate` (2,412),
  `PlayerUpdateShots` (876), `EnemyApplyBulletClear` (44), and
  `PlayerHandleCollisionHit` (627) strict checks are unchanged in their
  affected graphs.
- The enemy ECL dispatcher now carries typed `AnmVmView*` receivers in three
  child-animation paths; both 36-TU and 12-TU graph sizes and mismatch counts
  remain unchanged. It is not yet an exact owner.
- The only valid target is the original Japanese TH10 v1.00a executable,
  `resources/th10.exe`, SHA-256
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
  Keep it read-only and uncommitted. Use the scoped target and compiler checks
  described in `AGENTS.md` rather than rerunning setup checks every iteration.

The generated [PROGRESS.md](PROGRESS.md) is the count authority. The current
canonical exact total is 1,556 functions / 214,824 codegen bytes, of which
640 functions / 144,495 bytes have confirmed authored origin. The provisional
inventory and authorship denominator remain open.

## Exact owners to preserve

Strict linked-function checks currently pass for:

- `EnemyMotionView::AdvanceMotion` — 171 bytes, seven bound REL32/DIR32 operands; selected 17-source cold exact on 2026-11-19.
- `PlayerHandleCollisionHit` — 627 bytes.
- `EnemyApplyBulletClear` — 44 bytes.
- `GuiSetLivesDisplayCount` — 77 bytes.
- ECL stack `Pop`, `Push`, `EnterFrame`, `LeaveFrame` — 122, 96, 64 and 41 bytes.
- `Chain::AllocElem` — 74 bytes; `Chain::~Chain` — 103 bytes.

The authoritative unit definitions and linkage fields are in `config/match-units.toml` and accepted exact rows in `config/matches.csv`. Preserve full extents and relocations; a matching leaf does not make its larger caller exact.

## Current open owners

All measurements below are the latest bounded local linked comparisons. They are not canonical replay receipts.

| Owner | Current candidate / target | Current differences | State / next focus |
| --- | ---: | ---: | --- |
| `EnemyRuntimeView::DispatchEclInstruction` (36-TU graph) | 14,468 / 14,416 | 10,495 bytes | `REGRESSED` against the older recorded residual offsets. Keep the current typed pattern/VM access and bulk-copy semantics; attack the remaining scheduling and extent surplus without refreshing the frontier to bless moved offsets. |
| Same dispatcher (12-TU diagnostic graph) | 14,472 / 14,416 | 10,531 bytes | Also `REGRESSED`; this graph is a useful secondary check, not a replacement for the 36-TU owner. |
| `EclVmContext::Run` | 7,020 / 7,020 | 23 bytes | Full extent, but residual offsets moved from the recorded set. Focus operand/stack homes and thread spawn/stop register scheduling. The four stack leaves above remain exact. |
| `AnmRenderManagerView::ExecuteScript` | 9,964 / 9,964 | 2 bytes | Residuals at +0x112/+0x115; opcode 91 child-layer argument is the key register-allocation issue (target EDX, candidate ECX). |
| `GameResetGui` | 1,965 / 1,965 | 47 bytes | Full extent. Its separate lives-display helper remains 77/77 exact. |
| `TransitionShakeLinear` | 439 / 439 | 27 bytes | Preserve the separate exact transition-envelope owner; naive RNG/division reordering regresses this function. |
| `BulletRuntimeView::UpdateAbsoluteDirectionChange` | 335 / 335 | 2 bytes | Remaining difference is the angle argument register (target EDX versus candidate EAX). |
| `ReplayPlaybackFrameControlCallback` | 68 / 68 | 4 bytes | Branch destinations differ; preserve the exact 3,999-byte FrontEnd options owner. |
| `Chain::Chain` | 59 / 59 | 8 bytes | Constructor remains nonexact; keep the separately exact allocator. |

### Enemy ECL source decisions already retained

- `dispatch_copy_player_to_motion` copies the 12-byte XYZ record with `memcpy`. This preserves NaN payloads and signed zero; the earlier bounded 36-TU comparison improved by 27 differing bytes.
- `ENEMY_ECL_INITIALIZE_BULLET_PATTERN` clears the complete 0x210-byte record before setting opcode defaults. A temporary `EnemyBulletPatternView()` assignment expanded the owner and caused MSVC switch-label initialization errors; the retained form is `memset` plus explicit fields.
- `ENEMY_ECL_COPY_BULLET_PATTERN` uses complete typed record assignment, not `memcpy`, because source and destination script indices may be equal (self-copy).
- Type-0/type-1 laser request paths copy the serialized pattern words into their request records with fixed-size `memcpy`; keep the request layouts and offsets in `src/EnemyLaser.hpp`.
- `EnemyManagedVmIdAt` expresses the ten four-byte ANM VM handles with the target's unchecked modulo-32-bit index arithmetic. Script-supplied indices remain unchecked; the flag loops are bounded to slots 0..9.
- The SET_ANM_SCRIPT, SET_MAIN_ANM_SCRIPT and PLAY_SELECTED_ANM paths now keep the resolved animation VM as a typed `AnmVmView*` across shared sprite-bound calculations, instead of treating a pointer as an integer. Both current 36-TU and 12-TU dispatcher sizes/residual counts remain unchanged, and the exact collision/bullet-clear owners are preserved.

Detailed offsets and earlier compiler experiments are in the Enemy, ECL VM, ANM and Chain sections of [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md). Do not promote the 36-TU or 12-TU dispatcher frontier to exact.

## Short validation loop

Use only the affected linked graph and pass every changed translation unit after `--dirty`:

```sh
# Main Enemy/ECL graph
scripts/repo-python scripts/diagnostics/triage/quick-relink.py player-handle-collision-hit --dirty src/EnemyEclDispatcher.cpp
scripts/repo-python scripts/check-frontier.py --no-build enemy-dispatch-ecl ecl-run anm-execute-script game-reset-gui
scripts/repo-python scripts/compare-linked-function.py --unit player-handle-collision-hit --json

# Secondary Enemy dispatcher graph and its exact leaf guard
scripts/repo-python scripts/diagnostics/triage/quick-relink.py enemy-apply-bullet-clear --dirty src/EnemyEclDispatcher.cpp
scripts/repo-python scripts/check-frontier.py --no-build enemy-dispatch-ecl-12tu
scripts/repo-python scripts/compare-linked-function.py --unit enemy-apply-bullet-clear --json

# ECL VM changes: include EclVm.cpp in --dirty and inspect the full Run owner
scripts/repo-python scripts/diagnostics/triage/quick-relink.py player-handle-collision-hit --dirty src/EclVm.cpp
scripts/repo-python scripts/compare-linked-function.py --unit ecl-vm-stack-pop --json
```

Do not run the full cold replay for ordinary iterations. Do not edit `config/frontiers.toml` to turn a moved residual set into an `OK` result. Keep commits local with the `gpt-web: ...` prefix and the repository's required co-author trailer.
