# ECL Run: shared native scalar word for six integer comparisons

Target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The `ECL_VM_EQUAL_INT`, `NOT_EQUAL_INT`, `LESS_INT`, `LESS_EQUAL_INT`,
`GREATER_INT`, and `GREATER_EQUAL_INT` opcodes consume two integer VM
stack values in pop order, compare their full 32-bit payloads as signed
integers, and push an integer truth value. The maintained arithmetic,
CALL and VM operand readers already use the native 4-byte `EclVmScalar`
union; these six comparison branches still independently declared raw
`const int` right and left temporaries. The source now uses the same
`EclVmScalar` word for both temporaries and explicitly compares their
`.integer` views. Operand widths, signed comparisons, native PopInt/PushInt
helper calls, right-before-left order, control flow and ABI are unchanged.
This describes the original serialized/stack word, not an original C++ name.

## Deliberate disproof of the tempting broader change

A separately compiled 12-opcode trial also changed six floating-point
comparisons. It retained the whole **7,020-byte** `Run` PDB extent and
all **189** independently target-bound fields but regressed original
ordinary matching from **6,997/7,020** to **6,994/7,020**. The full
candidate-to-candidate comparison identified seven changed address-mode/
register bytes, including x86 `mov edx,[ecx+eax]` becoming
`mov edx,[eax+ecx]`, an actual codegen regression despite equivalent
address arithmetic. Those six float cases were restored in full. A *fresh*
source-pinned two-TU compiler/linker build with **only six integer cases
modified** restores the existing correct frontier: the complete original
`EclVmContext::Run` matches **6,997/7,020 ordinary bytes**, validates
**189/189** independently bound fields and remains **NONEXACT** with 23
remaining differences. No large-function exact credit is claimed.

The complete **551-byte** `StartSubroutine` candidate retains its four
REL32 sites at `+0x9E/+0xD2/+0xE8/+0x1DE` and all candidate normalized
code bytes unchanged versus the preceding pinned graph. Both directly
related existing stack functions were independently original-bound selected
cold replayed, `ecl-vm-stack-pop` **122/122** and `ecl-vm-stack-push`
**96/96**: **218/218 exact**. Seven lightweight CI stages and
`git diff --check` pass; no historical all-unit cold replay.

This is a native source-model recovery, not a reconstructed whole game,
new original compiler ABI, runtime test or portability guarantee.
Private evidence:
`.analysis/gpt-web/20261106-ecl-comparison-int-only/verify.py`,
`build/gpt-web/20261106-ecl-comparison-int-only/current/native-result.json`,
`.analysis/gpt-web/20261106-ecl-comparison-int-only/selected.json`,
plus the rejected separate float/int combined trial under
`.analysis/gpt-web/20261106-ecl-comparison-scalars/`.

Current source `EclVm.cpp` SHA-256: `87ab2d4bec0ee65f6a78768de9922ee15a05b10ef7c03a61504cfd2b1edc4315`.
