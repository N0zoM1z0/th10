# TH10 Exact-Reconstruction Handoff

## Checkpoint state

- Repository `th10`, branch `main`, target `target:th10-main`, analysis provider `th10-ghidra`.
- Current packet base: `3fd501c gpt-5.6-sol: fix ANM exact owner boundaries`, branch `main`.
- Completed session checkpoint: `bd9b2e3 gpt-5.6-sol: promote exact PbgFile accessors`.
- Completed session checkpoint: `9b5e7eb gpt-5.6-sol: add exact replay workflow`.
- Completed session checkpoint: `4ed34ee gpt-5.6-sol: promote exact PbgArchive lifecycles`.
- Completed session checkpoint: `621add9 gpt-5.6-sol: promote exact LZSS state reset`.
- Completed session checkpoint: `88800b6 gpt-5.6-sol: report exact reconstruction backlog`.
- Completed session checkpoint: `96f17c6 gpt-5.6-sol: harden COFF extent diagnostics`.
- Completed session checkpoint: `d59f975 gpt-5.6-sol: batch probe exact backlog`.
- Completed session checkpoint: `26af714 gpt-5.6-sol: add LTCG linked-image diagnostics`.
- Completed session checkpoint: `f5a0c4f gpt-5.6-sol: add canonical LTCG replay`.
- Completed session checkpoint: `40c96ec gpt-5.6-sol: promote exact PbgArchive LTCG helpers`.
- Completed session checkpoint: `e4c9077 gpt-5.6-sol: promote exact LZSS LTCG helpers`.
- Completed session checkpoint: `37ebb83 gpt-5.6-sol: promote remaining exact LTCG helpers`.
- Completed session checkpoint: `ead21f9 gpt-5.6-sol: rank exact reconstruction backlog`.
- Completed session checkpoint: `d4af1a5 gpt-5.6-sol: recover ECL core lifecycles`.
- Completed session checkpoint: `e2fecf3 gpt-5.6-sol: recover ECL host hierarchy`.
- Completed session checkpoint: `2dbce7d gpt-5.6-sol: recover ANM VM lifecycle`.
- Completed session checkpoint: `260ac75 gpt-5.6-sol: recover ANM manager core`.
- Completed session checkpoint: `3e6de60 gpt-5.6-sol: recover ASCII text pipeline`.
- Completed session checkpoint: `e239b4b gpt-5.6-sol: recover ANM render buffer core`.
- Completed session checkpoint: `591f59b gpt-5.6-sol: recover ANM draw core`.
- Completed session checkpoint: `3616aeb gpt-5.6-sol: recover ANM camera projection`.
- Completed session checkpoint: `1923623 gpt-5.6-sol: recover ANM rotated draw modes`.
- Completed session checkpoint: `56ac2e8 gpt-5.6-sol: recover ANM projected 3D quad`.
- Completed session checkpoint: `9ad868a gpt-5.6-sol: recover ANM mode dispatch`.
- Completed session checkpoint: `fc3dce0 gpt-5.6-sol: recover ANM projected photo blend`.
- Completed session checkpoint: `f6d88dd gpt-5.6-sol: recover ANM direct 3D`.
- Completed session checkpoint: `09e4da8 gpt-5.6-sol: recover ANM generated geometry`.
- Completed session checkpoint: `2d8c48b gpt-5.6-sol: recover ANM radial trail and RNG`.
- Completed session checkpoint: `92db5f4 gpt-5.6-sol: recover ANM script variables`.
- Completed session checkpoint: `d994538 gpt-5.6-sol: reconstruct ANM script executor`.
- Completed session checkpoint: `94141ac gpt-5.6-sol: reconstruct ANM child VM lifecycle`.
- Completed session checkpoint: `8c254ad gpt-5.6-sol: reconstruct ANM manager update core`.
- Completed session checkpoint: `bdf8f14 gpt-5.6-sol: reconstruct ANM manager setup core`.
- Completed session checkpoint: `acfcf07 gpt-5.6-sol: reconstruct ANM resource and surface core`.
- Completed session checkpoint: `adfd172 gpt-5.6-sol: reconstruct Enemy ECL dispatcher`.
- Completed session checkpoint: `ff78689 gpt-5.6-sol: reconstruct Enemy callback core`.
- Completed session checkpoint: `d4352b5 gpt-5.6-sol: reconstruct generic ECL VM core`.
- Completed session checkpoint: `623cd37 gpt-5.6-sol: reconstruct GUI update and message cores`.
- Completed session checkpoint: `979522c gpt-5.6-sol: reconstruct Main execution corridor`.
- Completed session checkpoint: `048a3c8 gpt-5.6-sol: separate boundary inventory`.
- Completed session checkpoint: `e7615ce gpt-5.6-sol: reconstruct front-end options core`.
- Completed session checkpoint: `4142a89 gpt-5.6-sol: reconstruct front-end key config core`.
- Completed session checkpoint: `5b6a544 gpt-5.6-sol: reconstruct front-end selection core`.
- Completed session checkpoint: `41c0f09 gpt-5.6-sol: reconstruct front-end stage core`.
- Completed session checkpoint: `8edf352 gpt-5.6-sol: reconstruct front-end replay core`.
- Completed session checkpoint: `9cbe831 gpt-5.6-sol: reconstruct front-end practice core`.
- Completed session checkpoint: `5f719b6 gpt-5.6-sol: complete front-end practice draw`.
- Completed session checkpoint: `27b272a gpt-5.6-sol: reconstruct front-end score entry`.
- Completed session checkpoint: `3fd501c gpt-5.6-sol: fix ANM exact owner boundaries`.
- Completed session checkpoint: `7a2779f gpt-5.6-sol: recover exact ANM Draw3D`.
- Completed session checkpoint: `ac9541d gpt-5.6-sol: recover exact ANM Draw2D pair`.
- Completed session checkpoint: `ee13d3a gpt-5.6-sol: recover exact ANM 3D render state`.
- Planned current checkpoint subject: `gpt-5.6-sol: recover exact ANM generated draws`. Final commit hash is intentionally not self-recorded before the commit exists; recover it from live Git after checkpoint.
- Recovery continued from the clean boundary-inventory checkpoint. The ignored private target, existing `.analysis/`, toolchain, Wine prefix, Ghidra project, and build caches were preserved.
- Current campaign: `.analysis/gpt-5.6-sol/20260916-anm-ecl-exact-next/`. The earlier ANM/ECL exact triage, front-end/score-entry, front-end/practice-draw, front-end/practice-core, front-end/replay, front-end/stage, front-end/selection, front-end/key-config, front-end/options, boundary-inventory, Main, GUI, generic ECL VM, Player update, Enemy callback, Enemy high-opcode ECL dispatcher, ANM resource, manager-setup, manager-update, child-VM, executor, script-variable, radial-trail, generated-geometry, ANM direct-3D, mode-7, projected, draw, manager, VM, ECL host, ECL lifecycle, backlog-ranking, final-structural, Lzss, PbgArchive, canonical-replay, linked-diagnostic, and earlier session campaigns are checkpointed separately; earlier `gpt-web` campaigns remain ignored evidence and were not treated as current authority without replay.
- This session has not pushed. The exact-reconstruction campaign remains active/incomplete.

## Recovery and authority

The session inspected branch/HEAD/history, complete tracked/untracked state, and the prior handoff. Committed base `ee13d3a` is the current live authority.

All requested repository and Factory guidance was re-read from the live repository shell before tracked reconstruction work. No requested path was missing.

Fresh repository preflight passed at the start of the current front-end session:

- `python3 scripts/verify-target.py`
- `python3 scripts/verify-toolchain.py --execute`
- `python3 scripts/validate-tracking.py --require-target`
- `python3 scripts/report-reconstruction-status.py`

The execute toolchain path passed pinned VC7.1 SP1 build6030 normal COFF, C++ `/GL`, resource and PE32-i386 link smoke through headless Wine. Native `th10-ghidra` `check {}` passed for `target:th10-main` with `provider_transport=factory-native-command`. This packet combines that provider's read-only address-set ranges, decompilation, disassembly, calls and xrefs with direct hash-attested target disassembly.

