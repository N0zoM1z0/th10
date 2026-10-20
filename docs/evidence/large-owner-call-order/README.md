# Three large owners: call order and branch observations

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Starting source checkpoint: `9807edf5878f`.

This is an active exact-reconstruction investigation, not completion of any
of the three large owners. No new exact bytes or denominator changes are
claimed. Product closure, runtime validation, semantic reconstruction and
portability remain independent and unopened.

## ANM ExecuteScript

The initial current-source diagnostic reproduced 9948/9964 owned bytes,
an FC frame, 89/92 same-sized physical intervals, absolute interval-length
gap 18, and 7400/7995 case-aligned comparable bytes.

Two source changes improve the actual target correspondence:

* Opcodes 57 and 78 snapshot the initial packed color in blue, green, red
  order. The target's byte loads and stores have this order. This removes
  eight ordinary byte differences without changing the interval sizes.
* The scale initialization method takes duration, mode, initial vector,
  final vector. Its former initial/final/duration/mode declaration was a
  reconstruction choice, not a target-established source signature.
  Declaration, definition and all real callers now agree on the new order.

At 43FD7F the target tests the duration operand's variable flag. The variable
path calls GetIntVar and transfers the result to ESI; the immediate path
loads ESI directly. The former declaration instead let the compiler merge
these loads into EAX and shortened this interval by seven bytes. Changing the
ordinary parameter order recovers this sequence while preserving the preceding
Y-then-X float evaluations and the complete 136-byte scale initializer.
No operand evaluation is interleaved between those float calls.

The five-TU cold diagnostic after both changes has 9964 owned bytes,
90/92 same-sized intervals, absolute gap 2 and 7478/7995 case-aligned
comparable bytes. The entire SCALE_TIME interval matches diagnostically.
Remaining interval-length differences are FLIP_X (+1) and the common tail
(-1); equal-sized intervals still contain register and scheduling differences.
Equal total size and local diagnostics are not exact acceptance.

Two independent five-TU cold builds have identical recursive input hashes and
identical machine-code hashes for all 15 inspected owners. Twelve existing
complete owners pass strict comparison in each image: 4579 bytes and all 183
linkage fields. Canonical RGB820/all17 fields and Int3 scale56/all3 fields also
pass in the second image. The canonical scale
initializer recipe also passes all 136 bytes and its one linkage field after
updating its decorated symbol. The target boundary, profile, linkage recipe
and existing exact credit are unchanged.

TH08's read-only reconstruction uses a duration/mode/initial/final scale
method too. This is hypothesis corroboration only. The TH10 target and compiler
comparisons above are the acceptance evidence; original source spelling and
authorship remain unknown.

## Enemy DispatchEclInstruction

Fresh baseline: 14524/14416 bytes, frame 2C4, all 108 destination groups in
target order and all 181 selector bytes equal. These do not establish
instruction equality.

The inner polar shortest-path comparison at 40F58E uses an ordered strict-less
test (`test ah,5` and a parity branch). Writing `initial < final`, with the
initial increment in its true branch, restores this condition and successor
order. The previous `initial >= final` spelling emitted `test ah,1` and
reversed the successor bodies. The enclosing absolute-difference comparison
already excludes unordered inputs from this inner comparison.

An initial investigation note incorrectly called this control code-neutral
because the unaligned whole-owner score did not change. Direct comparison of
the corresponding polar instruction sequences disproved that conclusion.
Use the branch observation, not that score, when recovering this work.
Stack placement, operand adapters, scheduling and other owner differences
remain unresolved.

## ECL Run

Fresh current-dependency measurement reproduces 7020 owned bytes and the
historical 28 ordinary differences. Operand-reading helpers preserve their
existing matching diagnostics. The generic structural probe does not fully
decode the trailing selector data, so its 6236/6264 comparable-byte result is
not an acceptance result.

A noncanonical diagnostic recipe now uses the existing strict linked-image
comparator for the entire owner. All external fields resolve through
independently existing canonical symbol/target bindings; all 62 local fields
retain their target-relative destinations. Declaring the final undecoded
selector byte as trailing data completes coverage without excluding it from
comparison. All 189 linkage fields validate, and 6992/7020 bytes match:
exactly 28 ordinary bytes still differ. The diagnostic recipe adds no canonical
unit or exact credit.

The tested local lifetime, parameter-order and equivalent-expression controls
have not removed these differences. `/G7` grows Run to 7636 bytes and breaks
several operand-helper comparisons. No ECL source or profile change is retained.

## Dependent tests

The Hint frame fixture and its signature audit use the new parameter order.
Its existing reverse-vector, wrong-duration, wrong-mode and manager-reload
mutants are updated to mutate the actual new call spelling; behavior assertions
are retained. The actual-source frame baseline passes ASan/UBSan and
float-cast-overflow checks; all 55 strict mutants are detected.

The transition-effect host test extracts only the ANM timer, viewport and
random-range helper from the two changed files. Those extracted blocks are
unchanged after line-ending normalization. The reviewed whole-file pins are
refreshed, preserving strict rejection of unreviewed inputs.

All 97 standard CI checks pass, as do toolchain declarations, tracking,
generated progress and whitespace checks. These are repository/host checks,
not original-game execution.

## Local reproduction

Use `scripts/repo-python` and the pinned target/toolchain. The canonical
`anm-start-scale-interpolation` unit checks the changed method signature.
The canonical `anm-evaluate-rgb-interpolation` five-TU graph carries the
complete current ExecuteScript and interpolation dependencies.
`scripts/report-anm-execute-cases.py` provides the scoped interval diagnostic;
it deliberately grants no exact credit.

The active local pack is
`.analysis/gpt-6-astra/large-owners-20261020/`. Its recovery note records frozen
inputs, cold/incremental distinctions, strict guard checks and restored
controls. Builds are under the corresponding `build/gpt-6-astra/` directory.
Native disassembly and generated experiment reports stay in that local pack.
No target executable, database, toolchain or copied target code is committed.
