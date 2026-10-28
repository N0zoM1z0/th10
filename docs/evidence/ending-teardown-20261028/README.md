# Ending text destruction exact, 2026-10-28

**192 authored bytes / one complete owner** at `0x40B3A0..0x40B45F` are
accepted by `ending-text-destructor`. Two independent final-source18-TU
`/GL /GS /EHs` cold links pass every byte and all four fields. Each also retains
eleven existing exact contracts,1276 bytes/all43 fields, including the648-byte
constructor, real101-byte worker Stop/16-byte destructor and its56-byte Start
caller. No object reuse
occurs in either acceptance build; directed controls reuse unchanged objects.

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

## Target and compiler evidence

Target destruction calls worker Stop, releases five text IDs, clears each ID
once at the loop tail, then performs worker member destruction and its second
Stop. It has an EAX receiver/plain RET and no EH registration. The already exact
constructor separately proves that the worker really has member cleanup.

The source release helper previously reused a lookup that cleared missing IDs
before that common tail. Calling the actual FindVm directly removes the duplicate
clear/control flow: complete native size253 to232, still nonexact.

Stop's complete target body contains only WaitForSingleObject, Sleep and
CloseHandle calls, with no C++ callback/game call. The target caller has no
exceptional member-cleanup edge around this role. A standard `throw()` contract
on the actual declaration/definition expresses that nonthrowing role. With
unchanged compiler flags it removes the extra unwind frame, recovers the genuine
EAX receiver and reproduces all192 bytes. Original declaration spelling and
physical TU remain unknown; this is a reproduced source contract, not proof of
that exact original spelling. No register hint, forced return or copied bytes
is used. Changing Worker alone, then Worker and Ending to `/EHsc` was byte-neutral;
those private profile controls are rejected and no recipe adopts them.

The four independent field bindings are Stop44C150 twice (+12/+B6), manager
491C10 (+18), and real worker vtable4703E4 (+B1), all offsets hexadecimal.
The ordinary comparator, full PDB owner and all field/local-target rules remain
unchanged. No new Oracle code or game failure-path tests were added.

## Remaining work and reproduction

Whole Run remains1564/1568 and Initialize584/584, both nonexact. The outer
Ending destructor is now362/362 but retains four diagnostic operand-byte
differences at+2F/+31/+63/+65 in its two UnregisterElem argument preparations.
Do not credit it or force those registers through an invented ABI.

A separate complete2315-byte ANM constructor experiment used passive integer
storage with the genuine ID operators viewed over it. It still rejected all
four exact AddVm owners (117/118,117/118,102/112,102/112); all three source files
were restored. Its2296-byte constructor receives no credit. This model and the
neutral EH controls are parked pending new evidence.

Reproduce only the affected canonical units with
`scripts/repo-python scripts/replay-exact-units.py --unit ending-text-destructor --unit ending-text-constructor`.
The canonical image remains the existing shared EndingLoadScript output group.
Final receipts are under ignored
`.analysis/gpt-6.1-sol/ending-teardown-20261028/`; the rejected ANM patch uses the
sibling `anm-passive-id-20261028` directory. Final images remain in the build
directories recorded in the receipts;
[results.json](results.json) records concise pins and completed comparisons.
Runtime, whole-product and original TU claims remain open.

The seven default lightweight CI stages pass. Validation adds no host scenario,
sanitizer/mutation run, comparator modification or full-cohort replay.
