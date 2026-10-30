# Complete GUI message interpreter — 2026-10-30

Later source progress is recorded in the
[2026-10-31 refinement](../gui-message-refinement-20261031/README.md); this packet
retains its historical source epoch.

`GuiMessageVmView::Run` at415E90..41700F remains **NONEXACT**. The recorded
real-callee context emits4496/4480 bytes, with333 native link fields and121
fixed local branches. A fresh18-input cold reproduces the complete native
owner, both RET4 exits and all43 entries of its four tables. No new exact
owner, byte, boundary, origin or source-presence credit follows.

## Fresh target and source findings

The target owns4306 executable bytes/1146 instructions, two alignment bytes and
172 selector bytes. Table partitions at10D4,1134,1150,116C contain24/7/7/5
entries. Every destination is an instruction start inside the owner. Fresh
whole-owner decode records121 direct local branches,150 direct calls, both
RET4 exits and337 decoded address/control operands including43 table words.
These target operands are decoded explicitly; fresh PE-header review confirms
the target base-relocation directory is empty. Names/types and original TU
remain provisional.

The current17-input GUI image at entry emitted4492, rather than the historical
4136/4480 source epoch. Its opcode groups followed numerical order. The target
physical order is14,15,16,17,3,1,2,22,23,4,5,6,7,8,9,12,13,10,11,18,19,20,21,0.
The maintained complete case blocks now follow those physical groups. Each body
was initially moved intact; later target-backed corrections are separate below.
Matching physical group order does not prove relative destinations or bytes.

Six message lookup sites now pass the existing `AnmVmIdView` object, removing
integer-to-wrapper construction. An explicit decoded instruction-time cursor
preserves one time read at entry and one after each instruction advance. This
restores the first text-output group's actual ID/color register schedule. The
player-portrait opcode now has both physical character0/1 creation arms seen in
target, instead of a combined predicate and one call. Replay completion's two
branches share the target mode4 action. Direct audio constant branches remove
an actual temporary and restore theD8 frame and initial timer conversion slot.
None of these controls uses fake arguments, padding, volatile access, register
annotations, forced inline or a compiler-profile sweep.

The final native owner has149 direct calls. Its per-callee counts agree with
target for every callee except SetCurrent: native2 versus target3. Mode switching
now has the target two physical calls, and VM creation has the target18. Target
wait branches have separate zero-reset calls; the compiler still merges one in
this source context. Prefix time registers, later cached-manager lifetimes,
wait-tail merging, branch/store schedules and field/table positions remain open.
121 local branches on each side are a count, not proof of matching destinations.
The native prefix is4324 bytes including alignment/1161 decoded instructions;
all333 identities and all native destinations/tables are replayable receipts.

## Real leaf visibility and migration

The old graph declared the41-byte encoded-text transform but omitted its
implementation. That forced a stack-argument call where target passes EAX.
Adding the entire old leaf TU fails with four actual duplicate definitions
already present in Enemy.cpp. The interpreter's two real leaf dependencies,
decoder417010/41 and stage-state4175E0/32, therefore move unchanged into
`src/GuiMessageLeaves.cpp`. Their bodies, existing noinline annotations,
barrier, absolute accesses, types and optimization pragma are preserved; no new
retention mechanism, data alias or ownership claim is introduced.

Each migrated canonical recipe retains its original leaf primary, profile and
Fn404990 entry, adds the new support and qualifies its PDB owner. A distinct
output directory prevents two different graphs from sharing an artifact owner.
Two independent current two-input colds pass both complete raw-equal bodies:
73 bytes/zero fields each time. Both also pass in the actual18-input interpreter
image. Source ledgers move the two mappings without changing origin or totals.
The unrelated old leaf units are not replayed merely for sharing their primary.

## Bounded controls and reproduction

The sequence is4492 entry,4508 physical groups,4500 existing IDs,4492 real
leaves,4492 memory-left predicate,4528 distinct player arms,4512 direct audio,
4508 decoded time cursor,4508 target audio branch,4496 shared replay action.
Sizes describe complete observations, not progress percentages. Restoring a
missing real call is retained even when it increases size. An early-break wait
rewrite does not recover the third reset and is restored. Do not reopen that
spelling control without new evidence. No speculative profile/entry sweep is
adopted. The failed full-leaf graph and superseded native controls are private
retained evidence, not canonical recipes.

```sh
scripts/repo-python docs/evidence/gui-message-run-context-20261030/verify-native.py --artifact-dir build/gpt-6.1-sol/gui-message-run-context-20261030/candidate-cold
timeout 600 taskset -c 0 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit game-decomphelp-fulltu-00417010 --unit game-decomphelp-fulltu-004175e0 --json
timeout 600 taskset -c 0 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit player-calculate-damage-to-target --unit player-auxiliary-damage-at-position --json
```

The final diagnostic rebuilds all18 inputs without reuse, serial CPU0/nice10,
with bounded jobs and retained exits. Same-image strict checks pass decoder41,
stage-state32, damage1170 and auxiliary202:1445/all34. The previous complete GUI
updater receipt also reproduces3836/all72 in this image and still has precisely
its four ordinary differences. The explicit canonical Player replay passes
1372/all34. No full historical cohort or host failure/lifecycle/mutation tests
are required. Product/runtime, original helper/type/TU/data ownership and
portability remain open.

Current interpreter context is
`.analysis/gpt-6.1-sol/gui-message-run-context-20261030/candidate-cold-context.json`;
current separate17-input Player context is
`.analysis/gpt-6.1-sol/gui-message-run-context-20261030/player-cold-context.json`.
Previous Gui.cpp pins are historical. Native candidate reproducibility does not
waive any target byte, operand, local destination or table-entry difference.
Authored exact stays101532/275817 across495 owners; codegen169636/1372;
source1207/authored689/backlog194. No canonical Run recipe/match is added.
