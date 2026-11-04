# Recover typed ECL instruction opcode and operand fields in full TH10 boss

Target: original Japanese TH10 v1.00a (SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`).
The original 14,416-byte `EnemyRuntimeView::DispatchEclInstruction`
at `0x0040E770` remains **NONEXACT**. This change restores maintained
source **inside that full interpreter**, not an isolated compiler stub.

Four different branches used the same decompiler-style cast
`*(short *)(iVar27+4)` to retrieve the ECL opcode, including the main
switch, nested spell-difficulty switch and both laser-with-pattern guards.
The actual target instruction header was previously recovered and
`offsetof(EnemyEclInstructionView, opcode)==4` is statically asserted in
`src/Enemy.hpp`. All four now explicitly refer to the real `opcode` field
and preserve the *signed 16-bit* comparison/switch semantics via `short`.

Five original branches that parse raw ECL enemy-spawn arguments were still
reading the first operand as `*(int *)(iVar27+0x10)`. The established
`EnemyEclInstructionView::operands[0]` has that precise offset and width,
independently checked at compile time. These five now use the typed member,
keeping the original `+4`, `/4`, operand index and downstream raw
float/int interpretation unchanged. No fake ABI, alternate header layout,
target instruction bytes or artificial return instructions were introduced.

## Actual complete-owner VC7.1 proofs

Two separate pinned MSVC 7.1 SP1 `/GL` real 11-source Enemy update links
compiled the changed dispatcher from current source. At this epoch,
`AnmManager.cpp` and `EclVm.cpp` had changed since the historical
support-object pins and were **freshly compiled**, not incorrectly reused;
unchanged support objects required SHA-256 source/object checks.

- Typed-opcode native PDB contribution: **14,352 bytes**, **716 decoded
  address/control fields** with the same positions and kinds as baseline,
  zero differing candidate ordinary bytes.
- Typed-first-operand native PDB contribution: **14,352 bytes**, again
  **716** field positions/types identical, zero ordinary candidate-byte
  differences.

The full signed-opcode and raw-operand edits thus preserve the verified
candidate codegen at these functions. The comparison is against the
*previous generated candidate*, not against the original 14,416-byte
function. Therefore **no canonical exact unit or partial-byte credit**
is added. `opcode-result.json` and `operand-result.json` retain the
actual native artifact paths, current source SHA-256 and comparison.

Only relevant exact helper units were cold-replayed; no all-unit cold
replay, product build, runtime equivalence or original source/TU claims.
