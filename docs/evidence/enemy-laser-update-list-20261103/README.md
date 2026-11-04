# TH10 EnemyLaser update/draw chain — 274 + 91 + 46 target bytes

**Latest source status:** the 274-byte UpdateLinkedLasers owner now emits
**275 candidate PDB bytes** in the genuine registered-callback /GL graph
(**39/254** ordinary comparable bytes); a former 206-byte variant is kept
below as a historical rejected compiler control, not the current source.
The 46-byte draw callback remains canonical exact, the 91-byte update
callback remains nonexact.

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
update the manager's tail at +0x434 when applicable, and deallocate the original
laser allocation through the native scalar-delete/free CRT endpoint
at `0x004524A1`. Successful updates advance
`AnmVmTimerView` at node +0x10: previous gets current, normal-speed range
(0.99,1.01) increments current and subframe, and the alternative branch
adds scale to subframe and stores its integer conversion as current. The
loop repeats until the saved next pointer is null and returns 1.

The maintained function `EnemyLaserManagerStorageView::UpdateLinkedLasers`
models those virtual calls, both real removal branches, pointer adjustment
and the **existing** `AnmVmTimerView::Tick` (no shadow timer type).
Its target +0x50 removal byte, initially implemented through a raw pointer,
is now a named `EnemyLaserConstructionBaseView::removalPhase` member with a
compile-time **`offsetof == 0x50`** assertion. It retains the same byte
inc/compare and preserves the common laser-storage view layout. A new
actual pinned VC7.1 `/GL` caller-entry probe still emits 206/274 with
35/254 ordinary bytes and five candidate fields; no exactness promotion.
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
`.analysis/gpt-web/20261103/laser-draw-cold{1,2}.json` and
`laser-draw-after-delete.json` (not committed).

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

## Follow-up compiler negative controls after exact callback closure

The exact 46-byte callback continues to pass unchanged strict comparison
after replacing its sibling `g_BulletUpdateGate` alias with the true shared
`g_GameManager->m_Flags` typed word and changing the 274-byte function's
release to the scalar `::operator delete` pathway. The entire 274-byte
`UpdateLinkedLasers` still generates **206 bytes** with the real registered
91-byte updater selected as `/GL` entry; the compiler merges two distinct
native unlink/removal lanes into one, a remaining structural difference.
Three natural control-flow rewrites of the 91-byte updater (separate early
returns, nested gate and explicit high-byte test) were fully compiled:
the first two remained **88/91**, the third became **93/91**. All
three were rejected and the original source restored. Artifacts:
`.analysis/gpt-web/20261103/laser-update-91-variants.json` and per-variant
cold compiler logs. Neither the 274-byte manager update nor 91-byte
registered updater is claimed exact, despite the real compiler context.

## 2026-11-03 follow-up: restoring two distinct removal paths (275 vs 274)

The older maintained C++ had identical pointer-cached deletion bodies in
two branches. Pinned VC7.1 `/GL` naturally merged both native deletion
routes into one, yielding **206 vs target 274 bytes**. A fresh comparison of
the **entire** original executable at 0x0041C330 established that it does
**not** merge them: one route begins at original +0x33 and copies the
*next* pointer first, whereas the other begins at +0x78 and copies the
*previous* pointer first. Each route separately unlinks, updates the
manager's tail, calls scalar `::operator delete` at 0x004524A1, and rejoins
the loop after the chosen branch. The distinction matters for precise
register scheduling and potential pointer aliasing, not just bytes.

The maintained source now expresses the first removal route with **direct**
`laser->previous`/`laser->next` member reads and the second with cached
`previous`/`following` typed pointers. Five real, bounded source experiments
checked the two deletion routes: selected first-direct shape gave
**266/274 PDB bytes** (37/254 comparable ordinary), while changing only
the second path into direct reads merged the routes again (206/274); one
mixed variant yielded 263 and one extra-uncached-tail variant 247.

