# TH10 ResultMenu/Chain: recover pending-removal flag as native bitfield

Target: the independently verified Japanese TH10 v1.00a original executable,
SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The original task-retirement leaf at `0x00422200` checks the ResultMenu
update/draw callback pointers at +0x08/+0x0C and sets **bit 1** of each
non-null ChainElem flag word at +0x04. The registration counterpart
`ResultMenuView::Initialize @0x00422150` clears that bit for both newly
allocated tasks. The independently exact cleanup at `0x00422220` later
unregisters the tasks under scheduler critical-section handling. These are
separate operations; marking a task is not itself a free or unregister call.

`th10::ChainElem::flagBits` now identifies the *same* bit as
`pendingRemoval`, between its existing `heapAllocated` bit 0 and the 30
opaque upper bits. No new storage or ABI layout is introduced. Both complete
ResultMenu creation/initialization and retirement paths now access the typed
bit, rather than repeating unexplained raw `2u` masks. The structure remains
the target-sized 0x24-byte ChainElem, and the task pointer owners are already
offset-checked in `ResultMenu.hpp`. Upper bits remain opaque; the field name
is semantic reconstruction rather than proof of original identifier spelling.

## Complete selected cold evidence

A real pinned VC7.1 SP1 `/GL /GS /EHs` selected rebuild of the **current
modified source** passed four original-target exact owner replays, all full
PDB contributions and all their existing bound address fields:

- `result-menu-constructor`: 101/101 exact;
- `result-menu-initialize`: 142/142 exact;
- `result-menu-creator`: 67/67 exact;
- `result-menu-cleanup`: 311/311 exact.

Total **621/621 previously accepted bytes**, not new exact credit. The
independent `ResultMenuMarkTasks422200` still has a **26-byte complete PDB
candidate**, versus the **28-byte original**, with no external fields. Its
private target EAX receiver and ECX native receiver remain unresolved;
**NONEXACT**; no 26-byte prefix credit or unreviewed ABI forcing is claimed.
Target/candidate hashes, source pins and selected exact units are recorded in
`native-result.json`; the local PE/MAP/PDB lives in ignored
`build/match-linked/ResultMenuLifecycle/`. This is a native codegen checkpoint,
not a whole-game link or runtime equivalence proof. No historical all-unit
cold replay was run.
