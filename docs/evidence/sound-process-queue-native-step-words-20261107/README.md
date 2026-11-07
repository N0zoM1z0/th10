# TH10 sound ProcessQueues: native command-stage counter words

Target: original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`SoundPlayerView::ProcessQueues` has a complete 1,452-byte retail owner.
Its `SoundPlayerCommand::step` is a signed 32-bit word at command+8. The
retail instruction stream increments/decrements that word with i386
`INC`/`DEC` (including an observable in-memory `DEC` and `INC` for BGM
release retry), even at the signed extremes. Source `++command->step`
/ `--command->step` invoked undefined signed overflow. The maintained
source now uses unsigned native 32-bit addition/subtraction and stores
back to the same signed field for the **four increments** (PRELOAD,
LOAD, RELEASE and STOP paths) and one RELEASE retry decrement.

All original command steps, early exits, removed-command shifting,
positive and negative command arguments, threshold comparisons, and
thread/DirectSound calls are untouched. This restores the particular
Win32 arithmetic; it neither adds bounds protection nor proves game
runtime safety on malformed command state.

## Complete original-bound compilation evidence

Pinned VC7.1 SP1 `/GL /GS /EHsc`, real 2-TU `Sound.cpp` plus `ZWave.cpp`
graph with `ProcessQueues` as entry. Six separate C++ variants compiled
and linked, each compared against **every byte** of the 1,452-byte
original, not merely a disassembly snippet: PRELOAD/LOAD/RELEASE/STOP
increment individually, RELEASE decrement individually, and all five
in combination. Each yielded **1,452/1,452** code bytes and **43/43**
original-target bound fields; no branch or PDB-contribution offset
regressed. The combined source is installed.

A separate selected cold rebuild from the installed source verified
all affected existing exact Sound owners in their proper graphs:

| Existing exact unit | Original code bytes |
| --- | ---: |
| sound-process-queues | 1,452/1,452 |
| streaming-sound-init-buffers | 338/338 |
| sound-bgm-player-thread | 189/189 |
| sound-player-reopen-bgm | 55/55 |
| **Total** | **2,034/2,034** |

All 58 declared original link obligations remain correct (43+6+7+2).
This is source-semantic reconstruction with **no newly exact owner or
codegen-credit increase**, not a whole TH10 executable or runtime proof.

Reproduce only these two affected build contexts:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit sound-process-queues --unit streaming-sound-init-buffers \
  --unit sound-bgm-player-thread --unit sound-player-reopen-bgm --json
scripts/repo-python scripts/verify-handoff-pins.py
scripts/repo-python scripts/ci.py
```

Ignored trial runner/output and full PE/MAP/PDB:
`.analysis/gpt-web/20261107-active/{sound-process-stage-word-probe.py,
sound-stage-word-results.json,sound-stage-selected-replay.json}` and
`build/gpt-web/20261107-active-sound-stage/`.
