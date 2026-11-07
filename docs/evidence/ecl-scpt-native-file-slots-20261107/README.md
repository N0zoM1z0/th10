# ECL AddScriptData: native 32-bit SCPT file-slot indexing

Target: Japanese TH10 v1.00a executable, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
`EclVmScriptDatabase::AddScriptData @0x00450220` is an original
**589-byte** (still **NONEXACT**) owner. Its companion
`FindSubroutine @0x00450470` is an already exact 136-byte original owner.

The database contains 32 four-byte pointers to SCPT file headers. Native
i386 indexes this slot array with an unchecked `fileCount` DWORD, using
low-32-bit multiplication and address addition; no original bounds check
exists in the parser. C++ expressions such as `files[fileCount]` can form
an out-of-array pointer when the count is corrupted, whereas the native
address calculation wraps modulo 2^32. The maintained source now uses a
single typed `EclFileSlotAtWord(base, index)` helper for the initial file
publication, retained volatile file-slot alias, both name/header walks,
subroutine count reads, and final indexed file lookup. No new clamp,
storage, allocation, fake ABI or binary patch is introduced. The code
remains unsafe for malformed file counts; this is i386-specific source
semantic reconstruction rather than a safe parser rewrite.

## Fresh native experiments and full caller preservation

The *current* `EclVm.cpp`/`Enemy.cpp` two-TU VC7.1 SP1 `/GL` graph was
cold-built before trying changes. Isolated first-file-word, file-slot
address-only, and full file-index controls all kept **589 bytes**, the
same two CRT call field positions, and the original ordinary comparison
**502/581** (79 differences). Each independently preserved complete
`EclVmContext::Run` PDB **7,020 bytes**, **6,997/7,020** original
matched bytes, all **189 independently original-bound fields**, and the
171-byte exact float-value callee. Thus the changes are **codegen neutral**
and are *not* evidence of a new exact function.

The installed full helper was compiled again through the canonical real
2-TU graph and rechecked against the full 589-byte original: the two
existing malloc/free call fields stayed at +0x6C / +0x101, and all 79
ordinary mismatches are still present. `FindSubroutine` remains **136/136
exact** and `Run` **6,997/7,020** with its 189 original fields.

A focused *installed-source* cold replay of only four previously exact
original-target owners passed **575/575** bytes: `ecl-vm-find-subroutine`
136, `ecl-vm-host-run` 146, `ecl-vm-read-float-value` 171 and
`ecl-vm-stack-pop` 122. No full historical replay, game product or runtime
claim. No new canonical exact codegen credit.

Private source-specific receipts: `.analysis/gpt-web/20261107-turn4/
ecl-file-words.py`, `ecl-file-results.json`,
`ecl-files-installed-proof.json`, `ecl-files-installed-run.json`,
`ecl-files-guards.json`; actual PE/MAP/PDB in
`build/gpt-web/20261107-turn4-ecl-files/` and installed
`build/match-linked/EclVmHostRunEnemy/`.
