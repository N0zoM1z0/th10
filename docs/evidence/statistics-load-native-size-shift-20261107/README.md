# Statistics score-file loader: native 32-bit allocation-size shift

Original target: Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The real 423-byte `GameStatisticsOwner::Load42B030 @0x0042B030` reads
`GameStatisticsFileHeader::uncompressedSize` as a signed 32-bit field at
header +0x14. Original disassembly at `0x0042B069..0x0042B07A`
independently establishes the exact allocation-size sequence:

```asm
mov eax, [ebx]
lea esi, [eax+18h]
mov eax, [eax+14h]
shl eax, 2
push eax
call malloc
```

The inherited source `header->uncompressedSize << 2` performs a signed
left shift. C++ gives undefined behavior for negative sizes and positive
values whose shifted result is not representable; native x86 instead
unconditionally keeps the low 32 bits. The maintained source now shifts
`static_cast<unsigned int>(header->uncompressedSize)` left by two, preserving
the encoded original word. This does not modify the *separately passed*
original signed decompression length, add an input validation or silently
improve the native loader's behavior on malformed files.

The previously present uncommitted source edit was inspected in place,
its indentation/comment corrected, then verified; it was **not discarded**.
A fresh affected-unit original-bound canonical cold replay:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit game-statistics-load --json
```

passes the complete **423/423 source-owned target bytes** and **all five
independently bound real calls** (Decrypt, malloc, DecompressData, free,
second malloc). This is preservation of **existing** exact credit, not a
newly matched owner. The seven-stage lightweight CI and whitespace checks
pass; no full-repo replay or product/runtime claim is made. The selected
receipt is retained privately in
`.analysis/gpt-web/20261107-enemy-flag-shift/statistics-load-selected.json`.
