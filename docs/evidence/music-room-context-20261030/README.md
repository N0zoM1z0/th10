# Complete MusicRoom owner and dataflow — 2026-10-30

`FrontEndUpdateMusicRoom` at433EF0..4349DB remains **NONEXACT**. Its complete
physical target is2796 bytes, not the old2778-byte code-only ledger entry.
The current20-input candidate emits2788/all104 native fields. One zero-reuse
cold reproduces every native byte,97 fixed local branches, both RET4 exits and
all four table words. There is no new exact byte or owner credit.

## Complete ownership audit

Fresh dense target decoding covers2778 executable bytes/740 instructions through
RET4. Two alignment bytes precede the four-entry table4349CC..4349DB. The sole
encoded table-base reference in the target file is this owner's indexed JMP at
relative17. Table destinations are1E,456,52B,A5E, all instruction starts inside
the code. Four CC bytes follow before independently reviewed4349E0. The existing
entry and next tracked owner stay fixed. The reviewed end extends from4349C9
to4349DB without overlapping another tracked owner.

The corrected row adds18 previously omitted owned bytes to the authored
inventory denominator:275817 becomes275835. It adds no function, source mapping,
origin classification or exact byte. Target has96 direct local branches,
48 direct calls and106 decoded address/control operands including the four
pointers. The native97/48/104 counts and its different table positions are still
mandatory comparison differences; matching a count never establishes equality.

## Source recovery and real file dependency

Two row lookups used a shared ResolveVm helper that passed an integer to the
existing object-value FindVm API, constructing a temporary ID. MusicRoom now
passes its existing ID object and clears that same storage on lookup failure.
The manager captured before lookup is retained for both row text paths, matching
the target ESI lifetime. Existing storage views, shared ResolveVm, cursor helper
and headers remain unchanged. No fake ABI, forced inline, volatile, padding or
profile change is introduced. Both row loops remain complete.

The18-input baseline declares FileSystem::OpenFile without its real definition,
so it pushes a third path word where target loads EAX. Adding FileSystem alone
is insufficient: sizeOut promotes to EBX, again unlike target. The existing exact
OpenFile recipe also contains its real PbgArchive dependency. Adding both actual
sources with their already declared /GL /GS profiles recovers EAX filename,
stack sizeOut/mode and RET8 under the unchanged InitializeVm root. The caller's
two pushes and actual musiccmt.txt operand are checked. No root/flag sweep occurs.

That file callee is358/360 in this20-input context and remains NONEXACT. Its
existing360-byte canonical exactness belongs to its separate declared graph.
This batch claims the observed actual interface only. It grants no new file
callee, runtime, product or cross-context exactness credit.

## Restored controls and reproduction

A void cursor-operation helper and direct shared cursor call do not restore the
missing distinct zero/positive cursor schedules. Component-wise position writes
do not restore the target Z-store/FP order. Guarded parser pointer scope does not
restore its target schedules. All are restored and parked. Reopen only with new
evidence and a distinguishing prediction. The initial helper setup assertion
prevented a source edit; its subsequent unchanged-source relink is recorded as
an infrastructure control, not a cursor hypothesis result.

The retained source is the two row-ID/manager lifetime corrections plus the
complete physical-boundary comment. Original helper/class/TU ancestry remains
unknown. The whole owner includes all four states, parser, opening/scrolling
rows, comment display, confirm/warning/close paths and its dispatch table;
no partial case or aggregate-helper credit is used.

```sh
scripts/repo-python docs/evidence/music-room-context-20261030/verify-native.py --artifact-dir build/gpt-6.1-sol/music-room-context-20261030/candidate-cold
```

The diagnostic compiles all20 inputs without reuse, serial CPU0/nice10 with a
bounded job and retained exit. MAP/PDB-qualified complete ownership, every native
field identity, fixed destination, exit and table entry reproduce. Named static
callees are qualified through the real MAP static section; pooled title/warning
literals are identified by bounded payload hashes, and input+6 by its existing
view. These are native identities, not a blanket target-binding claim.

Same-image explicit selectors pass four audio owners381/all22 and existing
Player damage1170/auxiliary202:1753/all56. No full historical cohort or host
failure/lifecycle/mutation tests are needed. Source presence, boundary review,
origin, exact codegen, product/runtime and portability remain independent.
Current20-input context is
`.analysis/gpt-6.1-sol/music-room-context-20261030/candidate-cold-context.json`.
Older18-input GUI/audio and17-input Player contexts now have a historical
FrontEnd.cpp pin; relink their declared graphs before further work. Do not
substitute this20-input epoch for them.

Authored exact remains101913 across499 owners, now over275835 reviewed bytes;
codegen170017/1376, source1207/authored689/backlog190. Original data ownership,
product/runtime closure and portability remain open.
