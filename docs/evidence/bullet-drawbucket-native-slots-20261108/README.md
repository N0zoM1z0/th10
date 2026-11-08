# TH10 BulletManager: native draw-bucket table addresses

Original target: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The complete original `BulletManagerView::UpdateBullets @0x004065C0` is
**249 bytes**, with 6 independently checked target-bound constant/global/call
fields. The registered `BulletManagerUpdateCallback @0x00406770` is a
separate exact **40-byte**, 2-field owner. The source loop links live
bullets into six draw buckets. The per-bullet `drawBucketIndex` is used
directly as a signed index, without a range check. Original i386 addresses
bucket heads and tails using low-32-bit base plus index times four. The
previous C++ expressions `drawBucketHeads[bucket]`,
`drawBucketTails[bucket]` and `drawBucketTails[bullet->drawBucketIndex]`
would form abstract out-of-bounds array references for an invalid index.

A simple typed, forced-inline `BulletBucketAtWord(T *base, int index)` now
models the low-32-bit base/index/stride calculation for all three slots.
It keeps the existing branch and list-linking order, reads
`bullet->drawBucketIndex` separately for the final tail write (as the
target does), and adds no validation or runtime guard. This is native
Win32 source semantics, not a malformed-state safety or portable-pointer
claim.

## Actual full-owner experiments

Five different natural C++ source variants were compiled and linked
through the real pinned VC7.1 SP1 `/GL /GS` manager-update callback
entry: head only, tail only, final tail write only, both tail locations,
and all three bucket locations. All five preserved the **complete
249/249 original manager bytes and 6/6 original relocation fields**, plus
the **40/40 callback and 2/2 original fields**. No source variant was
accepted merely because the normal-COFF geometry matched. The combined
form was installed and formatted; a fresh original-bound selected cold
replay passed five previously canonical exact owners:

| Already exact unit | Original bytes | Original fields |
| --- | ---: | ---: |
| BulletManagerView::UpdateBullets | 249/249 | 6 |
| BulletManagerUpdateCallback | 40/40 | 2 |
| BulletRuntimeView::Deactivate | 158/158 | 2 |
| BulletPositionView::IsOutsidePlayfield | 109/109 | 6 |
| BulletWrapPositionView::IsOutsidePlayfield | 109/109 | 6 |
| **Total** | **665/665** | **22/22** |

This is a source-semantics checkpoint for owners **already exact**; there
is **zero new exact ledger/codegen byte credit** and no full-game runtime
claim. The change did not involve a full historical cold replay.

Reproduce only the affected canonical units:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit bullet-manager-update --unit bullet-manager-update-callback \
  --unit bullet-runtime-deactivate --unit bullet-position-bounds \
  --unit bullet-wrap-position-bounds --json
scripts/repo-python scripts/verify-handoff-pins.py
scripts/repo-python scripts/ci.py
```

Retained private trial source, full PE/MAP/PDB and selected receipt:
`.analysis/gpt-web/20261108-bullet-drawbucket-native-slots.py`,
`.analysis/gpt-web/20261107-active/bullet-bucket-native-results.json`,
`.analysis/gpt-web/20261108-bullet-buckets-final-replay.json`,
`build/gpt-web/20261108-bullet-bucket-words/`.
Final source SHA-256: `2050ed5197807702de5f250db77d6e37358a612cdec323849baaab1d786c7c35`.
