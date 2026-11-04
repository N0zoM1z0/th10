# TH10 9,964-byte ANM executor: explicit child-script argument evaluation

This changes actual maintained C++ in the complete
AnmRenderManagerView::ExecuteScript owner, original Japanese TH10 v1.00a
at 0x0043EE30 (9,964 bytes), not merely a test script.

The opcode-91 case creates a child VM through the already recovered
AnmLoadedView::CreateVmVariant2 callee, with a selected script index and
the caller's current render layer. Both arguments are target-visible at the
native call; the original code reaches the layer only after computing the
possibly indirect script operand. The source previously built an aggregate
AnmChildScriptLayer with a compound initializer. The maintained body now
explicitly evaluates GET_INT_VAR(0) into childArguments.script, then
reads vm->renderLayer into childArguments.layer, and only then invokes
CreateVmVariant2. This makes the actual operand evaluation/dependency
order explicit for this opcode while retaining the typed source model and
the real callee. It does not invent a register ABI or alter any child
request bytes or opcode semantics.

## Full-body strict original comparison

A real pinned VC7.1 SP1 /GL + /GS five-source ANM link, using the
existing exact AnmInt3Scale.cpp /Ob1 support profile, compiled the modified
AnmManager.cpp and four SHA-verified support units. The native PDB
contribution has the complete 9,964 bytes, exactly 350 target-bound
linkage fields, including 95 local/table obligations. The complete strict
comparator reports 9,962/9,964 equal ordinary bytes, with only:

- +0x113: target 0x55 (EDX layer load) vs current 0x4D (ECX).
- +0x115: target 0x52 (PUSH EDX) vs current 0x51 (PUSH ECX).

The previously retained aggregate form gave the same two mismatches.
This is a source-order and proof-stability improvement, not a new exact
function. Explicitly direct argument forwarding and separate script/layer
locals instead regressed to 9,954 and 9,952 matching bytes; signed-layer
alias, double layer read and bit-carrier controls also yielded 9,954.
The variant and exact source SHA-256, retained PE/MAP/PDB and all results
are recorded in native-result.json.

On the installed source, selected affected exact replays were
anm-loaded-create-vm-variant2 (73),
anm-vm-timer-add (88) and
anm-vm-timer-decrement (20):
181/181 bytes exact. This was a selected replay, not the historical
full cold cohort. The original 9,964-byte boss remains explicitly
NONEXACT, with no partial-byte or canonical-match credit, no target
byte editing, and no production-source or runtime claim.
