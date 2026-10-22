# Complete archive header parser

The `pbgarchive-parse-header` contract covers all350 bytes at
`0x00434F70..0x004350CD` and ten linked fields in original Japanese TH10 v1.00a,
SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

Maintained source now passes target-observed FILE_BEGIN=0 to the absolute seek,
guards both temporary frees on failure and clears the backend pointer after
the conditional delete. The previous unresolved seek-mode-array load was not
target evidence for this call. Its original source expression and any data-owner
or constant-folding explanation remain unknown. Header, interface, layout and
global declarations are unchanged.

The target review reuses the preceding fresh local four-function Ghidra/caller
inspection: readOnly/noanalysis/rollback, maxcpu1/heap768 MiB, all412289 mapped
text bytes unchanged. Complete target decoding independently enumerates one
DIR32 open-mode address and nine REL32 fields: two decrypt calls, allocation,
decompression, entry allocation and four frees. Every local branch and byte
remains part of the whole350-byte comparison.

The actual investigation separates source and context controls:

| Control | Actual compilation | Parser result |
| --- | --- | --- |
| Literal seek origin | Archive support only, three source objects reused | 351 bytes |
| State declarations after null check | Archive support only, three reused | 351 bytes; no improvement |
| Guarded cleanup and unconditional null store | Archive support only, three reused | 351 bytes; instruction routing exposes extra receiver push |
| Same corrected source, archive `/Ob2` | Archive support only, three reused | Whole350/all10 exact |

The first three retain ten existing whole guards1237/all62 in their `/Ob1`
context. No static-helper/header experiment is executed. The `/Ob2` control
also emits the existing327-byte allocator, independently compared over all327
bytes/all10 fields with the supported allocation-EH address binder. No allocator,
EH-code or metadata ownership receives new credit.

This new context intentionally differs from existing canonical recipes.
Its GetSize263, Load231 and registration35 do not match their target extents;
they are not included in a claimed new-context guard pass. Their canonical
recipes stay unchanged. The same corrected source retains the existing `/Ob1`
guard cohort, and the affected original-profile Load receives a fresh cold.
Original production profiles and translation-unit partition remain unknown.

Two independent canonical colds compile all four actual maintained inputs:
GL/Gz FileSystem, GL/Gz/Ob2 archive, GL/Gz PbgFile and normal-COFF/Gz ResFile.
Each checks new350/all10 plus eight complete guards1230/all48, totaling1580/all58:
lookup212, memory Open46, entry size51, FindEntry53, destructor46, path Read214,
entry reader281 and existing allocator327. Only one350-byte gain is credited.

Reproduce the new unit twice and the affected old-context unit once:

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/replay-exact-units.py \
  --unit pbgarchive-parse-header --json
scripts/repo-python scripts/replay-exact-units.py \
  --unit pbgarchive-parse-header --json
scripts/repo-python scripts/replay-exact-units.py --unit pbgarchive-load --json
```

[facts.json](facts.json) records contracts, source/profile scope and actual
carriers; [validation-summary.json](validation-summary.json) records checks and
cleanup. Private evidence is under
`.analysis/gpt-6.1-sol/archive-header-context-20261022/`. Keep both actual new-unit
colds, the affected old-context carrier and compact source/producers/results.
Superseded experimental carriers can be removed after their pins are checked.

Original identifiers/class/ownership/flags/TU, complete Windows i386 product and
runtime remain unknown. Exact units under different declared contexts do not
close the whole production graph. Semantic reconstruction and portability remain
unstarted.
