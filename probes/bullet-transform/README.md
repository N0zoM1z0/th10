# Authored BulletTransform protocol host regressions

Run `python3 run.py --mutations` or `python3 -O run.py --mutations`.
Requires Python 3 and g++ supporting C++17. No network or external services.
By default the source pair is read from the repository `src` directory (`ROOT.parents[1]/src`). Use `--source-dir PATH` for another location. Source pins identify canonical LF bytes: only CRLF is converted to LF, and any remaining bare CR is rejected. No other normalization is permitted. The pins equal the original raw LF source hashes; this explicit policy supports the repository CRLF checkout rules.

## What executes

The runner compiles the complete, unmodified `BulletTransform.cpp` implementation, including its angle helper; only its local header include is removed. The complete declarations/enums from `BulletManager.hpp` are used after removing two dependency includes, 36 target-ABI typedef assertions, and the `__stdcall` spelling. Dependency stand-ins instrument calls. Canonical LF source hashes are checked before extraction. Temporary build products stay under this directory and are cleaned up. No build products or saved run output are part of this source-only checkpoint.

This is a host behavioral regression suite for the current authored source, **not evidence of equivalence to the TH10 binary**, target layouts, compiler ABI, assembly, or runtime helper implementations. Host pointers have host width; target offset assertions are intentionally not exercised. The timer stub records `SetCurrent` arguments/call counts, not target timer internals. Trigonometry and sprite initialization dependencies are stubs, not separately verified implementations. No production source changes are made.

## Independent finite oracle

The oracle is a set of hand-authored concrete expected outcomes, using numeric action values rather than reusing implementation enum constants. It does not reimplement the dispatch loop. The entire production function is executed against the fixtures.

- Child 0x00400000 consumes two records: second kind/gate ignored; signed 8-bit color, signed 16-bit counts, low-byte start index, floats, flags, zeroed descriptor fields, inherited position, transform sound -1.
- All 18 records, including an otherwise-unused final-record canary, copied before the spawn callback mutates parent storage.
- Callback can change the parent's cursor to 7: continuation increments the live cursor to 8 and executes its newly written record. A separate callback mutates record 2 to prove continuation reloads parent storage.
- Fade cancellation occurs after spawn; parent execution continues. Standalone despawn also continues.
- NONE, upper bound 18, SKIP, unknown action, active gating, finite forward jump, owner/cull continuation, sound order.
- All ten timer-starting action variants select the expected timer and supply either zero or payload duration. Direction counters and deceleration vector reset checked.
- Angle boundaries -990/+990 and NaN; NaN angle takes aim branch while NaN direction speed retains parent speed.

Domain: readable/writable parent with cursor in [0,18]; child records start at most at index 16; jumps remain nonnegative and bounded and have no unbounded cycles. Callbacks are synchronous and do not free parent storage. Tests do not validate or repair malformed scripts, negative cursors, out-of-bounds child index 17, or unbounded jump cycles. Integer narrowing cases assume the host's conventional two's-complement narrowing behavior, as observed in the run. No fast-math.

## Mutation discipline

Each mutant is a single uniquely anchored edit of the extracted source. Every mutant must compile, execute, and exit with the explicit semantic-check code 31 and a `FAIL:` diagnostic. Compile failure, timeout, signal/crash, or other exit code is an error, never a kill. There are ten mutations covering color signedness, copy count, second-record cursor, post-callback live cursor, fade, despawn continuation, SKIP gate, timer duration, NaN branch, and two-record index consumption.

Python validation uses explicit exceptions, and C++ checks use explicit `exit(31)`, so Python `-O` and C++ `NDEBUG` cannot silently remove validation. Compilation currently uses `-O0 -fno-strict-aliasing` to avoid pretending that the authored reinterpret-cast accesses are portable optimized C++.

## Coverage exclusions and frozen artifact binding

This suite is not exhaustive action coverage. Bounce variants, cancel-behavior assignment, sprite initialization, and transform-sound gating are not checked. Timer startup tests do not check every payload destination. Compilation has a 30-second bound per build; execution has a 5-second bound per case. A timeout is a runner failure, not a semantic kill.

`manifest.json` binds raw bytes of this README, the runner, harness and local `.gitattributes`, plus canonical LF identities of the two baseline source inputs. The runner verifies these identities before compiling. The manifest intentionally does not hash itself. The local attributes override only `harness.cpp` to LF, preserving its raw artifact identity beneath the repository-wide C++ CRLF policy. No production files are included or changed.
