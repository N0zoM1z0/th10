# GUI lifecycle source recovery

Historical test record: the construction/lifecycle host suites cited here were
retired on 2026-10-28. Commands for deleted scripts apply to commit `47d9993`,
not the current tree. Their results are historical evidence, not requirements
for exact reconstruction; follow [the current workflow](RE_WORKFLOW.md).

## Scope and target identity

This proposal maintains three complete genuine owners: GuiView::~GuiView at 4145F0–41482D (574 bytes), GuiCleanupResources at 414370–41454B (476 bytes), and the necessary GuiMessageVmView::~GuiMessageVmView dependency at 4136C0–413720 (97 bytes). Total 1147 target bytes of source coverage; this is not accepted exact-byte credit. The 97-byte body closes a real dependency rather than serving as a count-oriented milestone.

Target SHA256: 2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040. Independent dense Capstone 5.0.6 decoding matched all four locked component hashes. The 574-byte and 476-byte target bodies end in RET4; the 97-byte target body has an ESI live-in receiver and plain RET. Source uses genuine ordinary C++ owner/member destruction and one descriptive resource-cleanup operation. It does not encode private registers or establish original identifiers, class names or translation-unit provenance. No inline assembly, ABI adapters, dummy production classes or forced reachability is introduced. The bounded /EHsc diagnostic below does not change shared compiler defaults or existing canonical profiles.

## Complete ownership contract

GuiCleanupResources first releases captured manager.loadedAnms[28] when (runtime flags & 9) is zero. Its slot address is retained across Release, then the slot value is reread for operator delete and cleared. Alternate mode marks stageUiAnm-owned VMs instead. It captures message, clears stageUiAnm, destroys/deletes the saved message, then clears the message pointer. Runtime flags are reread after that callback. Normal mode frees and clears messageFile and clears the 491BE8 retention slot; alternate mode publishes messageFile there without freeing it. It clears update-chain flag 2; marks/clears special IDs at 9E14 and 9E18 and boss gauge; zeros ten segment IDs; marks/clears eight value IDs; zeros segment count at 9E90. Manager is reloaded after the first special-ID call and retained thereafter.

The message destructor captures the manager once and performs six unconditional VM-mark calls, clearing each corresponding ID afterward. It does not release its own allocation. Existing HUD completion already has its own genuine inline six-ID cleanup followed by raw free; that path is unchanged and does not additionally invoke the new destructor.

GuiView destruction begins with GuiCleanupResources. It captures the update element and chain before entering the lock, increments the unsigned-byte depth after Enter, unregisters, leaves, then decrements. It reloads draw element and chain and repeats. Only updateCallback is cleared; drawCallback has no target clear. The manager is captured before that clear. The special ID at 9E50 is marked and cleared, then eight value IDs are processed through lexical primary-first/secondary lookup and root-only flat child propagation of 0x04000000. Each owner ID is cleared even on zero/stale lookup. frontAnm-owned VMs are marked and canonical g_GuiView is cleared before member destruction.

The existing full GuiView declaration owns 43 actual AnmVmView members: bossPositionVm, then arrays 5 through 0 in reverse declaration/element order (7, 2, 4, 9, 10, 10). The compiler emits this member/EH teardown naturally. gui + 9DA0 is bossPositionVm.generatedVertices, not a message allocation or independent field. No manual member free/destructor loop is added to production. Genuine AnmVmView::~AnmVmView remains unchanged and owns each generatedVertices free/clear. GUI destruction never releases its enclosing allocation; GameManager retains the saved GuiView pointer, invokes its real destructor once, then operator delete on that saved pointer.

## Canonical state and consumers

GameManager's obsolete GameGuiUpdateView and g_GameGuiUpdate are removed. All accesses use existing GuiView/g_GuiView (target slot 47770C). Its former integer at 9E14 is specialVmId9E14.value; former zero test at 9EB8 is message == NULL. Assertions preserve both offsets. Existing genuine external update/reload/reuse declarations use GuiView*. No second pointer global is defined. g_GuiRetainedMessageFile491BE8 is a neutral extern declaration for the observed retention slot; the repository reference audit found no existing declaration for that slot. Its diagnostic symbol binding is reviewed below; physical production storage remains unresolved, and the maintained source adds no storage allocation. Other canonical manager/chain/Main lock aliases are reused.

