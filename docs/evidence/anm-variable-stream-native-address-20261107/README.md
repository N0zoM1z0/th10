# TH10 ANM instruction stream: exact native 32-bit byte addresses

Original: Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

ANM instructions in `AnmRawInstructionView` use an actual variable-length
serialized byte record. `Next()` advances by the encoded `unsigned short
size`; `AtByteOffset(int offset)` is used by `JUMP`, `JUMP_DEC`, and
conditional jumps, whose offset may be **negative** and is relative to
the script beginning. Previously the C++ expressed these as a byte pointer
into a *single instruction object* plus offset. That can leave the extent
of that one C++ object even for legal script traversal. Retail i386 uses
the original **32-bit address-word ADD**, with a signed relative offset
retaining its low dword representation.

The maintained `src/AnmManager.hpp` now spells the native additions:

```cpp
return reinterpret_cast<AnmRawInstructionView *>(
    reinterpret_cast<unsigned int>(this) + static_cast<unsigned int>(size));
// Relative byte address, also for negative jumps:
return reinterpret_cast<AnmRawInstructionView *>(
    reinterpret_cast<unsigned int>(this) + static_cast<unsigned int>(offset));
```

This is a source-semantic correction within the real 32-bit Win32 ABI;
no out-of-bounds check, guessed script record, changed branch, inline
assembly, runtime guarantee on malformed input, or fabricated field is
introduced. The two expressions keep separate natural source purposes.

## Actual original-target whole boss, with live five-TU cold source proof

The complete original `AnmRenderManagerView::ExecuteScript @0x0043EE30`
contains **9,964 bytes** and **350 independently original-target-bound
links**, including local jump/data tables. A *fresh* five-input VC7.1 SP1
`/GL /GS` baseline built the current `AnmManager.cpp` and the real
`RandomMath`, `AnmVmCreate`, `AnmVmId`, and `AnmInt3Scale` supporting TUs.
The last TU uses its distinct `/Ob1` support profile. Four independent
full cold builds then tested `Next()` word arithmetic only, relative
`AtByteOffset()` only, both together, and the equivalent commuted-word
form. All variants preserve the **entire 9,964-byte PDB extent**,
**9,962/9,964 original ordinary bytes**, and all **350/350 original
field bindings**. The only remaining two ordinary mismatches are opcode
91 register allocation at `+0x113`/`+0x115` (original EDX, candidate
ECX); the boss remains **NONEXACT**.

The final installed, commented header was separately recompiled in a
**new five-TU native cold** (not just the trial image) and again passed the
same complete-original comparison. The current-source PDB, PE and
field/size receipt are under
`build/gpt-web/20261107-anm-stream-word/installed/`,
`.analysis/gpt-web/20261107-anm-stream-word/installed.json`.

Seventeen additional real opcode-91 C++ register/lifetime experiments
were also compiled against this fresh baseline. `register`, named
script/layer locals, file-pointer snapshots, aggregate initializers and
reference/pointer views did **not** resolve both bytes; some disturbed
other original fields or code. All controls were restored. Results are
in `.analysis/gpt-web/20261107-anm-op91-fresh/results.json`.

## Selected already exact guards

Seven affected canonical original-target units were cold replayed from the
installed header and current TU sources, **not** the historical all-unit
cohort:

| Previous exact unit | Original bytes |
| --- | ---: |
| `anm-loaded-create-vm-variant2` | 73/73 |
| `anm-get-int-var` | 144/144 |
| `anm-get-int-var-ptr` | 132/132 |
| `anm-get-float-var` | 424/424 |
| `anm-get-float-var-ptr` | 156/156 |
| `anm-vm-timer-add` | 88/88 |
| `anm-vm-timer-decrement` | 20/20 |

**1,037/1,037 original bytes**, all declared fields across three actual
native artifacts. The complete boss remains NONEXACT and this commit adds
**zero** newly exact functions or codegen bytes.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit anm-loaded-create-vm-variant2 \
  --unit anm-get-int-var --unit anm-get-int-var-ptr \
  --unit anm-get-float-var --unit anm-get-float-var-ptr \
  --unit anm-vm-timer-add --unit anm-vm-timer-decrement --json
scripts/repo-python scripts/ci.py
```

Installed `src/AnmManager.hpp` SHA-256:
`7baea2b709d535639ab5a3d44a8bb1adb6eefd9c26b70f50a89fc248c5ce0bc3`.
Native trial/installed source hashes, PDB/MAP/PE and strict target receipts:
ignored `.analysis/gpt-web/20261107-anm-{stream-word,op91-fresh}/` and
`build/gpt-web/20261107-anm-{stream-word,op91-fresh}/`.
No full game link, original physical source spelling or runtime behavior
is inferred from the exact local codegen guards.
