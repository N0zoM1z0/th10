# TH10 Sound: typed RIFF chunk length and WAVE prefix with unchanged native owners

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The original game does not implement generic, padded RIFF chunk traversal.
`SoundPlayerView::LoadSoundSample` reads the container size from the dword
after the four-byte `RIFF` tag, then checks `WAVE` and subtracts twelve
bytes before scanning for `fmt ` and `data` chunks. The independent
`FindWaveChunk` parser reads the raw chunk size at current cursor `+4`
and advances by that size plus eight **without RIFF pad rounding**.

Maintained `Sound.cpp` now expresses these observed wire fragments as
`SoundWaveRootAfterTagView` (container byte count at +0, WAVE tag at +4)
and `SoundRiffChunkPrefixView` (four-character tag at +0, byte length at
+4). Native compile-time assertions prove both eight-byte views and their
field offsets. The real two functions now use typed length fields rather
than independently reinterpreting adjacent `unsigned int*` words. No new
buffer, bounds checks, padding, parsing dependencies or historical source
spelling are inferred; the byte cursor's lifecycle is unchanged.

## Current-source native original-byte verification

A genuine pinned VC7.1 SP1 **five-TU `/GL`** Sound caller graph from
current source, with only `Sound.cpp` freshly compiled and four other
support objects reused after exact source+object SHA-256 checks, preserved
complete PDB owner sizes, candidate field geometry, and *all* ordinary
candidate bytes versus the preceding canonical sound graph:

- `LoadSoundSample`: **684** bytes, 25 candidate fields; original byte
  comparison remains **678/684** after normalizing field sites. Its only
  differences are still `+0x122..+0x127`, the MOV/LEA call argument order;
  **NONEXACT** with no new exact credit.
- `FindWaveChunk`: **71** bytes, one field, ordinary **71/71** target bytes.
- `SoundDataLoaderThread`: **107** bytes, 11 fields, **107/107**.
- `GetFmtIndexByName`: **227** bytes, four fields, **227/227**.

Independently selected **original-target canonical cold replay** under the
installed changed source passed `sound-find-wave-chunk` **71/71**, and the
pair `sound-data-loader-thread` + `sound-get-fmt-index-by-name`
**334/334**. Total 405/405 existing original-bound bytes, **zero new
ledger credit**. These are 4-TU and 5-TU scoped source graphs respectively,
not the full historical replay. The full 684-byte function is separately
tracked as NONEXACT rather than granted partial credit. Seven-stage CI and
`git diff --check` pass; no game product/runtime/portability claim.

Ignored reproducible proof:
`.analysis/gpt-web/20261105-sound-riff-views/verify.py`,
`build/gpt-web/20261105-sound-riff-views/current/proof.json`,
`.analysis/gpt-web/20261105-sound-riff-views/chunk-exact.json` and
`.analysis/gpt-web/20261105-sound-riff-views/loader-format-exact.json`.

Installed `Sound.cpp` SHA-256: `cb5cdabd1732dfbd93d6da73938df42dc541356fe7c34fd20a33950576b5b3b0`.
