# TH10 Enemy ECL 14KB dispatcher: complete typed bullet-pattern field layout

Source target: original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
This change affects the actual 14,416-byte `EnemyRuntimeView::DispatchEclInstruction`
(owner `0x0040E770`) and its native `EnemyBulletPatternView` array
(runtime +0x2C4, eight contiguous 0x210-byte records). The whole function
remains **NONEXACT**; no newly claimed owner byte equality with the target.

## Source and layout evidence

Original target dispatcher loads/stores independently establish the following
byte offsets within **each** 0x210-byte bullet-pattern record:

| Offset | Recovered typed member | Source command behavior |
| --- | --- | --- |
| +0x00/+0x02 | `spriteId`, `spriteVariant` | bullet sprite IDs |
| +0x10/+0x14 | `angleA`, `angleB` | pattern angle pair |
| +0x18/+0x1C | `speedA`, `speedB` | speed/rank interpolation |
| +0x1F4/+0x1F6 | `countA`, `countB` | density/count pair |
| +0x1F8 | `aimMode` | aim mode selector |
| +0x1FC | `flags1FC` | initialization flag word |
| +0x200/+0x204 | `soundId`, `soundVariant` | sound command values |

These fields replace the earlier `[0x18]`/`[0x1F0]` opaque padding while
retaining exactly the same 0x210-byte native object. C++03/MSVC static
`offsetof`/size checks now assert every field offset. Existing `words[0x84]`
union and the eight-record array are preserved, as is the constructor's
original `-1` initialization at +0x204. The maintained ECL opcode source
replaces the old raw offset casts with typed member writes across sprite,
angle, speed, count, aim and sound instructions. Aliases still using
native integer base arithmetic retain exactly their original address
calculations until they can be independently proved.

## Actual VC7.1 compiler check

The changed header and full dispatcher were recompiled into the real 11-TU
Enemy-update `/GL /GS` graph using pinned VC7.1 SP1; current changed Player
source was also recompiled and other unchanged support objects were reused
only after SHA-256 checks. The complete dispatcher PDB contribution stayed
**14,352/14,352 candidate bytes** with **716/716** identical decoded
address-field positions/types. The entire ordinary native candidate body
remains byte-for-byte identical after field normalization. This was checked
from the **final installed** source (not an earlier subset of the field edits).
The latest source hash and whole-owner comparison are in
[native-result.json](native-result.json), the independent header hash in
[header-sha256.txt](header-sha256.txt), and the ignored PE/MAP/PDB under
`build/gpt-web/20261104-live-enemy/bullet-template-complete-fields/`.

The candidate-versus-candidate result does **not** grant any new 14,416-byte
original-target match credit. Reproducing the corresponding 0x210-byte native
layout does not establish original C++ identifiers, historical TU ownership,
product build or runtime correctness. Only directly affected exact helpers
are replayed by selector; no full cold replay is required.
