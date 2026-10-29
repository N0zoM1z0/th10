# Complete Stage projected-bounds culling — 2026-10-29

StageObjectView::BoundsCulled403090..40384F adds **1984 authored exact bytes**.
Two independent canonical eleven-input StageCreate cold links reproduce its
complete518-instruction PDB owner, all30 semantic fields and69 fixed local
branch destinations. The owner keeps frame128, meaningful objectECX,
positionEDX, cameraEAX, stackdistanceSquared and both RET4 exits.
[Results](results.json) retain source/include and field-contract hashes, distinct
PDB GUIDs, artifact identities, all local destinations and focused guard results.

The full body rounds the translated object center and camera-relative delta,
checks squared distance, constructs eight half-extent corners, initializes the
world matrix, applies instance translation, projects all eight corners, scans
inclusive0<=Z<=1 with the original else-if extrema, and checks inclusive overlap
X32..416/Y16..464. Both early and final returns belong to the comparison.

Fresh complete analysis starts at1929/1984, rather than assuming old1855/1925
reports describe the current source. Three natural source changes recover the
full owner:

- Corner components are calculated directly while filling the real array.
  Computing a separate minimum vector had evaluated all minimum XYZ first;
  the target computes minimum Z, Y, then X when constructing the respective
  corners. Direct component expressions recover the target x87 value reuse.
- Half extent uses an ordinary vector-result assignment. This restores the
  target rounded result copies before corner construction. The initial direct
  initialization omitted those copies; source-size proximity was not accepted.
- The actual corner workspace holds translated center/delta in its first entry
  during the distance test, then all eight entries are repopulated before the
  projection call. This recovers target destination8C for both distance and the
  first corner, half10, projected output1C, half-result temporary7C, matrixEC and
  frame128. A reference names the real first entry; it adds no fake input or
  storage. Moving the matrix declaration earlier alone was byte-neutral and
  restored. The source retains the existing XYZ dot-expression accumulation,
  real matrix setup, eight-corner scan and actual callee interface.

No copied target bytes, padding, volatile, fake ABI/registers, unused inputs, forced
spill, noinline or compiler-profile changes are used. View/method spelling and
original production class/TU identity remain provisional; exactness establishes
the maintained complete body, rather than a unique original source spelling.

All30 fields are strict:28 float-constant occurrences and two D3DX thunk calls.
Every scalar occurrence is independently raw-equal in both images. Target
452182/452188 are verified JMP-IAT thunks for d3dx9_31.dll's actual
D3DXMatrixTranslation and D3DXVec3ProjectArray imports, matching the meaningful
four-word/nine-word stdcall declarations and actual12-byte strides/eight count.
External D3DX implementation/runtime equivalence is a separate phase.

Each cold explicitly selects only `stage-object-bounds-culled` and its real
`stage-draw-instances` caller, sharing the existing eleven-input graph and pinned
source profiles. Fourteen old same-image contracts pass3720/all220 twice:
Instances592, Early1099, Late929, camera330, selector60, fog53, adapters7 each,
actual Flush133, DrawLayer67, timers88/57, factory82 and Draw216 including its
complete table. Header-inline SetCurrent uses its physically verified
StageScript.cpp PDB owner here; its full old byte/field contract is unchanged.
No full cohort or unrelated original recipe is replayed for sharing a graph.

```sh
scripts/repo-python scripts/replay-exact-units.py --unit stage-object-bounds-culled --unit stage-draw-instances --json
scripts/repo-python scripts/ci.py
git diff --check
```

Only this1984-byte owner adds exact credit. Source mapping, reviewed
boundary/origin and authored denominator are unchanged. Default camera remains
282/287 nonexact. Physical global alias/data ownership, single-image/product
closure, runtime validation and portability remain open. The comparator is
unchanged; no host failure/lifecycle simulation is required for this byte proof.
