# TH10 ANM: one 32-bit operand word across full variable/literal value and lvalue paths

Target: pinned original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The complete 9,964-byte `AnmRenderManagerView::ExecuteScript` interprets a
variable-sized ANM instruction. Every int/float operand is one 4-byte word;
the independent 16-bit `variableArgs.bits` at full header +0x06 controls
whether it names a VM variable or a literal. The source previously sent
`intArgs[n]`/`floatArgs[n]` separately through the four core value and
write-address access macros. The maintained instruction payload now overlays
these word arrays with `AnmOperandScalarWordView scalarArgs[10]`, whose
`integer`, `real` and raw bit members occupy the *same* native four bytes.
The four real interpreter macros `GET_INT_VAR`, `GET_FLOAT_VAR`,
`GET_INT_VAR_PTR`, and `GET_FLOAT_VAR_PTR` use that view without changing the
original 16-bit mask, macro evaluation order, helper signatures, native
operand positions or variable-length instruction traversal. This is a
unified serialized word interpretation, not new data, a new ABI, or an
assertion about the original developer's type names.

Compile-time assertions prove 4-byte scalar width, operand start +0x08,
and next operand start +0x0C. On VC7.1, indexing an array element inside
`offsetof` is not an accepted C++03 constant expression, so the next
position is bound by the first member offset plus `sizeof` one word; no
host-specific extension or compiler flag was added.

## Whole boss and directly affected exact leaves

A **fresh pinned five-source VC7.1 SP1 `/GL /GS`** rebuild of the actual
`AnmManager.cpp` graph, including `RandomMath.cpp`, `AnmVmCreate.cpp`,
`AnmVmId.cpp` and `AnmInt3Scale.cpp` (`/Ob1` last), passes the independently
original-bound complete **9,964-byte** `ExecuteScript` function contract.
All **350** original address/branch/table fields, including **95** local
jump destinations, validate; **9,962/9,964** ordinary target bytes match.
The only two differences remain the known opcode-91 register bytes at
`+0x113/+0x115`; the boss is **NONEXACT**, no new ledger credit.

Selected cold original-target replay of **all four affected accessors**:
`anm-get-int-var` **144/144**, `anm-get-float-var` **424/424**,
`anm-get-int-var-ptr` **132/132** and `anm-get-float-var-ptr` **156/156**
passes **856/856 exact** across only their two relevant native artifacts.
Seven lightweight CI stages and `git diff --check` pass; no all-unit cold
replay, whole-game product, runtime or portability claim is made.

Private proof:
`.analysis/gpt-web/20261106-anm-scalar-operand/verify.py`,
`build/gpt-web/20261106-anm-scalar-operand/current/gptweb-proof.json`,
`.analysis/gpt-web/20261106-anm-scalar-operand/selected.json`.

Current source SHA-256: `AnmManager.cpp` `0573896fb5b5728af19fc30683aa3f35dc4b88886847786bf8fb192e23fa6547`, `AnmManager.hpp` `7699948c6b160b097b8af9b01c69a898d31d12501994f1d02fef9552803746fe`.
