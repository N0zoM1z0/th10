# ANM timed XYZ target payload: one 0x14-byte native duration/mode/vector record

Original: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The `POSITION_TIME` and `ROTATION_TIME` opcodes both carry an encoded
**duration dword, 32-bit interpolation mode, and three separate 32-bit
variable/literal target components**. The original 8-byte ANM instruction
header is followed by payload offsets 0x00/0x04 (duration/mode) and
0x08/0x0C/0x10 (target X/Y/Z). The maintained source now models that
full **0x14-byte** variable-length prefix with
`AnmTimedVector3TargetOperandsView`. Its mode and vector positions are
compile-time offset/size asserted; a 16-bit `variableArgs.bits` word
in the *existing header* still decides if each target component is a VM
float variable or a literal value.

The new `GET_TIMED_VECTOR_FLOAT_VAR(n)` macro expresses that same existing
mask and `AnmVmView::GetFloatVar` lookup for precisely the six XYZ
component reads in the real `ExecuteScript` switch cases. No pointer
lifetime, original masked variable semantics, argument evaluation order,
frame timer, allocation, compiler flags or original C++ type name changes.

## Current-source complete original-target native proof

A fresh five-TU pinned VC7.1 SP1 `/GL /GS` build of the current ANM
interpreter graph, including real `RandomMath.cpp`, `AnmVmCreate.cpp`,
`AnmVmId.cpp` and `/Ob1` `AnmInt3Scale.cpp`, produces the *entire*
**9,964-byte** `AnmRenderManagerView::ExecuteScript` PDB contribution.
All **350/350** independently original-bound link/control fields (including
**95** local destinations) match; **9,962/9,964** original ordinary bytes
match. The two opcode-91 register bytes at +0x113/+0x115 are unchanged,
so the big owner remains **NONEXACT**, not a new exact admission.

Only the directly affected full original-target leaves were cold replayed:
`anm-get-int-var` **144/144** and `anm-get-float-var` **424/424**,
**568/568** accepted exact bytes. Seven lightweight CI stages and
`git diff --check` pass. No full historical cold replay or whole-game
product/runtime/portability validation was attempted.

Private proof:
`.analysis/gpt-web/20261106-anm-timed-vector3-target/verify.py`,
`build/gpt-web/20261106-anm-timed-vector3-target/current/gptweb-proof.json`,
`.analysis/gpt-web/20261106-anm-timed-vector3-target/selected.json`.

Installed source SHA-256: `AnmManager.cpp` `fe0c381a142518b78ddb30b0d9d9c95a3b95bb301e1b055fc4641eb5a6960b35`, `AnmManager.hpp` `3adee93a8eaec70e0b7d469e96271126a5a631b6a44e5e17199892acd2dc9667`.
