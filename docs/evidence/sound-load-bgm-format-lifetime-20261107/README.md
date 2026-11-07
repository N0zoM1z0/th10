# TH10 LoadBgm: reload format pointer and recover EBP block alignment

Original target: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The target `SoundPlayerView::LoadBgm @0x0043D950` is a complete **298-byte**
function. Its unmodified source in the actual Sound.cpp + ZWave.cpp
VC7.1 SP1 `/GL /GS /EHsc` graph emitted **304 bytes**.

## Source recovery

Target disassembly loads the format's sample rate and 16-bit block
alignment into EDI and **EBP** near `+0x5A` and later divides `EDX:EAX`
by `EBP` (`DIV EBP` at `+0xB5`). The target **reloads the format pointer**
from the 16-slot preload-format table at `+0xEB` immediately before the
CreateStreamingFromMemory call. The old C++ instead kept `format` live
from the first read to that final call, forcing VC7.1 to preserve the
pointer in EBP and spill the block-align word to the stack (`DIV
[ESP+0x10]`). This also added a leading PUSH ECX and several extra
instructions, making the function 304 bytes rather than 298.

The installed C++ now passes `*SoundWordSlot(&bgmPreloadFormats[0], index)`
directly to the streaming creator. The earlier typed format pointer is
still used for computing notify size. This natural source expresses the
original *second* format-table load and leaves EBP available for its
original scalar lifetime. It introduces no fake ABI, inline assembly,
padding, invalid bounds checks, target byte constants or removal of
source functionality. The unchecked Win32 table index retains its native
32-bit behavior.

## Complete original-bound comparison

Four natural pointer-reload variants (direct typed slot, indexed array,
a pointer copy and a const local) were compiled in the real two-TU graph.
The direct typed slot and several equivalent variants reproduced the
**entire original 298-byte function**. The installed source has a fresh
independently rebuilt PE/MAP/PDB and a strict original-bound comparison:
**298/298 matched bytes; all 12 complete field positions, identities,
addends and widths; zero ordinary or field differences**. In particular:

- Supervisor fields bind to `0x00491C28` with addends `0x13B`, `0x150`
  and `0x48` (original dereferences `0x491D63`, `0x491D78`, `0x491C70`)
- `ReopenBgm` call binds `0x0043D790`; BgmPlayerThread callback binds
  `0x0043E3A0`; streaming creator binds `0x0044CBF0`
- Win32 import slots `CreateEventA` and `CreateThread` bind
  `0x00466120` and `0x00466118` respectively
- Four source GUID_NULL dwords bind to original base `0x0046A67C`
  with addends `0`, `4`, `8` and `12`

The complete original code, including conditional branches and both
normal/early return paths, is compared, not an isolated excerpt. This
is a strong *new exact-owner candidate*. Canonical ledger admission and
independent cold double-replay are tracked separately from the source
checkpoint; no ledger byte credit is claimed merely from this document.

Neighbor regression from the installed source passed five prior exact
owners **2,261/2,261 target bytes** (`ReopenBgm`, `ProcessQueues`,
`InitSoundBuffers`, `BgmPlayerThread` and `GetFmtIndexByName`). The nonexact
369-byte `PreloadBgm` remains at **365/369**, ten old target bindings.

Private reproducible diagnostics:
`.analysis/gpt-web/20261107-active/sound-loadbgm-format-lifetime.py`,
`sound-load-format-lifetime-results.json`,
`prove-load-bgm-installed-fields.py`,
`sound-load-bgm-298-installed-diagnostic.json`,
`sound-load-bgm-preledger-guards.json`; native two-input PE/MAP/PDB under
`build/gpt-web/20261107-active-load-format-life/` and
`build/match-linked/SoundInitBuffers/`.
