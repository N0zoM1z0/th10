# Complete Stage instance drawing — 2026-10-29

StageDrawInstances403A30..403C7F adds **592 authored exact bytes**. Two independent
canonical eleven-input StageCreate cold links reproduce its complete PDB owner,
all22 semantic fields and18 fixed local destinations. The156-instruction owner
retains its12-byte frame, two stack arguments (stage/layer) and RET8.
[Results](results.json) retain source/include and field-contract hashes, distinct
PDB GUIDs, artifact identities, branch destinations and selected guard results.

The complete body installs the camera viewport, walks signed-sentinel instance
and primitive lists, filters object layers, performs distance/projected-bounds
culling, sets object flags, adjusts quad VM offsets/scales, updates fog, draws
VMs and updates counts. Every branch belongs to the comparison. No host failure
scenarios or runtime claim are added.

Fresh native analysis corrects a real source error: the target writes the three
VM positionOffset components at340/344/348, rather than position at334/338/33C.
The per-instance culling snapshot uses an ordinary explicit XYZ value
constructor. This avoids an extra position induction pointer/spills, recovers
the target frame and two-stack-argument interface, and preserves the exact
Early1099/Late929 callers. In the corrected camera/fog context, implicit copy let LTCG
promote the stage input into EBX/RET4 and changed both callers; size proximity
was never accepted.

The bounds operation is a natural const StageObjectView member. All four actual
inputs remain meaningful: object/this in ECX, position in EDX, camera in EAX,
and distanceSquared on the stack with RET4. Its genuine argument ordering
restores the final camera/position/object setup schedule. No fake calling
convention, forced register, unused parameter, address-escape caller, noinline,
volatile, padding, code-byte patch or profile change is used. The source class
and method spelling remain provisional.

Camera activation uses the existing InstallViewport operation. Its cache write
now uses the actual Main+388 member. Fog uses the existing Main operation in the
two actual constant branches, binding Main+750 cache and Main+8 device. The old
unused standalone viewport/fog declarations are removed. Two Early fields and
one Late field now bind the real Main base plus388; their target destinations
and encoded bytes are unchanged. Coincident diagnostic anchor names do not
establish source identity. Camera/active-pointer alias storage remains open.

Each cold selects only `stage-draw-instances` and affected `stage-draw-early`,
sharing one unchanged eleven-input graph. Thirteen old same-image contracts
pass3128/all198 twice: Early1099, Late929, camera330, selector60, fog53,
adapters7 each, real Flush133, DrawLayer67, timers88/57, factory82 and Draw216
including its complete table. Header-inline SetCurrent is physically emitted
by StageScript.cpp here; its verified PDB owner is selected without changing
its old byte/field contract. A separate explicit cold of `stage-draw-late`
passes929/all78 in its original ten-input recipe. No full historical cohort or
unaffected Main recipe is rebuilt. Six scalar occurrences are also raw-equal,
including both zero constants used by the new owner.

The complete1984-byte bounds callee remains **1929/1984 nonexact**. Its address
and actual ABI are bound by the exact caller; its body receives no exact credit.
Default camera remains282/287 nonexact. Source class/TU identity, physical data
ownership, single-image/product/runtime closure and portability remain open.

```sh
scripts/repo-python scripts/replay-exact-units.py --unit stage-draw-instances --unit stage-draw-early --json
scripts/repo-python scripts/replay-exact-units.py --unit stage-draw-late --json
scripts/repo-python scripts/ci.py
git diff --check
```

The comparator is unchanged. Only this complete592-byte owner adds exact credit;
source mapping, reviewed boundary/origin and authored denominator are unchanged.
