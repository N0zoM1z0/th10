# Rejoin TH10 popup insertion with its real shared 0x477840 owner

**Scope:** original Japanese TH10 v1.00a, pinned SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
This checkpoint corrects a previously independent ItemManager-local popup type
and global assumption. It implements actual shared ownership and original
record insertion behavior; it does **not** claim that the entire 167-byte
insertion function is exact.

## Evidence: one shared owner, not two

The full target `ItemManagerUpdateBody @0x0041AFD0` has eight real calls to
`0x0042B9C0` which consume the popup pointer loaded from **0x00477840**.
For example, at original `0x0041B425`, the instruction `MOV ESI,[0x477840]`
precedes the `0x42B9C0` call at `0x0041B433`. The original popup constructor
at `0x0042B4AC` publishes **the same address**. Its destructor at
`0x0042B570` clears this global. The constructor is already 134/134 exact
in a selected cold; its 0xB884-byte object has its ANM VM at +0x18 and popup
rows beginning at +0x3C4. The update function is also 134/134 exact and
iterates 723 records of stride 0x40.

Previously `src/ItemManager.cpp` declared a separate `ItemPopupOwnerView` and
`g_ItemPopupOwner` (a different unbound symbol) and defined the insertion
method there. That was at odds with the original caller's owner address.
The corrected source now declares `ScriptViewerPopupOwnerView::EmitValuePopup`
in `src/ScriptViewerPopupCreate.cpp` and calls it from all **eight** real
`ItemManager` sites through the existing `g_ScriptViewerPopups` global.
The owner field at +0x14 is typed as the insertion cursor. Both Update and
Draw continue to use the same records and the original **723**-record extent;
insertion itself wraps at **720** exactly as the target does.

The moved method keeps the original sign/zero/decimal-digit logic, color,
conditional timer initialization and initial value publication, position
copy, and cursor increment. The timer storage layout is identical to
`AnmVmTimerView`, and the original scale pointer resolves to the maintained
`g_AnmGameSpeed` at **0x00476F78** rather than inventing a distinct timer
scale address. No source lifecycle or null guard is added merely to improve
compilation.

## Real native status and reproducible scoped guards

A **five-source** VC7.1 SP1 `/GL /GS /EHs` scoped native link includes
`ItemManager.cpp`, `ScriptViewerPopupCreate.cpp`, `Chain.cpp`,
`ScriptViewerLifecycle.cpp`, and `AnmManager.cpp` and finds the actual PDB
contribution for `ScriptViewerPopupOwnerView::EmitValuePopup`. Its **171**
bytes do **not** equal the complete original **167**. The candidate has one
linked `g_AnmGameSpeed` DIR32 field in the initializer; its code-flow order
and four-byte extent gap are not closed. No canonical exact match unit or
partial exact credit is recorded. In the same combined image the Popup
constructor134, update134 and update callback7 remain full-byte equal, while
Draw remains 537/540 nonexact and the larger ItemManager update remains
nonexact.

Separate **selected cold replays** of the source changes retain **470/470**
previously accepted bytes and field checks: `item-faith-add` 46,
`item-vector-from-angle-magnitude` 30, `item-rank-add` 42,
`item-manager-draw-callback` 29, `item-manager-update-callback` 48,
`script-viewer-popup-constructor` 134, `script-viewer-popup-update` 134,
and `script-viewer-popup-update-callback` 7. No full all-unit replay was run.
The [native-summary.json](native-summary.json) contains selected cold image
and PDB identifiers, complete function extent and original PE hashes.

The original game-author source, physical translation unit, and whole-game
runtime remain unknown. This commit is a grounded **source model / dependency
identity repair**, not a claimed exact codegen milestone for the 167-byte
owner. Earlier attempts at changing the sign branch did not reduce the
mismatch and were not retained.

To recheck the affected scope:

```sh
scripts/repo-python scripts/verify-target.py
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit item-faith-add --unit item-vector-from-angle-magnitude \
  --unit item-rank-add --unit script-viewer-popup-constructor \
  --unit script-viewer-popup-update --unit script-viewer-popup-update-callback --json
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit item-manager-update-callback --unit item-manager-draw-callback --json
scripts/repo-python scripts/ci.py
git diff --check
```
