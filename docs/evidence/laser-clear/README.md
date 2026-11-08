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

Each iteration reads signed request.color at44A and the current laser
manager47781C's resource458. The target then increments the sample count,
computes script index2*color+17, and calls the VM allocator449950 directly.
It sets flags35C bit40000000/layer0 and writes **positionOffset340** with
x+224/y+16/original z. It directly calls script binder43E710 and then inlines
the primary registrar4489D0, preserving its linked-list writes and nonzero-id
rule. It does not call the world creator448DB0 from this owner. The order
of color/resource/count/script-index/allocator operations matters to the
private LTCG register assignment. Script execution can change live state;
subsequent iterations sample resource, color, allocation owner and length
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

The earlier three four-TU /GL /GS diagnostics in the historical native
receipt predate the direct-allocation source correction. Their597/613-byte
Clear candidates and world-creator measurements are not measurements of the
current source. They remain useful historical controls, not current-source
proof.

## Direct ANM call restoration — 2026-11-08

The current source now follows the target's direct allocator/binder/registrar
path. A fresh focused seven-TU /GL /GS link used AnmManager.cpp, Gui.cpp,
EnemyLaser.cpp, LaserClear.cpp, LaserType1Clear.cpp, Player.cpp and
EnemyEclDispatcher.cpp. The selected Clear owner emits623 bytes against the
609-byte target. After masking22 candidate link fields (88 bytes),164/521
ordinary target bytes match; the previous same-graph indirect-world-creator
baseline was630 bytes and24/521 ordinary matches. This is a real structural
improvement, not exact codegen.

The same image emits the binder as197/203 bytes (17/187 ordinary matches)
with a different private ABI: the candidate passes resource/VM/script in
ESI/EDX/EAX, while the target caller passes them in ECX/EAX/EBX. The current
source computes scriptIndex before AllocateVm, matching the target's
data-dependency order; that improved Clear to164 ordinary matches but did not
yet restore the binder's global private-register assignment. The neighboring
AnmVmView::Initialize leaf is byte-equal at273/273 in this image and already
has its own canonical exact unit. These observations are diagnostic only:
291 unresolved anchors remain, the image is not executable evidence, and no
new exact bytes are claimed. See the direct-call receipt
(direct-anm-call-20261108.json).

The old world-creator contribution and Item Spawn contribution in
native-proof.json belong to their frozen historical context. The30-byte polar
helper remains structurally equal there, with no new credit. There is no new
canonical match unit for Clear.

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

The previous host fixture and its twenty-five strict named-CHECK ASan/UBSan
mutants are historical evidence for the earlier world-creator path. The test
and fixture were retired and are not validation of the current direct
AllocateVm/binder/registrar source. The current direct path has focused native
compile/link evidence above, but no active host fixture. Native x87
precision/exceptions, allocation failure, invalid/unbounded counters,
original object lifetimes and game runtime reachability remain open.

The ignored MIT [reference repository](https://github.com/levzzz5154/th10-decomp)
is pinned at c6791d82dc9f02e55eca45a5314f900a85481efd. Its
line_effect_create_all_segments.cpp supplies hypotheses only. Its code-segment
pragma, forced noinline, copied registrar and volatile Z workaround are not
adopted. All accepted target facts above come from TH10-local evidence.
[MIT attribution](../../licenses/th10-decomp-MIT.txt) is retained. The pre-existing
D3D evidence directory is untouched.

```sh
scripts/repo-python scripts/report-laser-clear.py
scripts/repo-python scripts/relink-laser-clear-direct-anm-path.py
```
