# TH10 MIDI track-file parser @ 0x0043AAD0 — complete source surface

Target: original Japanese TH10 v1.00a (hash in `config/target.toml`).
Boundary: **448 bytes** at `0x0043AAD0..0x0043AC8F`, RET 4, private
EBX receiver. This was an authored-origin, source-absent extent.

## Reconstruction against native instructions

- `0x0043AAE2`: release previous track allocations by calling the real
  `MidiOutputView::ReleaseTracks` at `0x0043AA60` *before* inspecting the
  selected `fileData[index]` slot at owner offset `+0x98`.
- Missing file slot returns `-1`. Otherwise the header length at byte +4 is
  big-endian; MIDI format, track count, and division use big-endian 16-bit
  fields at +8, +10, +12. Store the fields at owner offsets +0x11C,
  +0x118, +0x120 respectively. The native stores individual reversed bytes
  through one stack scratch buffer; maintained source models that buffer.
- The first track chunk begins at `file + 8 + headerLength`. Allocate
  `trackCount * 0x20` bytes at +0x138 and clear the entire record array.
  Each record sets active=1, stores the big-endian chunk length at record +8,
  allocates exactly that many bytes to +0x10, and copies from chunk+8. Advance
  the source cursor by chunk length plus the eight-byte chunk header.
- Set the active file slot at owner +0x10 and initial tempo at +0x124 to
  1,000,000 microseconds per quarter note. Return zero. No invented magic,
  bounds, allocation or validation branches were added to the target behavior.

The source now lives in `src/Midi.cpp` and uses its existing `MidiTrackView`
layout. This is a **source-present NONEXACT** mapping, not a codegen claim.

## Focused diagnostics

Two serial pinned VC7.1 `/GL /GS` link contexts with this method as entry:

- Simple shift-expression endian parser: linked **328** vs target **448**.
- Target-observed four-byte scratch reversal: linked **397** vs **448**;
  17/436 comparable ordinary bytes and three candidate linked fields. Keep
  the second implementation because its byte-buffer lifetime matches the
  original's security-cookie and stack-temp evidence better. Neither result
  is exact, neither qualifies for `config/matches.csv`.

Private logs: `.analysis/gpt-web/midi-track-loader-probe-20261103.log`
and `.analysis/gpt-web/midi-track-loader-array-probe-20261103.log`.
The original MIDI class declaration and physical TU remain provisional.
