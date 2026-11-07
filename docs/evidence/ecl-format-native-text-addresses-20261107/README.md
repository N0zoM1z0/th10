# TH10 ECL Run FORMAT: native scratch, percent and typed-argument addresses

Original target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
`EclVmContext::Run @0x0044E1A0` is the complete **7,020-byte** original
interpreter, including 60 jump-table entries and its 88-byte selector
suffix. This current source is still **NONEXACT**.

The FORMAT opcode scans inline `%` fields with an unchecked 0x400-byte
scratch allocation and a counted typed argument record. The target's
i386 stores a string terminator using a low-32-bit base plus the
`percent - cursor` byte distance (`MOV BYTE PTR [ECX+EDX],0` near Run
`+0x1937`), reads the post-percent character with `[EAX+1]`, and
forms argument-record addresses through native dword additions. The
previous C++ used `scratch[unsigned-difference]`, `percent[1]` and
`instruction->operands + sizeof(int) + inlineBytes + index*stride`.
On malformed oversized inline data these operations could form invalid
abstract-array pointers; original Win32 does arithmetic on flat 32-bit
addresses instead. The source now spells those three accesses as
explicit native `unsigned int` word additions with the same types,
format-string traversal, signed inline-length interpretation, argument
iteration and error behavior. **No buffer clamp or malformed-script
safety guarantee has been added.**

## Full real two-TU original-bound experiments

Four **different natural C++ source variants** were compiled with the
pinned VC7.1 SP1 `/GL` EclVm.cpp plus Enemy.cpp graph, each linked with
the actual `EnemyRuntimeUpdate` entry and checked over the **entire**
`Run` PDB contribution, all original-bound fields, and the previously
exact 171-byte ReadFloatValue callee:

| Source variant | Full original-byte match | Original fields | Result |
| --- | ---: | ---: | --- |
| Native scratch terminator | 6,997/7,020 | 189/189 | Preserved |
| Native percent-char read | 6,997/7,020 | 189/189 | Preserved |
| Native FORMAT record address only | 6,996/7,020 | 189/189 | Regressed; rejected alone |
| **All three combined** | **6,997/7,020** | **189/189** | **Installed** |

Importantly, the record-address variant alone changes unrelated register
lifetime and loses one byte, while the combined natural source restores
the prior whole-function match. The codegen comparison does *not* simply
mask the affected block. All 60 jump-table bindings and local branch
geometry remain checked. The 23 remaining ordinary byte mismatches
span VM loop/callback, LENGTH_SQUARED, POINT_ANGLE and FORMAT stack
homes. This owner receives **zero new exact codegen credit**.

After minor formatting of the accepted combined source, the complete
Run was independently compared from current source: **7,020 bytes,
6,997/7,020 original bytes, all 189 target-bound fields**, with exact
float-value callee. The diagnostic exits 1 because Run is nonexact;
that is expected, not a failed compiler/linker invocation.

An independent selected canonical cold rebuild also verified eight
previously accepted exact VM units, in their correct linked contexts:

| Existing exact unit | Original bytes |
| --- | ---: |
| ecl-vm-read-int | 144/144 |
| ecl-vm-read-int-value | 124/124 |
| ecl-vm-read-float-value | 171/171 |
| ecl-vm-find-subroutine | 136/136 |
| ecl-vm-stack-push | 96/96 |
| ecl-vm-stack-pop | 122/122 |
| ecl-vm-host-run | 146/146 |
| ecl-vm-spawn-thread | 142/142 |
| **Total** | **1,081/1,081** |

Focused reproduction only, not the historical all-unit cold replay:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-host-run --unit ecl-vm-spawn-thread \
  --unit ecl-vm-find-subroutine --unit ecl-vm-read-float-value \
  --unit ecl-vm-stack-push --unit ecl-vm-stack-pop \
  --unit ecl-vm-read-int --unit ecl-vm-read-int-value --json
scripts/repo-python scripts/verify-handoff-pins.py
scripts/repo-python scripts/ci.py
```

Complete local trial PE/PDB/MAP and selected strict receipt:
`.analysis/gpt-web/20261107-active/ecl-format-native-byte-address-trials.py`,
`ecl-format-native-byte-results.json`,
`ecl-format-native-final-run.json`,
`ecl-format-native-final-exact.json`, and
`build/gpt-web/20261107-ecl-format-native-address/`.
Current source SHA-256:
`62cbc480c658beec6fca953680cb1267faf546d6dcaf3d19d0937472f4c74766`.
