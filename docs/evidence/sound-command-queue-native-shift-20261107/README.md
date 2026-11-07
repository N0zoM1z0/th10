# TH10 sound command queue: native 0x10C-byte shift cursor

Target: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The retail `SoundPlayerView::ProcessQueues @0x0043DF10` is a full 1,452-byte
Sound command dispatcher with 43 independently target-bound fields. After
removing a command it shifts at most 31 remaining `SoundPlayerCommand`
records. The native compiled loop advances both the current cursor and
its next-record source by exactly `sizeof(SoundPlayerCommand) == 0x10C`
bytes (`ADD EBX,0x10C`, `ADD EDX,0x10C`, with `REP MOVSD` for the copy).
The source previously used bounded C++ `++command` / `command + 1`; on
an unchecked cursor it would form invalid abstract-array pointers, while
retail forms low-32-bit Win32 address words. The maintained source now
uses the existing `SoundWordSlot(command, 1)` for the two pointer steps.
The exact iteration count, early empty-command break, `memcpy` size,
post-shift repeat decision and all other queue operations are unchanged.
There is no additional range check, runtime guarantee on malformed
queues or attempted portable 64-bit pointer implementation.

## Five separate native trials and installed-source verification

Natural C++ variants were compiled and linked in the correct actual VC7.1
SP1 `/GL /GS /EHsc` Sound.cpp + ZWave.cpp context, with ProcessQueues as
entry: next-source word only, cursor increment only, both, named next
pointer, and inline native address expression. **All five** preserve
the complete original 1,452-byte owner, all **43/43** target-bound fields,
original 1,452/1,452 byte equality, and complete linked `ReopenBgm`
**55/55** with its two target-bound fields. The installed combined
source was cold rebuilt separately, not simply accepted from a trial:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit sound-process-queues --unit streaming-sound-init-buffers \
  --unit sound-bgm-player-thread --unit sound-player-reopen-bgm --json
scripts/repo-python scripts/replay-exact-units.py \
  --unit sound-data-loader-thread --unit sound-find-wave-chunk --json
scripts/repo-python scripts/verify-handoff-pins.py
scripts/repo-python scripts/ci.py
```

The first actual installed-source replay passes **2,034/2,034 bytes**
across four previously exact Sound owners and two real native artifacts;
the second WAV/load-thread replay passes **178/178 bytes**. The previously
recovered **298-byte** `LoadBgm` source in that same Sound+ZWave graph
was rechecked with all 12 original-bound fields: **298/298 strict bytes**.
This LoadBgm owner is not yet canonical-ledger admitted at this checkpoint;
its accepted exact byte credit remains separate. The nonexact
`PreloadBgm` remains outside the strict claim.

Evidence scripts, selected original-bound results and retained PE/MAP/PDB:
`.analysis/gpt-web/20261107-active/{sound-command-shift-address.py,
sound-command-shift-results.json,sound-shift-current-exact.json,
sound-command-shift-wav-guards.json,sound-shift-load-bgm-exact.txt}`
and `build/gpt-web/20261107-active-sound-command-shift/`.
No whole-image cold replay, product build or runtime equivalence is claimed.
