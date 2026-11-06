# EnemyBeginSpell: reproduce native modulo-2^32 bonus arithmetic before signed cap

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The complete original `EnemyBeginSpell @0x00409280` is 2,432 bytes.
At original owner relative `+0x5F4..0x61C`, it computes the spell starting
bonus using native x86 DWORD arithmetic: `lea eax,[eax+eax*2+0xA]`
from stage, `imul eax,[spellBonusBase]`, then `lea eax,[eax+eax*4]` and
`shl eax,1`. This is **32-bit arithmetic modulo 2^32** even on overflow.
The resulting raw word is published to `currentSpellBonus` and
`initialSpellBonus`; the subsequent `cmp eax,0x05F5E100` / `jl` is
**signed**, and only `initialSpellBonus` is capped to `99,999,999`.

Prior C++ directly multiplied three signed `int` values, making signed
arithmetic overflow undefined, even though the retail machine code does
wrap. The maintained source now makes the actual 32-bit wrap explicit:
`stage` and `spellBonusBase` are converted to `unsigned int`, multiplied
and added by unsigned literal factors, and the final 32-bit bits are
reinterpreted through the pinned VC7.1 32-bit `int` conversion for the
original signed cap. The stage, bonus fields and cap semantics, including
negative/wrapped values, remain the same on the target compiler. No
artificial padding, forced register, branch or original C++ identifier is
invented. This is a semantic improvement to source fidelity, not a new
external feature or a game-runtime proof.

## Fresh full-owner native regression and scoped original-bound tests

A genuine pinned VC7.1 SP1 **11-translation-unit `/GL`** current-source
link recompiled the edited `EnemyEclDispatcher.cpp` and all relevant
source/header inputs. Whole PDB candidate owner comparison against the
preceding source graph shows **zero ordinary byte and field-layout
regressions** in all five complete owners:

- `EnemyBeginSpell`: **2,432** candidate bytes, **108** native fields;
- Enemy ECL dispatcher: **14,352** candidate bytes and **716** fields;
- death finalizer 162/9, viewport 330/10 and Enemy full ctor 672/5.

The target ECL dispatcher is **14,416** bytes; BeginSpell has substantial
remaining ordinary codegen differences despite matching its target size.
Neither gains exact credit. The selected canonical original-bound cold
replay of two leaf functions compiled with the *changed Enemy source*,
`enemy-read-int-argument` **16/16** and `enemy-read-raw-int-argument`
**17/17**, passed **33/33** strict bytes. This is only the directly
involved 9-TU operand source graph, **not** an all-unit cold replay.
The separate 20-TU `enemy-end-spell` exploratory selected replay exceeded
the tool execution window and did not produce a usable result, so it is
**not** reported as passed. Seven lightweight CI gates and whitespace checks
passed. No original author identity, whole-game product, runtime
semantics, or portability assertion follows.

Private reproduction and receipts:
`.analysis/gpt-web/20261106-beginspell-bonus-wrap/verify.py`,
`build/gpt-web/20261106-beginspell-bonus-wrap/current/native-result.json`,
`.analysis/gpt-web/20261106-beginspell-bonus-wrap/selected-operand.json`.

Current `src/EnemyEclDispatcher.cpp` SHA-256: `496848214e0a40e21511c3ef1d9d5c3a1b7cc76bb1b61c2fecfcbf68a0a2a2d1`.
