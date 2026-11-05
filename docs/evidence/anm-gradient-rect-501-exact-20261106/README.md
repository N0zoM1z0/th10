# Newly admitted TH10 gradient rectangle: complete 501-byte original-bound exact owner

Target identity: original Japanese TH10 v1.00a at `resources/th10.exe`,
SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Original `0x0043BFA0..0x0043C194` is a *complete 501-byte* PDB/function
owner (`RET 16`), followed by unrelated padding. The source is maintained
in `src/AnmRectangle.cpp` as `AnmFloatRectView::DrawGradient`, with four
separate stack color arguments and a 16-byte rectangle view. This is a
source-authored exact native owner; the *historical* original C++ identifier,
original physical TU and direct caller identity remain unproven.

## The previously wrong native-codegen assumption

The older [gradient source reconstruction](../gradient-rect-source-20261103/README.md)
correctly identified target behavior but tested a one-function, isolated
compile whose PDB owner was **503** bytes, beginning with an ordinary member
receiver copy. That result is specific to the isolated source epoch and
**does not describe the current real integrated LTCG producer**. A genuine
14-input GUI/ANM `/GL` graph naturally holds the rectangle in EDI and
emits the complete **501-byte** retail function without artificial entry
ABI, forced register binding, target-byte copying or padding. The original
uses EDI after `FlushVertexBuffer` and finishes with `RET 16`; both are
actual native whole-function observations.

## Independently bound *complete* original-target proof

The target's entire PDB-owned 501-byte body was compared by the strict
linked-PE comparator, including its control flow and every non-relocation
instruction. The **14 required linked-address obligations** were manually
read from the pinned original and independently matched to native PDB/MAP
symbols: ANM render manager `g_AnmRenderManagerView @0x00491C10` at +0x06
and +0x177; real `FlushVertexBuffer @0x00442F50` REL32 at +0x0B; and
the Direct3D device global `g_Direct3DDevice @0x00491C30` at +0xCC,
+0x105, +0x118, +0x12A, +0x13C, +0x14E, +0x15E, +0x1A7, +0x1B6,
+0x1C9, +0x1DC. All field widths/kinds, symbol identities, addresses and
**501/501 original bytes** pass. This is **not** 501 masked candidate bytes
without target identity checks.

The *first* complete source/PDB proof used the existing pinned
`build/match-linked/GuiTransitionEffectFactory/` 14-TU graph. Its PE digest
was `2619e6129297441a1cdf9c3d07ededcfd9f60a0ac3334111099414ee388435db`.
The original-bound first comparator and the preserved initial PE/MAP/PDB
witness live under `.analysis/gpt-web/20261106-gradient-exact/`.

Then the **separately registered canonical**
`[units.anm-float-rect-draw-gradient]` recipe rebuilt the *actual installed
current source* from scratch in the **same coherent 14-TU group** with
pinned VC7.1 SP1 `/GL`; `scripts/repo-python scripts/replay-exact-units.py
--unit anm-float-rect-draw-gradient --json` selected **only this new unit**.
The independent complete-owner result is **exact, 501/501, 14/14
original-bound fields**. Fresh PE digest:
`424c6e6a65dbd55066fb7f31bc30ff0ad6123653073f4f8536b22f9c1dbfeaf5`,
not the first artifact's digest. Canonical receipt is private
`.analysis/gpt-web/20261106-gradient-exact/canonical-replay.json`;
canonical PDB/MAP/PE are at `build/match-linked/GuiTransitionEffectFactory/`.
The previous isolated 503-byte source experiment is superseded for this
unit, not deleted or retroactively declared exact.

**Newly accepted canonical credit: one complete authored owner, 501 original
bytes.** This is not product completeness, original source identity,
private-caller ABI certainty, runtime testing or portability. Historical
all-unit cold replay was *not* executed.

Current `src/AnmRectangle.cpp` SHA-256: `df7eac9259927692fe08ceee227f23d897849ad98bd4181cfc4e681e25ab2fa5`.
