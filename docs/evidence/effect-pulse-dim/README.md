# Complete Pulse and Dim callbacks (2026-10-20)

This checkpoint closes **329 authored exact bytes**: TransitionPulse154 at
0x43C460 and TransitionDim175 at0x43C310. Boundaries, origin classifications,
source mappings and denominator are unchanged from2ed9994. No partial-case or
address-bound-callee credit is used.

## Target-backed source changes

Pulse's first calculation branch already agrees. In its completion branch,
target0x43C4C9 clears alpha before0x43C4D0 stores the decremented count. The
baseline emits the opposite memory-store order and uses EDX instead of ECX for
the count. Moving the existing alpha reset before the existing unsigned
decrement naturally restores all154 bytes, including the signed completion
test and return0 behavior. No extra store, return or operand is introduced.

Dim's target places the released-frame limit return7 after the shared timer
tail. A positive `current <= 8` branch restores that layout but initially leaves
four ordinary bytes different: target slow-tick loads the scale value first,
then adds subframe; the baseline loads those two operands in the opposite order.
Binding a normal `AnmVmTimerView &timer = effect->timer` and using that actual
member for all five accesses restores the target load order and the complete
175-byte callback. The shared Tick algorithm, fields, types, compiler flags and
calling conventions are unchanged. No timer algorithm is copied into the caller.
Original names, source spelling, class/TU and production partition remain unknown.

Fresh local read-only Ghidra and independently pinned target decoding agree on
all three inspected callback extents and instructions. Full mapped `.text` hash
is checked before/after a rolled-back review. `target-and-controls.json` records
target hashes, decisive operations, all42 inputs and actual native outcomes.

## Rejected controls and scope

Both positive Dim branch spellings restore the tail but remain139/143 ordinary
diagnostic positions until the member reference is introduced. The ordinary
LinearShake439 frontier remains332/359: one FIDIV is scheduled before more of
the second RNG recurrence than in the target. A real timer reference and actual
duration/start snapshots are byte-neutral; staged amplitude assignments regress
to328/359. All LinearShake edits are restored. Its source is byte-identical to
the starting function; no439-byte or partial arithmetic credit follows.

Seven source diagnostics recompile only TransitionEffect.cpp and relink with
13 unchanged hash-checked objects from the attested retained Gui carrier.
Every other project input is verified unchanged before/after. These controls
are not independent cold proofs. The final retained source then receives two
fresh complete14-TU builds in the same real Gui updater graph. All42 project
inputs, profiles, entry and support order agree across those builds.

Each cold image strictly passes **1,398 bytes/all77 fields**: the two new owners
329/14 and ten existing guards1069/63, including initializer396 and its complete
table/address contract. New target fields are independently decoded from whole
target bodies: Pulse6 and Dim8. All constants are checked by payload and all
calls by reviewed semantic target. Complete static MAP/PDB contributions are
selected using the existing qualified-owner mechanism. No Oracle policy changes
or callback-size binding updates are needed.

```sh
scripts/repo-python scripts/build-match-unit.py --unit transition-pulse
scripts/repo-python scripts/compare-linked-function.py --unit transition-pulse --json
scripts/repo-python scripts/compare-linked-function.py --unit transition-dim --json
```

The MIT reference at c6791d82dc9f02e55eca45a5314f900a85481efd suggests alpha-first
completion and positive Dim branching in core/screen/updates.cpp and
dim_update.cpp. Its direct timer algorithm, volatile fields, forced compiler
controls, section pragmas and assembly are not adopted. All retained facts and
codegen conclusions come from TH10-local target/Ghidra/native evidence.

## Validation and remaining work

Host baseline retains14 scenarios/843 assertions and the documented kind8
precision gap. The two affected Dim mutation anchors preserve their original
boundary assertions with the new member spelling and inverted predicate.
Existing Pulse completion/underflow/return controls also compile and fail only
at their required assertion/exit86. Host dependency pins are unchanged.
Toolchain, tracking, progress, full public CI and whitespace checks pass.
`validation-summary.json` records the completed scope and cleanup. Duplicate
native/host experiments and superseded dumps are removed; one current canonical
carrier remains. The pre-existing D3D work is untouched.

FadeIn133, LinearShake439, Envelope349 and Cleanup127 remain nonexact. Original
class/TU ownership, production data/link closure, target runtime, dual-Oracle semantic
and port gates remain open. Revisit Linear scheduling with new TH10-backed
lifetime/partition evidence rather than repeating these restored controls.
