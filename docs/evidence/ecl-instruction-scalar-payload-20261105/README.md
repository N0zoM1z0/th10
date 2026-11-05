# TH10 ECL instruction scalar payload shared with VM stack words

Target: original Japanese TH10 v1.00a (`resources/th10.exe`), SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The generic variable-length `EclVmInstruction` encodes its operand stream at
header offset **+0x10**. The established maintained C++ had independent
`unsigned char`, integer, and float views of the same serialized operand
word. It also had a verified 4-byte `EclVmScalar` union for the corresponding
ECL VM stack payload, including its raw IEEE-754 representation.

This checkpoint adds **`EclVmScalar scalarOperands[1]` as an overlapping
instruction-stream view**, not a new field or buffer allocation. Compile-time
layout assertions bind the scalar, byte, integer and float aliases all to the
same +0x10 origin with four-byte word size. `OperandInt` and `OperandFloat`
use this typed word. The `StartSubroutine` CALL loop likewise uses the typed
integer member of its two 8-byte argument records to load the original scalar
value; it still reads each source/destination tag from the actual wire
record separately, and advances its own byte cursor exactly as before.

This is a source/wire-type recovery spanning the **full 7,020-byte VM
interpreter, subroutine dispatcher, and leaf readers**. It does not claim to
recover unknown original C++ identifiers, ABI, or runtime/product closure.

## Bounded original-bound and source-regression proof

A fresh pinned VC7.1 SP1 **two-source `/GL`** compiler/linker build retains
the entire `EclVmContext::Run` **7,020-byte PDB contribution**, passes **all
189 independently target-bound fields including local branch/table targets**,
and reproduces **6,997/7,020 ordinary target bytes** (23 still differing).
It is **NONEXACT**. The separately complete `StartSubroutine` remains
**551 candidate bytes versus original 550**, retaining its four target call
field sites at `0x9E/0xD2/0xE8/0x1DE`, with **zero ordinary candidate
code differences** versus the preceding source-pinned build. `AddScriptData`
remains 570 candidate bytes (original 589); `FindSubroutine` remains 136.
No nonexact owner is silently admitted.

Target-bound selected exact cold replay for five directly affected existing
owners — `ReadInt`, `ReadFloat`, `ReadIntValue`, `ReadFloatValue`, and
`FindSubroutine` — passed **758/758 complete original bytes**. The seven
lightweight CI gates and `git diff --check` passed, with no all-unit replay.

Private reproduction:
`.analysis/gpt-web/20261105-turn-ecl-operands/verify.py`,
`build/gpt-web/20261105-turn-ecl-operands/current/native-result.json`,
`.analysis/gpt-web/20261105-turn-ecl-operands/selected-exact.json`.
All original bytes and native symbol identities were compared; no target
instructions or private binaries were added to maintained source.
