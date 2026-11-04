# TH10 14,416-byte Enemy ECL dispatcher: recover ANM geometry and flag fields

Original Japanese TH10 v1.00a executable SHA-256:
2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.
The original EnemyRuntimeView::DispatchEclInstruction at 0x0040E770
has a complete 14,416-byte target and remains NONEXACT.

This checkpoint directly changes the maintained 14KB opcode interpreter
and restores nine genuine AnmVmView member accesses. The SET_MAIN_ANM_SCRIPT
and shared primary ANM bounds update path previously extracted four
opaque float words from VM+0x3C/+0x40/+0x4C/+0x50. The real AnmVmView
layout already provides scaleX, scaleY, spriteWidth and spriteHeight at
those exact offsets. The ECL visibility bounds now multiply the same
fields, preserving the observed **height*scaleY** and **width*scaleX**
evaluation order; no altered units, fabricated absolute bounds or
implicit aspect-ratio correction is added.

The PLAY_ANM_ROTATED path also writes bit 2 at VM+0x35C. The real
AnmVmView exposes flags35C with a separately compiler-checked offset.
The source now updates that field directly, retaining the original
one-bit OR operation and original 32-bit width.

Actual scoped native codegen: a pinned VC7.1 SP1 /GL 11-source Enemy
runtime-update graph compiles the changed dispatcher and any
epoch-changed ANM/ECL support sources. Unchanged support object reuse
requires source+object SHA-256 verification. The full linked-PDB
dispatcher contribution stays **14,352 bytes** with exactly **716**
linked address fields in identical offsets and types. Candidate
ordinary bytes are all identical to the preceding generated image
after matching candidate field normalization. The current source SHA
and actual native artifact are retained in native-result.json.

Because the *original target* has 14,416 bytes, this is real source
recovery and a codegen-preservation test, **not exact target codegen**,
not a newly credited function, and not a reconstructed runnable game.
Only relevant ECL/ANM exact helpers were selected for regression:
AnmLoadedView::CreateVmVariant2 73/73, ECL world anchor 64/64, and
enemy runtime constructor 145/145: **282/282 exact**.
No full cold replay, target-byte mutation, historical TU guess
or runtime/portability claims.
