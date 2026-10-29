# Complete Stage late draw — 2026-10-29

Late402CA0..403040 adds **929 authored exact bytes**. Two independent canonical
ten-input StageCreate cold links reproduce the complete PDB contribution and
all78 semantic fields. All15 local branch destinations stay fixed. The owner
has243 instructions and retains its stack StageScript pointer/RET4 interface.
Its source mapping, reviewed boundary/origin and authored denominator are
unchanged. [Results](results.json) retain field-contract and source/include
hashes, artifact identities, distinct PDB GUIDs and focused guard results.

The full body includes flag8 early return, camera copy/publication, fog and
render-state setup, layer17/18 and instance-layer8..11 drawing, mix-color state,
transition countdown/flags and final state restoration. Actual branches are
included in the complete comparison; no host lifecycle/failure fixtures run.

The starting933-byte source differed at one camera-pointer store and four fog
cache update sites. Two natural operations recover the complete owner:

- Main's cachedFogEnabled+750 and device+8 are expressed through the maintained
  Main view in an ordinary SetFogEnabled member. This restores each cache store
  before vtable retrieval and the device push. Direct assignment-as-argument,
  local-device and generic COM-wrapper controls were byte-neutral and restored.
- The camera's InstallViewport member publishes this, invokes ConfigureCamera,
  installs the active viewport and clears its configuration flag. This lets the
  compiler reuse EDI for publication instead of emitting a second pointer
  literal, removing the last four-byte extent gap. A pointer-parameter helper
  and consumed memcpy return did not repair that store and are restored.

There is no forced ABI/register, volatile spill, padding, byte patch, noinline or
compiler-profile change. The new helpers are source operations; separate target
owners and original class/TU identities are not inferred. The camera's existing
118-byte layout is unchanged.

All78 fields have complete semantic bindings. Twelve bind the real Main base
plus8 or750; two bind camera+E8/+EC. Four Main+8 operands happen to coincide with
an unrelated PbgArchives diagnostic-anchor label. The actual Main symbol and
member offset determine their binding. A nearby/coincident map label supplies
no ownership evidence. Local branches are never normalized to different labels.

Both colds select only `stage-draw-late`, `stage-configure-camera`,
`main-select-viewport` and `main-disable-fog`:1372 bytes. Eight old same-image
contracts pass745/all24, including camera330, viewport60, fog53, adapters7 each,
actual Flush133, DrawLayer67 and timer Add88. The last five use complete old
contracts by explicit unit name in the new image; their original recipes remain
separate. Both camera unsigned-correction occurrences and both timer threshold
constants are independently raw-equal. No historical cohort or unrelated Main
graph is rebuilt solely for unused new header methods.

The four instance-render calls retain their actual two-stack-argument/RET8 ABI;
the592-byte target callee remains nonexact and receives no credit here. Early
is1088/1099 and default camera282/287, both nonexact. Instance drawing retains a
separate full-word fog alias, so storage/global alias closure remains open.

Early has a concrete next lead: target402A61 calls the maintained82-byte factory
43C8B0 with priority in EBX, five stack arguments and RET20. The current unresolved
call pushes all six arguments because that real definition is absent from this
ten-input graph. Connect actual callee visibility before changing the declaration;
rectangle/register differences also remain. The [factory](../effect-constructor/README.md)
already has a separate exact recipe and adds no new credit.

The initial cold attempt failed compilation after an extern still used by
instance drawing was removed while deleting a stale byte-width comment. Its
C2065/C3861 logs are retained and excluded. The declaration was restored without
the stale comment; both accepted colds compile the corrected final source afresh.

```sh
scripts/repo-python scripts/replay-exact-units.py --unit stage-draw-late --unit stage-configure-camera --unit main-select-viewport --unit main-disable-fog --json
scripts/repo-python scripts/ci.py
git diff --check
```

The comparator is unchanged. Exact reconstruction does not establish data/global
alias ownership, single-image closure, product runtime or portability.
