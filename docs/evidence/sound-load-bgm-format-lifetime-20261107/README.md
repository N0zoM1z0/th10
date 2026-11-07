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
normal/early return paths, is compared, not an isolated excerpt. At the
source-checkpoint stage this was a *candidate*. Canonical admission is now
**complete**: `sound-load-bgm` binds the reviewed original owner and all
12 target fields in `config/match-units.toml`, the sorted accepted row in
`config/matches.csv`, and the source-inventory status in
`config/functions.csv`.

## Two independent canonical cold replays — newly accepted exact

The correct VC7.1 SP1 `/GL /GS /EHsc` real Sound.cpp + ZWave.cpp
`ProcessQueues` entry graph was **cold compiled twice from current source**.
Both strict normalizers accepted every complete code byte, relocations,
source-to-target referents, addends and call addresses:

| Owner | Cold 1 | Cold 2 | Original link fields |
| --- | ---: | ---: | ---: |
| **LoadBgm** | **298/298** | **298/298** | **12/12** |
| ProcessQueues | 1,452/1,452 | 1,452/1,452 | 43/43 |
| ReopenBgm | 55/55 | 55/55 | 2/2 |
| **Selected total** | **1,805/1,805** | **1,805/1,805** | **57/57** |

The two separate SoundInitBuffers PE SHA-256s are
`ce60a76352529be357f175ecdb7cf3324a8cce7ec60c5fec890f6e89c6115b03`
and `1108e08688915b8496a74389c655ff1d23bd7404b25fc75e3eddce081de8f39d`;
they are **different cold outputs**, not the same image checked twice.
Full PDB extents and all local branches/return paths were required.
The source SHA-256 is
`977faf3e332e03a8d1ef3200a1cfd047147c464b6b9f295d243e4aed5ce2bc02`.

This acceptance contributes **one new authored exact owner and 298 new
original codegen bytes**, raising the canonical totals from **1,498 to
1,499 exact functions** and from **195,967 to 196,265 exact bytes**.
Neither cold compilation nor target code equality establishes original
source spelling, the complete game product or runtime equivalence.
Only the affected Sound graph was replayed, not the full historical cohort.

Focused reproduction:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit sound-load-bgm --unit sound-process-queues \
  --unit sound-player-reopen-bgm --json
scripts/repo-python scripts/progress.py --check
scripts/repo-python scripts/ci.py
```

Private acceptance receipts:
`.analysis/gpt-web/20261107-active/sound-load-bgm-canonical-cold-1.json`
and `sound-load-bgm-canonical-cold-2.json`.

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
