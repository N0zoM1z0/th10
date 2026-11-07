# TH10 RIFF chunk scanner: native 32-bit byte-stream cursors

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
`FindWaveChunk @0x0043D250` is an **already exact** 71-byte function;
`SoundPlayerView::LoadSoundSample @0x0043D390` is a **nonexact** complete
684-byte larger caller.

`FindWaveChunk` consumes the untrusted RIFF chunk byte length directly,
without padding adjustment or clamping. Its original x86 `LEA` advances
its buffer cursor by a modulo-32-bit word even for malformed lengths that
point beyond the input allocation. `data += *chunkSize + 8` in C++ invokes
undefined out-of-array pointer arithmetic in such a case. The return
value's `data + 8` can have the same problem on corrupt input. The installed
source now describes both operations as native Win32 unsigned address-word
addition, using `sizeof(SoundRiffChunkPrefixView) == 8` for the original
header stride. The original `*chunkSize` publication, signed-independent
unsigned `remaining` subtraction, `strncmp` call, and control flow stay
unchanged; **no bounds check or safety guarantee has been added**.

## Full native experiments and acceptance scope

A fresh **current-source** five-TU `SoundDataLoaderThread` graph was cold
compiled before editing. The complete `LoadSoundSample` PDB owner had 684
bytes, 25 candidate address fields and only six non-field differences at
`+0x122..+0x127`: the original `MOV EAX, EBP` precedes `LEA EDI,[ESP+30]`,
where the candidate reverses them. Nine natural caller-argument/lifetime
forms were compiled separately against the original, and none changed the
six mismatches. All temporary caller source modifications were restored;
none are claimed as exact.

Two independent RIFF cursor replacements were then tested, plus the
combined source and two alternative named-stride forms. The installed
**combined** word-pointer form preserved the scanner's full **71/71**
original bytes and genuine `strncmp` REL32 bound at `+0x1A`. Its same-image
684-byte parent remained **578/584 comparable ordinary bytes**, 25 candidate
fields, exactly the **same six** differences. One named-stride trial suffered
a Wine linker page fault and another failed; neither is treated as passing.

The actual **installed source** was then cold replayed with only four
affected canonical exact owners, not the full cohort:

| Previously exact selected unit | Original bytes |
| --- | ---: |
| `sound-find-wave-chunk` | 71 |
| `sound-data-loader-thread` | 107 |
| `sound-process-queues` | 1,452 |
| `sound-bgm-player-thread` | 189 |
| **Total** | **1,819/1,819** |

All four selected owners pass with their original target linkage fields.
The installed-source five-TU parent was separately measured and still has
684 physical PDB bytes and the exact same six ordinary differences. There
is **no new canonical exact function**; the parent is still nonexact.

Private reproducibility: `.analysis/gpt-web/20261107-turn2/` contains
`sound-sample-schedule.py`, `sound-wave-cursor-trials.py`,
`wave-installed-exact-guards.json`, and `wave-installed-parent.json`;
compiled PE/MAP/PDB trial artifacts live below `build/gpt-web/20261107-
turn2-wave-cursor/`. The original EXE remains read-only. Neither a full
product build nor runtime equivalence is established.
