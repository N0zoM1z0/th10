# Complete Player damage owner — 2026-10-30

**Player::CalculateDamageToTarget is exact across all1170 owned bytes.** Two
independent canonical cold links reproduce its complete PDB contribution, all28
resolved address fields and all41 fixed local branch destinations. New authored
credit is1170 bytes/one owner. The [results](results.json) retain target review,
complete field bindings, source/object pins, cold reports and scoped guards.

The target entry428630 ends428AC1, followed by14 CC bytes. Fresh decoding consumes
355 instructions/1170 bytes, including the two-byte MOV EDI,EDI before the second
loop. Both exits are RET12. The sole direct caller40E13C pushes target size,
target position and Player, then clears ECX. The optimized member interface is
ECX optional hit flag, three stack values, EAX integer result and RET12. Source
keeps an ordinary Player member; no invented register declaration is required.

## Source and closure

One ordinary Float3 scratch value now serves the shot upper bound and later
rotated target coordinates. These lifetimes do not overlap. This restores the
shared ESP20/24 home, while target minimum/maximum and shot minimum retain their
separate slots. Reusing the lower bound instead does not reproduce the frame.

The rotated Y expression is written as an initial sine product followed by a
cosine-product addition. It reproduces the target x87 operand schedule without
an extra intermediate store. The hit-transition condition rereads descriptor
kind after damage accumulation. In the maintained /GL context, the read-only
interval predicate permits the existing BL value to be reused, while the damage
addition becomes the target direct memory ADD. Merely changing an integer
condition, a switch or commutative addition does not have that effect.

The complete owner covers the changed-frame gate,128 shot tests, callback path,
VM transition, type2 effect allocation, auxiliary damage,32 effect collision
rows, rotated/radial geometry, row hit caps and capped score update. No branch is
excluded from comparison. Runtime execution and failure-path tests are not used
to substitute for full byte coverage.

All eight direct call sites bind their actual target entries: interrupt40C480,
interval427C70, VM lookup449450 twice, release449630, creation448D00,
allocation427B50 and auxiliary damage4059F0. Actual definitions remain present
in the17-input graph. VM lookup/release use ESI and plain RET; creation uses EDI
return storage and three stack values/RET12; allocation uses EDX Player and five
stack values/RET20; interval uses ECX timer and one stack value/RET4. The auxiliary
callee reads ECX owner/EAX position and returns without stack arguments in both
images. Its189/202 native/target body remains nonexact; this owner does not grant
it callee exactness. The collision callback remains an indirect call, with the
same actual argument setup in the complete comparison.

The28 exhaustive fields comprise20 absolute references and8 relative calls.
A fresh target operand scan independently enumerates them; source/map identities
bind every field rather than accepting arbitrary address masks. All41 local
branches retain exact owner-relative destinations, including both loop backs.
The complete native extent is PDB-bound to Player.cpp's support object.

## Bounded controls

| Source control | Native bytes | Result |
| --- | ---: | --- |
| Earlier complete source | 1172 | Stack homes, Y arithmetic and ADD differ |
| Reuse shot lower bound | 1172 | Does not restore all homes; rejected |
| Reuse shot upper bound | 1172 | All vector homes restored; retained |
| Two-step rotated Y | 1174 | x87 instructions restored; ADD/alignment remain |
| Bool eligibility variable | 1186 | Restores memory ADD but adds SETNE/TEST pairs |
| Integer conditional / switch | 1174 | Same residual ADD expansion; rejected |
| Reread descriptor kind | 1170 | Complete target-bound exact owner; retained |

Reordered bounds output, array bounds, wider cosine, returned rotation value,
separate shot branches and reference-add helper fail or are neutral and restored.
No copied bytes, padding, volatile barrier, fake ABI or new compiler option is
introduced. Negative controls are parked until new evidence distinguishes them.
Original source identifiers, class design and production TU partition remain
unknown.

The separately investigated shot updater remains876/876 nonexact: its motion
LEA is still early and changes a local skip destination. Reference/switch/type
unification controls do not close it. Motion remains160/171 nonexact; ordinary
shared-vector addition can alter Cartesian operand order, but does not restore
the missing polar Z store and floor-call cleanup. All shot/motion experiments
are restored; the committed source change is confined to this damage owner.

## Reproduction and checkpoint

The canonical unit records the unchanged pinned17-input /GL graph and real
InitializeVm entry. The harness image is diagnostic; it does not establish the
original production partition or runnable product closure.

    timeout 600 taskset -c 0 nice -n 10 scripts/repo-python \
      scripts/replay-exact-units.py \
      --unit player-calculate-damage-to-target --json

First cold explicitly selects the new owner plus planar bounds55, effect
allocation181, interrupt65 and interval29: five units/1500 bytes/all35 fields.
The second cold independently rebuilds only the new1170-byte/all28-field unit.
The first successful cold report was subsequently overwritten by a same-label
structural diagnostic. Its comparisons are reissued from the retained first-cold
artifacts, with object/image hashes and the original exit/freeze receipt; no
recompilation is used for that report repair. No historical cohort is replayed.
Both canonical rebuilds retain17 freshly
compiled objects for the new unit; compilation/linking is serial, CPU0/nice10,
bounded and logged.

Thirty same-image strict bodies, including the new owner and its actual exact
callees, pass4825 bytes/all155 fields. Ten preceding native candidates retain
complete bytes/field identities, including main3096, state2 786, shots876,
EnemyRuntimeUpdate2412 and motion160. This preservation grants them no target
credit. Source pins and object hashes guard reuse during exploratory warm links;
canonical cold replay does not reuse these objects.

Target verification and toolchain declarations pass. No Ghidra observation,
compiler change or comparator change is needed. One independent cold attempt
fails when Wine page-fault text contaminates a linker path. The required
verify-toolchain.py --execute smoke then passes normal COFF, /GL, resource and
PE32 linking; the identical canonical unit is cold-retried without source/profile
changes. The failed receipt and recovery check are retained in results.json. Default seven-stage CI and
whitespace checks pass. Totals become99,794/275,752 authored owned bytes exact,
490 authored exact owners; boundary/origin/mapping denominators are unchanged.
Product closure, runtime and portability remain later independent claims.