Target remains the ignored operator file `resources/th10.exe`: size 487,936, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`, MD5 `7dc488d82c81dd4aee4ba098b8804d83`, PE32 i386 base `0x00400000`, entry `0x004537DC`, dominant Rich build6030. It was not modified, moved, staged or committed. `/mnt` was not searched and `TH10_TARGET_PATH` was not set.

## Completed packet: independent boundary inventory

Boundary state is now separate from origin state. `config/function-boundaries.csv`
contains one target-bound row for each of the 1,312 candidates, and
`scripts/validate-tracking.py` enforces its address order, one-to-one coverage,
span equality, state/confidence combinations, evidence and notes. Exact rows
must have a reviewed boundary.

`scripts/report-boundary-inventory.py --check-ledger` reproduces the automatic
review against the exact target with the pinned Capstone decoder. It densely
decodes each tracked extent, checks terminal control flow, independent code and
aligned non-text pointer references, overlapping extents, external branches into
another body, Ghidra sparse/remote ranges, `.text` union coverage and gaps. The
current ledger has **1,173 reviewed, 88 provisional and 51 needs-review**
boundaries. Tracked extents cover 340,424 union bytes. The remaining 71,865
`.text` bytes contain 519 pure alignment gaps / 4,186 bytes and 430 unresolved
gaps / 67,679 bytes. The report retains 26 overlapping extent pairs involving
37 candidates, four direct targets outside tracked extents and 97 aligned data
pointers into untracked `.text` as triage evidence.

The audit now also extracts absolute `.text` bases from indexed indirect jumps.
It finds 100 target references to such control tables: 61 table references are
inside their source owner's recorded span and 39 point beyond it. This is a
boundary-review queue, not automatic permission to extend those owners; table
length, selector domain, neighboring code/data and padding still require direct
target review.

The former inventory export treated each Ghidra function as one dense interval.
`python3 scripts/ghidra.py architecture` now exports every address-set range.
The current database has 1,419 ranges for 1,195 functions, including 122 sparse
bodies and two functions with ranges below their entry. `0x004451C0` owns remote
ranges at `0x004436C0-0x0044390E` and `0x00443910-0x00443B5E` in addition to
its entry range; `0x0046227B` owns the shared return range
`0x00452720-0x00452724`. These cases demonstrate why boundary review cannot use
`entry..maximum-address` as the physical owner.

Direct review added three Ghidra-missed front-end candidates. The reviewed
`0x0042D260-0x0042D2D3` body owns code through `0x0042D2B3` and an eight-entry
absolute switch table. The seven-byte adapters at `0x0042D2E0` and
`0x0042D2F0` move ECX to EAX and jump to `0x0042CDF0` and `0x0042D260`
respectively; target data references and following CC padding close both.

Changing the registered Ghidra wrapper/exporter caused the earlier provider
failure. The Factory compares an aggregate SHA-256 of native implementation
files with its private operator binding, so the message “native Ghidra
implementation differs from its operator binding” was a deliberate stale-binding
failure. The new implementation was reviewed, the ignored private binding was
refreshed to `c001f8ef3d9a64e96664d999c14891d9c92fa12f393bca64cc77bb087c0e2143`,
the Factory validator passed, and native `check {}` passed again. No tracked
Factory file changed.

`docs/PROGRESS.md` and `scripts/report-reconstruction-status.py` retain the
separate origin and boundary ledger counts. The generated SVG follows the TH09
status-card convention and shows one combined `Origin/boundary reviewed` bar;
its count is the conservative intersection of candidates whose origin and
boundary reviews are both complete. `scripts/rank-core-backlog.py` orders
source-absent, boundary-reviewed candidates by target size and optional Ghidra
connectivity while explicitly granting no ownership or semantic credit.

## Completed packet: exact ANM generated draws

The refreshed dual-lane rank placed `DrawGeneratedVertices @ 0x00444CE0` and
`DrawTexturedTriangleFan @ 0x004450E0` immediately after the retained
two-byte radial-trail near match. Their linked `/GL` contributions already had
the exact 224- and 211-byte target sizes; each remaining 13-byte region was the
same texture-bind scheduling pattern recovered in Draw3D.

Both target paths push the new texture and update renderer cache `+0x3ADA64`
before loading the D3D9 device. Expressing only these texture binds through the
TH10-proved `MainSupervisorView +0x8` device field restores that order. The
other D3D9 calls remain expressed through the standalone device global. No
volatile qualifier, padding, assembly, or copied bytes is used.

Two focused cold replays close `DrawGeneratedVertices` at **224/224 bytes and
7/7 fields** and `DrawTexturedTriangleFan` at **211/211 bytes and 10/10
fields**. The final full-source cold gate closes all **71 configured
`src/AnmManager.cpp` units / 10 artifacts / 14,054 target bytes**. Repository
totals are now **121 exact functions / 16,045 bytes**. The ANM module has 61
exact owners / 13,218 bytes and 62 source-present authored owners / 28,489
bytes remaining.

## Completed packet: exact ANM 3D render state

`AnmRenderManagerView::SetRenderStateForVm3D @ 0x004423E0` previously matched
396/408 comparable bytes across its complete 440-byte linked `/GL` PDB
contribution. Its only difference was the texture-factor state call: the
candidate loaded the standalone device global before pushing/storing the new
cache value, while the target performs the push and store first.

The already established Main layout proves that target address `0x00491C30`
is `MainSupervisorView +0x8`. Expressing this one call through the same
source-local supervisor prefix used by exact Draw3D restores the target
instruction schedule. The canonical manifest binds that field to supervisor
base `0x00491C28` with addend eight; the other device uses remain tied to the
standalone global. No volatile qualifier, padding, assembly, or copied bytes is
used.

Two focused cold replays close the complete **440/440 bytes and 8/8 fields**.
The final full-source cold gate closes all **69 configured
`src/AnmManager.cpp` units / 10 artifacts / 13,619 target bytes**. Repository
totals are now **119 exact functions / 15,610 bytes**. The ANM module has 59
exact owners / 12,783 bytes and 64 source-present authored owners / 28,924
bytes remaining.

## Completed packet: exact ANM Draw2D pair

The two 591-byte rotated two-dimensional draw owners at `0x004436C0` and
`0x00443910` previously reproduced every target field and all but one
equal-length scheduling region. In the near match, VC7.1 began loading the Y
coordinate before completing the horizontal-anchor `AND/SUB`; the target
completes that integer normalization first.

The maintained source had split each three-term coordinate sum across one
assignment and two compound assignments. Restoring each to one naturally
left-associated C++ expression retains the same operand order and arithmetic,
while giving VC7.1 the target instruction schedule. Both complete linked `/GL`
PDB contributions now replay exactly: **591/591 bytes and 21/21 fields each**.
No padding, volatile dependency, copied bytes, or new assembly was introduced.

Two focused cold replays close both owners. A final full-source cold gate closes
all **68 configured `src/AnmManager.cpp` units / 10 artifacts / 13,179 target
bytes**. Repository totals are now **118 exact functions / 15,170 bytes**. The
ANM module has 58 exact owners / 12,343 bytes and 65 source-present authored
owners / 29,364 bytes remaining.

## Completed packet: exact ANM Draw3D

Fresh ANM/ECL dual-lane ranking after `3fd501c` leaves the two-byte X87
`UpdatePulsingRadialTrail` experiment first and places the 929-byte
`AnmRenderManagerView::Draw3D @ 0x00444760` second. Its diagnostic linked `/GL`
contribution already had exact size, all 25 fields visible and only thirteen
non-field bytes different. Those bytes form one contiguous instruction-order
change around the texture-cache update.

Three ordinary source spellings through the standalone `g_Direct3DDevice`
pointer all emitted the same device-load/push/store order. Target-local Main
layout establishes that address `0x00491C30` is also the `d3dDevice` field at
`MainSupervisorView +0x8`, based at `0x00491C28`. Expressing the texture call
through a source-local supervisor prefix changes VC7.1's alias scheduling to
the exact target push/cache-store/device-load sequence. The source continues to
perform the same cache update and COM call; no volatile qualifier, inline
assembly, padding or copied bytes are used.

The canonical linked unit covers the complete 929-byte PDB contribution and
all 25 fields. Its supervisor field manifest records base `0x00491C28` with
addend eight, which disambiguates the address from a neighboring four-byte
diagnostic anchor. Two cold replays close Draw3D; the second also closes the
four adjacent renderer owners in the same artifact for **1,756/1,756 bytes**.
The final full-source regression gate closes all **66 configured units / 10
cold artifacts / 11,997 target bytes** with no failure.
Repository totals are now **116 exact functions / 13,988 bytes**. The ANM module
has 56 exact owners / 11,161 bytes and 67 source-present authored owners /
30,546 bytes remaining.

## Completed packet: ANM exact boundary repair

Dual normal-COFF and linked `/GL` backlog ranking exposed a systematic boundary
error: eight reviewed spans ended on the `C2` opcode of `RET imm16` but omitted
its two-byte immediate. The boundary audit had reported incomplete dense decode,
yet its prior-review path retained `reviewed` state. The corrected extents are:

- compiler-owned deleting helpers `0x00425070-0x0042508A` and
  `0x004296D0-0x004296EA`;
- `TranslateRotation @ 0x00443680-0x004436B3`;
- `QueueSpriteQuad @ 0x00444DC0-0x00444E50`;
- untextured strip/fan `0x00444E60-0x00444F9A` and
  `0x00444FA0-0x004450DA`;
- `LoadSurface @ 0x00447C80-0x00447EBF`;
- `CopyTextureRect @ 0x00448360-0x00448442`.

`scripts/report-boundary-inventory.py` now recognizes a one- or two-byte
trailing `C2/CA` suffix as a truncated `RET imm16` and refuses to preserve a
reviewed result for it. All eight corrected owners densely decode through a
complete return. The correction adds sixteen tracked `.text` bytes without
changing the candidate or review-state counts.

Four corrected ANM owners were already exact linked `/GL` contributions once
the missing immediates were admitted: 52-byte `TranslateRotation`, 145-byte
`QueueSpriteQuad`, and the two 315-byte untextured strip/fan methods. Their
complete canonical manifests cover 827 bytes and 37 linked fields. Normal COFF
also closes the adjacent 66-byte `MarkVmForDeletion` plus its sole `FindVm`
REL32 relocation. Two independent cold aggregate replay passes return exact
for all five units, **893/893 bytes**. Repository totals are now **115 exact
functions / 13,059 bytes**; ANM accounts for 55 exact owners / 10,232 bytes,
with 68 source-present authored owners / 31,475 bytes remaining.

The same ranking places `UpdatePulsingRadialTrail @ 0x00445620` at 598/598
bytes with all eighteen fields resolved and 524/526 comparable bytes equal.
The two differences swap commutative X87 operands for only the Y component of
`position + positionOffset`. Bounded natural-source variants did not reproduce
that scheduling, so this owner remains honestly non-exact.

The final full-source regression gate also caught a stale source-shape
interaction in an older exact owner. Typing ASCII manager field `+0x89A4` as
the non-trivial `AnmVmIdView` had inserted a target-absent pre-clear before the
constructor's full-object `memset`, growing its linked contribution from the
canonical 251 bytes to 255. Removing that constructor globally repaired ASCII
but changed exact `AddVmVariant0` from 118 to 119 bytes. The maintained layout
now expresses only this ASCII field as its target-used scalar integer storage;
general VM-id return values retain their established wrapper semantics. A
focused cold replay closes the 251-byte ASCII constructor and both 118-byte
AddVm tails. The final full `src/AnmManager.cpp` gate closes all **65 configured
units / 10 artifacts / 11,068 compared bytes** with no failure.

## Completed packet: front-end score entry

The score-entry screen discriminator 15 now routes to maintained update and draw
owners instead of unresolved external stubs:

- `FrontEndControllerView::UpdateScoreEntry @ 0x00432CB0-0x00433227`
  (1,400 bytes)
- `FrontEndControllerView::DrawScoreEntry @ 0x00433230-0x00433569`
  (826 bytes)

The four-state updater inserts the current score into the selected
character/shot/difficulty table, loads the profile's last eight-character name,
and drives a 91-entry wrapping keyboard laid out in thirteen columns. The final
three entries are space, backspace and accept. Accept copies the name into both
the inserted 0x18-byte score record and profile offset `+0x1D878`; cancellation
erases one character, while an insertion failure skips directly to the result
screen after confirmation. The owner creates and retires the target
`0x68/0x98+/0x9A+/0xA0+` ANM scripts and preserves the target opening/closing
timer thresholds.

The draw owner emits all ten score rows using the same target record layout as
the practice screen, highlights the inserted row, overlays the editable name and
caret on that row, and draws all 91 glyphs at `(212,360)` with 18-pixel columns
and 16-pixel rows. Controller layout review binds the name at `+0x58DC`, name
cursor at `+0x58E8`, insertion-failure flag at `+0x58EC`, and a complete second
`FrontEndCursorView` at `+0x58F4`; its target-written wrap field is therefore
the existing cursor member at `+0x59C4`.

Boundary review extends the update owner beyond Ghidra's executable end. Code
ends with `RET 4` at `0x00433213-0x00433215`, `MOV EDI,EDI` occupies
`0x00433216-0x00433217`, and the directly indexed four-entry table at
`0x00433218-0x00433227` targets `0x00432CCC`, `0x00432EDA`, `0x00432F00`, and
`0x004331AC`. Eight `CC` bytes separate the draw entry; its `RET 4` ends at
`0x00433569`, followed by six `CC` bytes. The focused
`scripts/report-frontend-score-entry.py --check` binds these extents, both
dispatcher call sites, direct-call multisets, alphabet/table bytes, source
markers and controller offsets to the canonical target.

Pinned VC7.1 SP1 build6030 compiles the expanded `src/FrontEnd.cpp` under normal,
`/GL`, and `/W4` profiles; `/W4` adds only the established anonymous-union
extension warnings. Normal COMDATs are 1,384/781 bytes and mismatch at 72/1,096
and 22/718 comparable bytes. With `UpdateScoreEntry` selected as the linked
`/GL` entry, PDB contributions are 1,392/781 bytes and mismatch at 88/1,100 and
22/718 comparable bytes. Every diagnostic has `acceptance_authority=none`; no
exact row is added. Repository totals become 282 source mappings and remain 110
exact functions / 12,166 bytes.

## Completed packet: front-end/options core

The selected title/options chain now has maintained source across three authored
owners and 8,315 reviewed target bytes:

- `FrontEndControllerView::Update @ 0x0042CDF0-0x0042D257` (1,128 bytes)
- `FrontEndControllerView::UpdateOptions @ 0x0042D920-0x0042E593` (3,188 bytes)
- `FrontEndControllerView::RefreshOptionsDisplay @ 0x0042E5A0-0x0042F53E`
  (3,999 bytes)

Boundary review corrected the first two imported extents. The title dispatcher
ends executable code at `0x0042D212`, has one NOP, and owns its directly
referenced 17-entry screen table through `0x0042D257`. The options controller
ends with `RET 4` at `0x0042E57D-0x0042E57F` and owns its directly referenced
five-entry state table through `0x0042E593`. Both tables were previously left
in untracked gaps by the terminal-instruction heuristic.

The source covers the complete 17-screen title dispatcher, 900-frame demo replay
rotation, front-end ANM cleanup and re-entry, the five-state/six-row options
controller, color-depth cycling, BGM/SFX adjustment and preview, defaults,
key-configuration transition and return. The display owner reproduces the
observed sound-state update and quartic BGM attenuation curve, queues `SetVol`,
updates twelve digit sprites and toggles the four normal/selected leading-digit
pairs.

Repeated child traversal in both this owner and the earlier GUI source compares
ANM VM `scriptIndex` at `+0x38A`. The maintained GUI helper had instead compared
`activeSpriteIndex` at `+0x384`; this packet corrects that semantic field error.
`FrontEndCursorView` is closed at `0xD8`, and the maintained partial controller
binds screen/state/cursor at `+0x1C/+0x20/+0x24`, timer at `+0x2B0`, and VM ids
from `+0x2C4` through slot `0x5B` at `+0x430`.

`scripts/report-frontend-core.py --check` fails closed on the target and verifies
the two tables, tails/padding, selected direct-call multiplicities, source marker
coverage, state enums, layout assertions and the `+0x38A` lookup invariant.
Pinned VC7.1 SP1 build6030 compiles `src/FrontEnd.cpp` under fixed normal,
`/GL`, and `/W4` profiles. All three normal and linked diagnostics are real
mismatches with `acceptance_authority=none`: normal object sizes are 324, 928
and 203 bytes; selected linked `/GL` contributions are 368, 884 and 199 bytes.
The target owners are dominated by optimizer-inlined helpers and repeated VM
lookup paths, so no match row or exactness promotion is made.

Repository totals at that checkpoint became **1,312 candidates, 266 source
mappings and 110 exact functions / 12,166 bytes**.

## Completed packet: front-end key-configuration core

The connected continuation now has maintained source across three more authored
owners and 3,532 reviewed target bytes:

- `FrontEndControllerView::UpdateKeyConfig @ 0x0042F540-0x0042F8AF`
  (880 bytes)
- `FrontEndControllerView::RefreshKeyConfigDisplay @ 0x0042F8B0-0x00430243`
  (2,452 bytes)
- `FrontEndControllerView::AssignKeyConfigBinding @ 0x00430250-0x00430317`
  (200 bytes)

The first owner ends code at `0x0042F89A`, has one NOP, and owns the five-entry
state table at `0x0042F89C-0x0042F8AF`. This closes the table that the boundary
inventory had previously reported outside its owner. The display and assignment
owners are separated from their successors by twelve and eight `CC` bytes.

The five-state/seven-row controller creates key-config root VM slot 2, scans 31
signed controller-button transition bytes, handles cursor interrupts, edits the
five configurable actions, resets from live mappings, commits the complete
mapping and returns to options. Duplicate button assignments are resolved by
swapping the displaced binding. The display path updates twenty digit VMs: two
digits in each of the normal and selected rows for each editable action.

TH10-local input and configuration consumers establish the runtime mapping at
`0x00474E88` as nine adjacent signed shorts with target defaults
`0,1,2,3,-1,-1,-1,-1,4`. Target button masks identify the first four as
shot/bomb/focus/menu and the last as skip. The maintained up/down/left/right
names for the preserved middle four entries are adjacent-supported and remain
provisional because the reviewed TH10 controller path derives direction bits
from axes. The key-config scratch copy contains the first four actions plus skip
at controller `+0x59CC`; commit preserves the middle entries and copies the
complete 18-byte mapping to persisted storage at `0x00491D4C`.

`scripts/report-frontend-key-config.py --check` fails closed on the exact target
and validates all three extents, the five-entry table, tails/padding, complete
direct-call multisets, the display owner's 40 FindVm transfers and 19 SetSprite
calls plus one tail jump, the nine-short defaults, source markers/state coverage
and layout assertions.

Pinned VC7.1 build6030 normal, `/GL` and `/W4` compiles pass. The new normal
COMDATs are 624/113/171 bytes versus target 880/2,452/200 and remain real
mismatches at 15/700, 9/2,432 and 19/192 comparable bytes, each with
`acceptance_authority=none`. No match row or exactness promotion is made.

Repository totals become **1,312 candidates, 269 source mappings, 285 reviewed
authored owners and 110 exact functions / 12,166 bytes**. Boundary coverage is
340,260 `.text` bytes; the indexed-table queue is now 54 references inside
their recorded owner and 46 outside.

## Completed packet: front-end selection core

The next contiguous front-end corridor now has maintained source across three
authored owners and 3,268 reviewed target bytes:

- `FrontEndControllerView::UpdateDifficulty @ 0x00430320-0x00430697`
  (888 bytes)
- `FrontEndControllerView::UpdateCharacter @ 0x004306A0-0x00430A5B`
  (956 bytes)
- `FrontEndControllerView::UpdateShotType @ 0x00430A60-0x00430FEF`
  (1,424 bytes)

All three owners use a five-state selection protocol. Difficulty distinguishes
the four normal difficulties from Extra, selects VM root slot `0x77/0x78`,
manages screen VM `0x5E`, auxiliary VM `+0x5D0` and transition VM `0x62`, then
advances to character selection or restores the title-menu state. Character
filters both entries from the six Extra unlock bytes at profile
`+0x1D888..+0x1D88D`, drives the horizontal ANM cursor and sends four
complementary child interrupts on confirmation. Shot type filters three
per-character unlocks, suppresses three unavailable profile-record child rows,
drives the vertical ANM cursor and commits normal-game or practice stage state.
The normal path creates the observed ASCII VM at `(480,392)`, starts the
32-frame transition and publishes stage record zero or six, stage one or seven,
and game mode seven.

Boundary review closes three tables previously reported outside their owners.
Difficulty code ends at `0x00430680`, has a three-byte alignment instruction,
and owns its five-entry table at `0x00430684-0x00430697`. Character code ends at
`0x00430A47` and its table immediately follows through `0x00430A5B`. Shot-type
code ends at `0x00430FDA`, has one NOP, and owns its table at
`0x00430FDC-0x00430FEF`. The controller layout now covers VM slots through
`0x97`, the auxiliary difficulty VM id at `+0x5D0`, saved difficulty at
`+0x58F0`, and the existing key-config scratch at `+0x59CC`. The ASCII manager
view names its separately observed auxiliary VM id at `+0x89A4`.

`scripts/report-frontend-selection.py --check` fails closed on the canonical
target and validates the three complete owners, table destinations,
alignment/tails, complete direct-call multisets, five state values, source
markers and the new layout offsets. The existing front-end/options and
key-config reports still pass after the shared layout expansion.

Pinned VC7.1 build6030 normal, `/GL` and `/W4` compiles pass. Normal COMDATs are
876/872/1,196 bytes against target owners 888/956/1,424. Strict normal
diagnostics are real mismatches at 40/640, 49/740 and 49/1,120 matched
comparable bytes, with 62/54/76 relocations and `acceptance_authority=none`.
No match row or exactness promotion is made.

Repository totals become **1,312 candidates, 272 source mappings, 288 reviewed
authored owners and 110 exact functions / 12,166 bytes**. Boundary coverage is
340,324 `.text` bytes; the indexed-table queue is now 57 references inside
their recorded owner and 43 outside.

## Completed packet: front-end stage core

The stage-selection continuation now has maintained source across two authored
owners and 1,471 reviewed target bytes:

- `FrontEndControllerView::UpdateStage @ 0x00430FF0-0x0043140B`
  (1,052 bytes)
- `FrontEndControllerView::DrawStageScores @ 0x00431410-0x004315B2`
  (419 bytes)

The five-state update owner restores a six-entry cursor, creates the stage and
character-specific ANM VMs, handles movement/cancel/confirm, rejects unavailable
profile stages, records numeric-key stage shortcuts, and completes the
stage-record/game-mode transition. Direct target review identifies one shared
256-byte keyboard buffer: the helper at `0x0044B010` returns nonzero after the
DirectInput path, selecting scan codes `2..10`, and zero after the Win32
`GetKeyboardState` path, selecting virtual-key codes `0x31..0x39`.

The display owner is called at `0x0042D2A8` with the controller retained in
private EDI. It draws the six stage rows at x=168 or 296 and y=216 with 18-pixel
spacing, prints the stored score or an unavailable marker, distinguishes the
current row by color, blinks it during confirmation, and restores ASCII color
and shadow state. Profile entries are eight bytes: score at `+0`, an unknown
byte at `+4`, availability at `+5`, then two unknown bytes. The selected record
is indexed by stage `1..6` plus difficulty times six within the
character/shot-group block.

Boundary review extends the update owner past its final `RET 4` at
`0x004313F4-0x004313F6`: one NOP follows and its directly indexed five-entry
state table occupies `0x004313F8-0x0043140B`. Four `CC` bytes separate the
display owner, whose RET at `0x004315B2` is followed by thirteen `CC` bytes.
This adds 21 bytes to tracked `.text` union coverage and moves one indexed table
inside its owner.

`scripts/report-frontend-stage.py --check` fails closed on the canonical target
and validates both extents, the state table, tails/padding, complete direct-call
multisets, draw-callback call site, stage-name pointers/strings, 18.0 row
spacing, source markers, profile layout tokens and dispatcher integration.

Pinned VC7.1 build6030 normal, `/GL` and `/W4` compiles pass; `/W4` reports only
the existing anonymous-union extension warnings. Normal COMDATs are 660 and 378
bytes versus target owners of 1,052 and 419 bytes. Strict diagnostics are real
mismatches at 26/876 and 5/371 matched comparable bytes, with 44 and 12
relocations and `acceptance_authority=none`. No match row or exactness promotion
is made.

Repository totals become **1,312 candidates, 274 source mappings, 290 reviewed
authored owners and 110 exact functions / 12,166 bytes**. Boundary coverage is
340,345 `.text` bytes; the indexed-table queue is now 58 references inside
their recorded owner and 42 outside.

## Completed packet: front-end replay core

Replay selection now has maintained source across two authored owners and 2,317
reviewed target bytes:

- `FrontEndControllerView::UpdateReplay @ 0x004315C0-0x00431B93`
  (1,492 bytes)
- `FrontEndControllerView::DrawReplay @ 0x00431BA0-0x00431ED8`
  (825 bytes)

The six-state update owner loads the twenty-five numbered replay slots, scans up
to twenty-five additional `th10_ud????.rpy` files, preserves the target's
unconditional `FindClose` error path, and constructs the seven-stage cursor from
the selected replay's stage headers. Confirmation publishes the selected stage,
stage record, replay path, character, shot type and difficulty before entering
game mode 12. Cancellation releases all fifty replay pointers and the replay
screen's ANM VM tree before restoring the outer cursor.

The display owner draws twenty-five fixed replay rows with replay name, date,
character/shot, difficulty, completion marker and slowdown percentage. After a
file is chosen, its summary moves from the selected list row to y=80 over ten
frames, then seven stage rows appear at x=220/y=128 with 18-pixel spacing. For
stages before the last available stream, the displayed score and trailing digit
come from the next stage header at `+0x0C/+0x1B4`; otherwise they come from replay
header `+0x10/+0x60`. Controller layout assertions bind the replay fields at
`+0x59D8/+0x59DC/+0x59E0` and the fifty-pointer array at `+0x59E4..+0x5AAB`.

Boundary review corrects the prior code-only extent for the update owner. Its
`RET 4` ends at `0x00431B79`, `MOV EDI,EDI` occupies `0x00431B7A-0x00431B7B`,
and the directly indexed six-entry table spans `0x00431B7C-0x00431B93` with
destinations `0x004315F2`, `0x0043174D`, `0x004317B1`, `0x004319B4`,
`0x004318FD` and `0x00431A9E`. Twelve `CC` bytes precede the display owner; its
`RET 4` is followed by seven `CC` bytes before `0x00431EE0`. This adds 26 bytes
to tracked `.text` union coverage and moves one indexed table inside its owner.

`scripts/report-frontend-replay.py --check` fails closed on the canonical target
and validates the two extents, table/tails, complete direct-call multisets,
update/draw callback call sites, name tables, display constants, source markers,
state domain and controller offsets.

Pinned VC7.1 build6030 normal, `/GL` and `/W4` compiles pass; `/W4` reports only
the existing anonymous-union warnings. The normal COMDATs are 1,588 and 554
bytes versus target owners 1,492 and 825, with 89/1,184 and 33/753 comparable
bytes matching and 88/18 relocations. In the selected replay-update linked
`/GL` context the contributions are 1,672 and 554 bytes with 102/1,196 and
33/753 comparable bytes matching. Every diagnostic is a real mismatch with
`acceptance_authority=none`; no match row or exactness promotion is made.

Repository totals become **1,312 candidates, 276 source mappings, 292 reviewed
authored owners and 110 exact functions / 12,166 bytes**. Boundary coverage is
340,371 `.text` bytes; the indexed-table queue is now 59 references inside
their recorded owner and 41 outside.

## Completed packet: front-end practice draw and discriminator correction

The practice-record screen is now complete through its draw path, and the
front-end draw dispatcher itself has maintained source:

- `FrontEndControllerView::Draw @ 0x0042D260-0x0042D2D3` (116 bytes)
- `FrontEndControllerView::DrawPractice @ 0x004329F0-0x00432CA0`
  (689 bytes)

Reviewing the target draw table exposed a semantic error in the maintained
screen enum. Target discriminator 11 dispatches to `UpdatePractice @
0x00431EE0` and `DrawPractice @ 0x004329F0`; discriminator 12 dispatches to
`UpdateReplay @ 0x004315C0` and `DrawReplay @ 0x00431BA0`. The enum is corrected
accordingly, so every maintained `SetScreen` and both central dispatchers now
use the target-proven values.

The practice draw owner renders ten 0x18-byte score records on summary page
zero. Each record supplies a score, stage-name selector, suffix digit,
ten-byte name, timestamp and slowdown percentage. Rows fade from white by
subtracting `0x10` from the red and green channels for each successive entry.
The footer
always prints the selected shot's aggregate value at profile `+0x4C8`, converts
the frame total at `+0x4CC` to `hours:minutes:seconds`, and prints the selected
difficulty value at `+0x4D0 + difficulty*4`.

The draw dispatcher code ends at `0x0042D2B3` and owns its eight-entry table at
`0x0042D2B4-0x0042D2D3`. Its `0x0042D2F0` callback adapter remains a separate
seven-byte candidate. `DrawPractice` ends in `RET` at `0x00432CA0`, followed by
fifteen `CC` bytes. The expanded `scripts/report-frontend-practice.py --check`
validates these extents, both dispatch tables, complete direct-call multisets,
formats, screen discriminators, profile-access source markers and the earlier
practice update/refresh evidence.

Pinned VC7.1 normal, selected-entry `/GL`, and `/W4` probes pass. Normal `Draw`
and `DrawPractice` COMDATs are 116/585 bytes versus target owners 116/689 and
mismatch at 10/60 and 51/625 comparable bytes with 14/16 relocations. Selected
`/GL` contributions are 120/585 and mismatch at 15/64 and 54/625 comparable
bytes. The equal normal size of `Draw` is diagnostic only; no exactness is
claimed.

Repository totals become **1,312 candidates, 280 source mappings, 296 reviewed
authored owners and 110 exact functions / 12,166 bytes**. Boundary coverage
remains 340,390 `.text` bytes and the indexed-table queue remains 60 references
inside their recorded owner and 40 outside.

## Completed packet: front-end practice-record core

The next front-end owner pair now has maintained source across 2,818 reviewed
target bytes:

- `FrontEndControllerView::UpdatePractice @ 0x00431EE0-0x0043268F`
  (1,968 bytes)
- `FrontEndControllerView::RefreshPracticeRecords @ 0x00432690-0x004329E1`
  (850 bytes)

The four-state updater maintains three independent cursors: six character/shot
combinations at controller `+0x24`, five difficulties at `+0x0FC`, and record
pages at `+0x1D4`. It creates the character, shot, difficulty, navigation and
ten ASCII-row ANM VMs, refreshes the displayed page after selection changes,
cycles back to the summary page, and retires the complete VM set when returning
to the title menu. The ten row ids occupy controller `+0x5D4..+0x5FB`; the
number of populated rows is stored at `+0x2AC`.

The refresh owner filters the target's 110-entry spell-card catalog by its
difficulty byte table, skips ten matching records per preceding page, and emits
up to ten centered ANM text rows. Unlocked catalog names at profile
`+0x19A8C + index*0x90` are padded to 42 bytes; the two displayed counters come
from profile `+0x624 + shot*0x437C + index*0x90`. Locked entries use the target
placeholder format, and unused row VMs receive a single space.

The Extra/third-shot selection also enables the target's hidden keyboard path.
It snapshots all 256 keys, derives rising edges, accepts the 22-entry scan-code
sequence at `0x0046EF10`, resets on another press among codes `0..56`, and
expires after 300 frames. Completion invokes the profile-record unlock helper
and queues sound 44. This behavior is source-present; the helper at
`0x0042C8C0` remains a separate source-absent leaf.

Boundary review extends the updater past its prior code-only end. `RET 4` ends
at `0x0043267C`, `LEA ECX,[ECX]` occupies `0x0043267D-0x0043267F`, and the
directly indexed four-entry table spans `0x00432680-0x0043268F` with state
destinations `0x00431F06`, `0x00432095`, `0x004320BA`, and `0x00432615`. The
refresh helper ends in `RET` at `0x004329E1`; fourteen `CC` bytes separate it
from `0x004329F0`. This adds 19 tracked bytes and moves one indexed table inside
its owner.

`scripts/report-frontend-practice.py --check` fails closed on the canonical
target and validates both extents, the state table, tails/padding, complete
direct-call multisets, parent dispatch call, difficulty and secret-key tables,
row formats, source markers, state domain, and controller offsets. Existing
front-end focused reports remain regression gates.

Pinned VC7.1 build6030 normal, selected-entry `/GL`, and `/W4` builds pass. The
normal COMDATs are 1,692/535 bytes against target owners 1,968/850, with
70/1,592 and 28/786 matched comparable bytes and 94/16 relocations. The `/GL`
contributions are 1,764/543 bytes and mismatch at 65/1,596 and 23/786
comparable bytes. Every diagnostic has `acceptance_authority=none`; no match row
or exactness promotion is made.

Repository totals become **1,312 candidates, 278 source mappings, 294 reviewed
authored owners and 110 exact functions / 12,166 bytes**. Boundary coverage is
340,390 `.text` bytes; the indexed-table queue is now 60 references inside
their recorded owner and 40 outside.

## Completed packet: Main execution corridor

Ten central application/window/D3D owners now have maintained source in
`src/Main.cpp` across 5,456 reviewed target bytes:

- `WinMain @ 0x00438AD0`
- `GameWindowView::WindowProc @ 0x004390E0`
- `GameWindowView::Present @ 0x004391F0`
- `GameWindowView::Render @ 0x00439390`
- `GameWindowView::GetTimestamp @ 0x00439540`
- `GameWindowView::CreateGameWindow @ 0x00439730`
- `GameWindowView::InitD3DRendering @ 0x00439890`
- `GameWindowView::ResetRenderState @ 0x00439D20`
- `GameWindowView::CheckForRunningGameInstance @ 0x00439FF0`
- `GameWindowView::CalcExecutableChecksum @ 0x0043A1C0`

The source covers startup and configuration, the Win32 message/device-loss
loop, restart and teardown, window activation/close handling, frame timing,
gameplay/draw callback chains, ANM buffer servicing, presentation and snapshot
capture, window/fullscreen setup, the D3D9 HAL/software/REF fallback ladder,
view/projection/viewport setup, single-instance and launch-path handling, and
executable checksum calculation. `ResetRenderState` retains the complete target
order of 19 render-state, eight texture-stage and six sampler-state calls plus
five ANM manager cache writes.

`src/Main.hpp` provides target-bounded Win32/D3D9 views without changing the
existing exact-sensitive D3D declarations. Compile-time assertions close
`GameWindowView` at `0x58` bytes and bind its QPC/path/timestamp fields. They
also bind the Main supervisor's D3D objects, matrices, viewport and
presentation parameters, serialized configuration, gameplay and active
viewports, transition/render flags, music owner, caps, seven critical sections,
render-state cache and last-frame duration. The allocation registry is exactly
`0xA004` bytes.

`scripts/report-main-corridor.py --check` fails closed on the wrong executable
and validates all ten extents, epilogues and separating padding, the registered
window-procedure pointer, source marker order, layout assertions and all 33 D3D
state calls. It reports ten owners / 5,456 bytes.

Pinned VC7.1 SP1 build6030 compiles `src/Main.cpp` under fixed normal `/GS`, the
same profile plus `/GL`, and `/W4`. Nine owners remain diagnostic mismatches.
`GetTimestamp` is 282/282 bytes in both lanes and matches 159/162 comparable
bytes; the only differences are three instances of the stack-frame constant
`0x10` versus target `0x14`. In the selected `Render` LTCG context,
`CreateGameWindow` is also 341/341 bytes but differs in 11 of 261 comparable
bytes.

`ResetRenderState` is the strongest result: normal and selected `/GL` lanes both
produce the complete 710-byte extent, account for all 34 linkage fields and
match all 574 comparable bytes. Both diagnostics are `structural-exact` with
`acceptance_authority=none`. Physical normal-COFF versus LTCG ownership and the
original TU identity are still unresolved, so no canonical unit or exact ledger
row is added.

Repository totals become **1,309 candidates, 263 source mappings and 110 exact
functions / 12,166 bytes**. The remaining nearby Main owners are bounded leaf
or medium seams at `0x00438A30/60`, `0x004392E0`, `0x00439350`, `0x00439660`,
`0x00439700`, `0x0043A1B0`, `0x0043A290`, and `0x0043A3A0`; they remain in the
Web queue. The central Main execution skeleton is now source-present.

## Completed packet: generic typed-stack ECL VM

`EclVmContext::Run @ 0x0044E1A0` is now maintained in `src/EclVm.cpp`.
This is the generic TH10 VM below the already reconstructed Enemy-specific
`0x100-0x1B4` dispatcher, rather than another part of that high-opcode switch.
The target's 88-byte selector table contains 59 active generic opcodes and
routes 29 values to the host's virtual extension dispatcher.

The source recovers time and difficulty gating, ordinary advancement, call,
return and conditional/unconditional jumps, child-thread control, frame entry
and leave, the typed operand stack, local and host-backed int/float operands,
assignment, arithmetic, comparison, logical and bitwise operations, sine,
cosine, polar conversion, angle normalization, vector length squared and point
angle. Opcode `0x1E` retains the target-observed format scan and operand
evaluation even though the target makes no output call. Opcode `0x55` retains
the target's unusual integer `NEG` of the raw float dword before pushing it as
type `f`; it is deliberately not rewritten as ordinary floating negation.

Direct target review expands the physical owner from Ghidra's reachable body
to `0x0044E1A0-0x0044FD0B` = 7,020 bytes. Executable code ends at
`0x0044FBC1`, two alignment bytes follow, the 60-entry destination table spans
`0x0044FBC4-0x0044FCB3`, and the 88-byte selector table spans
`0x0044FCB4-0x0044FD0B`; four `CC` bytes precede independent
`0x0044FD10`. `scripts/report-ecl-vm-table.py --check` verifies the target
identity, this layout, the sole default slot, and exact target/source opcode
coverage.

The context is a target-observed `0x1024`-byte extended view: float time at
`+0x0000`, instruction at `+0x0004`, `0x1000` stack bytes at `+0x0008`, stack
top/frame base at `+0x1008/+0x100C`, thread id and host at
`+0x1010/+0x1014`, thread control and difficulty mask at
`+0x1018/+0x101C`, and flags at `+0x1020`. For the embedded Enemy context this
view overlaps the separately known host fields immediately after its former
`0x1018` prefix; dynamically spawned contexts allocate the full `0x1024`
bytes. Integer operands use negative host ids and `-1` as stack-pop; float
operands use values below zero as host ids, `-1.0f` as stack-pop, and
nonnegative flagged values as frame offsets.

Raw entry/caller/return evidence establishes a private EAX context plus one
stack float and `RET 4`. Natural source expresses the logical member method;
fixed normal VC7.1 emits a 2,760-byte COMDAT, and the `/GL` PDB contribution is
also 2,760 bytes, versus the 7,020-byte source-owned target extent. Both
diagnostics are mismatches with `acceptance_authority=none`; normal, `/GL` and
`/W4` compilation pass and no exact claim is added. Repository totals become
**1,309 candidates, 251 source mappings and 110 exact functions / 12,166
bytes**.

The adjacent `0x0044DF70` subroutine setup and
`0x0044FD10-0x004506D0` runner/operand/thread/stack helpers remain bounded leaf
seams. The next user-selected core batch is GUI `0x00414900 + 0x00415E90`,
followed by the Main corridor.

## Completed packet: Player update callback/core split

The largest confirmed authored source gap was
`PlayerUpdateCallbackBody @ 0x00425730-0x00426340`, even though its full semantic
implementation already existed under the name `PlayerUpdateCallback`. The source
mapping had been attached only to the seven-byte registered entry at `0x00426500`.
Fresh target review confirms two distinct physical boundaries: the registered ECX entry is
exactly `push ecx; call 0x00425730; ret`, while the 3,089-byte body loads one
stack Player pointer and ends in `ret 4`.

Maintained source now represents those boundaries directly.
`PlayerUpdateCallbackBody` is a `__stdcall` body in `src/Player.cpp`; the
separate `src/PlayerUpdateCallback.cpp` defines the `__fastcall` chain entry.
The core body retains the complete five-state update machine, power and death
transitions, movement/options, 32 active effect rows, highlight and update
timers, collision-derived vectors, input owner and 128-shot update.

Ghidra still proposes a two-parameter `__thiscall` signature for the body, but
the raw entry, sole incoming call, stack load and `ret 4` establish the actual
machine ABI. Its reachable-body count remains 3,060 because the reviewed
physical extent contains five alignment sleds totaling 29 bytes; its separate
five-entry state table remains at `0x00426344-0x00426357`.

Pinned VC7.1 normal compilation emits a 2,800-byte body COMDAT including its
compiler-owned table versus the target's 3,089-byte body plus separate table;
the target diagnostic remains non-exact at 139/2,693 comparable bytes with 99
relocations. Normal, `/GL`, and `/W4` compilation all succeed. The isolated
adapter naturally emits all seven target bytes and its sole REL32 resolves to
`0x00425730`, but its source-written-versus-LTCG-generated origin remains
unknown. It therefore stays a structural diagnostic without a canonical exact
claim. Repository totals become **1,309 candidates, 250 source mappings and 110
exact functions / 12,166 bytes**.

## Completed packet: Enemy runtime callback resolver

`EnemyResolveRuntimeCallback @ 0x004127A0-0x004129F0` is now maintained in
`src/EnemyCallback.cpp` with its target-observed ECX-bound fastcall ABI. Both
direct callers in `EnemyRuntimeUpdate` supply the full `0x2518`-byte Enemy
owner. The function scans eight `0x10`-byte callback records beginning at full
Enemy `+0x2494`; each record contains a life threshold, timer threshold, life
callback name and timer callback name.

The first active life record stores `life - threshold` at full `+0x2404`. When
life reaches the threshold, it clamps life, disables that record, resets the
update timer, clears runtime flag `0x10000` and returns the life callback name.
The first active timer record publishes a capped `0..99` seconds countdown at
visual owner `+0x9EC0`. At expiry it performs the same life/timer transition,
sets flag `0x10000`, subtracts 3000 from the global timeout value with a floor
of 5000, and conditionally clears bit one from eight Player state words before
returning the timer callback name.

Direct disassembly corrected the timer reset source: the target initializes
`current=0`, `previous=-999999`, `subframe=0`, installs the shared scale pointer
and flag bit zero when needed, then always finishes with `current=0`,
`subframe=0`, and `previous=-1`. Callback-record and full-runtime fields now
carry descriptive or offset-neutral names instead of the former misleading
`threshold/callbackId/state` and `ageCounter` names.

The isolated source compiles with pinned VC7.1 SP1 build6030 under fixed normal,
`/GL`, and `/W4` profiles. Its final normal fastcall COMDAT is 598 bytes versus
the target's 593 bytes and remains a diagnostic mismatch at 48/565 comparable
bytes, so no exactness is claimed. Keeping it outside `Enemy.cpp` prevents the
new control flow from renumbering VC7 compiler-local switch labels; a cold
replay still reproduces all **20 existing Enemy units / 511 bytes** exactly.
Repository totals become **1,309 candidates, 249 source mappings and 110 exact
functions / 12,166 bytes**.

The adjacent `0x00412720` callback-record setter and the Ghidra-missed retained
bodies at `0x00412750/0x00412790` remain in the Web leaf queue. The next useful
core frontier should stay inside Enemy/ECL or move to another high-fanout core
owner instead of consuming those leaves.

## Completed packet: complete Enemy ECL dispatcher

`EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770` now has a complete
maintained body in `src/EnemyEclDispatcher.cpp`. Direct decoding of the
hash-attested target's 181-byte selector table and 108-entry destination table
finds 124 active opcode values and 57 values routed to the common default. The
maintained enum has the same 124 unique values and the switch has exactly one
case label for every enum member. The full family spans enemy creation and ANM,
motion/interpolation, hitboxes and lifecycle flags, items, dialog/spells,
rank/difficulty selection, bullet patterns and lasers.

The source preserves the separately reviewed physical owner
`0x0040E770-0x00411FBF` = 14,416 bytes: executable code through
`0x00411D2E`, alignment, the two top-level tables and both five-entry nested
difficulty tables. The 108 number describes jump destinations rather than
semantic opcode coverage. `scripts/report-ecl-dispatch-table.py --check`
replays this distinction directly against the canonical target and fails if a
target-active opcode is missing, a source opcode reaches the target default, or
enum/case values are duplicated.

Decompiler stack-overlap artifacts were removed from the source representation.
Enemy creation uses the existing typed 0x40-byte `EnemySpawnRequestView`; laser
instructions use a 0x1F8 union-backed request scratch that contains both the
0x1DC curved-laser prefix and the complete straight-laser request. This avoids
array overruns and preserves the target's packed short and float bit fields.
Opcode and helper identifiers remain descriptive where original symbols cannot
be recovered, and the six private argument helpers remain declaration-only leaf
seams for the Web queue.

Pinned VC7.1 SP1 build6030 compiles the complete dispatcher under both fixed
normal `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc` and the same profile plus
`/GL`. No target-byte or bounded linked-image comparison is accepted for this
LTCG-sized owner, so exactness remains unknown. Repository totals are **1,309
candidates, 248 source mappings and 110 exact functions / 12,166 bytes**.

The first `Enemy.cpp` canonical replay failed closed because the six new member
declarations advanced the compiler's internal resolver `$L...` labels by
exactly 14. Object inspection found the same five DIR32 offsets and target VAs
in each resolver; only the generated names changed. After refreshing those ten
manifest symbol names, two independent cold builds replay all **20 existing
Enemy units / 511 bytes** exactly.

## Completed packet: ANM resource, text and surface core

The manager setup corridor is now source-present from `0x00445900` through the
end of `CaptureToSurface @ 0x004485C8`. Its texture/resource half covers direct
and sub-rectangle texture loads, texture creation from files and ANM payloads,
empty-texture creation, synchronous/preloaded ANM parsing, external texture
data, postload relocation, texture upload, preload service, resource release,
and sprite materialization. The target manager owns 33 loaded-resource views at
`+0x3AD06C`; each complete view is `0x130` bytes and retains the file buffer,
entry pointers, 32 script pointers and 32 sprite pointers.

The text half recovers the shared rasterizer and left/right/centered formatters.
VM byte fields `+0x3A0/+0x3A1` are glyph width and height, while flag bit one at
`+0x360` selects the alternate text helper. The formatter family uses guarded
stack buffers and therefore requires the target's `/GS` profile. The same fact
also closes three previously authored ASCII-manager formatters.

The surface half covers load/preload/release, two surface-to-backbuffer copies,
texture capture, rectangle copy and surface capture. Manager storage consists
of two 32-pointer surface arrays, 32 retained file buffers and sizes, and 32
`0x1C` image-info records at `+0x3AD4E0..+0x3ADA5F`. Direct D3D9 vtable slots and
target branches establish render-target acquisition, offscreen backup creation,
filter selection and cleanup. The apparent null write in the preload service
and the redundant second `surfaceData[index] = NULL` in release are retained
because both occur in the target.

Canonical linked replay adds five resource units (540 bytes) and eight `/GS`
text/surface units (1,032 bytes). The latter include three ASCII formatters,
four ANM text functions and `ReleaseSurface`. Two focused cold replays pass each
new group independently; two full-source cold replays pass all **60
`AnmManager.cpp` units / 10,175 bytes** across ten artifacts. Repository totals
are **1,309 candidates, 247 source mappings and 110 exact functions / 12,166
bytes**. Four Ghidra-missed authored candidates were added at `0x00447080`,
`0x00447C80`, `0x00447EC0` and `0x00448360`.

The tracked core corridor contains 31/31 source-present functions and 10,628 /
10,628 source-present bytes. Across the full `AnmManager` module, 123/125
candidates are source-present (98.4% by function count); only the 10-byte
`0x004452E0` and 7-byte `0x004458A0` leaf bodies remain unmapped. Small retained
helpers and the `0x004485D0-0x00448850` wrapper/id-propagation family remain in
the Web leaf queue. The next core frontier is
`EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770`, the 14,416-byte ECL
dispatcher.

## Completed packet: ANM manager update and removal core

The former `th10-ghidra` discovery refusal was traced to the intentional
implementation-binding guard. Commit `f5a0c4f` added pinned Capstone identity to
the registered `config/tools.lock.toml`, while the ignored private Factory
configuration retained the previous aggregate hash. Per-file history proved
that this was the sole registered-file delta. The private binding was refreshed
to aggregate `406ab2a25af49916607390d2d2eaad09d31fe3df6291b93b07919d162926679d`;
Factory validator, live MCP discovery and live MCP `check {}` then passed against
the canonical target with `provider_transport=factory-native-command`.

Maintained source now covers the four central manager-update bodies.
`UpdatePrimaryVms @ 0x00448860` rebuilds ordinary layers 0–18 while updating the
primary manager list. `UpdateSecondaryVms @ 0x00448900` resets the shared
processed count and rebuilds reserved layer 19. Both save the next manager node
before callbacks, remove marked or completed VMs, run the optional position
callback before `ExecuteScript`, append survivors through `nextInDrawLayer`, and
count each visited VM. `DrawLayer @ 0x00448980` skips removal-marked VMs, invokes
the optional draw callback and reaches the common draw dispatcher.

`RemoveVm @ 0x00448BB0` is the shared 268-byte lifecycle owner. It repairs
either manager-list head/tail pair, unlinks both the manager and parent/child
nodes, and distinguishes the 4096-entry inline pool from heap fallbacks. Pool
VMs clear their occupancy byte, release generated storage and run `Initialize`
in place; heap VMs are destroyed and deleted. Direct-member expressions for the
four head/tail transfers reproduce the target's VM-relative encodings without
artificial constraints.

All four functions are canonical exact under pinned VC7.1 SP1 build6030 `/GL`
in the complete ANM executor context: primary update 149 bytes, secondary update
123, layer draw 67 and removal 268. Two independent final cold replays cover
**607/607 bytes** and all eleven declared linkage fields with zero differences.
The full `AnmManager.cpp` canonical set is now **47 functions / 8,603 bytes**;
repository tracking contains **1,305** candidates, **217** source mappings and
**97 exact functions / 10,594 bytes**.

The 206-byte manager destructor at `0x00446220` is also source-present. It drains
both lists through `RemoveVm`; target SEH states establish the subsequent member
order as twenty sentinel VM destructors, the embedded primary VM whose
generated-vertex pointer is at `+0x3AD488`, then 4096 pool VM destructors. Its current 197-byte linked candidate remains
non-exact because the target retains a different SEH/vector-destructor context.

Small update/draw wrappers and the Ghidra-missed VM scalar deleting helper are
left for the Web leaf-function queue. The next core frontier is the 2,315-byte
manager constructor at `0x00445900`, followed by the 472-byte initialization
owner at `0x004462F0` and other large bodies through the manager setup corridor.

## Completed packet: ANM child-VM and manager lifecycle

The ANM executor's former child-VM declarations are now connected to maintained
source. Twelve creator bodies cover the four manager-order variants and their
screen/world-positioned forms. Each obtains a VM from the manager's 4096-entry
inline pool or heap fallback, initializes the selected script, applies a render
layer or position, and inserts the VM at the front or back of the primary or
secondary manager list. World-positioned forms apply the TH10-local playfield
origin `(+224,+16)`; no adjacent-game offset was transferred.

The manager view now exposes the target-proved pool at `+0x68`, occupancy bytes
at `+0x3AC068`, list heads/tails at `+0x72DAD4..+0x72DAE0`, twenty draw-layer
sentinel VMs at `+0x72DAE4`, and the next VM id at `+0x732454`. Maintained bodies
cover allocation, all four insertion variants, two-list id lookup, stale-id
clearing, interrupt and immediate-interrupt propagation, tree deletion,
screen/world position propagation, position lookup, and resource-wide deletion.
The VM's `+0x04` manager node and `+0x10` parent/child layer node are distinct;
root operations walk only the latter child chain.

Three script-transition bodies close the resource side.
`SetAndExecuteScriptIndex @ 0x0043E7E0` binds and executes frame zero without a
full VM initializer, while `InitializeAndExecuteScriptIndex @ 0x0043E710`
initializes a newly allocated VM first. `SetAndExecuteScriptIdx @ 0x0043E8B0`
restarts an existing VM, rebuilding color, timer and interpolation state and
undoing a pending horizontal flip by
negating scale X. The restart body has the exact 287-byte target extent and only
22 ordinary differing bytes, all caused by an ESI/EDI register exchange; it is
retained as a strong near match without exact credit.

Pinned VC7.1 `/GL` in the complete executor context canonically reproduces three
new units: `SetAndExecuteScriptIndex` (194 bytes), primary-tail insertion (118),
and secondary-tail insertion (118). Two final independent cold replays rebuild
all **43 AnmManager.cpp units / 7,996 bytes** across eight artifacts with zero
differences. Current repository tracking contains **1,305** candidates, **240**
authored functions, **212** source mappings, and **93 canonical exact functions /
9,987 bytes**. The authored source-present exact backlog is **115**.

The remaining lifecycle bodies are source-present and non-exact. Representative
candidate/target extents are 197/203 for initialize-and-bind, 201/217 for
`InitializeVm`, 153/161 for allocation, 65/73 for two-list lookup, 19/21 for
stale-id lookup, and 74/73, 93/95, 116/118 for the three creator shapes. The two
front insertion variants are 94/112 because the current complete-program
context eliminates a generic list branch that the target retained. These gaps
are compiler/caller-context diagnostics and have no acceptance authority.

`scripts/progress.py` now keys cross-ledger origin lookups by parsed integer
addresses. This fixes a real report failure when equally valid uppercase and
lowercase hex spellings occur in different CSV files, while tracking validation
already treats addresses numerically.

At that checkpoint, a later provider discovery attempt refused operation because
the native Ghidra implementation no longer matched its operator binding. The
current manager-update packet reviewed the registered-file delta, refreshed the
private binding and re-attested the provider before making new Ghidra reads.

The 268-byte VM removal frontier is completed by the current packet. Remaining
id/child wrappers stay in the leaf queue, while core work proceeds to the large
manager constructor and initialization owners.

## Completed packet: ANM radial trail and RNG

The pulsing radial-trail family at `0x004452F0-0x00445898` is now
source-present. Its 0x4B0-byte VM-owned payload contains 33 textured vertices,
33 radii, 33 radial velocities, a two-float UV velocity and a trailing dword.
The initializer frees/replaces VM `generatedVertices`, installs callbacks at
VM `+0x398/+0x39C`, seeds a center plus 31 radial vertices, and uses RNG owner
`0x004918B0`. The updater scrolls both U and V with the X velocity, applies the
target's 33-vertex wrap loops, advances radii, rebuilds the ring with the
angle/magnitude helper, clears ring alpha and copies vertex 1 into closing
vertex 32. The draw callback submits the 33-vertex textured fan.

`UpdatePulsingRadialTrail` has the exact 598-byte target extent and matches
524/526 comparable bytes under normal and linked compiler probes; only two
commutative X/Y position-sum displacement bytes differ. In the single-source
LTCG diagnostic the initializer was only 572 bytes because unresolved RNG
methods could not be inlined. New multi-source `/GL` support lets the probe add
`src/RandomMath.cpp`; the initializer then becomes 804 bytes versus target 803,
but remains non-exact because its real caller is the source-absent 9,587-byte
ANM script executor at `0x0043EE30`, which supplies the target's private EDI VM
receiver.

Two trail units are canonical exact in that multi-source context:
`DrawPulsingRadialTrail` (25 bytes and two linked fields) and
`AnmFloat3View::FromAngleMagnitude` (30 relocation-free bytes). The recovered
RNG recurrence independently produces four normal-COFF exact units:
`GetRandomU16` (39), `GetRandomU32` (78), `GetRandomF32` (109), and
`GetRandomF32Signed` (115), totaling 341 bytes and five constant relocations.
Boundary review also added Ghidra-missed, unreferenced bodies at `0x004452E0`,
`0x004458A0`, `0x0044B9B0`, and `0x0044BA30`; only `0x0044B9B0` has sufficient
source/compiler evidence for authored exact promotion.

Current tracking contains **1,298** candidates, **190** authored functions,
**162** source mappings, and **79 canonical exact functions / 7,896 bytes**.
The authored source backlog is **79**. The full `src/AnmManager.cpp` canonical
set is **29 functions / 5,905 bytes** across six artifact contexts; its cold
replay passes with the trail artifact compiling both ANM and RNG inputs.

The next high-value ANM frontier is the script executor at `0x0043EE30`, which
owns the private trail-initializer call context, or the following generated
effect families beginning at `0x00445900`.

## Completed packet: ANM generated geometry

The full `0x00444B10-0x004451B2` generated-geometry corridor is now
source-present as eight distinct functions. Horizontal and vertical strip
initializers fill alternating UV rows or columns with the VM primary color and
unit RHW; the adjacent color helper updates a caller-supplied 0x1C-stride
range. Dispatcher mode 9 reaches the visible/alpha-gated textured strip
submitter with `generatedVertices` and twice `generatedVertexCount`.

The corridor also contains a batched supplied-quad path, two 0x14-stride
untextured diffuse submitters for triangle strips and fans, and a textured fan
submitter used by the 33-vertex callback at `0x00445880`. The untextured paths
select diffuse-only texture operations, disable Z writes for submission, then
invalidate the renderer's shader/color/blend/Z caches and restore normal
modulate/texture arguments. D3D9 vtable slots and all state/FVF/primitive
values are represented explicitly in the target-bound device view.

Ghidra originally omitted the independently CC-delimited bodies at
`0x00444CB0`, `0x00444DC0`, `0x00444E60`, and `0x00444FA0`; all four are now
included in the function denominator. Ghidra and raw scans find target callers
only for the dispatcher strip path and the 33-vertex textured fan. Names for
the retained no-xref bodies are descriptive or adjacent-supported rather than
original-symbol claims.

Three units are canonical exact after two cold linked-image replays:
`InitializeHorizontalTextureStrip` (201 bytes),
`InitializeVerticalTextureStrip` (201), and `SetGeneratedVertexColor` (34).
They add 436 exact bytes and have no linked fields. `DrawGeneratedVertices`
and `DrawTexturedTriangleFan` reproduce their complete 224/211-byte extents
apart from the same 13-byte texture-bind scheduling block seen in Draw3D.
`QueueSpriteQuad` and both 313-byte untextured submitters reproduce every
instruction and linked field, while the candidate PDB contributions own two
extra trailing `CC` bytes; strict extent policy keeps these five non-exact.

At checkpoint `09e4da8`, tracking contained 1,293 candidates, 154 source
mappings and 73 canonical exact functions / 7,500 bytes. The next frontier at
that checkpoint was the generated radial-trail family now covered above.

## Completed packet: ANM direct 3D

`SetRenderStateForVm3D @ 0x004423E0-0x00442597` is reconstructed from
TH10-local target evidence. It is the direct-3D sibling of the batched 2D
state helper: blend changes select inverse-source-alpha for mode zero and one
for modes one and two; the selected primary/secondary VM color is optionally
multiplied by the renderer color and published as D3D9 texture factor; and the
same point/linear sampler cache is maintained. Every incompatible state change
flushes pending 2D sprites, and the helper increments the per-frame state count.

`Draw3D @ 0x00444760-0x00444B00` rejects hidden, disabled and zero-alpha VMs,
flushes the 2D batch, updates scale and X/Y/Z rotations, and applies anchor
translation from `spriteOffset + preservedPosition + position`. It installs
the world transform, binds the sprite texture, optionally refreshes the
texture transform from UV scroll, and submits the renderer's four 0x14-byte
vertices as a two-primitive triangle strip. TH10 binds the texture before
refreshing the sprite/UV cache, repeats the `uvScrollX != 0` test in the
texture-transform condition, and writes world Z first without then with
`spriteOffset.z`; source retains those target-observed details rather than
normalizing them from adjacent code.

Pinned VC7.1 `/GL` reproduces both complete target contribution extents: 440
bytes for the state helper and 929 for the draw owner. The state helper matches
396/408 comparable non-linkage bytes and Draw3D matches 816/829. Each remaining
gap is one equal-length instruction-scheduling region around a D3D9 COM call:
12 bytes for texture-factor state and 13 bytes for texture binding. Natural
assignment-expression, explicit-local and inline member-wrapper probes do not
change those regions. They remain source-present and non-exact; no artificial
dependency or assembly constraint was added.

Current tracking contains **1,289** candidates, **174** authored functions,
**146** source mappings, and **70 canonical exact functions / 7,064 bytes**.
The authored source backlog is **72**. The full `src/AnmManager.cpp` canonical
set remains **24 functions / 5,414 bytes** across five artifact contexts.

The next ANM frontier is the generated-vertex initialization/submission
corridor at `0x00444B10`, `0x00444BE0`, `0x00444CE0`, and `0x004450E0`.

## Completed packet: ANM projected photo blend

`DrawMode7 @ 0x004445C0-0x00444751` is reconstructed from TH10-local target
evidence. After `Project3DQuad`, it transforms the four renderer-local
0x14-byte source vertices beginning at renderer `+0x3ADA78` through cached
world matrix `+0x3AD0F0`. Each resulting XYZ is measured independently from
camera global `0x00491D7C`.

Within the near distance at `0x00491E78`, a vertex keeps the VM's selected
primary or secondary color. Across the near/far interval it interpolates B/G/R
toward float channels at `0x00491E80/84/88` and preserves VM alpha. At or
beyond the far endpoint, it takes dword color `0x00491E90` and then restores VM
alpha. Unlike mode 6, this path neither applies renderer mix color nor rejects
a fully blended quad. It submits through `DrawInner(vm, 2)` and restores all
four shared RHW fields to one.

The dispatcher pushes VM then renderer for mode 7; the callee owns both stack
arguments and returns with `RET 8`. Its source view now names the four source
vertices and extends the photo-blend state through the far color, with compile-
time offset and size checks. TH095 supplied a source-family hypothesis only;
all accepted offsets, arithmetic and ABI facts are TH10-local.

Pinned VC7.1 `/GL` in the real two-VM caller context produces a 404-byte PDB
contribution versus the 402-byte target. Refining the initial 479-byte adjacent-
shaped candidate to TH10 float-channel arithmetic, pointer induction and a
12-byte live color/range/conversion aggregate closes the stack frame exactly at
`0x4C` and aligns the full transform/distance/control-flow core. The remaining
two-byte extent difference comes from the candidate retaining the shared-color
base with positive channel offsets and an adjusted loop comparison, while the
target retains an alpha pointer and offsets `-3..0`. It remains source-present
and non-exact without artificial codegen constraints.

Current tracking contains **1,289** candidates, **172** authored functions,
**144** source mappings, and **70 canonical exact functions / 7,064 bytes**.
The authored source backlog is **70**. The full `src/AnmManager.cpp` canonical
set remains **24 functions / 5,414 bytes** across five artifact contexts.

The next ANM frontier is `Draw3D @ 0x00444760`, followed by the generated-
vertex initialization/submission corridor at `0x00444B10-0x004450E0`.

## Completed packet: ANM mode dispatch

`DrawMode6 @ 0x00443FB0-0x00444232` is now reconstructed from TH10-local
evidence. It first uses the camera-facing projector, computes the distance from
`position + preservedPosition + spriteOffset` to background-camera global
`0x00491D7C`, and optionally applies the renderer color multiplier. Beyond the
near distance at `0x00491E78`, it interpolates color toward float B/G/R globals
at `0x00491E80/84/88` and alpha toward zero across the configured near/far
interval. A completed fade returns `-1`; otherwise all four shared vertices
receive the computed color and `DrawInner(vm, 2)` submits the quad.

The total `Draw @ 0x004451C0` owner is also source-present. Direct target and
PDB review correct its physical extent from Ghidra's 173-byte code-only body to
216 bytes through `0x00445297`: code ends at `0x0044526C`, a three-byte
alignment instruction follows, and `0x00445270-0x00445297` is the owned
ten-entry absolute jump table. It checks visible, draw-enabled and primary-alpha
state separately, extracts render mode from VM flag bits 22-25, and dispatches
modes 0-9 across the recovered draw family. Mode 9 exposes VM `+0x30C` as a
generated-vertex count and passes twice that count with the VM vertex pointer.

A real target caller at `0x00409230` draws two adjacent 0x3AC-byte VMs through
the global renderer when owner flag bit zero is set. Using this callback as the
pinned VC7.1 `/GL` entry reproduces both its complete 50-byte contribution and
the private 60-byte `DrawProjected3DQuad` contribution exactly, including all
ten declared linked fields. `DrawMode6` remains a close natural-source
candidate at 645 bytes versus target 643: its EDI/ESI private ABI is correct,
but the compiler candidate uses a `0x34` stack frame where the target uses
`0x20`. `Draw` remains non-exact at 208 versus the 216-byte physical owner
until its unrecovered mode-7, direct-3D, and generated-vertex callees are
restored in the same LTCG context.

This packet also fixes the linked-image Oracle for code contributions with
inline absolute tables. PE base relocations now override only overlapping
Capstone-derived false relative fields, while every other overlap still fails
closed. This lets switch-heavy ANM and ECL contributions be probed without
misclassifying jump-table data as code.

Current tracking contains **1,289** candidates, **171** authored functions,
**143** source mappings, and **70 canonical exact functions / 7,064 bytes**.
The authored source backlog is **69**. The full current `src/AnmManager.cpp`
exact set is **24 functions / 5,414 bytes** across five artifact contexts.

The next ANM frontier is mode 7 at `0x004445C0`, followed by the direct-3D and
generated-vertex owners reached by dispatcher modes 8 and 9.

## Completed packet: ANM rotated draw modes

Direct target boundary review splits the former `0x004436C0-0x00443B5E`
candidate at the `CC` byte at `0x0044390F`. The two resulting 591-byte bodies
are `0x004436C0-0x0044390E` and `0x00443910-0x00443B5E`. Ghidra xrefs and raw
control flow prove that `Draw @ 0x004451C0` render-mode cases 1 and 3 tail-jump
to the two entries at `0x00445203` and `0x00445263`. The dispatcher extracts
the four-bit mode from VM flags 22-25 after checking visible, draw-enabled and
primary-alpha state.

Both bodies are instruction-identical apart from their two address-dependent
REL32 displacements. Each handles zero Z rotation through
`DrawNoRotationNoRound`, uses one x87 `FSINCOS`, accumulates all three VM
position vectors, scales sprite width/height, applies the anchor modes in flag
bits 18-21, writes four rotated XY pairs plus common accumulated Z, and enters
`DrawInner(vm, 0)`. Maintained source therefore supplies `Draw2D` for mode 1
and a distinct `Draw2DRotatedOrAxisAligned` method for mode 3. The latter name
is corroborated by TH08's same mode table and remains a descriptive hypothesis;
all behavior, boundaries, ABIs and duplicate-body facts come from TH10.

Pinned VC7.1 SP1 build6030 `/GL` produces complete 591-byte PDB contributions
for both methods. The initial form matched 497/507 comparable bytes; its only
ten non-linkage differences were one equal-length scheduling choice at offsets
`+0x54..+0x5E`, where the target completes the horizontal anchor mask before
loading the first Y-offset operand. A later exact-source pass recovered the
original expression family: keeping each three-term coordinate sum as one
left-associated expression reproduces all 507 comparable bytes and all 21
fields in both owners. The canonical promotions are recorded in the current
exact Draw2D packet.

Current tracking contains **1,289** candidates, **166** authored functions,
**140** source mappings, and **68 canonical exact functions / 6,954 bytes**.
The authored source backlog is **68**. The full current `src/AnmManager.cpp`
exact set is **22 functions / 5,304 bytes** across four artifact contexts.

The photo-color owner and real dispatcher context identified here are completed
by the current packet above. Mode 7 and the direct-3D/generated-vertex paths
remain the open continuation.

## Completed packet: ANM projected 3D quad

`Project3DQuad @ 0x00444240-0x00444571` is recovered from TH10-local target
evidence. It establishes VM matrices at `+0x23C/+0x27C`, flag bit 14 as the
matrix-recalculation suppressor, and the renderer's cached world matrix at
`+0x3AD0F0`. Dirty transforms copy the base matrix, apply scale to `_11/_22`,
then compose nonzero X/Y/Z rotations through D3DX. Anchor bits 18-21 select
the local `-256/-128/0/128/256` corner coordinates before four projections
through the active viewport owner.

TH10 adds all three VM position vectors to existing world X/Y translation but
replaces world Z with their sum. This differs from the first adjacent-game
source-shape hypothesis and accounts for the only four bytes in the initial
822-byte candidate beyond the 818-byte target. With the corrected assignment,
the canonical `/GL` unit reproduces the complete PDB contribution and all 21
link fields exactly.

`DrawProjected3DQuad @ 0x00444580-0x004445BB` is source-present. It projects,
calls `DrawInner(vm, 0)`, restores the four shared RHW values to one, and
returns the draw result. Its standalone compiler entry emitted a conventional
72-byte wrapper. The subsequently recovered real two-VM draw callback supplies
the private ESI/EDI context and reproduces the complete 60-byte target form;
the current packet promotes it to canonical exact.

## Completed packet: ANM camera-facing projection

The camera-facing placement owner at `0x00443B60-0x00443F76` is now recovered
from TH10-local target evidence. It reads VM Z rotation at `+0x2C`, accumulates
`position + preservedPosition + spriteOffset`, builds a translated identity
world matrix, and calls the imported `D3DXVec3Project` thunk at `0x0045218E`
twice through the active viewport owner at `0x00491FAC`. The owner layout is
camera-right `+0x30`, view matrix `+0x4C`, projection matrix `+0x8C`, and D3D9
viewport `+0xCC`; all four offsets have compile-time checks.

The first projection supplies a zero vector and rejects projected Z outside
`[0,1]`. The second projects camera-right, and the screen-space distance between
the results supplies half-scale for sprite width/height. The same VM flag bits
18-21 select horizontal and vertical anchor modes before the two-dimensional
rotation is applied. TH10 writes `projectedPosition.z` to all four shared
vertices; this target-local behavior differs from the adjacent TH095 source
shape and was retained from TH10 evidence.

Pinned VC7.1 SP1 build6030 `/GL` in the regular-text entry context reproduces
the complete 1,047-byte PDB contribution and all 25 viewport, import, constant,
and shared-quad fields exactly. The 33-byte `DrawCameraFacingQuad` wrapper is
also exact with both REL32 calls replayed to `ProjectCameraFacingQuad` and
`DrawInner`. The compiler's local symbol ordering affects LTCG stack coloring:
renaming the two 12-byte projection temporaries alone produced eight stack-
displacement differences, while the retained source form reproduces all 1,047
bytes. This is compiler evidence, not an inferred target layout.

Direct target review also recovered a Ghidra-missed 50-byte
`TranslateRotation` body at `0x00443680-0x004436B1`. Its 50 instruction bytes
are raw-equal to current source, but the candidate PDB contribution includes
two trailing `CC` bytes and is therefore conservatively source-present/non-
exact under the current complete-contribution Oracle. The following rotated
draw range was subsequently split and reconstructed in the current packet.

## Completed packet: ANM shared draw core

The common renderer path at `0x00442670` is now reconstructed together with
its 25-byte color mixer at `0x004423C0`, 201-byte render-state owner at
`0x004425A0`, and axis-aligned callers at `0x00443080/0x00443290`. Direct TH10
evidence establishes screen shake at renderer `+0x5C/+0x60`, the texture/blend/
shader/filter cache at `+0x3ADA64..+0x3ADA6E`, mix color at `+0x732458`, VM UV
scroll at `+0x54/+0x58`, primary/secondary colors at `+0x2FC/+0x300`, and
sprite texture/UV/dimensions at `+0x04/+0x20..+0x34`. Every named layout field
has a compile-time offset check or belongs to an already size-checked view.

`DrawInner` adds screen shake to four shared vertices, optionally performs the
D3D9 half-pixel `FRNDINT - 0.5f` sequence, assigns sprite UVs, computes four
bounds, culls against the unsigned active viewport at global `0x00491FAC`,
flushes on texture/shader changes, applies primary or secondary VM color and
optional per-channel renderer mixing, updates render state, and expands the
quad into the packed buffer. `SetRenderStateForVm` handles destination blend
and point/linear min/mag filtering through D3D9 vtable slots `+0xE4/+0x114`;
the texture change uses `SetTexture` at `+0x104`.

Both axis-aligned draw owners implement the three horizontal and vertical
anchor modes from VM flag bits 18-21. The rounded path floors centered axes and
enters `DrawInner(vm, 1)`; the no-round path preserves sub-pixel coordinates
and enters with zero. TH10 has one target-local quirk: no-round writes vertex Z
from `spriteOffset.y (+0x350) + preservedPosition.z + position.z`, while the
rounded sibling reads `spriteOffset.z (+0x354)`. Maintained source preserves
the observed difference rather than normalizing it from adjacent games.

Pinned VC7.1 `/GL` in the real regular-text draw entry context reproduces all
five complete PDB contributions exactly: color mix 25 bytes/no fields, state
owner 201 bytes/six fields, `DrawInner` 1,111 bytes/91 fields, rounded axis draw
515 bytes/29 fields, and no-round axis draw 486 bytes/29 fields. Two cold
canonical passes replay all **2,338 bytes and 155 linked fields** with zero
differences. The full current `src/AnmManager.cpp` exact set is **19 functions /
3,406 bytes** across four artifact contexts.

Current tracking contains **1,286** candidates, **161** authored functions,
**133** source mappings, and **65 canonical exact functions / 5,056 bytes**.
The authored source backlog is **64**. This packet adds five source mappings
and five exact functions without adding speculative denominator entries.

The next ANM frontier is the neighboring transformed-quad corridor beginning at
`0x00443B60`; keep its rotation/projection semantics provisional until its own
TH10 matrix, caller, and compiler evidence is complete.

## Completed packet: ANM render buffer core

The actual ANM renderer reached through pointer `0x00491C10` owns a shared
`0x20000`-entry vertex buffer. TH10 directly establishes its `0x1C`-byte packed
vertex size, sprite count `+0x3ADAC8`, buffer base `+0x3ADACC`, end/start cursors
`+0x72DACC/+0x72DAD0`, and flush count `+0x58`. The complete buffer span is
therefore the observed `0x380000` bytes rather than an adjacent-game layout.

Maintained source covers the 29-byte clear owner at `0x00442F30`, 133-byte
flush at `0x00442F50`, and 157-byte quad-to-triangle expansion at `0x00442FE0`.
The clear resets count and cursors; the add path copies corners `0/1/2/1/2/3`,
advances by six vertices, and increments the count; flush configures D3D9
diffuse arguments and FVF, submits two triangle-list primitives per sprite,
advances the submitted range, clears the count, and increments the frame tally.

All three are canonical exact linked-PE units in the real regular-text draw
context: clear and add are complete raw-equal PDB contributions with no linked
fields; flush contains exactly four DIR32 fields, all replayed to D3D9 device
global `0x00491C30`. Two independent cold builds reproduce all **319 bytes**.
The pinned VC7.1 SDK lacks D3D9 headers, so `src/D3d9View.hpp` supplies the
minimal target-used COM prefix with compile-time checks for vtable slots
`+0x10C`, `+0x14C`, and `+0x164`.

Current tracking contains **1,286** candidates, **156** authored functions,
**128** source mappings, and **60 canonical exact functions / 2,718 bytes**.
The authored source backlog remains **64**. The current `AnmManager.cpp` source
replays fourteen exact units across **1,068 bytes** in four artifact contexts.

The next draw-core seam is the paired axis-aligned placement owners at
`0x00443080` and `0x00443290`. Their existing TH10 evidence already establishes
VM position/offset/scale fields and the horizontal/vertical anchor bits; their
final call reaches the common render-state/draw owner at `0x00442670`.

## Completed packet: ASCII text pipeline and ANM-owner correction

TH10-local queue append, formatting, glyph lookup, viewport, callback, and draw
evidence corrects the preceding checkpoint's owner hypothesis. The `0x89AC`-
byte polymorphic object at global `0x004776E0` is the ASCII text manager. It
contains 256 regular and 64 GUI records of `0x68` bytes, queue counts at
`+0x896C/+0x8970`, text color/scale/viewport/shadow/spacing state, an ASCII ANM
resource at `+0x8994`, and two embedded `0x3AC` ANM VMs. The true ANM renderer is
a distinct object published through the pointer at `0x00491C10`.

Maintained source now expresses the complete regular and GUI record layouts,
queue reset, both append paths, three variadic formatters, and the two large text
renderers at `0x00401760` and `0x00401A50`. The renderers cover both `0x62`-glyph
banks, `0x44`-byte sprite records, line and space advance, regular-text shadow,
viewport transitions, vertex-buffer flushes, and rounded versus non-rounded ANM
draw calls. Each named field and helper target is directly visible in TH10. The
TH095 ASCII implementation supplied only a source-shape hypothesis.

The retained reset at `0x004014D0`, regular append at `0x00401530`, and GUI
append at `0x004015C0` are canonical exact linked-PE units across **267 bytes**.
They are complete, raw-equal PDB-owned contributions with no linked fields in
the real `AsciiManagerCreate` LTCG context. Together with the four renamed exact
units from `260ac75`, the current source file replays eleven exact units across
**749 bytes**. The three formatters and two renderers remain source-present
non-exact work; no semantic recovery claim has been promoted to exactness.

Current tracking contains **1,285** candidates, **153** authored functions,
**125** source mappings, and **57 canonical exact functions / 2,399 bytes**.
The authored source backlog is **64**. Relative to `260ac75`, this packet adds
eight authored functions, eight source mappings, three denominator candidates,
and three exact functions / 267 bytes while correcting the manager owner across
source, ledgers, exact manifests, and documentation.

The following renderer-buffer packet takes up the real owner at `0x00491C10`
and closes its clear, flush, and quad-expansion seam before the neighboring draw
helpers at `0x00443080` and `0x00443290`.

## Completed packet: ANM VM lifecycle

Direct TH10 constructor, destructor and reset evidence establishes a `0x3AC`-
byte ANM VM. Its constructor runs nine `0x14` timer-like member constructors,
clears exactly `0xEB` dwords, and marks the active sprite index at `+0x384` as
invalid. Its destructor owns and clears the optional generated-vertex pointer at
`+0x358`. The reusable reset at `0x00401DE0` preserves `+0x20` and
`+0x340..+0x348`, clears the VM, restores list sentinels, scale, matrix, timer,
flags and seven observed state fields, and writes `-1` at `+0x2FC`.

Maintained `AnmVmView`, `AnmVmTimerView`, and `AnmMatrixView` source expresses
these facts with compile-time size and offset checks. Denominator review also
adds two CC-delimited helpers missed by Ghidra: the 55-byte matrix identity body
at `0x00401F00` and the 15-byte timer initializer at `0x00401F40`. Their logic
is independently visible inline in the reusable reset.

Two cold canonical passes reproduce four complete functions across **192
bytes**: the 34-byte destructor and 88-byte constructor as normal COFF, and the
55-byte matrix and 15-byte timer helpers as raw-equal linked-PE PDB
contributions with no linked fields. The 273-byte reset compiles to the same
extent under normal COFF but remains non-exact because the bounded harnesses do
not recover the target's private EDX receiver and register/save ordering. In the
new explicit manager-factory link context it is 271 bytes.
The emitted seven-byte timer constructor shape also occurs at two target
locations already associated with other or unresolved owners; it receives no
ANM mapping or exactness credit.

Current tracking contains **1,279** candidates, **137** authored functions,
**109** source mappings, and **50 canonical exact functions / 1,842 bytes**.
The authored source backlog is **55**. Relative to the preceding checkpoint,
this packet adds five authored functions / 465 bytes, five source mappings, two
denominator candidates, and four exact functions / 192 bytes.

## Completed packet: ECL host hierarchy

Direct TH10 evidence closes the previously unresolved class boundary. The
six-slot vtable at `0x0046D0D8` contains neutral dispatch/int/int-pointer/float/
float-pointer methods followed by scalar deleting destructor `0x0040C7B0`.
The derived table at `0x0046D0C0` replaces those five methods and ends in the
Enemy deleting destructor at `0x0040CC50`. Construction at `0x0040C710` and
`0x0040D830`, destruction at `0x0040DAE0`, and the inline base tail through
`0x0040DC5F` independently show a `0x103C` polymorphic ECL host followed by the
Enemy runtime tail. The manager's indirect call through slot `+0x14` with flag
one is virtual deletion.

Maintained source now expresses that evidence as an actual
`EnemyEclHostBaseView` base and `EnemyFullObjectView` derived class, including
real constructors/destructors, `new`, virtual `delete`, and a base-typed operand
resolver in the embedded context. Compile-time layout assertions preserve base
size `0x103C`, full size `0x2518`, active context `+0x004`, embedded context
`+0x008`, allocation list `+0x1034`, and runtime tail `+0x103C`.

Five neutral virtuals are canonical exact normal-COFF units across 27 bytes.
The 21-byte base constructor and 47-byte base destructor are canonical exact
linked-PE units; their complete PDB-owned extents declare the base-vtable field,
the constructor's one DIR32 field, and the destructor's two `free` REL32 fields.
Two independent cold builds replay all seven new units across **95 bytes**.
All 20 Enemy exact units replay together across **511 bytes** after refreshing
only VC7 compiler-local resolver label names. Base reset and the large derived
constructor/destructor remain non-exact.

Repository exact coverage is now **46 functions / 1,650 bytes**. This bounded
evidence establishes source-level class shape and the declared function bodies;
it does not establish production TU partition or whole-product link closure.

## Completed packet: ECL core lifecycle and resource base

Direct TH10 review establishes a reusable ECL host around the full object's
embedded `0x1018` context. The two callback-subroutine transition paths call a
45-byte allocation-list release at `0x0040C6E0` and then the 71-byte execution
reset at `0x0040C730`. Maintained `EnemyRuntimeUpdate` now expresses both calls.
The release helper is canonical exact; reset is source-present and remains
non-exact at 69 linked bytes versus 71 target bytes.

The neighboring ECL resource base is now source-complete for its reviewed
lifecycle seam. Base vtable slot one is a five-byte default `LoadPackage`
returning zero; the five-byte indexed getter returns one of 32 script pointers
from `+0x0C`; the 34-byte constructor clears the complete `0x1098` object and
installs vtable `0x0046D0F0`; and the 36-byte destructor restores that vtable,
frees lookup owner `+0x8C`, and clears it. The embedded-context constructor
independently clears the two storage cursors at context `+0x1008/+0x100C`.

Two cold linked builds replay all six newly exact functions across **140
bytes**. The three nonzero-field units declare both release calls, the base
vtable fields, and the direct free call exhaustively; omitted-field negative
manifests are rejected. Repository exact coverage is now **39 functions / 1,555
bytes**. Production TU partition and linked-image ownership remain unknown.

## Completed packet: dual-codegen exact-backlog ranking

`scripts/rank-exact-backlog.py` now turns the two existing batch probes into one
fresh selection tool. It obtains the current authored backlog once, runs both
normal COFF and LTCG diagnostics, requires both reports to match every current
address/name/source/size key exactly, and rejects a run if the target, ledgers,
tools, or maintained source tree changes between the two probes. Its explicit
distance combines relative complete-extent size delta with the mismatch ratio
over non-link-field bytes. Profile recommendation and `ltcg_gain` remain triage
signals with `acceptance_authority=none`.

A full target-bound run covers all **53** current authored backlog functions.
Both lanes report **53 mismatches**, zero structural-exact candidates, and zero
unresolved keys. The leading bounded experiments are:

1. `Lzss::AddString`, LTCG: candidate/target **512/516 bytes**, **51/408**
   comparable mismatches, 27 linked fields, distance **0.133**;
2. `CPbgFile::Write`, normal COFF: **67/63**, **10/59**, one relocation,
   distance **0.233**;
3. `EnemyManagerCreate`, LTCG: **94/95**, **22/75**, five linked fields,
   distance **0.304**.

The ranking does not make any function exact or determine its production
profile/physical owner. It gives later sessions a reproducible reason to choose
a source/codegen experiment and preserves both lanes' raw metrics in JSON.

## Completed packet: final structural-exact LTCG promotions

The final seven unreviewed fields from the original linked diagnostic are now
bound to TH10-local targets. The Enemy link uses `g_EnemyManager` at
`0x00477704`, derived/base ECL-resource vtables at `0x0046D0B4` and
`0x0046D0F0`, and the already reviewed `free` entry at `0x00452422`. The
PbgFile link uses the reviewed `strchr` entry at `0x00452730`, complete
45-byte `strrchr` entry at `0x00452930`, and the `GetModuleFileNameA` IAT slot
at `0x004660DC`.

Two independent cold linked builds reproduce:

- `EnemyManagerView::EnemyManagerView`: **29/29 bytes**, one DIR32 field;
- `EnemyEclResourceView::EnemyEclResourceView`: **34/34 bytes**, one DIR32;
- `EnemyEclResourceView::~EnemyEclResourceView`: **36/36 bytes**, one DIR32
  and one REL32;
- `CPbgFile::GetFullFilePath`: **121/121 bytes**, one DIR32 and two REL32.

Each complete `source.ltcg.obj` PDB contribution decodes without gaps and has
exactly the declared fields. Replaying all seven fields leaves zero differences
across all **220 bytes**. Negative manifests omitting the Enemy global DIR32 or
the PbgFile `strrchr` REL32 are rejected. The `strrchr` boundary is independently
reviewed and classified library/CRT; `strchr` keeps origin/boundary state open
because its optimized body shares a backward return tail outside the current
Ghidra candidate. A fresh Enemy/PbgFile backlog probe reports 18 mismatches and
zero structural-exact candidates. All 13 structural candidates / 715 bytes from
the original full LTCG diagnostic have now been promoted through canonical
replay; none remains a diagnostic-only exact candidate.

## Completed packet: canonical Lzss linked fields

All 24 linked fields across the four structural-exact Lzss candidates resolve
to the already target-proven `g_LzssTree` owner at `0x00477858`. Their only
addends are `+0` (`parent`), `+4` (`left`), `+8` (`right`), and `+0x18008`
(`tree[0x2000].right`). The last address is four bytes inside the final
0x0C-byte node of the independently bounded `0x2001`-node / `0x1800C`-byte
array; it is not promoted as a separate global.

Two independent cold linked builds reproduce:

- `Lzss::InitTree`: **36/36 bytes**, four DIR32 fields;
- `Lzss::ContractNode`: **79/79 bytes**, eight DIR32 fields;
- `Lzss::ReplaceNode`: **117/117 bytes**, nine DIR32 fields;
- `Lzss::FindNextNode`: **49/49 bytes**, three DIR32 fields.

Each complete `source.ltcg.obj` PDB contribution decodes without gaps and has
exactly the declared field offsets, types and widths. Replaying the reviewed
tree base/addends leaves zero differences across all **281 bytes**. Together
with the normal-COFF exact `InitEncoderState`, this seam has five exact helpers
/ 323 bytes in two explicitly separate artifact contexts. The result does not
establish a production source/data TU, physical compiler owner, or product
closure. A fresh Lzss backlog probe after the promotions reports only three
mismatches and no remaining structural-exact candidate for this source.

## Completed packet: canonical PbgArchive linked fields

The remaining three structural-exact PbgArchive helpers each have one external
REL32 field. Direct hash-attested target review closes `0x00452706-0x00452717`
as an 18-byte CRT `malloc` wrapper with 94 direct call sites, and
`0x0046054E-0x004605B6` as a 105-byte CRT `_stricmp` wrapper with 11 direct
call sites. Both boundaries end at RET and meet either CC padding or the next
independent CRT function. Their behavior and surrounding CRT owners justify
library/CRT exclusion; the exact original CRT library members remain unknown.

Two independent cold canonical linked builds reproduce all three functions:

- `PbgArchive::GetEntryDecompressedSize`: **51/51 bytes**, one `__stricmp`
  REL32 at `+0x15` replayed to `0x0046054E`;
- `PbgArchive::FindEntry`: **53/53 bytes**, one `__stricmp` REL32 at `+0x18`
  replayed to `0x0046054E`;
- `PbgArchive::CopyFileName`: **47/47 bytes**, one `_malloc` REL32 at `+0x11`
  replayed to `0x00452706`.

All three complete `source.ltcg.obj` PDB contributions decode without gaps and
contain exactly their declared fields. The 151 replayed bytes have zero
differences. Together with the two zero-field pointer helpers, this shared
PbgArchive `/GL` context now has five linked-PE exact functions / 214 bytes.
The diagnostic anchor image remains non-runnable and receives no product or
physical-owner credit. A fresh Factory Ghidra operation-discovery attempt was
unavailable because the native implementation differed from its operator
binding; it returned no semantic evidence, so this packet relies on direct
target and compiler/linker evidence only. A fresh PbgArchive backlog probe after
the promotions reports only seven mismatches and no remaining structural-exact
candidate for this source.

## Completed packet: canonical LTCG exact replay

The exact-unit graph now supports `artifact_kind="linked-pe"` alongside normal
COFF. Shared `scripts/ltcg_link.py` owns the fixed cold `/GL` compile and link
context. `build-match-unit.py` validates the artifact-specific manifest and
builds one image/map/PDB set per shared context. `compare-linked-function.py`
then rebinds PE/map/PDB identity, selects one unique `source.ltcg.obj` PDB
contribution, requires its full manifest size, verifies the pinned Capstone
5.0.6 component hashes, decodes the entire extent, rejects missing or extra
linked fields, validates declared semantic symbols through map publics or the
generated unique anchor aliases, replays target addresses, and compares every
byte. `replay-exact-units.py` cold-builds and checks both artifact kinds.

Two independent cold canonical builds reproduce
`PbgArchive::SeekPastInt` at `0x00435270` across **12/12 bytes** and
`PbgArchive::SeekPastString` at `0x00435280` across **51/51 bytes**. Their
complete PDB-owned extents decode without gaps, contain zero linked fields, and
are raw-equal to all **63 target bytes**. A separate focused `/OPT:REF` test
emitted a 16-byte ordinary stack-ABI `SeekPastInt`; the declared `/OPT:NOREF`
source context is required to recover the target's 12-byte private-register
form. These two accepted claims bind only the declared bounded LTCG context.
The non-runnable anchor image is not a reconstructed product, and production
TU partition, physical owner, link order, data owners, and whole-build closure
remain unknown.

## Completed packet: linked LTCG backlog diagnostics

The repository now parses MSF 7/PDB DBI section contributions directly and joins them to VC7.1 linker-map publics and PE raw bytes. It fails closed unless the PDB RSDS GUID/age matches the PE CodeView record and map timestamp, image base, public start, and section layout all agree. `verify-toolchain.py --execute` exercises this PE/PDB extent path in addition to the existing compiler, resource, and link smoke.

`scripts/probe-ltcg-backlog.py` cold-compiles one source at a time with fixed `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL`, then performs an iterative diagnostic link. Missing externals receive distinct non-runnable data anchors, allowing every linked field to retain a unique candidate-symbol identity without manufacturing executable stubs. PE HIGHLOW fields and decoded external control-flow operands are masked only for structural comparison. The harness, every result, and the report itself explicitly carry no acceptance authority.

The complete authored backlog scan covers all **66 functions across nine sources**. It found **13 structural-exact diagnostics / 715 bytes** and 53 mismatches: three Enemy lifecycle bodies, four Lzss tree helpers, five PbgArchive helpers, and `CPbgFile::GetFullFilePath`. The two PbgArchive pointer-advance helpers have no linked fields and reproduce all **63 raw bytes**. The other 11 candidates contain 34 resolved fields requiring explicit symbol/target review before any canonical linked unit can exist. Two independent cold PbgArchive passes reproduced the same result/extent/field signature. At checkpoint `26af714` all 13 remained diagnostics; later canonical packets have promoted the five PbgArchive and four Lzss candidates, leaving four structural candidates under review.

## Completed packet: batch normal-COFF backlog diagnostics

Checkpoint `d59f975` added `scripts/probe-exact-backlog.py`. It cold-compiles each selected source once under fixed `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc`, enumerates strict COFF functions, joins conservative source-name hints only when one external symbol matches, and runs every authored source backlog entry through target-bound probe comparison. Unresolved mappings, true size/byte mismatches and structural-exact results are separate states; the entire report has `acceptance_authority=none`.

The complete current run covers all **66 authored, source-present, non-exact functions across nine sources**. All 66 map uniquely to a COFF symbol and all 66 are real mismatches; there are zero unresolved and zero structural-exact results. Thus the fixed normal profile has no additional exact unit waiting for ledger-only promotion. This does not reject alternate normal profiles or LTCG ownership.

Probe relocation reporting now also handles a candidate relocation that crosses a shorter target extent: it retains the object addend, marks target encoding/candidate unavailable with `target_extent_complete=false`, and reports a normal size mismatch instead of aborting the batch. Canonical comparison remains strict. No source behavior, exact ledger row, or whole-build state changed.

## Completed packet: strict COFF extent diagnostics

Checkpoint `96f17c6` added decorated-symbol/extent/relocation enumeration and removed the unsafe probe fallback that could slice a caller-requested target length beyond the candidate COMDAT. Missing candidate bytes are now explicit differences. The corrected diagnostic reports `PbgArchive::GetEntryDecompressedSize` as a true 25-byte object against the 51-byte target, and all **20 canonical units / 700 bytes** still cold-replay exact under the strict extent rule.

## Completed packet: exact-work backlog reporting

Checkpoint `88800b6` added `scripts/report-exact-backlog.py`. It joins and validates the function, origin, source-mapping and exact ledgers, with source/module/state filters and JSON output. The 90 source mappings split into **20 canonical exact**, **66 authored non-exact functions / 26,540 bytes**, and **four origin-review Player callbacks / 38 bytes**. Progress and live status now expose the authored/origin-pending split.

## Completed packet: canonical LZSS state-reset backlog

Checkpoint `621add9` promoted the reviewed `Lzss::InitEncoderState` body at `0x00435FD0-0x00435FF9`. Two cold builds reproduced **42/42 bytes and three DIR32 relocations**: dictionary base `0x0048F868`, plus `g_LzssTree` base `0x00477858` with object addends `+4` and `+0x18010`. Ghidra still misses the standalone helper; raw target code and the encoder's inline reset establish its extent and behavior.

## Completed packet: canonical PbgArchive lifecycle backlog

Checkpoint `4ed34ee` promoted `PbgArchive::PbgArchive`, `PbgArchiveEntry::~PbgArchiveEntry`, and `PbgArchiveEntry::PbgArchiveEntry`. Two cold shared-object passes reproduced **51/51 bytes and one REL32 `free` relocation**. Fresh Ghidra review modeled only the entry destructor; raw boundaries and reviewed vector-constructor references remain the evidence for the two inventory-missed constructors.

## Completed packet: exact replay workflow and ResFile backlog

Checkpoint `9b5e7eb` promoted six `CMemoryPbgFile` methods across **210/210 bytes and four relocations** and added `scripts/replay-exact-units.py`. The tool cold-builds one shared object per source/profile and strictly compares every selected unit. The then-current 16 canonical units used three shared outputs instead of compiling the same three translation units 16 times. Manifest validation rejects malformed extents/relocations, overlapping target ranges and inconsistent output sharing; ledger validation joins canonical units to exact, source, function and origin records and requires both exact-unit sets to agree.

## Completed packet: canonical PbgFile exact backlog

Checkpoint `bd9b2e3` promoted six already reviewed PbgFile methods. Current `src/PbgFile.cpp` compiled from absent output paths twice and replayed exact across **220/220 bytes and nine DIR32 relocations**: destructor 46 bytes/three relocations, `Close` 34/one, `Read` 57/one, `Tell` 25/one, `GetSize` 21/one, and `Seek` 37/one. It also extended diagnostic probe output with target-derived relocation candidates; these remain non-authoritative until a reviewed canonical unit passes.

## Completed previous hard packet: Enemy ECL dispatcher context seam

This packet deliberately stayed on the central 14,416-byte `EnemyRuntimeView::DispatchEclInstruction` owner at `0x0040E770-0x00411FBF` instead of pivoting to easier leaf functions. It bounded opcodes `0x15D-0x164`, the nested difficulty tables, and the adjacent integer argument seam. The observable outcome was structural: recover context ownership/ABI and denominator gaps needed for a natural future dispatcher, without writing a partial `default`-return switch and falsely promoting the dispatcher to source-present.

Direct TH10 table decoding maps the eight selected opcodes to case entries `0x004118BB`, `0x004118E2`, `0x0041198B`, `0x004119FC`, `0x00411A3E`, `0x00411B23`, `0x0041009B`, and `0x004100EA`. `0x15F` uses rank interpolation with target constants `1024.0f` and `1/2048.0f`; `0x162` performs the integer analogue `(high-low)*(rank+0x400)/0x800 + low` with the target signed truncation-toward-zero correction. Difficulty cases reach five-entry tables at `0x00411F98` and `0x00411FAC`. These observations are provisional semantic/target evidence, not exactness credit.

The strongest ownership result is the ECL execution context embedded in full Enemy `+0x008..+0x101F`, exactly `0x1018` bytes, with the active-context pointer at full `+0x004`. Context `+0x004` is the current ECL instruction; its target-observed header has opcode at `+0x004`, variable-operand mask at `+0x008`, and dword operands at `+0x010`. Generic typed-operand helpers use context `+0x1008` as operand-stack state, `+0x100C` in local-storage addressing, and context `+0x1014` as the operand-resolver owner. Constructor machine code stores the full Enemy itself at full `+0x101C` = context `+0x1014`, directly binding the generic context to the already-reviewed Enemy operand vtable. Maintained `src/Enemy.hpp/.cpp` now express this exact-size context/instruction view while preserving all full-object offsets.

`0x00412A00-0x00412A0F` and `0x00412A10-0x00412A5A` are now boundary/ABI-reviewed integer argument wrappers. Both take `EnemyRuntimeView*` in EAX and argument index in ECX; the first forwards to the generic int reader and the second resolves a writable int argument address. Complete target scans find all 30 and 9 direct calls respectively inside the central dispatcher, but whether these are source-written helpers or compiler/LTCG outlines remains unknown, so origin stays `unknown/review` and neither is source-present.

Raw corridor review also found three initial-inventory misses: `0x00412A60-0x00412A6D` (14B), `0x00412A70-0x00412A7D` (14B), and `0x00412A80-0x00412A90` (17B). They are CC-delimited helper-shaped bodies adjacent to the seam, but fresh Ghidra xrefs plus complete direct-REL32 and absolute-pointer scans find no target references. They are therefore added only as new `unknown/review` candidates; authored/source/exact denominators do not increase.

Adjacent-game hypothesis provenance for this packet: clean committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97`; clean committed TH09 HEAD `f1d4c1e332828a0cb8b9308b61b79ce77f86e4d0`; TH095 live tree was dirty (4 paths), so only committed HEAD `dee282c33dbb75ca0545d3115cd421a6e4f6a7bd` was consulted. Adjacent source supplied only ECL operand source-family hypotheses after TH10-local recovery; no adjacent addresses, opcodes, layouts, owners, or exactness were transferred.

