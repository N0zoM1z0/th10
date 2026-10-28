# Bullet cancel transition and branch lifetimes

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Parent checkpoint: `b9d6cb5`. Reviewed on 2026-10-21.

The existing authored EnemyCancelBulletRecord408030..4080F6 becomes exact:
**199 complete bytes/all4 fields**. Boundaries, origin, source mapping and
authored denominator stay unchanged. The three original large owners remain
active/nonexact; this caller proves no additional callee or product exactness.

## Target and source evidence

Fresh direct Ghidra observes the complete199-byte body and seven direct entry
calls, including projectile commands, circle cancellation, message construction
and Player update. Raw target bytes end in RET at4080F6 followed by nineCC before
408100. Ghidra runs readOnly/noanalysis with rollback, CPU1 and768 MiB; disk
identity and complete mapped text401000..465A80 agree before/after:412289 bytes,
SHA-256 `cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.

Only states1/2 react. Bounds uses the actual position+3B4 and margins8. Both
paths set animation+30C to1 and state+446 to3. The screen path conditionally
creates a world VM for signed nonnegative script+438, then initializes/restarts
the timer at3F8..408. The outside path sets flags bit8 and leaves the timer
alone. The target carries the bullet in ESI and saves EBP for its zero value
only inside the screen path, after loading the script. The old source emits
200 bytes, shares zero in EBX with the outer comparison, and restores EBX even
on the outside path.

Natural branch-local state transitions recover the narrower zero lifetime:
each branch first assigns state3, then animation1. VC7.1 merges those stores
and emits target order animation1/state3 before the conditional branch. Merely
moving zero after the script load or replacing it with literal zeros preserves
the entire old200-byte body. An early return remains200 with different layout.
Branch-local animation-first source reaches199, but reverses the two emitted
stores and retains six ordinary differences. State-first source closes all199
bytes. These observations do not identify the original class, declaration or TU.

The MIT th10-decomp reference commit
`c6791d82dc9f02e55eca45a5314f900a85481efd`, file
`matching2/msvc/src/game/bullet/player_collision.cpp`, corroborates the two-branch
source shape. Acceptance comes from TH10 target and historical compiler bytes;
the adjacent TH09 transition is a different protocol. See the
[reference license](../../licenses/th10-decomp-MIT.txt).

All four fields are independently bound: bounds4086B0, resource pointer slot
4776F0, world creation448DB0 and game speed476F78. The resource+3E0B50 view is
already established; no complete data ownership is inferred. World creation
remains116/118 in this graph and receives no exact credit from its caller.

## Complete validation

Two independent full ten-TU /GL /GS colds through the real dispatcher freeze
31 equal recursive raw inputs. Only the existing scale support uses /Ob1.
Each image strictly passes new199/all4 plus33 complete prior helpers3965/all109:
**34 owners4164/all113 per image**. These include bounds91, Push96, Pop122,
death finalizer162, drop pair551, sound149 and Faith93. The new owner is dense
code with complete PDB ownership, no skipped bytes or comparison extension.
The [native summary](native-summary.json) pins actual profiles, sources,
artifacts, target bindings and observed controls.

Canonical replay selects only the new199-byte unit, one shared build group.
No full historical replay is performed. The second cold occupies the existing
`build/match-linked/EnemyItemDrops`; its new pins supersede older proofs for
that mutable cache. Independent cold1 is
`build/gpt-6.1-sol/enemy-cancel-record-lifetimes-20261021/maintained-cold1`.

Four controls reject: resource slot+4, a copied-native state3->2 byte, complete
same-size reversed-store193/199, and complete200/199 extent. Only the copied
diagnostic image is mutated; the original target remains unchanged.

The maintained host projection (`e1c4075:scripts/test-enemy-cancel-record.py`, retired host tooling)
passes288 scenarios under address/undefined/leak sanitizers and rejects eight
strict intended-assertion mutants. Logical live Bullet/Timer/resource/position
objects preserve actual pointer identities and source operations while mapping
target offsets to host fields. Mutable effect sinks verify timer-flag reloads,
initialization gating and resets after effect creation. This supplies storage/
sequence checks, without native layout, i386 ABI or game-runtime evidence.

Complete native ANM9964, Run7020 and dispatcher14468 are raw-equal to the prior
carrier and between both new colds. Full current Run7020/all189/62locals still
has27 ordinary differences and an exact171-byte float-value guard. All three
remain nonexact; their previous differences persist through full body equality.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-cancel-bullet-record --json
scripts/repo-python scripts/test-enemy-cancel-record.py --mutations
```

The [validation receipt](validation-summary.json) records final gates and cleanup.
Recovery is `.analysis/gpt-6.1-sol/enemy-cancel-record-lifetimes-20261021/RECOVERY.md`.
Diagnostic anchor images remain non-product artifacts; native-product/runtime/
semantic/port gates stay open.
