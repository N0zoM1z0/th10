# Enemy CREATE_ENEMY dynamic scalar tail — shared typed ECL words

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The complete Enemy extended ECL dispatcher processes relative, absolute,
global-offset and mirrored CREATE_ENEMY opcode variants. Each begins with a
counted inline name; the following 32-bit scalar tail is located using the
original `(inlineNameByteCount + 4) / 4` integer index. The maintained source
had independently cast the resulting words as 13 float and 15 integer
array entries, even though the generic ECL interpreter uses the same four
bytes as an `EclVmScalar` integer/IEEE-754 overlay.

`EnemyEclInstructionView` now aliases `scalarOperands[1]` onto the existing
operand stream at full instruction +0x10, bound to the independently
reconstructed generic `EclVmInstruction::scalarOperands` by compile-time
`offsetof` and sizeof assertions. All **28** dynamic-tail accesses in the
complete Enemy dispatcher now use `.real` or `.integer`. There is no
buffer allocation, new alignment, altered operand index, lifted pointer
lifetime, changed evaluation order, or claim to recover original C++ names.

## Scoped original-bound and complete candidate results

A fresh genuine pinned VC7.1 SP1 **11-input `/GL`** compilation preserved
all five complete PDB-owned candidate owners relative to the immediately
preceding matching-source native graph:

- `DispatchEclInstruction`: **14,352 bytes**, **716** candidate link fields,
  unchanged field geometry and zero ordinary candidate differences.
- `EnemyBeginSpell`: **2,432** bytes, 108 fields; finalizer **162** bytes,
  9 fields; viewport **330** bytes, 10 fields; full Enemy constructor **672**
  bytes, 5 fields — all zero-regression candidate comparisons.

Selected cold original-target replay of the three directly involved
parameter readers: `enemy-read-raw-int-argument` **17/17**,
`enemy-read-raw-float-argument` **26/26**, and `enemy-read-int-argument`
**16/16**; **59/59** strict original bytes and no new accepted unit.
Seven-stage lightweight CI and whitespace checks pass. The original
Enemy dispatcher is **14,416 bytes**, so the 14,352-byte owner remains
**NONEXACT**: byte-neutral typed recovery is not a new exact claim or a
whole-game/product/runtime assertion.

Private reproducer:
`.analysis/gpt-web/20261106-enemy-spawn-scalars/verify.py`,
`build/gpt-web/20261106-enemy-spawn-scalars/current/native-result.json`,
`.analysis/gpt-web/20261106-enemy-spawn-scalars/selected.json`.

Current source SHA-256: `Enemy.hpp` `f34369ef9e3c2e45e4d3fa2f71054b5e5750e173e1e5166da48e62d6dee5f40a`, `EnemyEclDispatcher.cpp` `445b1a33e110dbb871fd8c1bda20b1c11c1b4791d662dbd5fa9ad313112afa54`.
