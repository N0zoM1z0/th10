# Enemy ECL START_SPELL instruction's encrypted inline payload

Target: original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The five related START_SPELL opcodes (including the difficulty-adjusted
forms) share a variable-length instruction payload beginning at instruction
+0x10. The first three encoded dwords are operands for the generic ECL VM;
the fourth dword at full instruction **+0x1C** is the byte length of the
encrypted name, and encrypted name bytes begin immediately at **+0x20**.
Previously `EnemyRuntimeView::DispatchEclInstruction` reconstructed those
positions with independent `operands[3]`/`&operands[4]` casts. The maintained
`EnemyEclSpellInlineOperandsView` overlays the same instruction storage,
with both payload-relative and full-instruction `offsetof` checks. The
entire START_SPELL opcode family now uses `spellOperands.encryptedName`
and `spellOperands.encryptedNameLength`. This is a variable-length wire
prefix, NOT an allocated fixed-size string; no hidden terminator,
additional bounds check, ABI declaration or original source identifier is
invented. The original XOR/decryption loop and all five dispatch cases are
preserved.

## Actual current source native verification

Genuine pinned VC7.1 SP1 **11-source** `/GL` Enemy ECL native link with
this source/header compiled fresh. Whole PDB owner comparisons with the
prior pinned source graph found no changed ordinary candidate bytes and
identical field position/types for:

- ECL dispatcher: **14,352** candidate bytes, **716** candidate link fields
- BeginSpell: **2,432** bytes, **108** fields
- Death finalizer: **162** bytes, **9** fields
- Viewport: **330** bytes, **10** fields
- Full Enemy constructor: **672** bytes, **5** fields

Two explicitly selected original-target exact leaf replays under the
changed header, `enemy-read-int-argument` **16/16** and
`enemy-read-raw-float-argument` **26/26**, passed **42/42** bytes. The seven
lightweight CI stages and `git diff --check` pass. No complete historical
cold replay was performed.

The target ECL dispatcher is **14,416** bytes, so this 14,352-byte native
candidate remains **NONEXACT** with no new exact ledger credit. Candidate
regression comparisons do not assert original target equivalence. Private
reproduction:
`.analysis/gpt-web/20261105-next-enemy-spell/verify.py`,
`build/gpt-web/20261105-next-enemy-spell/current/native-result.json`,
`.analysis/gpt-web/20261105-next-enemy-spell/selected-exact.json`.

Maintained source SHA-256 pins: `Enemy.hpp` `639dba7ef740b3c5cdd6f045f0d3399af40cff048d607c371833c9f54645aed1` and
`EnemyEclDispatcher.cpp` `a791010b46e0e0a590013ca4c0a3b780956a651f476ecc982dc90978458800c2`.
