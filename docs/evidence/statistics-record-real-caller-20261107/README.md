# Wire the real statistics record initializer into its constructor caller

Original target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`GameStatisticsOwner::GameStatisticsOwner @0x0042AE60` really calls
`InitializeGameStatisticsRecord @0x0042ACB0` once per initialized record.
The original record initializer is **274 bytes, private stack argument and
RET 4**. Source `GameStatisticsLoad.cpp` previously called an unresolved
forward stand-in named `InitializeStatisticsRecord42ACB0` rather than the
reconstructed implementation in `src/GameStatisticsRecordDefaults.cpp`.
The maintained constructor now calls that actual source-present function
under its correct `__stdcall` declaration. No synthetic stub, fake compiler
receiver, no-op shim or new memory ownership is introduced.

## Real calling graph and native checks

The three affected registered original-exact units share their real
`CreateStatistics42AF20` entry and one output, with a pinned **six-source**
VC7.1 SP1 `/GL /GS /EHsc /Gz` graph. Their `support_sources` now include
`GameStatisticsRecordDefaults.cpp` with its own previously verified VC7.1
`/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL /GS` profile; `Decompress.cpp`
stays normal COFF. `src/GameStatisticsLoad.cpp` SHA-256:
`5e553e651f61a9eb4f85573ca292264c8a31805686e7bb68a514466d9b6dbb6d`.

A temporary six-TU proof was compiled first, with the old source restored
afterward. The **final installed** source then independently canonical-cold
replayed only the affected units:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit game-statistics-finalize \
  --unit game-statistics-load \
  --unit game-statistics-initialize-tail --json
```

All three pass **1,129/1,129 original bytes**: finalizer **564/564** with
24 independently original-bound fields, loader **423/423** with 5 fields,
and statistics tail initializer **142/142** with 7 fields. A same-image
original-target comparison also preserves `GameStatisticsOwner` destructor
**45/45** with 2 `free` targets. The result image has an actual PDB-owned
`InitializeGameStatisticsRecord` function and the constructor's `REL32`
call target **resolves to exactly that function's linked address**, not to
an unresolved anchor or dummy. The original default name/rank-kind view
is preserved. The record initializer still emits the full **274 bytes**;
**7/8 data-field positions** align, while the second name-word address
remains at candidate `+0x50` instead of target `+0x53`.

The original constructor is **132 bytes** versus current **137**; its
factory is **97 target versus 98 current**. Both remain **NONEXACT** despite
the now genuinely resolved callee, as does the record initializer. No new
canonical exact credit is taken for this wiring. This directly falsifies
an older hypothesis that the unresolved callee was necessary for the current
constructor's codegen, but proves the maintained source's real dependency
and selected accepted guard preservation.

Private proof:
`.analysis/gpt-web/20261107-stat-real-caller/native-result.json`,
`installed-result.json`, `installed-selected.json`; full installed PE/MAP/PDB
under `build/match-linked/GameStatisticsTail/`.
No full historical replay, original physical source/TU assertion,
whole-game executable or runtime/portability claim.
