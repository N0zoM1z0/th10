# Enemy ECL SET_ANM_SCRIPT: native managed VM ID slot address words

Original target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The full `EnemyRuntimeView::DispatchEclInstruction` target at 0x0040E770
is **14,416 bytes**, while the pinned eleven-input current-source VC7.1 SP1
`/GL /GS` compiled PDB owner is **14,352 bytes** with **716 candidate
fields**. This large function is still **NONEXACT**.

`SET_ANM_SCRIPT` takes the VM slot index directly from a counted ECL
operand without a range clamp. Its five `managedVmIds[iVar26]` uses
release the previous ID, publish the newly created ID, read it (twice),
and resolve the ID to a VM. A source expression that indexes the bounded
C++ array may form an undefined pointer when the script operand exceeds
its declared length; the original i386 instead computes the four-byte
slot location as a flat low-32-bit address word. The installed source now
uses the existing typed `EnemyNativeWordIndex(&managedVmIds[0], iVar26)`
for all five separate uses. This preserves the C++ source's original call
order, argument reads, slot member and ABI rather than hoisting a mutable
pointer through opaque calls. It adds no clamping, storage or safety claim.

## Whole-owner native comparison, including rejected alternatives

The prior native producer pins were stale after several actual ANM/ECL
source changes. A **fresh** current-source eleven-TU compiler graph was
built by recompiling the three changed source objects and reusing only
source/header hash-matched support objects. Full PDB-owner extent and
field geometry were verified. Standalone `managed-script` and
`resource-bank` variants each preserved all five owners. Separate
`managed-main-selected`, `managed-all`, and the *combined*
`script-and-resource` forms changed one ordinary dispatcher byte despite
unchanged size and candidate field geometry; those combined/main source
variants were rejected. This is direct evidence that isolated codegen
neutrality does **not** imply combined neutrality.

The installed `managed-script` variant was recompiled and checked against
that fresh current-source baseline:

| Complete PDB candidate owner | Bytes | Candidate fields | Ordinary code differences |
| --- | ---: | ---: | ---: |
| `DispatchEclInstruction` | 14,352 | 716 | 0 |
| `EnemyBeginSpell` | 2,432 | 108 | 0 |
| `EnemyFinalizeDeath` | 162 | 9 | 0 |
| `EnemyConfigureViewportOwner` | 330 | 10 | 0 |
| Enemy full-object constructor | 672 | 5 | 0 |

All candidate field offset/type/width triples remain the same. The
716-field candidate inventory is **not** an independently original-bound
inventory, and this does not prove the still-short giant original exact.

Only the five affected accepted original-target Enemy ECL argument leaves
were cold replayed, each with its true call field; **87/87** original
bytes passed. No full historical cold replay or game runtime check.

Reproduction and source-specific receipts are ignored developer artifacts:
`.analysis/gpt-web/20261107-turn5/enemy-anm-id-words.py`,
`enemy-anm-id-results.json`, `enemy-native.py`,
`enemy-managed-script-guards.json` and linked PE/PDB comparison in
`build/gpt-web/20261107-turn5-enemy/installed-managed-script/`.
