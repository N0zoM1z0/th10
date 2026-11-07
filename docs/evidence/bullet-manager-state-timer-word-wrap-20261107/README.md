# TH10 BulletManagerView::UpdateBullets: main-loop state timer word wrap

Original Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

This is a separate original-bound main-loop owner from the seven bullet
transform helpers. `BulletManagerView::UpdateBullets @0x004065C0` is a
complete **249-byte**, six-linkage canonical exact function; its associated
callback is 40 bytes. The main loop traverses 2000 slots, updates draw
bucket lists and advances each surviving bullet's `stateTimer`. After
checking the original scale window, the fast branch previously incremented
its signed `timer.current` using ordinary C++ `++` (undefined overflow at
`INT_MAX`). Native i386 instead advances the **low 32-bit dword** and wraps.
The maintained main-loop source now writes that fact explicitly:

```cpp
AnmVmTimerView &timer = bullet->stateTimer;
// Original signed scale window and previous-time copy are unchanged.
++*reinterpret_cast<unsigned int *>(&timer.current);
```

The unchanged code around this expression retains the exact original
`timer.previous` store, lower/upper floating threshold comparisons,
subframe update, `BulletUpdateRuntime` call and bucket publication order.
This is a natural source reinterpretation of the actual four-byte field,
not a new counter, allocation, synthetic register contract, copied bytes,
or a claim of portable pointer/lifetime behavior.

## Complete current-source original-bound scoped cold

The actual installed source and pinned VC7.1 SP1 `/GL /GS` compiler were
used in a **selected, not all-unit** canonical replay. Four original-bound
owners passed in two actual native linked artifacts:

| Existing exact owner | Complete bytes | Target fields |
| --- | ---: | ---: |
| `BulletManagerView::UpdateBullets` | **249/249** | 6 |
| `BulletManagerUpdateCallback` | **40/40** | 2 |
| `BulletRuntimeView::Deactivate` | **158/158** | 2 |
| `BulletPositionView::IsOutsidePlayfield` | **109/109** | 6 |

Total **556/556 original bytes** and all **16 declared field identities**
pass without added normalization. The 249-byte main loop was exact before
this semantic correction, so **zero new canonical exactness credit** is
claimed. The separate seven transform timer fast-path changes and 484/484
selected guard are independently documented in
[their own source-epoch proof](../bullet-transform-timer-word-wrap-20261107/README.md).

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit bullet-manager-update --unit bullet-manager-update-callback \
  --unit bullet-runtime-deactivate --unit bullet-position-bounds --json
scripts/repo-python scripts/ci.py
```

Final source SHA-256:
`34cb5ceb0c88620a77e3aa9fcf8ab7f19754fc5bbf4bcf292ad844f6da35bba9`.
Selected actual original-bound cold receipt:
`.analysis/gpt-web/20261107-bullet-timer-wrap/manager-selected.json`;
current source-specific real PE/MAP/PDB in
`build/match-linked/{BulletManagerUpdateCallback,BulletRuntimeDeactivate}/`.
No full historical cold replay, new function exactness, complete reconstructed
game executable, runtime equivalence, or original physical source spelling
is inferred from this result.
