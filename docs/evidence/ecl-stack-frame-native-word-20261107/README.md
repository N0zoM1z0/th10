# ECL stack-frame native 32-bit additions and saved-slot decrement

Target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The target `EclVmStackView::EnterFrame` spans **64 bytes** and `LeaveFrame`
**41 bytes**, both already accepted exact owners. Retail x86 frame arithmetic
uses a low-word `ADD` on the prior top and the signed local-byte size,
compares that result **signed** against `0x1000`, then conditionally reserves
and restores a four-byte saved frame word. LeaveFrame uses native dword
subtraction by four followed by a signed nonnegative test before restoring
the saved frame base. Ordinary signed C++ addition/subtraction would be
undefined at signed overflow, whereas the target hardware wraps the 32-bit
word before the signed test.

The actual maintained code now calculates the frame top, saved-slot bound,
LeaveFrame saved-slot bound and LeaveFrame decrement using unsigned 32-bit
intermediates before restoring their original signed comparisons/field
writes. No new frame storage, clipping, checked arithmetic, size changes,
assembly, fake ABI, or runtime guard was added. Signed conversion from the
low dword is established only under the pinned VC7.1 SP1 i386 compiler.

## Real owner and large interpreter evidence

Seven independent naturally written source variants were compiled in the
real one-TU `EclVmStack` `/GL` graph, including two EnterFrame variants,
three LeaveFrame variants and the combined form. For **every variant** the
four complete PDB-owned stack functions passed the strict original-target
comparator: Push **96/96**, Pop **122/122**, EnterFrame **64/64**, and
LeaveFrame **41/41**. The pinned single-TU `Run` baseline matched
**6,996/7,020**, **189** fields. The combined variant had the same entire
7,020-byte candidate ordinary code and field offset/type/width geometry
as that **same single-TU** baseline. This same-graph 6,996 number is *not*
the distinct ECL/Enemy two-TU baseline of 6,997, so the two must never be
mistaken for a regression or a newly exact byte.

After installing the properly line-ended final source, selected **current
source** canonical cold links built only these affected units in two graph
artifacts. All six preaccepted owners are independently original-bound:

| Selected exact owner | Original bytes |
| --- | ---: |
| `ecl-vm-stack-enter-frame` | 64/64 |
| `ecl-vm-stack-leave-frame` | 41/41 |
| `ecl-vm-stack-push` | 96/96 |
| `ecl-vm-stack-pop` | 122/122 |
| `ecl-vm-find-subroutine` | 136/136 |
| `ecl-vm-host-run` | 146/146 |

Total **605/605** original bytes with every declared real call field.
The guarded two-TU ECL/Enemy image was separately checked against the
**entire** original `EclVmContext::Run`: still **6,997/7,020**, all
**189/189 independently original-target-bound fields**, and its separate
171-byte `ReadFloatValue` callee exact. `Run` remains **NONEXACT**; no new
canonical exact credit is claimed.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-stack-enter-frame --unit ecl-vm-stack-leave-frame \
  --unit ecl-vm-stack-push --unit ecl-vm-stack-pop \
  --unit ecl-vm-host-run --unit ecl-vm-find-subroutine --json
```

Final `src/EclVm.cpp` SHA-256:
`b66651c092c017e996b205e305972d3b0d5306a4b88a1cb5a215c5d055cc9179`.
Actual native trial objects, same-graph baseline check, fresh guarded
PE/MAP/PDB and signed error inventory are retained in ignored
`.analysis/gpt-web/20261107-ecl-frame-native-words/`,
`build/gpt-web/20261107-ecl-frame-native-words/` and
`build/match-linked/{EclVmStack,EclVmHostRunEnemy}/`.
This does not establish whole-game link/runtime behavior or original source
spelling. Full historical cold replay was not performed.
