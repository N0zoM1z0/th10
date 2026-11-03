# TH10 alternate text rasterizer @ 0x00437FE0 — source recovery

Scope: target Japanese 1.00a, reviewed 509-byte function, 0x00437FE0..0x004381DC.
The source was absent from the code ledger before this checkpoint. The caller
in `AnmRenderManagerView::DrawText` already selected the function on its
alternate-render path; `TextRenderer.hpp` now declares and implements it.

## Native observations (independent of old handoff)

- The function takes the same six stack arguments and returns with `ret 18h`
  as the ordinary renderer at 0x00437DB0. Its 15 font-global reads select
  17..31, with a minimum glyph width of 17.
- It calls `TextRenderBufferView::FillColor` at 0x00436CA0 with the requested
  color **before** font selection, then calls the alpha inverter at 0x00437160.
- There is only **one** `TextOutA`: (x*2, 0), current text color, no shadow
  pass. It restores the saved font twice around the second inversion and
  alpha bleed at 0x00436DA0.
- The 0x400-pixel right clamp, `glyphWidth*2+2` bottom, texture level zero,
  `D3DXLoadSurfaceFromMemory` and conditional release are present.
- These observations are corroborated by direct original PE disassembly and
  a read-only, target-hash-attested Ghidra decompile; the original C++ owner
  and file layout are not established by these observations.

## Scoped validation / open boundary

The local `probe-ltcg-backlog.py --source src/TextRenderer.cpp
--entry src/TextRenderer.cpp=TextHelperView::RenderTextToTextureAlternate`
compiled/linked the new method using pinned VC7.1. The preliminary linked
extent is **575 vs 509** native bytes, 119/393 comparable ordinary bytes and
33 candidate address fields. This is strictly a source-present, NONEXACT
recovery, **not** a canonical comparison, field proof, or exactness claim.
The ordinary text renderer in the same native probe has 554-byte PDB scope,
but is also nonexact (390/414 ordinary comparable bytes). Investigate compiler
inlining/context and original caller scheduling before claiming equivalence.
The transient probe log is retained at
`.analysis/gpt-web/text-alternate-probe-20261103.log` (ignored).
