# TH10 vertical wrap — complete 172-byte exact owner

Original Japanese TH10 v1.00a, executable SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
`BulletRuntimeView::UpdateVerticalWrap` owns the complete
`0x00407E40..0x00407EEB` extent, **172 bytes**, with the actual private EBX
receiver and plain RET.

The target reads Y separately for its double-zero and bottom comparisons.
Each successful branch reads sprite height first; the upper branch then adds
Y and bottom, while the lower branch adds bottom and subtracts from Y.
Both paths join before the position store and timer call. The transform sound
is captured after the timer call; the expiry check also remains on the
no-wrap path.

Duplicating the timer/sound tail within the two overflow branches, as in the
exact horizontal sibling, restores the saved-register and receiver setup.
Scoped volatile views preserve the observed comparison/height-read boundaries.
Direct compound expressions let VC7.1 merge the position store without the
extra live x87 temporaries produced by separate local intermediates. Original
volatile qualification and original expression spelling remain unknown.

The first final-source link freshly compiles the changed primary source and
uses six source/object-hash-checked support objects. Its complete strict
comparison passes **172/172 bytes, 8/8 independently original-bound fields**.
The fields cover the real wrap-bounds helper, double-zero constant, three
bottom references, timer Add, sound owner and QueueSoundSample. Original
constant payloads are checked as double zero and float448. All instructions,
local destinations and both exits are included.

A separate canonical cold recompiles **all seven actual inputs**, preserving
the existing `/GL /GS` callback graph and compiler/linker profiles:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit bullet-update-vertical-wrap \
  --unit bullet-update-horizontal-wrap --json
```

Both pass: new vertical owner **172/172, 8/8** and existing horizontal guard
**148/148, 8/8**, totaling **320 bytes /16 fields** in one cold image. The
recipe is `[units.bullet-update-vertical-wrap]` in `config/match-units.toml`;
the shared output is `build/match-linked/BulletHorizontalWrap/`.
[The compact record](native-result.json) pins both images, final sources and
the original field bindings. **New confirmed-authored exact credit: one
complete owner /172 bytes.**

Private drivers and full receipts are under
`.analysis/gpt-web/20261106-bullet-vertical-wrap/`, including
`ordered-expressions-strict.json` and `canonical-replay.json`. Earlier
source forms remain measured candidates, not additional exact credit.
An unsuccessful Wine-path attempt produced no comparison; the subsequent
toolchain execution smoke and completed links passed. No historical cohort,
host failure scenarios, game runtime or product-closure tests were run.
Original TU/data ownership and runtime closure remain independent.
