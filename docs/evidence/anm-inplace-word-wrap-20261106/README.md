# ANM ExecuteScript: native 32-bit in-place integer arithmetic

Target: original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The **9,964-byte** `AnmRenderManagerView::ExecuteScript @0x0043EE30`
contains both the three previously reconstructed integer assignment
operations (`ANM_OP_I_SET_ADD`, `_SUB`, `_MUL`) and the three **distinct
in-place** integer operations (`ANM_OP_I_ADD`, `_SUB`, `_MUL`). The latter
formerly used signed C++ compound expressions, which become undefined on
overflow, while the original i386 machine arithmetic retains the low 32 bits.

The maintained source now performs the three in-place operations via the
`unsigned int` alias of the actual `GetIntVarPtr(0)` destination, with the
`GetIntVar(1)` value converted to the same word type. Corresponding signed
and unsigned 32-bit types may alias the same object. The existing lvalue
resolver and argument getter are invoked in their existing expression positions,
so no operand-order change, widened store, new data layout, or bounds check is
introduced. Unsigned low-word `ADD`, `SUB` and `IMUL` preserve native wrap;
actual signed division/modulo operations are untouched.

## Original-bound full owner and selected leaf checks

Four temporary VC7.1 SP1 `/GL /GS` source experiments tested each in-place
operation independently and all three together, with only `AnmManager.cpp`
recompiled in the original five-input graph. Every one reproduced the full
9,964-byte owner with **9,962 matching original bytes**, **350 independently
original-bound fields**, and **95 local destinations**; the only remaining
ordinary differences are opcode91 `renderLayer` register allocation at
`+0x113` and `+0x115`. All temporary sources were restored byte-for-byte.

The final maintained (commented) source was independently recompiled from
**all five actual source inputs** with the pinned VC7.1 SP1
`/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL /GS` profile,
`AnmInt3Scale.cpp` on `/Ob1`, and real `ExecuteScript` entry:

```sh
scripts/repo-python .analysis/gpt-web/20261106-chat-th10-anm-word/maintained-cold.py
```

Strict original-target full-owner comparison again returns
**9,962/9,964**, **350/350 bound fields**, **95 local targets**. The large
owner remains **NONEXACT** and grants no exact ledger credit.

Only the four related exact helper owners were selected for canonical cold:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit anm-get-int-var --unit anm-get-int-var-ptr \
  --unit anm-get-float-var --unit anm-get-float-var-ptr --json
```

All four pass **856/856 bytes** and their original-target external fields;
these are previously credited guards, **zero newly exact code**. Neither a
full historical replay nor a native game product/runtime test was performed.

Retained ignored evidence:
`.analysis/gpt-web/20261106-chat-th10-anm-word/results.json`,
`selected.json`, and
`build/gpt-web/20261106-chat-th10-anm-word/maintained-cold/current-proof.json`
with PE/MAP/PDB and complete original field proof. Installed source SHA-256:
`e4c4e24b4e7c3879e2a88265c161867a3ea8db5dcfe6d9abc6bf0d833beb0707`.
The original source spelling, physical TU ownership, runnable product and
portability remain open.