Gui.hpp adds only nonvirtual destructor declarations and the resource cleanup prototype, preserving data members and x86 offset/size assertions. Four source TUs consume it directly: Gui.cpp, GameManager.cpp, FrontEnd.cpp, EnemyEclDispatcher.cpp. The inventory identifies 54 existing units in 28 actual producer groups, including normal-support exceptions. It is a producer invalidation/rerun inventory, not a claim those units failed or retained fresh receipts. Existing host tests that use bounded GUI field seams do not inherit the added header destructor; the GameManager caller fixture was explicitly adapted and revalidated. The complete selected unit inventory appears below; compiler profiles, entry symbols and source partitions remain defined by config/match-units.toml.

## Validation and boundaries

The new fixture extracts all three complete maintained source bodies, complete GuiView/GuiMessage declarations, and real maintained FindVm, MarkVmForDeletion, MarkLoadedVmsForDeletion and AnmVmView destructor bodies. It uses explicit host pointer-width/ANM/Win32 seams; it does not claim target x86 layout. Both destructor end checks run inside the destructor body, before member lifetimes end. An observational member-entry hook records 43 actual destructor calls, while matching allocation/free tracking tests null/mixed/full generated-vertex ownership and natural reverse order. No post-lifetime owner fields are read.

Loaded Release and selected Mark callbacks have declared synthetic effects to prove captured-slot/manager/flag reload contracts. They are not claims that actual dependencies perform those mutations. Loaded-resource delete is a documented boundary seam rather than deletion of static fixture objects. Message and GUI lifetimes use actual matching new/destructor/operator-delete; generated vertices/message files use matching malloc/free. The lock fixture records valid control ordering without acquiring real Windows locks. Invalid list owners, bad pointers, repeated object destruction, allocation failure outside target preconditions, and target execution are excluded.

135 complete lifecycle scenarios cover all (flags & 9) variants, every update/draw null combination, message/resource/file presence, slot replacement, mode/manager changes, primary-first duplicate lookup, zero/stale IDs, root child chains, nonroot child exclusion, preserved unrelated flags, global-clear order and 43 destructor entries. 22 source-body mutants and 2 safe header declaration mutants require compile success, process status 1 and exactly the predeclared CHECK expression at its uniquely derived source line. Compile errors, signals, sanitizer reports and timeouts are harness failures, never kills. Header mutants reorder existing arrays or add one zero-initialized member; they remain valid C++ and test order/count without illegal double destruction.

GameManager's independent caller fixture preserves all 70 cases and 50 prior strict mutation classifications. It observes an opaque canonical GuiView boundary through a separately named host allocation token; it does not fabricate a GuiView or claim to execute its member destruction. The new lifecycle fixture handles that contract separately.

## Native validation and remaining boundaries

Three serialized, non-runnable VC7.1 diagnostics used the genuine GameManagerCleanup entry, with GameManager.cpp as the primary source and the following support sources in order: Hint.cpp, Chain.cpp, AnmManager.cpp, FileSystem.cpp, GameStatistics.cpp, Lzss.cpp and Gui.cpp. PE/map/PDB identity, complete contribution boundaries and dense decoding were independently checked. Sizes below are message/resource/GUI destructor contributions, compared with target sizes of 97/476/574 bytes:

- Base /GL profile, original six-ID loop: 45/511/662 bytes. PE SHA256 `1cd97420f8cb825a4d8311908aabb6dcae2ca4c442172b3009997f27d7303248`.
- Same source and graph, adding only /EHsc: 45/511/574 bytes. PE SHA256 `d3cffec91f9e7be0817bbbd507edbdfcc0834ce0084e9767ad8d4c1c26dd546a`.
- Same /EHsc graph, replacing only the message loop with six existing named-role mark/clear pairs: **97/462/574 bytes, all nonexact**. PE SHA256 `78cb349c3b6966dce6d148bb792fbb0a8efc0ab5d378c9e8a3625159181c3a0b`. Final contributions are the message destructor at 412780–4127E0, resource cleanup at 4127F0–4129BD and GUI destructor at 4129E0–412C1D. GameManagerCleanup remains 1118 bytes.

