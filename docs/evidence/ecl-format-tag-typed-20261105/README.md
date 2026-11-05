# TH10 7,020-byte ECL interpreter: recover the tagged FORMAT record

Target: hash-attested Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
`EclVmContext::Run @ 0x0044E1A0` is the complete 7,020-byte owner.

The FORMAT opcode follows an inline string at `EclVmInstruction.operands`
`+sizeof(int)`. Its subsequent variable arguments are eight-byte records:
a one-byte `type` tag, three opaque bytes, and a 32-bit integer/float
payload. The target reader uses tag at `+0` and values at `+4`. The
maintained `EclVmFormatArgumentView` now enforces `sizeof == 8` and
`offsetof(integer) == 4`, and the complete interpreter obtains the tag
through this typed field rather than an anonymous `instruction+0x14`
byte cast. The value read stays in its compiler-compatible original
expression, preserving two distinct argument lifetimes.

This choice was established by an actual source experiment, not an
assumption that a more natural C++ form matches retail: changing both
tag and numeric value loads previously collapsed the owner to 6,892
bytes, and the new narrower **float-only** typed value experiment emitted
6,940 bytes with only 188 linked fields. Both fail the complete target
boundary and were rejected. The retained **tag-only** version was built
with pinned VC7.1 `/GL` in the real `EclVm.cpp` + `Enemy.cpp` graph.

The whole modified PDB contribution is **7,020 bytes**, with **189**
linked-field positions/types matching the independent target-bound
contract, and exactly **23 ordinary target-byte differences**, identical
in position to the preceding installed source epoch. This is **NONEXACT**;
no new exact credit or original source-name claim is made.

Selected canonical replays pass: `ecl-vm-host-run` 146/146,
`ecl-vm-read-int` 144/144 and `ecl-vm-read-float` 183/183
(**473/473 bytes**). The seven-stage lightweight CI and whitespace checks
pass; no full historical replay, game-product link or runtime validation.
Reproducible local native evidence is under
`build/gpt-web/20261105-continuation-ecl-run/typed-format-tag-only/`.
