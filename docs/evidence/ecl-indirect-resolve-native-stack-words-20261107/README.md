# ECL ResolveInt/ResolveFloat: original i386 lvalue-address arithmetic

Original: Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The original two ECL destination lvalue resolvers use real VM stack
frame offsets: `EclVmContext::ResolveInt @0x00450030` is a complete
**58-byte** owner; `ResolveFloat @0x00450070` is a complete **93-byte**
owner. Direct original code shows native 32-bit `ADD` operations to
combine the current stack byte buffer, signed frame base and the indirect
script operand, after the original sign/indirection predicate.

Previously the C++ formed the destination address using pointer addition
inside an `unsigned char data[0x1000]` array. Even when the script
calculation legitimately refers to variable-length stack views, ordinary
C++ addition past the declared array object can be undefined; the retail
Windows/i386 code instead forms the low **32-bit** integer address word.

The maintained `ResolveInt` and `ResolveFloat` implementations now use:

```cpp
reinterpret_cast<unsigned int>(operandStack->data) +
static_cast<unsigned int>(operandStack->frameBase) +
static_cast<unsigned int>(...script operand...)
```

before converting the result to the same actual `EclVmScalar*` lvalue.
The original signed positive-index filtering, float-to-int offset
conversion, returned `integer`/`real` member address, and real virtual
host resolver fallbacks remain unchanged. No range check, fake ABI,
synthetic data, compiler-register forcing, or target binary patch is
introduced. Exact behavior is scoped to pinned MSVC 7.1 32-bit codegen,
not a portable C++ alias/pointer guarantee.

## Two independently accepted source stages, not just leaf matching

The preceding ECL scalar-read commit installed analogous low-word arithmetic
in **four readers** and validated its current-source complete two-TU
`EclVmContext::Run` owner. Before this lvalue change, six separate
resolver/reader operations and the all-six combination were compiled with
pinned VC7.1 SP1 `/GL`, separately checking every native scalar leaf and
**the entire 7,020-byte** interpreter, not only its first instructions.
Both individual lvalue changes and their combined form were byte-neutral
in the one-TU ECL source graph: all six scalar getter/resolver target
owners remained strict exact, and Run retained that graph's independently
verified **6,996/7,020** and 189 target-linked fields.

The combined resolvers were then *installed on top of the already
committed reader changes*, not substituted for the live worktree.
A separate selected canonical cold built only nine affected units across
two native actual linked artifacts, and all pass the **original target**:

| Existing exact unit | Original bytes |
| --- | ---: |
| `ecl-vm-resolve-int` | 58/58 |
| `ecl-vm-resolve-float` | 93/93 |
| `ecl-vm-read-int-value` | 124/124 |
| `ecl-vm-read-int` | 144/144 |
| `ecl-vm-read-float-value` | 171/171 |
| `ecl-vm-read-float` | 183/183 |
| `ecl-vm-find-subroutine` | 136/136 |
| `ecl-vm-host-run` | 146/146 |
| `ecl-vm-spawn-thread` | 142/142 |

**1,197/1,197** original bytes, including all declared original-linked
fields, exact. The separate complete ECL/Enemy **two-TU** original-bound
`Run` stays **6,997/7,020** with **189/189** fields and its exact
171-byte float-value callee. This is an independently current-source
verification, not a transfer of the one-TU 6,996 number.

Only the affected nine units were replayed. Neither `Run` nor another
nonexact owner is promoted; there are **zero new exact codegen bytes**.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-resolve-int --unit ecl-vm-resolve-float \
  --unit ecl-vm-read-int-value --unit ecl-vm-read-int \
  --unit ecl-vm-read-float-value --unit ecl-vm-read-float \
  --unit ecl-vm-host-run --unit ecl-vm-spawn-thread \
  --unit ecl-vm-find-subroutine --json
```

Installed `src/EclVm.cpp` SHA-256:
`99c972b17343009accd42385987630a0bd771ef33e07d2830484ac19e642899b`.
Retained ignored original-bound source trial/selected cold receipts:
`.analysis/gpt-web/20261107-ecl-operand-index-word/`,
`build/gpt-web/20261107-ecl-operand-index-word/`, and current
`build/match-linked/{EclVmStack,EclVmHostRunEnemy}/` PE/MAP/PDB.
No all-unit cold replay, complete runnable game or original source-TU
identity is established.
