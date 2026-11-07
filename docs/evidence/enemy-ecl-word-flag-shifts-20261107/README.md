# Enemy ECL dispatcher: native 32-bit script-to-flag bit selection

Target: original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

Two cases of the giant `EnemyRuntimeView::DispatchEclInstruction` owner
change **one bit** of runtime flags, based on bit 0 of a VM integer operand.
The original target independently disassembles as:

- `0x00410FCC`: `SHL EAX,0x13`, then `XOR EAX,ESI`, `AND EAX,0x80000`,
  `XOR ECX,EAX`, and publish to `[EBX+0x1444]`.
- `0x00411BE0`: `SHL EAX,0x14`, then the same pattern with
  `AND EAX,0x100000` and publication to `[EBX+0x1444]`.

The prior maintained C++ first converted the operand to `int` and shifted
it left. In C++ this can be undefined for negative operands and for values
whose shifted bits overflow signed `int`, unlike the native x86 word shift.
The two opcode bodies now explicitly shift `unsigned int` representations.
The XOR/mask structure, real source operand read, flag field, execution
order and original behavior for all 32-bit input patterns are retained.
This does not add any bounds or runtime check or assume original spelling.

## Current-source, complete native caller comparison

A new 11-translation-unit source-pinned VC7.1 SP1 `/GL` candidate baseline
uses the real `EnemyRuntimeUpdate` entry. Eight prior objects were reused
**only** after checking their original C++ source and all 62 header SHA-256
hashes; the three changed translation units (`AnmManager.cpp`, `EclVm.cpp`
and `EnemyEclDispatcher.cpp`) were compiled fresh. After editing the two
opcodes, only the changed dispatcher translation unit was recompiled and
the complete image linked again. All five entire native PDB owners below
have **zero ordinary code-byte differences** from the immediately previous
source epoch; all field offsets/types/widths match:

| Complete native function | Candidate bytes | Candidate fields |
| --- | ---: | ---: |
| `EnemyRuntimeView::DispatchEclInstruction` | 14,352 | 716 |
| `EnemyBeginSpell` | 2,432 | 108 |
| `EnemyFinalizeDeath` | 162 | 9 |
| `EnemyConfigureViewportOwner` | 330 | 10 |
| `EnemyFullObjectView` constructor | 672 | 5 |

The **original dispatcher spans 14,416 bytes**, not 14,352: this is a
**NONEXACT** boss with a remaining 64-byte size gap. Full candidate-to-
candidate equality is not an original-target match or canonical ledger
credit. It does, however, demonstrate the two changes preserve the existing
entire generated boss and its graph without dropped chunks or changed fields.

Affected original-bound canonical cold replay, **selected only**:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-read-int-argument \
  --unit enemy-read-float-argument \
  --unit enemy-resolve-float-argument \
  --unit enemy-read-raw-int-argument \
  --unit enemy-read-raw-float-argument --json
```

Five existing leaf owners remain **87/87 exact bytes**. Lightweight
seven-stage CI and whitespace checks pass. No whole-repository cold replay,
full game build, original compiler ownership or runtime validation is
implied. Private SHA-pinned source/PE/MAP/PDB and fully normalised owner
comparisons are retained in
`.analysis/gpt-web/20261107-enemy-flag-shift/` and
`build/gpt-web/20261107-enemy-flag-shift/`.
