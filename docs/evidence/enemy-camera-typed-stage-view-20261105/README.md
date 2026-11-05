# TH10 330-byte camera configuration: reuse target-checked stage camera layout

Original TH10 Japanese v1.00a SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The maintained `EnemyConfigureViewportOwner` previously read camera
position, forward direction, up vector, rocking offset, FOV and screen-shake
fields as an anonymous `float *values` array, with several raw +0x18 casts.
`StageCameraStateView`, independently reconstructed from the stage-owned
camera pipeline, describes the **same 0x118-byte camera storage layout**:
position +0, direction +0x0C, up +0x18, rockingOffset +0x3C,
field-of-view +0x48 and screen shake +0xE8/+0xEC. `StageScript.hpp`
asserts those offsets and the complete size. The real 330-byte camera
configurator now reads those members via a layout-compatible camera view,
while preserving existing floating-point evaluation order and D3DX argument
lifetimes. No second original class identity is asserted.

A real **eleven-source VC7.1 /GL** link compiled the entire changed
`EnemyEclDispatcher.cpp`, recompiling changed `src/EclVm.cpp` instead of
reusing a stale object and reusing **nine** other source/header SHA-verified
objects. The complete 330-byte camera configurator (10 address fields) and
60-byte projection caller (1 field) preserve identical candidate geometry
and **zero** ordinary candidate-to-candidate byte changes. Complete
`EnemyBeginSpell` 2,432-byte candidate (108 fields), ECL dispatcher
14,352-byte candidate (716 fields), and CancelAllBullets 418-byte candidate
(12 fields) are similarly unaffected. Original ECL dispatch target is
14,416 bytes and remains NONEXACT. Candidate equality does not award new
original exactness to the 330-byte camera owner.

Selected canonical original-target checks of `enemy-prepare-projection`
60/60, `enemy-set-chapter` 16/16 and `enemy-configure-interrupt` 48/48
pass with the edited code. Seven-stage lightweight CI and whitespace
checks pass. No historical full cold replay, runnable product or runtime
claim. The linked artifact is in the ignored
`build/gpt-web/20261105-camera/typed-camera-from-stage-view/`.
