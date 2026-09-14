# TH10 Exact-Reconstruction Handoff

## Checkpoint state

- Repository `th10`, branch `main`, target `target:th10-main`, analysis provider `th10-ghidra`.
- Packet starting HEAD: `93a141c3d223eef62dc4ff7a2c235893a347a842` (`gpt-web: reconstruct TH10 enemy ECL dispatch seam`).
- Planned checkpoint subject: `gpt-web: reconstruct TH10 enemy ECL operands`.
- Final commit hash is intentionally not self-recorded before the commit exists; recover it from live Git and the ignored campaign manifest after checkpoint.
- The live packet entry on `93a141c3...` was clean. During the packet another local/Factory flow wrote the same operand packet (`src/Enemy.cpp`, `src/Enemy.hpp`, match-unit and later ledger/doc files). File contents changed while being observed. Target-dependent editing stopped, hashes/status were sampled until stable, and the coherent dirty set was classified `recoverable-current-work`, audited and completed rather than reset or overwritten. No unrelated or unknown tracked/untracked path was found.
- This recovery/resume conversation itself entered at the same HEAD with 12 unstaged tracked paths, 0 staged paths, 0 untracked paths and 0 conflicts. All 12 paths belonged to the same coherent operand packet and were retained as `recoverable-current-work`; no unrelated or unknown path was staged, reset, deleted or overwritten.
- Recovery/resume `.analysis/` entry inventory was **365 regular files / 4,218,280 logical bytes / 0 files >64 MiB**; legacy and unknown ignored state was preserved.
- `origin/main` independently advanced to `93a141c3...` during the session. This Web session did not push.
- Clean-live packet-entry `.analysis/`: 194 regular files / 1,850,048 bytes / 2344 KiB allocated / 0 files over 64 MiB.
- Current campaign: `.analysis/gpt-web/20260914-enemy-operands/`.
- The exact-reconstruction campaign remains active/incomplete; this handoff is only a bounded conversation checkpoint.

## Recovery and authority

The session called `factory_describe`, `factory_list_repositories`, and `factory_get_repository_status(th10)`, inspected branch/HEAD/history, porcelain-v2 state, complete tracked/untracked state, ignored target/tool/build/analysis state, and the prior handoff. A newer clean checkpoint `93a141c3...` was found as a direct descendant of remembered `9f6cd941...`; it was adopted as live authority instead of restoring old chat state.

All requested repository/Factory guidance and contracts were re-read through `factory_repository_run_shell` before tracked reconstruction work. No requested path was missing.

Fresh repository preflight passed on `93a141c3...`:

- `python3 scripts/verify-target.py`
- `python3 scripts/verify-toolchain.py --execute`
- `python3 scripts/validate-tracking.py --require-target`
- `python3 scripts/report-reconstruction-status.py`
- `python3 scripts/ci.py`
- `python3 scripts/build-match-unit.py --check`
- `python3 scripts/build.py --check`

The execute toolchain path passed pinned VC7.1 SP1 build6030 normal COFF, C++ `/GL`, resource and PE32-i386 link smoke through headless Wine. Native `th10-ghidra` operation schemas were rediscovered and `check {}` passed for `target:th10-main` with `provider_transport=factory-native-command`; a later final refresh also passed. Useful Ghidra results used this same passed target binding. Capacity/ownership/transport failures that returned no semantic result were treated as unavailable evidence, never as mismatch or rollback.

