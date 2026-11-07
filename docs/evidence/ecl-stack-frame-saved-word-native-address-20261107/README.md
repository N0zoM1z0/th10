# EnterFrame and LeaveFrame: retail i386 saved-frame word positions

Original: Japanese TH10 1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The complete original `EclVmStackView::EnterFrame @0x00450690` has **64
bytes**, and `LeaveFrame @0x004506D0` has **41 bytes**. Both are already
canonical exact. On entering, the retail i386 code saves `frameBase` to
its stack byte buffer by `MOV [EAX+ECX],ESI` and then executes
`ADD DWORD PTR [EAX+0x1000],4` to advance the actual `stackTop` word.
On leaving, the original reads the saved word through `[EAX+ECX]` after
the target's signed nonnegative check. The byte-buffer address may be
formed from script-dependent stack word positions outside a single C++
subobject; i386 uses native low 32-bit address addition. The ordinary
signed C++ `+=4` also has undefined behavior on overflow, unlike the
original dword ADD.

The maintained C++ now spells **three native word operations**:
EnterFrame saved slot as unsigned `data + stackTop` word,
EnterFrame dword `stackTop + sizeof(EclVmStackSavedWordView)` before
publishing signed stored top, and LeaveFrame saved slot as unsigned
`data + stackTop` word. Its original signed comparisons, update order,
old-frame fallback, exact real field owners and return results remain.
No new bounds checks, ABI constraints, additional frame data, target
code bytes or claimed portable pointer semantics are introduced.

## Complete native source trials and final installed strict cold

Nine independent real VC7.1 SP1 `/GL` source variations tested these
three operations and the separate two Push destinations one by one and
in combinations. All trials preserved full original-bound Push96,
Pop122, EnterFrame64 and LeaveFrame41 exact bytes. The **same one-TU**
7,020-byte `Run` graph remained **6,996/7,020** with 189 original fields;
this is not the separate two-TU ECL/Enemy graph.

The installed source now contains the previously committed Push
byte-address operations *plus* this Enter/Leave pair. It was
independently original-bound selected-cold replayed across two actual
compiler artifacts for seven previously canonical exact owners:

| Existing unit | Original bytes |
| --- | ---: |
| `ecl-vm-stack-push` | 96/96 |
| `ecl-vm-stack-pop` | 122/122 |
| `ecl-vm-stack-enter-frame` | 64/64 |
| `ecl-vm-stack-leave-frame` | 41/41 |
| `ecl-vm-find-subroutine` | 136/136 |
| `ecl-vm-read-float-value` | 171/171 |
| `ecl-vm-host-run` | 146/146 |

**776/776** original bytes with all nine declared field identities.
The separate *complete* current ECL/Enemy two-TU `Run` remains
**6,997/7,020**, **189/189 independently target-bound fields**, and its
171-byte real float-value callee exact. Large `Run` stays **NONEXACT**;
there is **zero** new canonical exact credit. No all-history replay.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-stack-push --unit ecl-vm-stack-pop \
  --unit ecl-vm-stack-enter-frame --unit ecl-vm-stack-leave-frame \
  --unit ecl-vm-find-subroutine --unit ecl-vm-read-float-value \
  --unit ecl-vm-host-run --json
```

Final maintained source SHA-256:
`65909682e0b1bb3e9d6347e3e36311babc745eb7ef0f4aa65efa3990144cb5d7`.
Real native PE/PDB/MAP variants and final-source selected cold receipts
are retained under ignored `.analysis/gpt-web/20261107-ecl-frame-remaining-word/`,
`build/gpt-web/20261107-ecl-frame-remaining-word/` and
`build/match-linked/{EclVmStack,EclVmHostRunEnemy}/`.
