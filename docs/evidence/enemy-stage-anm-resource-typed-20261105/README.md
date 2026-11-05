# TH10 complete spell-start: typed stage ANM resource in manager slot +0x38

Original TH10 Japanese v1.00a SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The established 0x68-byte `EnemyManagerView` has four resource-pointer
slots beginning at +0x30, with the third at +0x38. The actual
`EnemyBeginSpell` caller uses that slot as an `AnmLoadedView *` in seven
stage branches for VM initialization and stage effect creation. The
maintained view now provides a typed `stageAnimationAnm` field at +0x38
via a union overlay of the original `effectResources[4]` array. Independent
native `offsetof` assertions preserve slot +0x30, typed slot +0x38,
and the following timer +0x40, as well as the complete 0x68-byte manager.
All other resource writers and variable-index consumers retain the
original array. The full source replaces **24** ANM resource-load casts
with the member, preserving the observed per-call global reloads.

A real VC7.1 /GL 11-source ECL graph was linked with changed sources;
unchanged supports were reused only under verified source/header pins.
The **entire** `EnemyBeginSpell` candidate remains **2,424 bytes** vs
original 2,432, with identical 108 linked-field offsets/types and **zero**
changed ordinary candidate bytes. The companion **14,352-byte** ECL
opcode interpreter candidate (target 14,416) retains 716 fields and
**zero** changed ordinary bytes. Neither large owner is exact; no target
match credit is claimed from candidate-to-candidate agreement.

Actual selected cold replays confirm `enemy-manager-constructor`
29/29, `enemy-ecl-resource-load-package` 239/239 and
`enemy-configure-interrupt` 48/48, **316/316 original bytes** already
accepted as exact. The seven-stage lightweight CI and whitespace checks
pass. There is no all-unit cold replay or product/runtime claim.

Current source hashes, comparison rows and selected replay are retained
in `native-result.json`; ignored local link under
`build/gpt-web/20261105-stage-anm/boss-typed-stage-resource/`.
