# FileSystem::LoadArchive: native i386 archive-count publication

Japanese TH10 v1.00a target SHA-256: 2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

The complete 36-byte FileSystem::LoadArchive at 0x004357D0 attempts to load the next archive slot, then increments g_PbgArchiveCount on success. The retail x86 increment is a 32-bit DWORD operation. Its previous C++ expression computed unsigned modulo-32-bit addition but converted the high-bit result back to signed int; that conversion is implementation-defined outside INT_MAX. The new ordinary C++ accesses the original signed counter through its corresponding unsigned int lvalue and increments that same object. This preserves the original low-word wrap and is allowed signed/unsigned integer aliasing on the pinned i386 target. It adds no range validation, branch, new ABI, side-effect reordering or machine-code patch.

Four independent VC7.1 SP1 /GL /GS /EHsc four-TU FileSystem/PbgArchive/PbgFile/normal-COFF ResFile link experiments checked prior C++, unsigned lvalue, unsigned reference and a memcpy bit copy. Every form emitted 36/36 exact original bytes with all four original bound call/global fields. We selected the simplest unsigned-lvalue source; the source and original ABI remain separate hypotheses.

Local trial script: .analysis/gpt-web/20261110-filesystem-loadarchive-word-trials.py; receipts: build/gpt-web/20261110/filesystem-loadarchive-word/results.jsonl. The canonical FileSystem::LoadArchive owner was already exact, so this source improvement adds no new exact bytes. Focused FileSystem exact-unit replays protect the source and its direct archive/disk consumers; no full historical cold replay or runtime-product claim.
