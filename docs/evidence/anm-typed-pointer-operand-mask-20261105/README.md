# Typed 16-bit ANM argument mask through both pointer operand paths

TH10 original Japanese 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The complete `AnmRenderManagerView::ExecuteScript` owner at original
`0x0043EE30` uses one 16-bit variable-argument mask in every instruction
header at offset `+0x06`. The earlier maintained source used
`currentInstruction->variableArgs.bits` for ordinary int/float value reads,
but still sent `currentInstruction->variableMask` to the integer and float
*lvalue pointer* resolvers. These names are members of a previously verified
same-address, same-width union. Both pointer macros now use the typed mask
word as well, covering all four instruction argument access families without
changing the original 16-bit helper ABI or the control flow.

## Complete current-source native result

A fresh, pinned VC7.1 SP1 **five-input `/GL /GS`** compile includes the
current `AnmManager.cpp` and `RandomMath.cpp`, `AnmVmCreate.cpp`,
`AnmVmId.cpp`, `AnmInt3Scale.cpp` (last under `/Ob1`). The target-bound
strict comparator validates the *complete* **9,964-byte** PDB function,
**350/350** original link/control fields, including **95** local/table
branch destinations. It matches **9,962/9,964 ordinary bytes**, with only
opcode-91 offsets `+0x113` and `+0x115` still ECX in the candidate rather
than EDX in the original. No relocated field or owner extent changed.

Explicit native exact replay of affected pointer accessors:
`anm-get-int-var-ptr` **132/132** and `anm-get-float-var-ptr` **156/156**
(288/288 strict, one selected source graph). Default seven-stage repo CI and
`git diff --check` pass. No full historical cold replay.

Private reproducer:
`.analysis/gpt-web/20261105-anm-pointer-mask/verify.py`,
`build/gpt-web/20261105-anm-pointer-mask/current/gptweb-proof.json`,
with `source.exe`, `source.map`, `source.pdb` and
`.analysis/gpt-web/20261105-anm-pointer-mask/selected-leaf-replay.json`.

**The 9,964-byte ANM boss is still NONEXACT**; no partial or new exact
credit, original source spelling, runnable product, or runtime-equivalence
claim follows from the four operand accesses sharing their typed mask.
