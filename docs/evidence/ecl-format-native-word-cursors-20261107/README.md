# TH10 ECL FORMAT: original i386 word-sized parameter and text cursors

Target: original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The `ECL_VM_EVALUATE_FORMAT_OPERANDS` case belongs to the complete
`EclVmContext::Run @0x0044E1A0`, **7,020 target bytes** and **189
independently original-bound fields**. It scans a counted inline text
stream, copies each literal prefix, dispatches typed eight-byte format
argument records, and advances to the next `%` directive. The actual
serialized count remains signed for the original **signed divide-by-four**
scalar-index conversion; no guessed alignment or extra input checks have
been added.

## Selected maintained natural C++

The original x86 code uses 32-bit address and index words even at overflow,
where signed C++ multiplication or out-of-array pointer addition is not
well-defined. The maintained C++ now explicitly expresses low 32-bit words
for the **8-byte parameter record stride**, the flag index passed to
`ReadFloatValue` / `ReadIntValue` (`argumentIndex + 1u`), the raw dword
distance between `percent` and `cursor` when splitting literal text,
and the `percent + 2u` next-token byte address. The original `strchr`,
`strcpy`, format character test, typed scalar operands, string length,
argument index declarations and increment sites remain unchanged.
No target-instruction copying, forced register choices, bounds changes,
synthetic ABI, or different argument evaluation order are introduced.

The implementation is pinned to VC7.1 SP1 i386 and does not guarantee
portable arbitrary integer/pointer casts, malformed-script safety or
original source spelling.

## Complete source-codegen trial inventory and rejected traps

The actual current two-input `EclVm.cpp` + `Enemy.cpp` `/GL` graph was
compiled and fully linked for **31 individual or combined FORMAT source
variants**, checking the entire original-bound `Run`, the same target call
fields and independent `ReadFloatValue` callee. This includes eight initial
operations; twelve isolated float/int scalar-index, call-index and increment
controls; five combined source forms; and six focused followups.

The most important rejected hypotheses were not hidden:

- A global `unsigned int argumentIndex` or conversion of **either** scalar
  payload index to unsigned repaired two old SIB bytes (`Run +0xAF`,
  `+0x1FB`) but introduced three **new** differences (`+0x14D7`,
  `+0x168A`, `+0x1710`), yielding **6,996/7,020** instead of
  **6,997/7,020**. Thus changing the scalar index type is **rejected**.
- Two separately byte-neutral operations—explicit unsigned loop increment
  and unsigned format-record stride—**combined** altered register
  allocation across the large owner and regressed to **6,996/7,020**.
  This refutes assuming that separate native-probe successes compose.
- A selected combination of unsigned format stride, call flag index +1u,
  percent-minus-cursor byte distance, and percent+2u cursor **does**
  preserve **6,997/7,020**, the complete 7,020-byte PDB extent, all
  **189 original-bound fields** and the exact float-value callee.

The full source after formatting and comment installation was **recompiled
from the actual maintained source** and checked with the canonical
`diagnose-ecl-run-native.py` original-binding machinery: the 23 remaining
original mismatch offsets are unchanged, with the complete instruction
and jump/selector tables validated. `Run` remains **NONEXACT**.

Only eight directly affected, previously exact owners were cold-replayed
from the final source in the actual one-TU and two-TU ECL contexts:

| Existing canonical owner | Original-bound bytes |
| --- | ---: |
| `ecl-vm-read-float` | 183/183 |
| `ecl-vm-read-int` | 144/144 |
| `ecl-vm-read-float-value` | 171/171 |
| `ecl-vm-resolve-float` | 93/93 |
| `ecl-vm-find-thread` | 36/36 |
| `ecl-vm-stack-pop` | 122/122 |
| `ecl-vm-host-run` | 146/146 |
| `ecl-vm-spawn-thread` | 142/142 |

**1,037/1,037** strict original bytes with actual external bindings.
**Zero new canonical exact functions or codegen bytes** are credited.
No all-historical cold replay, complete rebuilt game product, original
physical-TU attribution or runtime equivalence is asserted.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-host-run --unit ecl-vm-read-float \
  --unit ecl-vm-read-int --unit ecl-vm-read-float-value \
  --unit ecl-vm-stack-pop --unit ecl-vm-resolve-float \
  --unit ecl-vm-find-thread --unit ecl-vm-spawn-thread --json
```

Final `src/EclVm.cpp` SHA-256:
`918aa4be269e7d60635cc267da980b9b99182990352651882a2c340cde556481`.
True compiled PE/MAP/PDB and source-specific full trial receipts are retained
as ignored local evidence under
`.analysis/gpt-web/20261107-ecl-format-{native-index,scalar-selector,selected-words,word-select2}/`,
`build/gpt-web/20261107-ecl-format-{native-index,scalar-selector,selected-words,word-select2}/`,
and current `build/match-linked/{EclVmStack,EclVmHostRunEnemy}/`.
The additional eleven early `RETURN` and thread control codegen trials are
retained in `.analysis/gpt-web/20261107-ecl-run-return-spawn/` and likewise
were not accepted as exact progress.
