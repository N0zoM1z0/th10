# TH10 reconstruction — agent handoff

**Current as of 2026-11-15.** This is the working brief. Durable findings,
codegen lessons and the per-owner open frontiers are in the
[knowledge base](KNOWLEDGE_BASE.md). Read [AGENTS.md](../AGENTS.md) for the
target, toolchain, exactness and Git rules.

## State and first actions

- Start with `git status --short --branch`, `git diff` and `git log -8
  --oneline`; take over any dirty or untracked work. Do not reset or push.
- Validate the private target: `scripts/repo-python scripts/verify-target.py`.
- Exact truth is `config/matches.csv`, `config/match-units.toml` and
  `config/functions.csv`; [PROGRESS.md](PROGRESS.md) is the generated count
  authority (`scripts/repo-python scripts/progress.py --check`).
- `_reference/th10-decomp` (sparse checkout including
  `matching2/msvc/{src,include,build_support}`) has natural MSVC C++ for most of
  the backlog. Port its natural spelling, never its synthetic LTCG contexts,
  escape functions, `volatile` reads or fake ABIs, and verify in our graph.
- Commit checkpoints as `claude-opus-5.5-high: ...` (current operator request)
  with the author and trailer from `AGENTS.md`.

## Working loop

1. Pick an owner from the knowledge base's **Open owners** list and read its
   source and target bytes.
2. Clone an existing unit whose graph contains the owner's TU, with
   `linkages = []`, and build it:
   `scripts/repo-python scripts/build-match-unit.py --unit NAME`.
3. Align candidate and target with
   `scripts/repo-python scripts/diagnostics/triage/idiff.py IMAGE SYMBOL ADDR SIZE`.
   For big graphs, `scripts/diagnostics/triage/quick-relink.py UNIT --dirty
   src/X.cpp` recompiles only the named sources (about 4 minutes instead of
   15). Both are triage only.
4. When the body matches, draft fields with
   `scripts/diagnostics/triage/linkages-from-candidate.py IMAGE SYMBOL ADDR SIZE`,
   canonicalize aliased globals to their semantic base plus addend, and run
   `scripts/repo-python scripts/compare-linked-function.py --unit NAME`.
5. Cold-replay the new unit and the exact units in every output directory
   whose graph compiles the changed sources:
   `scripts/repo-python scripts/replay-exact-units.py --unit A --unit B --json`.
   Replays stop at the first failure, so group by output directory. Never edit
   the manifest while a replay runs.
6. Update `config/matches.csv` and `config/functions.csv`, run
   `scripts/repo-python scripts/progress.py`, and commit.

The most complete graph is `build/match-linked/ClaudeReplayInitLoadScript`
(the ReplayInit graph plus `AnmLoadedStartScript.cpp`, entry
`AnmLoadedView::InitializeVm`). Clone `player-handle-collision-hit` for new
Player/Enemy/FrontEnd/Chain candidates. Without `AnmLoadedStartScript.cpp`,
`LoadScript_0043e710` is anchored and every `StartAnmLoadedScript` caller is
wrong; without `Chain.cpp`, chain registrations are wrong.

## Accepted exact owners (highlights)

The ledgers hold the full list. Large or instructive guards:

