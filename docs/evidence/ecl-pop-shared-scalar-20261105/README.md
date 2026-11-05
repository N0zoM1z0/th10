# ECL VM: shared typed scalar word through Push/Pop and CALL

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`EclVmScalar` is the 4-byte value word read by the generic ECL interpreter,
operand reader/lvalue resolvers, and `StartSubroutine` CALL argument writers.
The previous union definition lived after the stack implementation in
`EclVm.cpp`, so the earlier `EclVmStackView::Pop` still used independent
`int*` and `float*` casts to convert the same four bytes. This checkpoint
moves the unchanged union to `EclVm.hpp` (with a 4-byte compile-time size
assertion and a forward declaration for its instruction-pointer overlay) and
uses the same scalar view for both actual Pop conversion directions. The
stack's separate optional type-tag slot remains exactly four bytes; the
readable word and its address are unchanged. This is a C++ wire/storage model
of observed x86 behavior, not a claim about original class names.

## Full native compiler and target-bound checks

A fresh pinned VC7.1 SP1 `/GL` two-source `EclVm.cpp` + `Enemy.cpp` native
link retains the **7,020-byte** complete `EclVmContext::Run` PDB owner,
all **189** independently original-bound fields, and **6,997/7,020**
matching ordinary bytes. Its **23 remaining** differences still make the
whole Run owner NONEXACT. The 551-byte `StartSubroutine` candidate (original
550) has exactly **four** unchanged call fields at `0x9E, 0xD2, 0xE8,
0x1DE` and zero ordinary candidate-to-candidate byte differences relative
to the prior pinned graph. Both are explicitly not admitted as exact.

Only the directly affected accepted stack owners were cold-replayed against
the original target: `ecl-vm-stack-pop` **122/122** and
`ecl-vm-stack-push` **96/96**, **218/218** strict original bytes. The
seven-stage lightweight `scripts/ci.py` and `git diff --check` passed.
No full historical cold replay or whole-game/runtime/portability proof.

Private reproduce: `.analysis/gpt-web/20261105-ecl-pop-scalar/verify.py`,
`build/gpt-web/20261105-ecl-pop-scalar/current/native-result.json`,
`.analysis/gpt-web/20261105-ecl-pop-scalar/selected-exact.json`.
