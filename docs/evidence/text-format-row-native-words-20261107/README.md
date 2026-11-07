# TH10 GDI text format table and DIB geometry: native Win32 words

Original: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The current `TextRenderer.cpp` has a sentineled array of
`TextRenderFormatInfo` records, each exactly **0x18 bytes**. The helper
`FindTextRenderFormat` searches until it finds a matching format or the
`-1` sentinel. Retail i386 computes the record address by unsigned
32-bit base plus scaled index, even if the index would leave the C++
array when format metadata is malformed. Source `g_TextRenderFormats[index]`
formed abstract out-of-array pointers; the maintained source now uses
`TextNativePixelAt(&g_TextRenderFormats[0], static_cast<int>(index))`,
reusing the typed native low-word helper already established for the
946-byte alpha-bleed body. The `-1` request rejection and sentinel
return path remain unchanged.

The complete **421-byte** `TextRenderBufferView::TryAllocateBuffer`
owner allocates and fills a `BITMAPV4HEADER` with source width, height,
line pitch and image bytes. Its signed C++ `requestedWidth * bitCount`,
`-(requestedHeight + 1)` and `rowPitch * requestedHeight` could overflow,
whereas the target's i386 arithmetic wraps the low 32-bit word. The
maintained source uses unsigned-word multiplication/negative publication
for these three expressions and **keeps the original signed divisions
by eight and four**, original DIB row padding, initialization order,
DIB masks, API calls and return branches. No extra bounds check,
allocation-failure behavior, or portable 64-bit guarantee is introduced.

## Six actual original-bound C++ codegen trials

The real VC7.1 SP1 `/GL` two-TU `TextRenderer.cpp + RandomMath.cpp`
GDI lifecycle graph was cold built from current source. Six separate
natural C++ variants were recompiled and fully linked: table indexing,
row-pitch product, image-size product, height negation, all three DIB
words, and the combined table-plus-DIB source. **All six** reproduced
the complete original-target bytes and all linked field positions,
identities, addends and widths of these four owners:

| Existing exact owner | Full original code | Target-bound fields |
| --- | ---: | ---: |
| `TextRenderBufferView::TryAllocateBuffer` | **421/421** | 7/7 |
| `TextHelperView::CreateTextBuffer` | **781/781** | 40/40 |
| `TextRenderBufferView::ReleaseBuffer` | **107/107** | 3/3 |
| `TextRenderBufferView::FillColor` | **72/72** | 0 |

The selected combined source was installed with readable C++ formatting
and then *separately* cold replayed in real original-bound artifacts.
The exact **946-byte** alpha-bleed owner was included as an additional
whole-function guard. Five affected exact owners pass **2,327/2,327
original bytes** in two distinct producer artifacts. Three nonexact
functions in the same source were independently normal-COFF compiled;
their current results do **not** regress:

- `InvertAlpha`: target/candidate both 157, **112/157** ordinary bytes;
- `RenderTextToTexture`: target/candidate both 554, **390/414** comparable;
- `RenderTextToTextureAlternate`: target/candidate both 509,
  **185/381** comparable.

This is a target-specific source semantics recovery, not new exact-owner
credit. The large 781-byte creation owner and 946-byte pixel-bleed owner
were already exact and stay so; the three nonexact bodies remain open.
No whole-game build, historical full replay, runtime or sanitizer
assumption is made.

Reproduce the affected accepted owners only:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit text-create-buffer --unit text-try-allocate-buffer \
  --unit text-release-buffer --unit text-fill-color \
  --unit text-buffer-apply-alpha-bleed --json
scripts/repo-python scripts/probe-exact-backlog.py \
  --source src/TextRenderer.cpp --json
scripts/repo-python scripts/verify-handoff-pins.py
scripts/repo-python scripts/ci.py
```

Ignored trial script, all six native PE/MAP/PDB and selected receipt:
`.analysis/gpt-web/20261107-active/text-format-row-native-trials.py`,
`text-format-row-native-results.json`,
`text-format-all-native-selected-replay.json`,
`text-format-final-nonexact.json` and
`build/gpt-web/20261107-active-text-format-words/`.
Installed `src/TextRenderer.cpp` SHA-256:
`1a250e674a282812be4aa7b99934fc1820e38670d1807b0ce9c59182424da9af`.
