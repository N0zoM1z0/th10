# TH10 reconstruction — agent handoff

**Current as of 2026-11-14.** This is the working brief, not a chronological
experiment log. The detailed [knowledge journal](KNOWLEDGE_BASE.md) contains
older source-epoch observations, some of which were superseded. Read
[AGENTS.md](../AGENTS.md) for the target, toolchain, exactness and Git rules.

## State and first actions

- Start with `git status --short --branch` and `git diff`; take over any dirty
  or untracked work. Check `git log -8 --oneline`. Do not reset work or push.
- Validate the private **Japanese TH10 v1.00a** target with
  `scripts/repo-python scripts/verify-target.py`. Required SHA-256:
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
- Local exact function/binding truth is in `config/matches.csv`,
  `config/match-units.toml` and `config/functions.csv`. The generated
  [PROGRESS.md](PROGRESS.md) is the count authority
  (`scripts/repo-python scripts/progress.py --check`). Exact counts do not
  imply a closed, runnable reconstructed game.
- `_reference/th10-decomp` (upstream `levzzz5154/th10-decomp`, sparse
  checkout incl. `matching2/msvc/{src,include,build_support}`) has
  byte-exact MSVC C++ for most of our backlog. Treat it as hypotheses: port
  the natural spelling, never its synthetic LTCG contexts, escape functions,
  `volatile` reads or fake ABIs, and verify in our real graph.
- Use pinned VC7.1 SP1 and the particular unit's real `/GL`, `/GS`,
  support-object and PDB-owner recipe. Compare **complete functions**, all
  original-bound fields and local branches. A similar-size candidate,
  masked opcode resemblance or an exact caller is not an exact callee.
- Run **only selected** cold replay units affected by a source edit:
  `scripts/repo-python scripts/replay-exact-units.py --unit UNIT --json`.
  Never run the full historical replay. For docs-only work, no compile or
  replay is necessary. Commit useful checkpoints as `claude-opus-5.5-high: ...` (current
  operator request) with the
  author/trailer in `AGENTS.md`.

## Accepted exact owners and useful guards

These are highlights, **not** the full list of 610 authored exact owners.

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
| `EnemyEclHostBaseView::SetSubroutine @0x450700` / `enemy-ecl-host-set-subroutine` | **30/30**, one call field to the exact 136-byte `FindSubroutine`. Its enclosing `EnemyRuntimeUpdate` is separately **2,412/2,412 / 70 fields** in `enemy-runtime-update`. |
| `EclVmScriptDatabase::AddScriptData @0x450220` / `ecl-database-add-script-data` | **589/589**, 581 ordinary bytes plus `malloc @0x452706` and `free @0x452422` REL32 fields. Two independent five-input VC7.1 `/GL` cold replays; `EnemyEclResourceView::LoadFile` remains **95/95**. |
| `EnemyRuntimeVectorView::FromAngleMagnitude @0x44C5D0` / `enemy-runtime-vector` | **30/30** in the full Enemy runtime graph. |
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

## 2026-11-14 session results

- New exact owners: `CMemoryPbgFile::Seek` 107, `InvertAlpha` 157,
  `StageResetPublishedCameras` 465, `MainConfigurationDialogProc` 233,
  `SoundPlayerView::PreloadBgm` 369, `ScriptViewerBombOwnerView::Update` 223,
  `ResultMenuOpenPause` 382 (units `memory-pbgfile-seek`,
  `text-buffer-invert-alpha`, `stage-reset-published-cameras`,
  `main-configuration-dialog-proc`, `sound-preload-bgm`,
  `script-viewer-bomb-update`, `result-menu-open-pause`).
