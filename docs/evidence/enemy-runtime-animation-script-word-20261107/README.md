# EnemyRuntimeUpdate: original i386 animation script index dword ADD

Japanese TH10 original v1.00a SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The real target `EnemyRuntimeUpdate @0x0040DC80` is **2,412 bytes**.
At relative `+0x6DF` it loads the signed script-base from the existing
`EnemyRuntimeView::animationBaseScript` at Enemy runtime offset `+0xF4`,
and at `+0x6E5` executes **`ADD EDX,ECX`** before passing the resulting
word to the real `AnmSetVmScriptIndexAndExecute` service. The added
`scriptOffset` is selected from the existing three-way facing direction
mapping; no new animation mode or bounds check is justified by the target.

Previously the maintained C++ used `animationBaseScript + scriptOffset`
as signed C++ arithmetic, which may be undefined at the signed boundary
although native i386 ADD wraps the low word. The source now uses the
corresponding **unsigned 32-bit word addition** before converting back to
the same `int` script index, preserving the original direction selection,
animation state publication, receiver, and ANM callee call order:

```cpp
AnmSetVmScriptIndexAndExecute(&enemy->managedVmIds[0],
    static_cast<int>(
        static_cast<unsigned int>(enemy->animationBaseScript) +
        static_cast<unsigned int>(scriptOffset)));
```

The installed source is pinned to 32-bit VC7.1 SP1. This does not prove
historical source spelling, runtime behavior for untrusted indexes, ABI
portability, or a complete rebuilt game product.

## Current two-TU full native owner and selected original-bound guard

Five actual natural-source experiments were compiled in the current
`EclVm.cpp` + `Enemy.cpp` linked graph, separately and together: life
loss, damage flash decrement, animation script index addition, their
pair and all three. Each preserved the complete original compiler-
produced **2,371-byte EnemyRuntimeUpdate candidate**, all **69 candidate
linked field positions/types/widths**, and every ordinary candidate
byte. Other complete same-graph `SetSubroutine`30/1,
`EclVmContext::Run`7020/189, `FindSubroutine`136/0 and
`StartSubroutine`551/4 similarly preserved full candidate geometry.
This Enemy update remains **NONEXACT** versus its 2,412-byte original;
no candidate-to-candidate comparison is mistaken for original exactness.

After installing all three native integer corrections in successive
commits, a fresh selected original-bound cold was run from the **final
source** in four related actual graph artifacts. All five previously
canonical exact owners passed:

| Existing exact owner | Original bytes |
| --- | ---: |
| `enemy-manager-update` | 166/166 |
| `ecl-vm-host-run` | 146/146 |
| `ecl-vm-spawn-thread` | 142/142 |
| `enemy-ecl-release-allocations` | 45/45 |
| `enemy-ecl-reset-state` | 71/71 |

**570/570** original bytes and all **14** independently declared
external fields. The installed two-TU image additionally retains the
strict original-target **6,997/7,020** `Run` with all **189** independently
bound original code/data fields and exact float-value callee, and the
Enemy update **2,371 candidate bytes/69 fields** identical to the source-
pinned cold baseline. No additional exact function/codegen credit, whole
historical cold replay or game-executable claim is made.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-manager-update --unit ecl-vm-host-run \
  --unit ecl-vm-spawn-thread --unit enemy-ecl-release-allocations \
  --unit enemy-ecl-reset-state --json
```

Installed `src/Enemy.cpp` SHA-256:
`5b9ca4dfcd672a8ca6cc7cabb276f1ca831594b0c816755d6ece8914677e8380`.
Native trial PE/MAP/PDB, source/field geometry and final original-bound
selected-cold receipts:
`.analysis/gpt-web/20261107-enemy-native-combat-words/`,
`build/gpt-web/20261107-enemy-native-combat-words/`,
`build/match-linked/{EclVmHostRunEnemy,Enemy,EnemyResetEclState,EnemyManagerUpdate}/`.
