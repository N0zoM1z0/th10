# Enemy interpolation timer: recover original native INC/DEC word semantics

Target: Japanese TH10 v1.00a original image, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The target's complete `EnemyScalarInterpolationView::Evaluate`
(`0x00412AC0`, **652 bytes**) contains the inline 0x14-byte enemy timer
state machine. Original target-relative `+0x3B` performs `INC EDX`
followed by the dword store at `+0x42`. When the interpolation expires,
`SetCurrent` writes the requested timer and original target-relative
`+0x99` performs `DEC ECX` before storing `previous` at `+0x9A`.
These are native i386 low-32-bit arithmetic words, not saturating counters.

The maintained inline `EnemyInterpolationTimerView::Tick` previously
used signed `++current`, and its `SetCurrent` used `previous = value - 1`.
Both can invoke signed-overflow undefined behavior at native word limits.
They now publish the result of explicit unsigned 32-bit addition or
subtraction back to their original signed fields. All original signed
subframe comparisons, time scaling, `current` assignments, branch
conditions and update order remain intact. No bounds checking or
portable-architecture guarantee has been introduced.

## Independently verified current-source results

The `Tick` increment and the `SetCurrent` decrement were each compiled
**separately** in the true five-support `/GL` Enemy scalar-interpolation
graph, with the real `EnemyRuntimeUpdate` as the entry. Each selected
canonical original-bound full-function replay passed the complete
**652/652** byte original `Evaluate` owner and all its target-bound
fields. The combined installed source was also cold checked against
four further previously accepted original-bound Enemy owners:

| Existing exact original owner | Bytes matched |
| --- | ---: |
| `enemy-evaluate-scalar-interpolation` | 652/652 |
| `enemy-manager-update` | 166/166 |
| `enemy-runtime-vector` | 30/30 |
| `enemy-ecl-reset-state` | 71/71 |
| `enemy-ecl-release-allocations` | 45/45 |
| **Selected total** | **964/964** |

The two additional pairs were run as separate small scoped cold replays,
not an all-unit historical replay. This verification preserves five
**already exact** functions; it creates no new exact ledger credit.

The modified header and unchanged real Enemy/EnemyCallback sources were
also independently cold linked in the actual two-TU Enemy runtime graph.
After masking only actual linked code/data fields, the complete
**2,371-byte** `EnemyRuntimeUpdate` candidate, **69 candidate fields**,
and **606-byte** `EnemyResolveRuntimeCallback` candidate, **7 fields**,
were identical before and after the header change (zero ordinary byte
changes, identical candidate field geometry). Their *original* owners
are **2,412** and **593** bytes respectively: both remain NONEXACT.
Candidate fields are not falsely promoted to independent original
bindings. This is not proof of a complete game product/runtime.

Focused reproduction:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-evaluate-scalar-interpolation --json
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-manager-update --unit enemy-runtime-vector --json
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-ecl-reset-state --unit enemy-ecl-release-allocations --json
scripts/repo-python scripts/ci.py
scripts/repo-python scripts/verify-handoff-pins.py
```

Ignored current-source proof material is under
`.analysis/gpt-web/20261107-active/enemy-{tick-exact-probe,
setcurrent-exact-probe,timer-leaf-a,timer-leaf-c}.json`, with full
linked source image/PDB/candidate comparison in
`build/gpt-web/20261107-active-enemy-callback/{native-words,
timer-native-words}/`. No full cold replay was performed.
