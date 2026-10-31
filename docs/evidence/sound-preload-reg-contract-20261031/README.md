# BGM preload: two real profiles and an ABI correction (2026-10-31)

This checks the *complete* `SoundPlayerView::PreloadBgm`, target
`0x0043D7D0..0x0043D940` (369 bytes), without claiming codegen exactness.
The independent TH10 1.00a target SHA-256 is
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

## The callee contract, checked from bytes rather than notes

At `PreloadBgm+0xCD` the target pushes the owner (`ESI`), then at `+0xCE`
it executes **`mov ecx, ebx`**, then at `+0xD0` calls target
`GetFmtIndexByName` at `0x0043D2A0`. The latter begins with a stack-cookie
prologue and `mov esi, ecx` at callee offset `+0xF`. Thus the target path
argument enters in **ECX** and the owner is on the stack. Previous shorthand
calling this an EAX path was inaccurate; a no-`/GS` scratch link does emit
`mov eax, ebx` there, but it is not the correct target private contract.
No forced calling convention is introduced into maintained source.

## Bounded complete-owner experiments

* A one-source `/GL` backlog diagnostic retains the full 369-byte owner,
  121 decoded instructions and 10 address fields, with **7** ordinary-byte
  differences after masking these 10 specific four-byte fields. The extra
  mismatch at `+0xCF` is precisely ECX-versus-EAX above.
* A *fresh* two-source `/GL /GS /EHsc` link with actual `ProcessQueues` entry,
  `Sound.cpp` source and `ZWave.cpp` support retains a 369-byte owner,
  121 instructions and 10 address fields, with **six** non-address byte
  differences. Their offsets are `+0x11B`, `+0x121`, `+0x122`, `+0x127`,
  `+0x12A`, `+0x12B`. All reside in the successful allocation path just before
  `ReadFile`: target loads format table into ECX, requested size into EDX,
  and the output DWORD address into EAX, while the candidate rotates these
  three scratch registers. There is no length or field-count shortcut.
* Eight complete two-source variants were compiled independently, replacing
  `bytesRead` with a one-element DWORD array or single-field struct, introducing
  a named format reference/pointer, using inline ReadFile helpers with either
  argument order, and using named size/output expressions. **Every variant
  retains the same six mismatches.** All temporary source edits were restored.

The target does not check the ReadFile result or bytesRead, and its later
publication takes the requested allocation size from the format record. We
have not converted that observation into an invented successful read or
simplified it to a `memcpy`. Profile/ABI observations are useful without
constituting an accepted reconstruction. Retained diagnostic intermediates
are ignored under `.analysis/gpt-web/sound/` and `build/gpt-web/sound/`.
Original source spelling, physical TU provenance, runtime outcomes and product
closure remain unknown. This does **not** add a canonical exact unit or match.
