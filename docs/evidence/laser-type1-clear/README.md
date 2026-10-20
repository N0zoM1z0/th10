# Type1 laser clearing

The complete **623-byte authored owner41F400–41F66E** now has maintained source
in [LaserType1Clear.cpp](../../../src/LaserType1Clear.cpp). This adds source
presence, with **no new exact bytes or authored denominator change**.

## Target observations

Local readOnly/noanalysis Ghidra attests all412289 mapped text bytes through
465A80, SHA256 cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f.
The review rolls its transaction back and checks unchanged bytes. Its complete
Clear body is623 bytes, referenced by slot14h at46DA24. Dense raw decoding,
two RET4 exits, internal branches, the preceding12 CC bytes and following
single CC byte are independently checked by [the audit](../../../scripts/report-laser-type1-clear.py).
The separately observed226-byte slot20 query41F670 remains an untracked frontier;
it gains no source, origin or exact claim here.

Clear receives ECX=this plus one stack mode and returns an **integer count in
EAX**. Ghidra's provisional float return and extraout_EDX are wrong. The
30-byte polar helper41F800 writes XY only and never changes EDX. The caller's
XOR EDX,EDX therefore establishes both zero stepZ and the empty-path integer
result. This follows from complete raw callee/caller evidence, without changing
its declared ABI or adding artificial register arguments.

The first sample is angle/magnitude6 plus the original position. XY step then
doubles; Z step is explicitly zero. Initial ordered12<length and subsequent
ordered distance+6<live length exclude equality and NaN. Each iteration counts
**before any boundary rejection**, including wholly offscreen segments.
Point XYZ advances after each iteration, including addition of zero to Z;
negative zero can consequently become positive zero after the first sample.
Finally stateC=1 is written after all script executions, and count is returned.

Only points passing four negated **margin16** rejects create an animation:
`!(x+16<=-192)`, `!(x-16>=192)`, `!(y+16<=0)`, `!(y-16>=448)`.
NaN passes these rejects. The target computes y+16 during the bounds check,
stores it with FST and reuses that stored value as the VM Y offset after
allocation. The maintained used local preserves this read/storage timing.
Original x87 precision/control/exception state remains unknown.

Inside that rectangle the target reads signed color466, selects2*color+17,
and captures current laser manager47781C resource458. It calls the actual
renderer allocator449950, sets VM flags35C|40000000/layer0, and writes
**positionOffset340**, with224 added to X and the cached16 offset for Y.
Actual binder43E710 executes the script. A fresh renderer491C10 then calls the
**outlined registrar4489D0**; unlike type0 Clear, no copied registrar body is
present in this target owner.

After script execution a nonzero mode and nested four negated **margin32**
rejects permit actual ItemManager477818/Spawn41BB00 with
8/FFFFFFFF/-pi/2/0.6. Resource, color, allocation owner, registration owner,
Item manager and length are read at their observed times. Geometry is captured
once. A completely offscreen line can count its segments without dereferencing
any of those owner globals. No artificial callbacks are added to allocation,
reset, list registration or kind8 item spawning.

## Compiler observations

Two independent full four-TU /GL /GS diagnostics freeze18 source/header inputs:
LaserType1Clear.cpp with AnmManager.cpp, EnemyLaser.cpp and ItemManager.cpp.
The initial generic world-creator call stays outlined and emits503/623 bytes.
The maintained direct allocation/binding/registration sequence emits627/623,
274/507 comparable ordinary bytes. The five call identities and loop/bounds
protocol agree, but the preserved receiver/spill, binder's private registers,
VM write scheduling and other register choices differ. This is **nonexact**;
size closeness is not exact credit. No profile sweep, forced attributes,
volatile/barrier, copied registrar, fake ABI or byte padding is used.

The helper registrar118 and polar30 are structurally equal in this graph, with
no duplicate credit. World creator116/118 remains nonexact. Item Spawn has795
PDB code bytes versus793 target code bytes; its819-byte physical owner also
includes26 bytes of alignment/tables, so that size diagnostic is not a full
extent replay. The diagnostic image has unresolved anchors and is not runnable.
[The receipt](native-proof.json) pins inputs, observations and retained artifacts.
No existing canonical primary/support input changes in this batch, so no
historical cohort replay is needed and no match manifest is added for Clear.
The superseded generic-creator carrier and completed sanitizer carrier are
pruned after recording receipts; the final627-byte native carrier is retained
for the unresolved claim. Previous diagnostic artifact hashes remain historical.
This batch frees1,254,669 bytes. All93 public CI checks pass in one fresh
complete canonical run, including the new baseline. Final target, tracking,
toolchain, progress, documentation and whitespace checks also pass.

## Host evidence and limits

[The source-extracted fixture](../../../scripts/test-laser-type1-clear.py)
executes the actual Clear, allocator, registrar, node insertion and full Item
Spawn bodies. The actual world creator closes unexercised non-kind8 branches
of full Spawn. Polar math and script execution are explicit host roles; script
execution alone can mutate live state. Header assertions and constructors
receive explicit host adaptations over typed live objects.

The positive scenario suite and **29 strict named-CHECK mutants** pass with
ASan, UBSan and leak detection. Controls distinguish count-before-reject,
outer16/inner32, equality/NaN, no-owner offscreen execution, sampling/stride,
signed-zero Z, live owners/color/length, flags/offsets, actual lists/IDs,
kind8 fields/busy-ring behavior, and retirement/return. Compile failures,
sanitizer failures and unrelated CHECKs do not count as successful controls.
These tests do not establish original object lifetimes, all Item kinds,
allocation failure, unbounded lengths/counter overflow, native x87 behavior or
gameplay/runtime reachability.

The ignored MIT [reference](https://github.com/levzzz5154/th10-decomp), pinned at
c6791d82dc9f02e55eca45a5314f900a85481efd, supplied a source-shape hypothesis
in matching2/msvc/src/game/laser/fun.cpp. Its forced noinline/code placement,
assembly wrappers and volatile Z workaround are not adopted. Target facts
above come from TH10-local evidence; [MIT attribution](../../licenses/th10-decomp-MIT.txt)
is retained. The pre-existing D3D evidence stays untouched.

```sh
scripts/repo-python scripts/report-laser-type1-clear.py
scripts/repo-python scripts/test-laser-type1-clear.py --sanitize
scripts/repo-python scripts/probe-ltcg-backlog.py \
  --source src/LaserType1Clear.cpp \
  --entry 'src/LaserType1Clear.cpp=EnemyLaserType1View::Clear' \
  --support src/LaserType1Clear.cpp=src/AnmManager.cpp \
  --support src/LaserType1Clear.cpp=src/EnemyLaser.cpp \
  --support src/LaserType1Clear.cpp=src/ItemManager.cpp \
  --profile-flag /GS --json
```
