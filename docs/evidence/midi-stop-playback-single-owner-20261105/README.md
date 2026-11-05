# TH10 MIDI StopPlayback: recover original helper boundary and zero-store schedule

Target: original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The original complete `MidiOutputView::StopPlayback @ 0x0043AE20..0x0043AEA0`
(129 bytes) uses an optimizer-private **EDI** receiver, iterates the 32
MIDIHDR* slots with an EBX cursor / EBP count, calls the separate
`UnprepareHeader @0x0043AEB0` for non-null entries, stops the timer,
then resets/closes the MIDI output and clears active file index. The
standalone direct-entry source originally emitted a 200-byte owner because
`UnprepareHeader` was inlined. Natural C++ member declaration
`__declspec(noinline) int UnprepareHeader(MIDIHDR *header);` restores the
actual helper call without inventing a private register-bound declaration.
A direct-entry StopPlayback graph emitted 130 bytes; building the real
**destructor caller** under pinned VC7.1 SP1 `/GL /GS` exposes the native
internal EDI receiver and produced **129 bytes** with all five field locations
matching the original. Its only remaining 13 byte differences were the
compiler's movement of the `timerId=0` store past the following output-handle
load. A scoped intrinsic `_ReadWriteBarrier()` immediately after the existing
store preserves the source execution order, has no generated instruction, and
makes the pinned compiler reproduce the exact original store/register sequence.
The observed ordering is a compiler-source model, not a synchronization,
original C++ spelling, or new ABI claim.

## Complete first native proof and existing leaf guard

Fresh genuine pinned **one-source** `src/Midi.cpp` `/GL /GS` owner graph
with real `MidiOutputView::~MidiOutputView` entry. The PDB-owned complete
StopPlayback extent is **129/129**, all five independently original-bound
fields compare correctly (`UnprepareHeader` REL32 @+0x29; timeKillEvent IAT
@+0x3D; timeEndPeriod IAT @+0x47; midiOutReset IAT @+0x5D; midiOutClose
IAT @+0x6A); the original-bound strict comparator reports **129/129
matching bytes with no differences**. Complete own `UnprepareHeader` also
produces 79/79 ordinary bytes and unchanged 3 field locations. Its existing
canonical `midi-unprepare-header` selected original-target replay independently
passed **79/79**, as did seven lightweight CI checks and whitespace.

Private first proof: `.analysis/gpt-web/20261105-next-midi-stop/strict.py`,
`build/gpt-web/20261105-next-midi-stop/compiler-barrier/` (full PE/MAP/PDB,
`trial.json`), and
`.analysis/gpt-web/20261105-next-midi-stop/unprepare-replay.json`.

The one-source diagnostic destructor differs by one ordinary byte at +0x03
relative to original because the synthetic caller graph is not the whole
production context. Crucially, a **separate selected cold replay of the
original 30-input `midi-output-destructor`/`midi-release-tracks` canonical
source graph** with the installed changed Midi source passed **156/156 +
107/107 = 263/263** original bytes (9 + 2 original-bound link fields). This
was restricted to those two affected owners, not a historical full replay.
Private selected receipt:
`.analysis/gpt-web/20261105-next-midi-stop/existing-dtor-selected.json`.

## Independently accepted new canonical StopPlayback owner

The new `midi-stop-playback` unit uses its *own* fresh pinned one-source
`/GL /GS` destructor-caller graph under `build/match-linked/MidiStopPlayback/`.
Explicit selected canonical cold replay of this unit passed **129/129** bytes
and **all five independently target-bound helper/import linkages**, with no
local tail/branch normalization or ignored code. It is the **second**
original-bound build, separate from the first diagnostic and from the
longer existing destructor regression producer. Private receipt:
`.analysis/gpt-web/20261105-next-midi-stop/canonical-replay.json`.

This adds **one authored exact function / 129 new original-bound bytes** to
the ledger. No credit is duplicated for the already accepted 156-byte
destructor, 107-byte ReleaseTracks, or 79-byte UnprepareHeader. Neither whole
product/runtime closure, new original ABI knowledge, nor portability follows.

Installed `Midi.cpp` SHA-256: `1892650e1eae61877c583d1fcd4b70ad3e3f8714cfc842ff0703fff73f32d782`.

Installed `Midi.hpp` SHA-256: `2c0227d3743fd685883b6acfb6d4464747649692289a1215d771d4096a71397c`.
