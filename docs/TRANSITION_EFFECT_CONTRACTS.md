# Transition-effect semantic proposal

## Scope and status

Local-only proposal atop committed67ced17fff5dfe758a33000b3f83c92002187373 (lexical Gui Run). Existing dependency bodies were read from the clean exported50b971f source and applied-current-prefix; source hashes in config/transition-effect-host-inputs.json pin exactly what was consumed. This packet adds ordinary source only, not production integration, native ABI closure, target execution, exact-byte credit, or a recovered original class/TU.

Two production files implement the shared complete nine-kind initializer, eleven update/draw callbacks, factory, release trigger, cleanup and required scheduler deletion entry. `TRANSITION_EFFECT_TARGET_MATRIX.md` lists every target extent. Small scalar-deleting adapter43C890 is not manually emitted: no forcedABI or guessed virtual class is introduced. Source-level explicit destruction covers the logical cleanup/free behavior, with compiler-emitted boundary ownership unresolved.

## Source design

- `TransitionEffectView`: target0x44 allocation/zeroing proves seventeen words. +04/+14 are explicitly unknown zeroed words, not guessed fields/vptr or arbitrary code padding. Existing ChainElem and AnmVmTimerView are reused. Compile-time i386 layout checks fail on incompatible physical layouts.
- Factory: nothrow ordinary new constructs canonical timer (flag clear), then memset zeros the whole target object, flags OR2, full Initialize. Allocation failure returns null without registration. Nothrow is selected only to expose the null-return branch in this logical candidate; target allocator overload, runtime binding against452493, and throw behavior remain unproven.
- Initialization: registration and pointer publication precede deletedCallback assignment and timer/argument writes. Kind3 alpha255 is visible during registration. Invalid kind and failed registration are not safe-success cases; callers must satisfy the target's valid-allocation/kind precondition.
- Callback return values are preserved:1 active;7 invokes delete callback;0 on kind4 finish/abort requests only node unregister. This packet does not add scheduler runners.
- Cleanup: capture update node and current chain, enter recursive Windows critical section, increment nesting, canonical UnregisterElem, leave, decrement; reload draw node and global chain for second independent cycle. No pointer clearing or idempotence promise. Destroy cleanup then deletes effect exactly once.
- Rendering: canonical DrawColoredRectangle and FlushVertexBuffer remain real dependencies. Full-screen fade resets only viewport X/Y/W/H; minZ/maxZ persist. Dim-screen mode intentionally does not reset viewport. Direct viewport and shake externs are unresolved physical-binding seams, not claims that current manager fields alias those globals.
- Game-manager partial declaration is identical to existing FrontEnd.cpp ResultGameManagerView. Consolidation into one shared header is an integration concern; this packet does not modify the existing owner.
- Kind1 advances RNG four times, duplicating second half per axis; kind8 calls the canonical bounded RNG twice. The apparent kind1 defect is preserved.

## Host test contract inventory

Tests must compile actual proposed source bodies and canonical reused dependencies, not a parallel reference implementation. Test harness adaptation of 32-bit raw Chain allocation accesses to host pointer fields must be explicit and narrow. Scheduler, allocation observation, graphics device, and recursive lock instrumentation are labeled host seams.

Required observations:
1. All nine selectors, priorities, callback identities, node flags/arguments/deletion hook, and publication-before-parameter-write ordering; all seventeen zeroed words when applicable.
2. Allocation failure: no object/registration; valid factory returns flags2 with initialized timer and exact payloads.
3. Falling/rising alpha: finite positive/negative/truncating values, zero frames, terminal thresholds, startup abort, null GM, flags1/4 pause, unrelated flags continue.
4. Dim: inclusive current<=frames, indefinite persistence, release timer reset, current8 active/current9 deleted, no invented startup/pause gate.
5. Pulse: ARGB alpha source, masked RGB, repeat count/reset/tick ordering, return0 on finish/abort. Observe stale update pointer without dereferencing it; do not run fictional safe cleanup on a freed node.
6. Kind1: tick before expiry, initial/final interpolation, duplicated-half RNG sequence, generation counts, choices0/1/2 and unchanged shake on expiry.
7. Kind8: null GM and flags0x77 gates, attack/hold/decay thresholds, signed comparisons/unsigned decay conversion, two real bounded RNG calls, unchanged shake on terminal/paused paths.
8. All five draw entries: exact rectangle/color, screen fade flush+viewport ordering and retained depth fields, dim screen's lack of reset, post-callback manager reload performed by canonical rectangle dependency when modeled.
9. Return7: observed calc-dispatch protocol caches next before call, reacquires lock before deletion callback; recursive nested lock works, both nodes removed, effect deleted once, no current-node dereference afterward.
10. Cleanup: separate lock/nesting cycles, chain reload between nodes, null-node branches; no pointer clearing; deletion callback null argument returns0.
11. Strict mutation runner: exactly one intended replacement, successful compile, expected assertion identifier, nonzero assertion failure. Compilation errors/timeouts/unrelated failures are not successful kills.