Pinned VC7.1 SP1 build6030 compiles the refined Enemy source under fixed normal and `/GL`. A separately compiled clean-HEAD baseline and the refined normal object emit byte-identical 586-byte `EnemyConstruct` COMDAT contents with the same four relocation sites; both fresh target diagnostics are non-exact at `63/657` comparable bytes. The old `64/657` note is superseded. This is structural source feedback only, not constructor or dispatcher exactness.

The first cold canonical replay after adding the header declarations correctly failed on the int lvalue resolver because VC7.1 renumbered compiler-internal `$L...` symbols by exactly +37. Clean-HEAD versus revised COFF symbol review proved every int/float resolver jump-table/case label kept the same section offset, and the canonical relocation offsets, types and target VAs are unchanged. `config/match-units.toml` therefore refreshes only those generated object-symbol names. A second cold deletion/rebuild/replay then returns exact 6/6, 11/11, 80/80 and 80/80 for all four configured units. The failed receipt is retained as recovery evidence; exactness was not assumed across the manifest update.

## Completed hard packet: Enemy ECL operands

The packet reconstructed the four vtable-owned operand methods immediately following the previously reviewed 14,416-byte Enemy ECL dispatcher physical owner. Their boundaries/origin were already reviewed authored-game denominator entries at packet entry; this packet adds maintained source and exactness evidence rather than inflating the authored denominator.

