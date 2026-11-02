# Popup-number Draw: complete 540-byte real-source implementation, NONEXACT

The Japanese TH10 v1.00a original has a complete **540-byte**, reviewed
authored Draw owner at `0x0042B780..0x0042B99B`, followed by `CC` alignment,
the **7-byte exact update callback** at `0x0042B9A0`, more padding, and the
independent **10-byte draw callback** at `0x0042B9B0..0x0042B9B9`.
The latter entry was previously omitted from the tracking ledgers. The full
target is decoded end to end; there is no prefix-based source match.

The maintained `ScriptViewerPopupOwnerView::Draw` is now implemented in
`src/ScriptViewerPopupCreate.cpp` using the same **0xB884-byte object**,
embedded ANM VM at `+0x18`, and **723 0x40-byte records** from `+0x3C4`
that support the already-exact constructor and per-frame update. It performs
the original optional fog-state change and draw-buffer flush, consumes
active records, computes digit spacing, player distance and alpha, chooses
the original three sprite banks by age, draws every reverse decimal digit,
then refreshes the player pointer at the target's loop boundary. The source
preserves the unguarded record float division, signed conversion and other
original behaviors rather than inventing safety repairs.

## Actual native result and remaining differences

The true selected VC7.1 SP1 `/GL /GS /EHs` graph includes
`ScriptViewerPopupCreate.cpp`, `Chain.cpp`, `ScriptViewerLifecycle.cpp`,
and `AnmManager.cpp`. The fresh retained PDB contribution is **537 bytes**
versus the original **540**, with **15 native address-field slots**.
Earlier real-source variants with the same target semantics also emitted
535 bytes. Neither candidate is exact; local spacing/alpha/digit-pointer
stack allocation, sprite-register scheduling, and internal branch positions
still differ. Because the extents differ, the field positions are an
**inventory**, not a claim of all fifteen verified original address identities.

The new `ScriptViewerPopupDraw42B9B0` source adapter has a **10-byte** PDB
extent and calls the actual 540-byte owner (one original target-bound REL32
callee). Its candidate preserves the original call and return behavior but
uses saved `EBX` where original TH10 saves `EDI`. After binding the callee,
**three ordinary bytes differ**, at offsets `+0x00`, `+0x02`, `+0x08`.
Its original authored-vs-compiler-generated origin remains indeterminate.
These are **NONEXACT owners with zero new exact-byte credit**.

Two falsifiable source variants were tried in the same narrow producer:
marking the digit pointer `volatile` produced a 521-byte owner, and reducing
alpha's type to an unsigned byte produced 534. Both regress the complete
native extent and have been **rejected and restored**; they are not part of
this checkpoint. Historical October 25 model-only proposals do not override
these fresh actual maintained-source measurements.

## Exact-regression guard

Rebuilding the selected real-source graph and cold-replaying only its three
accepted units preserves **275/275 bytes exact**: popup constructor 134,
Update 134, and Update callback 7, each with its retained strict address
fields. The rejected Draw and Draw callback receive no exact match-unit,
no `config/matches.csv` entry and no added exact percentage. The existing
old Draw proposal remains a historical experiment; the maintained
implementation and the new explicit missing callback boundary are real
source/ledger progress, not a product or runtime proof.

[Native snapshot](native-summary.json) includes original target function
SHA-256s, candidate sizes and caller-byte differences. Original source
spelling, physical translation unit, 540-byte exact codegen and full-game
runtime remain open.

Reproduce without full cold replay:

```sh
scripts/repo-python scripts/verify-target.py
nice -n 10 scripts/repo-python scripts/build-match-unit.py --unit script-viewer-popup-update
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit script-viewer-popup-constructor --unit script-viewer-popup-update \
  --unit script-viewer-popup-update-callback --json
scripts/repo-python scripts/ci.py
git diff --check
```
