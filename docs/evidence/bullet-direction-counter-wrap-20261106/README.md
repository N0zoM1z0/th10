# Direction-change transform counter: native 32-bit wrapping

Original target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The three state-3 transform owners in `src/BulletManager.cpp` all increment
`BulletExStateView::int2` before testing it against signed `int1`:

- `UpdateRelativeDirectionChange`, original `0x00407780`, 338 bytes;
- `UpdateAbsoluteDirectionChange`, original `0x004078E0`, 335 bytes;
- `UpdateAimedDirectionChange`, original `0x00407A30`, 431 bytes.

Each native 32-bit increment wraps at `INT_MAX`. The previous C++
`++state.int2` has undefined behavior at that limit. The maintained source
now explicitly adds `1u` to the unsigned representation of the existing
signed 32-bit count and publishes the resulting 32-bit signed word,
*before* retaining the target's signed `int2 >= int1` comparison. No
new field, cap, branch, test, target-byte replacement, or ABI is added.

## Whole-owner source exploration

Fourteen isolated natural-source variants were actually compiled with pinned
VC7.1 SP1 from the real seven-input `/GL /GS` BulletManagerUpdateCallback
context to study the 335-byte nonexact absolute owner. Capturing or
reordering outgoing angle bits, direct `memcpy` from the member, unsigned
angle views, const/copy/reference speed lifetimes, and typed velocity owner
were all byte-neutral at **333/335**; changing speed assignment/initialization
widened the owner, and member float-angle access regressed two more bytes.
All temporary sources were restored. The remaining original `MOV EDX`
versus candidate `MOV EAX` in the speed forwarding sequence at offsets
`+0xD7` and `+0xDD` is **still unresolved**.

Three *separate* cycle-counter forms (unsigned reference increment,
unsigned aliased add, signed word publication) were then compiled and
strictly compared for full original-linked owners. Each preserved the
identical physical body and all independently bound fields:

| Owner | Complete original bytes matched | Original-bound fields |
| --- | ---: | ---: |
| Relative direction | 338/338 exact | 8 |
| Aimed direction | 431/431 exact | 12 |
| Absolute direction | 333/335 nonexact | 8 |
| Horizontal wrap guard | 148/148 exact | 8 |

The simplest natural unsigned-add/signed-word-publication form is retained.
On the **actual installed source**, only the five relevant canonical exact
units were independently cold-replayed in two small graphs:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit bullet-update-relative-direction \
  --unit bullet-update-aimed-direction \
  --unit bullet-update-horizontal-wrap \
  --unit bullet-update-vertical-wrap \
  --unit bullet-update-vector-acceleration --json
```

All **1,394/1,394 previously accepted exact bytes**, and every declared
original-target linkage field, pass. A fresh strict whole-owner comparison
of the remaining 335-byte absolute direction function independently binds
all eight target fields and reports only `+0xD7/+0xDD` as mismatches.
This is source-semantic reconstruction, **not** new exact credit.

Pinned ignored evidence:
`.analysis/gpt-web/20261106-chat-th10-bullet-abs/`,
`.analysis/gpt-web/20261106-chat-th10-bullet-wrap/results.json`,
`selected.json`, `current-absolute.json`, and native PE/MAP/PDB under
`build/match-linked/BulletHorizontalWrap/` and `build/match-linked/BulletState8/`.
Source SHA-256 `ddb77dc65b7e1ae9629d16b64fee178cff743f3192ce95617d8dcbcf64a34a63`.
Neither physical original TU naming, full game product/runtime semantics,
nor portability is established.
