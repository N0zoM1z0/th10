# TH10 ECL 14KB boss: reconstruct the 24-byte bullet-extra field record

Original target: Japanese TH10 v1.00a, SHA-256
2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

The complete EnemyRuntimeView::DispatchEclInstruction at target 0x0040E770
is 14,416 bytes and is still NONEXACT. This changes real source in the
ENEMY_ECL_SET_BULLET_EXTRA case, not an unrelated small helper.

The original opcode uses a linear index (patternIndex*0x16 + extraIndex)
and a record stride of 0x18. It writes six 32-bit values at offsets
+0x2E4, +0x2E8, +0x2EC, +0x2F0, +0x2F4 and +0x2F8 from the EnemyRuntime
base. Five original integer/floating writes are now expressed through a
single private EnemyBulletExtraView with a compiler-checked 0x18-byte size
and exact checked offsets: floatA at +0x00, floatB at +0x04,
value08, value0C, value10 and value14. The names describe observed fields,
not asserted historical identifiers. The source retains the original byte
stride, argument evaluation order and all original value conversions.

The sixth assignment, floatB, *remains* the original indexed destination
rather than forcing a new pointer lifetime or changing the selector math.
This is a deliberately verified compiler scheduling boundary, not a claim
that the underlying field structure is unknown.

## Actual native results / controlled failure

Two actual pinned VC7.1 SP1 /GL 11-source Enemy-update candidate PE/MAP/PDB
images were generated with the altered source epoch, checked against the
preceding native image. Changed primary source recompiled, and cached
unchanged other sources require their SHA-256 source/object pins.

- **Six** typed field assignments: full PDB owner 14,348 bytes, 716 fields,
  but address-field geometry *changed*. This trial was NOT adopted.
- **Five** typed field assignments and original final indexed float store:
  full PDB owner **14,348 bytes**, all **716 address fields** at the same
  locations and types, **zero ordinary bytes different** from previous
  candidate after identical candidate-field normalization. This is retained.

See retained-native.json and rejected-native.json for the observed size,
geometry and exact source hashes. The current code's exact source SHA is
in retained-native.json; retained PE/MAP/PDB under
build/gpt-web/20261104-live-enemy/native-bullet-extra-five-20261104/.

This work does not close the 14,416-byte original target; it produces no
new exact unit, partial-byte credit, fabricated runtime checks, assembler
code, forced registers or product-build claim. The only acceptance here is
newly recovered source structure without regressing the current compiled
candidate, independently of prior notes. No full historical cold replay.
