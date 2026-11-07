# TH10 text alpha: native 32-bit bitmap cursor arithmetic

Original: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The original `TextRenderBufferApplyAlphaBleed @0x00436DA0` is a complete
**946-byte** owner. It scans the 32-bit A8R8G8B8 or 16-bit A4R4G4B4
bitmap as one continuous pixel cursor, testing horizontal and vertical
neighbors before averaging non-transparent components. Native i386 forms
low 32-bit addresses, including when a caller supplies an inconsistent
pitch, height or row count. C++ `++pixel`, `pixel +/- 1` and
`pixel +/- pitch / pixelBytes` expressed pointers outside the declared
object for unchecked offsets.

The maintained natural C++ now uses a typed, forced-inline
`TextNativePixelAt(pixel, delta)` helper: 32-bit base address plus signed
pixel displacement multiplied modulo 2^32 by the typed pixel stride.
It replaces the two row-cursor increments and all eight horizontal and
vertical neighbor accesses, **without** changing the original signed
pitch division, row/width decisions, opacity tests, division and halving
of color components, or loop order. This is target-specific Win32
address behavior, not a buffer bounds fix or a portability guarantee.

The neighboring **157-byte** `TextRenderBufferView::InvertAlpha @0x00437160`
likewise now models its 4-byte/2-byte index increments and GDI byte/word
address accesses with native 32-bit word arithmetic and the same helper.
The A1R5G5B5 alpha-and-color behavior and all return paths remain intact.

## Native experiments and installed source results

Seven independent normal-COFF VC7.1 SP1 source variants separately tested
both pixel increments, left/right A8 neighbors and both vertical directions
for both pixel formats. Each reproduced the original **946/946 code bytes**
with **zero relocation fields**. Four additional full-function trials
replaced raw casts with the natural typed helper, separately and combined;
all stayed 946/946. The current installed source was then freshly cold
replayed in the original-bound canonical compiler graphs:

| Previously exact text owner | Original code bytes |
| --- | ---: |
| `text-buffer-apply-alpha-bleed` | 946/946 |
| `text-create-buffer` | 781/781 |
| `text-try-allocate-buffer` | 421/421 |
| `text-release-buffer` | 107/107 |
| `text-fill-color` | 72/72 |
| **Total** | **2,327/2,327** |

The selected text alpha-bleed owner was tested as an actual complete COFF
function. Four lifecycle owners were validated together in their real
linked two-source `/GL` context, including all independently original-bound
field references. No existing exact owner regressed. `InvertAlpha`
remains **NONEXACT**: its entire 157-byte normal-COFF candidate still
matches **112/157** original bytes, with no relocations; the register
allocation differs. The two larger render-text functions likewise remain
nonexact at 554 and 509 bytes, without an extent regression. This is a
source semantic recovery and exact-regression proof, **zero new exact
codegen ledger credit**. No whole-game or runtime claim.

Reproduce affected units only:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit text-buffer-apply-alpha-bleed --unit text-create-buffer \
  --unit text-try-allocate-buffer --unit text-release-buffer \
  --unit text-fill-color --json
scripts/repo-python scripts/probe-exact-backlog.py \
  --source src/TextRenderer.cpp --json
scripts/repo-python scripts/ci.py
```

Ignored source epochs, full COFF probes and selected exact receipts:
`.analysis/gpt-web/20261107-active/{text-bleed-words-results.json,
text-bleed-template-results.json,text-invert-words-results.json,
text-invert-helper-cleanup-results.json,text-alpha-final-guards.json,
text-alpha-final-backlog.json}`, native object candidates in
`build/gpt-web/20261107-active-text-{bleed,bleed-template,invert-words,
alpha-cleanup}/`. The source hash is verified by the live handoff pins.
