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
- Mode 10 requires calls 418150 and 4180E0, field +0x390 = 7, field +0x39C = 1, and the normal continuation membership. An extra store/instruction effect remains unclassified.
- Mode 11 requires call 4180E0, field +0x390 = 7 and field +0x39C = 0. Call 418150 is CFG-possible.
- Mode 12 requires call 4180E0, field +0x390 = 7 and field +0x39C = 1. Call 42CDB0 is CFG-possible.
- Mode 13 requires calls 418150 and 4180E0, field +0x390 = 7 and field +0x39C = 1.
- Mode 14 requires call 40B940; call 418150 is CFG-possible.
- Modes 4 and 15 have CFG-possible calls 40B9D0, 418150 and 42CD50 and the normal continuation membership. Mode 15 also has an unresolved +0x390 store and an unclassified extra store.

The public high-level model is consistent with this structure, but does not establish helper meanings, guards, or ordering. The default/normal continuation effects above are required membership, not a proposed execution sequence.

## Remaining body-level obligations

1. Pre-dispatch store effects and critical-section argument/depth-update roles.
2. Exact guards for optional helpers and failure paths; ordering and reload points around opaque calls.
3. Mode 10 and 15 unresolved effects; the mode 0 +0x768 value origin is now proved, without an inferred pointer type. The mode 0 zero/nonzero result guard is also proved, but source ordering remains open.
4. Meaningful helper source declarations and native ABI evidence. The existing natural int(context*) baseline remains conditional.

A faithful executable source body is not yet justified. The supplied fixture checks only recognized-vocabulary projections of proved unordered mode obligations; extra real effects outside its vocabulary are not disproved. It deliberately cannot certify a body that merely rearranges the same effects.

## Evidence

Latest guard proof HEAD 0821ec6ed83fabc2559d3cdca5b6aff86b708ec9; the preceding proof checkpoint was 1d55297d317574a0b0b18cb7b2738682a5b79e24; prior proof receipts used raw-identical production/target files at 6892fa5. Owner SHA 219ce9d6dfdbe4218b7657910f2bc3d9f8cc2f2b5844953d58908e9bc9c3a56e.

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

Expected: PASS, 34 finite inventory/count checks, 95 recognized-projection schema/membership mutation rejections, 2 guard projection checks and 10 guard projection mutation rejections. These are bookkeeping checks against recorded facts, not semantic body-equivalence tests or a new native-target proof. They do not execute the game, read its executable, build native code, or establish feasibility/order.

Optimized assertions must be rejected:

    python3 -O probes/main-mode-transition/check_obligations.py

Expected: nonzero exit with `optimized Python`. The facts digest is checked before use. `manifest.json` binds the three human/fixture/facts files; no native bytes, instruction traces, full command receipts or collectors are packaged.


## Conditional proof assumptions

- Memory accesses complete normally on valid memory and opaque calls return normally.
- Opaque calls preserve EBX, ESI, EDI and EBP register values; pointee memory is not preserved.
- Named IAT slots retain their normal loader-resolved API bindings.
- The code owner and all separately pinned auxiliary dispatch tables remain immutable.

The per-mode event facts begin at the direct dispatch case target. Pre-dispatch effects are excluded. These facts neither preserve pointed-to memory across calls nor establish complete effects, event ordering, stack restoration or helper-internal behavior.
