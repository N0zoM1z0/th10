# CMemoryPbgFile::Seek: restore x86 cursor-word address arithmetic

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Complete original `CMemoryPbgFile::Seek @0x004364F0..0x0043655A` is
**107 bytes** with a genuine `RET 8` and no external code relocations.
The maintained candidate is still **107 bytes / 100 matching raw original
bytes**, a **NONEXACT** leaf. Its seven remaining register/branch byte
differences are at `+49,+51,+55,+59,+5E,+61,+63`. No new exact credit.

## Changed natural C++ semantics

The original `FILE_CURRENT` branch accepts certain negative signed offsets
without checking the resulting cursor against the beginning of the memory
region. Consequently the old C++ `m_Current + signedOffset` could form a
pointer outside the allocated array, which is undefined in the C++ abstract
machine even though the original i386 simply adds two 32-bit register words.
The `FILE_END` path similarly computes its cursor using the signed offset.

The actual maintained C++ now performs both final address calculations as
unsigned **32-bit Win32 `DWORD`** arithmetic, turning the low result back
into the original `BYTE*` cursor:

```cpp
// FILE_CURRENT, after the original signed remaining-space comparison:
position = reinterpret_cast<BYTE *>(reinterpret_cast<DWORD>(m_Current) + offset);
// FILE_END, after the original sign and magnitude checks:
position = reinterpret_cast<BYTE *>(reinterpret_cast<DWORD>(m_Data) + m_Size + offset);
```

`offset` is the same four input bytes as `signedOffset` (the original
checked signed view). Its unsigned representation is the exact 32-bit word
used by x86 ADD, including negative offsets. The original FILE_BEGIN,
FILE_CURRENT, FILE_END and default branches, comparisons, shared cursor
store and return behavior are otherwise unmodified. This is limited to
TH10's pinned 32-bit Windows compiler; it is **not** a generic portable
pointer/integer conversion guarantee and does not invent safety bounds.

## Actual whole-function trials and selected cold replay

Twelve additional FILE_BEGIN address-writing variants and six CURRENT/END
variants were compiled using the **actual** VC7.1 SP1 normal-COFF profile,
with full symbol extents and no relocation masking. Equivalent C++ array,
integer-word and signed-index expressions are generally codegen-neutral;
the selected combined CURRENT/END unsigned-word version retains exactly
the 107-byte/100-byte frontier, and no tested form reaches exact.
Other variants either failed compilation or did not improve the whole
original-bound comparison. Every temporary edit was restored byte-for-byte.
Private results and native COFF objects are under
`.analysis/gpt-web/20261107-seek-{address,cursor-word}/` and
`build/gpt-web/20261107-seek-{address,cursor-word}/`.

The installed final `src/ResFile.cpp` source SHA-256 is
`8aaa77044b6f9cbabf16d071dcb4d7931ecddac885eeaed93ca9dc5934fe0712`.
Selected original-bound cold replay **only** of the affected nine
ordinary-COFF and one linked-source accepted unit passed **10/10 owners,
491/491 original bytes**, over two build artifacts:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit memory-pbgfile-open --unit memory-pbgfile-destructor \
  --unit memory-pbgfile-close --unit memory-pbgfile-read \
  --unit memory-pbgfile-tell --unit memory-pbgfile-get-size \
  --unit memory-pbgfile-write --unit win32-resource-pbgfile-open \
  --unit win32-resource-pbgfile-destructor-00436680 \
  --unit memory-pbgfile-constructor --json
```

Ignored full selected receipt:
`.analysis/gpt-web/20261107-seek-cursor-word/selected.json`.
No full historical replay, new exact owner, production game executable,
original physical compiler source provenance or runtime claim is made.
