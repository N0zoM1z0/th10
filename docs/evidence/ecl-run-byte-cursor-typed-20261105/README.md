# TH10 7,020-byte ECL Run: typed byte cursor and format-string origin

Original Japanese TH10 v1.00a executable SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The original complete `EclVmContext::Run` at `0x0044E1A0` has **7,020
bytes** and is still **NONEXACT**.

`EclVmInstruction` stores `opcode` at +4, `size` at +6,
`difficultyMask` at +0xA, and operands starting at +0x10; the new native
`offsetof` assertion covers these fields. The ECL interpreter formerly
used unsigned-char intermediate pointer casts plus an `+=` to advance the
instruction cursor. The maintained `EclVmInstruction::AtByteOffset(int)`
now names the real signed *byte* displacement shared by ordinary instruction
advances and script-control-flow jumps. The FORMAT opcode's inline string
begins at `instruction->operands + sizeof(int)` (+0x14), made explicit in
its source without changing the evaluation order or opaque numeric records.

## Complete linked original comparison, not an isolated source score

A fresh pinned VC7.1 /GL `EclVm.cpp` + `Enemy.cpp` link was run **before**
the edit and against the **installed edited source**. Both provide the full
**7,020-byte** PDB-owned body with all **189** linked field positions/types
matching the independently recorded previous target-bound contract.
Both retain **23 ordinary target-byte discrepancies at the same offsets**
(`+0xAF`, `+0xEC`, `+0xF1`, `+0x12A`, `+0x12C`, `+0x1FB`,
`+0x151D` and the other x87/FORMAT/loop-tail offsets listed in
`native-result.json`). These comparisons mask only the 189 diagnosed
address fields and do not infer exactness from the target-sized body.

A separate attempt to represent the FORMAT variable arguments as typed
8-byte records resulted in **6,892 candidate bytes and 188 linked fields**,
breaking the complete native layout. That variant was rejected and restored;
it is not the retained source and receives no exact credit.

On the new current source, selected existing canonical byte-exact units
`ecl-vm-host-run` 146/146, `ecl-vm-read-int` 144/144, and
`ecl-vm-read-float` 183/183 were independently cold-replayed exact
(**473/473 bytes**). Seven-stage lightweight CI and whitespace check
passed. No unselected historical replay, product-link, runtime, original
source-name or new exact-credit claim is made.

Full ignored graphs: `build/gpt-web/20261105-continuation-ecl-run/`.
