# TH10 359-byte Player option-trail body: native Y-anchored sample cursor

Target: original Japanese TH10 v1.00a,
SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The complete physical native owner `0x00427960..0x00427AC6` is **359 bytes**,
including the unreachable six-byte `LEA` alignment sled at `0x00427A0A`.
Its sole known direct entry is the separately canonical-exact 12-byte
`0x00427950` callback wrapper, which supplies its option pointer privately
in ESI to the body. The source belongs to `src/Player.cpp`; original
source naming/TU and authored-versus-generated provenance remain unproven.

## Newly target-aligned source shape

The original loop computes seven intermediate history samples from each
option's 8-sample ring, interpolating signed X/Y coordinates with the real
`0.125f` step. Target assembly anchors the destination cursor at the
**Y** word of each 8-byte `PlayerPositionPair`, loading
`[EDI + EBP*8 + 0x4370]` and storing computed X through `[EBX-4]` and Y
through `[EBX]`. The maintained source previously anchored its C++ sample
reference at **X**, making VC7.1 use
`[EDI + EBP*8 + 0x436C]`, then `[EBX]` and `[EBX+4]`.

This checkpoint changes only the destination-address expression in that
complete `PlayerOptionTrailCallbackBody` loop. It obtains a byte cursor
from the **entire** `PlayerPositionPair` object, advances by
`offsetof(PlayerPositionPair, y)`, and writes X/Y at `sampleY-sizeof(int)`
and `sampleY`. These are the same real integer members of the same
`sizeof(PlayerPositionPair)==8` structure already used throughout Player
source, and remain within the sample object representation. No fake VM,
synthetic input, inserted instruction, arbitrary padding, or changed
interpolation logic is introduced. The existing 8-byte pair layout and
option/history indexing are retained.

## Fresh complete-owner native diagnostics — still NONEXACT

VC7.1 SP1 `/GL` was run in the real
`Player.cpp` + `AnmVmId.cpp` + `AnmManager.cpp` callback source graph,
using entry `RebuildPlayerOptions`, **not** a fake ESI receiver entry.
The PDB body remains **355 bytes** against the physical **359-byte** target,
so the complete owner is **not exact**, and no match record is admitted.
The relocation-aware ordinary-byte diagnostic on this complete contribution
improves from **92/339** before the source change to **140/339** after it,
48 more equal byte positions. This normalized preliminary byte score is a
*diagnostic, not an exactness percentage*; size, all independently bound
original fields/local destinations and original callable ABI must still
close before an exact claim. Five candidate linkage locations are reported
but do not turn this nonexact PDB extent into a canonical match.

Two separate actual compiler experiments gave the same **355-byte,
140/339** result: an initial pointer-to-Y expression and the retained,
object-base `unsigned char* + offsetof` expression, which keeps byte
arithmetic within the whole 8-byte sample. After installing that source,
an additional selected **cold** canonical `RebuildPlayerOptions` replay
passed all three affected existing exact callback owners:

| Retained exact owner | Scoped result |
| --- | ---: |
| `PlayerOptionTrailCallback` wrapper | 12/12 |
| `PlayerOptionSpecialCallback` wrapper | 12/12 |
| `PlayerOptionSpecialCallbackBody` | 99/99 |
| **Total** | **123/123 exact** |

The maintained source SHA-256 at that replay is
`acf9b8da39b6bea56d2f2a14aba44eb38f1e7a40c4f24a12fc3788c62b6fbc89`.
Private ignored receipts are
`.analysis/gpt-web/20261104/player-trail-destination-panel.json`,
`player-trail-byte-offset-panel.json`,
`player-trail-y-owner-scoped-guards.json`, and
`player-trail-y-owner-maintained-result.json`.

Run **only affected exact units**, not the all-unit cold cohort:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit player-option-trail-callback \
  --unit player-option-special-callback \
  --unit player-option-special-body --json
scripts/repo-python scripts/ci.py
```

This is a verified improvement in native source codegen for a **large,
nonexact** function. It establishes neither a complete byte match nor
whole-game product/runtime/portability closure.
