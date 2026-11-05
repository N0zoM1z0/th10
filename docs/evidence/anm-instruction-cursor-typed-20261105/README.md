# TH10 full ANM ExecuteScript: typed variable-length instruction navigation

Original: Japanese TH10 v1.00a, target SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The complete `AnmRenderManagerView::ExecuteScript` at `0x0043EE30`
contains **9,964 original bytes**, not a collection of independent
fragments. The current source remains **NONEXACT**.

The actual ANM instruction header has `opcode` at +0, byte `size` at +2,
`time` at +4, variable-mask at +6 and operands at +8. Compile-time
native-offset checks now cover all header positions. The maintained
`AnmRawInstructionView::Next()` advances by the instruction's *byte*
length, not `sizeof(AnmRawInstructionView)`. The companion `AtByteOffset`
selects a script-relative instruction location with its signed encoded
byte offset. In the complete opcode interpreter these concrete operations
replace three per-instruction stream advances (interrupt search,
interrupt resume and normal dispatch advance) and three script-relative
jumps (JUMP, JUMP_DEC and conditional branch). The native instruction
ordering, variable arguments, fallback interrupt sentinel and time resets
remain exactly as in the prior maintained body.

A pinned MSVC 7.1 SP1 **five-source /GL + /GS graph** compiled the complete
modified ANM owner, retaining the existing `/Ob1` override solely for
`AnmInt3Scale.cpp`. Full PDB-owner strict original target comparison with
its prior independently reviewed 350-field contract (including **95
local/table destinations**) reproduced **9,962 / 9,964 matching bytes**.
Only ordinary register-selection differences `+0x113` (`0x4D` vs `0x55`)
and `+0x115` (`0x51` vs `0x52`) remain, around child opcode-91 argument
transport. No relocation normalization was invented, target bytes changed,
or near-match promoted to exact. The source and header hashes, candidate
PE hash and exact findings are recorded in `native-result.json`.

Focused native exact replays of `anm-vm-timer-add` 88/88,
`anm-vm-timer-decrement` 20/20, and `anm-loaded-create-vm-variant2` 73/73
passed from installed code (**181 / 181**). Seven-stage lightweight CI and
`git diff --check` passed. The local link artifact is retained in ignored
`build/gpt-web/20261105-followup-anm/typed-instruction-next/`.
No full-cohort cold replay, whole-product build or runtime claim.
