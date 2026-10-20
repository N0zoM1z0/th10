# MIDI event parser and timer recovery

The maintained MIDI source now includes the actual event parser, timer Tick,
track reset, channel volume update and variable-length integer reader. The
default timer callback is recovered with origin left indeterminate. Source
presence, exact code generation, object/data ownership and runtime remain
separate claims.

## Target and physical ownership

All observations use the original Japanese TH10 1.00a executable, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Local headless Ghidra ran read-only/noanalysis with a 768 MiB heap and one active
processor. Executable identity, PE mapping and complete mapped `.text` hash
were checked; temporary function metadata was rolled back, and mapped bytes
stayed unchanged. Decompiler types and class names remain hypotheses.

| Function | Executable bytes | Complete owned bytes |
| --- | ---: | ---: |
| `MidiReadVariableLength @ 43A730` | 26 | 26 |
| `MidiOutputView::ResetTracks @ 43ACE0` | 132 | 132 |
| `MidiOutputView::Tick @ 43AF30` | 473 | 473 |
| `MidiOutputView::ProcessEvent @ 43B110` | 1,323 | 1,670 |
| `MidiOutputView::ApplyChannelVolumes @ 43B7A0` | 140 | 140 |
| `MidiTimerView::Tick @ 43B8B0` | 1 | 1, origin indeterminate |

The parser ends in RET8 at `43B638`. Its physical owner continues through
`43B795`: one NOP, three pointer tables (4/5/8 entries), compressed byte indices
(113/65/94 bytes), and two three-byte alignment instructions. Unsigned guards,
both stages of each indexed dispatch, complete byte domains and local code
destinations establish ownership. Ten CC bytes separate it from the volume
owner at `43B7A0`. This corrects **347 missing authored denominator bytes**;
it grants no exact credit for the parser. The target has no base relocations.

The multimedia trampoline at `43A6E0` loads ECX from its user argument, calls
the object's first vtable slot, and returns RET20. Slots `46F80C` and `46F810`
name the derived Tick and default one-byte callback respectively. The latter
is followed by fifteen CC bytes, before a separate retained RET entry. Its
source-written versus generated/shared origin remains unknown.

The partial view preserves timer fields at +4/+8/+C, headers at +14, ring index
at +94, track count +118, signed clock parameters +120/+124, unsigned 64-bit
clock state +128/+130, track pointer +138, handle +13C, sixteen 23-byte channels
at +154, transpose +2C4, fade state +2C8 through +2E8, and loop clocks +2EC onward.
Native offset assertions pass. A flat virtual class fails those assertions
under VC7.1; a 16-byte timer base preserves the observed prefix. This supports
the descriptive source model without closing constructors, vtable data,
original class identity or the complete allocation owner. Ordinary member
declaration for ApplyChannelVolumes replaces an unnecessary stdcall hypothesis;
that change is byte-neutral in the measured graph.

## Recovered behavior

Running status leaves the current data byte unconsumed. Zero-velocity note-on
uses note-off handling; transpose applies before the held-note bit operation.
Program, bank, pan, reverb, chorus and volume state follow their target channel
offsets. Volume remembers the requested value, scales and clamps the transmitted
value. The standalone volume updater applies a wrapping 32-bit adjustment before
its signed clamp, without overwriting the remembered channel state.

Tempo metadata uses **multiply by 0x101**, observed at `43B368`, including
ordinary three-byte values. The example payload `07 A1 20` yields 503752. Before
changing tempo, the parser banks elapsed MIDI ticks using signed-to-unsigned
64-bit extension of both signed parameters. End-of-track returns immediately,
without scheduling another delta or overwriting running status. Unknown metadata
skips its payload. The retained F7 path consumes the following variable-length
value as the next delta; it does not acquire conventional SysEx continuation
semantics absent from this target.

Loop controllers 2/4 snapshot/restore every track cursor and event time plus
tempo and both 64-bit clocks. The final delta is sampled after output calls and
after any loop restoration. SysEx replaces a pending header through the actual
UnprepareHeader body, prepares/queues a newly allocated payload prefixed by F0,
and frees payload/header on either API failure. It reloads the handle after
prepare and the ring index when clearing a failure. A null device retains the
allocated pending header; index 31 advances to zero. Target allocation, stream
bounds, terminator and nonzero-tempo preconditions are preserved.

Tick compares sign-extended event times as unsigned 64-bit values, recomputes
time after event processing and reads the current track table afterward. An
active track encountered earlier prevents same-tick rewind even if its event
ends playback. With no encountered active track, the elapsed counter is
incremented before ResetTracks resets it. Fade completion sets volume zero and
returns before processing tracks or incrementing the clock. The native numeric
sequence retains the first x87 value after FST and reloads the stored float
before the second conversion; finite host tests do not prove x87 equivalence.

## Proof scope and limits

Two independent complete single-TU VC7.1 SP1 /GL cold builds through the actual
virtual Tick entry freeze Midi.cpp and Midi.hpp. Strict comparison covers
**378 complete bytes and all five fields** per build: new authored volume140, reset132
and reader26, indeterminate default callback1, and existing UnprepareHeader79.
Reset/reader/default callback fully decode and are raw-equal with zero fields.
Volume binds REL32 +3D to __ftol2 at463B2C and DIR32 +76 to the WinMM short-message
IAT slot466298; all140 bytes then compare equal. The used status byte is computed
before the x87 conversion, preserving its target lifetime across FTOL.
The unchanged canonical StopPlayback-entry recipe also cold-replays all79
UnprepareHeader bytes/three fields. See [the compact input/artifact proof](native-proof.json).

Tick481/473, parser1614/1670 and StopPlayback200/129 remain
nonexact. The equivalent fade if/else control emits484/473 and is rejected
and restored. No compressed-table Oracle exception, forced volatile, placement
pragma, dummy argument, instruction patch or byte injection is introduced.
The anchor images are non-runnable and establish no production graph/runtime.

`scripts/report-midi-events.py` checks target/decoder identity, complete code,
returns/calls/local branches, all six physical owners, three complete compressed
dispatches, adjacent padding, timer vtable/trampoline and tempo recurrence.
`scripts/test-midi-events.py` compiles the **complete actual Midi.cpp** with its
canonical header, adapting only host SDK/layout and allocation observation roles.
It exercises the parser, Tick and real reset/volume/UnprepareHeader/StopPlayback
bodies, callback-sensitive stream/table reads, failure cleanup, unsigned clock
arithmetic, loop state and fade boundaries. ASan/UBSan/leak checks and thirteen
strict named mutants pass. WinMM, allocation and host SDK roles do not execute
TH10, prove native ABI/layout or establish product/runtime behavior. Only the
three transmitted short-message bytes are observed; the target leaves the high
byte untouched, and no host guarantee is claimed for that byte.

The MIT issue reference `levzzz5154/th10-decomp` remains pinned at
`c6791d82dc9f02e55eca45a5314f900a85481efd`; its
[license is retained](../../licenses/th10-decomp-MIT.txt). TH08/TH09 MIDI source
also supplied hypotheses for names and timer structure. All accepted offsets,
clock arithmetic, stream behavior and exactness here come from TH10-local
evidence. Upstream compiler-control attributes and metrics are not imported.
Pre-existing D3D evidence stays untouched.
