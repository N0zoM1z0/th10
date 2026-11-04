# ItemManager::Draw — target-shaped 281-byte full owner, NONEXACT

Target: Japanese TH10 v1.00a, original 281-byte
`ItemManagerView::Draw @ 0x0041B8E0..0x0041B9F8` from the pinned PE
SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
This modifies the **real** draw method over 2,198 records of 0x3F0 bytes;
it is not an unrelated leaf or stub.

The preceding dual-cursor source (maintained in `ItemManager.cpp`) reduced
the PDB contribution from 296 to 283 bytes, making the genuine EDI Item
base and ESI VM position-Y pointer. The target's next instructions load
world X/Y from the Item record and then add the global screen offsets:
`FLD [ESI+0x74]; FADD [screenX]` and
`FLD [ESI+0x78]; FADD [screenY]`.
MSVC's original source candidate instead reversed those arithmetic operand
loads. The selected change uses explicit, typed *volatile read views* of
`item->worldPosition.x` and `.y` as the two operands, without changing the
actual Item layout, cursor stride, floating precision, draw APIs or any
other function. In the genuine pinned VC7.1 SP1 `/GL /GS /EHs` ItemManager
factory graph, it reproduces both original `FLD/FADD` instruction orders
and emits a complete **281-byte PDB contribution**.

The **14** decoded address fields are not all in their original positions:
the compiler still compares the x87 Y value before storing the new
VM position, while the target first stores Y and then reloads it for
`FCOMP`. It also retains a different x87 alpha lifetime. Consequently this
new 281-byte candidate is **NONEXACT**, and neither a new canonical match
nor a partial-byte credit is claimed. The nearby 185-byte creator, 216-byte
destructor, and 84-byte registration owners remain separate exact functions;
all three retain their complete original physical size in the diagnostic.

Nine native controls with separate source hashes and linked PE/MAP/PDB were
compiled: X-only/Y-only reads, both reads, explicit destination store,
comparison readback, Y reload, combined views and compiler barrier.
Their complete PDB sizes, candidate-field geometry and selected codegen
measurements are in [native-results.json](native-results.json). Natural
reversed arithmetic expressions and volatile alpha views from the preceding
panel did not improve the target code ordering. Nothing was inferred from
function size alone. Full precise source ancestry, original target-link
field equality and runtime equivalence remain open.

All compiler outputs are ignored under
`build/gpt-web/20261104-live-itemdraw-x87/`, and the experiment is
reproducible from `.analysis/gpt-web/20261104-live/item-draw-x87-lifetimes.py`.
Re-run only affected existing exact units with `replay-exact-units.py --unit`;
never rely on an all-cohort cold replay for this local step.
