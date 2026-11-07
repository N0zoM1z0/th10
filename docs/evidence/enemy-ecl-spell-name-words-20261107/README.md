# Enemy ECL spell-name decoding: native 32-bit indexed byte addresses

Pinned target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`EnemyRuntimeView::DispatchEclInstruction` handles five spell-start opcode
variants by decoding a counted encrypted name into `local_90.bytes`. The game
reads the input length directly from ECL and does not clamp the copy to the
local buffer. The original i386 loop uses a native 32-bit address calculation
for both the name source and its destination. Both prior C++ expressions,
`encryptedName[iVar17]` and `local_90.bytes[iVar17]`, instead formed
potentially out-of-array C++ references. The installed source uses the
existing typed `EnemyNativeWordIndex` on the first byte of each byte stream,
with the original signed counter and bytewise XOR/key progression unchanged.
This is faithful unchecked native address semantics, not a buffer-overflow
repair, a portability claim, or an assertion of valid corrupt ECL input.

## Real complete-function comparison, not a summary-byte approximation

Five natural VC7.1 SP1 `/GL` 11-TU source variants (read-word, write-word,
both-word, destination pointer, scalar-index local) were compiled separately
and fully linked. The selected **both-word** version preserves all five
complete current-source PDB contributions and their original candidate field
geometry:

| Entire source function | Candidate bytes | Candidate linked fields | Difference from fresh current-source baseline |
| --- | ---: | ---: | ---: |
| Enemy ECL dispatcher | 14,352 | 716 | 0 ordinary bytes |
| EnemyBeginSpell | 2,432 | 108 | 0 |
| EnemyFinalizeDeath | 162 | 9 | 0 |
| EnemyConfigureViewportOwner | 330 | 10 | 0 |
| EnemyFullObjectView constructor | 672 | 5 | 0 |

The **target** dispatcher is still 14,416 bytes, not 14,352. This
comparison is candidate-vs-candidate and makes **no new exact-owner claim**;
the 716 candidate fields are not independently original-bound.

Three further full native builds combined the name-word source with the
previously open `SET_MAIN_ANM_SCRIPT` / `PLAY_SELECTED_ANM` managed-VM-ID
index variants. Each changed the same spell decode SIB encoding at relative
`+0x271F` (`8A 0C 31` to `8A 0C 0E`) despite otherwise unchanged size
and candidate fields. All combined variants were **rejected**, preserving
this observed original opcode form; read/write name-word source alone was
accepted. The original encrypted-name loop itself has the corresponding
`8A 0C 31` at target offset `+0x273B`; absolute local offsets differ
because the large candidate is nonexact.

Five selected previously exact Enemy ECL argument leaves were cold replayed
against original target code and field bindings from the installed source:
**16 + 14 + 14 + 17 + 26 = 87/87** complete bytes. Seven-stage lightweight
CI and `git diff --check` also passed. No full cohort replay or whole-game
runtime check was attempted.

Ignored trial scripts/results: `.analysis/gpt-web/20261107-active/`
(`enemy-spell-name-words.py`, `enemy-spell-name-results.json`,
`enemy-name-vm-combo.py`, `enemy-name-vm-combo-results.json`,
`enemy-spell-name-exact.json`). Linked artifacts are in
`build/gpt-web/20261107-active-enemy-graph/`.
