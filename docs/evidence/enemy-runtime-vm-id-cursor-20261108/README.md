# EnemyRuntimeUpdate: shared managed-VM cursor and native register lifetime

The complete `EnemyRuntimeUpdate @0x0040DC80` source now declares its
managed-VM ID cursor once before the `SetVmWorldPosition` / `SetVmPosition`
branch, then advances that same cursor in either loop. The previous source
redeclared a cursor independently inside each branch. This is the ordinary
C++ form that restores the target's register roles in the actual full
Enemy/ANM/ECL/Player caller graph: target EDI holds the VM ID cursor and EBX
holds the render manager; the prior candidate assigned those roles in the
opposite registers. The selected code uses no register keyword or assembly.

## Complete boss diagnostic

The target and candidate owners both remain **2,412 bytes** in the pinned
12-source VC7.1 `/GL` diagnostic graph. Against the complete 671-instruction
target, the source-level instruction-token alignment increases from **647**
to **655** instructions; the token sequence ratio rises from **0.965672** to
**0.977612**, and aligned instruction bytes rise from **2,331** to **2,353**.
The candidate-only relocation-normalized comparison rises from **1,575/2,136**
to **1,578/2,136** ordinary comparable bytes, with **69 detected candidate
fields**. The latter are not 69/69 original-bound fields: this is a
source/shape diagnostic, not an exhaustive original linkage contract. The
owner remains **NONEXACT** and receives no exactness or ledger credit.

The full graph's existing support objects were individually SHA-pinned and
reused; only `src/Enemy.cpp` was compiled for each variant. The final current-
source build was independently linked from that graph and reproduced the
2,412-byte extent, 655/671 aligned instructions and 1,578/2,136 comparison.
The trial matrix and final receipt are under
`.analysis/gpt-web/20261108-current/enemy-runtime-vm-loop-variants.py`,
`enemy-runtime-vm-loop-results.json`, and `enemy-runtime-vm-loop-final.json`.

The selected loop change preserves six previously exact Enemy owners across
their declared canonical cold graphs: ECL host constructor **21/21**, ECL
context constructor **15/15**, ECL resource-base constructor **34/34**, ECL
resource-base destructor **36/36**, Enemy runtime constructor **145/145**,
and Enemy scalar interpolation **652/652** — **903/903 bytes** total.
`scripts/ci.py` and `git diff --check` pass. No full historical cold replay
was run. `EnemyAdvanceMotion` remains **160/171**, and the 30-byte
`SetSubroutine` helper remains nonexact; neither was changed by this patch.

The full-parent alignment is deliberately only a heuristic. It does not
prove source authorship, all relocation referents, exact native ABI, runtime
equivalence, or product closure.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-ecl-host-constructor \
  --unit enemy-runtime-constructor \
  --unit enemy-evaluate-scalar-interpolation --json
scripts/repo-python scripts/ci.py
git diff --check
```
