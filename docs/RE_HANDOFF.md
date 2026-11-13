# TH10 reconstruction — agent handoff

**Current as of 2026-11-13.** This is the working brief, not a chronological
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
  **1,522 exact functions / 200,968 bytes**; of these, **608 / 130,653**
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
| `PlayerUpdateShots @0x428280` / `player-update-shots` | **876/876**, 22 fields in the real 12-input Enemy/Player graph. Branch-local motion `LEA` timing and direct slow timer x87 addition are solved. `SpawnShot` is **717/717 / 17 fields**; the 91-byte bounds function also passes. Recent native slot/script fixes are `e60fa9ab`, `d01d1a90`, `58989c8f`. |
| `Player::CalculateDamageToTarget @0x428630` / `player-calculate-damage` | **1,170/1,170**, 28 fields. Hit-count, score, shot-damage and effect-damage DWORD publications are codegen-neutral (`e6d1f733`, `be58bce9`, `12099ae8`, `d0657fd9`). |
| `AnmRenderManagerView::SetupVertexBuffer @0x4462F0` / `anm-setup-vertex-buffer` | **472/472**, 22 fields. Eight UV IEEE-754 words now copy via `memcpy`, not incompatible integer lvalues. Strict `ExecuteScript` parent is **not** exact. |
| `GameWindowView::ResetRenderState @0x439D20` / `main-reset-render-state` | **710/710**, 34 COFF relocations. Three floating-point render-state values now publish their bit patterns with `memcpy`. |
| `TransitionFadeIn @0x43C230` / `transition-fade-in` | **133/133**, five fields; native unsigned frame-bound + target VC7.1 signed conversion. `FadeOut` 93 and `Dim` 175 are exact; `ShakeEnvelope` is not. |
| `ResultMenuSynchronizeScore @0x421F60` / `result-menu-score-synchronize` | **28/28**, three fields. This does **not** make the 377-byte score preparation or 3,227-byte result state machine exact. |
| `ResultMenuMarkTasks422200 @0x422200` / `result-menu-mark-tasks` | **28/28**, zero fields, two independent five-source VC7.1 cold links after restoring direct owner-field reloads (`89422b95`, `7f0958d6`). The target's private EAX register emerges naturally; original source authorship remains indeterminate. |
| `ReleaseModeOwners @0x4203F0` / `main-release-mode-owners` | **128/128**, 15 fields; `MainSupervisorView::ReleaseGame` 293/293 also passes its own unit. |
| `EnemyEclHostBaseView::SetSubroutine @0x450700` / `enemy-ecl-host-set-subroutine` | **30/30**, one call field to the exact 136-byte `FindSubroutine`. The 2,412-byte Enemy update remains nonexact. |
| `ScriptViewerBombOwnerView::EmitAuxiliary @0x405AC0` / `script-viewer-bomb-emit-auxiliary` | **124/124**, six fields. The 223-byte Bomb update caller is still nonexact. |
| `BulletManagerView::DrawBucket @0x4066E0` / `bullet-manager-draw-bucket` | **134/134**, five fields. Bullet deceleration164/vector305/polar209/relative338/aimed431/state8-306 and pool-update249 are separately exact after native timer fixes. |
| `ReplayManager::SaveReplay @0x429B60` / `replay-manager-save` | **1,681/1,681**, 83 fields. All USER text appends, NUL advances and both padding cursors now use native DWORD address arithmetic (`ff0cafd5`, `aa773a7d`, `943698c8`, `f9b25210`); each checkpoint passed the complete selected cold replay. |
| `GameScoreStateView::AddFaith/DecayFaith/AddRank` | **46/46 + 46/46 + 42/42**, no fields. Faith ADD/SUB results publish through their signed words before the original clamps (`b9540161`). |
| `FileSystem::LoadArchive/FindArchive/ArchiveOrDiskPathView::Read` | **36/36 / 4 fields**, **212/212 / 7 fields**, **214/214 / 14 fields**. Native archive-count publication preserves all three owners (`f709e891` plus earlier exact guards). |
| `EnemyReadDialog @0x415DB0` / `enemy-read-dialog` | **124/124**, nine fields in the real EH graph. |

