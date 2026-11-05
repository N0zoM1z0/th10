# TH10 reconstruction — current agent handoff (2026-11-05)

**Read this first when taking over.** The current Git worktree was clean at
handoff preparation; inspect it again before editing. The latest reviewed
source checkpoint is **4ba28ab** (ANM interpreter typed traversal); newer
**960d2e1** adds additional evidence without source edits. Later commits may
supersede this page.
Do not assume any historical dated experiment describes the currently
installed source. The function/match/origin ledgers and generated PROGRESS.md,
not old handoff counts, determine exact credit and attribution.

## Start here (no full cold replay)

    git status --short --branch
    git log -8 --oneline
    scripts/repo-python scripts/verify-target.py
    scripts/repo-python scripts/verify-toolchain.py --check
    scripts/repo-python scripts/validate-tracking.py
    scripts/repo-python scripts/ci.py

The sole valid original target is privately supplied Japanese TH10 v1.00a,
resources/th10.exe, 487,936 bytes, SHA-256
2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.
The original executable, pinned VC7.1 SP1 and Wine toolchain under .tools,
Ghidra project, private archives and ignored experiment images are **not
committed** and must be transferred separately to a different workspace.

Preserve and finish dirty changes; do not assume another worker caused them.
Use substantive, separate commits with subject **gpt-web: ...**, established
N0zoM1z0 author identity and gensokyo-dev co-author trailer. Never push
without a separate instruction.

## Verified ledger snapshot

| Generated measure at this handoff | Current value |
| --- | ---: |
| Reviewed target functions / boundaries | 1,829 / 1,829 |
| Source-present function mappings | 1,309 |
| Canonical byte-exact functions | **1,482** |
| Canonical exact codegen bytes | **189,797** |
| Confirmed authored functions / code bytes (provisional) | 750 / 276,120 |
| Confirmed-authored exact functions / code bytes | 574 / 119,761 |
| Confirmed-authored source-present nonexact backlog | 176 |
| Classified exclusions / origin-indeterminate entries | 520 / 559 |

These values come from PROGRESS.md as checked on 2026-11-05. The previously
printed **1,478 / 189,217**, **570 / 119,181** and **181 backlog** were
stale. Run scripts/repo-python scripts/progress.py after actual ledger
changes; do not manually edit its generated output. Source presence,
source-shape improvements, equal-sized functions and historical byte scores
do not add codegen-exact credit. A byte-exact function also does not prove
that its original source was handwritten, that the game links, or that it runs.

## Recent actual source progress — big bosses AND leaves

- **Full Enemy ECL instruction dispatcher**, target 0x0040E770 / 14,416
  bytes, in src/EnemyEclDispatcher.cpp: real typed bullet pattern and
  difficulty fields, laser storage, player/world motion, timeout callbacks,
  opcode/header/variable-length ECL arguments and names, and ANM VM
  position/rotation/geometry fields have replaced many raw offset casts.
  Latest full real 11-source VC7.1 /GL PDB candidate **14,352 bytes** with
  **716 linked fields**. The latest confirmed batch 652113c typed the real
  position/scalar interpolation timers; their independent 50-byte COFF helpers
  each match the original 50-byte code shape (one exactly resolved DIR32), but
  this deliberately changes **18 ordinary candidate-to-candidate bytes** in
  the 14KB dispatcher (two independent store-scheduling regions) without
  changing extent or linked-field geometry. Earlier typed boss slot, world
  coordinates, and bullet-offset vector work preserved the prior candidate's
  ordinary bytes; evidence is source-epoch-specific.
  **14,352 / 14,416 are sizes, NOT 14,352 matching original-target bytes**.
  The full owner is still **NONEXACT**; its 716 fields are diagnostic, not a
  canonically accepted full original-target binding contract.
  Current evidence:
  evidence/enemy-ecl-manager-player-typed-20261105/README.md,
  evidence/enemy-ecl-bullet-offset-typed-20261105/README.md,
  evidence/enemy-interpolation-typed-timers-20261105/README.md.
  Latest whole graph: build/gpt-web/20261105-followup-ecl/typed-interpolation-timers/.
