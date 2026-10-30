# Complete Player main: value copies and state2 cleanup — 2026-10-30

**The complete main remains nonexact:3096/3112 native/target bytes.** This batch
restores its history-copy and state0 scheduling, fixes the actual state2 callee's
stack cleanup, and cold-reproduces both complete candidates. New exact credit
is **zero**. Existing exact owners retain their separate canonical recipes.
The [results](results.json) contain fresh target review, all native field/local
identities, controls, source/object pins and selected canonical replay reports.

## Retained source changes

PlayerPositionPair now has ordinary value assignment: copy x, copy y, return
*this. Its size remains8. Both main history loops recover the target two-word
loads/stores, source LEA, destination cursor and loop alignment. The first history
slot also uses this assignment on the existing contiguous fixed-position pair,
matching the view already used by PlayerInitialize. This restores the target
load/store order instead of hoisting the Y load across the X store. Reusing the
existing timer SetCurrent(0) member recovers the state1 store before timer flag
loads. Original pair/timer class names and original source spelling remain unknown.

Fresh state2 target review consumes all211 instructions/all796 bytes at
4269D0..426CEB. Its sole main caller pushes one Player pointer; both target exits
are RET4. The old cdecl declaration emitted caller ADD ESP,4. The declaration
and definition now use __stdcall to model the observed cleanup, and the main
caller's extra cleanup disappears. This is a real stack contract repair, not
an inferred original annotation or forced register ABI.

Actual Gui.cpp and Hint.cpp definitions are included with their existing plain
/GL profiles in the diagnostic graph. They restore state2's EAX GUI/ECX count
and ESI Hint receiver interfaces. Their complete77-byte GUI and104-byte caution
bodies pass the existing exact contracts in the cold image. The full state2
candidate remains **786/796**, including all19 native/target fields. Its option
loop, stack home and ordinary register/order differences remain open. The target
has one additional four-byte local slot; no inert local or padding is introduced
to imitate it. The actual script-initialization interface also remains a lead.

## Complete ownership and comparison

The [prior ownership review](../player-update-owner-20261030/README.md) continues
to define all3112 main bytes:3089 code including retained loop alignment,3 more
alignment bytes and20 bytes of dispatch data. Fresh decode again inventories
all112 target fields; the native owner has110. Both scaled-decrement FMUL-one
fields remain missing, alongside ordinary scheduling/motion/vector differences.

All five state table entries now have the exact target owner-relative destinations:

| State | Target/native hex offset |
| --- | ---: |
| 0 | 21 |
| 1 | 234 |
| 2 | 379 |
| 3 | 5F5 |
| 4 | 366 |

The table base itself is **C04 native versus C14 target**, sixteen bytes early.
That field cannot be normalized to excuse the changed destination offset.
Other fixed locals after the residual timer differences are also not declared
exact. Complete PDB ownership, all bytes, field identities and each local table
destination participate in native warm/cold identity checks. A zero-reuse
seventeen-input /GL cold reproduces all3096 main and786 state2 native bytes,
with all110/19 field identities. This is reproducibility of nonexact candidates,
not a target match or partial exact credit.

Twenty-five selected existing exact bodies pass3511 bytes/all118 fields in this
same cold image. Nine earlier native nonexact bodies, including destructor262,
remain byte/field-identical to entry. Complete native consumer review finds initializer1404 still the same size but
changed ordinary bytes/field positions, movement1588→1576, and constructor250
byte/field-identical. The first two remain nonexact; this review grants no target
credit and does not claim their codegen is preserved. Canonical cold selection separately covers only
the affected owners/interfaces; source/header graph membership is not a demand
to replay the historical cohort.

## Bounded controls and stopping evidence

| Hypothesis | Complete main size | Observation |
| --- | ---: | --- |
| Pair value assignment | 3096 | Restores both copy loops/alignment; retained |
| Ordinary effect-row member | 2916 | Outlined; does not reproduce complete main |
| Forced inline effect member | 3096 | Entire native main/field identities equal to pair control; reverted |
| Correct state2 stack cleanup | 3096 | Removes erroneous caller cleanup; retained |
| First history slot as pair | 3096 | Restores X-store/Y-load order; retained |
| Add actual Gui/Hint callees | 3096 | Main ordinary bytes unchanged; actual state2 interfaces restored |
| Timer compound subtraction | 3096 | Whole native main/field identity unchanged; FMUL still absent; reverted |
| Ordinary history member | 2952 | Outlined; reverted |
| Existing SetCurrent member | 3096 | Restores state0 store order; retained |
| Tick compound addition | 3096 | Whole native main/field identity unchanged; reverted |
| Shared EnemyMotion view | 3096 | Changes ordinary operand choices; residual mismatch remains; reverted |
| Derived dual cursor | 3100 | Motion base recovers, but prefix registers and cursor update regress; reverted |
| Independent dual cursor | 3116 | Adds an unwanted spill/alignment and retains wrong motion base; reverted |

Size is not an acceptance score: neither3100 nor3116 is retained merely for
being near3112. No fake ABI, register hint, copied target code, inert padding,
volatile barrier or new compiler flags are introduced. The complete missing
FMUL and motion-cursor evidence is retained; byte-neutral controls are parked.
Reopen only with new evidence and a prediction that distinguishes the model.

## Reproduction and validation

The final graph and56 source pins are retained in results.json. Its primary
source remains Enemy.cpp, with the preceding fifteen-input initializer-entry
context plus actual Gui.cpp and Hint.cpp. Every input retains its pinned profile
and /GL. The real InitializeVm entry remains a diagnostic interface constraint;
original TU/production partition and runnable product closure are unproven.

The [reproduction script](reproduce.py) verifies target/source pins and cold-builds
that recorded graph with the unchanged local driver:

    timeout 700 taskset -c 0 nice -n 10 scripts/repo-python \
      docs/evidence/player-main-shapes-20261030/reproduce.py \
      --output-dir build/gpt-6.1-sol/player-main-shapes-reproduction

`--prepare-only` verifies and prints the graph without compiling or creating the
output directory; that path is exercised at this checkpoint. The actual17-input
cold receipt is from the retained private probe using the same cold_link driver.

The target must first pass verify-target.py; toolchain declarations must pass
verify-toolchain.py --check. No Ghidra observations or toolchain changes were
needed. Comparison jobs are serial, CPU0/nice10, bounded and logged. Source and
object hashes are checked before reuse, and cold receipts show zero reuse.

Selected canonical cold reports cover SpawnShot717, input174, bonus81, clear44,
the two result openings411/411, rows105, callback7, angle67, reset273, GUI77 and
caution104: **12 units/2471 bytes/all94 fields**. Default seven-stage CI and
whitespace checks pass. Exact totals remain98,624/275,752 authored owned bytes,
489 authored exact owners. Boundary/origin/source status is retained; product,
runtime and portability remain separate and incomplete.
