# TH10 stage ANM resource slot recovered in the complete Enemy ECL boss

Original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The 0x68-byte `EnemyManagerView` places the four resource slots at +0x30,
with index two at +0x38 and the timer starting at +0x40. The maintainer now
preserves the original `void* effectResources[4]` array for the ECL loader
and all other callers, while exposing the independently established
`AnmLoadedView*` at index two through a union member
`typedEffectResources.stageAnimationAnm`. Native offset and manager-size
assertions fix the physical shape, without asserting unknown original names.

The entire maintained 2,432-byte target `EnemyBeginSpell` contains **24**
reads of this resource across all seven stage arms, including both sides of
the stage-one spell-id conditional and the stage-seven chapter conditional.
All 24 now access the real typed field. Independent per-call reloads were
retained: caching the pointer across ANM calls could modify code generation
or observable global data dependencies.

An **actual 11-source VC7.1 /GL linked graph** rebuilt the changed whole
ECL source and the modified-header consumers. Only eight support objects
whose transitive included-header sets did not reach modified `Enemy.hpp`
were reused, with exact source/object SHA-256 checks. Complete PDB owners:

* `EnemyBeginSpell`: **2,424 candidate** vs **2,432 original** bytes,
  **108** identical linked-field locations/types, zero ordinary
  candidate-to-candidate differences.
* `EnemyRuntimeView::DispatchEclInstruction`: **14,352 candidate** vs
  **14,416 original** bytes, **716** identical linked-field locations/types,
  zero ordinary candidate-to-candidate differences.

Both large owners remain **NONEXACT**. Separately selected canonical cold
replays of `enemy-manager-constructor` (29), `enemy-set-chapter` (16),
`enemy-configure-interrupt` (48), `enemy-ecl-resource-load-package` (239),
and `enemy-spell-panel-update` (1,057) are exact: **1,389/1,389** existing
bytes preserved. No new exact codegen credit, full historical replay,
original physical-TU identification, runtime or whole-product claim.

Reproducible ignored native outputs:
`build/gpt-web/20261105-stage-anm/boss-typed-stage-resource/`.
