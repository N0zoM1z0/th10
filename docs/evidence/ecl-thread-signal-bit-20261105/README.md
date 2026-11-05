# TH10 ECL thread signal flag: native bit zero in full Run interpreter

Original Japanese TH10 v1.00a SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

In the complete original `EclVmContext::Run @0x0044E1A0`, the
`SET_THREAD_FLAG` and `CLEAR_THREAD_FLAG` opcodes independently load
an active thread context and respectively execute `OR DWORD PTR
[...+0x1020],1` and `AND DWORD PTR [...+0x1020],0xFFFFFFFE`.
Only **bit zero** of the +0x1020 context flags word is affected. The
maintained `EclVmContext` now overlays its existing `unsigned int flags`
with a `threadFlag0:1` bitfield and an opaque 31-bit remainder; the
existing 0x1020 offset and 0x1024 context extent assertions remain.
The opcode bodies write `thread->context->threadFlag0 = 1/0` instead
of repeating the opaque masks. No new state, checks, or original-source
naming assumption is introduced.

Real pinned VC7.1 SP1 `/GL` EclVm.cpp + Enemy.cpp compile, including
changed public header, produces the **complete 7,020-byte** ECL Run
PDB function with **all 189** independently target-bound linked fields,
including local destinations and trailing opcode data. The full
strict original-target comparison remains **6,997/7,020** with the
same 23 ordinary discrepancies; **NONEXACT**. The entire candidates
for `Run` (7,020/189 fields), `StartSubroutine` (551/4) and
`FindSubroutine` (136/0) are identical to the preceding source
compiler epoch after normalization of their verified fields. The
`StartSubroutine` target remains 550 bytes and is also NONEXACT.

Five explicitly selected original-bound cold replays from the changed
source passed: `ecl-vm-host-run` 146, `ecl-vm-find-thread` 36,
`ecl-vm-read-int` 144, `ecl-vm-stack-push` 96, and
`ecl-vm-stack-pop` 122 — **544/544 previously accepted exact bytes**.
This grants **zero new exact credit**. The source/header hashes,
full-owner comparison and selected results are in `native-result.json`;
ignored PE/MAP/PDB are under
`build/gpt-web/20261105-ecl-thread-flag-bit/full-run/`.
No all-unit cold replay or full-game executable/runtime claim.
