# Type1 laser lifecycle and state-table ownership

The maintained source now covers **1314 owned authored bytes**: the previously
omitted Initialize41E5C0/309 and Draw41EA40/173 plus Update41E700/832. The latter
was recorded as816 bytes but omitted its guarded four-pointer table41EA30.
The authored denominator therefore grows by498 bytes, independently of exact
codegen. Type1 Update832 and the existing type0 Update1001 now close
**1833 new authored exact bytes**. The separate C++ storage initializer41C680/219
remains indeterminate.

## Target and boundary evidence

A fresh local Ghidra readOnly/noanalysis review creates two temporary function
bodies and rolls them back. It attests all412289 mapped text bytes through465A80,
SHA256 cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f,
and checks that mapped bytes remain unchanged. Full raw decoding, returns,
neighbor padding, virtual references and the state table are independently
replayed by [the target audit](../../../scripts/report-laser-type1.py).

Table46DA10 slots4/8/C point to these three entries. Init has ECX receiver,
one stack request, RET4/EAX0. Update and Draw take ECX with plain RET. Update
has816 code bytes and16 data bytes. Its selector subtracts2 from state, rejects
unsigned values above3 and jumps through exactly four pointers at41EA30.
All destinations lie inside its code: state2→41E829,3→41E7DB,4→41E800,
5→41E845. The next entry begins immediately at41EA40. The original image has
no base relocations; ownership follows the guarded selector, destinations and
references, rather than relocation presence.

The two new entries are classified game-authored at medium confidence from
their TH10-local custom laser table, object geometry/script-VM protocol and
reviewed game resource/render/sprite-index interactions. This origin inference
is separate from the directly observed bytes and any compiler result. No
original class/TU or exercised runtime path is thereby established.

## Layout and behavior

The new partial runtime view keeps sizeD74, request424/1F8, VMs61C/9C8 and the
common position24,angle3C,length40,width44,growSpeed48,traveled4C prefix.
For type1, field4C is established here as the secondary-VM gate; the shared
name traveled does not identify its writer or motion ownership. It
exposes known virtual slots without installing the original vtable or replacing
the existing allocation/construction storage views.

TH10 consumers establish semantic aliases while legacy request names remain:

| Request offset | Meaning |
| --- | --- |
| 0/C | initial XYZ / per-update XYZ velocity |
| 18/1C | angle / angular velocity |
| 20/24 | maximum / initial length |
| 28/2C | full beam width / length growth speed |
| 30/34/38/3C | signed wait/grow/hold/shrink durations |
| 40/42/44 | signed type/color / flags |

Init copies126 DWORDs and sets state3 before initializing/stepping both VMs.
It uses actual VM wrapper404F30 followed by explicit ExecuteScript43EE30.
Signed type selects sprite base474170 and signed color adds the script offset;
the secondary uses color+103. Current laser manager47781C resource458 is sampled
for each VM. Live flag2 controls the primary20→10 change; the secondary change
is unconditional. Masks areFC63FFFF|600000 andFC7FFFFF|400000. After script
execution it samples owned geometry again, sets sound24 and **width2**, and
copies growth speed from request2C. It does not reset age, assign velocity,
reset traveled or compute polar velocity.

Update grows/caps length, normalizes angle with game-speed-scaled angular
velocity, optionally copies Enemy manager477704 slot0's world position1068,
then applies request velocity scaled by game speed. The whole12-byte coordinate copy uses the laser vector view across our
distinct maintained vector types; the original common vector class remains
unestablished. The four phases use signed current-frame comparisons and
fractional subframe interpolation. Wait completion resets age and enters4;
it does not immediately run the grow case. Grow completion resets to2 and
immediately checks hold expiry. Hold completion resets to5 and immediately
checks shrink expiry. Zero/negative durations can therefore chain or return1
in the same call, before VM execution. This callback resets age at transitions
but **does not tick it**; ownership of external advancement is not inferred.

States2/4 and ordered length>16 enable Player contact. There is no type0-style
width>3 gate. Width<32 uses half width; otherwise collision width is
width−(width+16)/3, with the observed float coefficient. Polar writes XY only;
the native scratch Z prefill is unknown. Hit1 invokes slot18 with the current
Player position, size32/32/0 and mode0. Graze2 uses signed age%5, actual Enemy
primary resource4776F0+3E0B50/script434 and sound28 with positionX sampled after
VM creation. ScaleX divides sprite.width34 and scaleY sprite.height30. Script
execution then tests the live traveled==0 gate for the secondary VM.

Draw writes VM.position334, at950/CFC in this object, plus224/16; it does not
write offset340. It rotates the primary by normalized angle+pi/2, sets flag4
and calls the current renderer. The post-draw tail decision, geometry and
renderer are live. Equality accepts signed zero and rejects NaN.

## Compiler observations

The native diagnostic uses the pinned VC7.1 SP1 /GL /GS surfaces and real
maintained callers/callees. Every context freezes recursive source/header
hashes. Diagnostic anchor images are non-runnable and grant no exact credit.
The frozen contexts and final accepted replay scope, if any, are recorded in
[native proof](native-proof.json).

