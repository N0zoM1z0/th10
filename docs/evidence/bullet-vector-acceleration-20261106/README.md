# TH10 vector acceleration — complete 305-byte exact owner

Original Japanese TH10 v1.00a, executable SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
`BulletRuntimeView::UpdateVectorAcceleration` owns the complete
`0x00407560..0x00407690` extent, **305 bytes**.

The fresh baseline emitted285 bytes. The original handles expiry first,
computes all three game-speed-scaled increments before velocity writes,
spills Z to `[ESP+8]`, updates XYZ, then reloads XY for fabs/atan2. Capturing
an `AnmFloat3View` increment and applying it through an ordinary inline
reference-add helper restores this temporary/store boundary and the12-byte
frame. Direct scalar application of the temporary had emitted296 bytes and
incorrectly retained velocity results in x87.

The first complete305-byte source retained seven ordinary differences in the
slow timer branch: scale was loaded before subframe. Plain operand reordering,
a scale read qualifier and Tick were neutral and were restored. The exact
431-byte aimed-direction sibling supplies the concrete successful model:
read the actual subframe through a scoped volatile view, add scale, publish
that sum and convert the same sum to the integer frame. This restores the
original subframe-first x87 order without an extra read or ABI change. Original
volatile qualification and vector-class identity remain unknown.

The first final-source link freshly compiled the changed primary source and
used six source/object-hash-checked support objects. Its strict original-bound
comparison passes **305/305 bytes,10/10 fields**, zero differences. Each field
was independently checked in the original: four game-speed references, both
fabs thresholds, three timer constants and the ftol2 call. Float payloads,
all ordinary instructions, local destinations and both exits are preserved.

A separate canonical cold then recompiled **all seven actual source inputs**
with the established `/GL /GS` Bullet callback recipe. Only the new unit and
its related existing guard were selected:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit bullet-update-vector-acceleration \
  --unit bullet-update-horizontal-wrap --json
```

Both pass: new owner **305/305,10/10** and horizontal guard **148/148,8/8**;
**453 bytes /18 fields** in one cold artifact. The canonical recipe is
`[units.bullet-update-vector-acceleration]` in `config/match-units.toml`;
output is `build/match-linked/BulletHorizontalWrap/`.
[The compact record](native-result.json) pins both images and final sources.
**New confirmed-authored exact credit: one complete owner /305 bytes.**

The same final artifact independently binds all eight fields of the unchanged
absolute-direction owner: **333/335 bytes**, still NONEXACT at+0xD7/+0xDD
(EAX versus target EDX). Its [candidate contract](absolute-contract.json)
remains outside the canonical registry. This supersedes the earlier
candidate-field masking diagnostic and grants no extra exact credit.

Private drivers and full receipts are under
`.analysis/gpt-web/20261106-bullet-vector-accel/`, particularly
`ordered-subframe-strict.json`, `canonical-replay.json` and
`absolute-strict.json`. No historical cohort, host failure scenarios, game
runtime or product-closure tests were run. Original TU/data ownership and
runtime closure remain independent of this exact claim.
