# TH10 alternate text rasterizer: 20-byte native local stack frame

Target original: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The original `TextHelperView::RenderTextToTextureAlternate @0x00437FE0`
is a complete **509-byte** function. Its 4:4:4 foreground text path
selects one of fifteen GDI fonts, performs the original alpha inversion,
bleed, repeated font restoration and D3DX upload. The original i386
prologue allocates a **0x14-byte** local frame; the previous natural
source compiled to a **0x18-byte** frame, spilling redundant space.

As with the now-accepted 554-byte main rasterizer, the source now
places the late `D3d9RectView sourceRect` and `D3d9SurfaceView *surface`
texture-upload work in a natural local C++ block. No GDI drawing,
component arithmetic, call/return or error handling was changed.

## Actual original-byte and caller controls

A source-pinned VC7.1 SP1 normal-COFF trial compiled the **entire 509-byte
alternate function**. It remains 509 bytes with 32 relocations, but
its local stack allocation improves from `SUB ESP,0x18` to the exact
`SUB ESP,0x14` and the original ordinary-byte score improves from
**185/381** to **193/381** (eight newly matched bytes). The 32 COFF
relocation positions are candidate fields, *not* independently accepted
original target fields. **This function remains NONEXACT.** It still
differs in the EBP/EDI/EBX register lifetime over the glyph-width/font
selection and later text upload, as well as other ordinary code bytes.

Nine further natural C++ width/font alias and declaration variants were
compiled. None improved beyond 193/381; changing the width to a new
local, for example, expanded the candidate to 513 bytes. Those
unsuccessful variants were rejected without modifying maintained source.

The source edit is confined to the alternate body. The same final normal
COFF compilation proves that the newly accepted **554-byte main
`RenderTextToTexture` remains 414/414 ordinary bytes**, with its 35
separately bound original fields. A separate selected real cold replay
of all seven affected **already exact** owners in their correct source
graphs passes **3,030/3,030** full original bytes:

- `text-render-main` **554/554**, all **35/35** original target fields;
- `text-font-release-resources` **149/149**, 18 fields;
- TextBuffer lifecycle `781+421+107+72` = **1,381/1,381**, 50 fields;
- `text-buffer-apply-alpha-bleed` **946/946** in the normal-COFF graph.

The eight additional ordinary matches of the **nonexact** alternate
function do not increase the canonical exact owner or exact byte totals.
They are not whole-executable or runtime-equivalence proof.

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

Private trial sources, original-comparable scores and selected cold receipts:
`.analysis/gpt-web/20261108-text-alternate-stack-probes.py`,
`20261108-text-alternate-width-probes.py`,
`.analysis/gpt-web/20261107-active/text-alternate-stack-results.json`,
`text-alternate-width-results.json`, `text-alt-scope-final-exact.json`,
and object files under `build/gpt-web/20261108-text-alternate-stack/`.
