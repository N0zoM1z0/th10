# TH10 BGM format and preloaded-buffer table: native 32-bit indices

Original: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The 55-byte `SoundPlayerView::ReopenBgm @0x0043D790` is already exact in
its true `LoadBgm`-entry Sound/ZWave VC7.1 SP1 `/GL /GS /EHsc` graph.
The original BGM format row selection uses a low-32-bit base-plus-index
stride (`0x34` bytes) after name lookup, not bounded C++ pointer
arithmetic. The maintained source now uses `SoundWordSlot(bgmFormats,
index)` instead of `&bgmFormats[index]`.

The associated 298-byte original `SoundPlayerView::LoadBgm @0x0043D950`
accesses a 16-slot BGM filename table (`0x100` bytes per name) and four
16-slot preloaded-buffer tables (allocation, format, bytes, size) without
adding an ID range guard in the function. Direct C++ `table[index]` can
be undefined for bad indices while original i386 forms an address modulo
2^32. All five table reads now use the typed low-32-bit `SoundWordSlot`
helper, preserving `ReopenBgm`, event/thread creation, format calculation,
manager invocation, return values, and the original signed argument
conditions. No bounds fix or valid-malformed-input claim is implied.

## Actual complete VC7.1 native graph experiments

Seven independent natural C++ variants were compiled in the real two-TU
Sound.cpp + ZWave.cpp ProcessQueues-entry graph: Reopen format index,
LoadBgm filename index, allocations, format pointer, data+size pointers,
all five LoadBgm tables, and the final combined Reopen+LoadBgm source.
Each whole PDB owner was measured. All seven variants passed:

| Function | Original | Current candidate | Evidence |
| --- | ---: | ---: | --- |
| `ReopenBgm` | 55 | 55 | **55/55 target bytes, all 2 original fields** |
| `LoadBgm` | 298 | 304 | Zero ordinary changes vs pinned baseline; 12 candidate fields unchanged |
| `PreloadBgm` | 369 | 369 | Four ordinary differences at +0x121,+0x127,+0x12A,+0x12B; ten original fields retained |
| `ProcessQueues` | 1452 | 1452 | **1452/1452 original, all 43 target fields** |

`LoadBgm` and `PreloadBgm` are still **NONEXACT**. Candidate-vs-candidate
zero differences do not establish original-bound equivalence, and no
newly exact owner is asserted. The `PreloadBgm` count excludes 40 field
operand bytes: **325/329 ordinary** plus ten already validated four-byte
fields = **365/369** overall source/target bytes. The 304 vs 298 byte
`LoadBgm` gap remains open.

After installing and formatting the combined C++ variant, the current
source was independently cold replayed in four actual artifact graphs,
with only six affected existing exact units selected:

| Existing exact owner | Original bytes |
| --- | ---: |
| `sound-player-reopen-bgm` | 55/55 |
| `sound-process-queues` | 1452/1452 |
| `streaming-sound-init-buffers` | 338/338 |
| `sound-bgm-player-thread` | 189/189 |
| `sound-find-wave-chunk` | 71/71 |
| `sound-data-loader-thread` | 107/107 |
| **Total** | **2212/2212** |

All previously declared 70 original-bound fields across these owners
also passed. No full cold historical replay, game executable/runtime
validation or original production-source identity claim.

Reproduce via selected units only:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit sound-player-reopen-bgm --unit sound-process-queues \
  --unit streaming-sound-init-buffers --unit sound-bgm-player-thread \
  --unit sound-find-wave-chunk --unit sound-data-loader-thread --json
scripts/repo-python scripts/verify-handoff-pins.py
scripts/repo-python scripts/ci.py
```

Trial/source receipt: `.analysis/gpt-web/20261107-active/` files
`sound-bgm-index-native-variants.py`, `sound-bgm-index-results.json`,
`sound-bgm-words-installed-replay.json`, `sound-bgm-words-wav-guards.json`;
native PE/MAP/PDB trial graphs retained at
`build/gpt-web/20261107-active-bgm-indices/`.
