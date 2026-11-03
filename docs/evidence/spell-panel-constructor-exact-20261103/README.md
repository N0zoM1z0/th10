# TH10 spell-panel constructor — full 176-byte codegen and EH contract

Original Japanese TH10 1.00a: **0x00408910..0x004089BF**, 176 bytes, one
explicit stack receiver with `RET 4`, called by the 136-byte factory at
0x00408C90. Original executable hash matches `config/target.toml`.

## Fresh correction to the established type model

The target initializes the two VMs at owner+0x10, the eight VMs at
+0x778 and the five VMs at +0x24D8 with three real VC7.1 vector
constructors, then clears the timer flag at +0x3744, all 0x37B0 bytes of
the enclosing object with `REP STOSD`, sets owner flag 2, and publishes
pointer 0x004776F4. Importantly it **does not** invoke three additional
`AnmVmIdView` default constructors at +0x768,+0x76C,+0x770. The old
maintained member `AnmVmIdView panelVmIds[3]` did invoke them and produced
an incorrect 210-byte or larger constructor.

A small typed bit-storage member `SpellPanelIdSlotsView` now preserves
12 ordinary data bytes at those offsets, with `operator[]` exposing
existing `AnmVmIdView` references for the caller's actual VM-ID operations.
The enclosing `EnemyGameStateView` remains exactly `0x37B0` bytes and all
pre-existing field offsets hold. This restores the target's lack of extra
construction while retaining existing lifecycle/GUI call spelling.

## Two independent native codegen proofs

New canonical unit `enemy-spell-panel-constructor` uses actual maintained
`EnemyGameStateLifecycle.cpp` plus the real `AnmManager.cpp` with pinned
VC7.1 SP1 `/GL /GS /EHsc`; the unmodified `CreateEnemyGameState` is the
entry to provide a real constructor call. The PDB reports the full **176**
bytes with a unique primary-source contribution, no cropped extent.

After normalization of **14 exact target-bound address fields**, every byte
matches. This includes FS exception-list entries (target `0`), all three
VC7.1 vector-constructor calls (0x0045252D), the original
`AnmVmView::AnmVmView` and `~AnmVmView` identities (0x00402050 and
0x00401FF0), and the real 0x004776F4 published pointer. Both independently
cold-built linked images have distinct SHA-256 and PDB GUIDs. Strict
JSON reports: `.analysis/gpt-web/20261103/constructor-pod-ids/first-compare.json`
and `second-compare.json` (private, regenerable).

The handler field `+0x09` additionally uses an explicit, opt-in verifier.
The compiler-generated **62-byte** EH region is exhaustively checked in
both candidate and native executable: two cleanup actions (25-byte action
for two VMs at +0x10 and 27-byte action for eight VMs at +0x778), followed
by 10-byte `___CxxFrameHandler` dispatch. Target handler **0x00465514**
loads the immutable `FuncInfo` record at **0x00471590**, which identifies
its two-state unwind map at **0x00471580**, with predecessors -1 and 0 and
correct cleanup addresses. The candidate must have the same independent
readonly metadata, all six generated code/address fields, and the complete
34-byte VM destructor from the real AnmManager source, with the original
single `free` relocation bound correctly. Third-array destruction is in
the actual object's destructor and is not incorrectly attributed to this
constructor's unwind map.

The previously Stage-only generated-array EH validator was generalized to
this *reviewed* spell-panel protocol, keeping its exact opt-in, full
PDB-contribution, runtime-callee and source-provenance constraints. The
focused synthetic test suite verifies the two original Stage cases plus
this short-offset panel variant, including **103 negative mutation tests**.
Neither generated EH code nor the original C++ class source is declared
byte-exact merely from handler-address agreement; this metadata proof
justifies the handler **address identity** for this owner comparison.

## Narrow affected scope

Only the new constructor and directly affected spell-panel exact owners
were cold-replayed. No full historical cohort or product cold replay is
inferred or required. The original constructor's authorship remains
**origin indeterminate**, even though its maintained source is exact in
this compiler context. `0x00408AF0` destructor remains nonexact. `0x00408C90` factory has since
closed exact under a separate, real Chain compiler graph; see
[factory evidence](../spell-panel-factory-exact-20261103/README.md).
Runtime/game equivalence is still open.
