# EnemyRuntimeUpdate: native 32-bit damage flash frame decrement

Target: original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The original complete `EnemyRuntimeUpdate @0x0040DC80` is **2,412 bytes**.
At original relative `+0x8B8` the real instruction is
`DEC DWORD PTR [EBP+0x1414]`, which decrements the existing signed
`EnemyRuntimeView::damageFlashFrames` field as a low 32-bit hardware word.
The old maintained C++ used ordinary signed `--`, which has undefined
behavior at signed underflow. The maintained C++ now preserves the
real four-byte word wrap without changing flash lifetime or branches:

```cpp
--*reinterpret_cast<unsigned int *>(&enemy->damageFlashFrames);
```

The explicit corresponding unsigned type is for the existing signed field;
the target's zero check, flash reset to four, VM flags and original signed
consumers are untouched. This is a VC7.1/i386 semantic model, not proof
of historical C++ spelling, original producer file, runtime state safety,
or a complete game reconstruction.

## Whole owner and original-target evidence

Five original-source-pinned two-TU `EclVm.cpp` + `Enemy.cpp` VC7.1 `/GL /GS`
experiments independently and jointly tested life loss, damage-flash
counter, and animation-script index arithmetic. All five preserve the
entire **2,371-byte/69-real-candidate-field** `EnemyRuntimeUpdate`
contribution and four adjacent complete owners with zero ordinary
candidate differences. The original target is **2,412 bytes**, so the
candidate is still expressly **NONEXACT**. These field positions are
candidate inventory; they are not accepted original target bindings.

The separate ECL `Run` parent is original-target-bound: the actual
two-TU image retains **6,997/7,020 original bytes**, all **189 original
field obligations**, and the independent 171-byte `ReadFloatValue` exact
callee. These full-body facts are preserved, not promoted.

After installing both the previously committed life subtraction and this
new flash decrement, a **fresh selected** canonical cold replay rebuilt
only five related previously exact units across four actual native graphs:

| Existing exact unit | Complete original bytes |
| --- | ---: |
| `ecl-vm-host-run` | 146/146 |
| `ecl-vm-spawn-thread` | 142/142 |
| `enemy-ecl-release-allocations` | 45/45 |
| `enemy-ecl-reset-state` | 71/71 |
| `enemy-manager-update` | 166/166 |

**570/570** exact original codegen bytes with all **14** declared target
linkage fields. Full installed-source `EnemyRuntimeUpdate` PDB contribution
was compared independently against the separate SHA-pinned two-TU baseline:
**2,371/2,371 candidate ordinary code** and identical **69 candidate
field offset/type/width**; the full real `Run` still matches
**6,997/7,020** with 189 original-bound fields. No historical all-unit
cold, new exact function, or partial original byte credit.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-host-run --unit ecl-vm-spawn-thread \
  --unit enemy-ecl-release-allocations \
  --unit enemy-ecl-reset-state --unit enemy-manager-update --json
```

Installed `src/Enemy.cpp` SHA-256:
`50ff057332b846bc97e9629e3ccf70d4becacd22ceaa2168c18a28bedb60bbad`.
Actual complete native images, source variant/trial hashes and selected
original-bound cold receipts are in ignored
`.analysis/gpt-web/20261107-enemy-native-combat-words/`,
`build/gpt-web/20261107-enemy-native-combat-words/`, and
`build/match-linked/{EclVmHostRunEnemy,Enemy,EnemyResetEclState,EnemyManagerUpdate}/`.
No full whole-product/game-runtime claims are made.
