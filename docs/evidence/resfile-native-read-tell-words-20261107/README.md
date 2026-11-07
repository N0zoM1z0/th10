# Memory-file Read and Tell: native 32-bit cursor words

Original target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
These are **already accepted** exact full owners, not newly discovered
owners: `CMemoryPbgFile::Read @0x00436460`, 108 bytes, and
`CMemoryPbgFile::Tell @0x004364D0`, 7 bytes. The related `Seek` is 107
original bytes but remains **NONEXACT**.

## Correction in natural C++

The original memory-file `Seek` permits negative FILE_CURRENT offsets that
can make `m_Current` point before the start of `m_Data`. The original i386
`Read`/`Tell` still use low 32-bit pointer-address words: the remaining
capacity is `data + size - current`; `Tell` returns `current - data`; and
both paths through `Read` advance the cursor by an unsigned 32-bit count.
The previous source expressed these as C++ array pointer subtraction and
addition, undefined whenever the cursor is outside the allocated array.

The maintained source now explicitly takes the i386 `DWORD` address words
for the two subtractions and both cursor additions, casts back to the
original `BYTE*` representation on publication, and otherwise preserves
the existing branch conditions, `memcpy`, exact size, source layout, and
calling conventions. This does not add a bounds check or claim malformed
buffer reads are safe. These target-specific 32-bit conversions are not a
portability or runtime guarantee.

## Five source-codegen trials, then final-source replay

Five complete pinned VC7.1 SP1 normal-COFF trials were compiled: just the
remaining count, just the `Tell` difference, both `Read` cursor advances,
remaining+Tell together, and all four native expressions combined. In
**each** trial the complete original `Read` **108/108** bytes and `Tell`
**7/7** bytes remained exact, with zero relocations in these two owners.
The combined form is now installed and was compiled again through a fresh
selected cold replay.

Ten selected original-target exact file units (the two changed functions,
Open, Close, destructor, constructor, GetSize, Write, Win32 resource Open,
and Win32 resource destructor) passed **491/491** original-bound bytes,
including the declared relocations of the other owners. The same source's
complete `Seek` COFF owner is still **107 bytes / seven original differences**
at `+0x49,+0x51,+0x55,+0x59,+0x5E,+0x61,+0x63` (zero relocations). It was
not silently promoted. No canonical match ledger changes and no new exact
credit are claimed.

Ignored local evidence: `.analysis/gpt-web/20261107-session-th10/
resfile-word-probe.py`, `resfile-trials.json`, `resfile-final-exact-guards.json`,
`resfile-seek-control.json`; actual current native object
`build/match/ResFile.obj`. No all-unit historical replay or game runtime
test was run.