| Address | Maintained source | Target bytes | Final state |
| --- | --- | ---: | --- |
| `0x00411FC0-0x004122F3` | `EnemyFullObjectView::ReadIntOperand` | 820 | source-present, non-exact |
| `0x00412300-0x0041234F` | `EnemyFullObjectView::ResolveIntOperand` | 80 | source-present, canonical exact |
| `0x00412350-0x004126C3` | `EnemyFullObjectView::ReadFloatOperand` | 884 | source-present, non-exact |
| `0x004126D0-0x0041271F` | `EnemyFullObjectView::ResolveFloatOperand` | 80 | source-present, canonical exact |

The reviewed full Enemy vtable at `0x0046D0C0` owns these entries in slots 1-4. Raw target review closes each `RET 4`, adjacent padding and each resolver jump table. Ghidra does not model the four vtable entries as standalone functions; its semantic view remains provisional and carries no exactness credit.

## Operand protocol and layout refinement

The two lvalue resolvers and the target spawn copy independently close the 0x20-byte block at full Enemy `+0x1138..+0x1157` / runtime `+0x0FC..+0x11B` as four writable integer ECL variables followed by four writable float ECL variables. Spawn executes `REP MOVSD` for exactly eight dwords from request `+0x20` to full Enemy `+0x1138`, so maintained `EnemyEclVariableView { int integers[4]; float floats[4]; }` is shared by `EnemyRuntimeView` and `EnemySpawnRequestView`. This refines the prior neutral eight-dword transport block without changing the reviewed object extents.

