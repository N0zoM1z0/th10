# Hint exporter dependency: natural writer return contract

2026-10-25; target Japanese TH10 1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Starting checkpoint `dfacb697`; tracked tree clean, nine unrelated D3D evidence
files preserved. Target/toolchain execution/tracking and Factory-native Ghidra
attestation passed before target work. All native jobs were serial, one CPU,
nice10, with bounded runtime and retained terminal logs. No push.

## Early whole-owner experiment

The independent target audit covers exporter `41A200–41AAC6`, all 2247 bytes,
including the `RET 8` at `41AAAC` and both post-return lookup tails. The following
nine `CC` bytes are excluded. Target has 71 internal branches, nine direct calls
to writer `44B740`, then 15 inline writes. All 64 source table entries agree;
shared physical tables `474494` and `4744B0` are also used by the loader and are
not silently appended to exporter ownership.

A fresh unchanged whole `Hint.cpp` + `FileSystem.cpp` graph, `/GL` without `/GS`,
with the real decorated `ExportAutomaticHints` entry, emits **2669 bytes**.
This repeats the historical size but is new source-current evidence. One
isolated ordinary-parameter int-return writer model then recovers the target's
`-1` invalid-handle / `-2` short-write / `0` complete-write contract. No ABI,
noinline, register hint, padding, or compiler-profile change is introduced.
Prediction: the return contract could alter the call/inline partition.

That prediction is **falsified in this graph**: the complete exporter remains
raw-identical at 2669 bytes, with all 227 fields unchanged and all 24 writes
inline. Whole-owner raw SHA-256 is
`ff4d065c465e4a8c2a5ff13a762b81c1a090cd20722b7c40bd4b47ebf294a146`.
The exporter remains nonexact; no prefix or masked similarity receives credit.
This result does not establish that every return contract or caller context is
neutral. Reopen the parent only with new independent context/behavior evidence,
not an unsupported flag or spelling panel.

## Independently exact writer dependency

The same natural model changes the independently emitted writer from 63 to
**76 bytes**, matching the already reviewed authored owner `44B740–44B78B`.
Ordinary C++ parameters naturally lower to EDX=data and ESI=size; EAX carries
the three results. This ABI is observed compiler output, not forced source.
The two fixed local branches are `+9 -> +15` and `+34 -> +72`.

All seven complete DIR32 fields are independently bound:

| Offset | Identity | Target |
| --- | --- | --- |
| `02`, `25` | `gReplayFileHandle` | `474C38` |
| `1B` | `WriteFile` IAT | `4660D4` |
| `2C` | `CloseHandle` IAT | `4660CC` |
| `31` | file critical section | `4922A4` |
| `37` | `LeaveCriticalSection` IAT | `4660B4` |
| `3D` | byte open count | `49231E` |

Two distinct cold canonical replays of `filesystem-write-to-open-file` pass
**76/all 7**, including complete PDB contribution ownership and all ordinary
bytes. Independent review separately checked boundary/origin, source, ABI,
fields, local destinations and both retained canonical images. Normalized
complete writer SHA-256 is
`80834731dcad67a6a423b466f18d8c6ca11c6489ce1b64830a030b444cc140ee`.
The canonical unit keeps the actual Hint exporter-entry graph and qualifies
`FileSystem.cpp` as the support PDB owner. No standalone ABI approximation is
substituted for this graph.

Source and exact admission add **one owner / 76 bytes** each. The existing
735-owner / 273658-byte authored denominator and boundary/origin rows are
unchanged. Authored source becomes 619 / 248637; authored exact becomes
414 / 75179. Overall exact becomes 1257 / 142110, with 1103 mappings.
This is repo-native canonical proof, not a new Factory acceptance-registry
receipt, production storage identity, original TU ownership, or Windows runtime
claim. The exporter and Replay Save receive no new exact credit.

## Regression and failure accounting

