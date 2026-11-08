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

At this **source checkpoint**, `OpenDiskBgm` has *strong target-bound
exact evidence* but is **not yet admitted to the canonical ledger**.
A separate exact unit and two independent canonical cold replays are
still required for new exact codegen credit. No full historical replay,
whole executable or runtime-equivalence claim is made.

Focused old-exact guard reproduction:

```sh
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
