# Source-current Ending refresh, 2026-10-25

## Complete measurement and preserved contracts

One fresh **18-TU checksum-entry producer** at checkpoint d0e7040 refreshes
Ending without changing Ending.cpp. Root, cold build, comparison and sampling
all exit 0; independent replay and historical/current identity review pass.
Complete Run remains **1576 versus target 1568 bytes, nonexact**. Alignment,
all **18 owned table words** and **53 local branches** are retained. Current
inventory has92 fields; the historical observer reports target90. The
[2026-10-28 correction](../ending-id-storage-20261028/README.md) finds two omitted
BSS sound-owner immediates, so the correct target count is92. These are not accepted parent bindings,
and the eight-byte extent gap is not a mismatch-byte count.

Ten strict guards retain **943 exact bytes / all 57 fields**, and all six
sampling checks pass: four creation-order and two initializer checks. An
additional same-image PendingThree replay is strictly exact **65 bytes / all
2 fields**, but is **existing credit**, not a new match. This refresh adds
**zero exact bytes, source owners or denominator bytes**.

## What changed in the image

The complete historical/current Run comparison proves all **92 field identities
and payloads**. Among **1208 raw non-field bytes**, only offsets **0xBC and
0x169** change: ECX becomes EAX, matching target and reflecting the previously
corrected FrontEnd PendingThree dependency. Alignment, all table-local offsets
and all 53 local branches are unchanged. This is a complete before/after
comparison, not a target exactness score or a runtime/global-storage proof.
No new semantic discrepancy was identified; park further Ending source panels.

## Correct graph and source identities

The latest producer is the final 18-TU graph referenced by
`docs/evidence/ending-initialize/native-replay.json`, **not** the older 17-TU
EndingCreate graph. Entry is CalcExecutableChecksum; primary source is
Ending.cpp, retaining `/GL /GS /EHs`. Its native project closure has **57 files**:
22 of the 52 previous inputs changed, five headers were added, none removed.
Ending.cpp itself is unchanged.

- Ending.cpp: `dac885626c1100f6df8b3fe5df5ab71584556ff0728804b44aec3a0fd2982b78`
- Fresh image: `f825e3a0cc4c030bc8b61b57ff00d370fdf9c213f84647bff681d311c3d25cb8`

[Summary JSON](summary.json) records the actual TU order/profile, inventory,
source identities, overlap-free scope of the additional PendingThree check and
hashed receipts. The frozen source and original artifacts are preserved; no
source, configuration or metric change was made for this refresh.

Ignored packet `.analysis/gpt-dots/ending-source-current-20261025/` retains
`receipt.json`, `comparison-receipt.json`, `complete-run.json`, `sampling.json`,
and `independent-review/receipt.json`, `history-identity-comparison.json` and
`pending-three.json`. These artifacts are not supplied by a Git clone. No
disassembly or target payload is copied here.

## Validation scope and next pilot

The prior **470dca2 112-stage CI pass**, verified at 2026-10-25 00:36:31 UTC,
remains scoped to the unchanged maintained source/configuration. This follow-on
checkpoint performs focused documentation checks, **not a new full CI**.
Keep core, HUD, WinMain and Ending parked against their old unsupported panels.

The next bounded pilot being prepared is **GameManagerUpdateBody, target 1544
bytes**: target cleans a captured owner then calls scalar delete at 0x4524A1;
current source uses free(menu). An actual **single-TU /GL, without /GS baseline**
is required before any one-call source trial. No GameManager build or edit has
occurred. The proposal is not an exactness or runtime claim.
