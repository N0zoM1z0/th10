# TH10 GDI font/bitmap release: 149/149 exact (2026-11-03)

The complete original Japanese TH10 v1.00a owner at **0x00437D10..0x00437DA4**
contains **149 bytes**. Its maintained source, `TextReleaseFontResources`
in `src/TextFontRelease.cpp`, first invokes the already source-reconstructed
`TextRenderBufferView::ReleaseBuffer` on the real global buffer
**0x00474CB0**, then calls Win32 `DeleteObject` through its original
GDI IAT address **0x00466030** on exactly 15 original font slots.
The slots are individually maintained `g_TextFont17` through `g_TextFont31`
at **0x004918A0..0x00491868**, descending in steps of four bytes.
The routine does not reset those global handles or insert extra null guards.

## Actual binary proof

The natural C++ source and genuine retained `TextRenderer.cpp` and
`RandomMath.cpp` support were compiled with pinned VC7.1 SP1 `/GL` LTCG.
The resulting PDB section contribution is 149 bytes long; all bytes decode
as instructions and **149/149** match the target after binding the original
**18 external fields** (one buffer data address, one `ReleaseBuffer` call,
15 individual fonts and one `DeleteObject` IAT field). The exact caller has
no anonymous target bytes, no forced-register ABI, and no extra placeholder
object implementation.

**Two independent selected cold builds** are both exact, with different PE
SHA-256 digests and PDB GUIDs, as recorded in [native-summary.json](native-summary.json).
The first selected cold also reproduces the genuine downstream
`TextRenderBufferView::ReleaseBuffer` at `0x00436A30` **107/107** with all three
previously bound IAT fields intact, yielding **256/256 selected bytes**.
No full cold replay or product/runtime closure was attempted.

The identity and original source spelling of the C++ function and the
physical original TU are not proven by the codegen match. This source is a
reviewed authored operation because it owns GDI resource destruction and
is reached from the original game teardown path.

To replay only the selected owners:

```sh
scripts/repo-python scripts/verify-target.py
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit text-font-release-resources --unit text-release-buffer --json
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit text-font-release-resources --json
scripts/repo-python scripts/ci.py
git diff --check
```
