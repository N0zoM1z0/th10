# TH10 reconstruction — agent handoff

**Current as of 2026-11-12.** This is the working brief, not a chronological
experiment log. The detailed [knowledge journal](KNOWLEDGE_BASE.md) contains
older source-epoch observations, some of which were superseded. Read
[AGENTS.md](../AGENTS.md) for the target, toolchain, exactness and Git rules.

## State and first actions

- Start with `git status --short --branch` and `git diff`; take over any dirty
  or untracked work. Check `git log -8 --oneline`. Do not reset work or push.
- Validate the private **Japanese TH10 v1.00a** target with
  `scripts/repo-python scripts/verify-target.py`. Required SHA-256:
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
- Accepted function/binding truth is in `config/matches.csv`,
  `config/match-units.toml` and `config/functions.csv`. The generated
  [PROGRESS.md](PROGRESS.md) is current (`scripts/repo-python scripts/progress.py --check`).
  **1,521 exact functions / 200,940 bytes**; of these, **608 / 130,653**
  have authored source; **142 authored** functions remain nonexact. These
  counts do not imply a closed, runnable reconstructed game.
- Use pinned VC7.1 SP1 and the particular unit's real `/GL`, `/GS`,
  support-object and PDB-owner recipe. Compare **complete functions**, all
  original-bound fields and local branches. A similar-size candidate,
  masked opcode resemblance or an exact caller is not an exact callee.
- Run **only selected** cold replay units affected by a source edit:
  `scripts/repo-python scripts/replay-exact-units.py --unit UNIT --json`.
  Never run the full historical replay. For docs-only work, no compile or
  replay is necessary. Commit useful checkpoints as `gpt-web: ...` with the
  author/trailer in `AGENTS.md`.

## Accepted exact owners and useful guards

These are highlights, **not** the full list of 608 authored exact owners.

| Exact owner / canonical unit | Complete original-bound scope |
| --- | --- |
| `PlayerUpdateShots @0x428280` / `player-update-shots` | **876/876**, 22 fields in the real 12-input Enemy/Player graph. Branch-local motion `LEA` timing and direct slow timer x87 addition are already solved. The 717-byte spawn and 91-byte bounds functions also pass. |
| `Player::CalculateDamageToTarget @0x428630` / `player-calculate-damage` | **1,170/1,170**, 28 fields. Signed DWORD accumulator updates were made type-correct without changing codegen. |
| `AnmRenderManagerView::SetupVertexBuffer @0x4462F0` / `anm-setup-vertex-buffer` | **472/472**, 22 fields. Eight UV IEEE-754 words now copy via `memcpy`, not incompatible integer lvalues. Strict `ExecuteScript` parent is **not** exact. |
| `GameWindowView::ResetRenderState @0x439D20` / `main-reset-render-state` | **710/710**, 34 COFF relocations. Three floating-point render-state values now publish their bit patterns with `memcpy`. |
| `TransitionFadeIn @0x43C230` / `transition-fade-in` | **133/133**, five fields; native unsigned frame-bound + target VC7.1 signed conversion. `FadeOut` 93 and `Dim` 175 are exact; `ShakeEnvelope` is not. |
| `ResultMenuSynchronizeScore @0x421F60` / `result-menu-score-synchronize` | **28/28**, three fields. This does **not** make the 377-byte score preparation or 3,227-byte result state machine exact. |
| `ReleaseModeOwners @0x4203F0` / `main-release-mode-owners` | **128/128**, 15 fields; `MainSupervisorView::ReleaseGame` 293/293 also passes its own unit. |
| `EnemyEclHostBaseView::SetSubroutine @0x450700` / `enemy-ecl-host-set-subroutine` | **30/30**, one call field to the exact 136-byte `FindSubroutine`. The 2,412-byte Enemy update remains nonexact. |
| `ScriptViewerBombOwnerView::EmitAuxiliary @0x405AC0` / `script-viewer-bomb-emit-auxiliary` | **124/124**, six fields. The 223-byte Bomb update caller is still nonexact. |
| `BulletManagerView::DrawBucket @0x4066E0` / `bullet-manager-draw-bucket` | **134/134**, five fields. Bullet deceleration164/vector305/polar209/relative338/aimed431/state8-306 and pool-update249 are separately exact after native timer fixes. |
| `EnemyReadDialog @0x415DB0` / `enemy-read-dialog` | **124/124**, nine fields in the real EH graph. |

Additional accepted larger guards: `GuiView::UpdateStageElements` **3,836**
bytes/72 fields (native modulo-32 ending counter, commit `3f227ec3`),
`ReplayManager::SaveReplay` **1,681** bytes/83 fields (native DWORD payload
accumulators, `6edca467`), and `EndingTextVmView::Run` **1,568** bytes.
The score loader **423**, finalizer **564**, and tail initializer **142**
remain exact after byte-safe record publications (`e90a06bc`, `c43747ba`).
Rebuild only if a change touches their dependency graph.

## Open functions — choose a concrete hypothesis before compiling

All counts below are **selected graph snapshots**, not universal claims across
all LTCG contexts. Check current source and the original byte offsets first.