| Owner | Unit | Scope |
| --- | --- | --- |
| `GuiView::UpdateStageElements @0x414900` | `gui-update-stage-elements` | 3,836 bytes, 72 fields |
| `EnemyRuntimeUpdate @0x40DC80` | `enemy-runtime-update` | 2,412 bytes, 70 fields |
| `ReplayManager::SaveReplay @0x429B60` | `replay-manager-save` | 1,681 bytes, 83 fields |
| `EndingTextVmView::Run @0x40BD80` | `ending-text-run` | 1,568 bytes, 92 fields |
| `ReplayManager::Initialize @0x428F60` | `replay-manager-initialize` | 1,344 bytes |
| `Player::CalculateDamageToTarget @0x428630` | `player-calculate-damage-to-target` | 1,170 bytes, 28 fields |
| `PlayerUpdateShots @0x428280` | `player-update-shots` | 876 bytes, 22 fields |
| `PollInputFrame @0x44A5F0` | `poll-input-frame` | 850 bytes |
| `PlayerTransitionToState2 @0x4269D0` | `player-transition-to-state2` | 796 bytes |
| `SetupVertexBuffer @0x4462F0` | `anm-setup-vertex-buffer` | 472 bytes, 22 fields |
| `ReplayManager::~ReplayManager @0x4294A0` | `replay-manager-destructor` | 367 bytes |
| `EnemyManagerView::Initialize @0x40D280` | `enemy-manager-initialize` | 334 bytes |
| `FileSystem::Decrypt/Encrypt @0x44B0D0/0x44B220` | `filesystem-decrypt`, `filesystem-encrypt` | 323 and 307 bytes, `__stdcall` |
| `ItemManagerView::Draw/ConvertPowerItems @0x41B8E0/0x41BA50` | `item-manager-draw`, `item-convert-power-items` | 281 and 165 bytes |
| `MainRegisterChain @0x420470` | `main-register-chain` | 204 bytes, 21 fields |

## Biggest open levers

- **Dangling declarations.** A function declared but defined nowhere is
  anchored in every graph, so all its callers get a stack ABI. Roughly 50
  remain. List them from a big graph's `link-aliases.rsp` (`/alternatename`
  symbols with no definition in `src/`), map each to its retail callee, and
  call the real owner (examples already fixed: `FrontEndPlaySound`,
  `FrontEndBeginSelectionTransition`, `EnemyRandomView`). Candidates include
  `GameCleanup*` stand-ins for real destructors, `EnemySetScreenShake` and
  `FrontEndReadKeyboardState` (= `InputKeyboardSnapshotView::ReadKeyboardState`
  with a dropped receiver).
- **Laser vtables (about 8.6 KB of backlog).** Retail's type-1 vtable
  `0x46DA10` names the real `EnemyLaserType1View` methods followed by ten shared
  one-byte leaf slots (`0x41BF90`..). Our `EnemyLaserType1StorageView` (exact
  ctor `0x41C680`) derives from `EnemyLaserConstructionBaseView`, whose
  virtuals are undefined stand-ins, so `EnemyLaserType1View::Initialize` is
  never referenced and LTCG gives it a private EAX ABI. Type 0 and
  `EnemyLaserLineView` have the same split. Unify each storage view with its
  real class, update the ctor's vtable DIR32 symbol, then replay the laser
  Update/Draw/ctor guards. `EnemyFireLaser` probably depends on this.
- **`AnmVmIdView` default constructor.** It emits an extra
  `[this+0x732454] = 0` in the AnmRenderManager ctor (655/670); retail's ID
  type was probably plain data. Changing it touches many exact owners.
- **`AnmVmInitialize(void *unused, AnmVmView *)`** is a fake-ABI declaration
  with 13 consumers; replace it once they can be replayed.
- **Chain allocator and constructor.** `AllocElem` is 74/74 in the ReplayInit
  graph with only an ECX/EDX swap and tail store order left; the Chain ctor's
  inlined `ChainElem` constructor still carries `_ReadWriteBarrier` calls. Try a
  barrier-free `ChainElem` constructor there and replay the Chain-dependent
  exact units.
- **Popup owner.** Build `ScriptViewerPopupOwnerView::Initialize` and
  `GameCreateOwner42B660` in a graph with `AnmVmInitialize.cpp`; their current
  graph anchors it, which moves `this` from EAX to EBX.

## Known stale units

`result-menu-open-game-over` and `game-complete-stage-transition` (the
`ResultTransitions` graph) already failed their field checks at `d69a5efc` and
still need repair. Other accepted units may be stale the same way (a file split
leaving a callee anchored, or a storage-type wrapper changing argument
materialization); a grouped replay per output directory finds them.

## Private inputs

Preserve the real executable, `.tools/` (especially the game Wine prefix),
`ghidra-project/`, `_reference/`, all of `build/match-linked/`, `build/match/`
and unrelated `.analysis/d3d-*` work. [RE_ARTIFACTS.md](RE_ARTIFACTS.md) lists
the few retained diagnostic contexts and the scratch policy.