- All **14 original FileSystem-primary canonical units / 2205 bytes** pass
  after the API change. The first aggregate stopped with exit1 in ArchiveLookup:
  Wine page-fault text polluted a converted object pathname and caused LNK1104.
  The original error/exit and failed output tree are retained. Six earlier
  successful cold artifacts were strictly compared separately; the failed
  archive group passed one isolated unchanged-source cold retry. Seven remaining
  units passed fresh separate cold runs. The original aggregate is not relabeled.
- Original Replay-entry opener and new same-image Hint-entry opener each pass
  **137/all 12**. Replay Save remains **1794/1681 nonexact**: all 95 field
  dictionaries and 41 local branches are identical to the previously reviewed
  combined image. Its start moves by 16 bytes; exactly 26 encoded REL32 bytes
  change by the corresponding displacement, with all ordinary bytes preserved.
- Hint export passes its exact-stream/183 table-pair baseline and all original
  12 mutation controls plus three new return-code controls. Hint load passes
  22 controls; archive routing passes 112 source assertions/eight controls;
  Replay payload order passes three tests.
- Statistics save passes 229 scenarios and 44 compiled mutation controls.
  The first run correctly rejected an obsolete mutant's bare `return` as an
  int-function compile failure, not a killed mutation. Only that mutant's return
  value was corrected to `-1`; the expected IO counterexample and exact CHECK
  expression are unchanged. Both original failure and successful rerun remain.
- Full **112-stage CI passes**, exit0, verified 06:12:10 UTC. All 1497 frozen
  tracked/new proof inputs and nine protected files (1506 total) are unchanged,
  as are all147 pre-admission evidence files. The RPC timed out while the
  original batch ran; terminal logs were recovered without restarting CI.
  Tracking/target/progress/whitespace checks pass. Prior CI is not relabeled.

## Reproduction and retained evidence

Canonical writer:

```sh
scripts/repo-python scripts/replay-exact-units.py --unit filesystem-write-to-open-file --json
scripts/repo-python scripts/replay-exact-units.py --source src/FileSystem.cpp --json
scripts/repo-python scripts/test-hint-export.py
scripts/repo-python scripts/test-game-statistics-save.py
```

Run compilers serially under a one-CPU `taskset`, `nice -n 10`, and a timeout.
The ignored packet `.analysis/gpt-dots/hint-writer-dependency-20261025/`
retains source pins, baseline/model producers, full comparisons, terminal logs,
source snapshot, prior-artifact freeze and failure records. Unique experiment
artifacts and the first canonical image are retained below
`build/gpt-dots/hint-writer-dependency-20261025/`; current canonical output is
`build/match-linked/HintWriteToOpenFile`. Previous canonical trees were copied
before cold rebuilding. Compact hashes/results are in `summary.json`.

Key Factory command receipts: preflight `372ac1d79fe34f9399035e684bfac82a`;
fresh baseline `0da15eee93af4214b5dc4818e98f069f`; model
`ac3e4dcd51d54351a9ef1dbaace610c8`; canonical first/second
`c9d3475a2c9c433ea3087d2030f4454e` / `96b26d9dfb6e4d17b35212c3b5f25611`;
recovered aggregate failure `b7de1062e9a34789a6595cdc544604ce`; isolated archive
retry `12211456cc5e4ea9a1087965704dcc66`; corrected statistics controls
`6248dc1131dd4d12adf3d7113886a831`. A failed attempt to copy a prior command's
`/tmp` log does not alter its durable successful preflight receipt.

Project remains active-incomplete; exact-first 98% goal and product/runtime,
semantic and portability gates are unchanged.

Final CI recovery receipt: `6be02a5ee2d648b3bf4528a5424c6673`; frozen-input
verification: `e610e7d8348b463685fe7724d5dd6cb1`. Only documentation updates (CI results and stale void-API wording)
follow the frozen pass. Retained packet/build trees total approximately
6 MiB and remain reproducibility evidence for the unresolved parent/accepted
writer dependency, not disposable output to be silently overwritten.
