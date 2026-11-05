# TH10 ECL StartSubroutine: typed CALL name byte origin

Pinned Japanese TH10 v1.00a target SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The CALL opcode's subroutine name is stored after its leading operand word:
the instruction header establishes byte operands at `+0x10`, with a native
32-bit leading operand, so the name begins at instruction `+0x14`.
`EclVmContext::StartSubroutine` formerly added a raw `0x14` to the entire
instruction pointer. It now uses the independently checked `operands` byte
stream and `sizeof(int)` to derive the same name pointer while retaining the
existing host/context lifetime and lookup call ordering. No null check,
relocation or guessed original variable-length header is introduced.

The actual pinned VC7.1 SP1 `/GL` two-TU `EclVm.cpp` + `Enemy.cpp` context
was recompiled from the modified source, and compared against the immediately
preceding source epoch. The **complete** `StartSubroutine` PDB contribution
remains 551 candidate bytes, with four unchanged linked fields and zero
ordinary candidate byte differences. Its original target is **550** bytes;
this helper remains **NONEXACT**. The full 7,020-byte `EclVmContext::Run`
interpreter retains 189 target-bound fields and a strict original-bound score
of **6,997/7,020** (23 ordinary byte differences), unchanged. The separately
exact `FindSubroutine` leaf remains 136 bytes with identical candidate code.

Four selected original-target canonical cold replays from the installed source
passed: `ecl-vm-host-run` 146, `ecl-vm-find-subroutine` 136,
`ecl-vm-stack-push` 96, `ecl-vm-stack-pop` 122 — **500/500 previously
accepted bytes**, with **zero new exact credit**. This is a source
representation and native codegen-preservation checkpoint, not original
source-name identity, complete game linkage or runtime equivalence.

Machine-readable source/header pins, full owner comparison and exact guards
are in `native-result.json`; ignored PE/MAP/PDB under
`build/gpt-web/20261105-ecl-call-name/full-run/`. No historical all-unit
cold replay was run.
