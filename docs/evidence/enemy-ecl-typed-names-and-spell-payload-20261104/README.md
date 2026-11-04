# Full TH10 ECL boss: typed names, callbacks and encrypted Spell payloads

Original Japanese TH10 v1.00a target SHA-256:
2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.
The full 14,416-byte EnemyRuntimeView::DispatchEclInstruction at
0x0040E770 remains NONEXACT. This batch edits that actual C++ function.

The native instruction record already has a separately checked
EnemyEclInstructionView with a 0x10-byte header and a variable-length
32-bit operand payload starting at +0x10. The enemy-spawn, callback and
spell command branches previously treated instruction+0x14/+0x18/
+0x1C/+0x20 as four independent anonymous integer/byte/string pointers.
These all refer to offsets within that same original instruction operand
payload. The current C++ now expresses all seven of these fields
through the checked instruction->operands array:

- The three enemy-spawn subcases use the inline script/name bytes at
  &operands[1] (+0x14).
- The callback argument is read as the inline byte payload at
  &operands[2] (+0x18).
- The spell decryption's encrypted bytes begin at &operands[4] (+0x20),
  with its original signed byte count in operands[3] (+0x1C).

The original decryption XOR step, byte-count check, difficulty offset
selection, and operand evaluation order remain unchanged. No artificial
string termination, endian conversion, bounds check, shadow structure,
register annotation, or target code-byte patch is added. These words
are variably sized instruction payload bytes, not a fixed-length string.

## Current actual scoped native proof

Pinned VC7.1 SP1 /GL genuine 11-source Enemy runtime-update graph physically
compiled the edited source and linked it with the real required inputs.
ANM and ECL support sources were recompiled when their current source pins
differed from the earlier object epoch; unchanged object reuse required
SHA-256 source/object checks. The complete ECL PDB contribution remained
14,352 bytes with all 716 linked address fields at identical positions and
with identical types. There are ZERO ordinary byte differences versus the
preceding complete candidate after normalizing the same linked fields.
The source SHA-256 and retained PE/MAP/PDB location appear in native-result.json.

This is candidate-to-candidate proof that recovered source ownership did
not regress compiler codegen, not an original-target 14,416-byte match.
The boss remains NONEXACT with zero new canonical codegen credit.
Selected affected exact operand and callback helpers were cold-replayed.
No full historical cold replay, original source-file identification,
game-product runtime claim, or portability claim.
