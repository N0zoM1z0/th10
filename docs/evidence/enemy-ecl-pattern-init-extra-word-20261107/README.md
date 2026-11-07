# Enemy ECL opcodes 0x190 and 0x199: native pattern index words

Target: original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The full target `EnemyRuntimeView::DispatchEclInstruction @0x0040E770`
remains **14,416 bytes**, vs **14,352 candidate bytes**, still **NONEXACT**.
This work fixes two independent instruction families within the full
large owner, not a small synthetic test or isolated decompiler fragment.

## Independently checked retail instructions and natural C++

For opcode **0x190, INITIALIZE_BULLET_PATTERN**, the target at
`0x00410139` contains `IMUL EDX, EDX, 0x210`, followed by
`ADD EDX, EBX` at `0x0041013F`, then `LEA EDI, [EDX+0x2C4]` and
`REP STOSD` (0x84 dwords). This shows a low-32-bit pattern-record
stride and runtime base addition, not signed C++ multiplication without
overflow. The maintained initial pattern cursor is now explicitly
`unsigned index * sizeof(EnemyBulletPatternView) + unsigned runtime word`,
then cast back to the original signed word for the existing storage helper.
The original zeroing loop and typed record layout remain unchanged.

For opcode **0x199, SET_BULLET_EXTRA**, the retail instructions at
`0x00410BAA` calculate the pattern's **22 overlapping extra records**
with `IMUL ESI, ESI, 0x16`, then `LEA EDI, [ESI+EAX]`,
`LEA EAX, [EDI+EDI*2]`, and `LEA ESI, [EBX+EAX*8]`.
The current C++ preserves this separate pattern/extra-slot/index schedule,
with unsigned low-32-bit multiplication and addition for the 22-slot cursor,
its 24-byte scaling and runtime base, and the final `floatB` store's
original eight-byte-unit address expression. This final destination
is the +4 float member of the **same** 24-byte extra slot, not an
invented second array. There is no new guard, allocation, hardcoded
absolute address, synthetic ABI, or target instruction patch. The existing
out-of-range script-index behavior remains unguarded; this is not a
memory-safety claim.

## Current 11-TU native tests, with source-pinned support reuse

Since `EclVm.cpp` and `EnemyEclDispatcher.cpp` both changed in this
conversation, the old 11-TU native artifact **was not reused as a current
baseline**. We built a new current baseline from the complete 11-source
native producer, recompiling the two changed source objects and using only
source- and header-hash-attested objects for the unchanged sources. The
factory's installed VC7.1 SP1 linker then generated actual PE/PDB images.

Five explicit *real* compiler variants were tested on that new baseline:
0x190 alone, 0x199 pattern/index alone, 0x199 final floatB alone,
0x190 + 0x199 indices together, and all three site groups together.
The final installed source was compiled **again** independently. In every
trial, the complete 14,352-byte dispatcher and four adjacent large/leaf
owners had **zero ordinary code differences** from the new baseline and
identical full PDB extents and candidate field offsets/types:

| Complete owner | Candidate bytes | Candidate fields | Baseline ordinary differences |
| --- | ---: | ---: | ---: |
| DispatchEclInstruction | 14,352 | 716 | **0** |
| EnemyBeginSpell | 2,432 | 108 | **0** |
| EnemyFinalizeDeath | 162 | 9 | **0** |
| EnemyConfigureViewportOwner | 330 | 10 | **0** |
| EnemyFullObjectView constructor | 672 | 5 | **0** |

These numbers are **candidate-vs-current-source-candidate preservation**,
not original-target fields or newly exact bytes. A separate installed-
source selected original-bound cold replay of the five ECL argument
owners passed **87/87** target bytes (`enemy-read-int-argument`,
`enemy-read-float-argument`, `enemy-resolve-float-argument`,
`enemy-read-raw-int-argument`, `enemy-read-raw-float-argument`).
No new canonical exact credit is awarded. This also is not a product
runtime test or a full historical cold replay.

Private witnesses: `.analysis/gpt-web/20261107-session-th10/
enemy-native-v2.py`, `enemy-pattern-word-probe.py`,
`enemy-installed-proof.log`, `enemy-pattern-final-exact-guards.json`;
actual current eleven-TU PE/PDB artifacts under
`build/gpt-web/20261107-session-th10-enemy-v2/{base,installed-pattern-words}`.
