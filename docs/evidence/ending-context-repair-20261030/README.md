# Ending exact-context repair and complete Run review — 2026-10-30

The current 18-input Ending recipe no longer reproduced its previously exact
648-byte text constructor. This batch repairs that real regression with two
actual source dependencies. Two independent 20-input canonical colds restore
the complete constructor648/all12 and LoadScript106/all5:754 existing bytes,
zero new exact credit. Game C++ is unchanged. The complete Run interpreter
remains **NONEXACT**,1564/1568.

## Fresh failure and dependency evidence

The first cold used the current tracked recipe and current sources. Compilation
and linking completed, but strict text construction failed at17 ordinary bytes,
first at+0x92, despite retaining the648-byte PDB contribution and all12 fields.
Its InitializeVm call promoted script index/VM to ECX/EDX and pushed the bank;
target retains ECX bank and two stack arguments/RET8. Prior historical reports
do not prove the current graph. The failed exit/report is retained privately.

The same image emitted Run1576/1568. Its text decode call pushed an argument
instead of using EAX. Fn00417010 had moved unchanged from DecomphelpLeafAccessors
to GuiMessageLeaves.cpp, while this graph still included only the former file.
Adding the actual latter definition restores the EAX interface and the complete
Run1564-byte candidate. The41-byte decoder independently passes raw-equal in
the repaired images. It is existing exact code, not a new helper claim.

That19-input model still failed construction. Gui.cpp contains two real
InitializeVm callers in UpdateStageElements. Adding that actual source restores
the target initializer incoming interface and every constructor byte, without
changing the checksum entry, existing18 profiles, source/header declarations,
visibility or any ABI annotation. The initializer's own207/217-byte body stays
nonexact. Its interface and the parent's complete equality are separate facts.

The two new sources retain their existing /GL profiles from
game-decomphelp-fulltu-00417010 and gui-score-add. Existing Ending inputs retain
/GL /GS /EHs. Only ending-text-constructor and ending-load-script migrate to
the common20-input output EndingTextContext; unrelated recipes remain separate.
The old18-input receipts are historical, not current incremental caches.

## Complete Run comparison and restored controls

The target remains40BD80..40C39F:1488 reachable code bytes, six internal
alignment bytes, two tail-alignment bytes and18 owned four-byte table words at
40C358. Independent callback40C3A0 follows immediately. The complete instruction/
alignment interval is1496 bytes; the table has one encoded base reference and
all18 destinations are instruction starts in the same owner. Every one of53
direct local branches and all92 image operands remain accounted for.

The two sound-owner immediates at+0x204/+0x284 are492590 in the target's virtual
.data tail; they are not raw-backed bytes. The existing virtual-section inventory
recognizes both without manufacturing data contents. The native1492-byte code/
alignment interval plus72 table bytes reproduces1564/all92 fields/all53 locals.
Complete native identities and fixed destinations are retained. None authorizes
normalizing the different target positions or claiming partial equality.

From command8 onward, the native destinations are two bytes earlier than target;
tail alignment accounts for the remaining size gap. At command8 target retains
the existing ID storage address across deletion, while native retains its array
index. This is a concrete remaining mismatch, not proof of an original type.
Pointer-view spelling did not restore the address lifetime. Live color and
sampled-line controls also did not close the owner. All three are restored to
the entry source. No shared ID constructors, helpers, headers or inline controls
change. Reopen these controls only with new evidence and a distinguishing
prediction. A natural shared release operation is a later hypothesis; no such
source change or original helper ownership is established by this checkpoint.

## Reproduction and acceptance limits

```sh
scripts/repo-python scripts/replay-exact-units.py --unit ending-text-constructor --unit ending-load-script --json
scripts/repo-python docs/evidence/ending-context-repair-20261030/verify-proof.py --artifact-dir build/match-linked/EndingTextContext
```

Each canonical cold compiles all20 inputs without reuse, serial CPU0/nice10,
bounded with retained exit. All69 transitive source/header pins agree with the
unchanged entry source. Full648/106 comparisons include every linked field and
the constructor's existing worker-member EH address/metadata binding. The
same-image41-byte decoder adds a required-callee check:795 existing exact bytes
and17 fields in total. No full historical cohort, host failure/lifecycle/mutation
campaign or generated EH-code/runtime claim is needed or made. Comparison
infrastructure is unchanged; the new verifier is read-only evidence.

The verifier rechecks the private target hash, Capstone pin, both strict parent
contracts, decoder, qualified PDB contributions, every native Run field identity,
all table/local destinations and actual initializer RET8. results.json contains
compact cold receipts, exact input graph/pins, complete inventories, the initial
failure and restored controls. Working logs and current context are under
`.analysis/gpt-6.1-sol/ending-run-context-20261030/`; the current receipt is
canonical-b-context.json, pointing to the actual canonical producer.

No match, source, boundary, origin or denominator is added. Authored exact stays
103090/275835 across503 owners; total codegen171194/1380. Original class/TU/data
ownership, product/runtime closure and portability remain open.
