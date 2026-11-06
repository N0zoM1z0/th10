# ANM opcode 84: native 0x38-byte pair of 0x1C-byte render vertices

Target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The complete ANM interpreter's `ALLOC_VERTICES` opcode sets `renderMode=9`
and allocates **two consecutive 0x1C-byte** `AnmRenderVertexView` objects
per requested logical point. The original multiplies its encoded integer
operand by the **0x38-byte** pair stride. The maintained source previously
expressed the same byte count as `GET_INT_VAR(0) * sizeof(vertex) * 2`.
The newly reviewed `AnmGeneratedVertexPairView` describes that actual
contiguous pair as `first` at +0 and `second` at +0x1C; native compile-time
assertions bind its total size to 0x38. The real opcode now uses
`GET_INT_VAR(0) * sizeof(AnmGeneratedVertexPairView)` for the original
allocation. VM ownership remains unchanged: `generatedVertices` still
holds the same `malloc` pointer, and `RemoveVm`/other lifecycle frees the
same allocation. No new heap allocation, vertex stride, instruction
payload, failure-path guard or historical original type name is inferred.

## Entire original-bound boss and dependent exact leaf checks

A **fresh five-translation-unit pinned VC7.1 SP1 `/GL /GS`** build of
`AnmManager.cpp`, `RandomMath.cpp`, `AnmVmCreate.cpp`, `AnmVmId.cpp`,
`AnmInt3Scale.cpp` (`/Ob1` on the last) retains the complete original-bound
**9,964-byte** `AnmRenderManagerView::ExecuteScript` PDB owner. It validates
**all 350 independent target fields including 95 local switch/branch
destinations** and matches **9,962/9,964 original ordinary bytes**. The
two unresolved register encoding differences at opcode 91 +0x113/+0x115
remain; `ExecuteScript` is still **NONEXACT**, so no new canonical exact
credit is claimed. This is a genuine current-source compiler and original
field/byte proof, not a size-only assertion.

Original-target selected cold replay of the affected helper interface
`anm-get-int-var` **144/144** and `anm-get-int-var-ptr` **132/132** passes
**276/276 exact**. The default seven-stage lightweight CI and
`git diff --check` also pass. No historical full cold replay was run, and
whole-game product, runtime equivalence, original private ABI and
portability are not claimed.

Private reproduce:
`.analysis/gpt-web/20261106-anm-generated-vertex-pairs/verify.py`,
`build/gpt-web/20261106-anm-generated-vertex-pairs/current/gptweb-proof.json`,
`.analysis/gpt-web/20261106-anm-generated-vertex-pairs/selected.json`.

Current source SHA-256: `AnmManager.cpp` `7af073bbdb66376e54d3e7b242f39855a586ae7843e86411766479714d8c8c32`, `AnmManager.hpp` `23f4e6517de057e72ab34098be290d545e6f25411613fb79085b30815230e425`.
