# Handoff checkpoint: native 32-bit wrap in three ANM integer assignment opcodes

Target: original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

This source work was **already dirty when the handoff-cleanup task began**;
it was inspected, preserved, tested, and committed rather than discarded.
The complete `AnmRenderManagerView::ExecuteScript` has three opcode cases
`ANM_OP_I_SET_ADD`, `ANM_OP_I_SET_SUB`, and `ANM_OP_I_SET_MUL` that perform
32-bit arithmetic on VM integer words. The retail x86 results wrap modulo
2^32. The maintained source now performs these operations in `unsigned int`
then converts the 32-bit word back to `int` for the existing destination,
removing signed-overflow UB without changing the target's signed integer
value representation. Each case preserves original `GET_INT_VAR_PTR` and
`GET_INT_VAR` evaluation sequence and its instruction variable bit mask.

## Current-source native proof, not a new exact large owner

A fresh genuine pinned VC7.1 SP1 five-source `/GL /GS` graph recompiled
`src/AnmManager.cpp` from the dirty source and retained the complete
**9,964-byte** PDB owner. The independent original target field contract
validated **350/350** bound relocations/branches/table destinations,
including **95** local targets, and **9,962/9,964** ordinary original bytes.
The two unresolved opcode-91 register bytes remain at +0x113/+0x115;
`ExecuteScript` remains NONEXACT, with no new ledger credit.

Explicit selected original-bound cold replay under changed source passed
all four affected ANM value and lvalue helpers: `anm-get-int-var` 144/144,
`anm-get-int-var-ptr` 132/132, `anm-get-float-var` 424/424, and
`anm-get-float-var-ptr` 156/156, **856/856 exact bytes**. The seven-stage
lightweight CI and `git diff --check` passed. No historical all-unit cold
replay or whole-game/runtime proof was made.

Private reproducible receipts:
`.analysis/gpt-web/20261106-handoff-anm-wrap/verify.py`,
`build/gpt-web/20261106-handoff-anm-wrap/current/gptweb-proof.json`,
`.analysis/gpt-web/20261106-handoff-anm-wrap/selected.json`.

Installed `src/AnmManager.cpp` SHA-256: `94070661d61c5e144811bf220ac4cd618aac5cdfc0b3b3f7941a0ce4e12ff973`.
