# TH10 full ECL boss: restore ANM VM position-offset and rotation storage

Original target: Japanese TH10 v1.00a SHA-256
2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.
Complete EnemyRuntimeView::DispatchEclInstruction at 0x0040E770
has 14,416 target bytes and remains NONEXACT.

The real ENEMY_ECL_PLAY_ANM, PLAY_ANM_HIGH and PLAY_ANM_ROTATED opcode
branches create a typed AnmVmView, then copy three dword coordinates
from the independently reviewed ECL world anchor or enemy world motion
into VM+0x340/+0x344/+0x348. These offsets are the actual AnmVmView
positionOffset.x/y/z, beginning immediately after the independently
asserted AnmVmView::position at +0x334 (12 bytes each).

All eighteen 32-bit coordinate assignments now use the genuine
AnmVmView::positionOffset fields with explicit unsigned-int bit-copy
lvalues. This preserves the original integer/IEEE-754 bit pattern,
including NaN/signed-zero encodings, instead of accidentally turning
the target stores into extra floating-point arithmetic or a re-ordered
structure copy. One rotation angle assignment at VM+0x2C also now
uses the real AnmVmView::rotation.z, consistent with the existing +0x24
rotation vector field and its independently asserted layout.

## Whole-owner native result

The pinned VC7.1 SP1 /GL real 11-source Enemy update graph was linked
with the edited complete ECL dispatcher. The currently changed ANM/ECL
support sources were recompiled, and unchanged objects were reused
only under SHA-256 source/object pins. The complete PDB-owned ECL body
is 14,352 bytes versus the 14,416-byte original, and all 716 linked
address fields retain exactly the previous types/offsets. No ordinary
byte differs from the preceding complete candidate after identical
candidate-field normalization. Source SHA and retained native artifact
are in native-result.json.

This proves unchanged candidate codegen after substantial native
owner-field recovery; it DOES NOT prove the original 14,416-byte
function exact. Zero new canonical or partial exact credit, no guessed
historical C++ spelling/TU, no product linking or runtime claims.

Scoped exact cold replays cover ANM CreateVmVariant2 (73 bytes), Enemy
ReadIntArgument (16 bytes) and the ECL world anchor (64 bytes):
153/153 exact bytes. No full historical replay was run.
