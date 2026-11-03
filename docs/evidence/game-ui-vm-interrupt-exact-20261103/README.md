# TH10 paired game UI VM interrupt broadcasters — 537 native bytes exact

The two complete, independently reviewed authored functions were previously
source-absent:

- **0x00421070..0x0042117B** (268 bytes), selected interrupt **1**;
- **0x00421300..0x0042140C** (269 bytes), selected interrupt **2**.

Both are plain one-stack-argument / `RET 4` entries. This checkpoint adds
`src/GameUiVmInterrupts.cpp/.hpp` and typed independent function mappings.
The original caller class, source file, global declaration spelling and
product/runtime behavior remain unknown; `GameUi` is a *descriptive* module.
The actual TH10 Japanese 1.00a PE is pinned in `config/target.toml` and
has not been modified.

## Original executable observations

Direct original `objdump -D -Mintel` and target-hash-attested Ghidra analysis
confirm a state check at caller-object +0x6FC for value 1. When active,
each entry reads **three adjacent VM IDs** from 0x00477824/+4/+8,
finds each via the real `AnmRenderManagerView::FindVm` at **0x004491C0**,
then writes the selected interrupt as a **short** to the VM at +0x304.
If the VM's `layerNode.previous` at +0x18 is null, the child list at +0x14
is traversed through `node->next` and the same short interrupt is assigned
to each node's owner +0x304. After processing all three, it zeros each
stored ID. The interrupt-1 version sets the owner state +0x6FC to zero,
whereas the interrupt-2 version sets it to 2. Whether or not the state was
active, the separate global flag at **0x00491BEC** is tested and cleared
only when nonzero. Both bodies preserve all of these branches and fields;
there is no artificially introduced array loop in the compiler's three
unrolled native calls.

The maintained `GameUiVmTransitionView` is a partial view with an exact
+0x6FC state assertion. It is **not** identified as the unrelated
`MainSupervisorView`; no unsupported overlay/global alias ownership is
claimed. The two native global owners are described by typed VM-ID array
and integer flag declarations. Actual memory addresses remain linked
bindings, not hardcoded pointers inside C++.

## Compiler and full owner comparisons

Initially, pinned VC7.1 SP1 `/GL` linked the two natural functions to the
**exact target sizes 268/269**, but with **three** ordinary byte mismatches
each: 217/220 and 218/221 comparable ordinary bytes respectively. Every
mismatch was a private manager receiver-register encoding (`ECX` candidate
versus native `EDX`) at the three real FindVm call sites.

Adding the unchanged, real `src/AnmManager.cpp` as one `/GL` support input
correctly promotes that manager receiver to native `EDX`. The resulting
linked functions are fully **268/268** and **269/269**, with **220/220** and
**221/221** ordinary bytes equal and every target branch/instruction in the
complete owner scope. No assembly, register forcing, synthetic callers,
filler, byte patching or compiler-profile changes were introduced.

Two fresh zero-reuse, serial VC7.1 cold builds of the exact manifest graph
using `GameUiSendVmInterrupt1` as the real link entry yielded independent
PE SHA-256 and PDB GUIDs. Both strict comparator runs in each image passed
with **12/12** exhaustive source/target linkage fields *per function*:

- three REL32 calls to the actual 0x004491C0 FindVm owner;
- one manager DIR32 to 0x00491C10;
- six DIR32 references (with explicit addends 0,4,8) to the real three-ID
  storage beginning 0x00477824;
- two DIR32 references to the reset flag at 0x00491BEC.

Canonical units `game-ui-vm-interrupt1` and `game-ui-vm-interrupt2` share the
same compiler graph, not the same function identity. Their matching
full-owner reports are retained at
`.analysis/gpt-web/20261103/game-ui-vm-interrupt{1,2}-cold{1,2}.json`
(private, regenerable) and the initial mismatches are separately recorded
in `game-ui-interrupt-first.log`.

These add **two authored canonical exact functions, 537 bytes** without
crediting approximate code or unrelated owners. Native calling convention
under this selected LTCG graph is verified; original TU/compiler source
ancestry, retail product linkage, and runtime correctness remain separate
questions. Focused cold replay of these two units and the real FindVm callee
is sufficient; do not run the full project cold replay.
