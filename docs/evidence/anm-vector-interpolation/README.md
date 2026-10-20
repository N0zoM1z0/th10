# Exact ANM vector interpolation evaluators

> Handoff update, 2026-10-20: RGB820 and alpha370 have since closed. Use the RGB pack for the current five-TU carrier and source pins; measurements below retain their original scope. The operator ended the 95% campaign; see [current handoff](../../RE_HANDOFF.md) and [RGB checkpoint](../anm-rgb-interpolation/README.md).

Two complete authored owners now reproduce the target in two independent
canonical cold builds: **1,539 new exact bytes**. The authored numerator becomes
66,315/273,274 bytes (**24.27%**); the denominator is unchanged.

| Maintained owner | Target address | Complete bytes | Bound fields | Stack frame |
| --- | --- | --- | --- | --- |
| Float3 interpolation Evaluate | 0x00404610 | 887 | 10 | 0x44 |
| Float2 scale interpolation Evaluate | 0x00441AD0 | 652 | 10 | 0x20 |

See [target observations and bindings](target-bindings.json),
[two-cold canonical proof](native-summary.json), and
[scoped source controls](controls.json).

## Source and target evidence

Modes 7 and 17 preserve the initial tuple before updating it. The source now
uses a normal `const` value snapshot for these additions. Both evaluators
calculate `twiceValue = value + value` once, before `minusOne`, and reuse it in
the two Hermite endpoint weights. This is ordinary common-factor reuse, not a
register or stack-placement instruction. A mathematically similar multiplication
or regrouping did not produce the same C1/C2 lifetime graph.

Float2 arithmetic now returns normally constructed values, as Float3 already
does. Hermite and the default curve use whole-vector expressions, preserving
component storage and the target's x87 evaluation. Float3's completed-duration
returns put the final value first and the additive initial value second,
matching the target's physical return branches. No calling-convention spelling,
formal parameter or maintained output-buffer API changes.

These source changes naturally recover both complete private-ABI owners,
including all state updates, timer paths, scalar rounding instructions and
returns. No target bytes, padding, inert operations, forced registers, new
forced-inlining attributes or fabricated returns enter the source. Byte equality
is scoped to the declared compiler/caller graph; original class names, source
return spelling and production translation-unit partition remain unknown.

Local Ghidra is read-only/noanalysis with temporary metadata rolled back,
one CPU and 768 MiB. The full mapped range 401000..465A80 inclusive is checked
before/after: 412,289 bytes, SHA256
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
The existing reviewed 887/652-byte extents are unchanged. The last bounded
review includes both vector evaluators and the still-open integer family.

All four float literal identities (0.99, 1.01, 1 and 3) are checked against target
bits. The game-speed pointer and the `__ftol2`/CalculateAnmInterpolation call
identities use their existing target mappings. Canonical comparison decodes each
entire PDB-owned extent and rejects missing/extra fields; every ordinary byte
and all twenty declared address fields replay with zero difference.

## Reproduction and scope

Both units share the existing ExecuteScript graph: `src/AnmManager.cpp`, then
`src/RandomMath.cpp`, `src/AnmVmCreate.cpp`, `src/AnmVmId.cpp`; entry
`?ExecuteScript@AnmRenderManagerView@@SGHPAUAnmVmView@@@Z`;
`/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL /GS`. Each final cold compiles all
four TUs and freezes all eleven recursive project inputs before and after.
The two PDB identities are distinct. Source-only diagnostic reuse is not exact
acceptance evidence.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit anm-evaluate-scale-interpolation \
  --unit anm-evaluate-float3-interpolation --json
scripts/repo-python scripts/report-anm-execute-table.py --check
```

Each final image also strictly preserves nine existing complete guards:
2,670 bytes/all 151 fields. Together with the new pair, eleven complete units
pass **4,209 bytes/all 171 fields** in each image. The extra existing-guard
contexts are explicitly recorded; no old canonical recipe is migrated and no
full historical native cohort is replayed.

The source controls explain why earlier near results were insufficient. Nested
expressions alone, constructor returns alone, by-value APIs, value-parameter
operators, reversed products, scalar qualifiers and multiplication/regrouping
variants either regress or leave ordinary differences. A 656-byte candidate
recovers the additive branch but still has the wrong frame/Hermite sequence.
Explicit reuse of `t+t` in the coupled value model is the first full match;
Float3 then closes by applying the same observed pattern. The 29 frozen records
include baseline, discoveries and subsequent integer-family controls; they are
not a universal rejection of a source spelling or API.

RGB/alpha factor and vector-interface controls remain nonexact and are restored.
Their layouts, semantics and ABI must be reviewed independently before any
further promotion. The existing Int3 scale operator is unchanged. The transition
host extracts byte-identical timer, viewport and RNG blocks; only two reviewed
whole-input pins change, with strict normalization/rejection unchanged.

The **9,964-byte ExecuteScript parent remains nonexact**, frame FC, all 92
physical destinations ordered, 89 equal-sized intervals and absolute length gap
14. Its aligned diagnostic remains 7184/7995; equal lengths are not exact blocks.
No parent or runtime/product credit follows from the two exact children. Whole
Windows i386 closure and later semantic/port gates remain open. The former
95% campaign is closed; see the current handoff.

All **95 standard CI steps pass** with source/config frozen. Target/table,
toolchain declarations, tracking, generated progress and whitespace pass; the
two new match rows are ordered by address. Only later documentation additions
receive focused doc/link checks. About **33.6 MB** of superseded artifacts are
pruned: 56 source snapshots become verified deltas and 28 native control dirs
are removed. One current canonical carrier remains. The pre-existing D3D work
and prior ECL carrier are untouched. See [validation summary](validation-summary.json).
