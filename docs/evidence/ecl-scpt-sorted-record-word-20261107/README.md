# TH10 ECL AddScriptData native sorted 8-byte record addresses

Original Japanese TH10 v1.00a SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
`EclVmScriptDatabase::AddScriptData @0x00450220` is an original complete
**589-byte** function. Its real current two-TU ECL/Enemy VC7.1 `/GL /GS`
owner is also 589 bytes, with the target CRT call fields at `+0x6C` and
`+0x101`; it is still **NONEXACT**, **502/581** comparable original
ordinary bytes and **79** differences.

The SCPT sorted subroutine map consists of eight-byte entries (name and
stream-header pointers). Unchecked script counts/insertion indices are
used to form slot addresses; the native i386 arithmetic is a modulo-32-bit
base+index*8 word, not a bounded C++ array pointer calculation. The
installed natural C++ uses a typed `EclMutableEntryAtWord` helper to form
initial record header/name writes and sorted insertion header/name writes,
without moving those writes across any `strlen`, `strcmp`, `free` or
argument parsing. It also advances the loop's existing `entry` pointer
through a Win32 low-32-bit stride, preserving the original comparison
order. No array clamp, new allocation, false ABI or original-byte patch.
This is native address fidelity, not evidence that corrupt packets are safe.

## Recompiled full-owner experiments and explicit rejection

Source-pinned native two-TU linked PE/PDB trials were compiled for
`initial-record`, `insert-record`, `cursor-word`, `move-record`,
`all-records`, and `initial-insert-cursor`. Each trial compared its entire
`AddScriptData`, complete **7,020-byte** `EclVmContext::Run`, and
`FindSubroutine` (already exact 136-byte leaf). The first three isolated
trials and the installed three-part combination retained 589 bytes,
**502/581** original ordinary bytes, the two CRT field positions, and
full `Run` **6,997/7,020**, all **189 independently original-bound
fields**, and exact `ReadFloatValue`.

Crucially, rewriting `subroutines[index] = subroutines[index - 1]`
changed 63 additional bytes (**439/581** remaining original matches),
despite unchanged length and two call fields. This and its all-records
combination were **rejected**; they were not merged or treated as success.

A separate native cold build from the **installed** source independently
confirmed complete AddScriptData **589**, target bytes **502/581**, two
CRT calls at the original offsets, and parent `Run` **6,997/7,020**, all
189 original fields. Four selected canonical ECL owners were cold
replayed against the original target only in their affected compiler
graphs: `ecl-vm-find-subroutine` (136), `ecl-vm-read-float-value` (171),
`ecl-vm-host-run` (146) and `ecl-vm-stack-pop` (122): **575/575 exact**.
No full historical cold replay, game build, runtime, source-owner claim or
new canonical exact codegen credit.

Private source-pinned evidence: `.analysis/gpt-web/20261107-turn5/
ecl-subrecord-sorting-probe.py`, `ecl-subrecord-results.json`,
`ecl-record-installed-owners.json`, `ecl-record-installed-run.json`,
`ecl-record-guards.json`; native PE/MAP/PDB under
`build/gpt-web/20261107-turn5-ecl-record/` and canonical linked
`build/match-linked/EclVmHostRunEnemy/`.
