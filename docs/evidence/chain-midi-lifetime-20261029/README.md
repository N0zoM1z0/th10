# Chain and MIDI destruction through WinMain — 2026-10-29

Four complete owners add **378 exact bytes:366 authored and12 indeterminate**.
Two independent cold comparisons reproduce every byte and all19 fields. These
are four new source mappings; the reviewed authored denominator is unchanged.
[Results](results.json) retain source/include and Oracle hashes, both profile
contexts, PE/MAP/PDB identities, handler binding and explicit replay scopes.

| Complete owner | Bytes | Fields | Selector |
| --- | ---: | ---: | --- |
| Chain destructor438880 | 103 | 8 | `chain-destructor` |
| ChainElem destructor449A90 | 12 | 0 | `chain-element-destructor` |
| MIDI output destructor43A910 | 156 | 9 | `midi-output-destructor` |
| MIDI track release43AA60 | 107 | 2 | `midi-release-tracks` |

WinMain now allocates/deletes the actual48-byte Chain and uses the existing
`th10::g_Chain` pointer at491BE4. The old untyped creation/destruction bridge and
separate g_MainCallbackOwner alias are removed. This closes that source alias,
without claiming all other views or production storage. The real constructor
remains59/59 with ordinary store-order differences. Its physical call is retained;
the original outlining annotation and source TU remain unknown.

The Chain destructor stops Main's worker, removes both sentinel lists and then
automatically destroys draw/calc sentinels in reverse construction order. Each
member clears all three callback slots+8/+C/+10; the old two-field navigation
note is incorrect. The12-byte member remains origin-indeterminate. Normal and
exception cleanup belong to the complete103-byte comparison.

The new opt-in Chain EH binding checks the named handler at parent+9, its unique
unaliased generated MAP entry, complete29-byte PDB region and shared parent
module/object. Both images require immutable28-byte two-state FuncInfo, exact
unwind predecessors/actions, saved this atEBP+4 and sentinel offsets0/24h. Both
actions tail-jump to the actual qualified12-byte member destructor, whose entire
zero-field code is raw-equal to449A90. The dispatch must name its FuncInfo and
CxxFrameHandler. This supplies handler address identity only; the generated29
bytes, metadata and EH runtime receive no separate credit. The new synthetic
Oracle test covers parser routing, positive/neighbor-tail cases and33 targeted
rejections. No game exception/failure scenarios are executed.

MIDI destruction stops playback, releases each track's data and track array,
frees all32 file buffers, closes the output and performs automatic timer-base
destruction. Reloads after external frees and both repeated buffer clears are
retained. The base cleanup keeps both period-end calls, following any call made
by StopPlayback. Main's actual delete expression now binds this156-byte owner.

The old global timer-destructor noinline annotation was too broad: it kept
Main shutdown293 exact but prevented MIDI's automatic base cleanup from inlining.
It is removed. With the same natural source, two bounded recipes are verified:

| Context | MIDI output destruction | Main game shutdown |
| --- | --- | --- |
| Default Midi.cpp `/O2 /GL /GS` | 156 exact | 321/293 nonexact |
| Midi.cpp with `/Ob1` | 116/156 nonexact | 293 exact |

StopTimer is an ordinary inline member. The updated Main recipe is cold-replayed
twice and adds no new byte credit. Both contexts retain the existing pinned
compiler/linker and other per-source profiles; this does not identify original
production flags/TU partition or prove a single whole-program image.

Each default image also compares21 selected existing contracts9290/all373,
covering Chain insertion/unregistration, worker and Main lifetime, MIDI timer,
FrontEnd/Ending/Startup/browser lifetime and configuration UI. RemoveAllFromList,
UnprepareHeader and FrontEnd CreateVm are separately replayed with their existing
canonical recipes. Only selected related owners are tested, not the historical
cohort. The default graph's uncredited child bodies remain separate from their
other accepted recipes.

During the second cold, Winepath crashed and polluted one object path before
linking. Toolchain execution smoke passed; the completed second Main context was
retained/rechecked and only the failed default context was freshly retried. The
original failure and recovered report remain in the private evidence.

Complete Render is419/425 and WinMain is1390/1389; both remain nonexact.
WinMain still has local/register lifetime differences. StopPlayback is129/129 with13 ordinary scheduling bytes
different. The allocation-registry lifecycle role changes code but produces no
exact parent and is restored; flag spelling in the complete59-byte Chain
constructor is byte-neutral and restored. A caller-only /Ob1 partition does not
fix timer inlining and is removed. These controls are parked; size proximity
grants no partial credit.

```sh
scripts/repo-python scripts/replay-exact-units.py --unit chain-destructor --unit chain-element-destructor --unit midi-output-destructor --unit midi-release-tracks --unit main-release-game --json
scripts/repo-python scripts/ci.py --only 'Validate chain-member EH address binding'
scripts/repo-python scripts/ci.py
git diff --check
```

Original identifiers/classes, data ownership, product/runtime and portability
remain separate open claims.