The /EHsc control addresses a target-observed language feature, not an arbitrary size-fitting flag. Target 4145F0 registers FS:[0], uses handler 46574E and FuncInfo 471740 with seven unwind states, and invokes EH vector-destructor iterator 4525FF for its six arrays. The default diagnostic profile omitted C++ unwind support; existing canonical PbgArchive and Sound units already use /EHsc. The isolated control restores seven corresponding generated unwind actions, their member offsets/counts and state transitions, plus the six normal-path helper calls. Each action and handler has identical ordinary bytes after independently justified bindings for the ANM destructor, vector helper, FuncInfo and CxxFrameHandler; the 28-byte FuncInfo payload agrees after its unwind-map pointer binding. These are bounded compiler-structure observations, not canonical exact credit for the owner, helpers or EH data, and not proof of the original complete compiler profile or exceptional runtime behavior.

The final 97-byte message body genuinely uses the target's ESI live-in and plain-RET contract. Its actual resource caller invokes that retained body, then deletes the saved message pointer once. Exhaustive normalization finds exactly seven fields: manager DIR32 at +07 binds to 491C10; six MarkVmForDeletion REL32 fields at +0F, +1F, +2D, +3B, +49 and +57 bind to 4492A0. After those bindings, **47 ordinary bytes still differ**, in five scheduling regions +13–1D, +23–2B, +31–39, +3F–47 and +4D–55 (hex). The target clears the previous ID before loading the next; the candidate loads/pushes/prepares the next call before the clear. The first region also moves the next-ID load before XOR EBX, EBX. These are instruction-order differences, not undeclared relocations. No canonical exact unit is warranted.

The larger owners remain nonexact: resource cleanup retains an optimized EBX receiver and plain RET rather than target stack receiver and RET4, with differing register allocation and scheduling. The final 574-byte GUI contribution has unchanged non-linkage bytes relative to the first /EHsc control, so its resource-call setup, manager/list registers and scheduling residuals persist despite equal size. Its genuine GameManager caller retains the saved canonical GUI pointer across destruction and releases it once. Generated member cleanup still covers bossPositionVm and arrays 5 through 0, 43 members total, with global clearing before generatedVertices release.

The map has one typed canonical GUI symbol and one retained-message-file symbol, but diagnostic globals, imports, allocation functions and EH runtime helpers still resolve to non-runnable link anchors. In particular, __except_list is an anchor in the FS operands and needs the independently justified ABS0 binding; this is not physical production storage. Physical ownership of target storage at 47770C and 491BE8, executable product closure, actual exception handling and runtime scenarios remain unproven. The factory-failure path is supported by pinned target decoding, not a newly maintained native factory.

Stop this bounded sequence here: the two controlled changes have explained the missing unwind machinery and recovered the genuine message caller/receiver without forcing the remaining scheduling. No further register shaping, inline/ABI directives, profile sweeps or unrelated source tuning is justified by these results. Preserve the natural source and explicit residuals; any future hypothesis needs separate target evidence and full-extent canonical replay. No shared compiler default or existing canonical profile was changed, and existing dependencies' exactness does not transfer to these owners.

Host validation completed remotely: 135 lifecycle scenarios and 24 strict intended-CHECK mutation classifications, plus 70 caller scenarios and 50 caller mutation classifications. Two earlier PIE sanitizer runs timed out and remain recorded as harness failures, never successful mutant kills. A bounded repeated-mutant diagnostic observed intermittent AddressSanitizer failures with PIE and successful controls without PIE; it does not establish a universal root cause. The host fixture adds -fno-pie/-no-pie only when AddressSanitizer is enabled. The final remote lifecycle run passed with ASan, UBSan and LSan enabled. This workaround changes neither production source nor the historical target compiler profile.

On 2026-10-17 at 02:38 UTC, the serialized cold regression completed: **54 existing units, 12,420 exact bytes, 28 producer groups** (26 linked-PE and two COFF). All full canonical unit comparisons passed with unchanged profiles. These are regression results, not new exact-byte credit or newly accepted Factory receipts. The source/fixture hashes stayed fixed throughout. A permission-review timeout before group 13 started was checked for absent output/process and retried once; a later session reset was recovered from saved reports without rebuilding completed groups. Host results remain bounded contract evidence; no target execution or whole-product claim follows.

## Host fixture reproduction

Run from the repository root with a host g++ toolchain. These commands execute the full baseline and mutation suites, not target code:

```sh
ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 scripts/repo-python -O scripts/test-gui-lifecycle.py --root . --optimization=-O1 --sanitizers=address,undefined
scripts/repo-python -O scripts/test-game-manager-teardown.py --root . --opt O1 --sanitize
```

