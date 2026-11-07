# TH10 memory-file Seek: remaining-space calculation through Win32 address words

Original Japanese TH10 1.00a SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The complete `CMemoryPbgFile::Seek @0x004364F0..0x0043655A` owner is
**107 bytes**, zero external relocations, and a true `RET 8`.

The original FILE_CURRENT branch computes the *remaining number of bytes*
using native 32-bit pointer/address-word subtraction. Unlike conventional
bounded C++ seeking, the original signed check can accept negative offsets
that put `m_Current` *before* the source memory array. Subsequent pointer
subtraction `m_Data + m_Size - m_Current` would be undefined under C++
when its second pointer no longer belongs to the array. The maintained
source now explicitly uses the original processor's low-word operation
and interprets that word with the original **signed** comparison:

```cpp
LONG remaining = static_cast<LONG>(
    reinterpret_cast<DWORD>(m_Data) + m_Size -
    reinterpret_cast<DWORD>(m_Current));
if (remaining <= signedOffset)
    return false;
```

`DWORD`/`LONG` are both 32-bit in this Win32 compiler environment. The
original final 32-bit address cursor computations, branch conditions and
shared successful cursor publication remain untouched. This is **not** a
new bounds guard, a claim of portable pointer-to-word conversions, or a
claim that out-of-buffer memory accesses are generally safe.

## Ten compiled, complete-function native source controls

Nine source variants of the remaining-space calculation were rebuilt with
pinned VC7.1 SP1 normal COFF `/O2` in the real `ResFile.cpp` source.
The direct low-word expression and seven other alternatives produced the
entire **107-byte**, zero-relocation owner with exactly **100/107 raw
original bytes matching**. Switching the *remaining-space comparison*
into unsigned arithmetic regressed the last seven-byte frontier to
**99/107**; reversing that signed comparison regressed to **98/107**.
The previously documented CURRENT/END native address-word changes are
retained. All nonselected variants were restored. Native artifacts and
per-function diff offsets are under
`.analysis/gpt-web/20261107-seek-remaining-word/` and
`build/gpt-web/20261107-seek-remaining-word/`.

The installed final source SHA-256 is
`0a4814ca8330abed37d10bca160178052cd647fd91965c8ef5fa4bfa242e0f25`.
An independent selected *current-source* native cold replay of the nine
other memory/resource COFF methods and one real linked constructor passes
**10/10 complete previously accepted owners, 491/491 original bytes**.
The same freshly generated `build/match/ResFile.obj` was examined for the
nonexact Seek and has exactly seven ordinary-byte differences at
`+0x49,+0x51,+0x55,+0x59,+0x5E,+0x61,+0x63` and zero relocation fields.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit memory-pbgfile-open --unit memory-pbgfile-destructor \
  --unit memory-pbgfile-close --unit memory-pbgfile-read \
  --unit memory-pbgfile-tell --unit memory-pbgfile-get-size \
  --unit memory-pbgfile-write --unit win32-resource-pbgfile-open \
  --unit win32-resource-pbgfile-destructor-00436680 \
  --unit memory-pbgfile-constructor --json
scripts/repo-python scripts/ci.py
```

The original 107-byte Seek still remains explicitly **NONEXACT** with
**zero newly credited exact bytes**. No historical all-unit cold replay,
original physical compiler TU/source spelling, runnable product or runtime
claim is made.
