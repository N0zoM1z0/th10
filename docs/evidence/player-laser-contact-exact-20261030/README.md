# Complete Player laser contact exact — 2026-10-30

`Player::CheckLaserContact4267F0..4269C6` now matches **all471 owned bytes**, all8
independently bound address fields and14 fixed local branches. Both independent
canonical cold compilations pass. This adds **471 authored exact bytes /1 owner**;
source presence, boundaries and origin classifications are unchanged. Authored
exact becomes100,467/275,817 bytes across492 owners. Original names, types,
helper/inline ancestry and production TU remain unknown.

## Target and actual interface

The canonical Japanese v1.00a target is independently hash-verified. Complete
raw decoding yields132 instructions, three RET12 exits at+178/+1CC/+1D4,
four preceding CC bytes and nine following CC bytes. Whole `.text` E8/E9 scanning
finds only two entry references and no interior destinations:41D6BE in the1001-byte
type0 update and41E935 in the832-byte type1 update. Decoding each caller from its reviewed entry through the call establishes ECX Player, EAX origin and three stack floats, with EAX integer result
and callee RET12. Both canonical caller units are explicitly cold-replayed.

The body rotates the full relative position, forms lower/upper XYZ bounds,
performs hit and wider graze interval checks, gates hits on GUI message,
states2/4/3 and positive highlight timer, then invokes the real stack/RET4 hit
boundary426CF0. The wider Y tests both use lower.y. Negated rejection predicates
retain the observed unordered behavior. This is not a runtime/NaN simulation.

Independent target field review binds7 DIR32 fields:0.5 at470B0C, zero at470B04
(twice),-0.5 at470B78, GUI global47770C,1.5 at470CA8 and-1.5 at470CA4. The sole
REL32 field binds PlayerHandleCollisionHit426CF0. Numeric constant payloads are
checked separately. No moved local destination, callee interface or unused Z
calculation is omitted from the complete comparison.

## Natural source change

The first complete comparison in the actual nineteen-input Player/laser graph
reproduces the old453-byte candidate. Its captured extent copy creates a0x30
frame. Referencing the live existing extent restores the target0x24 frame.
Assigning the complete upper vector through its ordinary value operation restores
all three component calculations/copies and the target storage reuse. Copy
initialization had allowed the compiler to discard unused upper Z work.

An ordinary static rotation helper consumes only its real output, input and
angle. It writes X/Y while the caller retains relative Z. Negative angle is
formed after promotion to double; sine/cosine are float values. The existing
compiler completely inlines this helper and recovers the target x87 lifetime
and consuming operations. No inline-forcing, register/ABI annotation, volatile,
padding, copied bytes or compiler/profile change is introduced. There is no
new separate helper mapping or claim about the original inline ancestry.

The exact result first appears in the real nineteen-input graph, then passes
all8 strict bindings in both that graph and the existing four-input laser-update
graph. The nineteen-input image contains the real624-byte hit implementation;
its complete native receipt and the1616-byte movement receipt remain unchanged.
The hit owner itself remains NONEXACT624/627 and receives no parent credit.

Shared upper storage gives the wrong local homes. Const/float trig changes,
consumed products and separate scalar Y variants fail to recover the inline
lifetime. Their complete candidates span443..481 bytes and remain rejected;
all trial source is restored to the single retained helper form. The original
four-input context independently retains479 before the helper; caller-count or
entry/profile changes are not the explanation. The preceding9964-byte ANM
executor review yields no new source lead; its parked controls are not rerun.

## Cold acceptance and scope

The new unit copies the existing `laser-line-update` recipe: primary LaserLine,
then AnmManager, EnemyLaser and EnemyEclDispatcher, pinned VC7.1 /GL /GS and the
real Update entry. The first fresh cold explicitly selects the new471-byte owner
and its1001-byte type0 caller, passing1472 bytes/all39 fields. The second freshly
recompiles that graph for471/all8 and separately selects its832-byte type1 caller, passing1303 bytes/all42 fields.
The three distinct owners cover2304 bytes/all73 fields.
The complete machine receipt records both exit statuses, independent PDB/image
identities, field bindings, local destinations and source/recipe freeze hashes.
No historical cohort, all-header-consumer replay or host failure campaign runs.

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
timeout 600 taskset -c 0 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit player-check-laser-contact --unit laser-line-update --json
timeout 600 taskset -c 0 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit player-check-laser-contact --unit laser-type1-update --json
scripts/repo-python scripts/ci.py
git diff --check
```

Run compiler/linker jobs serially. Scratch, compiler logs and private PE/MAP/PDB
carriers remain under the matching `.analysis/gpt-6.1-sol/` and
`build/gpt-6.1-sol/` task directories. The repository tracks only source, the
canonical manifest, ledger entry and compact receipts. Product build/runtime,
semantic dual-Oracle scenarios and portability remain later phases.
