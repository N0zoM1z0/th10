# Complete enemy runtime candidate — 2026-10-30

EnemyRuntimeUpdate at40DC80 now emits **2412/2412 bytes**, with647/671
address-normalized instructions aligned against the complete target. It remains
**nonexact** and adds **zero exact bytes**. The previous2346-byte checkpoint
aligned440/671 instructions. These diagnostic counts do not prove ordinary-byte
equality, field identities or fixed local destinations. See the complete
[native/control record](results.json) and [literal observations](literal-proof.json).

## Recovered source operations

World velocity now consumes a vector sum followed by a vector subtraction and
aggregate copy. Bounds calculate each alternative when its branch is reached,
retain the half-size float, and rebuild offset position through a full vector
difference. The destination of the second position interpolation is addressed
through the runtime owner while the base position remains captured separately.
These forms restore the target temporary storage and copy schedule.

The target readonly section supplies float values-192,192,0,448 and direction
thresholds-0.1/+0.1. Their addresses, bits and section flags are read directly.
The source uses these observed literals instead of six undefined writable-global
aliases. Original data names and physical storage ownership remain unknown.

Direction selection tests velocity below the negative threshold, then above the
positive threshold, then selects zero. The target x87 unordered route also selects
zero; the old inverse nested predicates selected the negative branch. This is a
source-fidelity correction established from branch/status flow, not a game runtime
or exception-execution test. The initialized script-offset value precedes these
predicates; a switch restores the three previous-direction paths.

Tracking comparison retains two Player-X reads and evaluates the tracked distance
before the candidate distance. Animation transitions use paired mode/flag predicates.
Flash handling places the zero-frame path first, selects the two actual sound cues
through separate calls, and reads threshold values through the full enemy owner.
The VM loops consume an eight-entry countdown and retain their manager for the later
lookup. Update Tick uses the existing timer operation, including both scale reads.

## Actual callees and verification

The explicit ten-input graph adds actual EclVm, Player, PlayerCollision and
EnemyEclDispatcher implementations to the earlier six-input graph. This restores
real outgoing ECL, damage, collision and sound calls without fabricated conventions.
Every profile and source pin is recorded; PlayerCollision uses the pinned primary
/GL diagnostic profile. This graph is a compiler-context hypothesis, not original
TU or product closure. Existing canonical recipes and profiles are unchanged.

One complete cold rebuild compiles all ten game sources with **zero reused objects**.
All36 recursive project inputs stay frozen. The complete decoded runtime contribution
still mismatches. Two strict same-image guards pass798 bytes/all14 fields: scalar
interpolation652/10 and ECL Host Run146/4. Explicit canonical colds then replay those
same two units in their original six-input and two-input recipes; all798/14 pass.
No unrelated historical cohort is rebuilt.

```sh
scripts/repo-python scripts/replay-exact-units.py --unit enemy-evaluate-scalar-interpolation --unit ecl-vm-host-run --json
scripts/repo-python scripts/ci.py
git diff --check
```

## Remaining differences and parked controls

The remaining differences are concentrated in world-sum Y/Z operand order, the
early eight-entry count and manager/ID register lifetimes, SetSubroutine's ECX
name versus native EAX, and life/state register scheduling. Equal extents and
647 aligned instructions do not close the owner. Motion160/171 and the30-byte
SetSubroutine helper also remain nonexact.

A position snapshot adds extra copies and grows the parent2432; it is restored.
An explicit subroutine-database capture is byte-neutral and restored. Their logs
and result records remain; repeat them only with new discriminating evidence.
The current fully cold ten-input cache is pinned at
`.analysis/gpt-6.1-sol/enemy-world-context-20261030/final-context.json`.
