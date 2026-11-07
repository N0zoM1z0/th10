# EnemyRuntimeUpdate: native life-loss subtraction word

Original game: Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The actual original `EnemyRuntimeUpdate @0x0040DC80` spans **2,412 bytes**.
At original offset `+0x536`, the combat damage branch executes
`SUB DWORD PTR [EBP+0x13C0],ECX`. The field is the actual
`EnemyRuntimeView::life`, and the operand is the original computed
signed damage. The hardware subtraction retains the low 32 bits even
if the signed value underflows; ordinary C++ `enemy->life -= damage`
would invoke signed-overflow undefined behavior in that case.

The maintained source instead subtracts the same operand from the
corresponding unsigned 32-bit word lvalue, without an extra bounds test,
damage conversion, altered branch condition or changed enemy object:

```cpp
*reinterpret_cast<unsigned int *>(&enemy->life) -=
    static_cast<unsigned int>(damage);
```

Signed life/health/zero comparisons elsewhere are unchanged. This is
reconstruction of the verified i386 low-word operation under VC7.1 SP1;
it does **not** establish exact original source spelling, game-runtime
safety or portable behavior for malformed memory state.

## Complete native function and adjacent graph experiment

The **actual current-source** pinned VC7.1 SP1 `/GL /GS` `EclVm.cpp` /
`Enemy.cpp` two-TU graph was rebuilt from scratch. Five independent
natural C++ source variants separately and jointly tested native low-word
life subtraction, the four-byte damage-flash counter decrement, and
animation-script index addition. Each was compiled and linked with
current pin-verified support objects. The complete native PDB-owned
`EnemyRuntimeUpdate` candidate remains **2,371 bytes / 69 candidate
fields** and is **NONEXACT** versus the 2,412-byte original. All five
source controls preserve **zero changed ordinary candidate bytes** and
identical field offset/type/width geometry for the complete Enemy update,
`SetSubroutine` 30/1, `EclVmContext::Run` 7,020/189,
`FindSubroutine` 136/0 and `StartSubroutine` 551/4 in that same source
graph. Matching candidate geometry does **not** confer new target exact
credit on these nonexact large functions.

In every actual variant, independent strict original-target comparison
of the *whole* ECL `Run` retains **6,997/7,020** original bytes and all
**189 independently original-target-bound** links, including the exact
171-byte float-value callee. It also remains NONEXACT.

The life-subtraction change was installed alone, and a fresh selected
original-bound cold replay rebuilt four affected previously exact native
owners across three actual linked artifacts:

| Existing exact function | Original bytes |
| --- | ---: |
| `ecl-vm-host-run` | 146/146 |
| `ecl-vm-spawn-thread` | 142/142 |
| `enemy-ecl-release-allocations` | 45/45 |
| `enemy-ecl-reset-state` | 71/71 |

**404/404** original codegen bytes and their nine real declared linkage
fields passed. The installed-source image also retained the complete
Enemy update **2,371 bytes / 69 candidate fields**, and complete
ECL `Run` **6,997/7,020** / **189 original fields**.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-host-run --unit ecl-vm-spawn-thread \
  --unit enemy-ecl-release-allocations \
  --unit enemy-ecl-reset-state --json
scripts/repo-python scripts/ci.py
```

Installed `src/Enemy.cpp` SHA-256:
`76df11f182b71c7cd88d74caad8daf793e70607b64d5bd3d1fc125a571cb8d1f`.
Actual source-trial hashes, complete PE/MAP/PDB, selected original exact
cold, and Run target-bound reports are retained under ignored
`.analysis/gpt-web/20261107-enemy-native-combat-words/`,
`build/gpt-web/20261107-enemy-native-combat-words/`,
`build/match-linked/{EclVmHostRunEnemy,Enemy,EnemyResetEclState}/`.
No all-unit historical replay, new exact function/bytes or whole-product
reconstruction is claimed.
