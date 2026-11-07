# Enemy ECL difficulty bullet-speed stores: native 32-bit address words

Target is original Japanese TH10 v1.00a; SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The entire original `EnemyRuntimeView::DispatchEclInstruction` at
`0x0040E770` is **14,416 bytes**. The current VC7.1 SP1 eleven-input
`/GL /GS` native candidate is **14,352 bytes**, still **NONEXACT**.

The `ENEMY_ECL_SET_BULLET_SPEED_BY_DIFFICULTY` opcode resolves a script
index and retains its 0x210-byte pattern stride as a signed 32-bit word.
The original i386 address formation adds the runtime base to that stride
modulo 2^32 before writing the selected `speedA` and `speedB` fields.
Maintained source previously used signed C++ `iVar26 + (int)runtimeAddress`
at **both** publication sites, invoking undefined signed overflow for
script-provided extreme indices. Both sites now explicitly use unsigned
32-bit intermediate addition and cast the low word back to the existing
native pattern-pointer overlay. The established field types, difficulty
selection, separate float reads, and write ordering remain unchanged.
No new bounds check, extra data, target-byte patch, or fake ABI is present.

A fresh **current-source eleven-TU baseline** was rebuilt, rather than
reusing the earlier dispatcher object; all unchanged source and header
pins are checked by the local native harness. A separate compiled control
changed both address sites, then the actual *installed* source was compiled
again with the same ten other real current-source objects. Complete linked
PE/PDB owner comparisons show:

| Candidate owner | Bytes | Candidate fields | Ordinary differences vs current baseline |
| --- | ---: | ---: | ---: |
| `DispatchEclInstruction` | 14,352 | 716 | 0 |
| `EnemyBeginSpell` | 2,432 | 108 | 0 |
| `EnemyFinalizeDeath` | 162 | 9 | 0 |
| `EnemyConfigureViewportOwner` | 330 | 10 | 0 |
| Enemy full-object constructor | 672 | 5 | 0 |

The field counts are **candidate geometry only**, not separately
original-bound target obligations. The dispatcher still differs in size
from its original target; this source correction adds **zero** new exact
functions or original codegen bytes.

A focused **installed-source cold replay**, not the historical all-unit
replay, checks five previously exact original-target ECL argument leaves:
ReadIntArgument16, ReadFloatArgument14, ResolveFloatArgument14,
ReadRawIntArgument17 and ReadRawFloatArgument26. **87/87** exact original
bytes and their existing declared external fields pass.

Private raw evidence: `.analysis/gpt-web/20261107-turn3/enemy-speed-word-trials.py`,
`enemy-speed-word-results.json`, `enemy-speed-installed-native.log`,
`enemy-speed-guards.json` and current link artifacts at
`build/gpt-web/20261107-session-th10-enemy-v2/installed-speed-words/`.
No full product build, runtime proof, or physical original TU attribution.
