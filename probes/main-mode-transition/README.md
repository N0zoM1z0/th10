# 0x4218D0: bounded source reconstruction status

## Established source structure

The 572-byte code owner and 94 bytes of separately pinned dispatch data are structurally closed. Both switches have recursively decoded in-owner targets and dominating unsigned range guards. The first switch selects a load-site snapshot from entry-EAX context +0x390. The second selects a context +0x38C snapshot minus 1 modulo 2^32, giving source snapshot values 1..14. Neither theorem equates later memory with those observations.

A unique equality guard compares snapshots from fields +0x38C/+0x390. Its equality continuation normally returns 1 without calls or stores. Frame restoration and equality of current memory at a later time are not established by this result.

Direct calls made by this owner have the following critical-API counts: normal return 1 paths contain either zero Enter/Leave calls or one of each; return 4 paths contain one Enter and no Leave. Both return classes occur in the modeled CFG; feasible execution is not established. This is a count theorem under normal unchanged loader bindings and normal opaque calls; it does not prove API argument identity, lock ownership, or helper-internal behavior. The return 4 behavior must not be silently corrected by inserting an unlock.

## Mode continuations

These are unordered required effects or CFG-possible effects after the first dispatch target. CFG-possible does not prove feasibility. Helpers retain address-based neutral names because their bodies and source types are not recovered.

- Snapshot modes 1, 2, 5, 6, 8, 9 share the default continuation. Required: Leave, return 1, a +0x38C store whose value originates at a +0x390 load, and byte +0x6F9 decrement modulo 256.
- Mode 3 requires calls 4203F0 and 421C50 and return 4. No Leave event occurs in its continuation inventory.
- Mode 0 requires call 41FD00, field +0x390 = 1 and a field +0x768 store of call 41FD00's unconstrained machine EAX result. Other modeled continuations can store +0x390 = 3, execute the two failure hooks and return 4, or use the normal continuation. The zero-result guard is now proved as described below. The return type and effect order remain open.
- Mode 7 requires call 4180E0 and field +0x39C = 1, then the normal continuation membership. Call 42CDB0 is CFG-possible.
- Mode 10 requires calls 418150 and 4180E0, field +0x390 = 7, field +0x39C = 1, and the normal continuation membership. The two data stores below are required. An instruction-effect category remains unclassified.
- Mode 11 requires call 4180E0, field +0x390 = 7 and field +0x39C = 0. Call 418150 is CFG-possible.
- Mode 12 requires call 4180E0, field +0x390 = 7 and field +0x39C = 1. Call 42CDB0 is CFG-possible.
- Mode 13 requires calls 418150 and 4180E0, field +0x390 = 7 and field +0x39C = 1.
- Mode 14 requires call 40B940; call 418150 is CFG-possible.
- Modes 4 and 15 have CFG-possible calls 40B9D0, 418150 and 42CD50 and the normal continuation membership. Mode 15 additionally has CFG-possible stores of 4 to context +0x390 and 3 to data variable 0x491C00. Their requested-snapshot guard is now proved in the four-class partition below.

The public high-level model is consistent with this structure, but does not establish helper meanings, guards, or ordering. The default/normal continuation effects above are required membership, not a proposed execution sequence.

## Remaining body-level obligations

1. Complete pre-dispatch ordering/effects and critical-section lifecycle/ownership. The fixed prefix effects and both API arguments below are established conditionally.
2. Exact guards for optional helpers and failure paths; ordering and reload points around opaque calls.
3. Mode 10 instruction-effect classification and remaining conditional helper guards; the mode 0 +0x768 value origin is now proved, without an inferred pointer type. The mode 0 zero/nonzero result guard is also proved, but source ordering remains open.
4. Meaningful helper source declarations and native ABI evidence. The existing natural int(context*) baseline remains conditional.

A faithful executable source body is not yet justified. The supplied fixture checks only recognized-vocabulary projections of proved unordered mode obligations; extra real effects outside its vocabulary are not disproved. It deliberately cannot certify a body that merely rearranges the same effects.

## Evidence

Latest mode 15 proof HEAD 5c1f353e9e942fe6490b240a9f2636c9c1607300; store proof HEAD 2fc031bd162c2f9a57c5357ddea3d933c4521150; prefix proof HEAD b33c5f9cd893b75a44f69e2d58124f6d1c34f1f5; guard proof HEAD 0821ec6ed83fabc2559d3cdca5b6aff86b708ec9; the preceding proof checkpoint was 1d55297d317574a0b0b18cb7b2738682a5b79e24; prior proof receipts used raw-identical production/target files at 6892fa5. Owner SHA 219ce9d6dfdbe4218b7657910f2bc3d9f8cc2f2b5844953d58908e9bc9c3a56e.

