# TH10 Popup owner: unify typed 723-row storage without altering construction

Target: Japanese TH10 v1.00a, original SHA-256
2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

The actual ScriptViewerPopupOwnerView has a target-proven 0xB884-byte
allocation, an ANM VM beginning at +0x18 and a region of 723 records of
0x40 bytes each beginning at +0x3C4. Existing constructor and update
functions were already exact, while the 540-byte Draw and 10-byte Draw
callback remained nonexact.

The former ScriptViewerPopupCreate.cpp repeated raw
reinterpret_cast<ScriptViewerPopupRecord *> from (this+0x3C4) three times
in Update, Draw and EmitValuePopup. Each cast assumed the same record
structure, but its declaration was private to that source file. The
canonical shared header ScriptViewerOwners.hpp now contains the actual
ScriptViewerPopupRecord layout, with assertions for sizeof 0x40 and its
timer offset +0x1C. The owner has a single typed PopupRecords() accessor
to the same byte-addressed region; all three real methods use it. The
owner also asserts the record storage begins at +0x3C4 and the
723 * 0x40 span ends at the actual +0xB884 allocation extent.

Crucially the record region remains opaque byte storage, NOT a constructed
C++ array member. The target's actual outer constructor initializes the
ANM VM and then clears the whole owner; adding an automatically constructed
array member could silently introduce a large constructor/destructor
sequence. This change avoids that semantic and exactness regression.
No new shadow array or new runtime allocator is introduced.

## Pinned target-bound results

The real four-input pinned VC7.1 SP1 /GL /GS /EHs Popup creation graph was
recompiled with the modified shared source, not assumed from an older
handoff. Complete existing owners all pass exact selected cold replays:

- Popup constructor 134/134
- Popup Update 134/134
- Popup Update callback 7/7
- Total 275/275 bytes, all original bound address fields.

The complete Draw remains 537 PDB-owned candidate bytes against the
original 540-byte target, with 15 linked fields. Its callback is still
10 bytes but differs at three ordinary positions; target uses saved EDI,
candidate saved EBX. Neither is credited exact. Evidence in native-result.json
records source hashes, complete PDB extents, linked-field counts,
candidate/target comparisons and selected guards.

This checkpoint is real source-model and owner-interface recovery; no
canonical exact count changes, no target-byte editing, no fabricated
register ABI, no original historical TU or runtime claims. No full
all-unit cold replay was run.
