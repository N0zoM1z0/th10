# TH10 ANM POSITION_TIME and ROTATION_TIME: full native 0x14-byte vector target wire packet

The oracle is the pinned original Japanese TH10 v1.00a executable,
SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The complete 9,964-byte `AnmRenderManagerView::ExecuteScript` handles
`POSITION_TIME` and `ROTATION_TIME` as two timer-driven three-dimensional
interpolations. Their variable-length instruction payload encodes one
32-bit duration operand at +0, one full DWORD interpolation mode at +4,
then three native 32-bit float-or-variable operands at +8/+0xC/+0x10.
All of these offsets are relative to the regular ANM eight-byte header.
The existing common 16-bit variable operand mask still controls whether
one encoded float word names a VM variable or supplies a literal.

The maintained `AnmTimedVector3TargetOperandsView` now overlays this
20-byte serialized prefix on the existing variable-length instruction union.
Compile-time assertions prove the full 0x14-byte payload, the +4 mode and
+8 target origin, and the native four-byte component width. Both real
interpreter paths now read their XYZ target through a narrowly scoped
`GET_TIMED_VECTOR_FLOAT_VAR(n)` that uses the *same mask and GetFloatVar*
behavior as `GET_FLOAT_VAR(n)`. Component evaluation order, duration read,
mode, timer initial/final tangents, and `ResetTimer` remain unchanged.
The union owns no additional bytes, creates no new instruction allocation,
and does not claim historical source names or a private calling convention.

## Current-source native and original-bound evidence

A fresh pinned VC7.1 SP1 five-source `/GL /GS` graph, with
`AnmInt3Scale.cpp` under `/Ob1`, recompiled the actual changed
`AnmManager.cpp` and header. The whole **9,964-byte** PDB-owned
`ExecuteScript` source contribution compares against the original with
**9,962/9,964** ordinary code bytes, all **350 independently original-bound
fields** and **95 local jump/table destinations**. The two opcode-91
register-encoding mismatches `+0x113/+0x115` remain; the large executor is
**NONEXACT** and receives no new exact credit. The native compile preserved
all existing exact owner boundaries and complete field geometry.

Explicitly selected cold original-target replay of the actual operand
helpers `anm-get-int-var` **144/144**, `anm-get-float-var` **424/424**, and
`anm-get-float-var-ptr` **156/156** passed **724/724** exact bytes in one
scoped source graph. The seven lightweight CI stages and whitespace checks
pass. No full historical replay, complete product build or runtime-equivalence
claim is made.

Private proof:
`.analysis/gpt-web/20261106-anm-timed-vector3-target/verify.py`,
`build/gpt-web/20261106-anm-timed-vector3-target/current/gptweb-proof.json`,
`.analysis/gpt-web/20261106-anm-timed-vector3-target/selected.json`.

Maintained source SHA-256: `AnmManager.cpp` `fe0c381a142518b78ddb30b0d9d9c95a3b95bb301e1b055fc4641eb5a6960b35`, `AnmManager.hpp` `3adee93a8eaec70e0b7d469e96271126a5a631b6a44e5e17199892acd2dc9667`.
