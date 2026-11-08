# EnemyAdvanceMotion: restore x87 operand order and the polar vector spill

The maintained `EnemyAdvanceMotion` source now reproduces the target's
Cartesian X x87 load/add order and the complete polar temporary/result path.
This is a natural C++ reconstruction experiment under the actual 12-source
Enemy/ANM/ECL/Player LTCG graph, not a byte-for-byte owner recovery.

## What changed

- The Cartesian X source now reads the position through a volatile lvalue
  before adding velocity. Without that sequenced read, VC7.1 canonicalized
  the commutative expression to `fld velocity; fadd position`. The original
  instruction is `fld position; fadd velocity`.
- The polar temporary explicitly zeroes its Z word through a volatile
  lvalue and then adds the complete three-component vector. Without the
  visible zero store, LTCG folds away the target's `mov [esp+0x14],0` and
  simplifies the result copy. The original zero store and following
  component-transfer sequence are now reproduced.

The volatile accesses are compiler-ordering controls for these temporary
operations; they are not a claim that the original object fields themselves
were declared volatile.

## Complete leaf comparison

The current-source candidate is **168 bytes** versus the original **171**.
All **7/7** independently reviewed target linkage fields have the correct
original offsets and referents:

| Offset | Type | Original referent |
| ---: | --- | ---: |
| `+0x32` | REL32 | `FromAngleMagnitude @ 0x0044C5D0` |
| `+0x76` | DIR32 | binary32 `100.0 @ 0x00470B44` |
| `+0x7E` | REL32 | floor routine `0x00452FF0` |
| `+0x84` | DIR32 | binary32 `0.01 @ 0x00470B00` |
| `+0x8F` | DIR32 | binary32 `100.0 @ 0x00470B44` |
| `+0x97` | REL32 | floor routine `0x00452FF0` |
| `+0x9D` | DIR32 | binary32 `0.01 @ 0x00470B00` |

Across the complete 168-byte candidate prefix, excluding those 28 linkage
bytes, **133/140** ordinary bytes match. The only seven remaining ordinary
mismatches are `+0xA1..+0xA7`, all in the final stack cleanup/store sequence:

```text
original:   add esp, 8; fstp [esi+4]; add esp, 0x18; ret
candidate:  fstp [esi+4]; add esp, 0x20; ret
```

The candidate therefore remains **NONEXACT**. This change adds no exact
function or exact-byte ledger credit.

## Parent and regression guards

The full `EnemyRuntimeUpdate` remains **2,412/2,412 bytes in extent** and
**1,578/2,136** relocation-normalized ordinary bytes with 69 detected
candidate fields. Those 69 fields are candidate-derived, not a complete
original-bound parent contract. The leaf change does not claim to close the
boss function.

Seven selected pre-existing exact owners cold-pass **933/933** original
bytes in their canonical recipes:

- `EnemyRuntimeVector::FromAngleMagnitude`: 30/30
- `EnemyRuntimeView` constructor: 145/145
- `EnemyScalarInterpolationView::Evaluate`: 652/652
- ECL host constructor: 21/21
- ECL context constructor: 15/15
- ECL resource-base constructor: 34/34
- ECL resource-base destructor: 36/36

`scripts/ci.py` and `git diff --check` pass. No full historical cold replay
was run. The final leaf and parent measurements were freshly linked from
the installed source in the 12-source graph; its eleven support objects and
link harness inputs were SHA-pinned. The full private receipt is
`.analysis/gpt-web/20261108-current/enemy-motion-final-current-build.json`.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-runtime-vector --unit enemy-runtime-constructor \
  --unit enemy-evaluate-scalar-interpolation \
  --unit enemy-ecl-host-constructor --unit enemy-ecl-context-constructor \
  --unit enemy-ecl-resource-base-constructor \
  --unit enemy-ecl-resource-base-destructor --json
scripts/repo-python scripts/ci.py
git diff --check
```
