# ECL StartSubroutine: CALL scalar values belong to a variable-length word stream

Target original: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Original `EclVmContext::StartSubroutine @0x0044DF70` owns **550 bytes**;
the real current pinned VC7.1 SP1 ECL/Enemy **two-TU** PDB candidate is
**551 bytes with four candidate fields**. It remains **NONEXACT**.

CALL includes a serialized inline name followed by tagged 8-byte operands.
The `EclVmInstruction::scalarOperands[1]` source declaration is only a
first-word *overlay*, not an actual C++ one-element allocated array. The two
`StartSubroutine` float/int cases previously indexed it using an ECL-supplied
byte cursor, potentially invoking out-of-bounds C++ array arithmetic.
The current natural source uses the existing `OperandWord` helper, which
forms the original low-32-bit `instruction + offsetof(scalarOperands) +
index*4` byte address, and then loads the integer word. It retains the
saved CALL argument types, actual evaluation order and original unchecked
accesses. No bounds checks, invented allocations, ABI register forcing,
assembly bytes, or targets were patched. This is pinned Win32 i386 semantics,
not a portable malformed-input safety claim.

## Original-bound parent and candidate callee comparison

An independently rebuilt pinned current-source `/GL` native ECL/Enemy graph
was used for isolated CALL scalar-index controls: a direct operands byte
cursor, raw `int` pointer, the shared word helper in one branch and in
both branches, and direct word address forms. The raw integer form changed
**76 ordinary candidate** callee bytes despite preserving the parent `Run`.
The chosen helper version was installed and recompiled. Its complete
`StartSubroutine` remains 551 bytes, retains four candidate call fields at
unchanged code offsets, and has **zero** ordinary PDB-owner differences
against the current-epoch word-helper trial. The original is still 550
bytes; this is **candidate preservation**, NOT original acceptance.

The complete original `EclVmContext::Run @0x0044E1A0` was compared from
the same installed source against the **7,020 original bytes** and
**189 independently original-bound code/data fields**. It remains
**6,997/7,020** matching bytes, all 189 fields checked; its 171-byte
`ReadFloatValue` still compares exact. `StartSubroutine` remains nonexact.

The **nine directly selected** existing canonical ECL exact units cold-pass
**1,161/1,161** original-bound bytes. No full historical cold replay was
performed. Private reproducibility:
`.analysis/gpt-web/20261107-turn2/ecl-call-payload-trials.py`,
`ecl-call-installed-owner.json`, `ecl-call-installed-run.json`,
`ecl-call-installed-guards.json`, and actual PE/MAP/PDB under
`build/gpt-web/20261107-turn2-ecl-call/` plus the current canonical
`build/match-linked/EclVmHostRunEnemy/` graph. Original physical source
partition, product rebuild and runtime remain unproved.
