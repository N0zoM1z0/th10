# Popup-number Draw: complete first models, not accepted

Target: original Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Baseline checkpoint `efee2a8cdd4de5eae417763fbad7b4620f5d0b1a`.
Disk/toolchain/tracking preflights and Factory-native Ghidra attestation passed.

## Result

| Complete model | PDB-owned Draw bytes | Result |
| --- | ---: | --- |
| First source | 484 / target 540 | Structural rejection |
| Explicit iterator, branch-local sprite publication, dx/dy locals | 535 / 540 | Structural rejection |
| Existing hardware-flags global identity | 535 / 540 | Structural rejection; normalized whole body identical to preceding model |

Every native run exited 0, serial, one CPU, nice 10, bounded 300 seconds.
All use the same two TUs, `/GL /GS /EHs`, and genuine fastcall callback entry;
there are no forced ABI, register, padding or noinline alterations.
Both actual callees pass complete canonical contracts in each image:
FlushVertexBuffer **133/all 4**, DrawNoRotation **515/all 29** (648 bytes).
No new exact/source credit, canonical recipe or ledger mutation follows.
The five-byte extent gap is **not** a mismatch-byte count.

The callback is a separate 10-byte target at 42B9B0, forwarding ECX through
EDI to Draw; source authorship remains indeterminate. It is not admitted here.
Draw owns dense 42B780..42B99B, 540 bytes, with 15 link fields, 18 fixed local
branches and four calls: Flush, device-vtable slot E4, ftol2 and DrawNoRotation.
Candidates have all 15 link fields; their local branches number 15/17/17.
`field-bindings.json` verifies every target field, candidate symbol role and
constant payload diagnostically; it does not normalize incompatible local layout
or accept different extents. `native-summary.json` retains whole callee results.

## Target-local source model

Allocation B884 is 3C4 + 723 records of 40 bytes. VM begins at owner18;
loaded pointer10, records3C4. Draw/update traverse **723**, whereas insertion
wraps **720**. Record digits occupy12 bytes, position0C, color18, signed age20,
ageFloat24, active38, unsigned digitCount39. The proposal asserts layout.

Fog checks existing byte `g_AnmHardwareFlags` at491D78 mask0x04 and the full DWORD
`g_StageFogEnabled` at492378. Player is captured after fog setup and refreshed
only after a nonempty digit loop. Distance preserves x87 extended dx/dy squares;
spacing is a float. Alpha is80 below/equal1024,208 above4096, otherwise
`80 + ((distance-1024)<<7)/3072`. The loaded resource, sprite, renderer, age and
digit are read inside the loop; VM x is reread after drawing. No z write occurs.
Three sprite-bank branches publish independently (196,207,217). Existing
hardware flag declaration replaces the first invented alias without code change.
Global storage/runtime alias closure and original source/TU identity remain open.

The revised native model recovers record-start EBP addressing, actual dx/dy
load/order without extra float spills, and three branch-local sprite stores.
Residual scheduling differs: target alpha EBX/digit pointer stack, candidate
alpha stack/digit pointer EBX; spacing stack slot and VM field receiver choices
also differ. Target reads device vtable after clearing fog, candidate before.
No independent evidence currently justifies another source/profile permutation.
Park this representative until a new target-supported contract predicts a change.

## Reproduction and preservation

`proposals/` contains isolated source proposals, not maintained game source.
`reproduce.py --trial identity --output-dir build/...fresh...` cold-links one;
run through `scripts/repo-python`, one CPU/nice10/timeout, recording stdout,
stderr and exit. It refuses an existing output directory. The original ignored
packet is `.analysis/gpt-dots/popup-digit-draw-20261025/`; original linked images
are `build/gpt-dots/popup-digit-draw-{first,model,identity}/`.
`review-original.py` and `bind-original.py` rerun the retained original-image
proofs from the repository root (read-only images, output to ignored packet).
The packet retains all runners, target/candidate disassembly, full inventories,
review/binding logs and exit files. `artifact-hashes.json` freezes originals.

This documentation-only checkpoint runs focused tracking/toolchain/diff checks,
not a new full CI or runtime claim. Source, canonical config, metrics and the nine
protected D3D files are unchanged. The preceding Spt Test full CI is not relabeled.

Independent read-only review passed at07:08 UTC: both proof scripts regenerated
byte-identical JSON under a no-write verification shim; all three callee guard
pairs, all15 field roles, structural rejections and identity neutrality were
confirmed. All63 frozen original artifacts remain unchanged.