Initial complete five-TU Initialize-entry context emits313/309,844/832 and
169/173 for Init/Update/Draw. An actual Draw-entry two-TU context emits173/173
but still differs; matching size is insufficient. A complete five-TU actual
Update-entry context avoids the unused callback's private stack-receiver ABI
and emits836/832. Init's two extra VM-pointer moves reflect wrapper404F30's
private EDX/ESI disagreement. Its existing pure reset401DE0 already has a
complete273-byte canonical exact proof and is not changed to fit this parent.

A shadow five-TU control reorders only the script binder's formal VM/index
arguments and all true selected declarations/callers. Before modification,
every copied input matches the initial context hash. The three parent
structural diagnostics remain unchanged; that control is not promoted. It
does not establish the original declaration order. No volatile, code placement,
forced attributes, copied target code or fabricated register interface is used.

The expiry-first grow branch restores the target block order. A memcpy control
still hoists the first Enemy-coordinate load before the source-pointer advance;
the ordinary typed vector assignment restores the complete word-copy shape.
This leaves exactly the same30-byte scale scheduling difference as type0.

A used sprite pointer read **before** X-scaling, then sampled again from the
owning VM before Y-scaling, restores the target read/store order. Both Update
owners then match all ordinary bytes. These are used values with target-observed
read timing, not forced memory/register annotations. Initial support-object
reuse requires unchanged recursive input hashes and grants no cold exact credit.

| Canonical owner | Complete owned bytes | Full cold builds | Frozen inputs | Bound fields |
| --- | ---: | ---: | ---: | ---: |
| Type1 Update | 832 | 2 | 24 | 34 |
| Type0 Update | 1001 | 2 | 22 | 31 |

Every cold rebuilds its entire five-/four-TU graph and uses the actual virtual
Update entry. Complete strict replay passes every byte and field, including the
state-table base and four internal pointers. The two PDB GUIDs for each owner
are distinct; frozen source/header inputs agree. The existing Motion0010269-byte
unit passes its focused same-primary-TU guard and all10 fields. No historical
cohort replay or product/runtime credit is claimed. Init313/309 and Draw's
size-equal173 context remain nonexact. See the tracked unit declarations and
[native proof](native-proof.json).

The public CI command reached its ledger gate before the new matches completed
their second cold, and correctly rejected the two pending unit rows. After
promotion, the unchanged canonical CI commands are resumed privately:81 completed
checks are retained from that successful prefix, and11 checks run afresh,
including current Python, both changed lifecycle checks, documentation, the
failed ledger gate and every unreached tail check. All92 canonical checks are
therefore covered across the two runs. No public CI gate or default is changed.
Superseded warm/argument controls, shadow source copies and completed host
carriers free7,851,211 bytes; current canonical carriers and the two
unresolved Init/Draw contexts retain their frozen inputs/evidence.

## Host scope

The actual three owner bodies execute with real timer methods, embedded-VM
wrapper, angle normalization, Player contact and sound queue. Canonical headers
have explicit native-assertion and constructor-prefill adaptations. All outer
objects have live typed storage, including the full Enemy/base hierarchy.
VM reset is pure observation. Bind/execute/create/draw/hit and unimplemented
virtuals are declared roles; callbacks are not injected into pure reset, polar,
normalizer or sound methods. GCC host diagnostics use -fno-strict-aliasing for
the source-facing cross-view pointer accesses; original C++ union/object
lifetimes remain outside this proof. The unknown native polar scratch Z is explicitly
prefilled17 in the host only.

The type1 suite passes34 strict named-CHECK mutants under ASan/UBSan/leak
checks, and the changed type0 suite passes all21 again. They cover full
request/script/mask/gate/resource/geometry
initialization, frame-scaled motion and attachment, phase boundaries/resets/
fallthrough/fractional interpolation, retirement, collision taper and gates,
box arguments, signed graze modulo, sprite axes/flags and live script/draw
state. Compiler errors, sanitizers and unrelated CHECKs do not count as mutant
kills. Runner (`e1c4075:scripts/test-laser-type1.py`, retired host tooling),
fixture (`e1c4075:scripts/fixtures/laser-type1.cpp`, retired host tooling).

These checks do not establish original C++ union/object lifetime, native x87
precision/exceptions, invalid indices, unbounded arithmetic, allocation/assets,
original constructor/vtable installation, product closure or runtime reachability.

The MIT [reference repository](https://github.com/levzzz5154/th10-decomp) remains
pinned at c6791d82dc9f02e55eca45a5314f900a85481efd. laser_initialize.cpp,
laser_grow_update.cpp and laser_grow_draw.cpp supply hypotheses. Its names put
width at runtime48/request2C and swap sprite axes; TH10's Init/Update consumers
instead prove width44/request28, growth speed48/request2C and width34/height30.
Its colour/script names are also provisional. Its code-segment pragmas,
forced inline/noinline and assembly boundary adapters are not adopted.
[MIT attribution](../../licenses/th10-decomp-MIT.txt) is retained.

```sh
scripts/repo-python scripts/report-laser-type1.py
scripts/repo-python scripts/test-laser-type1.py --sanitize
scripts/repo-python scripts/test-laser-line.py --sanitize
scripts/repo-python scripts/build-match-unit.py --unit laser-type1-update
scripts/repo-python scripts/compare-linked-function.py --unit laser-type1-update --json
scripts/repo-python scripts/build-match-unit.py --unit laser-line-update
scripts/repo-python scripts/compare-linked-function.py --unit laser-line-update --json
```
