# Replay stage helpers and complete BeginStage — 2026-10-31

Two complete authored owners add **176 exact bytes**: frame-list cleanup93/all1
and position restoration83/all2. The complete BeginStage remains **566/582,
NONEXACT**; frame allocation139/139 also remains nonexact. Neither receives
fragment or equal-length credit. Existing canonical recipes remain unchanged.

## Independent target review

Read-only local Ghidra is freshly target-attested. Independent full decoding
checks every instruction, resolved field, direct local destination and return.
Ghidra's provisional stack-only arity does not describe the private interfaces:

| Complete owner | Meaningful target inputs / return | Bound fields | Local branches |
| --- | --- | ---: | ---: |
| ReplayClearStageFrameData42AB20,93 bytes | ECX manager, EAX stage / RET | 1 | 5 |
| ReplayRestoreStagePosition428E10,83 bytes | EAX Player, ECX position pair / RET | 2 | 0 |
| ReplayAllocateFrameData42AA50,139 bytes | ESI stage, stack manager / RET4 | 1 | 5 |
| ReplayManager::BeginStage42A450,582 bytes | EBX manager / RET | 25 | 10 |

Allocation callers are428F89/4298DE/42A49B; cleanup callers are428F7D/4294D6/
42A48F. Position restoration is called at42A5C8. The582-byte parent includes the
skipped three-byte LEA alignment at42A60D; reachable count579 cannot trim it.
The five actual parent calls name cleanup, allocation, position restoration,
RebuildPlayerOptions426F70 and PlayerResetRuntimeState424D90, in equal order in
target and final native source. Every local target is an instruction start.

## Natural complete source

ReplayStageHelpers.cpp uses existing ReplayManager, frame/list and Player views.
The cleanup captures node.next before payload release, skips null payloads,
conditionally writes both neighbors, clears the embedded link and invokes scalar
operator delete4524A1 before advancing through the captured next. No added head
publication or defensive repair changes the target. Its REL32 is at+4D.

Position restoration uses the existing PlayerPositionPair assignment to copy
both signed3CC/3D0 coordinates before x87 conversion. It writes draw3C0/3C4 and
sets all four option reset flags at332C/33C4/345C/34F4. Both float fields,+1E and
+30, independently bind the real0.01f constant470B00. Scalar assignment and
memcpy instead schedule FILD before the second coordinate copy. No ABI or
register attribute is added to either exact helper.

Frame allocation uses a real6284-byte constructor, including complete memset,
record/fps cursors and embedded link initialization. Nullable construction and
the following unchecked link address are retained. It traverses to the stage
list tail and preserves the conditional successor insertion. Both stage and
manager are meaningful; ordinary two-parameter stdcall lets LTCG derive the
observed ESI/one-stack-word/RET4 interface. Full139 bytes still reject because
LEA of the fps pointer precedes the recordEnd store. Reversing the two meaningful
initializers remains unequal and is restored. No constructor or partial-owner
credit is claimed.

## Complete-parent investigation

| Source model | Complete native / target |
| --- | ---: |
| Previous actual28-input context | 1000 / 582, nonexact |
| Actual helpers, source index rereads, post-loop flag resets | 568 / 582, nonexact |
| Genuine frame constructor and callee-pop allocation | 565 / 582, nonexact |
| memcpy pair model | 886 / 582, nonexact |
| Existing pair assignment operators | 565 / 582, nonexact |
| Existing GameScoreState aggregate view | 572 / 582, nonexact |
| Final stage-index snapshot | 566 / 582, nonexact |

Pair assignment recovers target recording and playback induction-base offsets.
Reusing the existing score-state view for real474C40 storage recovers alternating
recording loads/stores and character/shot reads inside the playback loop. Scalar
external aliases incorrectly let the optimizer hoist the selection expression.
The source keeps Player snapshots/rereads around actual position/rebuild calls,
uses the actual runtime-reset definition and writes four reset flags after the
pair loop. Current native code nevertheless preserves EAX through position
restoration instead of reloading Player. Loop registers, skipped alignment,
final index schedule and fixed local destinations remain unlike. Native has
24 fields/nine locals versus target25/ten. No fake clobber, volatile qualifier,
dummy argument, copied bytes, manufactured return or padding is introduced.

A natural Player member version of position restoration leaves the complete
parent566 and the helper83 unchanged, and is restored. The memcpy, constructor
store-order and member controls are parked pending new distinguishing evidence.
GameManager becomes1538/1544 and its cleanup remains1108/1105 in the actual29-input
graph. EnemyBeginSpell2424/2432, stage reset184/214, embedded200/202 and binder197/
203 remain nonexact. These measurements are complete-owner diagnostic results.

## Scoped cold acceptance

Two independent zero-reuse29-input canonical colds reproduce both new owners,
all176 bytes/all3 fields, complete local destinations and private interfaces.
All76 transitive source/header pins are retained in [results.json](results.json), with both
PE/PDB identities, independent target inventories and rejected controls.
The29-input graph adds the actual helper TU to the previous28-input graph and
keeps all pinned profiles and the real InitializeVm entry. The comparator is
unchanged. Canonical producer: `build/match-linked/ReplayStageHelpers`.

Both frozen29-input images pass24 selected existing complete contracts3120/all113.
The complete Stage destructor explicitly rejects460/499; it is never trimmed.
The first scoped canonical run additionally cold-passes existing cleanup66 and
callback7 through their unchanged28-input recipes. No old recipe is migrated and
no historical cohort is rebuilt. Native jobs are serial, CPU0/nice10, bounded
and logged under `.analysis/gpt-6.1-sol/replay-stage-20261031/`; frozen images
remain below the corresponding `build/gpt-6.1-sol/` directory.

```sh
scripts/repo-python scripts/replay-exact-units.py --unit replay-clear-stage-frame-data --unit replay-restore-stage-position --unit game-clear-linked-objects --unit game-manager-update-callback --json
scripts/repo-python scripts/ci.py
git diff --check
```

Authored exact becomes104828/275863 across511 owners; total canonical172932/1388.
Three new source mappings yield1217/authored699 and backlog188. Existing reviewed
boundaries and origins are retained; no denominator changes. Original names,
class/TU/data ownership, product closure, runtime and portability remain open.
