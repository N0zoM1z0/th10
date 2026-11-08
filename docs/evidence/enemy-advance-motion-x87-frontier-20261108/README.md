# EnemyAdvanceMotion: recovered Cartesian and polar x87 paths

`EnemyAdvanceMotion @0x0044C2A0` is a small but nontrivial shared motion
primitive used three times by the large `EnemyRuntimeUpdate` owner. The
maintained C++ now expresses two compiler-sensitive details explicitly:

- The Cartesian X accumulator is read through a volatile lvalue before
  adding its velocity. This preserves the target's `fld [esi]` then
  `fadd [esi+0x0c]` operand order, rather than allowing LTCG to select a
  different register/operand sequence.
- The polar vector's Z word is explicitly materialized through a volatile
  lvalue before the three-component addition. This preserves the target's
  stack temporary and the zero word at the third component. The three-word
  `EnemyRuntimeVectorView` is 0x0C bytes, so this is an in-object field, not
  an out-of-bounds write.

The polar branch now reproduces the target's complete `+0x25..+0x6C`
sequence: angle/magnitude call, x/y x87 loads and additions, the third-word
zero initialization, and all three result stores. The Cartesian branch's
first three additions also match the original instruction order. This was
found by compiling and linking full 12-source VC7.1 `/GL` graph variants,
not by comparing an isolated one-TU object.

## Complete original-bound diagnostic

| Measure | Original | Current candidate |
|---|---:|---:|
| Owner extent | 171 bytes | 168 bytes |
| Decoded instructions | 52 | 51 |
| Aligned instruction tokens | — | 43 |
| Original relocation/data fields | 7 | 7 at the same offsets |
| Ordinary bytes outside those 7 fields | 140 | **133/140 match** |

The seven original-bound fields are the `FromAngleMagnitude` call at
`+0x32`, the `100.0f` constants at `+0x76/+0x8F`, the two `floor` calls at
`+0x7E/+0x97`, and the `0.01f` constants at `+0x84/+0x9D`. The original
referents are respectively `0x0044C5D0`, `0x00470B44`, `0x00452FF0`,
`0x00470B00`, `0x00470B44`, `0x00452FF0`, and `0x00470B00`.

The remaining seven ordinary-byte differences are confined to `+0xA1..+0xA7`.
The target pops the 8-byte `floor` argument before storing Y, then releases
the 0x18-byte local frame (`add esp,8; fstp [esi+4]; add esp,0x18; ret`).
The candidate coalesces those stack adjustments after the store
(`fstp [esi+4]; add esp,0x20; ret`). Multiple natural C++ variations of the
coordinate expression, local lifetime, volatile reference/pointer, scoped
locals, and result temporaries did not remove this final stack-cleanup
coalescing. The function is therefore **NONEXACT**; no exact ledger credit.

The same full-graph build keeps the large parent at **2,412/2,412 bytes**
with **655/671** instruction-token alignment and 69 candidate-detected
fields. Those 69 are not an original-bound relocation contract. Four
selected canonical exact owners remain **848/848**: Enemy ECL host
constructor 21/21, Enemy runtime constructor 145/145, Enemy runtime vector
helper 30/30, and Enemy scalar interpolation 652/652. No full historical
cold replay was run. `scripts/ci.py`, `scripts/verify-target.py`,
`scripts/verify-toolchain.py --check`, and `git diff --check` pass.

The diagnostic receipts and natural-source trial matrices are retained in
`.analysis/gpt-web/20261108-current/` (`enemy-motion-installed-fullgraph.json`,
`enemy-motion-frontier-results.json`, `enemy-motion-floor-lifetime-results.json`,
`enemy-motion-y-reference-results.json`, `enemy-motion-safe-volatile-z-results.json`).
They are research receipts, not runtime or full-product evidence.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-ecl-host-constructor \
  --unit enemy-runtime-constructor \
  --unit enemy-runtime-vector \
  --unit enemy-evaluate-scalar-interpolation --json
scripts/repo-python scripts/ci.py
```