- Dispatch: actual 51430a881fd14a3cae4459db4f574a15.
- Selector origins: actual 2be7243f79154d44b2212480e00e0acb.
- Equality return: actual 4fa8c615097f4e689b943e8690ae6eff.
- Critical counts: actual d584c17c59ce49598516898cf667355f.
- Per-mode effects: actual f3563a1a1e76480f8a93ca9ed0f92274.
- Mode 0 result origin: actual 9191aecb85a94dce99e634212bd23b73.


## Mode 0 result guard and superseded limitation

The first adjacent-only attempt, actual 8e4fe32c2cda4fd4ae1090e282c4ce60, did not recognize the guard. This was a recognition limitation rather than a disproof. A later bounded diagnostic found a retained full 32-bit result self-test, one flag-preserving MOV, and a not-equal branch.

Actual 4e190410333641a79d6a636f52dae13b proves the guard after validating both CFG predecessor boundaries, the MOV's exact effects, and a unique 41FD00 call site. The intervening store remains included in the case effects. Every modeled normal continuation from mode 0 encounters this guard:

- Zero machine EAX result requires calls 4203F0 and 421C50, a +0x390 store of 3, and return 4. It does not return 1.
- Nonzero machine EAX result requires LeaveCriticalSection and return 1. It does not return 4.

The tested result is the same unique call-site origin supplying the required +0x768 store. This does not infer a pointer return type, full source ordering, API argument identity, complete effects or helper-internal behavior. The earlier failed recognition is retained in the evidence metadata as superseded.

## Reproduce this checkpoint

From repository root:

    python3 probes/main-mode-transition/check_obligations.py

Expected: PASS, 34 finite inventory/count checks, 97 recognized-projection schema/membership mutation rejections, 2 guard projection checks and 10 guard projection mutation rejections, plus 2 prefix/receiver projection checks and 6 mutation rejections, and 4 mode 15 projection checks with 31 mutation rejections. These are bookkeeping checks against recorded facts, not semantic body-equivalence tests or a new native-target proof. They do not execute the game, read its executable, build native code, or establish feasibility/order.

Optimized assertions must be rejected:

    python3 -O probes/main-mode-transition/check_obligations.py

Expected: nonzero exit with `optimized Python`. The facts digest is checked before use. `manifest.json` binds the three human/fixture/facts files; no native bytes, instruction traces, full command receipts or collectors are packaged.


## Conditional proof assumptions

- Memory accesses complete normally on valid memory and opaque calls return normally.
- Opaque calls preserve EBX, ESI, EDI and EBP register values; pointee memory is not preserved.
- Named IAT slots retain their normal loader-resolved API bindings.
- The code owner and all separately pinned auxiliary dispatch tables remain immutable.

The per-mode event facts begin at the direct dispatch case target. Pre-dispatch effects are excluded. These facts neither preserve pointed-to memory across calls nor establish complete effects, event ordering, stack restoration or helper-internal behavior.


## Conditional pre-dispatch effects and critical API arguments

Actual 638f869cfa9946dfa77d2be7b990c0c4 establishes four required recognized effects on modeled normal entry continuations that reach the direct dispatch:

- EnterCriticalSection is called.
- Context +0x394 receives a value with a +0x38C load-site origin.
- Context +0x780 receives 0xFF000000.
- Byte context +0x6F9 receives its proved byte-load snapshot plus one modulo 256.

The byte theorem requires a matching low-byte load and INC, unique dense CFG predecessors, and no intervening calls, control boundaries, stores or lane clobbers. It does not assert equality with later field contents. The prefix projection is unordered, excludes equality-return and post-dispatch effects, and does not establish feasibility or complete effects.

Actual 036054aaf763438890eaef25af546f0b proves that both Enter and Leave top stack arguments originate at entry EAX context +0x6C4. The normal valid nonwrapping stack must be disjoint from the full context object, including the live pushed argument and CALL return slots. The maintained layout suggests critical-section slot 5.

The Leave proof permits exactly one intervening dword context +0x38C store whose source has the +0x390 load-site origin. That store has no register or flag writes; PUSH, store and CALL have unique dense predecessors. Its effect remains in the case inventory. The earlier unresolved Leave result was a recognition limitation, now superseded. These argument facts do not establish critical-section initialization, ownership, helper-internal behavior or complete source ABI.


## Mode 10 scalar/record relation and mode 15 stores

Actual e66657eeffb04567aed708231e59faec proves a shared scalar load-site origin from dword data cell 0x474C80. The mode 10 store to 0x474C7C receives that scalar snapshot. The store to 0x477848 receives 0x474788 + 0x30 times the same snapshot, with uint32 modular arithmetic. The data cell and table-base addresses lie in validated non-executable data sections. No table bytes, record count, allocation bounds or equality across reloads are inferred.

