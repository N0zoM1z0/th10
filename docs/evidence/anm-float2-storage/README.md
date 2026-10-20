# Complete ANM executor: Float2 construction and UV storage

> Handoff update, 2026-10-20: Float2/Float3, alpha and RGB evaluators have since closed. Current ExecuteScript is9948/frameFC in the RGB five-TU graph, still nonexact. Measurements below describe this historical storage checkpoint. The operator ended the 95% campaign; see [current handoff](../../RE_HANDOFF.md) and [RGB checkpoint](../anm-rgb-interpolation/README.md).

`AnmRenderManagerView::ExecuteScript` at 0x43EE30 remains **nonexact** across its
complete 9,964-byte owner. A separate raw UV storage view permits a normal Float2
constructor, correcting the earlier C2620 compile error without changing vertex
layout. Explicit value construction in SCALE_TIME restores the target 0xFC stack
frame and 9,964-byte extent. There is **zero new exact credit**; authored exact
remains 64,776/273,274 bytes (23.70%).

## Evidence and retained source

Local read-only Ghidra and the hash-attested target agree on the full executor,
its 94-entry/92-destination table, Float2 evaluator 0x441AD0/652 and the actual
caller at 0x4410C0. The entire mapped `.text`, 0x401000..0x465A80 inclusive, is 412,289
bytes with SHA256 `cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
Ghidra runs noanalysis, one CPU and 768MiB; temporary metadata rolls back and
mapped bytes are checked before/after. No target bytes or durable database changes are made.

Vertex UV has two 32-bit float fields at 0x14/0x18 within the 0x1C-byte record. The old source
used the mathematical Float2 inside an anonymous union with `u/v` aliases;
VC7.1 rejects a constructor-bearing member there. The maintained UV storage
view now has the same x/y fields, size and aliases, with explicit offset
assertions. It has no constructors or math operations. This separates two
maintained roles; original type identity, class names and production TU are
unknown. Other Float2 consumers use ordinary members or locals, with no
aggregate initializer or remaining union use.

Math Float2 gains the same normal empty/default and two-scalar construction
idiom already used by Float3. Existing math operators and evaluator API stay
unchanged. The empty constructor leaves components uninitialized. UV separation
alone and the added constructors alone are code-neutral in the fresh graph.
SCALE_TIME then constructs `finalScale(GET_FLOAT_VAR(2), GET_FLOAT_VAR(3))`.
VC7.1 evaluates Y before X, keeps decoded values live and copies the completed
pair. The first 79 bytes of that target interval now agree after accounting for
real GetFloatVar call fields; duration decoding remains different. No normal
input gameplay defect in the prior source is claimed.

## Full-owner diagnostics

| Source model | Owner / target | Frame | Equal-sized physical groups | Aligned comparable matches | Sum of absolute length gaps |
| --- | --- | --- | --- | --- | --- |
| Fresh unchanged HEAD 2cf915c | 9948 /9964 | 0xF8 /0xFC | 84 /92 | 6492 /8000 | 56 |
| Raw UV separation only | 9948 /9964 | 0xF8 /0xFC | 84 /92 | 6492 /8000 | 56 |
| Above plus math constructors only | 9948 /9964 | 0xF8 /0xFC | 84 /92 | 6492 /8000 | 56 |
| Above plus SCALE_TIME value construction, retained | 9964 /9964 | 0xFC /0xFC | 89 /92 | 7184 /7995 | 14 |

All 92 groups retain target order; pre-table span becomes 9,588/9,588. Generic
whole-owner comparable matches remain 1846/8564. Case-aligned scores normalize
branch destinations separately and are diagnostic only, not exact fractions.
Equal-sized groups still contain stack-home and register/index differences;
89 equal sizes do not mean 89 exact blocks.
The remaining unequal intervals are FLIP_X (61) (+1), SCALE_TIME (60) (-7), and the
physical interval beginning at STATIC (2) (+6). The latter includes common update
code and internal alignment, not just one opcode. Target has 13 internal
alignment bytes at 0x4411C3..0x4411CF and one byte before the table. A length match
must not be substituted for complete instruction/field agreement.

SCALE_TIME still differs in the integer-duration phi/register allocation.
The common scale update still expands the flag OR into a register load/store,
where target uses a direct memory OR; copies also choose different registers.
The target Float2 evaluator frame is 0x20, current 0x28, size 653/652. Source ownership,
aggregate return spelling and optimizer context remain unresolved.

A final read-only comparison identifies a more concrete next lead. Target mode7
snapshots the initial pair before addition; the current native member operator
reads it directly. Target Hermite doubles `t` before adding1/subtracting from3,
and multiplies final tangent, initial tangent, final and initial in that physical
order. Current named weighted locals produce the opposite order and extra
copies. Natural nested vector expressions could explain this lifetime pattern;
original operator parameters and return spelling remain unknown. This combined
hypothesis is untested. The retained source is frozen throughout final CI.

## Rejected controls and reproduction

Named duration and explicit cached-pointer duration branches are code-neutral.
Interleaving duration between Y and X obtains 90 equal-sized groups and gap 2,
but changes the observed Y/X/duration call order, so it is rejected. Decoding
all scalar values before construction regresses the owner to 9980. Constructor-
return math operators shrink the Float2 evaluator to 575/652 and are rejected.
Whole-scale assignment and raw flag OR yield only two extra aligned matches;
they do not repair the actual update sequence and are restored. Capturing raw
FLIP_X flags regresses aligned matches. Reading the live instruction for duration
regresses other groups. A by-value Float2 evaluator leaves the main owner
unchanged and grows its helper to 685/652; in-place mode updates shrink that
helper to 615/652. Both are restored. These are scoped controls, not universal
claims that a spelling or interface is impossible.

The fresh four-TU graph uses `src/AnmManager.cpp`, followed by
`src/RandomMath.cpp`, `src/AnmVmCreate.cpp`, `src/AnmVmId.cpp`; entry
`?ExecuteScript@AnmRenderManagerView@@SGHPAUAnmVmView@@@Z`; profile
`/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL /GS`. It is the existing Initialize/
SetupVertexBuffer/radial/primary-alpha canonical context. Source-only diagnostic
controls reuse only SHA-checked support objects from that graph. Final evidence
uses complete independent colds, never those incremental controls.

[Native summary](native-summary.json) records source pins, complete-owner
results, controls and strict guards. The generic probe plus the graph above and
`scripts/report-anm-execute-cases.py` reproduce the full-owner diagnostics.
Existing focused canonical commands are:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit anm-start-scale-interpolation --unit anm-get-float-var --json
scripts/repo-python scripts/report-anm-execute-table.py --check
```

The diagnostic image uses unresolved data anchors and is never run. Whole
Windows i386 build/link/runtime closure and later semantic/port gates stay open.
No forced attributes, register declarations, padding, ABI tricks or target code
are introduced. Historical caches whose recursive source pins differ are not
current evidence. Only one current carrier is retained after pruning controls.

Two final complete cold links freeze all 11 recursive project inputs before/after
and independently pass all 9 existing guards: 2670 bytes/151 fields each. The
necessary one-TU canonical replay additionally passes StartScaleInterpolation
and GetFloatVar: 560 bytes/all 45 fields in one cold artifact; see
[canonical summary](focused-canonical-summary.json). No historical native
cohort is replayed. The transition host extracts unchanged range-helper/timer/
viewport blocks; only the two reviewed whole-input pins change, with strict
normalization and rejection unchanged. About 24.5 MB of superseded artifacts
are pruned, retaining verified source deltas and one current native carrier.
The pre-existing untracked D3D evidence remains untouched.

All95 standard CI steps pass with source/config frozen. Toolchain declarations,
tracking, regenerated progress, table coverage and whitespace pass. Later
documentation-only observations receive a focused doc/link and whitespace
check. See [validation summary](validation-summary.json). The former95% campaign is closed.
