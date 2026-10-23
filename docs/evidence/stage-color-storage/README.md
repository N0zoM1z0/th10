# StageExecuteScript color write scheduling

2026-10-23, startingcf3e379. Adopt the existing four-byte AnmColorView over the
fog color word in opcode8 of the complete1868-byte Stage dispatcher. This
recovers five target bytes without losing any formerly matching byte. The
function remains nonexact; no new exact unit or percentage credit is added.

## Target and source change

Target403E7F reads the packed word, extracts its low byte, then writes the word
to stage+2B60 before the first integer scratch spill. The baseline instead
spills that integer before writing the word. The typed color view moves the
six-byte packed store into the target position. The rest of the channel
conversion schedule still differs.

The full parent has exactly nine changed raw bytes, at+206..20E. None overlaps
any of the complete candidate's36 linked fields. Five become target-equal;
four were already different and remain different. No target-equal byte regresses,
and all other1859 bytes, including every linked field/table word, are unchanged.
An additional comparison of all258 complete public PDB owners in the nine-TU
graph finds only StageExecuteScript changed; the other257 retain raw equality.
Unowned/static fragments are not promoted by this check.
This is independently checked from both PE images and raw target bytes, not
inferred from an aggregate score. It does not establish the original field type
or production TU; AnmColorView is a bounded existing representation view.

The host dispatcher fixture now imports that real union declaration from the
maintained header, retaining its prior observable tests rather than inventing a
second host color layout. Both actual-source state/fog tests pass, including the
packedFEDCBA98 channel case. These host seams do not prove Windows game runtime.

## Current-context experiments

| Form | Parent bytes / frame | Case matched / comparable | Gap / same-size groups |
| --- | --- | --- | --- |
| Fresh maintained baseline |1868 /8C|1438/1619|2 /13|
| Low-byte snapshot before color store |1856 /8C|1341/1619|14 /11|
| Existing typed color view, adopted |1868 /8C|1443/1619|2 /13|
| Six by-value vector copies in splines10/11 |1824 /74|831/1619|43 /9|
| Four explicit signed-int channel values |1892 /98|1304/1627|24 /11|

All15 table entries, internal instruction destinations, complete table extent,
alias domains and physical group order are checked. Only opcode8 changes under
the adopted form. Whole-parent structural positions rise1236->1241 of1724;
these masked structural diagnostics are not an exhaustive semantic binding or
whole-function exactness proof. Existing11 full guard contracts987/all36 pass
for the fresh baseline, all four private controls and the maintained adoption.

The fresh baseline compiles nine actual current game TUs using the real
StageCreate entry and unchanged /GL /GS profiles. Its observer also captures
two normal diagnostic-anchor compilations; an overly broad post-link count
assertion rejects that wrapper after the native cold has already succeeded.
Recovery classifies the nine game inputs and two anchor compilations, verifies
their hashes/entry and collects the full checks without rebuilding. The failed
observer status and successful native stages are recorded separately.

Private controls each compile only StageScript and reuse eight verified game
objects plus two harness inputs. A separate maintained-path primary compile
reproduces every raw parent byte of the selected private candidate, and all11
full guards pass again. This is two primary compilations with fixed supports,
not a new independent nine-TU cold pair. The canonical StageRealCallback output
is refreshed with the actually validated source; its former outputs are
hash-archived before replacement. The three larger-owner baseline artifacts and
source/header pins stay unchanged; their reconstruction work remains open.

## Evidence and continuation

[Evidence](evidence.json) binds source/fixture transitions, complete raw delta,
actual profiles/object hashes, parent/case observations, guard scope, failed
observer recovery, host test and cleanup. Four adjacent patches preserve the
actual hypotheses. Private inputs, full disassembly/field observations and
producers remain in .analysis/gpt-6.1-sol/stage-script-values-20261023. Historical
manifests intentionally bind before-state hashes; do not restore source or
silently relax pins to rerun them against this adoption.

All60 private candidate files are hash-verified and removed, reclaiming2704921
bytes. Keep the current canonical image, fresh pre-adoption baseline and former
canonical witness for the unresolved parent. Source, fixture and documentation
are the only maintained changes; nine unrelated D3D files remain untouched.
Target and current readOnly/noanalysis Ghidra mapped-text attestations pass,
with maxcpu1/heap768m. Native builds are serial nice10. Focused gates replace
full CI under operator instructions; product/runtime and semantic gates remain
open or unstarted. Next investigate opcode13's accumulator store and its
one-byte size excess, opcode12 receiver lifetime, and remaining spline registers.
