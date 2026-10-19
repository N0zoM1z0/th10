# Loaded-ANM slot release (2026-10-20)

`AnmRenderManagerView::ReleaseAnm @ 0x004477D0` now closes its complete
**55 authored bytes**. Boundary, source mapping, origin and denominator are
unchanged. The canonical unit is `anm-release-slot`.

## Target and source

The original Japanese TH10 v1.00a input is SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Fresh local read-only Ghidra agrees with target bytes and the complete mapped
text hash before/after the review. The dense body ends at `0x447806` with plain RET;
no table or padding is included in this 55-byte owner.

Target checks a negative index first, then the unsigned 33-slot upper bound.
It reads the selected slot for Release, **reloads the slot after that call**
for deletion, then clears the slot. Manager/index are in EBX/ESI; Release gets
the selected loaded owner in EDI. The old source cached that pointer across
both calls. Its 48-byte native body consequently lacked the seven-byte slot
reload. Reading `loadedAnms[index]` for delete restores 55 bytes, leaving only
JGE versus target JNC at `+0x07`. Using the array's unsigned `sizeof` element count
restores that final byte. No signature, layout, profile or added compiler
attribute changes; no fake return, target patch or inert operation.

## Two complete native proofs

Two independent full eight-TU /GL /GS cold links freeze the same 26 source/header
inputs. Primary EnemyEclDispatcher.cpp and real DispatchEclInstruction entry,
with ordered EclVm/AnmManager/AnmVmCreate/EnemyLaser/Enemy/AnmVmId/RandomMath
supports, are explicit in the canonical recipe and `native-replay.json`.
Qualified MAP/PDB selection owns the helper in support-01-AnmManager.ltcg.obj.
The strict comparator consumes all 55 bytes and exhaustively replays both fields:

| Offset | Field | Independently decoded target |
| --- | --- | --- |
| 0x16 | REL32 AnmLoadedView::Release | 0x00447810 |
| 0x23 | REL32 operator delete | 0x004524A1 |

In each image the actual 242-byte Release callee also passes all 8 fields using
its existing target recipe adapted only to this carrier. This is a guard,
with no duplicate credit or migration of its canonical recipe. Each image
therefore passes 297 complete bytes/all 10 fields after target-field replay.

```sh
scripts/repo-python scripts/build-match-unit.py --unit anm-release-slot
scripts/repo-python scripts/compare-linked-function.py --unit anm-release-slot --json
```

## Incoming caller and large owners remain open

The target's sole direct caller is `0x40C2F8` in the currently source-absent authored
owner `0x40BD80`, not the 95-byte Enemy resource loader. This diagnostic retains the
helper through `/OPT:NOREF`; it does not reproduce that incoming source edge.
Original source partition and data ownership remain unknown. The actual outgoing
Release edge and complete helper bytes are covered by both strict proofs.

The fresh eight-TU baseline corrects the stale Chain.hpp input in the old
snapshot. Dispatcher remains 14524/14416, frame 0x2C4, and ANM ExecuteScript remains
9948/9964. Neither parent gets exact or partial-case credit. Broad byte masks
and diagnostic scores are not canonical equality.

Follow-up target review confirms `0x40BD80` interprets four-byte variable-record
headers and 18 switch destinations at `0x40C358`. The pinned MIT reference suggests
ending-text recovery, but its volatile casts, forced helpers and explicit output
slots are not adopted. Review the current 1494-byte boundary and following
alignment/table ownership before reconstructing that larger incoming owner;
this checkpoint leaves its ledger untouched.

## Focused host regression and checkpoint

`test-anm-release-slot.py` extracts the actual production body and checks invalid
indices, an empty valid slot, ordinary release/delete and a synthetic Release
callback replacing the manager slot. Its cached-pointer mutant must fail exactly
the replacement assertion. All host objects have real lifetimes; survivors are
cleaned in both variants. ASan/UBSan and leak detection pass. The minimal owner,
Release and destructor are explicit host seams. The replacement witness does not
claim that maintained Release normally replaces the slot or that target runtime
was exercised. Temporary fixture binaries clean themselves up.

The initial CI stopped at the whole-file TransitionEffect dependency pin.
Reversing only these two statements reproduces the complete old raw input and
its old LF pin; the actually extracted AnmRandomU32InRange body is unchanged.
Only that input's normalized acceptance hash is advanced with a review record.
Historical raw observations and strict normalization/rejection policy remain
unchanged. The focused 843-assertion suite and 19 pin/EOL controls pass before
fresh complete CI; no native source input changed after the two proofs.

One reload-only and one array-count diagnostic compile just AnmManager.cpp,
reusing seven hash-checked objects after verifying the other 25 project inputs.
Those controls are not cold proofs. The two final complete proofs rebuild all
eight TUs separately. Only the changed complete function and its actual callee
are strictly replayed; no historical full native cohort is rebuilt.
See `validation-summary.json` for the completed CI, checks and cleanup.
Pre-existing D3D evidence stays untouched. Product/runtime, semantic and port
gates remain open.
