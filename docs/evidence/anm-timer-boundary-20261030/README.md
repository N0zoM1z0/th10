# ANM timer call boundary — 2026-10-30

The actual WAIT callee AnmVmTimerView::Decrement at **00428DD0..00428DE3** is
now complete20-byte exact, including its sole field, in two independent canonical
cold compilations. Add remains88/all4 exact. **ExecuteScript remains9960/9964
nonexact** after binding its complete350 fields and95 local/table destinations.
This adds20 codegen bytes; wrapper origin remains indeterminate, so authored
exact bytes stay96,725/275,729. The [results](results.json) retain bounded proofs.

Fresh target decode confirms20 contiguous bytes, RET at+13, five preceding and
twelve following CC bytes. EAX carries a signed integer, ESI the timer. The body
negates the integer, converts it to a float stack argument, calls Add44BF40 at
REL32 field+E, then returns. The full executor contract binds its WAIT call to
this entry at parent field+7CE. The wrapper is not a new call-identity guess.

The existing ordinary Decrement member already expressed negate/convert/Add.
VC7.1 had inlined Add into a68-byte wrapper. Applying the existing noinline
boundary annotation to Add's declaration preserves the real target call and
naturally recovers20 bytes and the ESI/EAX interface. Decrement itself receives
no ABI annotation, invented return or padding. The actual call also preserves
conversion to a float argument before Add, including its x87 store boundary.
The original annotation/TU spelling is unproved. The maintained inline definition
is in AnmManager.hpp; the declared PDB materialization is AnmManager.cpp.

The complete ANM executor was compared before editing. All13 project pins, five
objects and the previous image hash were valid. The before/after strict bound
comparisons both yield9960/9964 with exactly the same four ordinary residuals:
child91 ECX/EDX at+113/+115 and radial Z operand order at+249A/+24A0. Every one of
350 fields, including95 fixed local/table destinations, is still validated by
the unchanged full binding contract. This closes an actual callee; it does not
close the parent or justify repeating parked record/vector controls.

Canonical acceptance retains the existing one-TU Add recipe: pinned /O2 /GL,
real ExecuteScript entry, original link profile. The new wrapper's sole REL32
binds Add44BF40, whose complete88-byte body/all4 fields is also checked in the
same image. The separate five-input /GL /GS diagnostic independently passes both
complete bodies with the actual support sources and original profiles.

Only six existing exact bodies with actual Add REL32 bindings were additionally
selected: horizontal-wrap148, BulletRuntime885, faith-extension93, faith-decay65,
LaserLine1001 and StageDrawLate929. Together with Add88 and Decrement20, the first
coverage is8 units/3229 bytes/all164 fields. This is scoped interface preservation,
not a historical-cohort or all-header-consumer replay.

The initial eight-unit driver **failed** later in BulletRuntime after a Wine
page fault polluted a tool path and produced LNK1104. It did not return success.
The completed Add/Decrement and horizontal-wrap outputs were frozen, source-epoch
hashes checked, and independently revalidated:3 units/256 bytes/all13 fields.
Required toolchain --execute smoke passed. Only the failed/uncompleted five
units were replayed, giving the remaining2973 bytes/all151 fields. A second
independent cold selects Decrement20 and Add88:108 bytes/all5 fields. Full logs,
initial error, frozen images and the split reports remain under the matching
scratch/build roots. Completed groups were not rebuilt merely for recovery.

Exact codegen and source presence do not distinguish this retained wrapper from
compiler-generated/outlined code. Original class/TU, authored origin, production
ownership, product build, runtime and portability closure stay separate. No
boundary/origin or authored denominator changes are made. Comparator and profiles
are unchanged; no host failure/sanitizer scenario is required by this exact claim.

Current valid five-input warm context:
`.analysis/gpt-6.1-sol/anm-executor-context-20261030/final-context.json`.
Older Stage/Player contexts retain their previous header/source epochs. Revalidate
pins and rebuild changed inputs before reuse; do not relabel them as current.

```sh
scripts/repo-python scripts/replay-exact-units.py --unit anm-vm-timer-decrement --unit anm-vm-timer-add --json
scripts/repo-python scripts/ci.py
git diff --check
```
