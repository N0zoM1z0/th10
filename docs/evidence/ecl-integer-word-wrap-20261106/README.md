# ECL interpreter: signed-overflow-free native word operations

The original Japanese TH10 1.00a ECL VM executes its integer stack
operations in 32-bit x86 registers. `EclVmScalar` already contains both the
signed integer operand and an `unsigned int bits` view of the **same four
bytes**. The maintained `EclVmContext::Run @0x0044E1A0` now uses this word
view for integer ADD, SUBTRACT, MULTIPLY, NEGATE and POST_DECREMENT. The
original signed comparisons, division, modulo, stack tags, operand evaluation,
value-forwarding and instruction advance paths are unchanged.

The old `left.integer += right.integer` and analogous source expressed signed
C++ overflow as undefined behavior even though 32-bit x86 native ADD/SUB/IMUL
naturally wrap modulo 2^32. The new C++ expressions deliberately compute the
low 32-bit result using the already modeled binary-word view; negation also
handles `INT_MIN` as `0u - bits`. Post-decrement stores the 32-bit result back
through the original signed destination, with the ordinary VC7.1 32-bit
conversion semantics. No additional stack field, ABI trick or assembly is
introduced. Signed integer DIV/MOD are intentionally untouched.

## Original-bound native scope

A fresh **two-TU** VC7.1 SP1 `/GL` compilation of `src/EclVm.cpp` and
`src/Enemy.cpp` using real `EnemyRuntimeUpdate` entry (not a full-game build)
retains the complete **7,020-byte** ECL Run PDB contribution. The target-bound
diagnostic passes **all 189 relocation/local/table fields**, including the
complete 60-target jump table and 88 selector bytes; ordinary bytes remain
**6,997/7,020**. The existing 23-byte register/stack-home mismatch is neither
fixed nor enlarged. The same-image original-bound `ReadFloatValue` callee
remains **171/171 exact**.

Selected independent cold replay only (no unbounded all-unit replay):

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-read-int-value --unit ecl-vm-read-float-value --json
```

Both pass: **124 + 171 = 295/295 bytes**, one selected native artifact, all
original external fields correct. Proofs are retained privately in
`.analysis/gpt-web/20261106-chat-th10/ecl-wrap-final-run.json`,
`ecl-wrap-selected-replay.json`, and the native PE/MAP/PDB under
`build/gpt-web/20261106-ecl-count-reference/gptweb-arithmetic-wrapping-final/`.
Installed `src/EclVm.cpp` SHA-256 is
`f7d7e6e1aa4a1939cae3a05e03f883d3725d18ad8d9f599a8915a066571929fb`.

This is a **source-level semantic reconstruction improvement**, not a new
whole-owner exactness claim or evidence of runnable native-product closure.
