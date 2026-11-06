# Type-1 circle cutter — real owner and target sample semantics

Original Japanese TH10 v1.00a, executable SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The reviewed complete owner is `0x0041EFA0..0x0041F3F3`, **1108 bytes**.
This checkpoint adds **zero exact bytes**. The installed current-source
22-input diagnostic emits **1178 bytes /30 fields**, against original
1108/29; the strict complete-owner comparator rejects the extent.

## Rechecked original facts and source recovery

Dense decoding consumes all1108 original bytes, including the final RET C.
The original vtable at `0x0046DA10+0x1C` contains this entry. The body now
implements `EnemyLaserType1View::CheckCollisionCircle` in that actual known-slot
runtime view. Its declaration replaces UnknownSlot1C without adding or moving
a virtual slot or changing object layout. The disconnected provisional
`EnemyLaserType1CollisionView`, including its duplicate offset assertions,
is removed. Existing boundary and origin reviews remain unchanged.

The original prologue aligns ESP and allocates0x32C bytes. Ordinary vector
copies and operations now recover that frame in the native candidate.
The original explicitly clears sample Z **after** step plus position; the old
source omitted this clear. The step's pre-polar Z clear and subsequent loop
Z arithmetic remain present. Radius is squared in its existing parameter
home after vector setup, matching the original stack argument role.

Original `+10A..+116` rejects only an ordered squared distance greater than
the squared radius; unordered values pass. The maintained expression is
`!(dx*dx + dy*dy > radius)`, replacing the old positive `<=` predicate.
Capture bounds use the actual91-byte IsOutsidePlayfield helper's semantics,
including its negated ordered rejects. These are target code observations,
not a claim that all floating-point or runtime paths have been exercised.

The ANM effect path spells out the actual allocation, flag/layer writes,
X/Z/Y coordinate stores, script initialization and primary registration
visible in the original. It calls real maintained implementations. The gap
request remains exactly0x77 zeroed dwords, speed8, with observed type/color
and terminal distance read through the existing request at444/464/466.

The MIT source
[`laser_circle_split.cpp` at d1f3121](https://github.com/levzzz5154/th10-decomp/blob/d1f31212514781c928fba145d8207397c8fb5650/matching2/msvc/src/game/laser/laser_circle_split.cpp)
provided counter/vector/sample and effect-path leads. Copyright(c)2026 levzzz;
the [MIT notice](../../licenses/th10-decomp-MIT.txt) and source attribution are
retained. No upstream section control, stand-in callee, forced inlining,
explicit return-output slot or synthetic caller is used.

## Complete comparisons and real caller context

| Complete candidate | Graph | Native bytes / fields | Result |
| --- | --- | ---: | --- |
| Previous disconnected view | Fresh four actual inputs | 1244 /33 | Baseline source epoch, nonexact |
| Installed real owner and vectors | Same four-input graph | 1258 /30 | Correct0x32C frame; VM registration and factory inline |
| Installed source, actual broader callers | 22 actual inputs | 1178 /30 | Registration is out of line; factory still expands |
| Request-member factory control | Same22-input graph | 1178 /30 | Factory157/RET8 versus original153/RET4; rejected and restored |
| Original complete owner | Original EXE | 1108 /29 | Required acceptance extent |

The four-input graph uses EnemyLaser, AnmManager, ItemManager and the actual
EnemyEclDispatcher with the existing `/GL /GS` profile. The broader graph
extends the existing real InitializeVm-root replay/ANM carrier with EnemyLaser
under its established profile. It retains all original per-source overrides.
The owner TU is freshly compiled; 21 hash-checked IL objects are reused from
the independently cold-built canonical SaveReplay carrier. Their source/header
pins agree, apart from EnemyLaserType1.hpp, which none of those21 inputs
includes. Every reuse is recorded. This is a fresh nonincremental context link,
**not** a second22-input cold. Graph-dependent sizes are not source-only gains.

All29 target fields were independently decoded before candidate binding:
22 DIR32 and seven REL32. Existing reviewed symbol roles, independently checked
6/12 float payloads and the reviewed factory role form the proposed comparison
contract. These bindings do not waive the rejected extent or missing factory
call. The target calls the factory at+42A; the candidate instead emits operator
new and the type-0 constructor. Original direct calls to AllocateVm, script
initialization and AddVmVariant0 are otherwise represented by actual source.
AllocateVm still emits153 versus original161 bytes in this context.

The request-member control moved the full genuine factory body, retaining a
free-function adapter for existing callers. It did not restore RET4 or prevent
the expansion, so the original source factory is restored byte for byte.
No ABI/inline attribute is added to retain this failed model. Further factory
work needs a distinguishing real-call/production-context lead.

## Focused protection and evidence

The installed22-input image strictly preserves the polar helper30, bounds91
and actual primary-registration118: **239 existing exact bytes /four fields**.
The153-byte allocator is recorded as rejected, with no callee credit.
The shared virtual declaration's affected Update caller independently cold
passes **832 bytes /34 fields** with the selected command:

```sh
scripts/repo-python scripts/replay-exact-units.py --unit laser-type1-update --json
```

Draw is169 bytes in the Update-root image, so that image is rejected as a
guard for its independent173-byte canonical contract. Draw separately passes
173/all10 in its actual `--unit laser-type1-draw` cold recipe. The two selected
colds preserve1005 existing exact bytes/all44 fields. No historical
cohort or all-consumer replay is required. The circle candidate
remains absent from matches.csv and the canonical match-unit manifest.

[Compact evidence](native-result.json) retains original fields, complete
candidate inventories, source/image pins, rejected context controls and
focused protection results. Full source snapshots, dumps, objects and logs stay
privately under `.analysis/gpt-web/20261106-laser-type1-circle/` and
`build/gpt-web/20261106-laser-type1-circle/`. No comparison mechanism changed,
and no host failure campaign, runtime, product closure or portability is claimed.