Both readers preserve the target's exact 51-selector domain `-10000..-9950`. TH10-local cases cover RNG U32/unit/signed-unit values, signed random angle scaling, current/offset/base motion values, Player position and angle-to-point helpers, update timer, runtime flag bit `0x10000`, the four int/four float variables, special-enemy coordinates, managed-VM id resolution followed by signed VM word `+0x38A`, rank, difficulty, velocity angle, life and difficulty equality tests.

The integer and float readers deliberately remain separate because TH10 contains type-dependent behavior elsewhere. Direct table entry sends integer selectors `-9996` and `-9976` to target `0x0041219A` while `ECX` still names this Enemy, so both readers expose this Enemy's world-Y and the integer form applies the target float-to-int helper. Only selector `-9962` enters through `0x00412192`, replaces `ECX` with the manager primary special-enemy pointer, and then falls through to `0x0041219A`; selector `-9963` resolves the corresponding special-enemy X directly. Maintained source preserves this target control flow rather than normalizing it from adjacent games.

TH10-local writers establish `0x00474C74` as a `0..4` difficulty index and `0x00474C98` as a signed rank-like value restored from replay and adjusted/clamped to `[-0x400,+0x400]`; maintained source uses descriptive names `g_EnemyDifficulty` and `g_EnemyRank`, not original-identifier claims. Helper evidence independently identifies RNG helpers at `0x0044B9E0/0x0044BB20/0x0044BB90`, Player angle-to-point helper `0x00426660`, float-to-int helper `0x00463B2C`, and VM id resolve-or-clear helper `0x00449450`. Source does not add a target-absent null guard around the VM lookup.

