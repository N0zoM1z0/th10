# Sound GetFmtIndexByName: original 0x34-byte BGM record address words

Original Japanese TH10 1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The original complete `SoundPlayerView::GetFmtIndexByName @0x0043D2A0`
(227 bytes) repeatedly advances its BGM-format name lookup through
**0x34-byte records**. The actual target x86 uses `IMUL EAX,EAX,0x34`
followed by table-base address addition in the loop, and another `IMUL
ECX,ECX,0x34` in the final missing-name check. The layout is verified
by `sizeof(ThBgmFormat)==0x34`, including all row members and the
original basename buffer. An unchecked signed index can overflow C++
array pointer arithmetic even though i386 always computes a low 32-bit
address word.

The maintained `src/Sound.cpp` now uses explicit native unsigned 32-bit
address arithmetic for **all three** accesses to `bgmFormats[index].name`:

```cpp
reinterpret_cast<ThBgmFormat *>(
    reinterpret_cast<unsigned int>(bgmFormats) +
    static_cast<unsigned int>(index) *
    static_cast<unsigned int>(sizeof(ThBgmFormat)))->name
```

The previously committed native `INC EBP` unsigned-lvalue index wrap is
retained. All three original use sites, path/filename selection, `strcmp`
call behavior, zero fallback and signed `int` result remain in their
original order. No added table bounds check, allocation, script value,
new struct member or ABI convention is implied. This is intentionally
pinned to the original VC7.1 i386 low-word semantics, not portable
host-pointer behavior or original C++ source spelling.

## Compiled complete-owner evidence

Eleven real source variants were compiled in a freshly built current
`Sound.cpp`/`ZWave.cpp` two-TU `/GL /GS /EHsc` graph, including direct
single/combined native address sums, signed and unsigned index variants,
and explicit wrapping increments. All eleven preserved complete
`GetFmtIndexByName 227/227` original bytes and four original fields,
`ProcessQueues 1,452/1,452` (43 original fields), `ReopenBgm 55/55`,
and nonexact `PreloadBgm 365/369` with all ten original fields. This
second change is a different native source correction from the separately
committed index increment.

The installed final source was **independently selected for cold replay**
in three genuine original-bound native artifacts, using the correct
five-TU SoundDataLoaderThread producer for GetFmtIndexByName and the
native SoundInitBuffers and SoundReopenBgm producers for other functions:

| Existing exact owner | Full original bytes | Original fields |
| --- | ---: | ---: |
| `sound-get-fmt-index-by-name` | 227/227 | 4 |
| `sound-process-queues` | 1,452/1,452 | 43 |
| `sound-player-reopen-bgm` | 55/55 | 2 |
| `streaming-sound-init-buffers` | 338/338 | 6 |
| `sound-bgm-player-thread` | 189/189 | 7 |

**2,261/2,261 original bytes** and all **62** declared original fields,
with no historical all-unit cold replay. A separate original-target
complete 369-byte PreloadBgm check of the final SoundInitBuffers image
passes 365 ordinary bytes and all ten real fields; its remaining four
register bytes are still **NONEXACT**. No new canonical exact function
or partial byte credit is claimed.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit sound-get-fmt-index-by-name --unit sound-process-queues \
  --unit sound-player-reopen-bgm \
  --unit streaming-sound-init-buffers \
  --unit sound-bgm-player-thread --json
```

Installed `src/Sound.cpp` SHA-256:
`bd73bb4a184fd3151b1b36513f96d11e229a91fd8d6205eb76884a0c1e9cd0a9`.
Whole-source native variant results and actual PE/MAP/PDB are under
ignored `.analysis/gpt-web/20261107-sound-getfmt-dword/`,
`build/gpt-web/20261107-sound-getfmt-dword/`, and installed selected
`build/match-linked/{SoundDataLoaderThread,SoundInitBuffers,SoundReopenBgm}/`.
No whole-game rebuild, runtime equivalence, or arbitrary C++ overflow
behavior is inferred from this compiler-original evidence.
