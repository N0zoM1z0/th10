# Enemy ECL dispatcher: native script-name, pattern-stride and difficulty words

Original Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`EnemyRuntimeView::DispatchEclInstruction` is a large, original
**14,416-byte** owner, still **NONEXACT**; the current compiled PDB
contribution is **14,352 bytes** and has **716 candidate** linked code fields.
This change removes signed C++ overflow in real native 32-bit script
arithmetic without claiming target equality.

The original executable contains repeated `IMUL` with 0x210-byte bullet
pattern strides and `LEA` address formation, as well as native dword
arithmetic on counted spawn-name byte lengths and signed difficulty ranks.
The old C++ produced signed intermediate products/sums which are undefined
on overflow (even when the hardware still writes the low 32-bit result).
The source now explicitly computes unsigned 32-bit intermediate words before
publishing the result to each original signed consumer, preserving
all original division-by-four and signed difficulty comparisons:

- **5** ECL spawn cases: count of inlined name bytes plus four bytes before
  locating the actual scalar-argument stream.
- **7** pattern-index `* sizeof(EnemyBulletPatternView)` products followed
  by the native address word, plus **2** products used without that base.
- **4** difficulty-rank bias expressions (`+0x200` or `+600`) selecting
  the original bullet speed/count interpolation cases.

The unchanged native 0x210-byte pattern view, active context, source operand
order, branch layout, actual callers and RAM owners remain intact. No range
clamp, guessed new field, artificial ABI, target bytes, assembly or stub is
introduced. Original declaration spelling and portable pointer/runtime
behavior remain unproven.

## Independent whole-owner 11-input native experiments

A **fresh current-source** pinned VC7.1 SP1 `/GL` 11-TU baseline was
constructed, rebuilding every support source that actually changed since the
previous epoch; reused objects were permitted only under their exact source
and header pins. Four serial trials then rebuilt the edited dispatcher TU
and linked it back with the same source-pinned support objects:

1. Pattern scaled address words alone.
2. Those pattern words plus difficulty-rank biases.
3. Spawn inline-name counted words alone.
4. All eighteen operations together.

The final installed, naturally formatted C++ was **recompiled once more**.
All five entire PDB-owned source functions have identical field-offset/type/
width geometry and **zero nonfield candidate byte differences** against the
current native baseline:

| Complete source owner | Candidate size | Candidate linked fields | Ordinary differences |
| --- | ---: | ---: | ---: |
| Enemy `DispatchEclInstruction` | 14,352 | 716 | 0 |
| `EnemyBeginSpell` | 2,432 | 108 | 0 |
| `EnemyFinalizeDeath` | 162 | 9 | 0 |
| `EnemyConfigureViewportOwner` | 330 | 10 | 0 |
| `EnemyFullObjectView` constructor | 672 | 5 | 0 |

These full-owner comparisons are **candidate-vs-candidate** and do **not**
constitute independently original-bound exactness for the 14,416-byte boss.
The unchanged 64-byte size gap remains, as does the pending target-field
acceptance. The selected exact leaf replay used the actual installed
formatted C++ and checked **five original-bound helper owners**:

`enemy-read-int-argument` **16/16**, `enemy-read-float-argument` **14/14**,
`enemy-resolve-float-argument` **14/14**, `enemy-read-raw-int-argument`
**17/17**, `enemy-read-raw-float-argument` **26/26** — total **87/87**
original target bytes and all their original binding fields.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-read-int-argument --unit enemy-read-float-argument \
  --unit enemy-resolve-float-argument \
  --unit enemy-read-raw-int-argument \
  --unit enemy-read-raw-float-argument --json
scripts/repo-python scripts/ci.py
```

Final maintained source SHA-256:
`1823e96cd57454d3a630f9ae738674918d8c3414363f42691cbff753ec41e286`.
Trial source hashes, complete PE/MAP/PDB fields and selected original cold
are retained in ignored `.analysis/gpt-web/20261107-enemy-more-words/`,
`build/gpt-web/20261107-enemy-flag-shift/{base,gptweb-20261107-formatted-word}/`.
No all-unit cold replay, new exact credit, full game link or runtime claim.
