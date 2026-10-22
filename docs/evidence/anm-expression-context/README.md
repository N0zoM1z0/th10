# ANM aggregate expressions and FlipX inline context

2026-10-22, starting5584aba. Five actual private-source controls test the large
ExecuteScript owner. None is adopted; maintained ANM remains9964 bytes,
7541/7995 case-aligned positions,90/92 target-sized groups and aggregate gap2.
No source, ABI, profile, recipe or exact credit changes.

Fresh target/Ghidra review retains the known two size differences: FlipX61 uses
target EAX with a five-byte accumulator XOR, while native uses EDX/six bytes;
the common-tail case2 span compensates with one fewer byte of skipped loop
alignment. Multiplication by float-1.0 remains FMUL. No target bytes, padding,
register constraints or fake returns are introduced.

## Source controls and full-body observations

| Private source form | ANM extent | Case score / same-sized / gap | Complete-body observation |
| --- | ---: | --- | --- |
| Explicit component constructor, first vertex sum only |9964|7541/7995;90;2|8 ordinary bytes regress from target equality|
| Explicit constructor, second sum only |9964|7541/7995;90;2|All three large bodies raw-identical to baseline|
| Explicit constructors, both sums |9964|7541/7995;90;2|2 ordinary bytes regress from target equality|
| Ordinary TU-local FlipX helper |9932|7246/7998;88;40|Helper remains a CALL; FlipX27 versus52|
| Same helper with __forceinline |9964|7541/7995;90;2|Complete ANM baseline-equal after verified address replay|

The aggregate controls preserve component arithmetic, reads after the polar
calls and float stores. They replace `position + positionOffset` with the
existing three-component AnmFloat3View constructor. Unlike the preceding Enemy
motion improvement, this ANM context does not benefit from that representation.
Changing only the first sum reverses four FLD/FADD operand pairs: first vertex Z
and second vertex X/Y/Z. Changing both retains only the first-Z reversal. The
second-only form is raw-neutral. All8/2 changed displacement bytes were equal
to the target at those same complete-function offsets before the change and
are unequal afterward. Identical aggregate scores hid these regressions.

The FlipX helper contains exactly the existing XOR/OR and scale multiplication.
With ordinary static spelling, the case calls its qualified MAP symbol at the
observed helper entry; the target has no such call there. No helper exactness
is claimed. Requiring inlining restores the old53-byte case and whole extent,
not the target52-byte case. The diagnostic image retains a helper copy before
ExecuteScript, moving ANM's entry by48 bytes. Raw hashes alone therefore differ.

For the forced-inline comparison, both complete9964-byte contributions have
350 identically located/typed fields and complete decoded field coverage.
All303 changed fields bind either the same qualified semantic symbol or the
same local destination offset. No ordinal anchor identity is assumed; harness
aliases resolve back to semantic names. Replaying only these verified fields
leaves zero ordinary differences across the full owner, including alignment
and tables. This proves baseline preservation, not target exactness. The
complete field proof is in [evidence.json](evidence.json).

## Validation and retention

Original Japanese v1.00a SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Fresh disk/toolchain execution/tracking and direct Ghidra attestations pass.
Ghidra runs readOnly/noanalysis, maxcpu1,768 MiB and transaction rollback;
all412289 mapped bytes retain SHA-256
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.

Each control compiles ANM once at nice10 and reuses nine hash-verified game
objects plus two pinned harness files, retaining GL/GS and the scale-support
profile. Each passes36 complete guards4618/all121, including the194-byte/all3
direct caller, and the bounded child-routing report. Five canonical COFF owners
are additional linked comparisons only; their artifact kinds and credits do
not change. Each complete ECL diagnostic remains7020/6993 with189 fields,
62 local destinations and the exact171-byte float callee. Enemy retains14336
bytes. Raw identity of other owners is recorded per control rather than inferred
from their sizes. Full cold pairs and full CI were not run.

Private producers, overlays, reports and semantic field replay live in
`.analysis/gpt-6-astra-medium/anm-expression-context-20261022/`.
The five patches are relative to the starting source. Maintained source,
canonical artifacts/recipes and nine unrelated D3D files remain hash-unchanged.
Retain the forced-inline carrier and baseline for the303-field proof, plus all
compact reproduction inputs. Four other carriers were hash-inventoried and
removed:64 files,4266991 bytes. Whole-product/runtime and semantic/port gates
remain open. These observations do not identify original declarations or TU
ownership; they rule out improvement from the specific tested forms.
