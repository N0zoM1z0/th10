# Enemy death finalizer and real service interfaces

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Parent checkpoint: `461ede1`. Reviewed on 2026-10-21.

The existing authored `EnemyFinalizeDeath40E5F0..40E691` becomes exact:
**162 complete bytes/all9 fields**. Boundaries, origin, source mappings and
authored denominator stay unchanged. Existing ExtendFaithTimer93/all2 retains
its exactness. The three original large owners and later phase gates stay open.

## Direct target evidence

Fresh local Ghidra runs readOnly/noanalysis with rollback, one CPU and768 MiB.
Disk identity and full inclusive mapped text401000..465A80 agree before/after:
412289 bytes, SHA-256
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
The finalizer has one direct target caller, RuntimeUpdate40E249. Its RET4 ends
40E691; the following fourteenCC bytes remain outside the162-byte owner.

Sound43DD10 receives the real492590 sound owner and sound ID in private ESI/EBX
plus stacked X. World creation448DB0 receives the selected ANM resource,
script index, actual position and hidden AnmVmId result home. Item Spawn41BB00
uses the existing ItemManager receiver; scatter40C9D0 receives the live position
and drop block. The finalizer clears the leading drop after scatter and calls
the474C40 score state with amount10 before returning one. The score-state base
is already independently established by GAMESTATE-001 and its timer/faith/score
helpers; this field does not establish production data ownership.

The old free `EnemyPlayDeathSound` and `EnemySpawnDeathEffect` declarations omit
real receivers and the ANM return protocol. The maintained caller now invokes
existing QueueSoundCue and CreateVmAtWorldVariant0 methods. No wrapper or ABI
override is introduced. The hidden return naturally reuses the no-longer-live
enemy parameter home: native LEA ECX,[ESP+18] at offset39 matches the target.
No fake argument, target bytes, padding or manufactured return is added.

All nine observed ExtendFaithTimer references are direct calls: one death path
and eight item-update sites. Ordinary noinline on its definition preserves this
shared helper in the declared graph. This is a compatible compiler policy,
not recovered original annotation/TU/profile evidence. Real interfaces alone
emit260 bytes; outlining the timer gives165. That candidate redundantly computes
the position and keeps the original object base live. Separate position/drop
block locals, introduced after effect creation, recover target ESI position and
EBP drop-block lifetimes, removing the last three bytes. Full byte comparison,
rather than equal size, establishes acceptance.

The MIT th10-decomp commit
`c6791d82dc9f02e55eca45a5314f900a85481efd` corroborates those local lifetimes.
Its assembly/ABI scaffolding is not used. Adjacent TH09 cleanup has different
fields/protocols and supplies no TH10 layout or behavior premise. See the
[reference license](../../licenses/th10-decomp-MIT.txt).

## Complete verification

Two independent full ten-TU /GL /GS colds through the real dispatcher freeze
the same31 recursive raw inputs. Each strictly replays new162/all9 and29 prior
owners3494/all95, including ExtendFaithTimer, sound, the drop pair, polar, four
RNG helpers and interpolation guards: **30 complete owners3656/all104 per image**.
All9 fields have independently reviewed semantic identities. The owner is dense
code with no skipped bytes, data-span annotation or compare-size extension.
The [native summary](native-summary.json) records source/artifact/field identities.

Canonical cold2 selects only new FinalizeDeath162 and modified ExtendFaithTimer93
in its pre-existing two-TU /GL (without /GS) recipe: two units255 bytes/two groups.
No full historical replay is performed. An initial global build-group check
rejects a conflicting output directory before compiling; the new unit is then
assigned the existing same-graph EnemyItemDrops directory. Source, profiles,
entry, support order and target field bindings are unchanged by that correction.

Three controls reject: a wrong sound-owner target; copied-native hidden return
home18->14 (161/162); and the complete pre-lifetime165-byte extent. Only the
copied diagnostic image is mutated; the original executable remains read-only.
The full165-byte PDB contribution cannot be cropped to claim162 bytes.

The [maintained host projection](../../../scripts/test-enemy-finalize-death.py)
passes432 scenarios under address/undefined/leak sanitizers and rejects seven
strict intended-assertion mutants. Mutable sound/effect/item/scatter sinks check
receiver identity, field reloads, actual position/block aliases, unconditional
scatter, leading-drop clear and timer ordering. Ordinary logical host containers,
live ANM/item/score objects, aliased float-vector host types and placement-created
sound objects avoid unconstructed-object fixtures. These are host sequence/storage
checks, not target layout, i386 ABI/rounding or original/native-game runtime proof.

The same actual graph retains complete native ANM9964, Run7020 and dispatcher14468
bodies after verified field rewrites. Full Run7020/all189/62locals retains27
ordinary differences. All three remain nonexact. World creator116/118, item
Spawn795/819, RuntimeUpdate2356/2412 and ItemManagerUpdate2232/2320 remain separate
nonexact diagnostics and receive no caller/callee credit from this162-byte claim.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-finalize-death --unit game-score-extend-faith-timer --json
scripts/repo-python scripts/test-enemy-finalize-death.py --mutations
```

Final gates, actual CI scope and attributable cleanup are in the
[validation receipt](validation-summary.json). Diagnostic anchor images remain
non-product artifacts.
