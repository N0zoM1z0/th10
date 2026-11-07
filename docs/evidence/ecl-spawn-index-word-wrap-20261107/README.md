# ECL Run: preserve native modulo-32 spawn inline-offset index

Target: original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The **complete 7,020-byte** `EclVmContext::Run @0x0044E1A0` remains
NONEXACT, not a new canonical match.

## Actual original opcode and C++ correction

`ECL_VM_SPAWN_THREAD_WITH_ID` reads a signed 32-bit inline byte count at
instruction operand+0x10. The original `Run+0xE2..+0xEE` loads the count,
performs the low-word x86 `ADD ECX,4`, then logically `SHR ECX,2`, before
reading the following script dword. This native arithmetic wraps at
`INT_MAX`; its unsigned shift does not depend on the signed value after
addition. The previous C++ performed signed `inlineBytes + 4` **before**
converting the sum to unsigned. For extreme encoded words, that signed
addition is undefined behavior, even though the target operates on one
32-bit word.

The installed source now performs the conversion *before* adding 4u:

```cpp
const unsigned int operandIndex =
    (static_cast<unsigned int>(current->spawnThreadOperands.inlineBytes)
        + 4u) >> 2;
```

The original `ReadIntValue(1, OperandInt(current, operandIndex))` call and
thread-creation control path remain untouched. This is not a new bounds
check, altered value width, fake ABI, or target-byte patch; the existing
serialized ECL instruction prefix remains the same signed dword view.

## Current-source, original-bound evidence

A fresh actual VC7.1 SP1 `/GL` two-translation-unit native build links
`src/EclVm.cpp` and its real `src/Enemy.cpp` support under the canonical
`EnemyRuntimeUpdate` entry. The complete **7,020-byte** `Run` PDB owner,
**189 independently original-bound fields** (including original code locals,
60 jump-table words and selector span) and the same entire ordinary code
profile are retained before and after this source change:

| Measure | Before | Installed source |
| --- | ---: | ---: |
| Run PDB owner | 7,020 | **7,020** |
| Original-bound fields | 189 | **189**, identical geometry/targets |
| Matching original ordinary bytes | 6,997 | **6,997** |
| Differing ordinary bytes | 23 | **23**, identical positions |
| Same-image `ReadFloatValue` | 171/171 | **171/171** |

Current-source diagnostic outputs:
`.analysis/gpt-web/20261107-ecl-spawn-indexword-native.log`,
`.analysis/gpt-web/20261107-ecl-spawn-indexword-run.json`, and actual
PE/MAP/PDB at
`build/gpt-web/20261106-ecl-count-reference/gptweb-20261107-ecl-spawn-indexword/`.
The preceding current-source control is
`.analysis/gpt-web/20261107-ecl-run-strict.json`.

Only these three affected existing exact original-target owners were
selected for canonical cold replay:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-host-run \
  --unit ecl-vm-read-int-value \
  --unit ecl-vm-read-float-value --json
```

All **441/441 previously exact bytes** pass, in two selected native
graphs: host-run 146, ReadIntValue124, and ReadFloatValue171. The
complete large `Run` remains NONEXACT and receives **zero new exactness
credit**. No full historical cold replay, native product/runtime claim,
physical source TU assertion, or portability proof is made. Current
`src/EclVm.cpp` SHA-256 is
`5c47304faae61c3ecb51fe31f8574d80a1dfaa710088c7d7d8cc6ba6110084de`.
