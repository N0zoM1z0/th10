# Circle cancellation, float3 storage and unordered comparison

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Parent checkpoint: `81f8d25`. Reviewed on 2026-10-21.

The existing authored owner408100..408203 is now mapped to maintained
EnemyCancelManagerView::CancelBulletPattern and canonical exact:
**260 complete bytes/all5 fields**. Its reviewed boundary, authored disposition
and denominator remain unchanged. One previously missing source mapping is
added. BulletManager is a maintained subsystem assignment; the original class,
physical TU, profile and complete data ownership remain unknown.

## Target and natural source

Fresh direct local Ghidra review observes five direct calls at4258D4,4258F0,
41181E,411866 and405B13. The dense260-byte body endsRET12, followed by twelveCC
before408210. EAX carries the manager and EBX the query point; radius/mode/flags
are stacked. The loop visits2000 records from manager+60, stride7F0. It skips
states0/3 and, when requested, nonzero cancelBehavior at row+4.

The target computes all three position differences, stores/copies them through
six float slots in a frame18, and uses only X/Y for squared distance. Radius
is collisionWidth*0.5f + radius. FCOMPP/FNSTSW followed by TEST AH,5 and JNP
skips an ordered outside point; unordered comparisons follow the collision
path. The old source's <= comparison incorrectly excluded NaNs. The accepted
negated > comparison preserves that target path.

A hit calls408030, then bounds4086B0 with margins2, then tests mode. If inside
and mode is nonzero, it calls the real ItemManager::Spawn with kind8, color-1,
angle-1.5707964f and speed0.6f. The cancel return value is ignored; bounds and
item position use the actual record after cancellation. These operations do
not prove the nonexact Spawn callee or a complete runtime path.

Four private source controls execute in the real ten-TU graph. Correcting only
the unordered comparison remains215 bytes. An existing float3 subtraction
operator plus literal0.5 also remains215: VC7.1 removes the unused Z/copies.
An ordinary derived float3 value copied into the base retains the three
components and frame18, but a raw byte loop produces279 bytes/RET16. Traversing
the actual BulletRuntimeView instead recovers the target register allocation
and complete260-byte body/RET12. The accepted code includes BulletManager.hpp
and removes the old partial BulletManagerView declaration. The ordinary value
constructor has no barrier, asm, padding or forced register spelling. Its
separately emitted native support code receives no target-owner/exact credit.

The MIT th10-decomp reference commit
`c6791d82dc9f02e55eca45a5314f900a85481efd`, file
`matching2/msvc/src/game/bullet/player_collision.cpp`, suggested the derived/base
value shape. Its barrier is not used. TH08's radius-removal vector shape is
hypothesis material only; its pool/protocol differ. Acceptance comes from TH10
bytes and historical compiler output. See the
[reference license](../../licenses/th10-decomp-MIT.txt).

The five complete fields are independently bound: half0.5 at470B0C (raw
3F000000, also established by other canonical units), cancel408030,
bounds4086B0, ItemManager slot477818 and Spawn41BB00. Each field is checked;
no wildcard address masking or ABI override is used.

Ghidra runs readOnly/noanalysis with rollback, CPU1 and768 MiB. Disk identity
and complete mapped text401000..465A80 agree before/after:412289 bytes,
SHA-256 `cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
Generated decompilation remains private.

## Validation and current large owners

Two independent full ten-TU /GL /GS colds through the real dispatcher freeze
32 equal recursive raw inputs. Only the existing AnmInt3Scale support uses
/Ob1. The real BulletManager header is the one added input; the other31 are
unchanged except the primary source. Both images strictly pass the new260/all5
and34 complete prior helpers4164/all113: **35 owners4424/all118 per image**.
Complete PDB ownership and dense decode qualify the whole260-byte extent.
The [native summary](native-summary.json) pins the actual graph, profiles,
source inputs, all artifacts, field bindings and complete guard owners.

Canonical cold2 selects the new260-byte unit in one shared build group.
A final affected-source replay compiles the six existing normal-COFF helpers
(197 bytes) once under their unchanged original profile; all six pass. Total
selected canonical replay is7 units457 bytes/two groups; no full historical
replay is performed. Cold2 occupies the existing
`build/match-linked/EnemyItemDrops`. This proof supersedes the previous
cancel-record proof's source/mutable-cache pins. Independent cold1 is
`build/gpt-6.1-sol/enemy-circle-cancel-20261021/maintained-cold1`.
Both new images have identical raw complete bodies for circle cancellation,
ANM ExecuteScript, ECL Run and the dispatcher. All32 raw source pins and30
retained native artifact pins are checked again after the final gates.

Four strict negative controls reject: wrong half-scale address, copied-native
status mask5->1, copied-native pool count2000->1999, and complete279/260 extent.
The first three compare259/260; the last fails complete PDB ownership without
an ordinary-byte score. Only diagnostic image copies are mutated; the original
target stays unchanged. Superseded215-byte controls and a duplicate private
exact image are pruned; their source/producer/strict/guard receipts remain.

The maintained host projection (`e1c4075:scripts/test-enemy-circle-cancel.py`, retired host tooling)
passes6480 scenarios and ten strict intended-assertion mutants under address,
undefined and leak sanitizers. It preserves the complete member/value source,
actual logical pointer identities and2000-row traversal. Finite boundary
witnesses, ignored Z, NaNs/infinities, states, filter/mode/bounds combinations,
mutable post-cancel position/global receiver and service ordering are checked.
Only four offset/pointer casts are adapted to live host objects. This supplies
logical storage/sequence evidence, without native layout, i386/x87 precision
or original/native-game runtime claims. Compilation errors, sanitizer faults
and unrelated failures cannot count as mutant rejection.

ANM9964 and Run7020 retain complete native equality with the preceding image
after independently verified field rewrites. Run/all189/62locals remains
6993/7020, **27 ordinary differences**, with its171-byte callee exact.
The dispatcher changes14468->14464 against target14416. A fresh case-aligned
diagnostic retains frame2C4,19 sentinel comparisons and all108 group/181selector
positions. Target-sized groups improve40->42/108; absolute case-size gap
652->650 and comparable matched bytes2924->2980/11152. These are noncanonical
partial diagnostics, not parent exact credit. The original three large owners
remain active/nonexact; native-product/runtime/semantic/port gates stay open.

The operator-requested SVG fix shows the ratio of canonical exact authored
bytes to currently classified authored bytes even while attribution/inventory
remain unresolved. The actual rendered bar is134.91/512 pixels, labeled
**26.35% · provisional**, for72088/273576 bytes and393/735 functions. The
complete-game denominator remains unknown; no ledger denominator is expanded.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-cancel-bullet-pattern --json
scripts/repo-python scripts/test-enemy-circle-cancel.py --mutations
```

All104 standard CI checks pass in one invocation. Final focused260/all5,
expected Run7020/27, affected COFF197, toolchain/tracking/progress/docs/whitespace
gates pass. All32 raw inputs,30 linked artifacts and the additional COFF object
remain hash-current. Six inspected owned terminal/unexecuted directories are
pruned/6719057 bytes. The [validation receipt](validation-summary.json) records
final gates and cleanup. Recovery is
`.analysis/gpt-6.1-sol/enemy-circle-cancel-20261021/RECOVERY.md`.