Committed adjacent source was consulted only after TH10 switch tables/storage were independently decoded. TH08 observed HEAD `a45e99fb1942714e6edded20847e32a654d56f97` was clean; adjacent material only corroborated the broad typed int/float/lvalue ECL source family. No selector, address, layout, ABI, owner or exactness fact was transferred. Dirty adjacent untracked content was not used.

## Compiler and exactness feedback

Final maintained `src/Enemy.cpp` compiles with pinned VC7.1 SP1 build6030 under fixed normal `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc` and the same profile plus `/GL`.

True final normal `/Gy` extents versus target are:

- `ReadIntOperand`: **800 / 820 bytes**; target-window diagnostic `mismatch`, **115/452 comparable bytes**, 92 object relocations, `acceptance_authority=none`.
- `ResolveIntOperand`: **80 / 80 bytes**.
- `ReadFloatOperand`: **800 / 884 bytes**; target-window diagnostic `mismatch`, **95/540 comparable bytes**, 86 object relocations, `acceptance_authority=none`.
- `ResolveFloatOperand`: **80 / 80 bytes**.

The two 80-byte lvalue resolvers each have an explicit canonical `artifact_kind="coff"` unit. Each complete owned extent carries five DIR32 relocations: one jump-table relocation plus four local case-target relocations. Current-source clean builds and repeated target-bound comparison return `result="exact"`, 80/80 bytes, with all relocation targets matching the target. These are the only new exact promotions in this packet.

