# Native ECL scalar operands share a 32-bit indexed byte stream

Target: original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The large `EclVmContext::Run @0x0044E1A0` is a **7,020-byte** original
owner with **189 independently original-bound fields**. Its current 7,020-byte
compiled body remains **6,997/7,020** original matching bytes, **NONEXACT**.

The decoded script's `EclVmInstruction::scalarOperands[1]` names the
first four-byte payload at `+0x10` of a *variable-length*, counted ECL
instruction, not a C++ array limited to one value. Its indexed reads can
traverse more than that one declared element. The previous `OperandInt` and
`OperandFloat` helpers used `instruction->scalarOperands[index]`, which
implicitly performs out-of-bounds C++ array pointer arithmetic for valid
script operand indices. The actual i386 script address is an unsigned
32-bit word: `instruction address + offsetof(scalarOperands) + index*4`,
wrapping at 2^32. A common `OperandWord` helper now forms this native word
and returns the typed four-byte scalar pointer; the integer and float
readers then select the corresponding overlay. No new allocation, range
check, ABI declaration, changed wire layout, or target-byte substitution.
The Win32 32-bit casts are target-specific, not general portability proof.

## Native full-owner trials

Four distinct natural C++ spellings were compiled with the pinned VC7.1
SP1 in the **current** two-TU `/GL` graph (`EclVm.cpp` and `Enemy.cpp`):
word addressing in the integer reader alone, the float reader alone,
both separately, and a common shared word-address helper. All four
preserved the complete **7,020-byte** `Run` PDB contribution, its
**6,997/7,020** original comparison, all **189 original-bound code/data
fields**, and its exact 171-byte `ReadFloatValue` callee. The installed
source is the common-helper variant; the other source probes were restored.

Two additional experiments constrained the operand-mask bit shift count
with an explicit `index & 31u`; both the mask-only and combined forms
changed `Run`'s complete PDB extent and were **rejected**. The existing
source mask expression was retained; we do not mislabel the rejected forms
as byte-neutral or improvements.

A **fresh installed-source**, selected cold replay of just eight previously
accepted related units passed **1,037/1,037 original bytes**:
`ecl-vm-read-float`, `ecl-vm-read-int`, `ecl-vm-read-float-value`,
`ecl-vm-resolve-float`, `ecl-vm-find-thread`, `ecl-vm-stack-pop`,
`ecl-vm-host-run`, and `ecl-vm-spawn-thread`. The full installed-source
`Run` was also compared independently against its original body and
189 fields; its **23 remaining ordinary-byte mismatches are unchanged**.
This is semantic reconstruction without any newly accepted exact owner.

Private reproducibility: `.analysis/gpt-web/20261107-session-th10/
ecl-word-operand-probe.py`, `ecl-sharedword-exact-guards.json`,
`ecl-sharedword-run.json`; local trial PE/MAP/PDB in
`build/gpt-web/20261107-session-th10-ecl/`. No historical all-unit replay,
full game runtime test, or new whole-program exactness claim.
