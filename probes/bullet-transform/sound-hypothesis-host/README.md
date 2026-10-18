# Authored BulletTransform protocol host regressions

Run `python3 run.py --mutations` or `python3 -O run.py --mutations`.
Requires Python 3 and g++ supporting C++17. No network or external services.
By default the three source files are read from the repository `src` directory (`ROOT.parents[1]/src`). Use `--source-dir PATH` for another location. Source pins identify canonical LF bytes: only CRLF is converted to LF, and any remaining bare CR is rejected. No other normalization is permitted. The implementation pin identifies the typed sound-interface hypothesis on the vector local-expansion proposal; the two shared-header inputs retain their snapshot pins. Canonical LF identity supports the repository CRLF checkout rules.

## What executes

The runner compiles the complete, unmodified `BulletTransform.cpp` implementation, including its local angle branches; its two local header includes are removed. The complete declarations/enums from `BulletManager.hpp` are used after removing two dependency includes, 36 target-ABI typedef assertions, and the `__stdcall` spelling. Dependency stand-ins instrument calls. Canonical LF source hashes are checked before extraction. Temporary build products stay under this directory and are cleaned up. No build products or saved run output are part of this source-only checkpoint.

This is a host behavioral regression suite for the current authored source, **not evidence of equivalence to the TH10 binary**, target layouts, compiler ABI, assembly, or runtime helper implementations. Host pointers have host width; BulletManager target offset assertions are intentionally not exercised; the genuine sound view's host-compatible offsetof assertion is retained. The timer stub records `SetCurrent` arguments/call counts, not target timer internals. Trigonometry and sprite initialization dependencies are stubs, not separately verified implementations. Sprite checks verify only the call receiver and arguments, not animation behavior. No production source changes are made.

## Independent finite oracle

The oracle is a set of hand-authored concrete expected outcomes, using numeric action values rather than reusing implementation enum constants. It does not reimplement the dispatch loop. The entire production function is executed against the fixtures.

- Child 0x00400000 consumes two records: second kind/gate ignored; signed 8-bit color, signed 16-bit counts, low-byte start index, floats, flags, zeroed descriptor fields, inherited position, transform sound -1.
- All 18 records, including an otherwise-unused final-record canary, copied before the spawn callback mutates parent storage.
- Callback can change the parent's cursor to 7: continuation increments the live cursor to 8 and executes its newly written record. A separate callback mutates record 2 to prove continuation reloads parent storage.
- Fade cancellation occurs after spawn; parent execution continues. Standalone despawn also continues.
- NONE, upper bound 18, SKIP, unknown action, active gating, finite forward jump, owner/cull continuation, sound order.
- All ten timer-starting action variants select the expected timer and supply either zero or payload duration. Direction counters and deceleration vector reset checked.
- All three bounce variants preserve existing flags, write only slot 4 value0/int1/int0, and leave timers/other state intact. Cancel-behavior assigns payload int0 (negative and positive) and continues without cancelling.
- Sprite initialization calls the parent VM with the manager animation pointer and indexed base plus signed offset; interpretation continues.
- Vector/polar transform sounds require both a nonzero program index and a nonnegative sound ID; -1, 0, and positive IDs are checked at indices 0 and 1. Transform sounds use position zero; explicit sound uses bullet X.
- Vector 0x10 and direction 0x40/0x80/0x100 matrices cover finite sentinel boundaries, both infinities and quiet NaN. Separate checks preserve both signs of literal zero without aim calls. Vector and direction NaN both store NaN without aiming. Each aim branch calls the helper exactly once, all others zero times. All direction kinds preserve speed for NaN speed payload. Gated vector and direction actions perform no state write or aim call.

Domain: readable/writable parent with cursor in [0,18]; child records start at most at index 16; jumps remain nonnegative and bounded and have no unbounded cycles. Callbacks are synchronous and do not free parent storage. Tests do not validate or repair malformed scripts, negative cursors, out-of-bounds child index 17, or unbounded jump cycles. Integer narrowing cases assume the host's conventional two's-complement narrowing behavior, as observed in the run. No fast-math.

## Mutation discipline

