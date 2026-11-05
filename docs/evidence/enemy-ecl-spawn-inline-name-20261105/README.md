# TH10 Enemy ECL: variable-length CREATE_ENEMY inline name and scalar-tail packet

Target identity: original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The complete 14,416-byte Enemy ECL dispatcher contains several related
CREATE_ENEMY opcode paths: relative, absolute, globally offset, mirrored,
and absolute mirrored, including no-boss variants. The encoded instruction
has a shared 16-byte header. At its operand payload +0, a 32-bit count
controls the inline subroutine-name byte span; the name bytes begin at
payload +4 (full instruction **+0x14**). The subsequent numeric arguments
are loaded at word indexes derived by the native `(nameByteCount+4)/4`
expression, NOT from a fixed C++ record size. Some modes load five scalar
arguments and the global-offset mode six, so the dynamic tail must remain
separate from the inline name prefix. The current code now models this
with `EnemyEclSpawnInlineOperandsView` (count + variable-name prefix)
overlaid on the existing `EnemyEclInstructionView` payload; compile-time
assertions fix both packet-relative and full-instruction positions.

Five dynamic-tail index-expression source paths and three shared direct
`EnemySpawn` name argument expressions now use the typed overlay.
Raw integer/float argument reads, pre-existing player projection,
no-boss filtering, runtime flags and spawn dispatch are unchanged. The
variable-sized byte buffer is not a heap-allocated fixed-size name,
and no guessed terminator, padding, original C++ identifier or bounds
check has been added.

## Fresh native codegen and original-bound guards

A genuine pinned VC7.1 SP1 **11-source `/GL`** rebuild with all changed
source and headers gives the *complete* current-source PDB Enemy ECL owner
at **14,352 bytes** (original **14,416**), with **716** unchanged candidate
field offsets/types and zero differing candidate ordinary bytes versus
the immediately preceding source epoch. Four related complete owners
remain unchanged: BeginSpell **2,432/108 fields**, death finalizer
**162/9**, viewport **330/10**, full Enemy constructor **672/5**.
This is a complete candidate regression check, **not original-byte
acceptance** for the huge dispatcher; it remains **NONEXACT** with no new
exact codegen bytes credited.

Three directly affected original-target exact operand readers were
**selectively** cold recompiled/replayed under the changed source:
`enemy-read-raw-int-argument` **17/17**,
`enemy-read-raw-float-argument` **26/26**, and `enemy-read-int-argument`
**16/16**, totaling **59/59 strict exact**. The seven-stage repository
CI and `git diff --check` pass. No historical full cohort was replayed;
this does not prove a complete game product or runtime equivalence.

Private native reproducibility:
`.analysis/gpt-web/20261105-th10-enemy-spawn-payload/verify.py`,
`build/gpt-web/20261105-th10-enemy-spawn-payload/current/native-result.json`,
`.analysis/gpt-web/20261105-th10-enemy-spawn-payload/selected.json`.

Current source SHA-256 pins: `Enemy.hpp` `7b3f3bf9465e1a54cfafb5970cd438fe6f9efd004a4de26a3aae6d4535f65324`, `EnemyEclDispatcher.cpp` `227fba94cccbd9d0e623bf82efafa002acd30e824d6fe418dbcfd0482eec3b44`.