| Priority and owner | Current frontier / next actionable discrepancy |
| --- | --- |
| **ANM Boss** `ExecuteScript @0x43EE30`, `src/AnmManager.cpp` | Target/candidate **9,964**; **9,962 matching bytes and 350 bound fields**. Only opcode 91 `CreateVmVariant2`: target EDX vs candidate ECX at `+0x113/+0x115`. Record layouts, script/layer temporaries, neighboring opcodes and aggregate forms did not fix it. **Do not** replace `I_ADD/I_SUB/I_MUL` destination aliases with typed compound forms without checking: that attempt shrank the Boss to **9,948** bytes. The separate `I_SET_ADD/SUB/MUL` paths already have typed publication. |
| **ECL Boss** `EclVmContext::Run @0x44E1A0`, `src/EclVm.cpp` | Full **7,020**, **6,997 matching ordinary bytes**, 189 fields; **23** remaining differences at early stack/register homes, x87 `LENGTH_SQUARED` / `POINT_ANGLE`, `FORMAT`. Natural stack-`Pop` arithmetic forms preserved the exact 122-byte leaf but **regressed the whole runner**; do not install on leaf evidence alone. `AddScriptData @0x450220` is separately **589 bytes, 79 differences**, two candidate fields. |
| **Result-menu Boss** `ResultMenuUpdateResult @0x4236F0`, `src/FrontEnd.cpp` | Target **3,227**, selected **3,228**; **784/907 normalized target instructions** and 173 candidate fields in four-TU `/GL /GS /EHs`. State-7 branch ownership, state-8 VM-ID and states 10–13 remain unresolved. `ResultMenuPrepareScore @0x423570` is **377/377 extent** but **31 ordinary differences** around `+0xA4..+0xC5` cursor high-bound; score-sync28 is exact. Avoid old 372-byte candidates. |
| **Enemy large owners** `EnemyRuntimeUpdate @0x40DC80`, `src/Enemy.cpp`; dispatcher `src/EnemyEclDispatcher.cpp` | Runtime is **2,412 bytes, 69 fields**, 1,575/2,136 comparable matched in a selected ten-TU graph. Life/flash unsigned-DWORD `memcpy` writes did not change codegen. ECL dispatcher has complete **14,416-byte** extent and **800** source field identities, still nonexact; unresolved register/ABI schedules, not merely type-alias cleanup. |
| **GUI and Player** `GuiMessageVmView::Run @0x415E90`, `src/Gui.cpp`; Player update `src/Player.cpp` | GUI target **4,480**, candidate **4,492**; preserve standalone 9-byte instruction-next leaf. Player callback target **3,112** vs candidate **3,100**; movement target **1,656** vs candidate **1,616**. Recent native 32-bit option/movement writes are codegen-neutral. The shot loop is already exact. |
| **Near-exact Bullet** `UpdateAbsoluteDirectionChange @0x4078E0`, `src/BulletManager.cpp` | Full **335** with **two ordinary differences** at `+0xD7/+0xDD`: target uses EDX to load/push angle, candidate EAX. Eight fields remain bound. Do not confuse the **301/303 unmasked** comparison with a 301/335 failure. Relative/aimed are already exact. |
| **Near-exact Sound** `PreloadBgm @0x43D7D0`, `src/Sound.cpp` | Full **369**, **four ordinary differences** `+0x121/+0x127/+0x12A/+0x12B` in `ReadFile` size/output register assignment; ten fields. Prior pointer/reference/volatile trials did not fix the register coloring. |
| **Other medium owners** | `ScriptViewerBombOwnerView::Update @0x405750`: full **223**, nine nonfield register bytes, 12 fields; EBP/EBX owner/VM allocation. `MainRegisterChain @0x420470`: full **204**, ten EBX/EBP register bytes, 21 fields. `CMemoryPbgFile::Seek @0x4364F0`: full **107**, seven register bytes, no fields. No known safe exact variant yet. |
| **Additional genuine backlog** | `InitializeGameStatisticsRecord @0x42ACB0` full **274**, 25 ordinary differences/eight fields; `TransitionShakeEnvelope @0x43C710` target **349**, candidate **350**, 207/285 comparable, 16 fields; `TextRenderBufferView::InvertAlpha @0x437160` full **157**, register frontier; `PollInputFrame @0x44A5F0` target/candidate **850**, repeat/cursor register and callee ABI scheduling. Check each complete source and graph before adopting a historic score. |

`EntityStartScript_00404F30` is **not** an accepted exact reconstruction:
its 202-byte reference source still uses `__declspec(naked)`/hand assembly.
A 194/194 comparable-COFF match is insufficient under the natural-source
rule. Recover the private ABI with ordinary C/C++ before considering credit.

**Comparison caveat:** The legacy `result-menu-update` unit's one-byte
`data_spans` conflicts with its current decoder contract; do not silently
relax the physical-owner check. A separate 15-input
`result-menu-open-game-over` field drift predated the later FrontEnd DWORD
changes and does not invalidate the stricter canonical graph.

## Efficient continuation and private inputs

Start with one large and one medium/leaf owner whose **remaining offset**
has a plausible source/ABI explanation. Use few natural C/C++ hypotheses,
real graph/PDB provenance and exact-target checks before editing the ledger.
Keep only codegen-equivalent source changes that repair actual semantics;
do not promote unchanged NONEXACT candidates or repeatedly re-probe known
neutral register-control variants.

Preserve the real executable, `.tools/` (especially the game Wine prefix),
`ghidra-project/`, `_reference/`, the canonical source/ledger/tool scripts,
**all of `build/match-linked/`** and unrelated `.analysis/d3d-*` work. The
five narrow private diagnostic contexts and four optional trial drivers are
listed in [RE_ARTIFACTS.md](RE_ARTIFACTS.md). Historical `build/gpt-web`
variants and `.analysis/gpt-web` source snapshots are disposable: a dated
knowledge-journal path is not a promise the file still exists. Recreate only
an affected source/link graph when needed; **no full cold replay**.