- **Stale exact claims at the session-start commit** (each reproduced from a
  clean worktree of `d69a5efc`, then repaired):
  `ProcessQueues` (`fe52288e` guard), `AllocateVm` (graph missing
  `AnmVmInitialize.cpp` after the 11-09 split), and `ResultMenuResume`
  (bisected to `3e9fca28`'s `ResultMenuVmIdStorage`). **Open risk:** other
  accepted units may be stale in the same ways (file splits leaving callees
  anchored, storage-type wrappers changing argument materialization). A
  grouped broad replay is the way to find them; replays stop at the first
  failure, so run one invocation per output directory.
- Comparator change: `linked_code_fields(..., code_limit)` stops instruction
  decoding at the unit's declared trailing data. Without it, decoding jump
  tables as instructions made `enemy-spawn` depend on its link address. Its
  span now declares the whole 53-byte selector table. Covered by
  `scripts/test-linked-data-coverage.py`.
- New candidate units (NONEXACT, partial linkages, for triage):
  `gui-message-run-candidate` (1,047/1,219 normalized instructions),
  `bullet-update-absolute-direction` (333/335), `transition-shake-linear`
  (late `fidiv`). `PollInputFrame` is 276/278 in `input-controller-buttons`.
- `ReplayManager::Initialize @0x428F60` is exact (1,344 / 74 fields) in
  `replay-manager-initialize` (`ClaudeReplayInit` graph = the
  `replay-manager-create` graph plus `FileSystem.cpp` `/GL /GS` and normal
  `Decompress.cpp`). Its last seam was a global *symbol* choice: writing
  `+0x5C` through a separate extern instead of `g_GameScoreState` moved a
  load. Watch for the same split-global pattern elsewhere; anchored
  addresses hide it. `LoadReplay @0x42A200` is 94/138 in that graph.
- `scripts/diagnostics/triage/quick-relink.py UNIT --dirty src/X.cpp`
  recompiles only the named sources and relinks (about 4 min instead of 15
  for big graphs). It is for triage only; acceptance still needs the cold
  replay. Never edit `config/match-units.toml` while a replay runs, because
  each unit build re-reads the manifest.
- Other leads: `PlayerHandleCollisionHit` (136/141 in a stale image),
  `ScriptViewerPopupOwnerView::Initialize` and its factory (LTCG receiver
  choice; see the knowledge base), `PollInputFrame` 276/278, GUI `Run`
  1,047/1,219, and the two `ResultTransitions` units that were already stale.
- Triage tools (never acceptance) in `scripts/diagnostics/triage/`:
  `idiff.py` aligns a linked candidate against the target with addresses
  masked; `target-fields.py` lists a target function's DIR32/REL32 fields;
  `linkages-from-candidate.py` drafts a unit's linkage list from a built
  candidate. To survey near misses cheaply, diff backlog functions that
  already sit inside existing `build/match-linked` images before compiling.
  Typical loop: add a unit with `linkages = []`, build it, run `idiff.py`,
  fix the source, then draft linkages and run `compare-linked-function.py`.
- Recurring lesson: most remaining seams were source *shape/type* errors
  that older sessions masked with `memcpy`, `volatile`, `_ReadWriteBarrier`
  or address-word arithmetic. Check graph completeness first (missing TUs
  turn private-ABI callees into anchored externals).
- Do not import the reference's synthetic contexts or `volatile`/fake-ABI
  spellings; `AnmVmInitialize(void *unused, AnmVmView *)` in our tree is an
  existing fake-ABI declaration worth replacing once its 13 consumers can be
  replayed.

## Open functions — choose a concrete hypothesis before compiling

All counts below are **selected graph snapshots**, not universal claims across
all LTCG contexts. Check current source and the original byte offsets first.

| Priority and owner | Current frontier / next actionable discrepancy |
| --- | --- |
| **ANM Boss** `ExecuteScript @0x43EE30`, `src/AnmManager.cpp` | Target/candidate **9,964**; **9,962 matching bytes and 350 bound fields**. Only opcode 91 `CreateVmVariant2`: target EDX vs candidate ECX at `+0x113/+0x115`. Record layouts, script/layer temporaries, neighboring opcodes, aggregate forms, resource-pointer lifetimes and three compiler barriers did not fix it. **Do not** replace `I_ADD/I_SUB/I_MUL` destination aliases with typed compound forms without checking: that attempt shrank the Boss to **9,948** bytes. The separate `I_SET_ADD/SUB/MUL` paths already have typed publication. |
| **ECL Boss** `EclVmContext::Run @0x44E1A0`, `src/EclVm.cpp` | Full **7,020**; **6,997/7,020 total bytes** match, including 189 bound fields. Of **6,264 non-field positions, 6,241 match**, leaving **23 ordinary differences** at early stack/register homes, x87 `LENGTH_SQUARED` / `POINT_ANGLE`, and `FORMAT`. `Stack::Pop` remains **122/122** exact. Recent Pop address forms, caller-side RETURN Pop expansion, SPAWN/STOP thread-local lifetimes, FORMAT declaration order and x87 local-order controls did not improve Run; the shared scratch-union experiment regressed and was reverted. `AddScriptData @0x450220` is **589/589 exact** in `ecl-database-add-script-data`: two independent five-input cold replays bind all 581 ordinary bytes plus `malloc @0x452706` and `free @0x452422`; `LoadFile` remains 95/95 exact. Continue the 23-byte Run frontier; do not reopen the exact database loader without new evidence. |
| **Result-menu Boss** `ResultMenuUpdateResult @0x4236F0`, `src/FrontEnd.cpp` | Target **3,227**, selected **3,228**; **784/907 normalized target instructions** and 173 candidate fields in four-TU `/GL /GS /EHs`. State-7 branch ownership, state-8 VM-ID and states 10–13 remain unresolved. The retained state-12 score-profile index uses native low-DWORD address arithmetic without changing 3,228-byte/173-field candidate geometry (`68f89dc7`). `ResultMenuPrepareScore @0x423570` is **377/377 extent** but **31 ordinary differences** around `+0xA4..+0xC5` cursor high-bound. Volatile clamp and six inline-wrapper alternatives shrank the score owner to 361/362 and changed the parent size/field map (3,156–3,264); do not install them. Score-sync28 remains exact. |
| **Enemy large owners** `EnemyRuntimeUpdate @0x40DC80`, `src/Enemy.cpp`; callback `src/EnemyCallback.cpp`; dispatcher `src/EnemyEclDispatcher.cpp` | `EnemyRuntimeUpdate` is exact: two independent pinned VC7.1 `/GL` cold replays reproduce **2,412/2,412 bytes / 70/70 target fields** in the 16-source graph. The ECL-result-derived eight-slot count retains the timer-tail recovery; direct manager calls preserve `MOV EBX,8` and both manager-loop lifetimes. The four-byte VM-ID overlay and 0x14-byte Enemy timer view remain part of the exact source. Player damage remains **1,170/1,170 / 28 fields** with nullable ECX hit-flag receiver and three stack arguments. `EnemyAdvanceMotion` remains **168/171** in natural C++ (**133/143 ordinary bytes, seven fields**); its remaining seam is the split outgoing x87 argument cleanup before the final Y store. `FromAngleMagnitude` is separately **30/30**. `EnemyResolveRuntimeCallback @0x4127A0` remains nonexact against its **593-byte** target. A full-graph baseline is **606 bytes / 58/565 ordinary bytes**; the best recent three-support diagnostic is **592 bytes / 104/565 / seven fields**, but it has not been revalidated in the 16-source graph. The maintained source remains the 606-byte baseline; the 592-byte candidate is diagnostic-only and was reverted. Rebuild the canonical graph before extending the callback. The ECL dispatcher remains **14,416 bytes / 800 source field identities**, nonexact. |
| **GUI and Player** `GuiMessageVmView::Run @0x415E90`, `src/Gui.cpp`; Player update `src/Player.cpp` | GUI target **4,480**, candidate **4,492**; preserve standalone 9-byte instruction-next leaf. Player callback target **3,112** vs candidate **3,100**; movement target **1,656** vs candidate **1,616**. Player damage `@0x428630` is now **1,170/1,170 / 28 fields** with the actual nullable ECX hit-flag receiver and three stack arguments; two independent canonical 17-TU `/GL` cold links pass. Same selected replay set passes auxiliary damage **202/202 / 6 fields**, shot update **876/876 / 22 fields**, and Enemy manager update **166/166 / 5 fields**. |
| **Near-exact Bullet** `UpdateAbsoluteDirectionChange @0x4078E0`, `src/BulletManager.cpp` | Full **335** with **two ordinary differences** at `+0xD7/+0xDD`: target uses EDX to load/push angle, candidate EAX. Eight fields remain bound. Ten natural angle/receiver/speed forms and three native cycle-store variants were neutral or regressed. The installed bit-copy cycle store preserves this baseline; a new EDX-liveness hypothesis is needed. Relative/aimed remain exact. |
| **Other medium owners** | `ScriptViewerBombOwnerView::Update @0x405750`: full **223**, ten nonfield register bytes, 12 fields; EBP/EBX owner/VM allocation. `MainRegisterChain @0x420470`: full **204**, **110/120 ordinary bytes**, 21 fields; nine natural owner/flag lifetime forms were neutral. |
| **Additional genuine backlog** | `InitializeGameStatisticsRecord @0x42ACB0` full **274**, **217/242 ordinary bytes**, eight fields after ten source-lifetime/name-copy controls; adjacent 423-byte loader is exact. `CreateStatistics42AF20 @0x42AF20` is **98 bytes** against a 97-byte target, 33/61 ordinary bytes and nine fields; seven natural `new`/return variants did not recover its private ABI. `ScriptViewerPopupOwnerView::Draw` is 537 bytes against 540, 211/480 ordinary bytes and 15 fields; its ten-byte adapter is 3/6 with one field. `TransitionShakeEnvelope @0x43C710` target **349**, candidate **350**, 207/285 comparable and 16 fields. `PollInputFrame @0x44A5F0` target/candidate **850**, repeat/cursor register and callee ABI scheduling. Check each complete source and graph before adopting a historic score. |

`EntityStartScript_00404F30` is **not** an accepted exact reconstruction:
its 202-byte reference source still uses `__declspec(naked)`/hand assembly.
A 194/194 comparable-COFF match is insufficient under the natural-source
rule. Recover the private ABI with ordinary C/C++ before considering credit.

**Comparison caveat:** `result-menu-update` and `enemy-spawn` now declare
their complete trailing selector tables as data spans (see the comparator
`code_limit` note above). `result-menu-open-game-over` and
`game-complete-stage-transition` (`ResultTransitions` graph) already failed
their field checks at `d69a5efc`; they still need repair.

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
