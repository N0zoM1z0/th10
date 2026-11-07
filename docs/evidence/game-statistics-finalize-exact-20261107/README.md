# GameFinalizeStatistics42B1E0 — complete 564-byte authored exact

Target: Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

**New complete original-bound authored exact owner**:
`GameFinalizeStatistics42B1E0 @0x0042B1E0..0x0042B413`: **564/564 bytes**,
**24/24 individually original-bound call/data/import fields**, all decoded
local control flow and return paths. Installed source:
`src/GameStatistics.cpp` SHA-256
`dc31f5c888d68d363571a74457ed3da000c2a36386e790cd9234b0798c229666`.

## Real source/compiler and natural C++ recovery

The original source-present function initially matched **481/564** in the
five-real-TU statistics constructor graph, even with identical complete PDB
size. Original disassembly establishes `EDX = statistics + 0x14`: the persistent
seven-record checksum/copy cursor addresses each record's `recordIndex`
field, not the record base. Reconstructing its field-anchored pointer using
actual `offsetof(GameStatisticsOwner, records)` and
`offsetof(GameStatisticsRecord, recordIndex)` plus the reviewed record layout
restored **553/564** bytes, with all 24 fields fixed. Moving the loop-index
zero declaration before cursor publication recovered the seven ordinary
setup-order bytes. Representing the 0x440-byte tail checksum naturally as
0x110 groups of four bytes, beginning at payload+2 with the observed four
relative loads and decrementing group counter, recovered the last four.
The combined source reaches **564/564** without register-binding hacks,
assembly, byte patches, fabricated branches or skipped owner ranges.

Record format, compression, encryption, score-file writes, file handle
cleanup and the original failure branches remain present, including existing
unchecked allocation behavior. Both record and tail cursors derive from the
whole `GameStatisticsOwner` object's byte representation rather than invalid
nested-array span assumptions. This is exact native codegen under the pinned
VC7.1 SP1 x86 environment, *not* proof of physical original source spelling,
product runtime correctness or cross-platform semantics.

## Independent complete native evidence

Two zero-reuse cold compiles of the **installed and unchanged** final source:

```sh
scripts/repo-python .analysis/gpt-web/20261106-chat-th10-finalize-tail/verify-installed.py installed-cold1
scripts/repo-python .analysis/gpt-web/20261106-chat-th10-finalize-tail/verify-installed.py installed-cold2
```

Both compile the real `src/GameStatisticsLoad.cpp` entry with
`src/GameStatistics.cpp`, `src/FileSystem.cpp`, `src/RandomMath.cpp` as LTCG
support, and `src/Decompress.cpp` normal COFF, using VC7.1 SP1
`/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL /GS /EHsc /Gz`.
Both return **564/564** in complete original-bound comparison and **24/24**
actual target-decoded external fields. PDB identities and image digests differ:

| Independent build | PE SHA-256 | PDB GUID |
| --- | --- | --- |
| Cold 1 | `2a3fb2f514f8d2a875575137f4827d306f5e8883ad0e8318b5e6d4627910cede` | `a3773ce51e75a948be4c22e76190339c` |
| Cold 2 | `d821b450225d34fa20f02b1636779bc9c1bf58e2df3dd69f3f86e5d56610fa39` | `5c99cb97691915418e95756393ec5e86` |

The same image also verifies the previously exact statistics loader **423/423**
(five fields), tail initializer **142/142** (seven fields) and statistics
owner destructor **45/45** (two fields). Private receipts and complete
PE/MAP/PDB in
`build/gpt-web/20261106-chat-th10-finalize-exact/installed-cold{1,2}/proof.json`.
The original-bound all-24 field manifest is separately checked against the
real PE in `.analysis/gpt-web/20261106-chat-th10-finalize/original-contract.py`.

Only the three selected canonical units were replayed, **not** the full cold
cohort:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit game-statistics-finalize \
  --unit game-statistics-load \
  --unit game-statistics-initialize-tail --json
```

Result: **exact, 1,129/1,129 bytes**, including newly exact **564** plus
previously exact **565**, with **36** total originally bound fields.
Receipt `.analysis/gpt-web/20261106-chat-th10-finalize-tail/canonical-selected.json`.
The new match unit reuses the existing five-source
`build/match-linked/GameStatisticsTail` group so all grouped source/profile
contracts and output paths remain consistent.

**Canonical credit: +1 confirmed-authored exact function, +564 bytes.**
No full historical replay or complete game link/runtime test was run.
