# Complete replay-save context — 2026-10-30

`FrontEndControllerView::UpdateReplaySave` remains **NONEXACT**, 1444 native
bytes against the complete 1460-byte target. This checkpoint changes no game
source and grants no exact credit. It establishes a reproducible 21-input
context containing the actual replay definitions, under the existing compiler
profiles and unchanged InitializeVm root.

The target owner is 433570..433B23: 1440 executable bytes, 399 instructions,
86 encoded image operands including its five table words, 63 fixed direct
local branches and one RET4. Its table starts at 433B10; destinations relative
to the entry are 29, BF, DD, 259 and 4C3. Every destination is an instruction
start, the table has one encoded base reference in the executable, and 12 CC
bytes precede the next independent entry at 433B30. The existing complete
boundary is confirmed without changing the denominator.

The preceding 20-input MusicRoom graph has only replay declarations. Its
replay-save caller emits 1448 bytes and uses a pushed deletion receiver and two
stack SaveReplay arguments. Adding the actual ReplayManager.cpp definition
under its existing /GL /GS /EHs profile restores target ESI/plain-RET Destroy
and ECX receiver/EDX filename/one stack name/RET4 SaveReplay. Load retains its
one stack filename/RET4 interface. No ABI declaration, root, compiler option,
source visibility or inline attribute is changed to obtain those interfaces.

In particular, target calls at relative 417/42D/43A become native 407/41D/42A:
the deletion receiver is sampled before deletion, the save name is pushed
before the EDX path load, and the load slot is sampled after saving. The full
writer remains nonexact at 1664/1681 in this context; its exact interface gives
no body credit. MusicRoom stays nonexact at 2788/2796.

Two independent cold builds compile all 21 inputs with no object reuse, one
CPU at nice10 with bounded runtime and retained exit logs. All 70 source/header
pins remain fixed. Both reproduce every normalized native byte, all 86 field
identities, 60 fixed local branches, RET4 and five table destinations. Native
code is 1424 bytes/392 instructions, and the table starts at relative 590.
The differing code, destinations and table positions remain mandatory failures.

The same images strictly pass only these explicit existing selectors:

| Selector | Complete bytes | Bound fields |
| --- | ---: | ---: |
| replay-manager-destroy | 18 | 2 |
| replay-manager-load | 181 | 12 |
| enemy-sound-queue-sample | 123 | 1 |
| game-complete-stage-transition | 411 | 28 |
| Total | 733 | 43 |

Load includes the comparator's allocation-EH address/metadata binding. This
does not establish generated EH code or runtime behavior. The four bodies
already have canonical exact credit; this context adds none. No full historical
cohort or host failure/lifecycle/mutation campaign is required.

The target reloads pressed input after the movement sound call while native
code retains an earlier value. The actual 123-byte sound queue callee is exact;
no missing effect has been established. Replacing six repeated-input calls
with the existing boolean helper preserves the 1444-byte candidate and does
not recover the target schedule. That control is restored and parked. Existing
cursor spelling controls remain parked. Reopen with new target evidence and a
distinguishing prediction; do not add volatile, opaque barriers or fake ABI.

```sh
scripts/repo-python docs/evidence/replay-save-context-20261030/verify-native.py --artifact-dir build/gpt-6.1-sol/replay-save-context-20261030/cold-b
```

`results.json` retains the exact graph/profiles, target review, native field and
destination identities, selected guard results, input pins and cold receipts.
The read-only verifier rechecks the private target hash, Capstone pin, complete
PDB contribution, every native field/destination/table word and the four strict
selectors. It does not normalize away parent differences or admit a match.
Current ignored context is
`.analysis/gpt-6.1-sol/replay-save-context-20261030/cold-b-context.json`;
build commands, source snapshots and retained exit logs are in that workbench.
The earlier 20-input MusicRoom/helper context remains valid and independent.

Authored exact remains 102270/275835 across 502 owners; codegen is 170374/1379.
Original source/TU/data ownership, product/runtime closure and portability
remain open.
