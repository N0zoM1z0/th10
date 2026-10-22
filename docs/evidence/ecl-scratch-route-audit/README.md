# ECL source-transform audit and actual scratch controls

2026-10-22, startingffbf0c8. Inspecting historical patches exposed a source-generator
routing defect in the earlier ECL layout campaign. Five actual current-context
controls now test the intended forms with explicit source-shape assertions.
No maintained game source, ABI, recipe or exact count changes.

## Corrected historical interpretation

The old private probe's broad Run-definition-order `elif` included labels later
handled by separate shared-scratch, reference-cursor and named-destination
branches. Those later branches were unreachable. The four profile labels also
moved Run before HostRun while applying their profile flags.

This is confirmed independently of labels or the current private script:
all seven affected records contain the same source SHA-256 as `run-before-host`,
`4f5e445c5cbcb36b77c04c9f5f3acc265c97f3b1c59df955dd8264bab0a73ab4`.
Their recorded patches are identical. Applying the shared-scratch record's patch
to src/EclVm.cpp from Git15cd8b1 reconstructs exactly that hash. The reconstructed
source moves Run before HostRun and contains none of the intended shared operands,
pointer reference or destination locals. It is not evidence that those forms
were neutral. Existing raw-byte comparisons remain observations of the source
actually compiled; no canonical exact proof or comparator rule is invalidated.

The [old README](../ecl-run-layout-controls/README.md), its structured evidence,
historical handoff paragraph and knowledge note are corrected. Original patches,
hashes and measured outputs are preserved. Do not reuse the old private generator
as an implementation of the misleading labels. The new producer asserts the
actual transformed expressions and preserves maintained HostRun-before-Run order.

## Actual current-context results

| Verified source form | Run extent | Strict complete-owner result |
| --- | ---: | --- |
| Shared two EclVmScalar scratch values across math and FORMAT |7036|Extent rejected; frame10C instead of108|
| Reverse those two declarations |7036|All three complete large bodies raw-identical to preceding control|
| Separate scratch union, existing payload union untouched |7036|All three complete large bodies raw-identical to first control|
| Name both math ResolveFloat destination locals |7020|External field at+18BC has no corresponding prebound target|
| Actual reference to the instruction pointer field |7020|Baseline-identical three large raw bodies; same27 differences|

The shared models use the same union members for each value's live interval:
length X/angle DY/FORMAT index share one scratch value; length Y/angle DX/FORMAT
buffer share the other. They retain read ordering, expressions, branch-local
counter increments and the disabled trace. Each read uses its previously written
active member; no bit punning, padding, target bytes or ABI coercion is introduced.
The separate-union control demonstrates that merely extending the original
payload union is not responsible for the observed7036-byte result in this graph.

The named-destination image has a ResolveFloat call field at6332/18BC; the target
bytes there do not encode the declared matching call. The unchanged strict
Oracle rejects it before an ordinary-byte score. Equal total extent does not
justify shifting or masking that field. The reference control binds all189 fields
and62 local destinations, keeps the full171-byte float callee exact and retains
all27 differing offsets and byte values from the fresh canonical baseline.
These conclusions apply to the tested compiler graph, not original authorship.

## Validation and retention

Private producers, frozen manifest, overlays, source assertions, target review
and complete reports:
`.analysis/gpt-6-astra-medium/ecl-scratch-route-audit-20261022/`.
The patches here apply to maintained source at the starting commit.
[evidence.json](evidence.json) records the historical Git/patch reconstruction,
actual source/producer hashes, owner comparisons and inventories.

Fresh disk/toolchain execution/tracking and direct read-only Ghidra checks pass.
Original Japanese v1.00a target SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
All412289 mapped bytes retain
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
Ghidra uses rollback, readOnly/noanalysis, one CPU and768 MiB heap. Each experiment
compiles only ECL serially at nice10, reuses nine verified game objects/two
harness inputs and retains GL/GS. All five producers finish exit0 and pass nine
complete ECL guards1137/all18; the four recorded Run rejections remain rejections.

Maintained inputs, canonical artifacts/recipes and nine unrelated D3D files are
hash-unchanged. All five new carriers were verified and removed:80 files,
5333146 bytes; the temporary historical source reconstruction was also removed
after hash verification. Compact reproduction inputs remain. Focused checks
replace full cold replay/full CI. Maintained Run remains7020/6993 with27 ordinary
differences. Authored409/74716 and overall1252/141647 exact counts do not change;
whole-product/runtime, semantic and port gates remain unopened.