Maintained GameManager.cpp names the destinations g_GameStartupMode and g_GameStageUpdate and the source g_GameComparison474C80; these are source naming observations separate from the address-based target proof. The record view has a maintained size of 0x30. The public model's difficulty terminology is not adopted.

Actual 29affe0c01ea42e08ad0c85622e306b9 makes both mode 10 stores required on all modeled normal continuations. It recognizes the mode 15 stores context +0x390 = 4 and data variable 0x491C00 = 3 as CFG-possible. The latter global retains a neutral name. Effects remain incomplete and unordered.

The preliminary mode 15 guard attempt, actual 01dc49477b8940fb8eaf314add7d22e4, found three conditional branches but no adjacent exact CMP form. This is a recognizer limitation. That preliminary limitation is superseded by the four-class proof below.


## Mode 15 guarded recognized effects

Actual 0df7b7446c7a48ada20791732bf58125 verifies three full-width destructive zero tests against one request load-site snapshot and an acyclic CFG. It establishes these four classes:

- Snapshot 2: required call 42CD50 and the two stores context +0x390 = 4 and data 0x491C00 = 3.
- Snapshot 7: the same required events, plus call 418150.
- Snapshot 14: the same required events, plus call 40B9D0.
- Every other uint32 snapshot value: none of those specific recognized helper/store events appears.

All four classes require the normal-continuation recognized events and return 1. These are unordered memberships; complement absence does not exclude unclassified or aliased writes or opaque helper effects. The proof does not equate later field reloads with the guarded snapshot or establish feasibility.

## Minimum path to a natural C++ candidate

1. The fixed optional-helper guards are now closed for modes 4, 7, 11, 12 and 14 as described below. The next source obligations concern precise snapshot identities and call/store timing.
2. Verify a compact set of source-order and reload obligations: Enter before prefix stores/dispatch; required lifecycle helpers relative to state stores and successor helpers; failure hooks in their source order; and the normal field-copy, Leave, depth-decrement tail. Avoid a generic instruction trace or claims derived solely from unordered memberships.
3. Classify the remaining mode 10 instruction-effect category against the already proved pure modular record expression, rather than assuming no extra effect.
4. Write ordinary C++ with neutral fields and meaningful opaque helper declarations under explicit natural-signature assumptions. Exact private helper ABI, stack/frame restoration, layout ownership and native comparison remain separate acceptance gates. Recovering all nine helper bodies is not a prerequisite for a clearly conditional compiler candidate.

This checkpoint still does not justify a complete executable source body. The next useful work is the small set of guard/order obligations above, rather than further broad inventories.


## Conditional helpers and a reference correction

Actual 3d78eb79e60d434a9b92f38c81733be8 closes the four optional-helper partitions. Each mode uses its own exact request-field observation; no equality with earlier or later reloads is assumed.

- Modes 7 and 12: direct helper 42CDB0 is required when that request snapshot equals 4, and absent from the recognized direct calls otherwise.
- Modes 11 and 14: direct helper 418150 is required when that request snapshot equals 7, and absent from the recognized direct calls otherwise.
- All eight classes have modeled normal return 1. Mode 11 preserves the real context +0x39C zero store in the flag-preserving interval; the earlier adjacent-comparison limitation is superseded.

Actual 2c379f91afc2497caf3d95776a8a4567 establishes the independent mode 4 request partition:

- Request snapshot 1 or 2: helper 42CD50 is required.
- Snapshot 7: helpers 418150 and 42CD50 are required.
- Snapshot 14: helpers 40B9D0 and 42CD50 are required.
- Every other uint32 snapshot, including zero and high-bit values: none of these recognized direct helper events occurs.

This corrects a specific hypothesis from the [pinned public semantic reference](https://github.com/levzzz5154/th10-decomp/blob/4e4ab0b73eb75f1d3c2e49b39330deabd5fdcf4a/matching2/reference/legacy_semantic/src/game/game_mode_switch.cpp): its mode 4 model places helper 42CD50 on the default branch instead. Independent remap/default composition checks support the target-derived partition above. No reference body was copied.

These facts quantify modeled CFG continuations and recognized direct helper memberships only. Opaque helper internals, feasible execution, source order, additional aliased effects and full source equivalence remain outside this checkpoint. Normal valid memory, normal opaque-call return, unchanged loader IAT bindings and immutable code/tables remain premises. Nonvolatile register values are preserved under the stated ABI premise; pointed-to memory may change.

The standalone fixture additionally checks 8 optional-helper memberships with 8 membership mutations, and 4 mode 4 projection cases with 9 membership mutations. These are bookkeeping regression controls, not native-body semantic mutants.
