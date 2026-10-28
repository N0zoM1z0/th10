# Laser line clearing and shared Item Spawn interface

The maintained source now covers the complete **609-byte authored owner
41E260–41E4C0**, virtual slot14h of table46DA60. No boundary or authored
byte-denominator change and **no new exact bytes** are claimed. Source presence,
exact codegen and product/runtime closure remain separate.

## TH10 target observations

A fresh local read-only/noanalysis Ghidra review attests all412289 mapped text
bytes through465A80, SHA256
cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f.
The review rolls its transaction back and leaves the mapped bytes unchanged.
Complete raw decode establishes the two RET4 exits, internal branches, slot14h
reference and following15 CC bytes. [The target audit](../../../scripts/report-laser-clear.py)
rechecks these facts against the hash-verified original Japanese1.00a input.

The function samples the initial angle once with magnitude6. Position starts
at that displacement plus the original XY and original Z. The step's Z is
explicitly zero; its XY doubles for the12-unit stride. Entry requires ordered
12 < length. Each completed iteration increments distance by12 and continues
only while ordered distance+6 < the **current** length. In particular, equality
and NaN terminate. The target retains a three-float construction/copy and adds
zero to sample Z after each segment; it does not just preserve a Z bit pattern.

Each iteration selects signed request.color at44A, script2*color+17 and the
current laser manager47781C's resource458. The ANM world creator allocates a VM,
sets flags35C bit40000000/layer0 and writes **positionOffset340**, adding224/16.
It calls the actual script binder43E710 and then registers with the current
render manager491C10. The target inlines the primary registrar4489D0, preserving
its linked-list writes and nonzero id rule. Script execution can change live
state; subsequent iterations sample resource, color, allocation owner and length
again. Slot14h finally sets stateC=1 and returns the number of segments, even
when no segment was created. It does not set deletion byte50.

A nonzero mode additionally spawns a kind8 item if the point overlaps the
playfield with margin32. The four **negated** rejects retain unordered acceptance;
replacing them by ordered positive tests changes behavior. The actual helper is
ItemManagerView::Spawn41BB00, receiverEAX loaded from477818, pointECX, stack
kind/color/angle/magnitude, RET16. Arguments are8/FFFFFFFF/-pi/2/0.6. The
maintained source shares ItemManager.hpp and g_ItemManager with this owner.

The four already maintained collision owners41D880/41DD80/41EB00/41EFA0 use the
same477818 receiver and41BB00 callee. Their invented
EnemyLaserHitEffectManagerView::SpawnCancelEffect/global alias was removed in
favor of that actual interface. Their source mappings and nonexact status are
retained; older diagnostics describe earlier source contexts.

## Native observations and limits

Three complete four-TU /GL /GS diagnostics freeze18 source/header inputs:
LaserClear.cpp with AnmManager.cpp, EnemyLaser.cpp and ItemManager.cpp.
Aggregate construction and direct AnmFloat3 construction each emit597 bytes;
ordinary default construction followed by assignment preserves the target's
three-float temporary/copy and emits613 bytes against609. The final source
uses this existing natural vector API and typed ANM/Item point arguments.
The latter has204/521 comparable ordinary bytes; a similarity score grants no
acceptance. The actual script binder's private register assignment and ANM
flag/layer/list scheduling remain different. No artificial padding, volatile,
code placement, forced inlining, fake ABI or byte copying is used.

The world creator emits116/118 in this context, and Item Spawn's PDB code
contribution is795 bytes against its793-byte target code span. The819-byte
Item physical owner additionally includes26 bytes of alignment/tables; a raw
PDB size comparison is not a complete ownership replay. The30-byte polar helper
is structurally equal here, with no new credit. Unresolved anchors make these
diagnostic images unsuitable for execution. [Native receipt](native-proof.json)
records the frozen contexts and the focused existing-unit guards; there is no
new canonical match unit for Clear.

The four necessary existing canonical guards pass: polar30, bounds91/all4
fields, Motion0010269/all10 fields and Motion0040329/all8 fields. They cover719
complete bytes and22 fields, using current frozen input closures. No full
historical cohort replay or new exact increment is claimed. Earlier laser-init
cache hashes are historical for the changed line header/EnemyLaser.cpp; the
current bounded carrier/cache is separate. Superseded native controls and the
finished host carrier are pruned after recording their receipts.
All91 public CI checks, final target/tracking/toolchain/progress/documentation
and whitespace checks pass. Cleanup frees1,865,298 bytes from this batch.

## Host evidence

The actual Clear body runs with actual ANM world/screen creators, allocator,
primary registrar, node insertion and complete Item Spawn
body. Tests exercise the kind8 path, its busy-slot count/cursor rule, timer and
motion fields, and the actual creator's world offsets, flags, primary links and
zero-id skip. Script execution and polar math are roles. The script role alone
can mutate live globals/state; no callbacks are injected into VM reset, pool
allocation, list registration or kind8 spawning. Native assertions and outer
constructors receive explicit host adaptations; all objects have typed live
storage. No original C++ object-lifetime claim follows from these host adaptations.

Twenty-five strict named-CHECK mutants pass ASan/UBSan/leak checks. They cover sampling, signed-zero Z addition, exact/NaN
loop boundaries, live length/color/resource/Item owner, mode/bounds/NaN rules,
script/world coordinates, kind/velocity, retirement order/state and return.
The runner rejects compile errors, sanitizer failures and unrelated CHECKs as
mutation evidence. See the test (`e1c4075:scripts/test-laser-clear.py`, retired host tooling) and
fixture (`e1c4075:scripts/fixtures/laser-clear.cpp`, retired host tooling). Assets, allocation failure,
native x87 precision/exceptions, invalid/unbounded counters, original object
lifetimes and game runtime reachability remain open.

The ignored MIT [reference repository](https://github.com/levzzz5154/th10-decomp)
is pinned at c6791d82dc9f02e55eca45a5314f900a85481efd. Its
line_effect_create_all_segments.cpp supplies hypotheses only. Its code-segment
pragma, forced noinline, copied registrar and volatile Z workaround are not
adopted. All accepted target facts above come from TH10-local evidence.
[MIT attribution](../../licenses/th10-decomp-MIT.txt) is retained. The pre-existing
D3D evidence directory is untouched.

```sh
scripts/repo-python scripts/report-laser-clear.py
scripts/repo-python scripts/test-laser-clear.py --sanitize
```
