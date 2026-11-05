# TH10 score-file header and tail: typed native members, exact loader and initializer

Original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The original score-file magic is `0x30314854` (little-endian `TH10`),
version is the low 16 bits at header +0x08, and the tail record magic is
`0x5453` at tail +0 with an independent 16-bit word at +2.

In the maintained `GameStatisticsFileHeader` the name `magic` and the
native `unsigned short formatVersion` replace `unknown00` and the
raw low-half `unknown08` cast. The upper +0x0A word retains an opaque
name. In `GameStatisticsTail` the previously anonymous 32-bit header word
is split into the actual 16-bit `magic` and opaque word; the existing
initializer now writes those members directly. Compile-time `sizeof`
and `offsetof` constraints enforce +0, +2, +8, +0xA, and preserve the
complete 0x18-byte file header and 0x448-byte tail layout. The code does
not assign unsupported semantics to other fields.

Actual pinned VC7.1 /GL native selective cold replays from the new installed
source establish complete **target-exact** bodies:

- `GameStatisticsOwner::Load42B030`: **423/423 bytes**, with its existing
  canonical bound fields and local destinations
- `InitializeStatisticsTail42ADD0`: **142/142 bytes**, with its independently
  reviewed original target structure

These two functions were **already** exact; their typed recovery preserves
rather than increases canonical exact credit. The actual linked PDB also
still emits the nearby **NONEXACT** `CreateStatistics42AF20` factory at
98 bytes (97-byte target) and constructor at 137 (132-byte target).
Those remain open, not new matches. No complete source owner / runtime
completion is asserted for the score subsystem.

Seven-stage repository CI and whitespace checks pass. Only the relevant
two canonical units were cold-replayed; no full cohort, product build or
runtime validation was run. See the current source hashes and exact unit
results in `native-result.json`.
