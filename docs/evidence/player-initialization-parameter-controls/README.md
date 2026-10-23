# Player initialization: parameter, partition and caller controls

Recovered for handoff on 2026-10-23 at source checkpoint bd2b5a4. Maintained
PlayerInitialize4247F0 remains **1435 bytes,1195 matching/240 different**, with
all49 independent address bindings. No source, header, profile, canonical recipe
or exact ledger is changed by this batch. These results supersede the unexecuted
parameter/vector suggestions in the [previous packet](../player-initialization-caller-controls/README.md).

[The compact results](controls.json) bind21 completed native images to their
producer reports and hashes. Eighteen have complete1435-byte comparisons; three
fail the strict size check without cropping. All21 pass eight full shared guards
1753/all99. No independent cold pair, new exact, product or runtime claim follows.

| Control | Full parent result | Complete replay versus maintained baseline |
| --- | --- | --- |
| Static Chain priority-first / chain-first / ordinary member priority-first | 1195/1435 each | Identical; all17 inputs freshly compiled for each coherent interface |
| Natural vector named-result return | 1195/1435 | Identical; all17 freshly compiled |
| VM wrapper alone / reset plus binding moved into separate TU | 1195/1435 each | Identical; two inputs compiled/16 pinned inputs reused; core273 exact |
| Explicit /G6 / /G7 Player profiles | 1195/1435 each | Identical; /GL retained |
| Natural callback registration helpers | 1195/1435 | Identical after automatic inlining |
| Precompute node flag mask before existing barrier | 1195/1435 | Nine changed still-wrong bytes; equal score is not byte neutrality |
| _WriteBarrier replacing first _ReadWriteBarrier | 1193/1435 | 13 changed bytes, zero repairs/two losses |
| Volatile manager reads / first manager only | 1192/1435 each | 35 changed bytes, one repair/four losses |
| Volatile manager reads with draw manager local | 1192/1435 | 191 changed bytes,16 repairs/19 losses |
| First volatile manager captured before flags | 1191/1435 | 27 changed bytes,one repair/five losses |
| Ordinary VM-owned script binder, coherent declarations/all24 maintained calls | 1195/1435 | Identical in both17- and19-input graphs; all inputs freshly compiled |
| Unchanged source plus two real script-binder caller TUs | 1195/1435 | Identical; two new inputs/17 pinned reused |
| /Op / /Os / vector copy-plus-assign | 1471 /1270 /1500 bytes | Strict complete-extent rejection; no partial parent comparison |

The VM-owned binder test converts every actual call in AnmManager, Player, Gui,
EnemyEclDispatcher and LaserType1Clear. Its19-input form freshly compiles all19
and retains the complete core guard; no old interface survives as an opaque
anchor. It still leaves the wrapper200/nativeEDX versus202/targetESI and child
binder197 versus203. Moving methods, signatures or TUs in these tested graphs
therefore does not explain the receiver bridge.

_ReadBarrier is rejected before linking: pinned VC7.1 reports C4163, unavailable
intrinsic. It has no completed image or parent result. Inherited vector storage,
volatile containing-bitfield storage, a plain manager getter, both manager locals
and the calc-volatile/draw-local combination are **prepared but unexecuted**.
Do not report those controls as tested or remove /GL to fit a normal-COFF Oracle.

## Missing real caller and next large-owner experiment

[Target observations](missing-caller-observations.json) retain the separately
attested read-only review of404450/214. Its sole incoming call4181E3 is in
GameManagerUpdateBody418190; the existing opaque source interface is
`GamePrepareAnimationSlots(g_GameAnimationSlots)`, not the later ResetGameStage
helper. The old ledger prose suggesting ANM manager-slot iteration is superseded
as a behavioral hypothesis: observed offsets match the maintained Stage view.

The target marks three Stage callback nodes, iterates signed object/primitive
records, assigns object flags1, initializes object VMs with the signed script
index at primitive+4, stores successive VM indices at+6, advances by signed
byteSize+2 and resets instruction+4C from scriptBase+1C. Existing StageScript.hpp
supplies the relevant fields; Ghidra's inferred wrapper arguments are incorrect,
so use the recorded raw ESI/EAX/stack call operands. Original classes/TUs remain
unknown. Source for this214-byte dependency is still absent.

A useful next experiment is to supply that real caller dependency in a private
Player graph, with its actual GameManager caller. Judge the full1435-byte parent,
all49 fields and full shared guards; the dependency is a prerequisite hypothesis,
not a standalone small-function exact objective. Target wrapper caller closure
remains open. No such reconstruction was started during cleanup.

## Local recovery

Producers, compact reports and prepared controls remain under
`.analysis/gpt-6.1-sol/player-parameter-partition-20261023`. Retained complete new
carriers are `child-callers-baseline-19` and `vm-script-owner-all-19` under the
corresponding build campaign; the VM-owner source/header snapshot remains expanded.
Both complete replays can run without restoring the closed controls:

```sh
scripts/repo-python .analysis/gpt-6.1-sol/player-parameter-partition-20261023/bound-parent-19.py child-callers-baseline
scripts/repo-python .analysis/gpt-6.1-sol/player-parameter-partition-20261023/bound-parent-19.py vm-script-owner-all
```

Each prints mismatch,1195/1435,49 fields. Archive locations, per-file verification
and restoration commands are in the [handoff cleanup receipt](../handoff-ready-20261023/README.md).
Original input pins are immutable; never restore historical code over src/ or
weaken a source/artifact hash merely to run an old consumer.
