# TH10 Enemy ECL pattern address final offset: 32-bit native-word arithmetic

Target: Japanese TH10 v1.00a; independently verified SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The complete original `EnemyRuntimeView::DispatchEclInstruction` is
**14,416 bytes**. The maintained owner remains **14,352 bytes / 716 candidate
address fields** and is **NONEXACT**. This checkpoint improves the natural
C++ behavior of its actual common pattern-address helpers; zero new exact
code bytes are claimed.

`EnemyPatternAtRuntimeByte` and `EnemyExtraRecordAt` produce the final
pattern record pointer from a VM-provided signed, byte-scaled address word
plus the *reviewed* `offsetof` of the real `EnemyRuntimeView` fields. The
old code performed the last addition as signed `int`, which is undefined
when the native low 32-bit word overflows. The original executable independently
shows, for example, `IMUL reg,reg,0x210` at 0x00410139 and an address-forming
`LEA EDI,[EDX+0x2C4]` at 0x00410143, and repeated `IMUL 0x210`/`LEA +0x2C4`
in nearby pattern ECL opcodes. The new C++ casts the signed byte cursor to
`unsigned int` before adding the real member offset. No second shadow
storage, data allocation, range guard, ABI override, stub or target assembly
patch is introduced. Its pointer/integer mapping is pinned to the actual
VC7.1 i386 compiler; this is not a portability/runtime-safety proof.

## Actual current-source 11-TU whole-owner check

The pinned `EnemyRuntimeUpdate` compiler graph was rebuilt for the
*current* source/header epoch. Eight unchanged real support objects were
reused only where each source hash matched the recorded owner; the changed
`AnmManager.cpp`, `EclVm.cpp`, and `EnemyEclDispatcher.cpp` support objects
were freshly built. The two pattern-helper definitions were then compared
against their **prior HEAD source** using a separate newly compiled dispatcher
TU and a link with the *same current* nine other native source objects.
Original source bytes were restored after the trial and current modified
source remains installed. The full actual linked PE/PDB contributions give:

| Complete native candidate PDB owner | Bytes | Candidate fields | Changed ordinary bytes |
| --- | ---: | ---: | ---: |
| `EnemyRuntimeView::DispatchEclInstruction` | 14,352 | 716 | 0 |
| `EnemyBeginSpell` | 2,432 | 108 | 0 |
| `EnemyFinalizeDeath` | 162 | 9 | 0 |
| `EnemyConfigureViewportOwner` | 330 | 10 | 0 |
| `EnemyFullObjectView` constructor | 672 | 5 | 0 |

All five entire owner extents, their field **offset/type/width geometry**,
and every non-field code byte are identical across the two actual builds.
This is deliberately candidate-vs-candidate preservation, **not** an
original-target exactness claim for the 14,416-byte dispatcher.

## Affected selected canonical original-target cold

With the installed source, `replay-exact-units.py` selected only the five
real ECL argument leaves affected by the giant dispatcher graph. The real
original-target replay passes **87/87** bytes: ReadIntArgument16,
ReadFloatArgument14, ResolveFloatArgument14, ReadRawIntArgument17,
ReadRawFloatArgument26; all five original call fields are bound.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-read-int-argument \
  --unit enemy-read-float-argument \
  --unit enemy-resolve-float-argument \
  --unit enemy-read-raw-int-argument \
  --unit enemy-read-raw-float-argument --json
```

Ignored executable provenance and measured byte comparison:
`.analysis/gpt-web/20261107-enemy-pattern-final-add/{compare.py,comparison.json,selected.json}`;
actual native images under `build/gpt-web/20261107-enemy-flag-shift/{base,old-helper}/`.
The original physical source/TU, complete dispatcher exactness, whole-game
product build and runtime behavior remain open.
