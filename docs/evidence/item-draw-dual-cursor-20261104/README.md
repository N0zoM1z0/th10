# TH10 ItemManager::Draw: reconstruct the dual-cursor loop in 281-byte owner

Pinned original Japanese TH10 v1.00a `0x0041B8E0..0x0041B9F8` is a complete
**281-byte** item renderer (private EAX ItemManager receiver, plain `RET`).
The original loops over 2,198 item records of 0x3F0 bytes each, with
one cursor referring to each row's VM and a second cursor referring to its
Y-coordinate. The target's prologue stores the manager VM cursor in EDI,
and derives its VM-Y cursor in ESI from EDI+0x338; both advance by 0x3F0.

The formerly maintained `ItemManagerView::Draw` source indexed only one
`ItemRecordView*` in its loop; pinned VC7.1 SP1 `/GL /GS /EHs` in the
real three-source `GameCreateItemManager` graph emitted a **296-byte**
complete PDB owner with a different one-pointer loop. The selected source
now tracks both the ItemRecord and typed `AnmFloat3View` VM position,
advancing both by the actual record extent. The compiler naturally recovers
all original prologue instructions through the active test, the same
EDI/ESI/EBP/EBX roles, and the 2,198-loop skeleton without fake assembly,
ABI tricks, padding or intrinsics.

## Target comparison, and rejected hypotheses

The selected actual candidate is **283 bytes** with **14 decoded linked
address fields**. Its first **42 raw bytes** agree with the original,
but the remaining float instruction scheduling, alpha scaling and branches
are still different. It is **NONEXACT**, with zero canonical exact credit.
The complete 281-byte original was not reproduced or truncated.

Real compiler experiments considered `volatile` Y/alpha locals, volatile
source coordinates, pre-compare barriers, inverse addition and alpha
parenthesization, and alternate Y/VM secondary pointers. None produces
an acceptable whole-owner exact result. The volatile source variant
produces 281 bytes but 94 ordinary mismatches; matching the length alone
is insufficient. The selected natural dual-pointer form preserves
semantic field access and is substantially closer to the original loop
without decorating source with artificial `volatile` constraints.

Reproducibility: actual PE/MAP/PDB in
`build/gpt-web/20261104-live-item-draw/dual-position-cursor/`, with
source-hash and geometry in [native-result.json](native-result.json).
The ignored experimental controller/results are
`.analysis/gpt-web/20261104-live/item-draw-281-panel.py` and
`item-draw-281-results.json`. This is source recovery and compiler frontier
progress, not original TU provenance, game runtime, or whole-codegen proof.
