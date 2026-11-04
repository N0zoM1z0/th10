# TH10 14KB ECL boss: reconstruct 28 variable-length operand reads

Original Japanese TH10 1.00a is independently attested by SHA-256
2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.
The complete EnemyRuntimeView::DispatchEclInstruction at 0x0040E770
owns **14,416 target bytes** and remains explicitly **NONEXACT**.

The enemy-spawn and player-targeting branches previously read variable
arguments by integer-cast arithmetic of the form
*(float/int *)(instructionAddress + 0x10 + variableCount*4 +
operandOffset*4). The retained EnemyEclInstructionView already has a
statically checked 16-byte header and a variable-length operands[1]
tail beginning at +0x10. The byte offsets, element width, decoded
operand mask and index arithmetic have separate target evidence in the
typed ECL operand helpers.

The real source now accesses **28 sites** through
EnemyEclInstructionView::operands[index], including the opcode's variable
argument start index and its additional 1..5 operands. Float arguments
explicitly view the corresponding 32-bit operand word as a float before
passing it to the original ReadRawFloatArgument; integer arguments use
the typed 32-bit operand directly. This preserves the original
interpretation of the operand bit patterns and the operation order. It
does not treat the packed ECL payload as a fixed-size array, add bounds
checks, move the enemy-spawn branches or invent register assignments.

## Whole-owner native result

A real VC7.1 SP1 /GL **11-source** Enemy runtime-update build physically
compiled the changed dispatcher. ANM and ECL support sources had changed
since older historical object pins, so those were **freshly compiled**;
unchanged support objects were reused only with source/object SHA checks.
The resulting complete PDB dispatcher contribution was **14,352 bytes**,
the same as immediately preceding candidate. All **716** linked address
field positions/types were identical, with zero other differing bytes after
normalizing the *same candidate address fields*. The current source SHA
and retained PE/MAP/PDB build path are in native-result.json.

This is a complete candidate-to-candidate codegen preservation test, NOT
a whole 14,416-byte original-target exact match. It adds **zero canonical
exact credit**, claims neither original C++ spelling nor original physical
TU attribution, and does not prove Windows product or runtime equivalence.
Only affected existing exact ECL helpers were selected for cold replay;
no full historical cold replay was executed.
