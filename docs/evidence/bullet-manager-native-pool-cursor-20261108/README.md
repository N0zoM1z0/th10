# TH10 BulletManagerView::UpdateBullets: native pool cursor and live-count words

Original Japanese TH10 v1.00a, target SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The complete original `BulletManagerView::UpdateBullets @0x004065C0`
occupies **249 bytes**, with six independently original-bound constants,
gate globals and call fields. The callback at `0x00406770` is a separate
**40-byte** owner with two target-bound fields.

The original i386 loops across **2,000** fixed-size bullet entries, with
`ADD EBX,0x7F0` and `ADD EDI,0x7F0` advancing the pool cursor. C++
`++bullet` forms an abstract array pointer beyond the final element at
the end of the loop; the target instead adds the actual 0x7F0-byte Win32
stride to a low 32-bit address. The installed natural source uses an
explicit `reinterpret_cast<unsigned int>(bullet) + sizeof(BulletRuntimeView)`
then casts back to the typed object pointer at the original loop site.
The loop still has the original 2,000-iteration bound and update gating;
no added pool validation or change in bullet order is introduced.

The active bullet count at manager offset +0x5C is signed `int`, while
the original uses `INC dword ptr [ESI+0x5C]`. The previous C++
`++activeBulletCount` was undefined at signed `INT_MAX`. It now stores
`static_cast<int>(static_cast<unsigned int>(activeBulletCount) + 1u)`,
retaining the existing signed field type and all draw-bucket list logic.
These are Win32 source-semantics changes, not portable 64-bit pointer
claims or fixes for invalid state.

## Four complete native trials and installed-source canonical replay

Four independent VC7.1 SP1 `/GL /GS` source variants were actually
compiled and linked under the genuine `BulletManagerUpdateCallback` entry:
count only, pool cursor only, both, and unsigned lvalue increment. Each
reproduced **all 249/249 original UpdateBullets bytes, six original fields**,
and the complete **40/40 callback with two fields**. The combined variant
was installed and formatted. A fresh selected original-bound cold replay
from the **final formatted source** passed five already-accepted owners in
the two relevant build contexts:

| Previously exact owner | Original bytes | Fields |
| --- | ---: | ---: |
| BulletManagerView::UpdateBullets | 249/249 | 6 |
| BulletManagerUpdateCallback | 40/40 | 2 |
| BulletRuntimeView::Deactivate | 158/158 | 2 |
| BulletPositionView::IsOutsidePlayfield | 109/109 | 6 |
| BulletWrapPositionView::IsOutsidePlayfield | 109/109 | 6 |
| **Total** | **665/665** | **22/22** |

There is no new exact-owner codegen credit: all five owners were already
canonical exact. No full historical cold replay, whole executable build,
or runtime equivalence is claimed.

Reproduce only selected affected units:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit bullet-manager-update --unit bullet-manager-update-callback \
  --unit bullet-runtime-deactivate --unit bullet-position-bounds \
  --unit bullet-wrap-position-bounds --json
scripts/repo-python scripts/verify-handoff-pins.py
scripts/repo-python scripts/ci.py
```

Ignored trial driver and full source/PE/PDB/MAP receipts:
`.analysis/gpt-web/20261108-bullet-manager-counter-stride.py`,
`.analysis/gpt-web/20261107-active/bullet-manager-native-word-results.json`,
`.analysis/gpt-web/20261108-bullet-manager-final-replay.json`, and
`build/gpt-web/20261108-bullet-manager-native-words/`.
