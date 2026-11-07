# Sound ProcessQueues: native 0x10C-byte command shift cursor

Original target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`SoundPlayerView::ProcessQueues @0x0043DDF0` is a complete **1,452-byte**
already exact owner with **43 original-target-bound links**. It removes a
command by shifting records left from each following command. Every
`SoundPlayerCommand` occupies **0x10C bytes** (three 4-byte words and a
256-byte path). The original body uses `LEA EDX,[EBX+0x10C]` and
`ADD EBX,0x10C` in the shift loop for the source and moving destination.
A sufficiently late command could make the source pointer one-past the
C++ command array, while the game executes the native flat 32-bit
address addition without a bounds check.

The natural C++ now uses the previously established typed
`SoundWordSlot(command, 1)` for **both** the advancing command cursor
and the `memcpy` source, preserving the original element stride,
31-iteration maximum, stop-on-none behavior, copy byte count, event/thread
operations and all command-state and early-termination branches.
No new bounds check, integer validation or portable pointer guarantee is
introduced. This fixes source-level Win32 semantics, *not* game safety.

## Full native comparisons and exact guards

Five independent natural VC7.1 SP1 `/GL /GS /EHsc` two-source
Sound.cpp+ZWave.cpp variants were linked in the real `ProcessQueues`
entry context: native source only, native destination advance only,
both, a named next-command pointer, and a direct low-word address
expression without the typed helper. All five reproduced the entire
**1,452/1,452** original code bytes and all **43/43** original-linked
fields. The installed combined source was separately cold replayed:

| Existing exact Sound owner | Original bytes |
| --- | ---: |
| `sound-process-queues` | 1,452/1,452 |
| `streaming-sound-init-buffers` | 338/338 |
| `sound-bgm-player-thread` | 189/189 |
| `sound-player-reopen-bgm` | 55/55 |
| **Sound subtotal** | **2,034/2,034** |
| `sound-find-wave-chunk` + `sound-data-loader-thread` (separate WAV context) | **178/178** |

The current separate **298-byte** `LoadBgm` complete function also
passes a strict candidate original-bound **298/298** comparison with all
12 independently reviewed address fields in the Sound graph, after the
command-shift change. Its *canonical ledger admission* remains a separate
step; do not attribute new exact ledger credit to this shift change.
The complete **684-byte nonexact** `LoadSoundSample` source owner in the
five-TU WAV graph retains its six prior ordinary mismatches, 25 candidate
field positions, and the exact 71-byte chunk-scanner callee.

Reproduce only the affected graphs:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit sound-process-queues --unit streaming-sound-init-buffers \
  --unit sound-bgm-player-thread --unit sound-player-reopen-bgm --json
scripts/repo-python scripts/replay-exact-units.py \
  --unit sound-data-loader-thread --unit sound-find-wave-chunk --json
scripts/repo-python scripts/verify-handoff-pins.py
scripts/repo-python scripts/ci.py
```

Retained ignored script/native PE/MAP/PDB and original-bound receipts:
`.analysis/gpt-web/20261107-active/sound-command-shift-address.py`,
`sound-command-shift-results.json`,
`sound-command-shift-selected-replay.json`,
`sound-command-shift-wav-guards.json`, and
`build/gpt-web/20261107-active-sound-command-shift/`.
