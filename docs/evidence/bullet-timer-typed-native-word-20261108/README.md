# Bullet transform timers: typed 32-bit wraparound in seven update paths

The TH10 Bullet transform timer fast path increments its signed current-frame
word with native i386 `INC`, including the `0x7fffffff -> 0x80000000`
transition. The previous maintained source accessed that `int` member as an
unrelated `unsigned int *`. This had the intended 32-bit machine behavior but
used a type-punning store outside ordinary C++ strict-aliasing rules. The
seven complete transform update methods now use:

```cpp
state.timer.current = static_cast<int>(
    static_cast<unsigned int>(state.timer.current) + 1u);
```

The unsigned calculation is modulo 2^32 and then is converted to the native
32-bit signed timer representation. It retains the existing signed stop tests,
previous-frame store, x87 fractional subframe path and transform-state update.
This is a source-semantics improvement, not a new target ABI or an assertion
about the exact spelling of the original source.

## Scoped original-bound cold verification

The current installed BulletManager.cpp is compiled in the declared VC7.1
source graphs for all seven changed and two neighboring accepted owners:

| Original-bound owner | Exact bytes |
| --- | ---: |
| `UpdateDeceleration` | 164/164 |
| `UpdateHorizontalWrap` | 148/148 |
| `UpdateVerticalWrap` | 172/172 |
| `UpdatePolarAcceleration` | 209/209 |
| `UpdateVectorAcceleration` | 305/305 |
| `UpdateRelativeDirectionChange` | 338/338 |
| `UpdateAimedDirectionChange` | 431/431 |
| `UpdateBoundaryBounce` | 446/446 |
| `UpdateState8` | 306/306 |
| **Total prior exact codegen preserved** | **2,519/2,519** |

In the real seven-input `BulletHorizontalWrap` link, the **complete**
`UpdateAbsoluteDirectionChange` still occupies exactly 335 bytes, with
**333/335** original bytes and **8/8** independently original-bound fields.
The two ordinary differences at `+0xD7,+0xDD` still represent EAX versus
original EDX while passing the raw angle into FromAngleMagnitude. This owner
remains NONEXACT and gets no partial/new exactness credit. `scripts/ci.py`
and `git diff --check` passed; no full cold replay was requested.

Selected replay receipts:
`.analysis/gpt-web/20261108-current/bullet-native-timer-guards.json` and
`bullet-timer-state8-guards.json`. The original-bound 335-byte diagnostic
was generated via `absolute-strict.py` in
`.analysis/gpt-web/20261106-bullet-vector-accel/` from the newly compiled
`build/match-linked/BulletHorizontalWrap` artifact. These are local source
and codegen checks, not a whole-game or runtime identity claim.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit bullet-update-deceleration --unit bullet-update-horizontal-wrap \
  --unit bullet-update-vertical-wrap --unit bullet-update-polar-acceleration \
  --unit bullet-update-vector-acceleration --unit bullet-update-relative-direction \
  --unit bullet-update-aimed-direction --unit bullet-update-state8 \
  --unit bullet-update-boundary-bounce --json
```