Target remains the ignored operator file `resources/th10.exe`: size 487,936, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`, MD5 `7dc488d82c81dd4aee4ba098b8804d83`, PE32 i386 base `0x00400000`, entry `0x004537DC`, dominant Rich build6030. It was not modified, moved, staged or committed. `/mnt` was not searched and `TH10_TARGET_PATH` was not set.

## Completed hard packet: Enemy ECL operands

The packet reconstructed the four vtable-owned operand methods immediately following the previously reviewed 14,416-byte Enemy ECL dispatcher physical owner. Their boundaries/origin were already reviewed authored-game denominator entries at packet entry; this packet adds maintained source and exactness evidence rather than inflating the authored denominator.

| Address | Maintained source | Target bytes | Final state |
| --- | --- | ---: | --- |
| `0x00411FC0-0x004122F3` | `EnemyFullObjectView::ReadIntOperand` | 820 | source-present, non-exact |
| `0x00412300-0x0041234F` | `EnemyFullObjectView::ResolveIntOperand` | 80 | source-present, canonical exact |
| `0x00412350-0x004126C3` | `EnemyFullObjectView::ReadFloatOperand` | 884 | source-present, non-exact |
| `0x004126D0-0x0041271F` | `EnemyFullObjectView::ResolveFloatOperand` | 80 | source-present, canonical exact |

The reviewed full Enemy vtable at `0x0046D0C0` owns these entries in slots 1-4. Raw target review closes each `RET 4`, adjacent padding and each resolver jump table. Ghidra does not model the four vtable entries as standalone functions; its semantic view remains provisional and carries no exactness credit.

## Operand protocol and layout refinement

The two lvalue resolvers and the target spawn copy independently close the 0x20-byte block at full Enemy `+0x1138..+0x1157` / runtime `+0x0FC..+0x11B` as four writable integer ECL variables followed by four writable float ECL variables. Spawn executes `REP MOVSD` for exactly eight dwords from request `+0x20` to full Enemy `+0x1138`, so maintained `EnemyEclVariableView { int integers[4]; float floats[4]; }` is shared by `EnemyRuntimeView` and `EnemySpawnRequestView`. This refines the prior neutral eight-dword transport block without changing the reviewed object extents.

Both readers preserve the target's exact 51-selector domain `-10000..-9950`. TH10-local cases cover RNG U32/unit/signed-unit values, signed random angle scaling, current/offset/base motion values, Player position and angle-to-point helpers, update timer, runtime flag bit `0x10000`, the four int/four float variables, special-enemy coordinates, managed-VM id resolution followed by signed VM word `+0x38A`, rank, difficulty, velocity angle, life and difficulty equality tests.

The integer and float readers deliberately remain separate because TH10 contains type-dependent behavior elsewhere. Direct table entry sends integer selectors `-9996` and `-9976` to target `0x0041219A` while `ECX` still names this Enemy, so both readers expose this Enemy's world-Y and the integer form applies the target float-to-int helper. Only selector `-9962` enters through `0x00412192`, replaces `ECX` with the manager primary special-enemy pointer, and then falls through to `0x0041219A`; selector `-9963` resolves the corresponding special-enemy X directly. Maintained source preserves this target control flow rather than normalizing it from adjacent games.

TH10-local writers establish `0x00474C74` as a `0..4` difficulty index and `0x00474C98` as a signed rank-like value restored from replay and adjusted/clamped to `[-0x400,+0x400]`; maintained source uses descriptive names `g_EnemyDifficulty` and `g_EnemyRank`, not original-identifier claims. Helper evidence independently identifies RNG helpers at `0x0044B9E0/0x0044BB20/0x0044BB90`, Player angle-to-point helper `0x00426660`, float-to-int helper `0x00463B2C`, and VM id resolve-or-clear helper `0x00449450`. Source does not add a target-absent null guard around the VM lookup.

Committed adjacent source was consulted only after TH10 switch tables/storage were independently decoded. TH08 observed HEAD `a45e99fb1942714e6edded20847e32a654d56f97` was clean; adjacent material only corroborated the broad typed int/float/lvalue ECL source family. No selector, address, layout, ABI, owner or exactness fact was transferred. Dirty adjacent untracked content was not used.

## Compiler and exactness feedback

Final maintained `src/Enemy.cpp` compiles with pinned VC7.1 SP1 build6030 under fixed normal `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc` and the same profile plus `/GL`.

True final normal `/Gy` extents versus target are:

- `ReadIntOperand`: **800 / 820 bytes**; target-window diagnostic `mismatch`, **115/452 comparable bytes**, 92 object relocations, `acceptance_authority=none`.
- `ResolveIntOperand`: **80 / 80 bytes**.
- `ReadFloatOperand`: **800 / 884 bytes**; target-window diagnostic `mismatch`, **95/540 comparable bytes**, 86 object relocations, `acceptance_authority=none`.
- `ResolveFloatOperand`: **80 / 80 bytes**.

The two 80-byte lvalue resolvers each have an explicit canonical `artifact_kind="coff"` unit. Each complete owned extent carries five DIR32 relocations: one jump-table relocation plus four local case-target relocations. Current-source clean builds and repeated target-bound comparison return `result="exact"`, 80/80 bytes, with all relocation targets matching the target. These are the only new exact promotions in this packet.

The two readers remain source-present/non-exact. Successful `/GL` compilation preserves a possible production LTCG/interprocedural context but gives no standalone exactness credit.

Because all current Enemy exact units share `src/Enemy.cpp`, the final integration gate deletes/rebuilds and compares every configured exact unit after the operand edits:

- `enemy-manager-draw-callback`: exact 6/6, zero relocations;
- `enemy-ecl-dispatch-forwarder`: exact 11/11, one relocation;
- `enemy-ecl-int-lvalue-resolver`: exact 80/80, five relocations;
- `enemy-ecl-float-lvalue-resolver`: exact 80/80, five relocations.

Total repository canonical exact coverage after this packet is **4 functions / 177 bytes**.

## Ledger and verification planes

Current ledger before checkpoint:

- candidates: **1261**
- origin/boundary pending: **1132**
- authored: **118 / 52,927 bytes**
- excluded: **11**
- source-present: **90 / 39,658 bytes**
- canonical exact: **4 / 177 bytes**

Packet delta from `93a141c3...`: candidate/pending/authored/excluded denominators unchanged; source-present +4; exact +2 functions / +160 bytes. `config/build.toml` remains unchanged and honestly open. `docs/KNOWLEDGE_BASE.md` adds/refines `ENEMY-008` and `TOOLCHAIN-028`; `docs/PROGRESS.md` and `resources/progress.svg` were regenerated.

**Source presence:** four already-reviewed authored operand bodies now have maintained source.

**Exactness:** both readers remain non-exact under fixed-normal diagnostics; both 80-byte lvalue resolvers are repeatable canonical exact. Existing Enemy exact units continue to replay exact after the shared source edits.

**Whole build:** actual `python3 scripts/build.py` returned **RC2 / explicitly open**. Production compiler flags, TU partition, libraries, resources and link order remain unknown. `build.py --check`, tracking and public CI pass this honest open state.

**Runtime:** not performed. There is no closed faithful reconstructed Windows-i386 product to execute.

**Factory Truth:** a fresh read-only `factory_get_accepted_snapshot(th10)` at the final precommit boundary returned sequence **0** with **0 accepted facts**. No replay/submission was performed. Repository canonical exact and Factory accepted-fact state remain independent.

## Scratch and recovery artifacts

At clean `93a141c3...` packet entry `.analysis/` contained **194 regular files / 1,850,048 bytes / 2344 KiB allocated / 0 files >64 MiB**. Concurrent same-packet experiments temporarily created many duplicate objects/JSONs. Only files inside the current campaign with established current-session ownership and final replacements were removed; generated configured `build/match` objects were also removed after replay.

The current campaign now retains **48 files / 340,362 bytes**: one final normal and `/GL` Enemy object pair, final two reader diagnostics, four final canonical exact receipts, bounded raw operand disassemblies/table/case/handler evidence, whole-build logs and the manifest. Current `.analysis/` overall is **349 regular files / 3,842,781 logical bytes / 4680 KiB allocated / 0 files >64 MiB**. Growth outside the current campaign is not attributed to this packet and was left untouched. This resume removed only its own superseded `*.recovery.*` / `*.target-corrected.*` scratch and retained fresh `*.resume-final.*` compiler/Oracle receipts plus whole-build logs. Legacy/unknown analysis, other campaigns, Ghidra state, toolchain, Wine state and the private target were preserved.

## Next hard frontier

Do not pivot to easy leaves. The strongest already-reviewed continuation remains the central `EnemyRuntimeView::DispatchEclInstruction` physical owner at `0x0040E770-0x00411FBF`, **14,416 source-owned target bytes**, still source-absent/non-exact. Its executable Ghidra body is `0x0040E770-0x00411D2E` (13,759 reachable bytes); the remaining physical extent owns the top-level/nested dispatch tables through `0x00411FBF`. Its 11-byte full-object forwarding adapter at `0x0040E760` is already canonical exact, and the four operand methods reconstructed here now supply the typed value/lvalue protocol needed by the dispatcher.

Nearby unresolved functions `0x004127A0` (593B) and ledger candidate `0x00412AD0` (652B) were considered as a smaller connected ECL cohort, but a fresh Ghidra caller query was unavailable because analysis capacity was busy; no call-graph relationship is claimed. They are secondary routes, not a reason to avoid the central dispatcher merely because they are smaller.

The next session should re-attest `th10-ghidra`, then attack the 14.4 KiB dispatcher in bounded opcode-family cohorts: reconcile the 181-opcode selector path and nested table ownership, recover helper/private ABIs and exits, and promote source only when a natural maintainable representation is supported. This packet-selection balance continues the central hard frontier rather than optimizing for easy function count.

The exact-reconstruction campaign remains active/incomplete. Faithful Windows-i386 product closure, runtime validation, Factory acceptance, semantic reconstruction and portability remain open.
