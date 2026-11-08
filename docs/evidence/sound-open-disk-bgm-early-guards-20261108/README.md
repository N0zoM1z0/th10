# TH10 OpenDiskBgm: original early-return lifetime and complete 241-byte candidate

Original target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The reviewed original `SoundPlayerView::OpenDiskBgm @0x0043D690` is a
**241-byte** private-EAX sound owner. It copies the disk BGM name, rejects
an absent sound manager or DirectSound device, stops playback, calculates
block-aligned notification size, creates an update event and BGM thread,
and calls `CSoundManager::CreateStreaming`. The maintained source previously
combined both device checks into a short-circuit `manager == NULL ||
directSound == NULL`. Under VC7.1 `/GL`, that selects a shared far exit,
yielding an entire **249-byte** linked candidate. Original i386 instead
returns immediately after the first failed device check and branches to
that early return for the second check.

Replacing the combined expression with two ordinary C++ guards:

```cpp
if (manager == NULL)
    return -1;
if (directSound == NULL)
    return -1;
```

restores the compiler's original early-return control flow. No streaming,
thread, failure or arithmetic semantics were changed; there is no manual
assembly, instruction patch or artificial ABI manipulation.

## Original-bound full-function evidence

Natural-source variants for separate guards, nested guards, negated and
positive guards, and numeric/conditional HRESULT return shapes were
actually compiled and linked against the target in the real VC7.1 SP1
`/GL /GS /EHsc` Sound.cpp + ZWave.cpp `ProcessQueues` graph. The chosen
**separate-guard** form reduced the whole candidate from **249 to 241
bytes** and strictly matched **201/201 nonfield original bytes**.

An independently decoded target relocation manifest additionally checks
all **ten** original `DIR32`/`REL32` fields, not just anonymous field
locations: StopBgm; CreateEventA import; `g_MainSupervisorView+0x48`
window pointer; BgmPlayerThread; CreateThread import; the four exact
GUID_NULL consecutive DWORD addresses and addends; and the
CreateStreaming method call. A strict original-bound linked-PE comparator
against the actual 241-byte function reports **241/241 bytes and 10/10
semantic fields**. This was repeated from the source **after installing**
the natural C++ change.

The installed source was separately cold replayed for seven previously
accepted Sound units across four proper native build contexts:

| Existing exact owner | Original bytes |
| --- | ---: |
| sound-load-bgm | 298/298 |
| sound-process-queues | 1,452/1,452 |
| sound-player-reopen-bgm | 55/55 |
| streaming-sound-init-buffers | 338/338 |
| sound-bgm-player-thread | 189/189 |
| sound-find-wave-chunk | 71/71 |
| sound-data-loader-thread | 107/107 |
| **Selected total** | **2,510/2,510** |

## Canonical exact acceptance — two independent current-source colds

The full 241-byte owner is now registered as `sound-open-disk-bgm` in
`config/match-units.toml`, `config/matches.csv` and `config/functions.csv`.
The ten original-bound fields include exact identities, original targets
and GUID_NULL member addends, not anonymous masked dwords. All ordinary
201/201 original bytes and 10/10 original fields pass strict linked-PE
comparison in the genuine Sound+ZWave ProcessQueues graph.

Two **separate** complete VC7.1 SP1 `/GL /GS /EHsc` canonical cold replays
were performed against the installed, committed Sound source:

| Canonical cold | OpenDiskBgm | Other already exact Sound | Total |
| --- | ---: | ---: | ---: |
| Cold 1, three units | **241/241, all 10 fields** | LoadBgm 298 + ProcessQueues 1,452 | **1,991/1,991** |
| Cold 2, two units | **241/241, all 10 fields** | LoadBgm 298 | **539/539** |

The two source-identical cold PEs have distinct SHA-256 identities:
`c9f7d4c648c469575d8de82b7e5df86b03fe477518cfd966a8bb2c0cd42ef180`
and `dc15db6708cdd493c2482cee3a82d9e872c688751828bc16c5ff26e7d707f63a`.
The original-target unit includes **all ten target referents** and
independent PDB contribution boundaries. This adds **+1 authored exact
function and +241 original exact code bytes**, with no double-counting of
existing Sound owners. The seven previously accepted Sound cold guards
still pass their independent **2,510/2,510** source-checkpoint replay.
No historical full cold replay, complete executable or runtime-equivalence
claim is made. This establishes codegen equivalence, not original human
source spelling or original physical translation-unit ownership.

Focused canonical acceptance and prior guard reproduction:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit sound-open-disk-bgm --unit sound-load-bgm \
  --unit sound-process-queues --json
scripts/repo-python scripts/replay-exact-units.py \
  --unit sound-load-bgm --unit sound-process-queues \
  --unit sound-player-reopen-bgm --unit streaming-sound-init-buffers \
  --unit sound-bgm-player-thread --unit sound-find-wave-chunk \
  --unit sound-data-loader-thread --json
scripts/repo-python scripts/verify-handoff-pins.py
scripts/repo-python scripts/ci.py
```

Private trial, original reference field manifest and full PE/PDB/MAP:
`.analysis/gpt-web/20261108-sound-open-disk-control.py`,
`.analysis/gpt-web/20261107-active/sound-open-disk-control-results.json`,
`sound-open-disk-original-strict-probe.json`,
`sound-open-disk-installed-strict.json`,
`.analysis/gpt-web/20261108-open-disk-source-guards.json`, and
`build/gpt-web/20261108-sound-open-disk-control/`.
Installed Sound source SHA-256:
`5c66f60e5b6a7e6d8a5e7e5e103bdbd742e981fbc5dd72917ac5457d75cce22b`.

Independent canonical acceptance receipts:
`.analysis/gpt-web/20261108-sound-disk-canonical-cold-1.json`,
`20261108-sound-disk-canonical-cold-2.json`, with private target-reference
field proof at `.analysis/gpt-web/20261107-active/sound-open-disk-original-strict-probe.json`.
