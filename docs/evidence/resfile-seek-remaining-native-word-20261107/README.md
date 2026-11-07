# Memory-file Seek: original remaining-space address-word subtraction

Target: original Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The target `CMemoryPbgFile::Seek @0x004364F0` has a complete **107-byte**
body, a `RET 8` calling convention and zero external relocations. It accepts
signed negative FILE_CURRENT seeks that can move its pointer before the
array start. The prior C++ computed the remaining length using
`m_Data + m_Size - m_Current`, which is undefined in C++ for pointers
outside the same array even when the original i386 executes a low-word
`ADD/SUB` sequence without checks. The maintained source instead computes:

```cpp
LONG remaining = static_cast<LONG>(
    reinterpret_cast<DWORD>(m_Data) + m_Size -
    reinterpret_cast<DWORD>(m_Current));
if (remaining <= signedOffset)
    return false;
```

These are actual Win32 32-bit unsigned address words, converted back to the
original signed 32-bit comparison before deciding the FILE_CURRENT result.
The final CURRENT/END address-word addition, all three seek-origin branches,
negative offset acceptance and shared `m_Current` publication are unchanged.
This does not introduce bounds enforcement or imply portable C++ pointer
arithmetic, original source spellings, or a production game build.

## Source variants and native target comparison

Nine newly authored natural C/C++ source forms were separately compiled
with the pinned VC7.1 SP1 normal-COFF profile: direct word subtraction,
end-pointer local, current-pointer local, pointer offset, reverse comparison
and other dword expression orderings. Eight retain the same complete
**107-byte / zero-relocation / 100 raw matching bytes** result, whereas
an unsigned comparison and reversed comparison regress the target branch
code. The selected form preserves the original signed remaining check;
there are still seven independent register/branch bytes different at
`+0x49,+0x51,+0x55,+0x59,+0x5E,+0x61,+0x63`.
**Seek remains NONEXACT**, without any new canonical match credit.

After installing the final source, the **ten selected** original-bound
file-interface units were recompiled in their actual two artifacts and
all ten still pass: **491/491 original codegen bytes**. The selected
owners include nine normal COFF methods and the memory-file constructor
through its declared linked entry. No historical all-unit replay was run.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit memory-pbgfile-open --unit memory-pbgfile-destructor \
  --unit memory-pbgfile-close --unit memory-pbgfile-read \
  --unit memory-pbgfile-tell --unit memory-pbgfile-get-size \
  --unit memory-pbgfile-write --unit win32-resource-pbgfile-open \
  --unit win32-resource-pbgfile-destructor-00436680 \
  --unit memory-pbgfile-constructor --json
```

Maintained `src/ResFile.cpp` SHA-256:
`0a4814ca8330abed37d10bca160178052cd647fd91965c8ef5fa4bfa242e0f25`.
Actual source variants, source hashes, compiled COFF and the current-source
selected result are retained in ignored
`.analysis/gpt-web/20261107-seek-remaining-word/`,
`build/gpt-web/20261107-seek-remaining-word/` and current
`build/match/ResFile.obj`. Do not treat old original-page disassembly,
masked bytes or earlier candidate reports as proof of this new epoch.