- **ANM ExecuteScript**, target 0x0043EE30 / 9,964 bytes,
  src/AnmManager.cpp: sequential opcode-91 child-script/layer evaluation
  is installed (eb854c4), not reverted. The 4ba28ab source now also models
  all three normal/interrupt byte-sized steps with
  `AnmRawInstructionView::Next()` and all three script-offset jumps with
  `AtByteOffset`, with native header offset assertions. A freshly linked real
  five-source VC7.1 `/GL /GS` graph retains **9,962/9,964 strict original
  bytes**, including **350 target-bound fields / 95 local destinations**.
  The only remaining mismatches are +0x113/+0x115 ECX-vs-EDX argument
  transport; four scoped canonical ANM units cold-replayed **1001/1001**.
  The large function remains **NONEXACT**, no forced-register ABI.
  Evidence: evidence/anm-boss-typed-instruction-walk-20261105/README.md;
  current image: build/gpt-web/20261105-followup-anm/typed-instruction-next/.
- **EnemyBeginSpell**, target 0x00409280 / **2,432 bytes**:
  the full 11-source VC7.1 /GL owner remains **2,424 bytes**, 108 linked
  fields. Current source reads the real full enemy runtime world position at
  +0x1068 and models `0x90`-byte spell-history entries with name +0,
  capped count +0x84, and preserved statistics group stride. Both large ECL
  owners retained ordinary candidate bytes during this batch. NONEXACT;
  evidence/enemy-spell-history-typed-20261105/README.md.
- **Exact small ECL helpers in the large-function producer**:
  `EnemyConfigureInterrupt` @0x412720 remains target-exact **48/48** after
  recovering typed timer threshold and life callback fields; unlike the
  failed direct-four-fields rewrite, its mixed pointer lifetime retains the
  original compiler stores. Position/scalar interpolator helpers each retain
  independent 50-byte original-body matches (one resolved speed-pointer
  field) with historically non-unique source attribution. These are source
  improvements, **zero newly credited canonical exact functions**. Evidence:
  evidence/enemy-interrupt-typed-exact-20261105/README.md and
  evidence/enemy-interpolation-typed-timers-20261105/README.md.
- **ECL Run**, target 0x0044E1A0 / 7,020 bytes: last explicitly checked
  native graph had **6,997/7,020** and 189 field obligations, with 23
  ordinary differences around index registers, x87 evaluation order and
  FORMAT stack homes. It is a retained diagnostic from an earlier source
  epoch, **not a fresh rebuild of all current sources**. Recheck field
  bindings and current source hashes before using its score.
  evidence/boss-ecl-anm-byte-frontiers-20261104/README.md.
- **EclVmScriptDatabase::AddScriptData**, target 0x00450220 / 589 bytes:
  current src/EclVm.cpp models the old table pointer and input offset
  cursor as reloadable pointer locals (22ca6b5). Real 5-TU /GL source
  candidate **570 bytes**, up from 547; retains original REP MOVSD copy
  and conditional release. Remains NONEXACT. The external caller
  EnemyEclResourceView::LoadFile is separately exact 95/95.
  evidence/ecl-add-script-data-cursor-lifetimes-20261104/README.md.
- **Popup Draw**, target 0x0042B780 / 540 bytes: current typed 723-row
  storage/accessor and `loadedDigitAnm` resource pointer at owner +0x10
  in ScriptViewerOwners.hpp and ScriptViewerPopupCreate.cpp. Whole PDB Draw
  candidate **537 bytes** and factory **71 bytes** retain their prior candidate
  codegen; the 10-byte draw callback still mismatches three non-field bytes.
  All three remain NONEXACT. Constructor, Update and Update callback remain
  exact **134 + 134 + 7**; selective native replay passed **275/275**.
  Evidence: evidence/popup-digit-resource-typed-20261105/README.md.
- **ResultMenu Chain owners**: real update/draw ChainElem pointers at
  +0x08/+0x0C, native 0x2C8 object. Exact constructor 101,
  initialization 142, factory 67, cleanup 311, all retained after typed
  source replacement (6a9216a). The independent MarkTasks @0x422200
  remains **26-byte candidate / 28-byte original**, nonexact due to
  private receiver/register ordering.
  evidence/result-menu-typed-chain-ownership-20261104/README.md.
- **Chain::AllocElem**, target 0x449ED0 / 74 bytes: source now writes the
  real callback/addedCallback/deletedCallback fields with offset asserts
  (e30dd9b). The 74-byte candidate is ordinary-byte identical to the
  previous candidate but still **22 ordinary mismatches vs target**.
  The 59-byte Chain sentinel constructor also remains NONEXACT.
  evidence/chain-typed-allocator-callbacks-20261104/README.md.
- **ItemManager Draw**, target 0x41B8E0 / 281 bytes: two typed cursors,
  target-shaped x87 source operands, actual 281-byte candidate but
  remaining x87 lifetime and 14 address-field placement differences;
  NONEXACT. Existing 84-byte callback registration, 216-byte destructor
  and 185-byte factory are independently accepted exact.
  evidence/item-draw-x87-operand-order-20261104/README.md.
