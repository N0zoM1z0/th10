# Stage draw call sites and fog-word recovery, 2026-10-28

Later same-day [dependency recovery](../stage-viewport-dependencies-20261028/README.md)
resolves the viewport/fog declarations and corrects Main's full-word cache.
The measurements and source hashes below describe the earlier checkpoint.

The complete early (0x402850, 1099 bytes) and late (0x402CA0, 929 bytes)
draw owners remain **nonexact**. This checkpoint recovers their target-backed
call sites, temporary scopes and raw-word accesses. It adds **zero exact bytes**
and **zero source-present owners**; both bodies were already maintained.

Only direct local tools were used after the operator's routing correction.
The canonical Japanese v1.00a disk target passed SHA-256 verification; local
Ghidra `check` and the bounded two-function decompilation each emitted the full
six-sample attestation marker. The earlier Factory check returned `not-checked`,
not a passed attestation, and is not used as target evidence.

## Complete native experiments

Each trial cold-compiled the same nine-TU StageCreate graph, serially on one
CPU at nice 10, with a 240-second bound and retained terminal logs. Every input
uses the unchanged canonical `/GL /GS` profile. Numbers below are **contribution
sizes**, not matching-byte counts or partial exactness scores.

| Source model | Early / target 1099 | Late / target 929 |
| --- | ---: | ---: |
| Fresh current-source baseline | 918 | 917 |
| Branch-local rectangles and explicit layer calls | 994 | 849 |
| Three fog setters at each real call site | 1106 | 960 |
| Alpha byte accesses | 1096 | 949 |
| Fog DWORD copied from the original member | 1080 | 933 |

All five images pass the **same five complete canonical contracts**, using only
an output-directory override: Stage update304, timer Tick72, update adapter7,
early adapter7 and late adapter7, **397 bytes / all 18 fields**. No comparison
rule, target binding, profile, canonical graph, source mapping, boundary, origin
or exact credit changed. Two dependency ledger comments are corrected below.
This is focused cold-image regression, not a replay of the complete exact set.

The maintained bounds/draw-instance/camera dependencies still structurally
reject at 1925/1984, 564/592 and 330/330 respectively in every trial. Equal
camera extent is not equality. The diagnostic images contain unresolved-symbol
data anchors and are never executed as game products.

## Target observations and retained source changes

- The two early color-clear branches independently initialize their rectangles
  at 0x4029C9 and 0x402A15. Moving initialization into each branch restores the
  early `0x20` local frame; the old shared rectangle used `0x10`. Stack-slot
  offsets and instruction scheduling are still not fully equal.
- Early calls the instance renderer eight times, for layers0..7; late calls it
  four times, for layers8..11. Explicit real calls replace both source loops.
  The first model also naturally restores late's outlined timer Add call;
  the old baseline inlined its arithmetic. No inline/ABI control is added.
- Target setters34/36/37 occur directly inside each parent, at early
  0x402929..0x402983 and late0x402E20..0x402E80. The old source-only
  `StagePublishFog` boundary produced a separate native call. Its removal
  restores the three actual setter sequences and their per-setter captures.
- Early0x402A95 compares the alpha byte at stage+0x1EEB; late0x402FAA writes
  that byte toFF before inspecting flags. Explicit unsigned-character access
  recovers these operations. The previous full-word alpha clear already
  compiled to a byte clear; that operation is not claimed as a new gain.
- Fog start/end are raw DWORD arguments. Each target reads the member intoEDI,
  calls Flush and pushes the captured word. The by-value float/union helper
  caused stack save/reload traffic in the expanded native parents. Copying the
  representation from `const float&` with `memcpy` restores the direct integer
  captures without aliasing casts, numeric conversion or an x87 operation.

The target dense decode covers all1099/929 bytes, with72/78 address operands,
18/15 direct local branches and43 calls in each owner. These are exhaustive
syntactic inventories, **not accepted candidate-to-target semantic bindings**.
Final candidate inventories still have73/79 link fields and their extents fail.
No structural mismatch is waived or scored as an exact parent.

## Remaining work

Late's933-byte owner has243 instructions, as does target929. Its extra four
bytes include a literal camera-pointer store where target storesEDI. Four fog
cache-update sites also schedule the vtable load/device push differently.
The four-byte size gap therefore does **not** mean four ordinary wrong bytes.
Early retains additional rectangle scheduling, VM-loop and private-callee
ABI differences. Do not reopen a register/spelling panel from extent proximity.

A newly rechecked target dependency supplies a concrete next route: 0x405300
selects a280-byte camera record at supervisor+0x154+index*0x118, publishes
supervisor+0x384, calls0x421480, sets the viewport through supervisor+8 and
stores the index at supervisor+0x388. Its old ledger prose describing resource
attachment is not supported by this decode and is corrected in-place. The53-byte0x420CD0 helper reads
and clears a **DWORD** at supervisor+0x750, flushes and sets render state28.
Main.hpp currently exposes a byte at that cache position, so real shared-owner
recovery needs a width/alias review before integrating those definitions.
Neither helper is admitted as source or exact in this checkpoint.

## Validation and reproduction

The three existing Stage draw-pass, camera and bounds/draw host tests pass.
The draw-pass fixture additionally checks signed zero, infinity, NaN payloads,
and a Flush that changes both fog members between their separate captures,
for both passes. Two isolated negative controls (discarding the sign bit and
capturing the end word too early) fail at the intended assertions. These are
synthetic host observations, not native D3D or target runtime evidence.

The required CI command was run once with a420-second/8MiB-log bound and
terminated at the time limit (exit124) during direct-creation caller controls.
All preceding stages returned successfully; the run is **incomplete**, not a
full-CI pass. Its exact stage count and log hash are in the result record.
Source/header pins are unchanged afterward. Toolchain declaration check,
tracking, regenerated progress and whitespace checks pass. No full cold replay
was run. Documentation/ledger-comment edits occurred during CI; no whole-input
freeze is claimed.

To reproduce the final native graph through repository tools:

```sh
scripts/repo-python scripts/build-match-unit.py --unit stage-update
scripts/repo-python scripts/compare-linked-function.py --unit stage-update --json
scripts/repo-python scripts/compare-linked-function.py --unit anm-timer-tick --json
scripts/repo-python scripts/compare-linked-function.py --unit stage-update-callback --json
scripts/repo-python scripts/compare-linked-function.py --unit stage-draw-early-callback --json
scripts/repo-python scripts/compare-linked-function.py --unit stage-draw-late-callback --json
```

Run compiler/linker commands serially under the documented CPU/nice/time bounds.
The PDB contributions for `?StageDrawEarly@@YGHPAUStageScriptView@@@Z` and
`?StageDrawLate@@YGHPAUStageScriptView@@@Z` belong to
`support-01-StageDraw.ltcg.obj`, not the primary StageScript object.
[The compact result record](results.json) pins actual trial inputs, images,
complete extents and guard receipts. Working producers, full inventories and
logs are retained under `.analysis/gpt-6.1-sol/stage-draw-20261028/`; builds are
under `build/gpt-6.1-sol/stage-draw-20261028/`. Generated Ghidra text stays ignored.
Product closure, shared storage, original C++ spelling/TU and runtime remain open.
