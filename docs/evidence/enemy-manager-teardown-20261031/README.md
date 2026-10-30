# Enemy teardown and cleanup dependencies — 2026-10-31

Two complete authored owners add **421 exact bytes**: EnemyManagerView teardown
at `40D530..40D65B`, 300 bytes/all25 fields, and GUI reuse at `414570..4145E8`,
121 bytes/all7 fields. Existing value50 increment20 retains its exact credit
after moving beside its actual GameManager caller. Boundaries, origins and the
authored denominator are unchanged.

The full 1105-byte GameManager cleanup remains **1098/1105, nonexact**. Equal
length statistics save564 and message teardown97 also remain nonexact. None
receives partial-owner credit.

## Correct calls and allocation release

The complete Enemy target first clears live state and removes both callback
nodes. The source now uses the real Chain::UnregisterElem with the observed
chain cached before entering the lock, replacing a register-shaped facade.
The 32 script-buffer releases reread scriptDatabase on each iteration; capturing
that owner once did not reproduce the target. The real ECL-resource destructor
remains an out-of-line call, followed by ordinary scalar delete. Its existing
complete36/all2 contract is retained, with no new credit.

The old animation teardown called an unresolved facade and used free for each
loaded owner. Target decoding instead identifies the actual ANM slot-release
operation for slots9–12: Release, scalar delete, then slot clear. Calling the
existing ReleaseAnm implementation recovers those operations and the physically
present signed/unsigned slot checks. No added null repair, index clamp or failure
simulation is involved.

The minimum shared startup-state view moves unchanged from GameManager.cpp to
GameScoreState.hpp. Its score/counter/flags offsets0/5C/60 remain asserted, with
no original allocation/class ownership claim. Using its real flags field removes
the final hoisted flag-byte read and recovers all300 bytes. Both plain returns,
ten fixed internal destinations and the private EAX receiver agree completely.

Player teardown had the same ANM allocation mismatch. It now calls real loaded-VM
marking and ReleaseAnm(8), and unregisters through the real Chain interface.
Its complete333/325 body remains nonexact, including a final EH-state write;
it gets no exact credit. A byte-neutral VM-view delegation experiment was
reverted. This checkpoint does not assert runtime or original class identity.

## Real lock imports and source profiles

AnmManager.hpp declared EnterCriticalSection and LeaveCriticalSection as ordinary
externals. The verified target IAT identifies KERNEL32 imports at4660B0/4660B4.
The corrected declarations use dllimport. A controlled before/after pair has
identical 34-input graphs and source pins differing only in this header. Strict
comparisons change from rejected to exact for both Chain insertions131, laser
factory91 and Stage destructor499. This is a declaration repair, with no source
algorithm or comparator change. Their previous rejections are not waived.

The new context retains the pinned VC7.1 profiles and /GL. GUI uses its existing
canonical /GL /GS /EHs profile; the earlier diagnostic graph omitted those GUI
overrides. Its complete136-byte factory consequently passes too. Player /EHs
retains the target-observed member unwind and stack interface, while its body
remains unequal. The /EHsc probe did not improve it and is not retained. No
existing canonical profile is changed.

FileSystem::Encrypt has an independently decoded RET20 and meaningful AL seed
plus five stack operands. Its ordinary six-parameter source declaration is now
stdcall, recovering the real callee-pop protocol when its definition is present.
The body286/307 remains nonexact. Actual GameStatistics and FileSystem sources
join the context; the save routine becomes564/564 but its checksum scheduling
still differs. Indexed/pointer/helper controls do not establish equality and
are parked; the original checksum source is retained.

## GUI reuse and retained leaf

The complete121-byte helper destroys/deletes the nullable message. When flags9
are clear, it releases/deletes actual ANM slot28, clears stageUiAnm, frees the
nullable message-file allocation and preserves both observed null stores.
Other modes retain those resources. All seven semantic fields, four fixed local
destinations and the EBX/plain-RET interface match. The message destructor itself
is still nonexact despite equal97-byte extent.

Only the existing20-byte418A90 body moves out of the legacy aggregate. It uses
the existing score view, preserves unsigned increment followed by the signed
cap-at9 test and returns the observed owner. The complete body remains raw-equal.
Its stable unit identity migrates to the actual caller graph; other legacy
bodies are unchanged and are not replayed as a cohort.

## Scoped acceptance

Two independent zero-reuse 34-input canonical cold builds reproduce all441 bytes
and32 fields:421 new plus20 retained. The first scoped run additionally replays
the unchanged ECL-resource36, Chain insertion262 and startup1009 recipes. The
new producer is `build/match-linked/GameCleanupContext`; only the two new units
and migrated20-byte unit use it.

Each produced image passes 46 selected existing contracts,6261 bytes/all275
fields. The protected startup additionally passes1009/all102 with its complete
qualified static28-byte callee. These checks run at the actual producer path;
the frozen copies retain identical PE/MAP/PDB bytes and the original producer
qualification. Independent frozen-image decoding rechecks the complete new
owners, every fixed destination, return, field and the static body. No static
path selector is weakened or spoofed.

[results.json](results.json) retains84 active source/header pins, contracts,
complete inventories, controlled import evidence, profiles, full-owner control
results, producer proofs and independent cold identities. Private logs/builds
are under the `cleanup-context-20261031` campaign. Native jobs are serial,
CPU0/nice10 and bounded. No comparison infrastructure, host failure/lifecycle
tests or full historical replay is added.

```sh
scripts/repo-python scripts/replay-exact-units.py --unit enemy-manager-destructor --unit gui-prepare-reuse --unit game-decomphelp-fulltu-00418a90 --unit enemy-ecl-resource-destructor --unit chain-add-calc --unit chain-add-draw --unit game-manager-initialize --json
scripts/repo-python scripts/replay-exact-units.py --unit enemy-manager-destructor --unit gui-prepare-reuse --unit game-decomphelp-fulltu-00418a90 --json
scripts/repo-python scripts/ci.py
git diff --check
```

Authored exact becomes107521/275863 across519 owners; canonical175635/1397.
Mappings1221/authored703 and backlog184 keep source presence separate. Original
names/TUs/data ownership, product closure, runtime and portability remain open.