- **Leaf near matches**: TextRenderBufferView::InvertAlpha @0x437160
  is 156/157, +0x69 JL vs target JS (natural JS variants disturb 19
  other bytes); BulletRuntime::UpdateAbsoluteDirectionChange @0x4078E0
  is 333/335; SoundPlayerView::PreloadBgm @0x43D7D0 is 365/369
  with 10 field bindings (ReadFile EAX/EDX swap); EnemyEclHostBaseView::
  SetSubroutine @0x450700 is 24/30; SoundDataLoaderThread @0x43D080
  has a target 107-byte body but the latest full real producer emitted
  112. No exact credit. Private experiments under
  .analysis/gpt-web/20261104-current/ record negative controls.
- **Other medium/large active fronts**: 274-byte laser list updater,
  226-byte type-0 laser CheckPoint, 359-byte Player option trail,
  335-byte Bullet direction changer, 540-byte Popup Draw, 645-byte GDI
  bitmap compositor, 2,320-byte Item update, 9,964-byte ANM and 14,416-byte
  Enemy ECL dispatcher. Target specific owners and compare complete
  bodies; do not spend an entire session on tiny leaf-only work.

## Newly accepted exact function to retain

EnemyEclResourceView::LoadFile at **0x0040CD20**, 95/95 original bytes,
was accepted by **two independent** real VC7.1 /GL linked colds with
**five exhaustive target-bound fields**. The critical correction is the
natural private EAX filename interface of the real
FileSystem::OpenFile @0x0044B360; the isolated earlier caller pushed the
filename on the stack and did not match. The new canonical unit is
enemy-ecl-resource-load-file-0040cd20. Its separately nonexact
AddScriptData callee gains **no extra exact credit**.
Evidence: evidence/enemy-ecl-loadfile-real-filesystem-exact-20261104/README.md.

Other useful accepted boundaries include ItemManager's factory 185,
destructor 216 and registration 84; Real FileSystem::OpenFile 360;
ANM CreateVmVariant2 73; ResultMenu's four owners 621 combined; and
the exact Chain/Popup helpers noted above. These examples are
not an exhaustive canonical list: search config/matches.csv and
config/match-units.toml for exact unit names and accepted field contracts.

## Scoped continuation workflow

1. Inspect dirty files and match the target SHA. Read the maintained source
   and its current exact-unit recipe, not an arbitrary historical PDB.
2. Examine complete target bytes and the **whole** compiled PDB/COFF
   function. Check real source/headers, compiler profile, linker entry,
   all original address fields and fixed local/callee obligations.
3. Make a small real source change and compile **only its actual source
   graph**. For a true exact addition require independent complete
   original-target, field-bound colds. Never score a function exact
   by size, a candidate-to-candidate match, or partial byte overlap.
4. Replay only affected existing exact units, explicitly naming --unit.
   Examples:

       scripts/repo-python scripts/replay-exact-units.py \
         --unit enemy-read-int-argument \
         --unit enemy-read-raw-float-argument --json
       scripts/repo-python scripts/replay-exact-units.py \
         --unit chain-register-calc \
         --unit result-menu-initialize --json

5. Run scripts/repo-python scripts/ci.py (seven light stages),
   scripts/repo-python scripts/progress.py --check if a ledger changed,
   and git diff --check. Commit substantive verified milestones separately.
   **Never run the full-cohort cold replay by omitting --unit.**

## Storage and transfer: read RE_ARTIFACTS.md

build/match-linked and every existing private input are protected. Old
experiment PE/MAP/PDB outputs may be archived and selectively restored,
not assumed expanded; a historic artifact is not current-source proof.
Evidence markdown and numeric receipts remain committed. The latest
storage inventory, exact preserved witness paths, restore commands and
per-file verified archive SHA-256 are in RE_ARTIFACTS.md and
evidence/handoff-storage-20261105/README.md. As of November 5, the
previous 269 MiB build/gpt-web expansion has been reduced to about
9.1 MiB by archiving 7,243 fully attested experiment files; 13 selected
frontier subtrees and all canonical build/match-linked outputs remain.
**Private archive bytes are not in Git.** For a new
workspace, Git alone will not provide the original game, pinned VC7.1
toolchain, Wine prefix, Ghidra database or ignored private archives.

The complete original Windows executable, real game runtime, authorship,
product linking, semantics and portability are still separate open phases.
