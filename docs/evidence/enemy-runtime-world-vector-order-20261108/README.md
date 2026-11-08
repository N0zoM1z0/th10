# EnemyRuntimeUpdate: recover world-velocity x87 operand order

A fresh 12-source VC7.1 `/GL` link of `src/Enemy.cpp` improved the large
`EnemyRuntimeUpdate @ 0x0040DC80` candidate without changing its 2,412-byte
extent. The change is a natural C++ expression/lifetime experiment, not an
exact reconstruction or proof of original declarations.

## What changed

The prior vector expression produced the target X addition order but reversed
the base/offset operands for Y and Z. The target performs these three x87
operations in sequence:

```text
fld [base.x] ; fadd [offset.x]
fld [base.y] ; fadd [offset.y]
fld [base.z] ; fadd [offset.z]
```

A small `EnemyFloat3SumView` value type, a subtract operator, and volatile
component references to the base position make LTCG retain the target's
base-before-offset order for all three components. The constructor accepts
its values as Z/Y/X so the compiler's right-to-left argument evaluation
produces the observed X/Y/Z load sequence. This is a code-generation control;
it does not claim the original source used volatile members or this helper.

The new candidate preserves the full 2,412-byte function extent and improves
both comparison measures:

| Measure | Before | After |
| --- | ---: | ---: |
| Aligned instructions | 655 / 671 | **657 / 671** |
| Comparable ordinary bytes | 1,578 / 2,136 | **1,580 / 2,136** |
| Candidate size | 2,412 | 2,412 |

The three X/Y/Z load/add pairs now have the target's operand order. Two
remaining local differences are the Z-sum spill/reload home (`ESP+0x2C`
instead of target `ESP+0x24`, at offsets `+0x25F` and `+0x27A`) and later
control-flow/register-allocation differences beginning at `+0x49D`. The 69
linkage fields are candidate-derived and are not a complete original-bound
contract. The parent remains **NONEXACT** and earns no exact-byte credit.

`EnemyAdvanceMotion @ 0x0044C2A0` remains 168/171 bytes with 133/140 ordinary
prefix bytes and all seven reviewed linkage fields. Its final stack cleanup
mismatch at `+0xA1..+0xA7` is unchanged by this parent-only source change.

## Scoped regression checks

A fresh targeted 12-source link compared both parent and leaf. The two
independent exact owners in the same source were also cold-replayed:

- `EnemyRuntimeView` constructor: **145/145 exact**
- `EnemyRuntimeVectorView::FromAngleMagnitude`: **30/30 exact**
- Combined scoped replay: **175/175 exact**

No full cold replay was run. The scoped replay command was:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-runtime-vector --unit enemy-runtime-constructor --json
```

The fresh parent/leaf linked artifact and private validation receipt are in
`build/gpt-web/20261108-enemy-runtime-world-vector-order/` and
`.analysis/gpt-web/20261108-current/enemy-runtime-world-vector-order-validation.json`.
