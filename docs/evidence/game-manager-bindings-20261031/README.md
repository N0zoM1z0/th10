# GameManager complete-owner call binding repair — 2026-10-31

The complete GameManager update remains **1535/1544, NONEXACT**. This checkpoint
repairs its actual calls and adds the complete **GameClearLinkedObjects66/all1
exact** dependency. New exact credit is66 authored bytes/one owner. The existing
7-byte callback is restored in the actual dependency graph, with no new credit
or promotion of its indeterminate origin.

## Target-backed source and actual dependencies

Independent full target decoding identifies114 address fields,60 local branches,
36 direct calls and four RET4 exits in418190..418797. The final native owner has
114 fields,59 local branches and the same36 actual callee identities in the same
order. All decoded local destinations are independently checked as instruction
starts; their differing owner-relative offsets are not normalized away.

Six facade interfaces now name the actual target functions already maintained:

| Former interface | Actual target |
| --- | --- |
| GameResetAnimationOwner | BulletManagerView::ResetPool405ED0 |
| GameClearEnemyVms | EnemyManagerClear409F90 |
| GamePlayStageMusic | FrontEndPlayMusic420B10 |
| GameDestroyMenu | StageScriptView destructor402440 |
| GameEmitDemoEffect | StageCreateTransitionEffect43C8B0 |
| GameQueueSound | MainSupervisorView::SetNextGameMode40AC90 |

The last call changes the Main mode; it is not a sound call. It uses the actual
MainSupervisor global at491C28. The demo effect's meaningful operands are
priority43/kind5/60 frames/three zeros. Stage cleanup keeps its real destructor
and ordinary scalar delete. The existing terminal cleanup also called the old
enemy facade: direct target edge417FAC confirms the same EnemyManagerClear
binding there. Other cleanup behavior remains unchanged and nonexact1108/1105.

BulletLifecycle, ScriptViewerWorker, StageLifecycle and ReplayManager join the
previous23-input graph, preserving existing declared profiles. The fifth added
TU is the new GameObjectReset body. No synthetic caller, fake operand, forced
register or global compiler-flag change is introduced. Local boundaries preserve
actual pool-reset, demo-factory and Main mode calls that the target keeps out of
line. Existing canonical graphs are retained except for the affected callback
recipe described below.

## Exact intrusive-list cleanup

417770..4177B1 receives its laser-owner view in EAX and ends in plain RET. It
reads sentinel.next at+18, captures next before virtual slot4, then rereads
previous/next after that call to unlink the node. The predecessor is unchecked;
the successor write is conditional. Ordinary operator delete releases the node,
then traversal uses the captured next. No extra head/count publication or null
repair is introduced. Existing laser storage/base views and local offset
assertions supply previous+4, next+8 and head+18. Original class/TU/name and
physical storage ownership remain unknown.

Freshly attested read-only Ghidra and independent target decoding agree on the
full66 bytes, three local branches, virtual CALL [vtable+10], scalar delete and
RET. The sole resolved field at+32 names operator delete4524A1. Both GameManager
call sites bind to the actual source body. Every ordinary byte, branch,
alignment byte and the independently bound field passes strict comparison.

## Complete-parent controls and unresolved boundaries

| Current model | Complete parent bytes |
| --- | ---: |
| Entry23-input source/context |1544/1544, nonexact|
| Real calls and28 actual inputs |1602/1544, nonexact|
| Preserve the two real ResetPool calls |1528/1544, nonexact|
| Three separate final flag checks |1533/1544, nonexact|
| Explicit sound-mode branch |1535/1544, nonexact|

The first model inlined both45-byte pool resets; a scoped real call boundary
restores the two target calls. Three separate post-GUI flag checks recover the
27-byte three-bit branch/return span raw-equal. Explicit QueueCommand branches
recover the31-byte sound-selection grammar: four independently bound fields and
15 ordinary bytes agree. These are mismatch diagnoses, never partial-owner
credits. Proper typed stage delete is byte-neutral and restored to the explicit
actual destructor/deallocator model. The failed first compile exposed the last
old enemy facade in terminal cleanup; its target edge was verified and repaired.

The final parent still differs in receiver/zero-register ownership, statistics
address formation, cached pointers, redundant null gate and fixed local offsets.
Its actual BeginStage body remains1000/582 with an unlike incoming interface;
Stage destructor is460/499 in this graph and fails its complete canonical
contract. Stage reset184/214, embedded initializer200/202 and binder197/203 also
remain unlike. EnemyBeginSpell remains2424/2432. Actual definitions are not proof
that their caller's private interface or whole bytes match.

## Callback regression and scoped acceptance

The first affected canonical replay rejects the old single-TU callback recipe:
OnUpdate becomes10 bytes, saving EBX, moving ECX to EBX, calling the private
plain-RET1552-byte core and restoring EBX. Target requires a7-byte adapter that
pushes ECX and calls a stack/RET4 core. The failed image/report are retained.

The unchanged callback is7/all1 exact in the actual28-input graph, whose core
retains the target stack/RET4 surface. Only game-manager-update-callback migrates
to that graph/producer; its PDB owner is explicitly GameManager.cpp. No field,
size, ABI requirement or comparator rule is relaxed. The old stage-delay recipe
continues independently, even though its unselected callback is not exact.

Two independent zero-reuse28-input canonical colds pass new cleanup66/all1 and
restored callback7/all1. The first additionally passes the affected existing
transition119 and stage-delay92 through their own recipes. Both frozen28-input
images pass23 selected existing contracts3054/all112, including the actual
worker, pool, enemy clear, music, Main mode and old transition/score/ANM seams.
The rejected Stage destructor460/499 remains reported separately. Header Tick72
is explicitly qualified to the primary Enemy object, as verified by the PDB,
rather than borrowing its historical StageScript.cpp owner. An initial scratch
summary assumed rejected records had a size; only that reporting code was fixed.

[results.json](results.json) retains independent target inventories, current pins,
call bindings, controls, cold identities, rejected guards and migration evidence.
Native jobs are serial, CPU0/nice10 and bounded below
`.analysis/gpt-6.1-sol/game-manager-bindings-20261031/`; frozen builds stay under
the corresponding `build/gpt-6.1-sol/` campaign. Canonical producer:
`build/match-linked/GameObjectReset`.

```sh
scripts/repo-python scripts/replay-exact-units.py --unit game-clear-linked-objects --unit game-manager-update-callback --unit game-enter-menu-state --unit game-stage-transition-delay --json
scripts/repo-python scripts/ci.py
git diff --check
```

Authored exact becomes104652/275863 across509 owners; canonical totals172756/1386.
Mappings1214/authored696 and backlog187 keep source presence separate from
exactness. No reviewed boundary/denominator or product/runtime/portability claim.
