# Complete 684-byte sound-sample loader recovered from natural WPO parameter order

Target: original Japanese TH10 v1.00a, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The genuine `SoundPlayerView::LoadSoundSample @0x0043D390` is a **684-byte** function. Its previous source produced the same extent but left six ordinary bytes differing at `+0x122..+0x127`: the original emitted `MOV EAX,EBP` followed by `LEA EDI,[ESP+0x30]`, while the native candidate reversed those instructions. Both `FindWaveChunk` calls previously passed the arguments as `data, remaining, tag, &chunkSize`.

## Recovered source and actual compiler decision

The source now declares `FindWaveChunk` as `__stdcall(data, chunkSize, remaining, tag)` and uses this order in the two RIFF searches for `"fmt "` and `"data"`. The change preserves each actual data pointer, 32-bit scan length, tag, and output pointer, and preserves the existing 71-byte scanner's behavior. With VC7.1 SP1 LTCG and the real WinMain producer, it changes the caller's register scheduling without changing control flow or the scanner's original private native register ABI. No byte patch, synthetic call wrapper, guessed machine-code return, or optimizer profile override was used.

Seven independently compiled source-formal permutations included the previous baseline. Reordering only the output pointer after `remaining` did not help; moving `chunkSize` *before remaining* recovered **all six** wrong caller bytes. More distant permutations regressed the private `FindWaveChunk` register use. The winning source is intentionally the simplest interpretation consistent with both original functions.

## Strict native proof and scope

The canonical `sound-player-load-sound-sample` linked-PE unit binds **all 25** original address/control fields: security cookie and three checker calls; the Sleep import; the real `g_MainSoundOwner + 0x5224` data-word; original `RIFF`, `WAVE`, `fmt ` and `data` constants; the two error-format globals; the GameErrorContext instance and two Log calls; two strncmp calls; two genuine calls to `FindWaveChunk @0x0043D250`; and four free calls. Each one has the target's exact offset, kind and target referent. The complete PDB contribution and all remaining bytes must replay exactly. The same source graph preserves the existing **71/71** `FindWaveChunk` owner and its actual `strncmp` REL32. The canonical helper symbol now reflects the corrected natural source parameter order, not a target ABI forgery.

Two independently selected zero-reuse cold VC7.1 SP1 builds of the installed source **have passed** the complete original-bound comparison, each verifying the 684-byte parent/all 25 fields, 71-byte scanner/one field, and 57-byte SoundSetupThread/all seven fields (**812/812 target bytes; 33/33 fields**). Results are retained in ignored `.analysis/gpt-web/20261109/sound-sample-installed-replay1.json` and `sound-sample-installed-replay2.json`. A broader multi-graph regression was attempted separately but timed out and is not cited as passing. The normal source/link graph is `Sound.cpp`, `Main.cpp`, `ZWave.cpp`, `GameErrorContext.cpp`, `/GL /GS`, WinMain entry, original version hash as above; no full historical replay is needed:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit sound-player-load-sound-sample \
  --unit sound-find-wave-chunk \
  --unit sound-setup-thread --json
```

A linked-PE diagnostic graph is not the finished running game. Retail original source spelling, whole game/resource closure, exception/runtime behavior and portability remain separate questions. Previous 578/584 normalized matches were nonexact and are not double-counted.