The lifecycle runner honors ASAN_OPTIONS; its explicit setting above enables LSan. The caller runner sets its own ASan/UBSan environment and disables LSan because of the documented ptrace limitation. Its explicit allocation assertions still run, without claiming LeakSanitizer coverage. Both runners reject compiler errors, sanitizer diagnostics, signals and timeouts as harness failures. O0/O2 or baseline-only checks are optional controls, not substitutes for the full suites.

## Existing canonical regression inventory

This selected rerun contains 54 unique existing units in 28 producer groups: 26 linked-PE groups and two COFF producers. It includes direct Gui.hpp consumers (Gui.cpp, GameManager.cpp, FrontEnd.cpp and EnemyEclDispatcher.cpp), plus canonical graphs compiling them as support sources. Player.cpp forward-declares GuiView; its graphs are included when they compile Gui.cpp. The complete selected run passed as reported above; the list below makes the same selection reproducible.

The group labels below name output directories under build/match-linked/, except the two explicit COFF object paths. Unit IDs refer directly to config/match-units.toml; retain each unit's current compiler/link profiles, real entry and ordered supports. In particular, BulletRuntimeUpdate retains its normal non-/GL BulletTransform.cpp support partition.

- AnmVmIdOperations: `anm-vm-id-clear-flag2`, `anm-vm-id-release`, `anm-vm-id-set-flag2`
- AnmVmIdSetInterrupt: `anm-vm-id-set-interrupt`
- AnmVmIdSetSprite: `anm-vm-id-set-sprite-explicit`, `anm-vm-id-set-sprite-own`
- AsciiManagerEnsureSelectionVm: `ascii-manager-ensure-selection-vm`
- BulletRuntimeDeactivate: `bullet-position-bounds`, `bullet-runtime-deactivate`, `bullet-update-deceleration`
- BulletRuntimeUpdate: `bullet-runtime-update`, `player-collision-check`
- BulletState8: `bullet-update-aimed-direction`, `bullet-update-relative-direction`, `bullet-update-state8`
- BulletBoundaryBounce: `bullet-update-boundary-bounce`
- BulletHorizontalWrap: `bullet-update-horizontal-wrap`, `bullet-update-polar-acceleration`
- GameManagerOnDraw: `decomphelp-game-manager-on-draw`
- EclVmPolar: `ecl-vm-polar-from-angle-magnitude`
- build/match/EnemyEclDispatcher.obj: `enemy-apply-bullet-clear`, `enemy-configure-interrupt`, `enemy-find-laser`, `enemy-set-chapter`, `enemy-set-laser-position`, `enemy-wait-for-effect`, `enemy-wrap-angle`
- EnemyBulletBounds: `enemy-bullet-playfield-bounds`
- EnemyDispatcherHelpers: `enemy-drop-vector-from-angle-radii`, `enemy-enable-shield`, `enemy-mark-pending-interrupt`, `enemy-resolve-int-argument`, `enemy-sound-queue-cue`, `enemy-sound-queue-sample`
- FrontEndCursorPush: `frontend-cursor-move`, `frontend-cursor-pop`, `frontend-cursor-push`, `frontend-input-repeated`
- build/match/FrontEnd.obj: `frontend-cursor-set-current`
- FrontEndDeleteVm: `frontend-delete-vm`
- FrontEndDrawCallback: `frontend-draw-callback`
- FrontEndKeyConfigDisplay: `frontend-keyconfig-display`, `frontend-practice-unlock`
- FrontEndOptionsRefresh: `frontend-options-refresh`
- FrontEndSetScreenState: `frontend-set-screen`, `frontend-set-screen-state`
- FrontEndUpdateCallback: `frontend-update-callback`
- GameManagerUpdateCallback-cold-2: `game-manager-update-callback`
- GuiScoreAdd: `gui-score-add`
- GuiLivesDisplayCount: `gui-set-lives-display-count`
- HintLifecycle: `hint-create-owner`, `hint-draw-callback`
- MainNextGameMode: `main-set-next-game-mode`
- PlayerResetRuntimeState: `player-reset-runtime-state`

Final target-independent CI passed on 2026-10-17 at 02:39 UTC, including the lifecycle baseline hook, canonical graph validation and generated progress checks. No exact unit was added or promoted for this packet.
