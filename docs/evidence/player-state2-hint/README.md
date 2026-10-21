# Player state2 transition and automatic caution hints

2026-10-22, baseline `dd241de`. Three formerly source-absent authored extents now
have complete maintained source: PlayerTransitionToState2 at4269D0/796,
HintOwnerView::AddAutomaticEntry at41A120/220 and AddCaution at424650/104.
Only **AddCaution104 is newly canonical exact**. Authored/source totals become
605/735; authored exact397/72387 bytes,26.46% provisional. Source-present authored
bytes are246417/273576 (90.07%); remaining source-absent130. This is not a complete
game denominator or a runnable product claim.

This packet describes the immutable `bdba3f7` source context. The later
[entry-copy controls](../hint-entry-copy-controls/README.md) change Hint.cpp and
replace the canonical carrier with its new source context; the private
final-frozen baseline remains retained. Replay historical claims from their
pinned commit, rather than treating these source hashes as current.

## Target observations and recovered source

Raw target instructions, callsites and read-only Ghidra agree on all three
complete dense extents. Player update calls the transition after its state4
timer reaches8. The transition applies scaled one-third faith loss/floor5000,
decrements lives and updates their display only when the new count is nonnegative,
sets state2/timers0 and180, executes Player animation script0, deactivates four
options and marks their primary/secondary VMs for deletion. It clears optionCount,
conditionally submits Caution text with the **live Player position**, invalidates
a spell bonus at timer>=60, then adjusts/clamps rank by-1024.

The panel clear touches VM indices0,1,3,4,5,6,7 and deliberately preserves2.
The existing EnemyGameState.hpp partial view already owns these maintained
fields; no duplicate GUI/global storage is introduced. Faith/rank use existing
GameScoreState methods and canonical shared globals. Source declaration and
physical TU choices remain hypotheses.

The104-byte caution helper saves next links and decrements only positive old
remaining counts, then calls the220-byte allocator with text and position.
It subtracts60 from the new count, floors it at1, and sets time90,
colorFFFF8080 and remaining30. The allocator appends before reading the supplied
position/text, copies64 text bytes and selects stage/chapter counters. Existing
constructor zeros after the bounded copy and textWidth zero are retained.
The old difficulty-indexed-resource description is superseded by the observed
stage-list relationships; missing private registers in decompilation are not
used as source signatures.

## Exact and nonexact compiler evidence

Two independent complete six-TU cold links compile Player, Hint, ItemManager,
AnmManager, AnmVmId and Gui with the unchanged VC7.1 `/GL` profile and the real
PlayerUpdateCallback entry. The104-byte PDB contribution belongs to Hint's
support object. Both bodies repeat raw; complete decoding and exhaustive
comparison bind stage DIR32 at+1 and automatic-entry REL32 at+32h. All104 bytes
pass. The canonical unit is `hint-add-caution`; its callee address binding
grants no exact bytes to the nonexact210/220 allocator. The new transition
remains782/796. No flag, acceptance-policy, ABI annotation or target-byte patch
is introduced to force a match.

Copied-native controls preserve the original image: a changed zero-remaining
guard yields103/104, a shifted stage global is rejected and an interior-entry
callee is rejected. A copied positive passes104/104. The unchanged normal-COFF
Player wrapper cold-replays7 bytes/all1 field. No broad prior-unit replay is run.
[Native results and source/artifact pins](native-summary.json) and
[target observations](target-facts.json) retain these independent scopes.

The old eleven-TU large graph containing Player now has historical input pins.
All32 source/header pins of the canonical ten-TU graph excluding Player/Hint
remain equal; it is not rebuilt here. Earlier three-large-owner measurements
keep their original contexts and grant no new credit from this batch.

## Bounded source contracts

`scripts/test-player-state2.py` executes the three actual source bodies plus
real timer, faith/rank, link append, entry construction and VM deletion source.
1792 defined-input transition cases and one bounded-copy case cover timer
initialization, zero-life display, positive/zero/negative remaining counts,
stage routing, rank clamps, retained VM2, child-owner propagation and live
position after the ANM boundary. Allocation changes the panel pointer/rank to
check that the source reloads them after the caution call. Seven strict
functional mutants are detected; compilation failures, crashes, sanitizer
diagnostics and unrelated CHECK failures are rejected as harness errors.

Host owner/VM/GUI/ANM boundaries are explicit role projections and seams.
ASan/UBSan/leak success proves these bounded source contracts, not native
layout/private ABI, original object construction, TH10 runtime or product closure.
Canonical native compilation separately uses the real maintained headers.

```sh
scripts/repo-python scripts/test-player-state2.py
scripts/repo-python scripts/replay-exact-units.py --unit hint-add-caution --json
scripts/repo-python scripts/replay-exact-units.py --unit player-update-callback --json
```

The independent global boundary audit still differs at historical manual
metadata/states. Current counts are in target-facts; it is neither bulk rewritten
nor waived. Native product, runtime, semantic and portability gates remain open.

## Final validation and retained evidence

The frozen source passes all106 public CI checks. Final independent cold links,
the ordinary wrapper and all seven strict source controls pass with current
input/artifact hashes. [Validation](validation-summary.json) records their scope
and the three earlier incomplete CI attempts. The frame fixture now extracts
through Update's complete closing brace rather than EOF; its original frame
region, assertions, models and mutation classifiers are unchanged.

Only validation documentation and ignored-output cleanup follow the passing CI.
The [cleanup record](cleanup-summary.json) removes5339430 logical file bytes,
including the superseded initial/typed-position carriers, reproducible host
outputs and Git-recoverable baseline copies. Both final native carriers, current
receipts and unresolved compiler/ownership observations remain. Independent D3D
work and unclassified earlier artifacts are preserved.
