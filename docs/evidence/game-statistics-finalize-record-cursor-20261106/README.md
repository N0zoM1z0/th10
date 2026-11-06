# GameFinalizeStatistics: target-index-cursor recovers 553/564 bytes

Target original Japanese TH10 1.00a SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`GameFinalizeStatistics42B1E0 @0x0042B1E0` is the complete **564-byte**
owner that collects seven optional 0x437C record blocks, computes per-record
byte checksums, copies the statistics tail, compresses, encrypts and writes
`scoreth10.dat` before releasing buffers. The source retains all existing
error returns and original payload/file limits; no invented failure check or
shortcut changes its complete disk-writing path.

## New source-model discovery

Direct disassembly of the original function shows the persistent record loop
cursor in `EDX` initially **`LEA EDX,[EBX+0x14]`** (function owner+0x14),
then `ADD EDX,0x437C` for each record. Since records start at owner+8 and
the original record's `recordIndex` is at +0x0C, this is **the index-field
address**, not the `GameStatisticsRecord` base. The target tests the record
magic at `EDX-0x0C`, writes the record index to `[EDX]`, sums payload
starting at `EDX-4`, stores checksum at `EDX-8` and copies the entire
record from `EDX-0x0C` using its existing `REP MOVSD` loop.

The previous maintained C++ indexed a `GameStatisticsRecord &record` for
each iteration. The actual VC7.1 SP1 optimizer used a base-record pointer
(`LEA EDX,[EBX+8]`) and a separate `LEA EAX,[EDX+0x0C]`, inducing many
native-register, checksum/loop and copy scheduling differences.

The maintained source now carries a natural byte cursor based on the
**actual `recordIndex` field**, stepping by `sizeof(GameStatisticsRecord)`;
`offsetof(GameStatisticsRecord, recordIndex/unknown08/checksum)` defines
all negative offsets, avoiding duplicate unreviewed magic constants. All
checksums still read the exact original byte ranges, and record
publication/`memcpy`/`appendOffset` occur at the original branches. This
preserves the layout types rather than adding a fake register annotation or
hand-written assembly.

## Strict complete original-bound result

A freshly rebuilt **five-real-source** VC7.1 SP1 `/GL /GS /EHsc /Gz` graph
uses the genuine `CreateStatistics42AF20` entry, with GameStatisticsLoad,
GameStatistics, FileSystem and RandomMath in LTCG plus Decompress as normal
COFF support. Against the original 564-byte function:

| Complete owner | Before | After |
| --- | ---: | ---: |
| Target and native sizes | 564 / 564 | 564 / 564 |
| Independently bound call/global/link fields | 24/24 | 24/24 |
| Original byte matches (fields normalized by identity) | 481/564 | **553/564** |
| Ordinary differing byte positions | 83 | **11** |

The private script `original-contract.py` checks each of the **24 original
encoded target addresses individually** before comparing the whole owner:
`malloc`, `CompressData`, `Encrypt`, the score filename, `OpenWriteFile`,
actual file handle/critical section/open count globals, system `CloseHandle`,
`WriteFile`, `LeaveCriticalSection`, and both final `free` calls. All field
positions, type, identity and complete decoded-body branch targets remain
constrained. An unnormalized address mask alone is not accepted as proof.

The **eleven** ordinary differences are precisely at `+0x52..+0x58`
(two independent setup instructions exchanged without changing their
meaning) and `+0xD7`, `+0xE3`, `+0xE7`, `+0xED` (tail-byte checksum cursor
and load offsets). These are open; the function remains **NONEXACT**, and
no exact-match row or partial credit is added.

Existing affected original-bound units were selected for a fresh cold replay:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit game-statistics-load \
  --unit game-statistics-initialize-tail --json
```

The loader **423/423** and tail initializer **142/142** pass, **565/565**
previously exact bytes total. In the same freshly rebuilt five-TU image,
`GameStatisticsOwner::~GameStatisticsOwner` remains **45/45 original-bound
exact** with both `free` relocation targets. There is **zero new canonical
exact credit** at this intermediate checkpoint. No all-unit replay was run.

Reproduction files (ignored in Git):
`.analysis/gpt-web/20261106-chat-th10-finalize/trial.py`,
`original-contract.py`, `results-index-layout-offsets.json`, `selected.json`
and `build/gpt-web/20261106-chat-th10-finalize/installed-cold/` PE/MAP/PDB.
Installed `src/GameStatistics.cpp` SHA-256:
`7549d302f2a3b267f55eec9d993229058af41fa8d221217b579507dff3f25f7f`.
This does not prove original physical TU/source spelling, a runnable game or
runtime equivalence.
