# TH10 EnemyLaser update/draw chain — 274 + 91 + 46 target bytes

Target: verified original Japanese TH10 v1.00a (`config/target.toml`).
This checkpoint restores **three complete** EnemyLaser runtime functions in
`src/EnemyLaserUpdateList.cpp` and reuses the existing *actual*
`EnemyLaserManagerStorageView` in `src/EnemyLaser.hpp`; it neither creates
an alternate `g_EnemyLaserManager` allocation nor replaces the real
Chain/ANM interfaces with fake stubs. The source-name and original-TU
ancestry of each function remain unproven. The two registered callback
boundaries have **indeterminate** source-authored-versus-generated origins;
physical target boundaries are independently established here.

## 274-byte manager list update @0x0041C330..0x0041C441 — NONEXACT

Original x86 code enters with its manager in private **EDI** and follows
`sentinel.next` at manager +0x18. It saves each laser's next pointer
at node +0x08 before any virtual callback or free. The byte at node +0x50
increments if nonzero; once its updated byte is at least two, or the
laser state at +0x0C is 1, it calls virtual `Unlink` at vtable +0x10.
Otherwise it invokes the real virtual `Update` at vtable +0x08; any nonzero
return triggers the same unlink. Both cleanup paths decrement count at
manager +0x438, patch the doubly linked previous/next fields at node +4/+8,
update the manager's tail at +0x434 when applicable, and free the original
laser allocation through `_free @0x004524A1`. Successful updates advance
`AnmVmTimerView` at node +0x10: previous gets current, normal-speed range
(0.99,1.01) increments current and subframe, and the alternative branch
adds scale to subframe and stores its integer conversion as current. The
loop repeats until the saved next pointer is null and returns 1.

The maintained function `EnemyLaserManagerStorageView::UpdateLinkedLasers`
models those virtual calls, both real removal branches, pointer adjustment
and the **existing** `AnmVmTimerView::Tick` (no shadow timer type).
Independent actual VC7.1 SP1 `/GL` linked-PE probes yield 207 PDB bytes
against native 274 as standalone entry, and **206/274** with the genuine
registered update callback selected as entry; the latter has 35/254
preliminary ordinary bytes and five candidate code/data fields. Manually
writing the full Tick math inside the source regressed to 196/274 and was
reverted. The compiler folds common cleanup/scheduling differently; none
of these probes constitute byte-exact codegen, canonical field binding,
or original source ABI proof. The complete original parent remains **NONEXACT**.

## 91-byte update callback @0x0041C480..0x0041C4DA — NONEXACT

The target's registration at **0x0041C157** directly pushes this address
into actual `Chain::AllocElem`, providing a real callable entry despite its
absence from the old denominator. Its original 91-byte body reads flags
at **`g_GameManager (0x00477810) + 0x58`**. Bit 0 or 2, and bit 10, block
updates. When bit 1 is set it saves global `g_AnmGameSpeed @0x00476F78`,
sets that speed to zero for one call to the 274-byte owner at 0x0041C330,
then restores speed; otherwise it directly calls the same owner. Returns 1
on all paths. `EnemyLaserUpdateCallback` uses the already-maintained
`GameManager` flags member, not a duplicate global/gate view. With the real
caller graph, the selected VC7.1 result is 88/91 bytes and remains
**NONEXACT**. The optional pause/restart branch is not omitted.

## 46-byte draw callback @0x0041C4E0..0x0041C50D — CANONICAL EXACT

The other actual registration at **0x0041C17A** pushes the native draw
callback. The independently decoded full 46-byte body checks the actual
`g_GameManager` flags bit 2, then traverses the existing `sentinel.next`
laser chain, preserving each saved next pointer at +8 before calling the
real **virtual Draw at vtable +0x0C**, skipping lasers in state 1. Returns
1. Its native CC-separated extent ends in `RET` at 0x0041C50D; 0x0041C50E/F
are CC padding before the distinct 0x0041C510 laser factory.

The maintained `EnemyLaserDrawCallback` (`__fastcall` ECX owner, plain RET)
compiles via pinned VC7.1 SP1 `/GL` to a **complete 46-byte PDB owner**.
Strict `enemy-laser-draw-callback-0041c4e0` canonical linked-PE comparison
finds 42/42 ordinary bytes and independently binds the only DIR32 address
field (+0x01) to **`g_GameManager` at 0x00477810**. Two zero-reuse cold
builds produce *different* PE SHA-256 and PDB GUIDs but both pass full
**46/46 bytes, one/one fields** with no differences. Rechecking after
normalizing the callback's sibling update to the real GameManager type also
passes 46/46. Private receipts are at
`.analysis/gpt-web/20261103/laser-draw-46-cold{1,2}.json` and
`laser-draw-after-owner-guard.json` (not committed).

This **one** new 46-byte physical function, of indeterminate authorship,
receives canonical exact codegen credit, and both previously untracked
registered callbacks receive independently reviewed physical boundary rows.
Neither the 91-byte update callback nor the 274-byte manager loop is
credited exact, and no whole-game runtime/product claims follow from exact
local codegen. We cold-replay **only** affected EnemyLaser initialization,
factory/cleanup, and constructor/draw-callback units; no full cold replay.

## Scoped regression receipt

The actual local canonical replay in this checkpoint completed with **four
exact units / 478 original bytes**, grouped into three independent scoped
cold PE artifacts: `enemy-laser-draw-callback-0041c4e0`,
`enemy-laser-manager-initialize`, `enemy-laser-manager-create`, and
`enemy-laser-linked-cleanup`. All four preserve complete PDB-owned
byte/field equality. `scripts/ci.py` independently passed all seven
repository exact-stage checks. Logs, not whole-cohort replays, remain under
`.analysis/gpt-web/20261103/laser-{scoped-cold-guard,ci}.log`.
