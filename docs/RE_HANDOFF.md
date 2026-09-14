# TH10 Exact-Reconstruction Handoff

## Checkpoint state

- Repository `th10`, branch `main`, target `target:th10-main`, analysis provider `th10-ghidra`.
- Current session starting HEAD: `444eb1a63d4e3a91c4bfcad90c63da25d55c4a02`, clean, branch `main`, ahead of `origin/main` by two local `gpt-web:` commits.
- Planned current checkpoint subject: `gpt-5.6-sol: promote exact PbgFile accessors`. Final commit hash is intentionally not self-recorded before the commit exists; recover it from live Git after checkpoint.
- Recovery found no staged, unstaged, or untracked files. The ignored private target, existing `.analysis/`, toolchain, Wine prefix, Ghidra project, and build caches were preserved.
- Current campaign: `.analysis/gpt-5.6-sol/20260914-pbgfile-exact/`. Earlier `gpt-web` campaigns remain ignored evidence and were not treated as current authority without replay.
- This session has not pushed. The exact-reconstruction campaign remains active/incomplete.

## Recovery and authority

The session inspected branch/HEAD/history, complete tracked/untracked state, the prior handoff, and the two commits ahead of `origin/main`. Clean HEAD `444eb1a...` was adopted as live authority.

All requested repository and Factory guidance was re-read from the live repository shell before tracked reconstruction work. No requested path was missing.

Fresh repository preflight passed on `444eb1a...`:

- `python3 scripts/verify-target.py`
- `python3 scripts/verify-toolchain.py --execute`
- `python3 scripts/validate-tracking.py --require-target`
- `python3 scripts/report-reconstruction-status.py`

The execute toolchain path passed pinned VC7.1 SP1 build6030 normal COFF, C++ `/GL`, resource and PE32-i386 link smoke through headless Wine. Native `th10-ghidra` operation schemas were rediscovered and `check {}` passed for `target:th10-main` with `provider_transport=factory-native-command`; a later final refresh also passed. Useful Ghidra results used this same passed target binding. Capacity/ownership/transport failures that returned no semantic result were treated as unavailable evidence, never as mismatch or rollback.

