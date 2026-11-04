# Recover typed motion and animation-state storage in the full enemy dispatcher

Current owner: `EnemyRuntimeView::DispatchEclInstruction` at original
`0x0040E770` (14,416 complete target bytes). The executable is original
Japanese TH10 1.00a with SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
This is a maintained C++ source improvement, not a claim of byte-exact
reconstruction.

The former decompiler-expression paths repeatedly accessed runtime-relative
`+0x2c/+0x30/+0x34` with raw integer-to-pointer casts, and the animation
selection/state at `+0xe8/+0xec/+0xf0/+0xf4/+0xf8` with more raw casts.
Those offsets are existing `EnemyRuntimeView` fields (worldMotion.position,
value0E8, value0EC, value0F0, animationBaseScript and animationDirection),
consistent with the existing static structure-layout assertions.

The real dispatcher's enemy-spawn, animated-VM initial position, movement
bounds, sound-cue position, and bullet-offset branches now use typed world
motion. Floating-point coordinates copied into a VM as *bitwise* dwords
remain explicitly 32-bit word reads to preserve the native bit-copy semantics;
normal float arithmetic continues to read floats. Animation bookkeeping
now goes through the actual named members rather than duplicate offset
projections. The preceding typed-flag repair is unchanged.

## Complete native owner validation

A selected 11-source VC7.1 SP1 WPO image was linked with the real Enemy
`Update` entry and the existing source graph, with the actual dispatcher
recompiled from the new source. Unlike historical receipts, the changed
Player support was also freshly compiled; other support object reuse was
guarded by SHA-256 source checks.

| Requirement | Previous typed-flags image | This batch |
| --- | ---: | ---: |
| Full PDB dispatcher contribution | 14,352 | 14,352 |
| Decoded native address fields | 716 | 716 |
| Field offsets and types vs previous | baseline | all identical |
| Ordinary differing bytes vs previous candidate, after candidate-field normalization | — | **0** |

The retained native PE/MAP/PDB is in
`build/gpt-web/20261104-live-enemy/motion-animation-typed/`; the
comparison and current source SHA are in [native-result.json](native-result.json).
The ignored driver `.analysis/gpt-web/20261104-live/large-enemy-typed-link.py`
recreates the bounded link using the previous flags image as comparator.

The 14,416-byte original owner remains **NONEXACT** and has not been
registered in `config/matches.csv`. Identical *candidate* codegen is a
meaningful preservation check, not a full original-target byte proof.
No cold replay of all historical units or executable runtime claim.
