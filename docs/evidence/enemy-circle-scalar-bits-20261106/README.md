# Enemy ECL circle interpolation: raw binary32 scalar words across motion and interpolation

Target: Japanese TH10 v1.00a executable SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The complete Enemy ECL dispatcher handles `INTERPOLATE_OFFSET_CIRCLE` and
`INTERPOLATE_BASE_CIRCLE` by retaining 32-bit representations of the two
circle-motion parameters at `EnemyMotionView` relative **+0x20/+0x24**.
These raw binary32 words must be preserved across several VM argument
readers before being stored into the **first two words** of the second
`EnemyScalarInterpolationView::initial` pair. Converting to float for these
stores, assigning a recomputed angle or changing the source-load lifetime
would change the native bit-pattern semantics.

The maintained motion record now overlays `value20`/`value24` with
`value20Bits`/`value24Bits` in C++03 anonymous unions, compile-time bound
to their original 0x20/0x24 offsets, retaining the complete 0x2C-byte
motion record. The distinct non-owning `EnemyFloat2NativeBitView` overlays
two integer words with the native eight-byte float pair. Four actual
source statements now use these types: two motion loads into the existing
`uVar4/uVar6` temporary words and two initial-value stores, in their
original explicit order. The original floating expressions before and
after those transfers, interpolation setup, timer and ECL flags are
unchanged. No new allocation, field or original type name is inferred.

## Entire boss and original-bound affected owners

Fresh pinned VC7.1 SP1 **11-translation-unit `/GL`** source compilation,
compared against its preceding current-source whole PDB graph, retained
complete candidate owner sizes, field geometry and every ordinary
candidate byte for:

- `EnemyRuntimeView::DispatchEclInstruction`: **14,352 bytes, 716 fields**;
- BeginSpell 2,432/108, death finalizer 162/9, viewport 330/10 and
  Enemy full constructor 672/5.

The original 14,416-byte Enemy dispatcher remains **NONEXACT**; these
candidate regression checks do not grant original-target byte credit.

Three directly affected **original-target canonical selected cold replays**
passed `enemy-read-int-argument` **16/16**, `enemy-read-float-argument`
**14/14**, and the **complete 652-byte**
`enemy-evaluate-scalar-interpolation` **652/652** — total **682/682
exact**. The larger evaluator confirms the header/layout change did not
regress genuine scalar interpolation codegen. This is scoped to only two
native source graph groups, not a historical all-unit cold replay. Seven
lightweight CI stages and whitespace checks pass. No whole-game product,
runtime-equivalence or portability assertion follows.

Private reproducibility:
`.analysis/gpt-web/20261106-enemy-circle-scalar-bits/verify.py`,
`build/gpt-web/20261106-enemy-circle-scalar-bits/current/native-result.json`,
`.analysis/gpt-web/20261106-enemy-circle-scalar-bits/selected.json`.

Installed source SHA-256: `Enemy.hpp` `2e2168b1d84e6dab64abb51c2d6dcdf0498c6f36856c9b1a25f18637ed35eea2`, `EnemyEclDispatcher.cpp` `c1e0e3a356f1d24956a2a3fdf95c95b8787ff7fb6c094fc0a950fc12bd40cc0a`.