Target remains the ignored operator file `resources/th10.exe`: size 487,936, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`, MD5 `7dc488d82c81dd4aee4ba098b8804d83`, PE32 i386 base `0x00400000`, entry `0x004537DC`, dominant Rich build6030. It was not modified, moved, staged or committed. `/mnt` was not searched and `TH10_TARGET_PATH` was not set.

## Current packet: canonical PbgFile exact backlog

The systematic audit found that the low exact count is partly a workflow backlog rather than a compiler failure. Earlier work had already compiled several PbgFile and ResFile methods to exact non-relocation bytes, but stopped at relocation-masked probe mode because the executable contains both normal COFF and LTCG inputs. Later Enemy checkpoints used the narrower and correct claim boundary: a canonical normal-COFF unit may prove exact reproduction of its declared target extent without claiming the original production TU or LTCG physical owner. This packet applies that same boundary consistently to six already reviewed authored PbgFile methods.

Fresh target inspection re-confirmed the complete PbgFile boundaries and import/vtable destinations. Ghidra models the destructor and `Close`; the existing TH10-local vtable and raw-byte evidence remains necessary for the four Ghidra-missed methods. Current `src/PbgFile.cpp` was compiled from absent output paths twice with pinned VC7.1 SP1 build6030. The six canonical units replay exact across **220/220 bytes and nine DIR32 relocations**: destructor 46 bytes/three relocations, `Close` 34/one, `Read` 57/one, `Tell` 25/one, `GetSize` 21/one, and `Seek` 37/one.

The packet also makes probe-to-canonical review cheaper: probe mode in `scripts/compare-coff-function.py` now decodes each relocation's object addend, target encoded word, and target-derived candidate destination. These values remain diagnostic and cannot grant exactness; the reviewer must still establish the symbol, boundary, ownership of the relocation, and a zero-difference canonical replay.

No source behavior or layout changed. Original PbgFile TU partition, production normal-COFF versus LTCG ownership, complete link graph, runtime behavior, and the general linked-image LTCG Oracle remain unknown.

## Completed previous hard packet: Enemy ECL dispatcher context seam

This packet deliberately stayed on the central 14,416-byte `EnemyRuntimeView::DispatchEclInstruction` owner at `0x0040E770-0x00411FBF` instead of pivoting to easier leaf functions. It bounded opcodes `0x15D-0x164`, the nested difficulty tables, and the adjacent integer argument seam. The observable outcome was structural: recover context ownership/ABI and denominator gaps needed for a natural future dispatcher, without writing a partial `default`-return switch and falsely promoting the dispatcher to source-present.

Direct TH10 table decoding maps the eight selected opcodes to case entries `0x004118BB`, `0x004118E2`, `0x0041198B`, `0x004119FC`, `0x00411A3E`, `0x00411B23`, `0x0041009B`, and `0x004100EA`. `0x15F` uses rank interpolation with target constants `1024.0f` and `1/2048.0f`; `0x162` performs the integer analogue `(high-low)*(rank+0x400)/0x800 + low` with the target signed truncation-toward-zero correction. Difficulty cases reach five-entry tables at `0x00411F98` and `0x00411FAC`. These observations are provisional semantic/target evidence, not exactness credit.

The strongest ownership result is the ECL execution context embedded in full Enemy `+0x008..+0x101F`, exactly `0x1018` bytes, with the active-context pointer at full `+0x004`. Context `+0x004` is the current ECL instruction; its target-observed header has opcode at `+0x004`, variable-operand mask at `+0x008`, and dword operands at `+0x010`. Generic typed-operand helpers use context `+0x1008` as operand-stack state, `+0x100C` in local-storage addressing, and context `+0x1014` as the operand-resolver owner. Constructor machine code stores the full Enemy itself at full `+0x101C` = context `+0x1014`, directly binding the generic context to the already-reviewed Enemy operand vtable. Maintained `src/Enemy.hpp/.cpp` now express this exact-size context/instruction view while preserving all full-object offsets.

`0x00412A00-0x00412A0F` and `0x00412A10-0x00412A5A` are now boundary/ABI-reviewed integer argument wrappers. Both take `EnemyRuntimeView*` in EAX and argument index in ECX; the first forwards to the generic int reader and the second resolves a writable int argument address. Complete target scans find all 30 and 9 direct calls respectively inside the central dispatcher, but whether these are source-written helpers or compiler/LTCG outlines remains unknown, so origin stays `unknown/review` and neither is source-present.

Raw corridor review also found three initial-inventory misses: `0x00412A60-0x00412A6D` (14B), `0x00412A70-0x00412A7D` (14B), and `0x00412A80-0x00412A90` (17B). They are CC-delimited helper-shaped bodies adjacent to the seam, but fresh Ghidra xrefs plus complete direct-REL32 and absolute-pointer scans find no target references. They are therefore added only as new `unknown/review` candidates; authored/source/exact denominators do not increase.

Adjacent-game hypothesis provenance for this packet: clean committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97`; clean committed TH09 HEAD `f1d4c1e332828a0cb8b9308b61b79ce77f86e4d0`; TH095 live tree was dirty (4 paths), so only committed HEAD `dee282c33dbb75ca0545d3115cd421a6e4f6a7bd` was consulted. Adjacent source supplied only ECL operand source-family hypotheses after TH10-local recovery; no adjacent addresses, opcodes, layouts, owners, or exactness were transferred.

Pinned VC7.1 SP1 build6030 compiles the refined Enemy source under fixed normal and `/GL`. A separately compiled clean-HEAD baseline and the refined normal object emit byte-identical 586-byte `EnemyConstruct` COMDAT contents with the same four relocation sites; both fresh target diagnostics are non-exact at `63/657` comparable bytes. The old `64/657` note is superseded. This is structural source feedback only, not constructor or dispatcher exactness.

The first cold canonical replay after adding the header declarations correctly failed on the int lvalue resolver because VC7.1 renumbered compiler-internal `$L...` symbols by exactly +37. Clean-HEAD versus revised COFF symbol review proved every int/float resolver jump-table/case label kept the same section offset, and the canonical relocation offsets, types and target VAs are unchanged. `config/match-units.toml` therefore refreshes only those generated object-symbol names. A second cold deletion/rebuild/replay then returns exact 6/6, 11/11, 80/80 and 80/80 for all four configured units. The failed receipt is retained as recovery evidence; exactness was not assumed across the manifest update.

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

Total repository canonical exact coverage after the Enemy packet was **4 functions / 177 bytes**.

