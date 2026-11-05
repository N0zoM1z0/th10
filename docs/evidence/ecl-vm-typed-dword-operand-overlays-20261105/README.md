# TH10 full ECL Run: typed overlapping instruction operand words

Target: original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The complete `EclVmContext::Run` target at `0x0044E1A0` owns **7,020 bytes**
and remains NONEXACT.

The reviewed ECL instruction header places the variable-length operand stream
at byte offset +0x10. The first operand bytes may be accessed either as a
32-bit integer, IEEE-754 float or byte-packed script data. The installed
`EclVmInstruction` now overlays its `operands[1]` with
`integerOperands[1]` and `floatOperands[1]` in an anonymous union and asserts
all three offset +0x10. The actual `OperandInt` / `OperandFloat` readers use
those members directly, preserving the existing variable-size traversal and
signed byte-jump calculation. This models storage already established by
TH10's compiled ECL interpreter without imposing an inferred source filename
or changing the instruction's encoded size.

A fresh pinned VC7.1 `/GL` two-source real ECL/Enemy graph compiles the
complete modified owner and compares it with **all 189 independently recorded
link-field positions/types**; the result is exactly the previous
**7,020-byte candidate**, with **23** ordinary original-target differences
at identical locations (listed in `native-result.json`). That remains a
strict NONEXACT result, not 7,020 matched original bytes.

Selected native canonical cold replays of `ecl-vm-host-run` (146 bytes),
`ecl-vm-read-int` (144 bytes), and `ecl-vm-read-float` (183 bytes)
remain byte-exact, totaling **473/473 previously credited bytes**.
The seven-stage lightweight CI and whitespace validation passed. Only affected
units were checked; no full historical replay or product/runtime claim.
Native linked evidence retained in ignored
`build/gpt-web/20261105-continuation-ecl-run/ecl-typed-word-operand-overlays/`.
