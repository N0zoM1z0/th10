# TH10 ECL ReadInt: reinterpret float-tagged popped word through the native scalar view

Target: original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The complete `EclVmContext::ReadInt` handles an indirect `-1` operand
by popping a 32-bit value and the preceding four-byte type slot. If the
record's first type byte is `f`, the value is interpreted as binary32,
then converted to signed integer; it must **not** convert the raw 32-bit
bits as an integer. The maintained source previously reinterpreted the
local `index` through an unrelated `float *`. That same VM already has
the native four-byte `EclVmScalar` union for encoded int/float values,
stack arguments and arithmetic operations. The current source uses
`reinterpret_cast<const EclVmScalar *>(&index)->real` for the existing
float-tagged path. The stack-top adjustments, tag read, conversion,
variable-indirection and returned int are untouched. No new stack slot,
ABI or historical private source identifier is added.

## Complete-source and original-bound regression evidence

A fresh pinned VC7.1 SP1 **two-source `/GL`** native link of changed
`src/EclVm.cpp` and its Enemy support owner keeps the complete
**7,020-byte** `EclVmContext::Run` PDB extent with **189/189
independently original-bound fields**, including branch/table targets;
**6,997/7,020** ordinary target bytes match. Run is still **NONEXACT**
with the same 23 remaining bytes, so no new ledger credit.

The affected accepted `ecl-vm-read-int` source owner was explicitly
cold replayed **144/144 exact**, together with the actual typed stack
reader `ecl-vm-stack-pop` **122/122 exact**, totaling **266/266** original
bytes. Seven repository lightweight CI gates and `git diff --check` pass.
No all-unit cold replay, game product closure, runtime proof, or portability
claim follows.

Private native reproduce:
`.analysis/gpt-web/20261106-ecl-readint-floatbit/verify.py`,
`build/gpt-web/20261106-ecl-readint-floatbit/current/native-result.json`,
`.analysis/gpt-web/20261106-ecl-readint-floatbit/selected.json`.

Current `src/EclVm.cpp` SHA-256: `80271137a8df906f23697048dff7c6ba0f0ce3b71402a160906cdd596e5e6ef0`.