## Ledger and verification planes

Current ledger at the PbgFile checkpoint:

- candidates: **1264**
- origin/boundary pending: **1135**
- authored: **118 / 52,927 bytes**
- excluded: **11**
- source-present: **90 / 39,658 bytes**
- canonical exact: **10 / 397 bytes**

Current-packet delta from `444eb1a...`: **+6 canonical exact functions / +220 exact bytes**. Candidate, authored, excluded, and source-present denominators are unchanged. The previous Enemy hard-packet delta was +3 candidates / +3 pending from the unreferenced `0x00412A60/70/80` bodies. `config/build.toml` remains unchanged and honestly open.

**Source presence:** no new target function is promoted source-present. All six PbgFile methods were already mapped to maintained source.

**Exactness:** repository canonical exact coverage is ten functions / 397 bytes. The six new PbgFile units pass two cold-output rebuild/replay cycles across 220 bytes and nine relocations. The four prior Enemy units remain separately replayable. This exact lane does not establish production object ownership or whole-build closure.

**Whole build:** actual final `python3 scripts/build.py` returned **RC2 / explicitly open**. Production compiler flags, TU partition, libraries, resources and link order remain unknown. `build.py --check`, tracking, toolchain execution and public CI pass this honest open state.

**Runtime:** not performed. There is no closed faithful reconstructed Windows-i386 product to execute.

**Factory Truth:** no Factory replay or acceptance submission was performed by this session. Repository canonical exact results and Factory accepted facts remain separate states.

## Scratch and recovery artifacts

The overall recovery/resume entry inventory earlier in this conversation was **365 regular files / 4,218,280 logical bytes / 0 files >64 MiB**. At the clean `2053a39e...` hard-packet boundary, `.analysis/` contained exactly **319 regular files / 3,709,138 logical bytes / 4,464 KiB allocated / 0 files >64 MiB**. Legacy/unknown analysis, other campaigns, Ghidra state, toolchains, Wine state and the private target were preserved.

The hard campaign `.analysis/gpt-web/20260914-enemy-dispatcher-hard/` exits with **28 files / 257,221 bytes**. It retains the manifest, bounded dispatch/table/helper disassemblies, top-level and nested table maps, clean-HEAD baseline source/object evidence, final normal and `/GL` objects, constructor/read-int/read-float diagnostics, local-label shift receipt, exact replay receipts (including the intentional failed pre-refresh receipt), and whole-build logs. Superseded current-packet `Enemy.context.*` scratch and reproducible `build/match/*.obj` outputs were removed only after final receipts existed.

The previous Web checkpoint's final `.analysis/` inventory was **347 regular files / 3,966,359 logical bytes / 4,796 KiB allocated / 0 files >64 MiB**. No legacy/unknown artifact was bulk-deleted, and the ignored private `resources/th10.exe` was not modified, moved, staged or committed.

The current PbgFile campaign adds one compact ignored manifest. Current `.analysis/` inventory is **348 regular files / 3,970,042 logical bytes / 0 files >64 MiB**. Reproducible compiler objects remain below ignored `build/`; no new decompiler dump or copied target was created.

## Next hard frontier

Continue the canonical backlog audit with the six already observed structural-exact `CMemoryPbgFile` methods, then the PbgArchive constructor/entry-constructor/destructor candidates. Each requires reviewed relocation destinations and cold canonical replay; do not promote the compiler-generated deleting destructors. This route can recover exact coverage that prior probe-only sessions left unregistered and provides a fast check of the new relocation-candidate output.

After the reviewed backlog is exhausted, return to the central Enemy dispatcher with a second opcode cohort that shares the typed execution context and argument wrappers. Keep `0x0040E770-0x00411FBF` source-absent until a complete maintainable switch representation is defensible. In parallel with later product work, the major infrastructure gap remains a target-bound linked-image extent Oracle for LTCG-owned code; normal-COFF exact units do not close that gap.

The adjacent `0x00412AA0` and later ECL helper corridor is also relevant as ABI/owner context, but smaller size alone is not a reason to abandon the central owner. The three newly discovered unreferenced `0x00412A60/70/80` candidates require origin evidence before any authored promotion.

The exact-reconstruction campaign remains active/incomplete. Faithful Windows-i386 whole-build closure, runtime validation, Factory acceptance, semantic reconstruction and portability remain open.
