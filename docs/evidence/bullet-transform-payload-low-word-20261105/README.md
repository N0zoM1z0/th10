# TH10 bullet transform interpreter: typed signed low word of child-pattern payload

Target is Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The complete `BulletRuntimeView::AdvanceTransformProgram` at
`0x00406D90` is a **1,454-byte** original owner that executes at most
18 transforms per bullet. The maintained `BulletTransformRecordView`
is a reviewed 0x18-byte record containing a 0x10-byte payload. The
SPAWN_CHILD_PATTERN case consumes **two** consecutive records: the
second record's payload `int0` at +8 supplies the child spawn's
`count2` via its **signed low 16-bit halfword**. The previous source
reinterpreted the address of the integer as a `short *`, relying on
a potentially invalid pointer alias. The payload now overlays the
existing 32-bit integer with `int0LowSigned16` in a union at the same
checked +8 offset. The source reads this member without a reinterpret
cast, preserving little-endian x86 signed-halfword semantics. Also,
the DECELERATE action resets `exStates[0].vector.z` using the actual
`float = 0.0f` member write rather than reinterpreting it as `int *`.
No target opcode, record stride, numeric constant or execution branch
was changed; unknown program behaviors remain unknown.

A real pinned VC7.1 **normal COFF** compile of the complete transformed
BulletTransform.cpp owner (the caller graph also compiles this support
without LTCG) produces the same **1,540-byte candidate**, with the same
**32** COFF relocation fields and **zero** ordinary candidate-to-candidate
byte differences from the prior canonical graph object. The original
complete target is 1,454 bytes; this is deliberately **NONEXACT**,
not a claim that a 1,540-byte candidate matches 1,454 original bytes.

Selected original-target VC7.1 replays of the affected real caller graphs
pass: `bullet-runtime-update` **885/885**, `bullet-update-horizontal-wrap`
**148/148** and `bullet-update-state8` **306/306** exact, totaling
**1,339/1,339 previously accepted bytes**. There is no new exact credit.
Seven-stage lightweight CI and diff checks pass. No historical all-unit
cold replay, production game link or runtime claim. Source hashes and
native comparison are in `native-result.json`; local normal object:
`build/gpt-web/20261105-transform-payload/BulletTransform.obj`.