The two readers remain source-present/non-exact. Successful `/GL` compilation preserves a possible production LTCG/interprocedural context but gives no standalone exactness credit.

Because all current Enemy exact units share `src/Enemy.cpp`, the final integration gate deletes/rebuilds and compares every configured exact unit after the operand edits:

- `enemy-manager-draw-callback`: exact 6/6, zero relocations;
- `enemy-ecl-dispatch-forwarder`: exact 11/11, one relocation;
- `enemy-ecl-int-lvalue-resolver`: exact 80/80, five relocations;
- `enemy-ecl-float-lvalue-resolver`: exact 80/80, five relocations.

Total repository canonical exact coverage after the Enemy packet was **4 functions / 177 bytes**.

## Ledger and verification planes

Current ledger:

- candidates: **1277**
- origin/boundary pending: **1130**
- authored: **132 / 53,233 bytes**
- excluded: **15**
- source-present: **104 / 27,584 bytes**
- canonical exact: **46 / 1,650 bytes**

Current hierarchy packet delta from `d4af1a5`: **+7 functions / +95 exact bytes**, seven source mappings, and one compiler-generated deleting-destructor denominator entry. The authored source backlog remains **54 functions**. Session exact delta from `444eb1a...` is **+42 functions / +1,473 bytes**.

