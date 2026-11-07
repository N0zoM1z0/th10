# TH10 BGM format / preload slot Win32 native address words

Target is the original TH10 Japanese v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The sound owner's BGM routines use IDs returned by the format-name search
and supplied by audio commands, with no new array bounds checks. C++
`bgmFormats[index]`, `bgmFileNames[index]`, and the different preload
slots formed bounded-array pointers, while retail i386 computes their
low-32-bit typed slot addresses modulo 2^32. The existing typed
`SoundWordSlot` (introduced for the WAV sample loader) now addresses:

- `ReopenBgm`: the 0x34-byte format record returned by name lookup.
- `LoadBgm`: the 256-byte filename slot (for un-preloaded fallback),
  preload allocation slot, format slot, data slot and allocation-size slot.

The signed index, 16-slot storage layout and all stream-buffer allocation,
thread, DirectSound, request-size and error-control behavior are retained.
These expressions recover the native Win32 indexing model; they add no
clamp, runtime safety for invalid IDs, or portable C++ pointer claim.

## Complete two-TU original-codegen comparisons

Seven independent natural source variations were compiled in the real
VC7.1 SP1 `/GL /GS /EHsc` Sound.cpp + ZWave.cpp graph, with
`SoundPlayerView::ProcessQueues` as the entry. Individual format,
filename, allocation, format/data/size and both combined forms were all
compared at their entire PDB contribution extent and field geometry.
The installed combined source was separately selected for a fresh native
replay from the formatted final source epoch.

| Whole owner | Original vs candidate | Field verification / result |
| --- | --- | --- |
| `ReopenBgm` | **55/55 exact** | two original target-bound fields |
| `ProcessQueues` | **1,452/1,452 exact** | 43 original target-bound fields |
| `PreloadBgm` | 369 / 369, **365/369** original bytes including ten original-bound fields | four remaining ordinary differences at `+0x121,+0x127,+0x12A,+0x12B` |
| `LoadBgm` | original **298** vs candidate **304** bytes | 12 *candidate* fields; zero ordinary or field-geometry changes compared to installed-source baseline; NONEXACT |

The PreloadBgm ordinary-only score is **325/329** (40 original-bound
link-field bytes excluded); it is *not* 365/329. `LoadBgm` is not
measured as exact just because the complete candidate is unchanged.
There is **no new exact-owner codegen credit** for any function.

Selected current-source original-bound cold replay, not a full cohort:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit sound-player-reopen-bgm --unit sound-process-queues \
  --unit streaming-sound-init-buffers --unit sound-bgm-player-thread --json
scripts/repo-python scripts/replay-exact-units.py \
  --unit sound-find-wave-chunk --unit sound-data-loader-thread --json
scripts/repo-python scripts/verify-handoff-pins.py
scripts/repo-python scripts/ci.py
```

The first selected group of four exact units passes **2,034/2,034**
original bytes in two artifacts; the second, related WAV guard, passes
**178/178** in two further artifacts. Selected original-bound linkages
also pass. No full cold replay, whole executable, or runtime equivalence
is claimed.

Ignored trial scripts/receipts/source variants/PDB/MAP/PE:
`.analysis/gpt-web/20261107-active/sound-bgm-index-native-variants.py`,
`sound-bgm-index-results.json`, `sound-bgm-words-installed-replay.json`,
`sound-bgm-words-wav-guards.json` and
`build/gpt-web/20261107-active-bgm-indices/`.
