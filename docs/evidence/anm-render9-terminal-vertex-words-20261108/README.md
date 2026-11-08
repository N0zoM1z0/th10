# TH10 ANM ExecuteScript: native renderMode-9 terminal vertex addresses

Original target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The complete original `AnmRenderManagerView::ExecuteScript @0x0043EE30`
occupies **9,964 bytes**, with 350 independently original-bound call,
constant and local table fields. Render mode 9 generates a ring of 0x1C-byte
vertices with a count read directly from the current script. At the tail it
copies the first two generated vertices to the end and replaces their V
texture coordinates. Its second source and destination vertices were
previously written through `vertices[1]`, which relies on a C++ array bound
that the native unchecked i386 instruction stream does not enforce.

The installed natural C++ now uses a small typed forced-inline helper:

```cpp
static __forceinline AnmRenderVertexView *AnmVertexAtWord(
    AnmRenderVertexView *base, unsigned int index)
{
    return reinterpret_cast<AnmRenderVertexView *>(
        reinterpret_cast<unsigned int>(base) +
        index * static_cast<unsigned int>(sizeof(AnmRenderVertexView)));
}
```

It computes a low-32-bit Win32 base plus an index times the actual
`sizeof(AnmRenderVertexView)==0x1C`, used for the terminal second-vertex
copy and V-coordinate write. It preserves the first-vertex copy and its
texture coordinate, all loop ordering, number of vertices and existing
unchecked behavior. This is not an overflow clamp or a portable
pointer-arithmetic guarantee.

## Complete owner and compiler trials

A stale intermediate build was **rejected before editing** because its
source SHA did not match the then-current `AnmManager.cpp` epoch. A fresh
five-TU VC7.1 SP1 `/GL /GS` baseline was rebuilt from the installed
AnmManager, RandomMath, AnmVmCreate, AnmVmId, and AnmInt3Scale sources.
Five isolated terminal-address trials were compiled and linked: second
source only, second destination only, last V field only, first source
and all terminal words together. Every complete 9,964-byte source owner
preserved **9,962/9,964 original bytes and 350/350 original fields**;
the only original byte mismatches remain opcode 91 at offsets
`+0x113,+0x115` (target EDX, candidate ECX). The owner remains
**NONEXACT**.

Two additional natural C++ alternatives were trialed. A named terminal
pointer changed the full owner extent to 9,968 bytes and was rejected.
The chosen typed forced-inline helper preserved the exact same 9,964-byte
candidate and 350 target-bound fields. After installing and formatting
that helper, the **whole five-TU graph was freshly rebuilt** once more
from final source: again **9,962/9,964 and 350/350 fields**, with no new
mismatches. Six affected previously accepted ANM helper owners also
passed their separate exact canonical cold replay:

| Existing exact owner | Original bytes |
| --- | ---: |
| AnmOpcodeVectorView::FromAngleMagnitude | 30/30 |
| AnmLoadedView::SetSprite | 356/356 |
| AnmVmTimerView::Add | 88/88 |
| AnmVmTimerView::Decrement | 20/20 |
| AnmInt3Scale | 56/56 |
| AnmVmView::GetIntVar | 144/144 |
| **Selected total** | **694/694** |

This is a source-semantic checkpoint, **not** a newly exact owner or
canonical byte-credit increase, nor a reconstructed complete executable.
No historical full cold replay was run.

Reproduce only selected ANM helpers:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit anm-vm-timer-add --unit anm-vm-timer-decrement \
  --unit anm-int3-scale --unit anm-loaded-set-sprite \
  --unit anm-get-int-var --unit anm-opcode-vector --json
scripts/repo-python scripts/verify-handoff-pins.py
scripts/repo-python scripts/ci.py
```

Ignored trial source and full PE/MAP/PDB:
`.analysis/gpt-web/20261108-anm-tail-native-vertices.py`,
`.analysis/gpt-web/20261108-anm-tail-cleaner.py`,
`.analysis/gpt-web/20261107-active/anm-tail-native-results.json`,
`anm-tail-clean-results.json`, `anm-tail-selected-replay.json`, `anm-current.json`,
and `build/gpt-web/20261108-anm-tail-{native,cleaner}/`.
