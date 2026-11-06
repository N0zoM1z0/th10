# ECL Run: shared 32-bit stack words across logic and bitwise opcodes

Original: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The ECL `LOGICAL_OR`, `LOGICAL_AND`, `BITWISE_XOR`, `BITWISE_OR` and
`BITWISE_AND` instructions each pop two 32-bit signed integer VM stack
values (right before left), execute the original boolean/bitwise expression,
and push a single full DWORD with type tag `'i'`. The existing VM stack,
arithmetic and six comparison branches already model the **same physical
four-byte payload** as `EclVmScalar`; these five opcodes previously
re-declared raw `const int` temporaries. Their maintained source now uses
`EclVmScalar left/right` and `.integer` for the existing comparisons/bitwise
operators. The real `PopInt`/`PushInt` calls, operand order, short-circuit
boolean expression, signedness, and original VM stack tag are unchanged.
No new native buffer, synthetic alignment, fake calling convention, or
original developer's C++ identifier is asserted.

## Current complete original-target native evidence

One fresh pinned VC7.1 SP1 **two-translation-unit `/GL`** native graph
(`src/EclVm.cpp` + `src/Enemy.cpp`) emits the complete original-bound
**7,020-byte** `EclVmContext::Run` PDB owner, **189/189 independently
original-bound** address/branch/table fields and **6,997/7,020** matching
original ordinary bytes. The same 23 prior unresolved machine-code bytes
remain, so this large owner is **NONEXACT** with no new ledger credit.
`AddScriptData` remains a 585-byte nonexact candidate and `FindSubroutine`
retains 136 bytes. The separately inspected full **551-byte**
`StartSubroutine` retains four call relocations at
`+0x9E/+0xD2/+0xE8/+0x1DE`, zero normalized candidate byte differences
from the preceding native source graph. This comparison of candidate
owners is regression evidence, not a claim that StartSubroutine is exact
(the original is 550 bytes).

Selective original-target *cold* replay of the two actual stack owners
`ecl-vm-stack-pop` **122/122** and `ecl-vm-stack-push` **96/96** passes
**218/218 exact**. The seven-stage lightweight repository CI and whitespace
check pass. No historical all-unit replay, complete game product, runtime
or portability claim follows from this proof.

Private reproduction:
`.analysis/gpt-web/20261106-ecl-logic-word/verify.py`,
`build/gpt-web/20261106-ecl-logic-word/current/native-result.json`,
`.analysis/gpt-web/20261106-ecl-logic-word/selected.json`.

Installed source SHA-256 `src/EclVm.cpp`: `00c4cebf8caf2a950ecd814d222459baab0b74378a80eda6a1f260fe6630f08e`.
