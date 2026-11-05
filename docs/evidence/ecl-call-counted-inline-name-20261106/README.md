# ECL CALL instruction: native counted name and typed scalar arguments

Target: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The ECL CALL opcode begins with a 32-bit inline name byte count and the
name byte stream immediately after it. Its eight-byte tagged argument
descriptors appear after the counted name; the two tag bytes live at +0/+1
and a four-byte int/binary32 scalar lives at +4. This is a variable-length
serialized record, not a fixed-size C++ name array. `StartSubroutine`
previously accessed the count using a generic operand helper and recreated
the name via `operands + sizeof(int)`. The maintained source now uses
`EclVmCallInlinePrefixView` for the actual count/name accesses at full
instruction `+0x10/+0x14`, alongside an overlapping `EclVmScalar` word at
+4 inside `EclVmCallArgumentView`. Both record positions and widths have
native compile-time assertions. CALL's byte-sized cursor increments,
source/destination tag decisions and current caller-pointer lifetimes are
unchanged. The source does not cache a descriptor pointer across calls,
which earlier genuine trials showed moves four native call fields.

## Current-source bounded native evidence

Fresh pinned VC7.1 SP1 two-source `/GL` `EclVm.cpp` + `Enemy.cpp` produces
the complete original-bound `EclVmContext::Run` **7,020-byte** owner,
validates all **189** independently original-bound fields, and matches
**6,997/7,020** ordinary original bytes; **NONEXACT**. The actual 551-byte
candidate `StartSubroutine` (original 550) preserves **all four** call
fields at `+0x9E/+0xD2/+0xE8/+0x1DE`, with **zero candidate ordinary byte
differences** relative to the prior full native graph. No exact owner is
admitted by these candidate-regression observations.

Selected original-target canonical replays of the three directly affected
value/lookup leaves: `ecl-vm-read-int-value` **124/124**,
`ecl-vm-read-float-value` **171/171**, `ecl-vm-find-subroutine`
**136/136** — **431/431 exact**. Seven lightweight repository CI gates and
`git diff --check` pass. No full historical cold replay; original private
ABI, whole-game build, runtime validation and portability remain open.

Private proof:
`.analysis/gpt-web/20261106-ecl-call-inline/verify.py`,
`build/gpt-web/20261106-ecl-call-inline/current/native-result.json`,
`.analysis/gpt-web/20261106-ecl-call-inline/selected.json`.

Current installed SHA-256: `EclVm.cpp` `9ed17001b07a7271273bdfee1dbe7027031498161a65996ed350481c0558a612`, `EclVm.hpp` `b7f6c877aff0fb70ac53f4b55b3e5da5be0b6032532eb3b983d97604ae93fe52`.
