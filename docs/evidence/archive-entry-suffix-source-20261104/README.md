# TH10 20-byte archive-entry suffix helper: independent complete source

Target: attested original Japanese TH10 v1.00a,
SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The physical CC-delimited body **0x004356D0..0x004356E3** is exactly
**20 bytes**, immediately before the separate exact 212-byte
`ArchiveOrDiskPathView::FindArchive` at `0x004356F0`. Its original source
spelling, translation-unit owner, and handwritten-vs-compiler-generated
origin are unknown. A fresh attested Ghidra `xrefs_to 0x004356D0`
query returned **no references**; absent discovered callers must not be
silently replaced by a guessed private register transport.

The original code receives the path pointer in private **ESI**, pushes
`'/'` (0x2F) and that pointer as arguments to the real CRT `_strchr`
`0x00452730`, then checks the returned pointer in EAX. If non-null it
returns the byte after the first slash (`EAX+1`); otherwise it returns
the unchanged ESI pointer. This behavior matches the separate
`ArchiveEntryName` inline helper in existing `src/FileSystem.cpp`, which
is used by the exact archive lookup/read/size paths. The inline helper's
existing byte-exact parent bodies were **not** changed or claimed anew.

## Actual new source and compiler result

The independent maintained source `src/FileSystemPathSuffix.cpp` contains
a complete ordinary C++ function `ArchiveEntrySuffix(const char *path)`
with a real `strchr(path, '/')` call, the two target branches and actual
return pointers. There is no invented global, duplicated array, hand-typed
machine code, dummy return or register-forcing ABI shim. This represents
the separate retained physical owner rather than moving the existing
successful inline source out of `FileSystem.cpp`.

Pinned VC7.1 SP1 `/GL` compiled and linked it with a complete PDB function
contribution. Its ordinary C++ stack ABI yields **27 bytes**, versus
20 in the target; the candidate retains the real external `_strchr`
REL32 identity but the call-field offset and ordinary bytes diverge
because its entry must load/save ESI. A second natural `__fastcall`
model emitted **25 bytes**, and an equivalent conditional-expression
form emitted **27 bytes**. Both were actually compiled and rejected;
`src/FileSystemPathSuffix.cpp` retains the ordinary source-level
prototype. No exact field geometry, original private ABI or complete
byte match has been proven, so this function is **source-present,
NONEXACT** and is deliberately absent from `config/matches.csv`.

This source has no existing unit consumer or modified exact sibling:
`FileSystem.cpp`, its exact `FindArchive`/`Read`/`GetSize` source and
its compiler graphs remain byte-for-byte unchanged. No historical
all-unit cold replay was run, as no accepted unit was changed. The seven
repository exact-stage lightweight checks cover the new ledger mapping.

Ignored diagnostic receipts:
`.analysis/gpt-web/20261104-next/archive-suffix-native.log`,
`archive-suffix-abi-results.json`, and
`archive-suffix-native-xrefs.txt`; current linked artifact:
`build/gpt-web/20261104-next-filesystem-suffix/native/`.
This is source reconstruction evidence only, not an original-source/
full-product/runtime/portability claim.