The selected version additionally stores an **8-bit** `removalPhase`
result explicitly after the increment and assigns its >=2 comparison into
`expired`. The target actually uses `INC AL; CMP AL,2; MOV [ESI+50],AL`,
so the wrap/zero behavior is preserved; this source is not a dummy flag or
register-forcing trick. It yields **275/274 PDB bytes**, 39/254 ordinary
comparable bytes, five candidate address fields (not an exhaustive bound
normalization contract), with unchanged real virtual Update/Unlink, real
operator delete and real inline timer Tick semantics.

Six additional source variations (increment-local/field, equivalent
const-byte variable and explicit branch, and store/check ordering) were
separately compiled to their complete extents; three neutral cases stayed
275/274, two regressed to 266/274 and one to 275 with 36/254. The
maintained direct-first + explicit-byte-state form is the best *full-owner
layout proximity*, still **NONEXACT**. This changes the large live source
meaningfully; it does **not** justify any new exactness credit or claims
about original C++ authorship, data origins, runtime/game behavior or the
exact native caller ABI.

Reproducible private reports are
`.analysis/gpt-web/20261103/laser-274-{deletion-variants,source-scheduling-results,phase-variants}.json`
plus `laser-selected-275.log`. The already exact 46-byte draw callback and
linked laser-manager/constructor functions are cold-replayed only in the
affected scope, not through full historical cold product replay.

## Independently separated 43-byte laser draw-list owner @0x0041C450

**Superseding result:** this previously NONEXACT 43-byte source has now
closed full **43/43 raw exact** through the genuine 46-byte registered
draw callback as the `/GL` link entry. The first isolated-source mismatch
diagnostic below remains as a historical compiler-context control, not
the selected current exactness status. Original caller provenance still
remains indeterminate.


Native 0x0041C450..0x0041C47A is a complete CC-delimited **43-byte**
laser Draw traversal, physically distinct from both the large 274-byte
update and the separately exact 46-byte registered draw callback. It reads
laser-manager +0x18 from native **EAX**, checks that pointer, walks the
+0x8 `next` chain with a saved node, skips state1 at +0x0C, dispatches
virtual slot3 `Draw` through vtable +0x0C for each remaining node, and
returns 1. This is independently decoded actual executable behavior, not
inferred from name similarity. There is **no independent direct inbound
call** or authentic C++ declaration to prove its original authored vs.
compiler-retained origin; both metadata ledgers mark it **indeterminate**.

New maintained `EnemyLaserDrawList(void *owner)` in the same typed
`src/EnemyLaserUpdateList.cpp` expresses this entire function. The
pinned VC7.1 SP1 `/GL` build from this real source emits a unique
**43-byte** PDB-owned function with **40/43 raw instruction bytes** equal
and **zero** relocation fields. Three bytes encode private receiver/register
choices: the original expects its manager in EAX and virtual vtable in EDX;
ordinary `__fastcall` source receives the manager in ECX and naturally
uses EAX for the vtable. The original caller context has not been found,
so this is **NONEXACT**, no canonical match row or codegen byte credit.
Proof: `.analysis/gpt-web/20261103/laser-draw-list-43-first.json` plus
its complete linked disassembly. Its source introduction does **not** change
the independent 46-byte draw callback or exact manager initializer, each
of which remains guarded via scoped cold replay. Do not manufacture a
caller or inline-assembly register shim just to force these three bytes.

## 2026-11-03: natural parent inlining closes the 43-byte owner exact

A fresh, nonbinary-edited **source** change lets the actual
`EnemyLaserDrawCallback` call the typed `EnemyLaserDrawList` helper rather
than spelling the identical loop twice. The target's 46-byte callback
still has **no physical E8 call**: pinned VC7.1 SP1 `/GL` inlines the
helper into that registered callback. Crucially, it also retains a
separate 43-byte PDB-owned callable `EnemyLaserDrawList` in the same
primary source module. The real call-context compiler sees the helper
receiver in **native EAX** and virtual vtable pointer in **native EDX**;
this naturally corrects all three register encoding differences of the
isolated 40/43 source experiment, without forcing registers, patching
machine code or inventing assembly. All original game-visible behavior
of the registered draw callback is unchanged.

