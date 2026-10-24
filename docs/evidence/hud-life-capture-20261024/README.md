# HUD boss-life capture, 2026-10-24

## Measured result

A fresh baseline preceded one ordinary `const int` capture in
GuiView::UpdateStageElements. The captured life now supplies both bossLife
publication and the existing float numerator. Denominator, types, guards and
source assignment order are unchanged; no forced spill, volatile access,
register hint or width change was used.

The native load/capture/FILD/publication/FIDIV sequence naturally equals the
corresponding **26 raw target bytes**. This is bounded diagnostic evidence,
**not fragment exact credit**. The complete owner remains **3772 versus 3836
bytes, nonexact**. This batch adds **zero exact bytes and zero source owners**.

| Complete owner | Code / alignment / table bytes | Syntactic fields / local branches / calls |
| --- | --- | --- |
| Fresh baseline, 3772 bytes | 3741 / 3 / 28 | 71 / 249 / 21 |
| Captured-life candidate, 3772 bytes | 3741 / 3 / 28 | 71 / 248 / 21 |
| Target, 3836 bytes | 3805 / 3 / 28 | 72 / 250 / 21 |

All three include the three-byte identity LEA alignment and all seven four-byte
table words. Alignment and table words are unchanged between baseline and
candidate. **226 raw baseline/candidate byte positions differ**; this is not a
target-mismatch score. Inventories are syntactic, not accepted semantic bindings;
no normalized target percentage is claimed and no owner/table bytes are cropped.

## Fresh baseline and exact identities

The older carrier was stale across six translation units and was not reused as
the experiment baseline. Fresh native baseline and independent replay passed
before the capture was applied. Baseline review preserved **111 artifacts**;
candidate independent review preserved **229 artifacts**. Both native exits are
0; all **13 complete guards, 1468 bytes / all 79 fields**, pass in both images.
The neighboring Gui Run owner was retained in full as nonexact, not independently
tuned or credited.

- Baseline Gui.cpp: `13229656bcc568a2a5ecf392660e718ffcbcacb1c13b5ea5b7817b36977b53ff`
- Candidate Gui.cpp: `142c14724e4d600f93fb462af60df08d43b42c5ca6880cee449256135661f6e6`
- Baseline image: `7af8caeb971f0b06e6d61ec3aed79e86c26a0c470be8c43f0739665529d72926`
- Candidate image: `7ee7125c32bf203c7b356d0872d696b3cd6b96a01a97d01823c5907107ff7117`

[Machine-readable summary](summary.json) retains graph, source pins, inventories,
validation scope and receipt hashes without copying disassembly or target bytes.

## Actual graph and retained receipts

This is the actual **14-TU Gui graph**, not a WinMain/Sound diagnostic graph.
Primary Gui.cpp is followed by AnmManager.cpp, AnmVmCreate.cpp, AnmVmId.cpp,
EnemyEclDispatcher.cpp, Player.cpp, FrontEnd.cpp, DecomphelpLeafAccessors.cpp,
Main.cpp, Sound.cpp, TransitionEffect.cpp, Chain.cpp, RandomMath.cpp and
AnmRectangle.cpp, all under `src/`.

Profile is `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL`, **without /GS**, with
no per-support overrides. Entry is the real static UpdateStageElements member;
exact symbol, link flags and harness are recorded in the summary. No WinMain
profile was transplanted into this experiment.

Ignored packet `.analysis/gpt-dots/hud-life-capture-preparation-20261024/` retains:

- `current-graph.json`, both stage freezes, receipts, full HUD/Run inventories,
  alignment/tables and guard replays.
- `baseline/independent-review/receipt.json`: passed fresh-baseline decision.
- `candidate/independent-review/receipt.json`: passed independent candidate,
  SHA-256 `7361e69b9961e23e0e2cb8db4a6f7bbc64af3f02a982479c731adde1e3e2b1ef`.
- `candidate/independent-review/complete-owner-comparison.json`: full raw
  baseline/candidate accounting, separate from the local capture observation.

These ignored artifacts are not supplied by a Git clone. The focused boss-gauge
suite passed **5 tests**. All **112 target-independent CI stages passed** for
this source, exit 0, with frozen inputs preserved, verified at 22:25:31 UTC.
Only CI-result/next-step documentation changed afterward. The separate automatic
boundary-ledger divergence remains unresolved. WinMain remains parked. An
isolated excess-segment conditional ID-clear model is being prepared, not tried.
