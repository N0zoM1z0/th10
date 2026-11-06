# TH10 ECL Run: shared 32-bit scalar destination for STORE_INT and STORE_FLOAT

Oracle: original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The generic ECL interpreter's `STORE_INT` and `STORE_FLOAT` opcodes pop
one four-byte tagged value and publish the 32-bit word at the address
resolved by `ResolveInt` or `ResolveFloat`. Other ECL stack and operand
paths already use the native `EclVmScalar` union (verified width four,
with integer, binary32 and raw-word views). Before this checkpoint, STORE
still passed independent raw `int *` and `float *` destinations directly
to `EclVmStackView::Pop`. The maintained implementation now passes
`&reinterpret_cast<EclVmScalar *>(ResolveInt(0))->integer` or the
corresponding `.real` address, always using `sizeof(EclVmScalar)`.
This is the same underlying memory address and four-byte transfer, without
a new allocation, altered evaluation order, new ABI or extra bound check.

## Full compiler evidence / exact leaf guards

Fresh pinned VC7.1 SP1 two-source `/GL` compilation of actual `EclVm.cpp`
and `Enemy.cpp` retains the complete **7,020-byte** PDB-owned
`EclVmContext::Run`, **189/189 independently original-bound**
relocations/local branches and **6,997/7,020** matching target ordinary
bytes. The same 23 machine-code mismatches remain, so Run is still
**NONEXACT**. `AddScriptData` and `FindSubroutine` candidate sizes remain
585 and 136 bytes; no new exact credit was admitted.

Explicit selected original-target cold replay of the directly involved
`ecl-vm-stack-pop` **122/122**, `ecl-vm-read-int` **144/144**, and
`ecl-vm-read-float` **183/183** passes **449/449 exact**. Seven-stage
repository lightweight CI and `git diff --check` pass. No historical
full cold replay or runtime/product closure is implied.

Private receipts:
`.analysis/gpt-web/20261106-ecl-store-word/verify.py`,
`build/gpt-web/20261106-ecl-store-word/current/native-result.json`,
`.analysis/gpt-web/20261106-ecl-store-word/selected.json`.

Current `src/EclVm.cpp` SHA-256: `480e2c574242417024b67b7295f5c367fc52cda59b83dd29bc704df74568a566`.
