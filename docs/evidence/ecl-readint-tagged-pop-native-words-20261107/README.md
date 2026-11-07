# ECL ReadInt(-1): native i386 tagged stack-pop address words

Target: original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`EclVmContext::ReadInt @0x0044FDB0` is a **144-byte** original-target exact
leaf. If the encoded script scalar operand is `-1`, the original function
first checks the VM stack top minus four as a **signed low-32-bit dword**
(`LEA` / signed branch), then consumes two native four-byte words: the
scalar payload and its preceding one-byte type tag in a 4-byte slot.
When the tag says `f`, the floating payload is converted to integer using
the original x87 conversion semantics; all other tag and indirect cases
preserve their original virtual reader paths and signed comparisons.

The previous C++ spelling still used signed `stack.stackTop - 4` and
`stack.stackTop -= 4`, which can overflow a signed `int` in the C++
abstract machine, even though the original i386 always wraps the low
32-bit register word. The source also used array-pointer addition when
forming the two stack slot addresses, which is not generally defined
outside the bounds of the declared `data[0x1000]` C++ array. The maintained
`ReadInt(-1)` path now expresses the check, both decrements and both
address calculations as **unsigned 32-bit native words**, converting the
result back to the original signed comparison or pointer view as needed.
No new guard, buffer, type tag, ABI, target code bytes or failure behavior
is introduced; pointer semantics are pinned to the actual 32-bit MSVC
compiler and do not assert portable C++ pointer bounds.

## Six actual source variants, full boss + leaf verification

Six genuine VC7.1 SP1 `/GL` one-TU variants were compiled and native-linked
under the real ECL interpreter entry: native signed-guard comparison,
repeated `stackTop` decrement, repeated payload/type address formation,
paired guard/decrement, paired decrement/address, and all three together.
Every trial preserved all **six independently original-bound** scalar
reader/resolver leaves exactly, including the full 144-byte `ReadInt`
and 171-byte `ReadFloatValue` with all its original call fields.
The same graph's **complete** `EclVmContext::Run` remained the previously
verified **6,996/7,020** (one-TU) with all **189** original link and local
control-flow obligations. This one-TU count is deliberately distinguished
from the canonical ECL/Enemy two-TU frontier.

The selected combined source was installed **on top of** the preceding
four native scalar-reader and two native lvalue-resolver changes. A new,
current-source strict original-target cold of only the affected units then
passed in the real one-TU and two-TU contexts:

| Existing exact unit | Original bytes |
| --- | ---: |
| `ecl-vm-read-int` | 144/144 |
| `ecl-vm-read-int-value` | 124/124 |
| `ecl-vm-read-float` | 183/183 |
| `ecl-vm-read-float-value` | 171/171 |
| `ecl-vm-resolve-int` | 58/58 |
| `ecl-vm-resolve-float` | 93/93 |
| `ecl-vm-stack-pop` | 122/122 |
| `ecl-vm-find-subroutine` | 136/136 |
| `ecl-vm-host-run` | 146/146 |
| `ecl-vm-spawn-thread` | 142/142 |

**1,319/1,319** original-target code bytes, all declared real field
identities, in two selected source-specific native linked artifacts. An
independent complete comparison of the selected current ECL/Enemy source
shows `Run` still **6,997/7,020**, all **189** original target fields,
with its separate 171-byte float callee exact. The remaining 23 ordinary
`Run` mismatches are unchanged; neither boss nor any other new owner is
promoted. No full historical cohort replay was run.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-read-int --unit ecl-vm-read-int-value \
  --unit ecl-vm-read-float --unit ecl-vm-read-float-value \
  --unit ecl-vm-resolve-int --unit ecl-vm-resolve-float \
  --unit ecl-vm-stack-pop --unit ecl-vm-find-subroutine \
  --unit ecl-vm-host-run --unit ecl-vm-spawn-thread --json
scripts/repo-python scripts/ci.py
```

Final `src/EclVm.cpp` SHA-256:
`6b5bb981dac3ad26ebf8f6770a154ba2021529d0b96800b35e9bf33f70a26c4a`.
The original-target and individual complete-PE/PDB/COFF proof artifacts,
selected cold and source SHA attestations are retained as ignored local
evidence in `.analysis/gpt-web/20261107-ecl-readint-pop-word/`,
`build/gpt-web/20261107-ecl-readint-pop-word/`, and the final real
`build/match-linked/{EclVmStack,EclVmHostRunEnemy}/` output.
No newly exact owner, full reconstructed game binary, portable-pointer or
runtime-equivalence claim is made.
