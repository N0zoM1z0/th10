# Sound GetFmtIndexByName: original i386 format-index dword increment

Original Japanese TH10 1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The original `SoundPlayerView::GetFmtIndexByName @0x0043D2A0` occupies
**227 bytes** with four independently target-bound external fields. It
keeps the format index in EBP, advancing each 0x34-byte record with a
**native `INC EBP` at `+0xA8`**. The maintained C++ previously used
signed `++index`, which is undefined at `INT_MAX`, whereas the retail
i386 instruction preserves the low 32-bit word. The source now makes
that four-byte operation explicit:

```cpp
// Retail INC EBP wraps the low 32-bit format-table index.
++*reinterpret_cast<unsigned int *>(&index);
```

This does not alter the delimiter search, basename copy, record-name
comparisons, missing-name fallback, or later `int` result. It is an
original-verified 32-bit VC7.1 C++ semantic correction, not a new bounds
check, original source spelling, portable pointer model or runtime claim.

## Eleven source variants and complete surrounding owners

An independent source-hash-pinned **two-TU current-source**
`Sound.cpp`/`ZWave.cpp` VC7.1 SP1 `/GL /GS /EHsc` graph was cold built.
Eleven different natural C/C++ variants were individually compiled and
linked: unsigned-lvalue increment, unsigned explicit assignment, a
named next-word copy, direct 0x34-byte record addresses individually and
jointly, and combinations with unsigned or register index declarations.
Each variant passed *the complete original target* `GetFmtIndexByName`
**227/227 bytes with 4/4 original field identities**, the original
`SoundPlayerView::ProcessQueues` large owner **1,452/1,452 with 43 fields**,
`ReopenBgm` **55/55**, and retained full original-bound
`PreloadBgm` **365/369** with ten valid external fields. Neither the
2,261 combined exact bytes nor the 365/369 partial parent match was
claimed as newly discovered. The four unmatched PreloadBgm register
bytes remain open.

The chosen single unsigned-lvalue increment was installed and then
**cold replayed independently from its final source** using only five
relevant canonical original-bound units in three genuine supported
native source graphs, including the required separate **five-TU**
`SoundDataLoaderThread` producer for the 227-byte owner:

| Preaccepted exact owner | Original bytes | Original fields |
| --- | ---: | ---: |
| `sound-get-fmt-index-by-name` | 227/227 | 4 |
| `sound-process-queues` | 1,452/1,452 | 43 |
| `sound-player-reopen-bgm` | 55/55 | 2 |
| `streaming-sound-init-buffers` | 338/338 | 6 |
| `sound-bgm-player-thread` | 189/189 | 7 |

**2,261/2,261 exact original bytes, all 62 external fields**, three
real native artifacts. The installed-source `SoundInitBuffers` image
also preserves the original-bound 369-byte `PreloadBgm` at **365/369**
with all ten real fields. `GetFmtIndexByName` was already canonical
exact, so **zero new exact owner/codegen credit** is added. No historical
all-unit cold or full-game executable link was run.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit sound-get-fmt-index-by-name --unit sound-process-queues \
  --unit sound-player-reopen-bgm \
  --unit streaming-sound-init-buffers \
  --unit sound-bgm-player-thread --json
```

Final `src/Sound.cpp` SHA-256:
`ee5f883cb6db7004913ae1a3fd2407d0cdb5f77b7e17c4187c64e864d4a1b22c`.
Real trial sources, full native PE/MAP/PDB and original-bound selected
receipts are retained under ignored
`.analysis/gpt-web/20261107-sound-getfmt-dword/`,
`build/gpt-web/20261107-sound-getfmt-dword/` and
`build/match-linked/{SoundDataLoaderThread,SoundInitBuffers,SoundReopenBgm}/`.
