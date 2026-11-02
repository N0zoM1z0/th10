# TH10 Sound/ZWave GetBuffer producer integration (2026-11-02)

This is a source-topology repair, not an additional codegen-exact function.
The existing CSound::GetBuffer at original 0x0044D3E0 was newly closed
as a 19/19-byte non-authorship-classified exact leaf in src/ZWave.cpp.
The separate src/Sound.cpp contained an inline member definition of
that same fully-mangled method, with the same branch logic.

After the ZWave leaf acquired an out-of-line definition, a genuine two-source
Sound.cpp and ZWave.cpp LTCG build failed with LNK2005: both .ltcg.obj
files contributed ?GetBuffer@CSound@@QAEPAUIDirectSoundBuffer@@K@Z.
The 19-byte exact leaf was not an independent complete product closure.

The retained fix removes only the duplicate inline body from the local
CSound view in Sound.cpp; it keeps the same member declaration, SoundPlayer
calls, object fields, call signatures and the single external definition
in ZWave.cpp. Sound.cpp now obtains its buffer via the actual shared
ZWave member rather than a second locally emitted body. No target binary
modification, linker multiple-definition suppression, fake ABI, or
duplicate special-case accessor was introduced.

Two targeted actual VC7.1 SP1 cold source-graph replays prove that the
change does not disturb complete preexisting authored/exact owners:

* Sound.cpp with ZWave support, real ProcessQueues root: full
  SoundPlayerView::ProcessQueues 1452 bytes/43 fields and
  BgmPlayerThread 189 bytes/7 fields, **1641/1641 exact**.
  The producer now links successfully with no duplicate-symbol error.
* ZWavePlayRoot in its selected actual GetBuffer definition context:
  GetBuffer 19/0, Get3DBuffer 52/1, Play 145/5 and FillBufferWithSound
  483/6, **699/699 exact**.

The target and source pins, unit sizes, field counts, producer and
replay totals are retained in native-summary.json. This is an
integration improvement with zero new canonical exact bytes. It does not
establish original physical source files, a fully linked/runnable game,
runtime equivalence or portability.

Reproduce the selected scope only:

    scripts/repo-python scripts/verify-target.py
    scripts/repo-python scripts/replay-exact-units.py \
      --unit sound-process-queues --unit sound-bgm-player-thread --json
    scripts/repo-python scripts/replay-exact-units.py \
      --unit zwave-csound-get-buffer --unit zwave-csound-get3d-buffer \
      --unit zwave-csound-play --unit zwave-csound-fill --json
    scripts/repo-python scripts/ci.py
    git diff --check
