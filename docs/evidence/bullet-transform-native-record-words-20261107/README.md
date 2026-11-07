# Bullet transform program: native signed-index/record cursors in a large owner

Target: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Original `BulletRuntimeView::AdvanceTransformProgram @0x00406D90` is a
**1,454-byte** reviewed authored owner. The real mixed-profile
`BulletRuntimeUpdate` producer compiles `BulletTransform.cpp` as a **normal
COFF** support (without `/GL`), and its current candidate owns **1,540
bytes / 32 COFF relocations**. This owner remains **NONEXACT**.

## Source semantics, not a change to the target

The original checks only the upper bound `transformIndex >= 18`, then
executes `LEA` word arithmetic using the **signed** script cursor and the
0x18-byte `BulletTransformRecordView` stride. A script-controlled negative
JUMP index is not separately checked in the original; its x86 record
address is a well-defined modulo-32-bit word even if the pointer lands
before the declared transform array. C++ `&transforms[index]` can instead
invoke out-of-array undefined behavior. The maintained source now forms
the record address through:

`uint32(this) + offsetof(BulletRuntimeView, transforms) + uint32(index)*sizeof(record)`

and reads exactly the same real member `record->kind` after materializing
the pointer. The child-pattern action consumes a **second** 0x18-byte
record even when the first is the last declared array entry. Its `++record`
operation is similarly replaced by the original modulo-32-bit byte-word
advance. The SET_SPRITE action's script index also uses the original
signed-index-to-address word multiplication and low-word signed result:
`int(uint32(scriptBase[index]) + uint32(delta))`. The script base array,
real record members, signed script result and execution branch order are
unchanged. No extra guard, fake ABI, synthetic register, padding, target
byte patch, or new allocation has been introduced; none of these casts
promise portable or safe accesses for malformed script indices.

## Five native controls and installed-source exact guards

The actual original entry/prologue was independently disassembled:
original `+0x20` MOV EDX,[EBP+45C], `+0x26` LEA EAX,[EDX+EDX*2],
`+0x29` MOV ECX,[EBP+EAX*8+474], `+0x30` LEA ESI,[EBP+EAX*8+464].
A fresh normal-COFF source experiment tested five natural expression
forms around the first record: just reversing the source reads, a
snapshotted signed index, an unsigned snapshot, a native-byte-address
cursor and a native-byte-address read before taking the pointer. Three
reordered variants grew the owner from 1,540 to **1,548** bytes; the
native pointer/address-word expression retained the **1,540-byte** shape.
These were actually compiled and disassembled, not inferred from source.

A second experiment compiled five isolated/combined forms of the first
record address, the second child-pattern record pointer, and the sprite
script-address/index word. The selected **all-three** combination was
compiled from the installed source in the actual normal support graph.
The full **1,540/1,540 candidate machine-code bytes** agree with the
baseline. All **32/32 COFF relocations** have unchanged offsets, types,
and symbol destinations. Compiler-private `$L` label names changed, so
these were resolved to the actual section + offset before comparing;
merely comparing local label text would give a false regression.
This is **candidate-to-candidate preservation**, not target acceptance.
The original is still 1,454 bytes, so zero canonical exact credit.

With the final source installed, only three directly affected existing
original-target units were cold-replayed:

| Canonical unit | Original-bound bytes |
| --- | ---: |
| `bullet-runtime-update` | 885 |
| `bullet-update-horizontal-wrap` | 148 |
| `bullet-update-state8` | 306 |
| **Total** | **1,339/1,339** |

All declared original-target call/data fields of those units passed. No
full-cohort cold replay, full game build or runtime proof is asserted.

Private native trials and reports:
`.analysis/gpt-web/20261107-turn2-bullet/transform-index-probe.py`,
`transform-full-cursor-probe.py`, `transform-full-cursor-results.json`,
`installed-exact-guards.json`; COFF candidates below
`build/gpt-web/20261107-turn2-bullet/`; installed support in
`build/match-linked/BulletRuntimeUpdate/`.
