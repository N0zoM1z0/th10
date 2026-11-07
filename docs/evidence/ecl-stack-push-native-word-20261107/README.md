# ECL Push: signed comparison after original i386 low-32-bit stack arithmetic

Target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
`EclVmStackView::Push` is a complete **96-byte** existing exact owner.
The original uses native 32-bit ADD for its stack-top + data-size
comparison against `0x1000`, type-tag slot allocation and final data
size advancement, then uses the signed comparison result for the range
check. Ordinary signed C++ `stackTop + size`/`+=size` can overflow even
when the original x86 low word is well-defined.

The maintained source now computes those three actual operations as unsigned
32-bit words and converts back to the original signed comparison/storage,
retaining all original data, type, memcpy, conditional and return behavior:

```cpp
if (static_cast<int>(static_cast<unsigned int>(stackTop) +
    static_cast<unsigned int>(size)) >= 0x1000)
    return -1;
// Existing type-tag write and memcpy are unchanged.
stackTop = static_cast<int>(static_cast<unsigned int>(stackTop) +
    static_cast<unsigned int>(size));
```

The current source also wraps the existing four-byte type-slot increment.
No new bounds clamp, different stack owner, synthetic register choice,
original binary patch or field change was added. This model is pinned to
VC7.1 SP1 i386; it is not a portable C++ pointer safety claim.

## Nine complete-source probes; rejected Pop changes

Nine natural compiler variants were actually built in the pinned native
single-TU ECL graph: each of the three Push operations separately, all
three combined, each of the three Pop operations separately, all three
Pop operations combined, and both functions combined. All nine still
passed independent original-bound **Push96, Pop122, EnterFrame64,
LeaveFrame41** owners. But **original-target exactness of a leaf alone is
not sufficient to install changes to a large interpreter**:

- Every Push-only variant leaves the entire 7,020-byte one-TU `Run` at the
  previously verified same-graph **6,996/7,020** and all **189** fields.
- Pop guard and data decrement variants regress that one-TU Run to
  **6,929/7,020**, Pop tag decrement to **6,949/7,020**, and combined Pop
  variants to **6,962/7,020**. They are rejected and their source is
  restored byte-for-byte; **no Pop change is installed**.
- The separate current two-TU ECL/Enemy `Run` original-bound frontier is
  **6,997/7,020**, with **189/189** target-linked fields and the exact
  171-byte float-value callee. These distinct graph scores are not
  confused with the one-TU score, and `Run` stays NONEXACT.

The combined **Push-only** source is installed. A fresh **selected** original
cold replay compiled only seven affected previous exact owners across two
real linked graphs:

| Existing owner | Exact target bytes |
| --- | ---: |
| `ecl-vm-stack-push` | 96/96 |
| `ecl-vm-stack-pop` | 122/122 |
| `ecl-vm-stack-enter-frame` | 64/64 |
| `ecl-vm-stack-leave-frame` | 41/41 |
| `ecl-vm-read-float-value` | 171/171 |
| `ecl-vm-find-subroutine` | 136/136 |
| `ecl-vm-host-run` | 146/146 |

Total **776/776** original bytes with the true linked fields. The final
source was additionally compared against the complete original two-TU
`Run` (6,997/7,020; 189 fields) and its float-value callee exact.
**Zero newly exact functions or codegen bytes** are claimed. No historical
all-unit cold replay or game-executable link was attempted.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-stack-push --unit ecl-vm-stack-pop \
  --unit ecl-vm-stack-enter-frame --unit ecl-vm-stack-leave-frame \
  --unit ecl-vm-read-float-value --unit ecl-vm-find-subroutine \
  --unit ecl-vm-host-run --json
```

Final `src/EclVm.cpp` SHA-256:
`af4c45e9404e046d33cb7a7d891b6fdbf3e44b08c11412db20dff3a3b2eee4a0`.
Original-bound/source-epoch results, restored control variants and selected
cold receipts are ignored local evidence under
`.analysis/gpt-web/20261107-ecl-stack-push-pop-word/` and
`build/gpt-web/20261107-ecl-stack-push-pop-word/`; final native images at
`build/match-linked/{EclVmStack,EclVmHostRunEnemy}/`.
