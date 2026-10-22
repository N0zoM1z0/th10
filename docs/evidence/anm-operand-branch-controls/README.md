# ANM float operand branch controls

Three actual private source controls examine rotation49, angular velocity53
and scale growth54 in the complete9964-byte ExecuteScript owner at43EE30.
The retained baseline remains the best measured form. Maintained source,
canonical recipes and exact counts are unchanged.

| Source form | ANM size | Aligned comparable bytes | Same-sized groups | Absolute gap |
| --- | ---: | ---: | ---: | ---: |
| Retained baseline | 9964 | 7541/7995 | 90/92 | 2 |
| Explicit conditional destination stores | 10012 | 7066/8001 | 86/92 | 70 |
| Literal-first value selection | 9964 | 7439/7995 | 90/92 | 2 |
| Ordinary inline operand helper | 9836 | 7277/7999 | 87/92 | 130 |

The first form preserves operand read/call/store order but grows the three
selected groups from112/112/92 to136/136/94 bytes. Reversing value-selection
branches preserves their sizes but changes instruction order and loses matching
positions. Adding an ordinary `static inline` helper still leaves eight actual
parent calls to that helper. No force-inline or ABI change is used. These are
bounded rejected forms; they do not identify the unique original source or
prove that other inlining contexts cannot work.

Each actual control compiles one private ANM GL input and reuses nine verified
source objects, keeping the existing GL/GS profile and scale-support override.
Each passes35 complete whole-function guards4424/all118 and all eight bounded
child-creation static paths. Five canonical COFF contracts supply additional
linked guards here; their canonical kind and ownership remain unchanged.

ECL remains whole7020/6993 with189 bound fields,62 local destinations and the
complete171-byte float-value callee exact. Its generic decoder reports7019 bytes
and a reviewed one-byte selector suffix; the diagnostic's generic normalization
flag remains false. Complete ECL raw bytes equal the retained baseline in all
three images. Enemy remains14352 versus target14416: the literal-first body is
raw-identical; the other two become raw-identical after replaying only changed
CALL fields whose callee symbols are independently identical in both maps.
This is candidate-to-baseline preservation, not new target exactness.

Fresh direct local Ghidra checks the existing ANM entry/call references and the
three operand groups plus final timer region. It uses readOnly/noanalysis and
transaction rollback, maxcpu1 and a768 MiB heap. All412289 mapped text bytes stay
unchanged. Ghidra's signature and noncontiguous function body remain provisional;
this review grants no new boundary or ABI claim.

TH08, TH09 and provisional TH095 source snapshots also use variable-first
conditional values and ordered component stores. This is source-shape hypothesis
material only; no adjacent address, bytes, layout, ownership or exactness transfers.
The accepted target remains original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

[facts.json](facts.json) contains source patches, producer hashes, actual results,
guard-contract revalidation and baseline comparison scope. The patches describe
rejected experiments. [validation-summary.json](validation-summary.json) records
final checks and cleanup. Compact private evidence lives under
`.analysis/gpt-6.1-sol/anm-branch-stores-20261022/`. Keep the unchanged canonical
baseline and the first10012-byte failure; the other two native carriers are
removed after their complete inventories and results are pinned.

All111 final CI groups pass with exit0. All373 final source/test/config inputs,
108 maintained source/header files,32/15 baseline pins and nine independent D3D
files stay unchanged. Focused case diagnosis, toolchain/tracking, generated
progress, documentation links and whitespace complete successfully. Cleanup
removes32 superseded native files/2083152 logical bytes.

Focused diagnostic for the retained failed control:

```sh
scripts/repo-python scripts/report-anm-execute-cases.py \
  .analysis/gpt-6.1-sol/anm-branch-stores-20261022/conditional-destination-stores.json
```

Case-aligned counts remain diagnostics with acceptance authority none. No new
exact or per-case credit, full ten-TU cold, separate ANM runtime campaign,
production-owner, whole-product, semantic or portability claim is made.
