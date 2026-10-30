# Complete untextured rectangle exact — 2026-10-30

`DrawColoredRectangle` at43BDA0..43BF82 is now complete exact:483 bytes,
142 instructions, all14 address fields,11 D3D virtual calls, no local branch,
and plain RET. This adds483 authored exact bytes/one owner. Its existing
boundary, authored origin and source mapping are unchanged.

## Target evidence and source model

The target reserves5C stack bytes: four genuine14-byte vertices plus one
reused0C-byte XYZ temporary. Coordinates are read after the real Flush call
and moved as raw words. Each zero Z is stored into the real temporary and
copied into its vertex; W and color are then stored in vertex3/2/1/0 order.
The prior direct-field source emitted407 bytes/frame50, eliminating the
temporary and using the other store order.

The first complete proposal constructs existing `AnmFloat3View` values and
assigns each through a view of the canonical vertex's XYZ fields. Reversing
the W/color assignment chains restores their observed order. This naturally
recovers all483 bytes under the existing /GL profiles. The temporary is used
by all four complete XYZ assignments; no padding, copied target bytes, forced
inlining, fake ABI or new compiler flags are used. No shared header changes.
Original vector ancestry, constructor/helper boundaries, spelling and TU remain
unknown; this does not establish a D3DX declaration or production source tree.

The actual rectangle EDI/color EBX/plain RET interface is recovered from its
real Player drawing caller. The callee Flush receives the manager in ESI.
Thirteen DIR32 fields bind two manager and eleven device reloads; the remaining
REL32 binds Flush442F50. All eleven D3D virtual calls, state constants, six
manager-cache invalidations and final stack cleanup are inside the complete
comparison. Input remains mutable and is read after flushing.

The physical owner has three preceding and thirteen following CC bytes.
Whole-text E8/E9 scanning finds12 encoded entry calls in seven enclosing
reviewed owners, including five GUI and two Player sites, with no encoded
interior transfer. This is a direct-reference review, not dynamic pointer
discovery. Target names and the former decompiler boundary are not acceptance
authority; the full byte extent and fields are independently checked.

## Reproduction and acceptance

The new canonical unit shares the existing three-input Player drawing recipe:
Player.cpp, AnmManager.cpp and AnmRectangle.cpp, with the real registered draw
entry as root. Plain /GL and the existing ANM /GL /GS profile stay unchanged.

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
timeout 600 taskset -c 0 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit anm-draw-colored-rectangle --unit player-draw-callback-body --unit player-draw-callback --unit anm-render-flush-vertex-buffer --json
timeout 600 taskset -c 0 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit anm-draw-colored-rectangle --json
```

Two independent zero-reuse cold builds pass483/all14. The first explicitly
replays the actual409-byte drawing caller,7-byte adapter and133-byte Flush:
1032 bytes/all42 fields. The second selects only the new483-byte owner.
The same four strict checks pass in one image. Jobs are serial, CPU0, nice10,
bounded and source/manifest-frozen, with retained exit logs.

No source/header input of the existing seventeen-input Player graph changes;
its58 source/header pins still verify. Main3108, hit624, movement1616, state2,
initializer and the separate724-byte GUI drawing owner remain nonexact.
No unrelated or historical cohort replay is required. Old GUI packet commands
and host reports remain historical evidence, not checkpoint requirements.

`results.json` records complete target, bindings, source pins, cold results
and the real before/after source model. Private native artifacts stay under
`build/` and `.analysis/`. Codegen exactness does not establish GPU/runtime,
original type/data/TU ownership or portability.

Authored exact is101532/275817 bytes across495 owners; total codegen169636/1372.
Source mappings remain1207/authored689, with194 mapped authored owners pending
exactness. The broader reconstruction goal remains open.
