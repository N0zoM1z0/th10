# ECL variable-length instruction jump: original i386 address-word addition

Original target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The canonical original `EclVmContext::Run @0x0044E1A0` occupies **7,020
bytes**. `EclVmInstruction::AtByteOffset` forms the address of the next
variable-length instruction, or the JUMP target using a **signed backward
byte displacement** from the serialized instruction stream. Previously the
maintained C++ computed `reinterpret_cast<unsigned char *>(this) + distance`,
which is undefined for a displacement outside the current C++ array object,
even though original Win32 i386 code adds the low two 32-bit address words.

The maintained `src/EclVm.hpp` now describes that native operation directly:

```cpp
return reinterpret_cast<EclVmInstruction *>(
    reinterpret_cast<unsigned int>(this) +
    static_cast<unsigned int>(distance));
```

The signed distance is preserved as the same four-byte bit pattern. The
32-bit sum wraps without signed-overflow UB, while serialized jump targets,
branch conditions, instruction-size widths and real VM owners are unchanged.
This does **not** add validation or claim arbitrary C++ pointer portability,
original source spelling, or runtime safety for malformed scripts.

## Independently compiled complete boss and scoped exact callee evidence

Four independent natural forms (`native-word`, commuted operands, a named
unsigned address-word local, and an array-index control) were built with
pinned VC7.1 SP1 `/GL` in the real two-input `EclVm.cpp` and `Enemy.cpp`
linked context, with each temporary source edit restored after its probe.
Every original-bound full `Run` comparison retained **6,997/7,020 matching
bytes**, all **189 actual original-linked code fields**, and the exact
171-byte `ReadFloatValue` callee. The same linked graphs retained
FindSubroutine136, StartSubroutine551 and AddScriptData589 complete candidate
PDB extents. This is **not** a newly exact Run; its existing 23 mismatching
bytes remain.

The installed maintained header was then **freshly cold built** for only the
eight directly relevant canonical units in the real one-TU and two-TU ECL
graphs. All selected original-target bodies and binding fields pass:

| Existing exact unit | Target bytes |
| --- | ---: |
| `ecl-vm-read-float` | 183/183 |
| `ecl-vm-read-int` | 144/144 |
| `ecl-vm-read-float-value` | 171/171 |
| `ecl-vm-resolve-float` | 93/93 |
| `ecl-vm-find-thread` | 36/36 |
| `ecl-vm-stack-pop` | 122/122 |
| `ecl-vm-host-run` | 146/146 |
| `ecl-vm-spawn-thread` | 142/142 |

**1,037/1,037 original bytes** across two actual selected build artifacts.
An additional current-source complete `Run` comparison from the guarded
ECL/Enemy image confirms **6,997/7,020, 189 fields**, with the separate
float-value callee **171/171**. No new exact credit, no physical original
compiler-TU claim, and no all-unit cold replay.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-host-run --unit ecl-vm-read-float \
  --unit ecl-vm-read-int --unit ecl-vm-read-float-value \
  --unit ecl-vm-stack-pop --unit ecl-vm-resolve-float \
  --unit ecl-vm-find-thread --unit ecl-vm-spawn-thread --json
```

Installed header SHA-256:
`96764231991c72ff5eebb2d56884fbaafef6406d87d3a751cb53cbf96679d1fc`.
Retained ignored native images and trial result files:
`.analysis/gpt-web/20261107-ecl-instruction-pointer-word/`,
`build/gpt-web/20261107-ecl-instruction-pointer-word/`,
`build/match-linked/{EclVmStack,EclVmHostRunEnemy}/`.
