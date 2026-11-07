# TH10 509-byte alternate text rasterizer: native late-lifetime stack scope

Target: TH10 Japanese v1.00a original executable SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`TextHelperView::RenderTextToTextureAlternate @0x00437FE0` is a complete
**509-byte** original function. The maintained source selects one of fifteen
GDI fonts, fills the render buffer with an alternate foreground treatment,
performs two original alpha-inversion calls and texture upload, then releases
its temporary surface. This function is still **NONEXACT**.

The prior source held `D3d9RectView sourceRect` and
`D3d9SurfaceView *surface` in the same lifetime scope as the font and GDI
locals. A natural C++ block around only the late upload/release creates a
shorter local lifetime, letting VC7.1 SP1 compile the original **0x14-byte
stack frame** instead of 0x18. It does not change the format/width, font
selection, text drawing, surface upload, or error paths.

## Complete native comparison and rejected alternatives

The current source was compiled as a whole VC7.1 SP1 normal-COFF TU with
the actual target function and all of its relocations. The complete
509-byte alternate body remains 509 bytes, preserving all **32** COFF
relocation fields and their positions. Original non-relocation-byte match
improves from **185/381 to 193/381**, a real gain of eight bytes. The
remaining 188 ordinary byte differences chiefly reflect EBP/EDI/EBX
register scheduling and the font/GDI call lifetime; these are not claimed
exact. In the actual current linked text-render-main hybrid build, the
alternate PDB contribution is also 509 bytes with 32 fields and 193/381
original ordinary bytes.

Five initial source alternatives tested late rectangle scoping, initialized
surface, early HDC/font ordering, `register` font and a copied glyph-width
local. Nine further C++ source variations tested width references, aliases,
constness, field type and declaration order. None improved upon the late
rectangle scope; some expanded to 513/515 bytes or substantially regressed
comparability. All losing variants were rejected.

The installed, neatly formatted source was cold replayed through all seven
accepted text-renderer exact owners in their **separately correct graphs**:

| Exact unit group | Original bytes |
| --- | ---: |
| `text-render-main` | **554/554**, 35 original fields |
| Existing six text exact owners (including alpha bleed) | **2,476/2,476** |
| **Total selected cold replay** | **3,030/3,030** |

There is **no new exact codegen ledger credit** for the 509-byte alternate
owner. The previously accepted main rasterizer is protected exactly, and no
historical full cold replay, complete executable or runtime-equivalence claim
is made.

Focused reproduction:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit text-render-main --unit text-font-release-resources \
  --unit text-create-buffer --unit text-try-allocate-buffer \
  --unit text-release-buffer --unit text-fill-color \
  --unit text-buffer-apply-alpha-bleed --json
scripts/repo-python scripts/verify-handoff-pins.py
scripts/repo-python scripts/ci.py
```

Ignored trial evidence: `.analysis/gpt-web/20261107-active/` files
`text-alternate-stack-results.json`, `text-alternate-width-results.json`,
and `text-alternate-scope-exact-current.json`, with retained normal COFF
artifacts under `build/gpt-web/20261108-text-alternate-*`.
Current `src/TextRenderer.cpp` SHA-256:
`f016d9482e8d94015c6dc18ffc1123407edc68e7b4c355b057c6448924d51ddb`.
