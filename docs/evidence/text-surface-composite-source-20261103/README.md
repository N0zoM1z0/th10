# TH10 645-byte GDI/text bitmap to D3D surface compositor — source recovery

Original target TH10 Japanese 1.00a body: `0x00437750..0x004379D4`, **645**
complete bytes, CC after `RET 12`. The source was missing. The physical
entry is a reviewed **origin-indeterminate** boundary with no independent
IDA entry xref; original historical TU, naming and native caller are not
established. The new `src/TextSurfaceComposite.cpp` and declaration in
`src/TextRenderer.hpp` provide a complete, type-checked *semantic source*
owner under descriptive name `BlendTextBitmapToSurface`.

## Native entry/lock/format behavior

The original receives an unusual compiler-private combination of registers
and stack words: EAX destination row, EDX `TextRenderBufferView *`, ECX
column, followed by the genuine `D3d9SurfaceView *`, pixel width and row
count on the stack; the physical function returns with `RET 0x0C`.
The source's ordinary `__fastcall` signature conveys each necessary value,
but has one extra stack word because it cannot prove the original EAX
transport. It **does not** attempt to fake that transport using inline
assembly or altered target bytes, and no exact ABI claim is made.

If the buffer's real HBITMAP at +0x11C is null, the function returns false.
Otherwise it invokes the actual D3D9 surface `GetDesc` through vtable +0x30,
constructs a RECT `(0,0, buffer.width, buffer.height)` using +0x104/+0x108,
and calls `LockRect` at +0x34 with flags zero. Any nonzero HRESULT returns
false. On successful locking it compares the surface format with the
buffer's own +0x100 format, then blends the selected rows from the
buffer pixels at +0x120 and source pitch +0x110 into the locked surface's
bits/pitch. Format mismatch and unsupported formats perform no blending
but still call `UnlockRect` through vtable +0x38 and return true. The
original code does not release the externally supplied surface or bitmap.

## All three native pixel-format branches implemented

- **0x15 (A8R8G8B8):** for each pixel, use source byte +3 as unsigned
  8-bit alpha; for each of the three RGB bytes, compute the target's
  wrap-width integer interpolation `dest + (((src - dest) * alpha) >> 8)`.
  Preserve destination alpha (fourth byte) completely.
- **0x19 (A1R5G5B5):** inspect the sign/high bit of each 16-bit source
  pixel; copy the whole source word **only when** that bit is set. Preserve
  the old destination word otherwise.
- **0x1A (A4R4G4B4):** reproduce the native low-byte high-nibble
  interpolation with a four-bit source alpha, then its *separate* low-nibble
  OR after the high-nibble byte write; the high byte computes a saturated
  alpha nibble from `(originalHigh >> 2) & 0x3C` plus source alpha and a
  shifted interpolation of the other color nibble. This is intentionally
  **not** replaced by idealized four-channel alpha blending; the original
  target's instruction order, integer shifts, saturation and intermediate
  write are the authority.

The loops are conditional on the signed row and pixel counts; both source
and destination cursors use the real byte pitches and back out the consumed
width at each row boundary. They do not blindly assume surfaces and GDI
bitmaps have identical row pitch. This is a complete function description,
not a partial color-format placeholder.

## Pinned compiler controls and nonexact boundary

The first actual pinned VC7.1 SP1 `/GL` compile/link of this complete owner
produced **722 PDB bytes versus 645 native**. Three real source controls
switched from per-row pointer reinitialization to the target-observed
continuous cursor and pitch-correction form: modifying only the 32-bit
path gave 689 bytes; only the 1-bit path gave 736 bytes; modifying all three
paths gave the selected **670 bytes**. The latter maintains actual original
cursor/pitch semantics and reduces size distance by 52 bytes. Its raw
normalization-free ordinary comparison remains **21/645**, indicating
private register allocation, branch layout and parameter ABI are unresolved;
this number is diagnostic and **not** claimed as partial exactness.

The selected source was rebuilt after replacing the editor buffer and the
same complete PDB extent was checked. Reproducible private experiment:
`.analysis/gpt-web/20261103/text-surface-645-probe.py`,
`text-surface-loop-controls.py`, their result JSON/logs and retained
all-row-cursor source. There are no original address fields in this owner
because it calls the surface through a real virtual interface; unproven
original physical entry xrefs remain separate.

The maintained `TextRenderer` source and type layout are reused, but the
original complete product TU/runtime behavior and 645-byte codegen
remain **NONEXACT**. No `config/matches.csv` row or exact byte credit is
claimed. Nearby accepted text-buffer exact units are cold-replayed only in
scope; do not run a full historical cold replay.
