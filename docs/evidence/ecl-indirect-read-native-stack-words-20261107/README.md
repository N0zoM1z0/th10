# ECL indirect stack scalar reads: native i386 low-word frame arithmetic

Original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

Six actual ECL scalar readers/resolvers use the real 0x1000-byte VM stack
buffer with a four-byte signed frame-base field and script-supplied int or
float-offset words. The retail i386 code loads these operands and adds the
low 32-bit dwords (observed explicitly in `ReadIntValue`, `ReadInt`,
`ReadFloatValue` and `ReadFloat`), then reads the target scalar.

The prior maintained C++ read paths formed an address with expressions like
`stack.data + stack.frameBase + value`. C++ array pointer arithmetic can be
undefined for an offset that crosses the stack buffer's C++ array bounds;
script-provided offsets and signed frame words must still be modeled as the
original native **32-bit** hardware additions. The installed source now
computes these address words with `reinterpret_cast<unsigned int>(stack.data)`
plus the unsigned views of the actual signed frame and operand, before
converting the final dword back into the pointer used to read the same
`EclVmScalar` member.

This is installed only in **four distinct read owners**:
`ReadIntValue`, `ReadInt`, `ReadFloatValue`, and `ReadFloat`.
Their existing positive-indirection checks, `-1` pop cases, virtual reader
calls, and precise read/convert order are unchanged. The separate
`ResolveInt`/`ResolveFloat` write-target paths are not modified here;
those require their own full native proof. No synthetic ABI, new bounds
check, copied binary code, or portable C++ pointer guarantee is asserted.

## Real source variants and original-bound verification

Eleven separate ordinary C++ variants were compiled in the real one-TU
VC7.1 SP1 `/GL` ECL graph, each tested against the full six native scalar
reader/resolver PDB bodies and the whole original `EclVmContext::Run`:
read-int-value, read-int, read-float-value, read-float, resolve-int,
resolve-float, combined four reads, combined resolvers, all six together,
and two manual `ReadInt` pop-address/word-decrement controls. All six
accepted leaf bodies remain independently **original-bound exact** and
retain their complete target-linked fields, with no new codegen credit.
The single-TU Run retains its own native baseline **6,996/7,020** and 189
original fields in every tested variant. This single-TU count is distinct
from the two-TU ECL/Enemy Run frontier and is not a regression.

The final installed, commented **read-only** source was selected for strict
original-bound cold replay, covering all six scalar leaf bodies and two
related host/thread leaves in the real ECL/Enemy graph:

| Existing exact owner | Original bytes |
| --- | ---: |
| `ecl-vm-read-int-value` | 124/124 |
| `ecl-vm-read-int` | 144/144 |
| `ecl-vm-read-float-value` | 171/171 |
| `ecl-vm-read-float` | 183/183 |
| `ecl-vm-resolve-int` (regression guard) | 58/58 |
| `ecl-vm-resolve-float` (regression guard) | 93/93 |
| `ecl-vm-host-run` | 146/146 |
| `ecl-vm-spawn-thread` | 142/142 |

**1,061/1,061** original bytes, all canonical bound fields, across two
selected native linked images. The complete two-TU `Run` is independently
verified **6,997/7,020**, all **189/189** target-linked code and table
fields, and separate 171-byte `ReadFloatValue` exact. `Run` remains
**NONEXACT** with 23 ordinary differences. No whole historical cold replay.

Reproduce the scoped acceptance guard:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-read-int-value --unit ecl-vm-read-int \
  --unit ecl-vm-read-float-value --unit ecl-vm-read-float \
  --unit ecl-vm-resolve-int --unit ecl-vm-resolve-float \
  --unit ecl-vm-host-run --unit ecl-vm-spawn-thread --json
```

Maintained source SHA-256:
`75c0e57a1bcad612900332a8a8421d1124a778efe82bf0c461d03f52bd50127a`.
Original-target data and native source-variant receipts are retained under
ignored `.analysis/gpt-web/20261107-ecl-operand-index-word/` and
`build/gpt-web/20261107-ecl-operand-index-word/`, with current selected
`build/match-linked/{EclVmStack,EclVmHostRunEnemy}/` PDB/PE/MAP.
No new exact owner, whole-game executable or runtime-equivalence claim.
