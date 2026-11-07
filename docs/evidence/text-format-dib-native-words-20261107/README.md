# TH10 GDI text buffer: native format row and DIB size words

Original: TH10 Japanese v1.00a (SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`).

The text-buffer format table has 0x18-byte rows. The original i386
`FindTextRenderFormat` accesses an unchecked, sentinel-terminated format
array; source `g_TextRenderFormats[index]` formed abstract C++ array
references outside that array on a missing sentinel. The maintained
natural C++ now uses the previously exact typed `TextNativePixelAt`
helper, forming the full low-32-bit Win32 row address with the native
0x18-byte stride and preserving each sentinel/format comparison and the
special explicit `-1` rejection. It adds no range check.

`TextRenderBufferView::TryAllocateBuffer` also forms several unchecked
Win32 arithmetic words. The source now uses unsigned 32-bit product for
`requestedWidth * bitCount` before the existing signed divisions;
unsigned addition and subtraction for the negative `BITMAPV4HEADER`
height; and unsigned multiplication of row pitch by requested height for
`bV4SizeImage`. The signed intermediate divisions, rounding, extra DIB
row, DirectDraw/GDI calls, failure paths and stored owner fields remain
as before. Native overflow is faithfully represented on 32-bit VC7.1;
this is not a portable or malformed-input safety guarantee.

Six independently built source variants (format row only, row-pitch
product, image-size product, height negation, combined DIB arithmetic,
and combined format+DIB arithmetic) were linked in the real pinned
VC7.1 SP1 `/GL` TextRenderer.cpp+RandomMath.cpp lifecycle graph.
Each reproduced *four full original-target PDB owners*, including all
50 independently original-bound fields:

| Existing exact owner | Original bytes | Original fields |
| --- | ---: | ---: |
| `TryAllocateBuffer` | 421/421 | 7/7 |
| `CreateTextBuffer` | 781/781 | 40/40 |
| `ReleaseBuffer` | 107/107 | 3/3 |
| `FillColor` | 72/72 | 0 |
| **Selected lifecycle subtotal** | **1,381/1,381** | **50/50** |

The actual combined source was then installed with formatting cleanup
and cold replayed from the installed epoch, along with the separate
normal-COFF exact `TextRenderBufferApplyAlphaBleed` 946/946. **Five
units pass 2,327/2,327** original bytes in two correct build contexts.
The existing nonexact `InvertAlpha` and both render-text owners have
not been promoted. No new exact codegen credit, full historical replay,
whole-game executable or runtime proof is claimed.

Focused reproduction:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit text-create-buffer --unit text-try-allocate-buffer \
  --unit text-release-buffer --unit text-fill-color \
  --unit text-buffer-apply-alpha-bleed --json
scripts/repo-python scripts/verify-handoff-pins.py
scripts/repo-python scripts/ci.py
```

Private source trials and selected cold receipt:
`.analysis/gpt-web/20261107-active/text-format-row-native-trials.py`,
`text-format-row-native-results.json`,
`text-format-all-native-selected-replay.json`, plus full PE/PDB/MAP
artifacts under `build/gpt-web/20261107-active-text-format-words/`.
Current installed `src/TextRenderer.cpp` SHA-256:
`1a250e674a282812be4aa7b99934fc1820e38670d1807b0ce9c59182424da9af`.
