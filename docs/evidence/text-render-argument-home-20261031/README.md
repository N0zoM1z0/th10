# Text render upload argument home — 2026-10-31

This is a **nonexact**, target-bound investigation of the **entire**
`TextHelperView::RenderTextToTexture` at `0x00437DB0..0x00437FD9`
(554 bytes). It does not add a match unit or exact credit. The native
single-TU VC7.1 `/O2` normal COFF body is 554 bytes/184 instructions with
35 relocation fields. The target is 554 bytes/184 instructions, too. The
candidate does not become exact from equal length or by ignoring fields.

## Independently established parameter-slot ownership

The target's prologue is `sub esp, 14h; push ebx; mov ebx, [esp+24h]`,
then `push ebp; push esi; push edi`. With the six original `__stdcall`
arguments, `[esp+24h]` at the first load is the incoming *glyphWidth*
argument. The code reads/clamps this value and stores 17 back to the same
parameter home when needed. At target offset `+1DF` it passes
`lea edx, [esp+30h]` to the texture's `GetSurfaceLevel`; after four saved
registers and 14h bytes of locals, `[esp+30h]` is **still glyphWidth's
argument home**. At `+212` the native code reloads `[esp+30h]` to test and
release the surface. The original `glyphWidth` value is the fallback if the
COM call does not overwrite its output pointer. This exact fallback behavior
must not be silently replaced with another parameter or an uninitialized local.

This corrects the older, unconfirmed identification of the destination as the
color parameter. Source names/class/TU ancestry are hypotheses; physical
stack observations and the original target bytes are independently checked.

## Complete native controls (no exact credit)

* Current maintained source: the COFF owner has the same 554-byte extent and
  35 relocations; after masking only these relocated four-byte fields at their
  target/COFF locations, **24 ordinary byte offsets differ**. Every mismatch
  is a stack displacement: 18h native local frame versus 14h target, and
  downstream surface-output storage/parameter offsets.
* Model the output through `&color` with reads from `color`: 554 bytes, 35
  relocations, **three** ordinary displacement mismatches at `+1E2`, `+207`
  and `+215`. This reproduces the frame but selects the **wrong ABI argument
  home**; it is rejected, not retained or credited.
* Model output through `&glyphWidth`: 554 bytes, 35 relocations, but VC7.1
  moves register saves/selection to a different schedule starting at entry.
  This does not match the complete owner. A `&color - 1` spelling likewise
  changes the schedule; neither is adopted. These are diagnostic source
  shapes, not an invitation to fake a stack ABI or force registers.

The original source is restored after the controls. The discriminating open
problem is to retain target's early EBX lifetime **and** the later glyphWidth
stack destination with a natural source model. Any future admission must check
all 554 bytes, every relocation's original target identity, the full branch
inventory, and the exact compiler/source ABI. Do not create a partial unit.

The independent original executable is Japanese TH10 1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Compiler output and disposable experiment sources remain ignored under
`.analysis/gpt-web/text/` and `build/gpt-web/text/`. No linker/product or
runtime semantics are claimed from the stand-alone COFF experiment.