Additional accepted larger guards: `GuiView::UpdateStageElements` **3,836**
bytes/72 fields (native modulo-32 ending counter, commit `3f227ec3`),
`ReplayManager::SaveReplay` **1,681** bytes/83 fields (native DWORD payload
accumulators and complete USER text/padding cursor publication, `6edca467`,
`ff0cafd5`–`f9b25210`), and `EndingTextVmView::Run` **1,568** bytes/92 fields
(including native text-row and animation-bank increments, `61e1fcd0`).
`Player::CalculateDamageToTarget` remains **1,170** bytes/28 fields exact
with native field-damage and impact-script additions (`32adecb4`, `bf9b62e4`).
The score loader **423**, finalizer **564**, and tail initializer **142**
remain exact after byte-safe record publications (`e90a06bc`, `c43747ba`).
More recent complete Boss guards: Player damage **1,170/28** after hit-count,
score, shot-hit and effect-total DWORD bit copies (`e6d1f733`, `be58bce9`,
`12099ae8`, `d0657fd9`); ANM
`ExecuteScript` still **9,962/9,964 / 350 fields**, with timer DWORD
publication codegen-neutral (`4320ebe6`), and GUI HUD cold-replay exact
**3,836/72** in the same updated source epoch. Rebuild only if a change
touches their dependency graph.

## Open functions — choose a concrete hypothesis before compiling

All counts below are **selected graph snapshots**, not universal claims across
all LTCG contexts. Check current source and the original byte offsets first.

