# TH10 Enemy ECL: native inline callback-name payload offsets

Original: Japanese TH10 v1.00a, target SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The complete Enemy ECL extended dispatcher handles `SET_INTERRUPT` by
reading three typed generic ECL operands (callback slot, life and time
thresholds), then passes a byte name beginning at the *fifth* operand
word (`&operands[4]`) to `EnemyConfigureInterrupt`. `SET_TIMEOUT` reads
an index and installs an inline callback-name pointer beginning at the
*third* operand (`&operands[2]`). The original wire instruction has a
16-byte header, so these names start at full instruction byte offsets
**+0x20** and **+0x18** respectively. The intervening fourth and second
words are still unknown and are **not** claimed to be fixed-size strings
or named lengths. The names remain variable-length byte sequences.

The source now describes the two observed packet layouts as
`EnemyEclInterruptInlineOperandsView` and
`EnemyEclTimeoutInlineOperandsView`, overlaid on the existing unallocated
variable-length `EnemyEclInstructionView` payload. Both local payload
offsets and both full instruction offsets have compile-time assertions.
The original extended dispatcher consumes their `callbackName` bytes at
the two native call/store sites; the ECL operand helper call order and
parameter values are unchanged. Original historical C++ type names, string
termination and unobserved operand roles remain unresolved.

## Bounded native source proof

A fresh pinned VC7.1 SP1 **11-source `/GL`** source graph gives complete
PDB-owned candidate extents, identical candidate field identities and
**zero ordinary candidate-to-candidate differences** relative to the
preceding source-pinned compile:

- `EnemyRuntimeView::DispatchEclInstruction`: **14,352** bytes, **716** fields
- `EnemyBeginSpell`: **2,432** bytes, **108** fields
- `EnemyFinalizeDeath`: **162** bytes, **9** fields
- `EnemyConfigureViewport`: **330** bytes, **10** fields
- `EnemyFullObjectView` constructor: **672** bytes, **5** fields

Selected independently original-target exact replays under the changed
header pass `enemy-configure-interrupt` **48/48** and
`enemy-read-int-argument` **16/16**, **64/64** bytes, with the existing
source interface retained. Default seven-stage repo CI and whitespace
checks pass. This is not a full historical cold replay.

**The original ECL dispatcher is 14,416 bytes, not 14,352.** This is a
wire/source reconstruction and native codegen regression proof, *not* a
newly exact large function or new credit. No runtime or product claim.

Private native witness:
`.analysis/gpt-web/20261105-next-enemy-callback-inline/verify.py`,
`build/gpt-web/20261105-next-enemy-callback-inline/current/native-result.json`,
`.analysis/gpt-web/20261105-next-enemy-callback-inline/selected-exact.json`.

Installed SHA-256: `Enemy.hpp` `12bfd2983becc5f94964b47e9cc1989d31acd2e265ffdfe0fc67857f84115729`, `EnemyEclDispatcher.cpp` `4eb0a4157a4066dd10492f0d91bad7426a51a0964243c5c728e132af04ec67ef`.
