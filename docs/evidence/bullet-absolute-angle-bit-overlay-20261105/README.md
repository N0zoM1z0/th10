# TH10 bullet transform: exact-width angle bit overlay with native unchanged

Original target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`BulletRuntimeView::UpdateAbsoluteDirectionChange` consumes one 32-bit
binary32 angle payload from `BulletExStateView +0x18`, but this particular
opcode copies its representation **without floating-point conversion**.
Other transform opcodes still interpret the same `value1` storage as a float.
The maintained record now overlays `float value1` with `int value1Bits` at
identical `+0x18`, with compile-time size/offset guards. In the absolute
transform's complete C++ body, the original `memcpy` to an `int` temporary
has become an ordinary typed `const int nextAngleBits = state.value1Bits`
read. Subsequent binary32 stores and the angle/vector path remain intact.
This is an overlay of existing storage, not a second field, a new allocation,
or a claim about the original identifier spelling.

An independent **seven-source** VC7.1 SP1 `/GL /GS` native link of the
**current** BulletManager, BulletRuntime, BulletTransform, PlayerCollision,
AnmManager, EnemyEclDispatcher and Enemy established a fresh *before*
source/header epoch. All seven were freshly recompiled from the modified
source for the *after* graph, rather than reusing the previous month's
support objects. Complete PDB function contributions compared in identical
compiler contexts:

- `UpdateAbsoluteDirectionChange`: **335/335 candidate bytes**, 8
  identically positioned linkage fields; **zero ordinary candidate changes**.
  Original target: 335 bytes, exactly **two remaining ordinary mismatches**
  at `+0xD7`, `+0xDD` (EDX versus EAX angle forwarding). **NONEXACT**.
- `UpdateHorizontalWrap`: complete 148 bytes, 8 field positions identical;
  no ordinary candidate changes.
- `UpdateDeceleration`: complete 164 bytes, 7 field positions identical;
  no ordinary candidate changes.

Selected canonical original-target cold replay on the modified source passed
`bullet-update-horizontal-wrap` **148/148** and
`bullet-update-deceleration` **164/164**, preserving **312/312 previously
accepted exact bytes**. There is **zero new exact credit**. The original
335-byte function has not been promoted from similarity to exactness;
address bindings and remaining local destinations beyond this bounded
comparison remain explicit work.

Pinned inputs, the complete candidate-field comparison and selected replay
results are retained in `native-result.json`; private native PE/MAP/PDB
witnesses are ignored under `build/gpt-web/20261105-continuation-bullet/`.
No all-unit cold replay, complete executable product or runtime claim.