| Priority and owner | Current frontier / next actionable discrepancy |
| --- | --- |
| **ANM Boss** `ExecuteScript @0x43EE30`, `src/AnmManager.cpp` | Target/candidate **9,964**; **9,962 matching bytes and 350 bound fields**. Only opcode 91 `CreateVmVariant2`: target EDX vs candidate ECX at `+0x113/+0x115`. Record layouts, script/layer temporaries, neighboring opcodes, aggregate forms, resource-pointer lifetimes and three compiler barriers did not fix it. **Do not** replace `I_ADD/I_SUB/I_MUL` destination aliases with typed compound forms without checking: that attempt shrank the Boss to **9,948** bytes. The separate `I_SET_ADD/SUB/MUL` paths already have typed publication. |
| **ECL Boss** `EclVmContext::Run @0x44E1A0`, `src/EclVm.cpp` | Full **7,020**; **6,997/7,020 total bytes** match, including the 189 bound fields. Of **6,264 non-field positions, 6,241 match**, leaving **23 ordinary differences** at early stack/register homes, x87 `LENGTH_SQUARED` / `POINT_ANGLE`, and `FORMAT`. The retained native `EclVmScalar` postdecrement publication preserves all original candidate bytes; direct `memcpy` regresses by one byte. Nine declaration/scope/result-type and ten RETURN branch parameter/last-test controls were neutral or distorted the body and did not repair x87 homes. `AddScriptData @0x450220` is separately **589 bytes, 79 differences**, two candidate fields. |
| **Result-menu Boss** `ResultMenuUpdateResult @0x4236F0`, `src/FrontEnd.cpp` | Target **3,227**, selected **3,228**; **784/907 normalized target instructions** and 173 candidate fields in four-TU `/GL /GS /EHs`. State-7 branch ownership, state-8 VM-ID and states 10–13 remain unresolved. The retained state-12 score-profile index uses native low-DWORD address arithmetic without changing 3,228-byte/173-field candidate geometry (`68f89dc7`). `ResultMenuPrepareScore @0x423570` is **377/377 extent** but **31 ordinary differences** around `+0xA4..+0xC5` cursor high-bound. Volatile clamp and six inline-wrapper alternatives shrank the score owner to 361/362 and changed the parent size/field map (3,156–3,264); do not install them. Score-sync28 remains exact. |
| **Enemy large owners** `EnemyRuntimeUpdate @0x40DC80`, `src/Enemy.cpp`; dispatcher `src/EnemyEclDispatcher.cpp` | Runtime remains **2,412 bytes, 69 fields**, 1,575/2,136 comparable. Its damage call now matches the target ABI: nullable optional-hit pointer in ECX, then Player/position/size on the stack; the full Player damage owner is **1,170/1,170 / 28 fields**. The first runtime seam is target `MOV EBX,8` at `+0x49D` before the damage branch; the candidate initializes the loop count in stack memory inside the later loop arms. Moving the declaration was neutral; a single conditional loop shrank the owner to 2,383 bytes and 1,124/2,136, so it was rejected. The shared `EnemyAdvanceMotion @0x44C2A0` is modeled as `EnemyMotionView::AdvanceMotion()`; the real graph restores its private ESI receiver. The Player shot/Enemy manager/death-finalizer guards pass (**876/876**, **166/166**, **162/162**). The motion leaf remains **168/171**; y-temporary controls were neutral, a two-axis loop regressed to 172 bytes / 11/155, and an inline floor helper retained 168 bytes but fell to 131/143 comparable. The `register` loop-counter hint was also neutral. ECL dispatcher has complete **14,416-byte** extent and **800** source field identities, still nonexact. |
| **GUI and Player** `GuiMessageVmView::Run @0x415E90`, `src/Gui.cpp`; Player update `src/Player.cpp` | GUI target **4,480**, candidate **4,492**; preserve standalone 9-byte instruction-next leaf. Player callback target **3,112** vs candidate **3,100**; movement target **1,656** vs candidate **1,616**. Player damage `@0x428630` is now **1,170/1,170 / 28 fields** with the actual nullable ECX hit-flag receiver and three stack arguments; two independent canonical 17-TU `/GL` cold links pass. Same selected replay set passes auxiliary damage **202/202 / 6 fields**, shot update **876/876 / 22 fields**, and Enemy manager update **166/166 / 5 fields**. |
| **Near-exact Bullet** `UpdateAbsoluteDirectionChange @0x4078E0`, `src/BulletManager.cpp` | Full **335** with **two ordinary differences** at `+0xD7/+0xDD`: target uses EDX to load/push angle, candidate EAX. Eight fields remain bound. Ten natural angle/receiver/speed forms and three native cycle-store variants were neutral or regressed. The installed bit-copy cycle store preserves this baseline; a new EDX-liveness hypothesis is needed. Relative/aimed remain exact. |
| **Near-exact Sound** `PreloadBgm @0x43D7D0`, `src/Sound.cpp` | Full **369**, **four ordinary differences** `+0x121/+0x127/+0x12A/+0x12B` in `ReadFile` size/output register assignment; ten fields. Eight fresh full-owner Sound/ZWave `ReadFile` declaration/address trials were neutral or regressed; seek genuine caller-visible register lifetimes rather than repeated pointer spelling. |
| **Other medium owners** | `ScriptViewerBombOwnerView::Update @0x405750`: full **223**, ten nonfield register bytes, 12 fields; EBP/EBX owner/VM allocation. `MainRegisterChain @0x420470`: full **204**, **110/120 ordinary bytes**, 21 fields; nine natural owner/flag lifetime forms were neutral. `CMemoryPbgFile::Seek @0x4364F0`: full **107**, **100/107**, no fields; six switch-case source orders preserve the same seven mismatches. `EnemyAdvanceMotion @0x44C2A0`: target 171, current candidate 168; the final x87 store/stack-cleanup order differs. |
| **Additional genuine backlog** | `InitializeGameStatisticsRecord @0x42ACB0` full **274**, **217/242 ordinary bytes**, eight fields after ten source-lifetime/name-copy controls; adjacent 423-byte loader is exact. `CreateStatistics42AF20 @0x42AF20` is **98 bytes** against a 97-byte target, 33/61 ordinary bytes and nine fields; seven natural `new`/return variants did not recover its private ABI. `ScriptViewerPopupOwnerView::Draw` is 537 bytes against 540, 211/480 ordinary bytes and 15 fields; its ten-byte adapter is 3/6 with one field. `TransitionShakeEnvelope @0x43C710` target **349**, candidate **350**, 207/285 comparable and 16 fields. `TextRenderBufferView::InvertAlpha @0x437160` full **157**, 138/157 across real graphs; noinline is neutral and exported ABI regresses to 112/157. `PollInputFrame @0x44A5F0` target/candidate **850**, repeat/cursor register and callee ABI scheduling. Check each complete source and graph before adopting a historic score. |

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