**Source presence:** seven ECL-host class functions became source-present.

**Exactness:** repository canonical exact coverage is 46 functions / 1,650 bytes. The complete mixed set cold-replays through the configured shared artifacts across five source files. This exact lane does not establish production object ownership or whole-build closure.

**Whole build:** actual final `python3 scripts/build.py` returned **RC2 / explicitly open**. Production compiler flags, TU partition, libraries, resources and link order remain unknown. `build.py --check`, tracking, toolchain execution and public CI pass this honest open state.

**Runtime:** not performed. There is no closed faithful reconstructed Windows-i386 product to execute.

**Factory Truth:** no Factory replay or acceptance submission was performed by this session. Repository canonical exact results and Factory accepted facts remain separate states.

## Scratch and recovery artifacts

The overall recovery/resume entry inventory earlier in this conversation was **365 regular files / 4,218,280 logical bytes / 0 files >64 MiB**. At the clean `2053a39e...` hard-packet boundary, `.analysis/` contained exactly **319 regular files / 3,709,138 logical bytes / 4,464 KiB allocated / 0 files >64 MiB**. Legacy/unknown analysis, other campaigns, Ghidra state, toolchains, Wine state and the private target were preserved.

The hard campaign `.analysis/gpt-web/20260914-enemy-dispatcher-hard/` exits with **28 files / 257,221 bytes**. It retains the manifest, bounded dispatch/table/helper disassemblies, top-level and nested table maps, clean-HEAD baseline source/object evidence, final normal and `/GL` objects, constructor/read-int/read-float diagnostics, local-label shift receipt, exact replay receipts (including the intentional failed pre-refresh receipt), and whole-build logs. Superseded current-packet `Enemy.context.*` scratch and reproducible `build/match/*.obj` outputs were removed only after final receipts existed.

The previous Web checkpoint's final `.analysis/` inventory was **347 regular files / 3,966,359 logical bytes / 4,796 KiB allocated / 0 files >64 MiB**. No legacy/unknown artifact was bulk-deleted, and the ignored private `resources/th10.exe` was not modified, moved, staged or committed.

The linked-diagnostic, canonical replay, PbgArchive, Lzss, final-structural, backlog-ranking, ECL lifecycle, and current host-hierarchy campaigns each retain one compact manifest; full JSON reports, objects, linked images, maps and PDBs are reproducible below ignored `build/`. No copied target was created.

## Next hard frontier

Continue the user-selected ANM/ECL exact campaign owner by owner. The generated
draw promotions leave `UpdatePulsingRadialTrail @ 0x00445620` as the highest
known near-exact owner: its complete 598-byte linked contribution matches
524/526 comparable bytes. Roughly twenty bounded natural-source variants have
not reproduced the target's exchanged X87 load/add operands for one commutative
Y sum. Retain that honest near match rather than introducing a volatile
dependency or assembly. The next productive non-radial candidate is
`AnmLoadedView::SetAndExecuteScriptIdx @ 0x0043E8B0`, whose current linked
contribution has exact target size 287 and matches 249/271 comparable bytes.

The largest remaining core owners are ANM `ExecuteScript @ 0x00450B30`
(9,587 bytes) and generic ECL `Run @ 0x0044E1A0` (7,020 bytes). Use smaller
renderer/update owners to recover production compiler context and source shape,
then apply that evidence to those dispatchers. Keep exactness tied to complete
owned extents and cold canonical replay; linked diagnostic proximity alone does
not promote a unit.

After ANM, continue the generic and Enemy ECL owner families, including a second
Enemy opcode cohort that shares the recovered typed operand context. The real
production link graph remains a major infrastructure gap; canonical bounded
linked-image units do not close it.

The exact-reconstruction campaign remains active/incomplete. Faithful Windows-i386 whole-build closure, runtime validation, Factory acceptance, semantic reconstruction and portability remain open.
