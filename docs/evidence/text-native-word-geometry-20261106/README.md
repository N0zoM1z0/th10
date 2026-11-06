# Native 32-bit word semantics in text bitmap geometry and alpha row count

Target: original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The target performs its raster geometry and bitmap row-length calculations
using ordinary i386 32-bit `IMUL`, `LEA`, `ADD` and `SUB` operations. These
retain the low 32 bits for extreme signed inputs. The reconstructed source
previously used ordinary signed C++ arithmetic, where overflowing additions
or multiplication are undefined behavior even though the actual original
machine instruction always wraps.

The maintained `src/TextRenderer.cpp` now uses explicitly unsigned 32-bit
arithmetic followed by the original MSVC i386 publication to signed `int`
for the following target-observed operations:

- `TextRenderBufferView::InvertAlpha @0x00437160`: signed row count times
  signed pitch to obtain the byte limit for the three format loops;
- `TextHelperView::RenderTextToTexture @0x00437DB0` (554 bytes): twofold
  glyph row range, shadow/foreground text X, source rectangle width formed
  from right-minus-left, and source rectangle bottom;
- `TextHelperView::RenderTextToTextureAlternate @0x00437FE0` (509 target
  bytes): the same glyph/rectangle scaling and single foreground X.

All existing signed comparisons, clipping (source width >1024), GDI calls,
alpha processing order, font restores, scanline loops, resource ownership,
API failure behavior, and original argument homes remain in place. No
range check, invalid-input repair, 64-bit intermediate or new helper ABI is
invented. The signed conversions are pinned to VC7.1 SP1 on x86; this is
not a general portability or runtime-equivalence proof.

## Native comparison and control limits

Six isolated temporary natural C++ source shapes (row bytes, glyph row
height, crop width/height and text X, then all combined) were separately
compiled using the pinned normal VC7.1 SP1 `/O2` COFF profile. All six
preserved the **entire** original codegen candidate lengths, every COFF
relocation location and every ordinary compared byte. After installing the
combined expression changes, an independent normal-COFF compile in the
selected replay and a full comparison against the previous current-source
baseline yielded:

| Complete source owner | Native size vs original | Relocations | Comparable original bytes | Previous-to-installed codegen |
| --- | --- | ---: | ---: | --- |
| `RenderTextToTexture` | 554 / 554 | 35 | 390/414 | identical |
| `RenderTextToTextureAlternate` | 509 / 509 | 32 | 185/381 | identical |
| `InvertAlpha` | 157 / 157 | 0 | 112/157 | identical |

**These COFF results are NONEXACT.** The ordinary fields are not credited
because they do not establish full target-linked identity. A genuine current
`/GL` source-plus-RandomMath image was also compared against its previous
source-epoch graph with identical profiles: complete `RenderTextToTexture`
554 bytes/all35 field locations, alternate renderer 575 PDB bytes/all33
field locations, and inverter157/no fields; **all three ordinary PDB bodies
remain candidate-vs-candidate byte-identical**. Alternate is not target-sized
in this LTCG context. No original-target equality is claimed for the
alternate renderer.

The independent *current installed source* two-TU `/GL /GS` verifier
recompiled the alpha inverter and measured **138/157** raw original bytes
with the original `JS` opcode at +0x69. Its remaining19 ordinary differences
are not hidden. It remains **NONEXACT**, not the older mismatched source-epoch
156/157 anecdote.

Affected existing exact units were selected only, with no all-unit cold replay:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit text-buffer-apply-alpha-bleed \
  --unit text-create-buffer --unit text-fill-color --json
```

All three retain **1,799/1,799 exact original bytes**, including all40
`text-create-buffer` external fields. **Zero new canonical exact credit**;
original author/TU and the runnable game product remain unproven.

Reproducible ignored receipts:
`.analysis/gpt-web/20261106-chat-th10-text-exact/word-geometry.py`,
`word-geometry-results.json`, `final-coff.json`, `linked-preservation.json`,
`selected.json` and `installed-GS.log`; source profile and compiler artifacts
under `build/match/TextRenderer.obj`,
`build/match-linked/TextBufferLifecycle/` and
`build/gpt-web/20261106-text-current-exact/O2-GS/`.
Installed source SHA-256:
`2e95a09f2ea5f0aef8fe3b22f487fce776ea4ae5a918606f464ebf6a4e2c0cc4`.