The new canonical `enemy-laser-draw-list-0041c450` unit uses the unchanged
pinned `/GL` profile and the real `EnemyLaserDrawCallback` as its root.
Two independent zero-reuse cold linked PEs have different SHA-256 and
PDB GUIDs. Both strict full-owner comparisons reproduce **43/43 raw-equal
bytes**, with zero relocation fields, zero skipped bytes and a unique
43-byte primary-source PDB contribution. The separately canonical
`enemy-laser-draw-callback-0041c4e0` continues to match **46/46 bytes**
with its sole independently bound `g_GameManager` reference at 0x00477810.
Receipt files:
`.analysis/gpt-web/20261103/laser-draw-list-cold{1,2}.json` and
`laser-draw-callback-inlined-cold2.json` (ignored, regenerable).

This is **43 additional exact physical codegen bytes** in the target,
credited only once. No new authored-game origin credit is claimed: the
original 43-byte target has no independently proven direct incoming call,
so its historic handwritten-vs-retained-compiler provenance remains
**indeterminate** even though its source expression is now exact in a
real maintained callback graph. The 91-byte update callback and
274-byte list update remain **NONEXACT** and are not promoted by this
result. Scoped cold replay protects both exact draw functions and nearby
laser manager functions, never a full cold product or historical cohort.

## 2026-11-03 follow-up: real source-level draw helper call is exact

The distinct CC-delimited 43-byte `EnemyLaserDrawList @0x0041C450` was
initially 40/43 ordinary bytes when compiled without a caller. Changing the
existing maintained `EnemyLaserDrawCallback` to call that typed list helper
caused the original VC7.1 `/GL` optimizer to inline the draw traversal into
the callback, **and** retain the list's separate PDB owner. Both source
functions now have exact codegen **in the same real compiler graph**:
43/43 raw exact for the helper, and 46/46 with its one bound global for the
existing callback. Two independent cold builds and the strict complete
function comparators close both; see the separate
[43-byte helper exactness evidence](../enemy-laser-draw-list-exact-20261103/README.md).
The original executable has no verified direct E8 call to the 43-byte
helper and its original authorship remains indeterminate. The larger
274-byte updater and 91-byte update callback remain nonexact; their
previous negative controls still apply.

## Separately recovered 54-byte generic point-check accumulator @0x0041C880

The original Japanese 1.00a contains a complete 54-byte, CC-delimited
`0x0041C880..0x0041C8B5` routine ending in **RET8**, immediately before
the 442-byte laser line `Initialize` owner at 0x0041C8C0. This was
previously source-absent but physically reviewed; no direct incoming
call or original authored identity was identified, so existing
`config/function-origins.csv` correctly leaves it **indeterminate**.

It accepts the real laser manager through native **EAX**, plus two stack
arguments (a typed `EnemyLaserVectorView *` at the first stack slot, and
a 32-bit float at the second). It reads the actual list head at +0x18,
starts sum in EDI at zero, saves each node's +8 next pointer before any
virtual call, skips nodes whose state word +0x0C equals 1, invokes the
real vtable **slot8 `CheckPoint` at +0x20**, and accumulates every virtual
return into EDI. After walking all nodes it returns the sum in EAX; no
special overflow/empty-list guard or error path was added.

`EnemyLaserManagerStorageView::AccumulatePointChecks` is now implemented in
`src/EnemyLaserUpdateList.cpp` and declared alongside the real manager's
other typed operations. Its first *isolated* VC7.1 SP1 `/GL` link emits an
independently PDB-owned **54-byte** function and zero relocation fields;
the complete comparison has **31/54 raw ordinary bytes** equal. The
remaining 23-byte differences include the compiler's normal ECX receiver
in place of the original private EAX manager and its spill/save choices,
plus the virtual vtable EAX versus native EDX. With no proven original
caller, this is **source-present NONEXACT** and receives no canonical match
or authored-origin credit. Reproducible local report:
`.analysis/gpt-web/20261103/laser-point-54-first.json`.

The same maintained file's separately exact 43-byte and 46-byte draw
functions are cold-replayed as narrow regression guards. The 274-byte
update and 91-byte callback remain NONEXACT, and no full historical or
product replay is claimed.
