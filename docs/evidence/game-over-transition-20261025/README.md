# Game-over transition: complete isolated pilot, 2026-10-25

Status: **parked; whole-owner nonexact, no source/exact credit**.
Baseline74b54198514cec72770e2c395b1112e49bad0a43. Reviewed authored owner
4231D0..42336A is411 bytes/108 instructions, plainRET, no switch table.

| Model | Whole candidate / target | Result |
|---|---:|---|
| First complete actual Player callback graph | 413 / 411 | structural rejection |
| Add actual Ending.cpp music caller only | 413 / 411 | diagnostic normalized whole-body identical |

The first comparison preceded broader host tests. Both graphs naturally recover
EDI receiver/plainRET from ordinary `GameOverTransition(ResultMenuView *)` and
the real PlayerUpdateCallback → PlayerUpdateCallbackBody edge. No forced ABI,
noinline, register hint, padding, flag panel or copied target bytes were used.
The existing declaration-only PlayerFallbackRuntimeTransition seam is replaced
at its real state2/updateTimer>=30/negative-lives branch without simplifying
surrounding conditions. Existing g_GameOwner477830 declaration is shared via a
new forward-declaration header, not duplicated under an invented alias.

The model reuses the existing423370 sibling body with target-verified script
129→128 and completed1→0 changes. The sibling is itself nonexact, not proof.
It caches game, handles mode1/configuration1000 transition2/4, sets state6 and
timer0, flags58|10, captures ASCII+8998 resource and manager, allocates/initializes
script0/adds secondary VM, requests rectangle32/16/384/448, stores GUI+9EC8
frontAnm, allocates/initializes128/adds primary VM, loads bgm/th10_17.wav, rereads
supervisor10 for optional dummy4 then dummy2, rereads statistics and marks
owner+1D8A3, sets completed0, saves speed and sets speed1. Shared canonical
GameStatisticsOwner.tail.payload[2F] has offset1D86C+8+2F=1D8A3; no allocation or
new global is invented. Other legacy FrontEndProfileData accesses are unchanged.
This symbol/alias mapping is evidence-backed, not production storage closure.
The frozen proposal inherits the sibling's preceding423370 comment; the adjacent
4231D0 annotation identifies the inserted body. The stale prose about native
EDI being open is superseded only for these diagnostic images, not product ABI.

## Complete evidence and remaining mismatch

`inventory.json` contains complete target/candidate instruction inventories,
all28 explicit field-role/base/addend bindings and all3 fixed local branches.
The stripped target's relocation-driven decoder sees only10 REL32 fields;
`target-fields.json` explicitly supplies all18 DIR32 obligations, checked against
raw encoded target values. Candidate identities use exact map publics, never
nearest-anchor labels. All3 fixed locals are retained, not masked as link fields.
Independent read-only review confirms semantic order, canonical statistics offset,
full28 field bindings,3 locals, ordinary ABI and complete guard scope.

The complete owner still differs: initial mode memory compare becomes an ECX
load, local stack8 replaces4, flags58 uses EBX rather than EDX, two aggregate ID
result slots are used rather than one, capture ID is privateECX rather than a
stack argument, and music filename is privateEAX rather than stack. VM flag/layer
scheduling also differs; historical AllocateVm/InitializeVm tuning is not reopened.
First graph dependencies remain Allocate153/161, Initialize207/217,
Capture168/162, LoadMusic90/120 and QueueCommand90/104, all nonexact.

Source search found only Ending.cpp:392 as an extra-TU direct music caller.
Adding it with the same profile specifically tested whether real caller visibility
restored the filename stack ABI. It did not: LoadMusic stays90 and GameOver's
whole413 bytes are identical after diagnostic link-field masking. Main/Gui
context panels were considered but not run because no new direct caller evidence
justified them. No further independent hypothesis was established; park rather
than search register, spelling, inline or compiler-flag combinations.

## Graph, guards, recovery

Player.cpp actual callback root plus FrontEnd.cpp, AnmManager.cpp,
AnmVmCreate.cpp, AnmVmId.cpp and Sound.cpp; second graph adds only Ending.cpp.
Pinned existing Player /GL O2 profile, no /GS override. The images contain
unresolved-symbol data anchors and must never be executed as products.
Five complete same-image canonical linked guards pass533 bytes/all0 fields in
both images: four AddVm variants118/118/112/112 and FindVm73. These are existing
credit, not new admission. Original canonical artifacts are not rebuilt/modified.

- Preflight target/toolchain-execute/tracking/status: exit0,
  repository-command:284b438697274e28b7a472ab8266319e.
- Fresh native Ghidra check10:41:20 UTC passed target:th10-main via
  factory-native-command, targetSHA2f14760...9040.
- First native exit0: repository-command:c1a0ae6a3f4c4990a090e9572f723259.
- Ending native exit0: repository-command:7b6ce7929d444e15a3dce9f58bf8f8d2.
- Both native jobs serial, one CPU/nice10, timeout300s, retained log/exit;
  no native timeout, cancellation or duplicate run.
- Initial analysis script named inspect.py shadowed Python's standard library;
  renamed analyze-native.py before successful analysis. Initial guard metadata
  incorrectly selected proposal pdb_source without its graph and attempted a
  COFF unit through linked comparator. Rejected outputs are retained separately;
  corrected actual graph and five genuine linked units pass. No failed result
  is relabeled. First disassembly attempt used nonexistent read_va, corrected
  to read_address without rebuilding. These were post-link analysis failures.

Frozen430 original source/tool/script/proposal/protected inputs remain unchanged;
`artifact-hashes.json` retains169 original artifacts. Native outputs reside in
build/gpt-dots/game-over-{first,ending}; ignored source/logs under
.analysis/gpt-dots/game-over-20261025. Nine unrelated D3D files remain untouched.

## Reproduction and stopping condition

Use a fresh path with one CPU/nice10 and retained log/exit:

    timeout 300s taskset -c "$CPU" nice -n 10 scripts/repo-python \
      docs/evidence/game-over-transition-20261025/reproduce.py \
      --trial first --output-dir build/gpt-dots/game-over-reproduction

`--trial ending` adds the one real caller. `--prepare-only` checks/materializes
the retained proposal without native execution; fresh materialization was verified
byte-identical to every isolated source file. Reproduction verifies target and317
maintained/tool/protected inputs before applying the patch. Proposal only: no
canonical source, recipe, ledger, denominator or metric change. No full-CI,
runtime/product or phase-gate claim. Prior CI is not relabeled for this packet.

The build command emits native artifacts, not comparison acceptance. Then run:

    scripts/repo-python docs/evidence/game-over-transition-20261025/compare.py \
      --first-dir build/gpt-dots/game-over-first \
      --ending-dir build/gpt-dots/game-over-ending \
      --output-dir .analysis/gpt-dots/game-over-comparison-new

Use the newly built directories when reproducing. This read-only retained-image
comparison regenerates full PDB extents/instructions, all28 explicit target and
candidate fields,3 locals, five canonical guards per image and diagnostic
neutrality. Recomputed packet JSON exactly matched all five retained JSON files,
repository-command:392ba3c45d164fc78fb5ad8b42df639d. No native rerun was needed.
Prepare-only is source-materialization validation, not a native replay.

Focused toolchain declarations, tracking, progress and diff checks pass,
repository-command:f707a11586ae43a88c6dc86a2e0c7ece; all430 frozen inputs and169
original artifacts unchanged. Progress metrics/SVG have no diff. No full CI was
rerun for this evidence-only proposal checkpoint.
