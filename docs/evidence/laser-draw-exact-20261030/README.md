# Complete type0 laser drawing exact — 2026-10-30

`EnemyLaserLineView::Draw41D7C0..41D86C` now matches **all173 owned bytes**, all10
independently bound fields and its fixed local branch. Two independent canonical
two-input cold builds pass. This adds173 authored exact bytes/one owner, reaching
100,640/275,817 bytes across493 owners. Boundaries, origin and source presence
are unchanged. Original class/type/helper ancestry and production TU remain open.

## Complete target and interfaces

The verified Japanese v1.00a target decodes into44 instructions and a plain RET
at+AC. Seven CC bytes precede and three follow. The only absolute entry reference
in the image is the reviewed pointer-table slot46DA6C (offset0C in46DA60).
Whole `.text` E8/E9 scanning finds no entry or interior destination. The observed
entry receives the line in ECX, preserves it in ESI and returns integer zero.
The table role does not prove original C++ class ownership or runtime construction.

Primary and optional secondary VM positions use VM+334, from current line
position+(224,16,0). The primary rotation is normalized angle+pi/2 and flag4 is
set before drawing. The first actual renderer can change line state or globals:
traveled, position and renderer are read again before the optional secondary
call. The one JP at+6E targets+A8 and retains unordered exclusion from equality.
No branch, Z copy, current-state reload or cleanup byte is omitted.

All10 fields are independently decoded from target operands: seven DIR32
occurrences bind224 at470B4C,16 at470B48, zero at470B04 and the renderer global
491C10; three REL32 fields bind AddNormalizeAngle44BC10 and Draw4451C0 twice.
Both actual callees are complete exact in the same image:94/all6 and216/all21,
including the renderer dispatch table and its fixed destinations. Their real
interfaces remain two float stack arguments/RET8 with ST0 result for angle
normalization, and ECX manager/EAX VM/plainRET with integer result for rendering.

## Natural source and first complete comparisons

The first complete current-source comparison reproduces172/173 in the existing
real Draw-entry two-input context. An explicit primary VM pointer recovers the
cached VM used through rotation/flags/rendering and emits173 bytes, but ordering
still differs. A normal screen-position helper then recovers the target read/write
sequence; passing the VM itself leaves the wrong addressing bases. Taking the
actual Float3 position field as the output preserves the target parent-relative
stores and the VM pointer lifetime. The complete function then passes every byte
and field. Both inputs/XYZ outputs are used; neither helper has a separate mapping.

No register annotation, forced inlining, volatile, inert padding, dummy argument,
manufactured return or changed compiler flag is introduced. The helper is fully
inlined by the pinned compiler. The existing471-byte laser-contact function also
passes its complete8-field same-image check; its source algorithm is unchanged.
The four scoped same-image bodies pass954 bytes/all45 fields. Exactness does not
establish original source spelling or inline boundaries.

The new recipe retains the already reviewed Draw-entry two-input context:
LaserLine primary, AnmManager support, pinned /GL /GS and unchanged link options.
The first cold explicitly selects only the new Draw and its two actual existing
callee units,483 bytes/all37 fields. The second independently rebuilds the new
173-byte owner. No historical cohort or unrelated shared-source consumer runs.

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
timeout 600 taskset -c 0 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit laser-line-draw --unit anm-render-draw --unit anm-add-normalize-angle --json
timeout 600 taskset -c 0 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit laser-line-draw --json
scripts/repo-python scripts/ci.py
git diff --check
```

Compiler/linker jobs run serially on CPU0 at nice10 with bounded runtime and
retained exits/logs. The [receipt](results.json) records independent PE/PDB
identities, complete target inventory, every field binding and source freezes.
Private binaries, databases, generated decompiler text and toolchains stay ignored.
No runtime, semantic dual-Oracle, product-build or portability closure is claimed.

## Complete9964-byte ANM executor follow-up

Before this Draw work, the preceding471-byte rotation-helper result justified
seven bounded inline-lifetime models on the complete ANM executor. The old
five-input context's AnmManager.hpp pin had become stale after a later header
change. A fresh current-source five-input baseline reuses five individually
verified newer Player-cold objects, relinks and strictly compares all350 bound
fields. It reproduces9960/9964 with exactly the old four ordinary residual bytes.
This is a warm relink, not a new cold or exact claim.

Argument-reading helpers (including ordinary inline and output-parameter forms)
and a record Create method retain unwanted calls or change the complete extent.
The trivial value factory is complete-byte neutral. Two radial helpers change
additional operand orders and regress the full bound comparison. All seven trials
are restored; the original CPP/header hashes agree. These specific models are
parked and no parent exact credit is granted. Current five-input baseline:
`.analysis/gpt-6.1-sol/anm-executor-inline-context-20261030/entry-current-context.json`.
The machine receipt retains their complete-owner rejection results and pin repair.
