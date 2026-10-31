# Replay frame allocation — complete 139-byte exact owner (2026-10-31)

`ReplayAllocateFrameData @ 0x0042AA50..0x0042AADA` is **canonical linked-PE exact: 139/139 physical target bytes and all one independently bound REL32 call field**, in the actual `ReplayManager::BeginStage` source graph. The independently verified Japanese TH10 v1.00a original is SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`. This adds **one already reviewed authored owner and 139 exact authored bytes**, without changing the target extent, function-origin classification or original file.

## Corrected VC7.1 constructor instruction schedule

The real `ReplayFrameData` C++ constructor in `src/ReplayStageHelpers.cpp` clears the entire **0x6284-byte** frame allocation, publishes `recordEnd` at `+0x5460` pointing to its `records` base, `fpsEnd` at `+0x6274` pointing to `fpsSamples` at `+0x5464`, and initializes the embedded list link at `+0x6278`. The full 139-byte target retains the nullable `operator new` branch, actual list traversal, conditional successor fix-up, and **private ESI stage / stack manager / RET4** source compiler interface. Its single external field at `+0x06` is a `REL32` call to `operator new @0x00452493`.

Prior maintained source spelled the cursor initialization `recordEnd = records;` directly. A fresh original-source A/B test in the **same genuine two-input compiler graph** (`ReplayManager.cpp` entry `?BeginStage@ReplayManager@@QAEXXZ`, with `ReplayStageHelpers.cpp` as its actual `/GL /GS /EHs` support TU) produced the correct 139-byte section but **133/139 ordinary bytes**: the compiler scheduled the `lea eax,[edx+0x5464]` for `fpsEnd` before the `mov [edx+0x5460],edx` cursor publication. The original target's physical order is the opposite. Flipping `fpsEnd`/`recordEnd` source order worsened the full comparison (130/139), and moving `link.value` between them worsened it further (129/139). A `volatile` publication remained 133/139, while an inline read/write barrier moved six different bytes but still failed (132/139). These are rejected controls, not prior documentation assumed correct.

The retained natural C++ source uses the already-existing typed cursor slot explicitly, with no new data storage, no private register annotation and no volatile trick:

```cpp
ReplayRecData **cursor = &recordEnd;
*cursor = records;
fpsEnd = fpsSamples;
```

The pointer is local, the write is the same publication to the same actual field, and VC7.1 naturally schedules that store before the independent `fpsSamples` address computation. Every target byte, final RET4, control edge and one real operator-new call now matches. The constructor's physical source/TU provenance is not inferred from the codegen result.

## Complete independent scoped acceptance and affected helpers

The original 29-input Replay source graph is **not** cold-replayed in full. Instead, the canonical recipe uses the *real* `ReplayManager::BeginStage` caller with the actual allocation implementation as the sole support source. Two **independent selected canonical cold replays** return **139/139 exact, one/one field**, with different PDB GUIDs `9fd848d2b03db54fb1dd5844e76ba543` and `e858dd7307537b4ebffcbb380dd315ce`. The unique 139-byte PDB section contribution is `support-00-ReplayStageHelpers.ltcg.obj`. The selected cold receipts are ignored under `.analysis/gpt-web/current-turn/replay-allocate-cold-{1,2}.json`.

Two previously exact functions in the *same source TU* were separately compared against their **complete original target extents** in the newly built two-input image: `ReplayClearStageFrameData` remains **93/93, one field** and `ReplayRestoreStagePosition` remains **83/83, two fields**, using the real shared source/PDB contributor. The reproducible `verify-native.py` enforces the new owner plus those two strict sibling guards. These existing **176 exact bytes are not counted again**. The nonexact 582-byte `ReplayManager::BeginStage` caller is not admitted by this proof.

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/replay-exact-units.py --unit replay-allocate-frame-data --json
scripts/repo-python docs/evidence/replay-frame-allocation-exact-20261031/verify-native.py --artifact-dir build/match-linked/ReplayFrameAllocate
scripts/repo-python scripts/ci.py
git diff --check
```

This proves the complete allocation function's machine-codegen and typed call-site target identity, not original C++ spelling, whole-game product/runnable EXE, runtime allocation-failure equivalence, unrelated Replay callers, portability or original translation-unit boundaries.
