# TH10 43-byte EnemyLaserDrawList — complete raw-byte exactness under real LTCG callback

Original Japanese TH10 1.00a PE is hash-attested in `config/target.toml`.
The complete target function `0x0041C450..0x0041C47A` has **43 bytes**,
ends in plain `RET`, then has CC padding before the separate `0x0041C480`
update callback. The complete target x86 uses a private **EAX** manager
pointer, reads its `sentinel.next` at +0x18, and iterates the real
`EnemyLaserConstructionBaseView` singly linked list, retaining each +0x8
next pointer before any virtual dispatch. It skips state-1 lasers and calls
the actual vtable `Draw` at +0x0C, then returns 1. It allocates and frees
nothing; no synthetic object, null guard, or shadow Chain manager is added.

The original 43-byte function has **no independently verified physical
incoming E8 caller** in the target. Its historical authored-versus-
compiler-generated/retained origin and original translation-unit name
remain **indeterminate**; byte-exact codegen does not prove either one.
The target manager and node fields are the existing real typed
`EnemyLaserManagerStorageView` and `EnemyLaserConstructionBaseView`.

## Independent actual compiler-context recovery

The first maintained natural `__fastcall` C++ source declaration of
`EnemyLaserDrawList(void *)`, compiled with itself as the VC7.1 `/GL` root,
produced **43/43 bytes in size** but only **40/43** matching: native EAX
manager input was encoded ECX by the isolated candidate, and its virtual
call used candidate EAX rather than native EDX for the vtable. The three
mismatches were **not** masked, patched, or credited exact.

To reconstruct the **real** source-level relationship, the existing
registered `EnemyLaserDrawCallback @0x0041C4E0` now calls the same typed
`EnemyLaserDrawList` when `g_GameManager->m_Flags` permits drawing. The
pinned VC7.1 LTCG optimizer **inlines that call into the 46-byte callback**
while retaining a separate 43-byte `EnemyLaserDrawList` source-owned
contribution. Both resulting instruction sequences match their different
original physical target bodies, naturally selecting the 43-byte function's
private EAX input and EDX vtable. This is an actual C++ call under the
**genuine registered callback entry**, not a fake register shim, naked
assembly, linker patch, copied target code, or direct E8 invented for the
original retail executable.

The canonical unit `enemy-laser-draw-list-0041c450` shares the exact same
primary source, VC7.1 SP1 `/GL` profile, real entry symbol, support set and
cold output directory as the independently accepted 46-byte
`enemy-laser-draw-callback-0041c4e0`. Its PDB isolates the full 43-byte
owner with **zero** relocations/normalization fields. The first and second
zero-reuse cold builds produce distinct PE SHA-256 and PDB GUID identities.
Each full comparator returns **43/43 raw_equal=true, normalization_complete**
with no differences. The same two PEs were separately compared to the
**46/46-byte callback exact**, binding its one original global field to
`g_GameManager @0x00477810`. Thus a real C++ caller transformation did not
silently regress the already accepted exact function.

Private regenerable receipts: `.analysis/gpt-web/20261103/laser-draw-list-cold{1,2}.json`
and `.analysis/gpt-web/20261103/laser-draw-call-inline-cold{1,2}.json`.
The earlier mismatch receipt remains at
`.analysis/gpt-web/20261103/laser-draw-list-43-first.json`.

This change adds exactly **one distinct physical canonical 43-byte function**
to `config/matches.csv`, without double counting the 46-byte callback or
claiming runtime/product equivalence or historical authorship. The
**274-byte** `UpdateLinkedLasers` and **91-byte** update callback remain
maintained source but **NONEXACT**; any progress on them is checked
independently against their full original extents. Only related exact units
are cold-replayed; no full historical/product cold replay is performed.