Each mutant is a single uniquely anchored edit of the extracted source. Every mutant must compile, execute, and exit with the explicit semantic-check code 31 and its designated exact `FAIL: ...` diagnostic. An unrelated assertion is a runner failure, never a kill. Compile failure, timeout, signal/crash, or other exit code is an error, never a kill. There are 52 mutations covering the original ten protocol changes plus bounce payload/counter/flag preservation, cancel payload, sprite offset/animation pointer, and each acceleration variant's sound index gate, zero/negative ID boundary and position argument. Four direction-specific mutants send NaN to aim, change either sentinel equality, or eagerly call aim. Vector-specific mutations send NaN to aim, change either sentinel equality, eagerly call aim, or bypass active gating. The earlier direction-to-shared-helper mutant was replaced because that change had equivalent angle semantics. The original 32 mutants remain semantic changes; helper presence is not itself a mutation oracle.

Python validation uses explicit exceptions, and C++ checks use explicit `exit(31)`, so Python `-O` and C++ `NDEBUG` cannot silently remove validation. Compilation currently uses `-O0 -fno-strict-aliasing` to avoid pretending that the authored reinterpret-cast accesses are portable optimized C++.

## Coverage exclusions and frozen artifact binding

This suite is not exhaustive action coverage. The helper implementations, target ABI, per-frame transform updates and runtime sprite effects are not checked. Timer startup tests do not check every payload destination. This finite matrix does not exercise every combination of actions or callback mutation. Compilation has a 30-second bound per build; execution has a 5-second bound per case. A timeout is a runner failure, not a semantic kill.

`manifest.json` binds raw bytes of this README, the runner, harness and local `.gitattributes`, plus canonical LF identities of the three pinned source inputs. The runner verifies these identities before compiling. The manifest intentionally does not hash itself. The local attributes override only `harness.cpp` to LF, preserving its raw artifact identity beneath the repository-wide C++ CRLF policy. No production files are included or changed.

## Typed sound-interface hypothesis

This is an isolated hypothesis layered on the A006 local-vector proposal, not production 72aa and not an adopted source change. Only BulletTransform.cpp changes: include the genuine EnemySoundQueue.hpp, declare the existing g_MainSoundOwner storage compatibly, call QueueSoundSample(transformSound, 0) at vector/polar sites, and QueueSoundCue(payload.int0, position.x) at explicit sound. Existing gates and statement placement are preserved. Target caller identity, arguments, private ABI, memory freshness and native code generation remain unproved by these tests. No native/remote experiment is included.

The shared EnemySoundQueue.hpp bytes and its existing noinline attributes are unchanged in the proposal. Host extraction removes exactly two __declspec(noinline) tokens and retains the complete type and its offsetof assertion; this exact policy and its source pin are bound in manifest.json. No blanket macro erases attributes. The preexisting token-different EnemySoundQueueView definition in BulletManager.cpp remains an unresolved ODR-consistency concern outside this source-only scope.

The host defines an aligned g_MainSoundOwner byte buffer and starts a real EnemySoundQueueView lifetime with placement new. Receiver mutants use a second separately aligned, live object. Both typed member stubs snapshot parent state immediately at entry, record method/receiver/int-or-float arguments, and append the old generic event ID. They never call a real sound queue or convert Cue floats to integers. This lifetime arrangement proves neither target ownership nor alias legality.

Finite Sample fixtures cover vector/polar, indices 0/1/17, IDs -1/0/27, active flags clear/set with admission allowed, and disallowed-active admission. They verify integer zero, exact receiver/method/ID, completed selected state initialization and preservation of unrelated state, cursor before increment, owner tag before continuation, and resulting continuation. Cue fixtures use distinct payload/configured IDs and X/Y, indices 0/1/17, configured IDs -1/0/27, active allowed/disallowed, fractional X, both signs of zero and quiet NaN. The next owner-tag action demonstrates ordering. Existing child callback continuation additionally checks typed Cue identity. Optional synthetic helper mutation/freshness callbacks and extreme sound IDs are deliberately not implemented.

The 20 additional mutants test per-acceleration method/receiver/state ordering/cursor ordering, explicit method/ID/X/receiver/gates/continuation ordering, and polar admission. Every mutant must compile, execute, and fail its exact designated diagnostic. The explicit Cue-to-Sample cast mutant runs only the finite fractional Cue fixture, so NaN-to-int conversion is never executed. Early Cue continuation/cursor mutants also use that bounded fixture. This finite lane is explicit in the runner; baseline still executes all fixtures. Compile errors, crashes, timeouts, survivors and unrelated assertions are not kills.

The native hypothesis overlay preserves exact raw LF bytes outside the documented replacements. Host canonical-LF source identity is a separate portability policy. No forced ABI/register/padding/volatile changes, shared-header edits or source-attribute changes are made. Source-change evidence is maintained outside the five-file test package. Prior frozen packages remain separate.
