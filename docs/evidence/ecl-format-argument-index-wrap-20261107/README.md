# ECL Run FORMAT: recover native i386 argument-index dword increment

Target: Japanese TH10 1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The original `EclVmContext::Run @0x0044E1A0` is one complete **7,020-byte
owner** with **189 independently original-bound code/data fields**. In
`ECL_VM_EVALUATE_FORMAT_OPERANDS`, a script's counted format text selects
integer/float tagged 8-byte argument records; each consumed scalar
increments the original `argumentIndex` dword. The previous C++ used
signed `++argumentIndex` in both cases, which is undefined upon signed
overflow, whereas i386 advances that same four-byte counter word modulo
2^32. The maintained source now increments the **existing** `int` lvalue
through its corresponding unsigned four-byte word:

```cpp
++*reinterpret_cast<unsigned int *>(&argumentIndex);
```

This is done in **both** real `FORMAT` scalar cases, without changing
encoded values, earlier script operand lookups, signed downstream index
interpretations, thread state, target code bytes or genuine callees.
This is an i386/VC7.1 semantic model, not a new bounds check, original
source-spelling claim or portable host overflow guarantee.

## Six independently compiled whole-Run variations

Six natural source variants were actually compiled and linked in the
real current-source pinned VC7.1 SP1 `/GL /GS` two-TU ECL/Enemy graph:
unsigned-lvalue increment alone, unsigned assignment increment alone,
word-based serialized argument pointer alone, two combined forms, and
alternate local order of that pointer sum. Each was compared against
the **complete** original 7,020-byte PDB owner and its 189 original
field obligations (including all local jump tables and selector data).

| Source variant | Original-bound ordinary bytes | Fields |
| --- | ---: | ---: |
| Native unsigned-lvalue increments (installed) | **6,997/7,020** | **189/189** |
| Word assignment increments | 6,996/7,020 | 189/189 |
| Explicit native argument-record pointer sum | 6,996/7,020 | 189/189 |
| Combined lvalue increment and pointer sum | 6,996/7,020 | 189/189 |
| Combined assignment and pointer sum | 6,996/7,020 | 189/189 |
| Alternate pointer word addition order | 6,996/7,020 | 189/189 |

The five regressing variants were rejected and all temporary source
modifications restored between trials. The selected form was then
**installed**. A separate current-source selected exact cold replay
compiled seven preaccepted original-bound functions in two actual graphs:

| Previously exact owner | Original bytes |
| --- | ---: |
| `ecl-vm-read-float` | 183/183 |
| `ecl-vm-read-int` | 144/144 |
| `ecl-vm-read-int-value` | 124/124 |
| `ecl-vm-read-float-value` | 171/171 |
| `ecl-vm-find-subroutine` | 136/136 |
| `ecl-vm-stack-push` | 96/96 |
| `ecl-vm-host-run` | 146/146 |

**1,000/1,000** original bytes with all declared original field
identities. Full *installed-source* ECL/Enemy `Run` diagnostic remains
**6,997/7,020**, **189/189 original fields**, and the separate 171-byte
float-value callee exact. The remaining 23 ordinary differences are
**NONEXACT** and receive zero canonical exact credit. No historical
all-unit cold was run or complete rebuilt product claimed.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-host-run --unit ecl-vm-read-int-value \
  --unit ecl-vm-read-float-value --unit ecl-vm-read-int \
  --unit ecl-vm-read-float --unit ecl-vm-stack-push \
  --unit ecl-vm-find-subroutine --json
```

Final `src/EclVm.cpp` SHA-256:
`7e3fc6a3739d35adb5fcb1551bff3718130f3926ef5e7364d4ef604e837d40c9`.
Source epochs, native PE/MAP/PDB control links and current-source
strict original receipts are retained under ignored
`.analysis/gpt-web/20261107-ecl-format-word-operand-base/` and
`build/gpt-web/20261107-ecl-format-word-operand-base/`, plus selected
`build/match-linked/{EclVmStack,EclVmHostRunEnemy}/`.
