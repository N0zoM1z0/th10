# Typed ASCII ANM ownership across Popup Draw, Initialize and creation

Original target: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The reviewed Popup owner stores the loaded ASCII animation resource at
`ScriptViewerPopupOwnerView + 0x10`: `Initialize()` copies it from
`g_AsciiManagerView->asciiAnm`, while all three sprite-selection paths in
`Draw()` access `AnmLoadedView::sprites` from the same saved pointer.
The maintained field is now an `AnmLoadedView *loadedDigitAnm` rather than an
untyped integer. A native `offsetof` check fixes its +0x10 location; the
0xB884-byte outer owner layout, unconstructed 723-record storage and its
0x18-placed VM remain unchanged. The independent Bomb owner still has its
own unrelated +0x10 field; this is **not** a wholesale offset renaming.

The real pinned VC7.1 /GL four-source Popup creation/update link graph
was rebuilt from these changed sources and compared against the matching
previous source epoch with original file SHA pins. Complete bodies:

- Popup Draw: candidate 537 vs original 540 bytes, 15 linked fields;
  **0 ordinary candidate-to-candidate differences** with equal field geometry.
- Popup Initialize: candidate 141 vs original 145 bytes, 11 fields;
  unchanged ordinary bytes and field positions.
- Popup factory `GameCreateOwner42B660`: **71-byte** candidate versus
  71-byte original but still NONEXACT, five unchanged field positions and
  zero candidate differences.
- Popup Draw callback: 10-byte candidate vs 10-byte original, one unchanged
  field and no candidate differences. This does **not** fix the three original
  target byte mismatches from the preceding evidence.

This source recovery does not make the four functions target-exact.
The separately exact **constructor 134/134**, **Update 134/134**, and
**Update callback 7/7** were independently cold-replayed under the relevant
canonical unit selectors and remain exact: 275/275. Seven-stage lightweight
CI also passed. No full-replay, product closure or runtime claim.

Reproduction: ignored local artifact `build/gpt-web/20261105-popup/typed-digit-anm/`
and the source-pinned comparison in `native-result.json`.