## Numeric and runtime boundaries

Signed word reinterpretation follows the target VC7.1 two's-complement model. Explicit unsigned addition/subtraction protects C++ arithmetic where target wraps32bits; conversion back to signed is target-compiler-dependent for out-of-range inputs. Tests must not claim portable signed-wrap semantics. Existing timer signed overflow and out-of-range float conversion remain outside defined fixtures.

Ordinary expressions use float source types; target x87 keeps extended precision at several sites, particularly kind1's first output. Host SSE tests establish only finite representable selected cases. NaNs, infinities, out-of-range conversion and full x87 exception/precision equivalence remain unsupported. No algebraic tuning or rounding hacks are added to match instructions.

No real scheduler dispatcher source exists for449C00/449D40. Host protocol tests validate only the observed interaction contract. Mode4 retains effect and draw node after scheduler return0, and updateNode becomes stale. External ownership/leak resolution remains unknown. No automatic cleanup repair is invented.

The extern direct-storage declarations, heap ownership, Windows recursive-lock implementation, private register ABIs, original source/TU, production build inclusion, exact target function/table extents and full native behavior all remain separate integration/Oracle obligations.

Concrete non-extreme precision counterexample: kind8 after Tick with subframe1,
attack3 and amplitude10 retains1/3 in target x87 until the final amplitude
store, yielding float3.3333332538604736. Strict-float host evaluation of the
candidate rounds the envelope first and yields3.3333334922790527. Branch and
lifetime contracts can pass while numerical equivalence fails. This candidate
is not universally target-equivalent even for ordinary finite inputs. A native
compiler observation is required before revising expression/storage strategy;
no precision matching is claimed from host fixtures.

## Ledger integration is separate

Read-only ledger query80b517dc520e43ab950e5fdb76d7c32d on clean67ced17 found
no functions.csv containing row for43BD40,43C230,43C310,43C3C0,43C460,43C550,
43C710,43C890,43C910,43C930, and no exact function-boundaries row at those starts.
These are dependency candidates needing distinct complete-boundary/origin review;
this packet does not add them to any denominator or claim they are authored.
Existing reviewed/source-empty authored rows43C1A0,43C2C0,43C410,43C500,43C8B0,
43C970 and43CB00 may receive provisional source mappings after review, with
exactness unchanged.43C870 has reviewed26-byte extent but indeterminate origin;
its source-backed callback semantics do not settle source-written versus
compiler-generated provenance. Table43CAD8 remains a separate data obligation.

The reused canonical timer constructor reads/modifies its flags before factory
memset, as does the target. On freshly allocated indeterminate C++ storage this
inherits an initialization-language concern; a host allocator that prefills
fixture bytes makes the test deterministic but does not establish general ISO
C++ uninitialized-read safety. Moving zeroing before construction would change
the target-observed order and is not done here.

Stale-node nuance: canonical UnregisterElem searches list identities before
reading the element fields. A later cleanup of a live kind4 effect can miss its
freed update-node address without dereferencing it if that address is absent.
Address reuse can instead match a different newly allocated node. The proposal
preserves this stale-identity hazard; it does not claim every later Cleanup is
an immediate use-after-free, nor that repeated cleanup is generally safe.

## Executed host evidence (2026-10-17)

The frozen maintained baseline and ASan/UBSan run each passed14 scenarios and
843 assertions. The serial strict run compiled all38 runtime mutants, each
failing at exactly its expected assertion; one source-order control and one
known-precision-gap observation control passed separately. The classifier
requires exit86, empty stdout and exactly the expected assertion plus newline;
noise, compile failure, timeout, signal or unrelated assertion is rejected.
These are bounded host observations under the disclosed seams and numeric gap.
No target/native execution or exactness credit is implied. The same baseline
also passed after actual git application with repository CRLF attributes.
