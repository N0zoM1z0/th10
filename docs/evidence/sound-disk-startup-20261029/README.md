# Complete disk and memory streaming creation, 2026-10-29

Seven complete owners add **2,130 exact bytes / 60 fields**:

| Owner | Target | Bytes | Fields | Canonical unit |
| --- | --- | ---: | ---: | --- |
| FinalizeStartupResources | 41F8D0..41F921 | 82 | 10 | `startup-preload-resources` |
| SoundPlayerView::PreparePlayback | 43DB60..43DC86 | 295 | 11 | `sound-prepare-playback` |
| CSoundManager::CreateStreaming | 44C8F0..44CBAD | 702 | 16 | `sound-manager-create-streaming` |
| CSoundManager::CreateStreamingFromMemory | 44CBF0..44CE7A | 651 | 13 | `sound-manager-create-streaming-memory` |
| CSound::CSound | 44CE80..44CEF5 | 118 | 3 | `csound-constructor` |
| CStreamingSound::CStreamingSound | 44D6A0..44D70C | 109 | 4 | `streaming-sound-constructor` |
| CWaveFile::Open | 44DBF0..44DC9C | 173 | 3 | `wave-file-open` |

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Local Ghidra attestation and fresh complete raw decode agree with reviewed
boundaries. The startup thread is absent from Ghidra's function inventory;
its 299-byte raw boundary and instructions are reviewed directly instead.

## Complete source and the decisive comparison

Eight previously unmapped owners now have complete natural source mappings,
covering 2,019 target bytes. This includes the real Startup41F990/299 and disk
caller43D690/241, which remain nonexact at 292 and 249 bytes. Main.hpp and the
existing Chain/ANM/FileSystem interfaces supply typed views; callback convention,
original identifiers, physical TU and product storage remain provisional.

The first comparison recompiles Main, Sound, ZWave and the new Startup input,
reusing twelve independently pinned unchanged objects. PreparePlayback295 and
Finalize82 match immediately, as do the historical Open and constructor proposals.
The disk parent remains 681/702 and the existing memory parent is 651/651 with
ordinary differences. Neither equal size nor the historical proposal is accepted
without complete field proof.

One target-backed source distinction resolves both large owners: save and test
each COM HRESULT separately. A combined CreateSoundBuffer/QueryInterface failure
expression has the same intended branch conditions but produces different cold
tails and epilogue merging. Separate checks recover the disk parent's complete
702 bytes, including eleven internal alignment bytes and fixed local branches.
Applying the same change to the memory creator recovers all 651 bytes. No ABI
declaration, target byte, padding, compiler flag or branch destination is forced.

Fresh review also establishes:

- PreparePlayback resets twelve queue slots, joins the **global** loader owner,
  and duplicates 47 samples through a target eight-byte metadata row. Its volume
  field is the table base plus four; linker-anchor adjacency is not a binding.
- The attenuation's zero gate uses signed sfxVolume while its quartic expression
  reads signed bgmVolume. Both behaviors are preserved literally.
- Open preserves its flags/memory-mode writes, sequential shared read, post-call
  pointer rereads, and success for flags other than one.
- Base construction fills the first buffer even for count zero, uses the incoming
  count for rewind, and leaves unobserved fields alone. Ordinary derived chaining
  recovers the actual ESI receiver/four-stack-argument/RET16 interface.
- Creation cleanup remains asymmetric. Wave deletion belongs only to Open failure;
  later COM failures retain the target's leaks. Notification allocation failure,
  SetNotificationPositions cleanup and repeated output-pointer reads are included
  in the complete comparisons, without host failure simulations.

## EH entry binding

Both creators have one streaming-allocation unwind action. Their complete
21-byte generated regions delete the saved pointer at **EBP−0x48**, then dispatch
through CxxFrameHandler. Target entries46533B/46531B select seven-word, 28-byte
FuncInfo records471398/471374 and one-state unwind maps471390/47136C.

The allocation-EH comparator now accepts an explicitly declared aligned negative
disp8 storage slot through `allocation_eh_storage_offset`. The default remains
EBP−0x10. The declaration constrains both images to the same literal slot; it
does not normalize stack offsets. Parent-named unaliased MAP entry, complete PDB
contribution, same parent object/module, immutable metadata, scalar-delete target,
one-state cleanup and frame-handler dispatch checks remain mandatory. Only the
parent handler address is bound: generated code, metadata, allocation storage
and runtime receive no extra exact credit.

The affected synthetic test passes five positives and 45 rejections, including
independent native/target wrong-slot controls and malformed declarations. The
retained default-slot ReplayManager::Load181 contract also still compares exact;
this is an infrastructure check of that retained image, not a new source replay.

## Cold reproduction and scope

Two independent canonical cold builds compile all sixteen declared inputs
serially on one CPU at nice10. Each compares all seven new owners and every
field, including both explicit EH protocols. The first also replays the two
affected normal-COFF destructor contracts. Thirteen selected existing linked
contracts compare 3,932 bytes / 112 fields in the new image, including the actual
FillBufferWithSound, StopBgm, ReopenBgm, queue, loader join and Log interfaces.
No historical full-cohort replay or product/runtime claim is made.

The first second-cold attempt stops before comparison because a Winepath page
fault contaminates one linker input path. Its log is excluded from the proof.
The required toolchain execution smoke passes; a fresh full canonical retry
then compiles all sixteen inputs again and passes. No failed-run objects are
reused for that accepted second witness.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit startup-preload-resources --unit sound-prepare-playback \
  --unit sound-manager-create-streaming \
  --unit sound-manager-create-streaming-memory \
  --unit wave-file-open --unit csound-constructor \
  --unit streaming-sound-constructor
```

[results.json](results.json) contains compact final pins, complete proof scopes,
EH evidence and diagnostic outcomes. Private producers, source snapshots,
listings and bounded exit logs remain in
`.analysis/gpt-6.1-sol/sound-disk-startup-20261029/`. Independent witnesses are
the retained first cold below `build/gpt-6.1-sol/sound-disk-startup-20261029/`
and canonical `build/match-linked/SoundDiskStartup/`.

The historical [disk pilot](../disk-streaming-20261025/README.md) remains evidence
of its old source models, not an acceptance requirement. Existing origin rows
are unchanged. SDK resemblance and byte equality do not settle original authorship;
the exactness and source-presence claims remain independent of that question.

The 786-byte initializer stays nonexact. This continuation's nested success model
also produces763, reproducing the previous success-first result; it is restored.
Startup's missing seven bytes, error-store scheduling and the disk caller's early
failure layout remain open. An ordinary member startup control is byte-neutral
and restored. No partial exact credit is taken for either caller.
