# TH10 ANM executor: two halfword render states and timed interpolation byte

Target is original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The full original `AnmRenderManagerView::ExecuteScript @0x0043EE30`
interprets variable-sized instructions with the already reviewed 8-byte
opcode/size/time/argument-mask header. The existing `ANM_OP_RENDER_STATE`
uses two 16-bit words from the payload at offsets +0/+2, while five timed
primary/secondary color, alpha and scale opcodes use a 32-bit duration
operand followed by a **single byte** interpolation mode at payload +4.
The other three bytes of that mode word remain unclassified; no original
source class spelling or fixed instruction size is claimed.

Maintained `AnmRawInstructionView` now overlays the existing `intArgs`,
`floatArgs`, `shortArgs`, `byteArgs`, jump and decrement-jump views with two
independent native wire prefixes: `AnmRenderStateOperandPairView` (two
unsigned 16-bit fields), and `AnmTimedInterpolationOperandsView` (32-bit
first operand, one mode byte and three opaque bytes). Compile-time checks
fix the complete view sizes, payload origin +8, second halfword at full
instruction +0x0A and mode byte at full instruction +0x0C. The actual
full `ExecuteScript` opcode switch now names **seven** authentic register/
operand accesses: two render-state writes and five timed mode reads.
There is no new allocated state or revised operand evaluation order.

## Original-bound full boss and selected leaf proof

A fresh pinned VC7.1 SP1 real **five-source `/GL /GS`** compilation
(`AnmManager.cpp`, `RandomMath.cpp`, `AnmVmCreate.cpp`, `AnmVmId.cpp` and
`AnmInt3Scale.cpp`, last with `/Ob1`) reproduces the entire **9,964-byte**
PDB-owned interpreter. The independent original-target 350-field manifest
passes **350/350**, including **95** local table and branch destinations.
Normal code bytes match **9,962/9,964**; only original opcode-91 register
bytes `+0x113` and `+0x115` remain different. This is a preserved NONEXACT
large owner, **zero new exact credit**. Previous speculation that ordinary
aggregate initialization might close the last two bytes was independently
recompiled and rejected (the same 9,962/9,964 result), not committed.

Selected canonical original-target cold replay of **three** directly related
helpers: `anm-get-int-var` (144), `anm-get-int-var-ptr` (132), and
`anm-get-float-var-ptr` (156) passed **432/432** bytes. No full historical
cold replay. Seven lightweight repository CI stages and whitespace gate
passed; no game product/runtime/portability claim.

Private native receipt:
`build/gpt-web/20261105-next-anm-render-mode/current/gptweb-proof.json`,
`.analysis/gpt-web/20261105-next-anm-render-mode/verify.py`,
`.analysis/gpt-web/20261105-next-anm-render-mode/selected-exact.json`.

Source SHA-256: `AnmManager.cpp` `649d506617f7e0d8c69aa3c7c5d1344dedf6f0453245f203af4f15eb4c448cba`; `AnmManager.hpp` `72773835994b02230434a62297d446a2681b530b359d37e7b4f9f1eef4849627`.
