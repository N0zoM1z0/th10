# TH10 ECL CALL: typed 32-bit numeric operand words without changing cursor lifetime

Original: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The `EclVmContext::StartSubroutine` CALL argument loop reads an 8-byte
serialized descriptor for each argument (source type +0, destination type +1,
32-bit scalar +4). The previous code already used checked typed descriptor
tags and byte strides, but still independently cast the entire instruction's
operand bytes to `const int *` in **two** scalar reads. The maintained code
now uses `EclVmInstruction::integerOperands[valueOffset / sizeof(int)]`.
This is the **existing +0x10-overlaid integer-word view** of the same
variable-length instruction; no new array or alignment assumption was added.
The separate unsigned scalar byte cursor is retained across the original
float/int service calls. Its independent lifetime matters to VC7.1 codegen.

A genuine source experiment reading `EclVmCallArgumentView::integer` via the
metadata cursor directly emitted a **527-byte** complete StartSubroutine
candidate versus the previous 551 bytes and original **550**. Its changed
source was restored; it is not a successful target match.

The retained typed-word version was compiled in the real, pinned MSVC 7.1
SP1 `/GL` two-source `EclVm.cpp` + `Enemy.cpp` graph. PDB-owned *complete*
`StartSubroutine` remains **551 candidate bytes**, all four linked fields
in identical positions/types and **zero** other candidate byte changes.
`EclVmContext::Run` remains **7,020 candidate bytes** with 189 target-bound
linked fields. A new full independent original-target comparison passed all
189 binding identities, local/table targets and the complete owner boundary,
with **6,997/7,020 ordinary bytes** matching and the same **23 original
mismatches**. `FindSubroutine` remains an unchanged 136-byte candidate.
No new full-function exact admission is claimed for the nonexact owners.

Five explicitly selected original-bound exact replays passed on the final
source: `ecl-vm-host-run` 146, `ecl-vm-read-int` 144,
`ecl-vm-find-subroutine` 136, `ecl-vm-stack-push` 96,
`ecl-vm-stack-pop` 122, totaling **644/644 previously accepted bytes**.
Source hashes and complete-owner diagnostics are in `native-result.json`,
and private PE/MAP/PDB artifacts are under
`build/gpt-web/20261105-ecl-call-words/full-run/`.
Only the affected graph and named exact units were built; no historical full
cold replay or runtime/product claim.
