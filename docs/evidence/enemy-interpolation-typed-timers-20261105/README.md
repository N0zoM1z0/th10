# TH10 typed position/scalar interpolation timers and complete boss codegen

Target Japanese TH10 v1.00a SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The maintained position/scalar initialization helpers are now expressed
through the established `EnemyInterpolationTimerView` (previous/current,
subframe, speed pointer and flag word) instead of raw integer-array offsets.
Position interpolation embeds this timer at `+0x30`, scalar interpolation
embeds it at `+0x20`; those offsets and the 0x14-byte timer layout are
already checked in `src/Enemy.hpp`. The source still initializes and
resets fields in the original target-observed order, including the prior
negative-time sentinel and game-speed pointer.

## Real original-target COFF checks

Independently compiled full VC7.1 normal COFF functions are **50 bytes**
each, with exactly one `DIR32` relocation to `g_AnmGameSpeed`. The complete
byte sequence equals retail target `0x004050D0` (position shape) and
`0x00412DB0` (scalar shape) when that single reference is resolved to the
attested original `0x00476F78`. No instruction bytes, tail lengths or
other fields were waived. Identical retail bodies exist at other reviewed
boundaries too, so byte identity **does not prove exclusive historical
source ownership or add new exact codegen credit**. The source functions
remain independently reviewed implementations, not guessed C++ names.

## Whole large-function impact, not silently hidden

A real VC7.1 /GL eleven-source rebuild of the **complete** ECL graph was
performed with SHA-pinned reused unmodified support. Both 50-byte helper
changes together preserve the 14KB dispatcher extent **14,352 bytes**
versus original **14,416**, and preserve every one of **716 linked field**
locations/types, but they cause **18 ordinary candidate-to-candidate byte
changes**. The isolated position-only change accounts for 14 and scalar-only
for four. Disassembly directly shows independent memory-store scheduling
changes in the dispatcher around candidate offsets `+0xBFF` and `+0x1117`,
not changed function meaning, suppressed target discrepancies or fabricated
ABI. The companion 2,432-byte `EnemyBeginSpell` target still emits a
2,424-byte complete candidate with 108 bound fields and zero changed
ordinary candidate bytes.

The 14KB and 2.4KB functions remain **NONEXACT**. Separate selected canonical
cold replays of `enemy-configure-interrupt` (48), `enemy-set-chapter` (16)
and `enemy-read-int-argument` (16) remain exact. Seven-stage lightweight
CI passed. No full historical cohort, product build or runtime assertions.

Reproducible ignored artifacts: `build/gpt-web/20261105-followup-interpolation/`,
`build/gpt-web/20261105-followup-ecl/typed-interpolation-timers/`, and
isolated `interpolation-position-only`/`interpolation-scalar-only` graphs.
