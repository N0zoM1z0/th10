# TH10 reconstruction handoff

Updated 2026-10-24 for the operator-approved
[exact-first method](TH10_EXACT_FIRST_METHOD_20261024.md) and Web/Factory resume.
Method checkpoint: **07424cf**; ANM/ECL core baseline: **00c7466**.
Committed source checkpoints: MIDI inline-clock recovery **45c16aa**,
Options WORD-consumer recovery **1e35737**, five-factor D3D **6b3b5e2**, and
real D3D callees **a454afc**. This checkpoint adds
[WinMain logging recovery](evidence/winmain-logging-20261024/README.md).
Three separately measured models restore two real Log calls, restart buffer
stores and a natural Flush member. WinMain remains 1405/1389, nonexact.
Flush was independently admitted as a complete authored owner before exact
credit, then freshly canonical-passed **79 bytes / all 6 fields** at 19:37:59 UTC.
This batch adds 79 exact bytes, 79 reviewed authored bytes and one source owner.
The same fresh ten-TU image preserves six canonical guards, 1436/all 69.
The old Log and Fatal canonical recipes cold-passed 201/all 12 and 201/all 11;
the original three-TU color recipe preserved wrapper108/all 11 and helper135/all 8.
All 112 CI stages passed for this revision at 19:55:46 UTC, exit 0, with frozen
inputs preserved. The separate global automatic boundary check still fails
on pre-existing manual-versus-automatic divergence, as detailed below.
The project remains **active-incomplete**. Goal: **98% of reviewed authored
owned bytes exact**. Current commit prefix: `gpt-dots: ...`; local commits only,
never push. Earlier `gpt-6.1-sol` prefixes and direct-local-only/no-subagent
directions belong to predecessor sessions.

The new method changes selection and experiment order, not acceptance rules.
MIDI and Options have completed bounded source experiments and are parked with
receipts below; neither added a whole-owner exact match or source-present owner.
The prior D3D callee checkpoint added 243 whole-owner exact bytes across
MainClearPresentTwice and MainSetScreenColor. Current logging recovery adds
**79 new whole-owner exact bytes** for Flush only, not its WinMain caller.
InitD3DRendering and WinMain remain nonexact, with no runtime/shared-global closure.
All experiments in the preceding ANM 15-control panel are terminal.

## Maintained state

| Owner / metric | Last verified maintained result |
| --- | --- |
| ANM ExecuteScript / target 9964 | **9960/9964**; 4 ordinary differences; all 350 fields / 95 fixed local destinations; case 7990/7994, 92/92 groups, gap 0 |
| ECL Run / target 7020 | 6995/7020; 25 differences; 189 fields / 62 locals; 171-byte float callee exact |
| Enemy dispatcher / target 14416 | 14336 bytes; frame 2C4; case 3245/11128, 45 sized groups, gap 328 |
| StageExecuteScript / target 1868 | Retained 1868 bytes; frame 8C; nonexact; case 1443/1619, 13/15 groups, gap 2 |
| PlayerInitialize / target 1435 | 1380/1435; 55 differences; 49 fields; actual 17-input graph recompiled ANM |
| MIDI ProcessEvent / target 1670 | 1690-byte candidate; all 37 fields recovered; structural rejection, parked at 45c16aa |
| UpdateOptions / target 3188 | 3276-byte candidate; two observed WORD consumers recovered; nonexact, parked at 1e35737 |
| InitD3DRendering / target 1159 | Current candidate 1238 bytes; wrapper inlined, no direct wrapper call; nonexact |
| MainClearPresentTwice / target 135 | Fresh canonical exact 135/all 8; also replayed in the wrapper's same fresh image |
| MainSetScreenColor / target 108 | Fresh canonical exact 108/all 11 with recursive same-image helper 135/all 8 proof |
| WinMain / target 1389 | 1405-byte candidate after three logging models; naturally inlined Flush; nonexact |
| GameErrorContext::Flush / target 79 | Fresh canonical exact 79/all 6; independently admitted complete authored owner |
| Authored / source / authored exact | 735 functions / 273658 bytes; source 617 / 248496; exact 412 / 75038 |
| Authored source-present nonexact / absent | 205 / 118 functions |
| Overall exact / mappings | 1255 functions / 141969 bytes; 1101 mappings |
| Windows i386 product / runtime | Open; semantic reconstruction and portability not started |

[Live progress](PROGRESS.md): 1777 candidates and reviewed boundaries,
518 excluded and 524 indeterminate origins. The focused Flush admission adds
one authored boundary/origin and source mapping; its 79 exact bytes were credited
only after fresh canonical replay. All prior ledger rows were byte-preserved.
Progress and SVG reflect the new numerator and denominator separately. No
profile, product/runtime or phase-gate promotion follows.

Fresh Ghidra architecture export passed, but the separate
`report-boundary-inventory.py --check-ledger` failed on pre-existing
manual-versus-automatic divergence across **48 states, 106 confidences and
evidence text**, with **zero address/extent changes**. This is neither a passed
check nor a text-only difference. Preserve prior manual evidence; focused
single-owner acceptance does not resolve that global automatic-check failure.
Receipts remain under `.analysis/gpt-dots/error-flush-boundary-review-20261024/`.

## Next bounded work

Select medium/large representatives by complete comparability, a concrete
semantic uncertainty and family reuse. Build natural low-level C++ with honest
provisional offset/numeric names, real widths/captures/rounding/control flow and
actual callee declarations; compare the first complete owner early. Use one
falsifiable coherent model per trial and targeted native controls only where
needed. Readability/class cleanup follows stable exactness, with complete replay
after each refactor. Keep new exact owned bytes and source coverage separate.

ANM's four-byte and ECL's 25-byte residues are preserved but parked against
further neutral spelling panels. Reopen only with new target, def-use, lifetime,
callee or real-context evidence, an explicit prediction and a distinct baseline.
After repeated byte-identical controls, retain a bounded receipt and change
representative; there is no mandatory trial count or global exhaustion rule.
The [method checklist and candidate-selection space](TH10_EXACT_FIRST_METHOD_20261024.md)
define the next experiment packet.

First representative **MIDI ProcessEvent** is now bounded and parked after one
informative model: target1670, current1690, all37 fields restored, five guards
exact378/all5. It remains a structural rejection, not an ordinary-byte score.
Private MIDI helper register edges independently support the retained `/GL`
lane; original per-TU flags remain unknown. See the result below.

The [Options pilot](evidence/options-input-word-20261024/README.md) now has a
fresh eight-TU baseline and one tested WORD-consumer model. Both target AX
conditions are recovered; Refresh3999/all112 and every executable byte outside
the controller are preserved. UpdateOptions remains3276/3188 with structural
and binding gaps. The two fewer static FindVm calls are branch factoring, not
lost runtime work. Park unsupported layout/adapter spelling panels.

The prior [real D3D callee checkpoint](evidence/d3d-real-callees-20261024/README.md)
accepted helper 135/all 8 and wrapper 108/all 11 using strict same-artifact
recursive callee proof, with unchanged legacy zero-field behavior. Its maintained
suite passes 110 tests and its historical full CI passed all 112 stages. D3D
still inlines the wrapper and remains 1238/1159, with no direct wrapper call.
Reopen only with a new evidence-backed boundary model, not global noinline controls.

Current [WinMain logging evidence](evidence/winmain-logging-20261024/README.md)
records three separate full-owner measurements: two real Log calls yield 1327
bytes; restart BufferEnd/first-byte stores yield 1343; the natural Flush member
yields 1405. All are nonexact against 1389; these are extents, not mismatch counts.
The reset trial's original post-build audit exited 1 and is preserved. A separate
same-image supplemental CFG proof establishes BL = 0 at the byte store; it does
not rewrite that failed audit. All trials preserve eight guards 1679/all 88 and
Log/Fatal 402/all 23; the last also preserves WriteFile 218/all 20 and literal
payloads. Flush boundary/origin was accepted independently before compilation,
then admitted before its fresh canonical 79/all 6 replay and exact credit.
The same fresh canonical ten-TU image preserves six guards 1436/all 69.
Log/Fatal and the original color canonical recipes have now cold-passed;
the new full CI passed all 112 stages at 19:55:46 UTC, with inputs preserved.
Only CI-result documentation changed afterward and was checked separately.
See the [validation summary](evidence/winmain-logging-20261024/validation-summary.json).
Original class/method/TU and runtime/shared-
global identity remain unproven. Device null recheck after Reset, instance
publication timing and allocation capture/final-null-store models remain untried.
Ending remains queued: its retained source matches its receipt, but 20/52
support/header inputs drifted and prior aggregate controls were neutral. Do not
tune against that stale graph or start another unsupported allocation panel.

ECL's residuals are mostly stack/register/SIB choices, including 16 stack homes.
Nonlocal coupling is observed; a wrong compiler or production TU cause is not
proved. The existing code is already compiler-oriented, so readability alone is
not a supported explanation. Near-match progress adds no whole-owner exact
credit until all ordinary bytes and complete comparison gates pass.

## Parked complete ANM parent

Read the [source/proof packet](evidence/anm-child-request-record/README.md),
[350-field contract](evidence/anm-child-request-record/binding-contract.json) and
[actual maintained comparison](evidence/anm-child-request-record/current-parent-proof.json).
Preserve all 9964 bytes: 9588 decoded code/alignment bytes and 94 owned table words.
Every local destination must retain its offset; never crop, mask ordinary bytes
or normalize a moved code/table target. Fifteen vector references use genuine
12-byte bases with addends, not coincident neighboring four-byte anchors.

| Offset, hexadecimal | Remaining difference |
| --- | --- |
| +113, +115 | Child91 layer MOV/PUSH uses ECX instead of target EDX |
| +249A, +24A0 | Second radial vertex Z addition swaps position.z/positionOffset.z loads |

The earlier slow-Tick attribution of the second pair was wrong. Fresh full-offset
Ghidra/decoding places it at4412C8/4412CE in the radial loop; baseline slow Tick
already matches. Original class/record spelling, register-allocation cause,
production TU/profile and data ownership remain unknown.

The [last 15 controls](evidence/handoff-request-cleanup-20261024/README.md) add no
improvement: value-return/read helpers, value/reference member accessors, local
type/signedness forms, current real-Player callers and three declared TU orders.
Twelve complete comparisons retain9960, two regress to9950 and one9948-byte
outlined-helper owner is strictly refused and fails child routing. Earlier23
controls are in the source packet. Do not repeat these without a new premise.
No new hypothesis after those 15 was compiled or measured.

## Expanded current inputs

Producer root: `.analysis/gpt-6.1-sol/anm-full-parent-bindings-20261024`.
Use **manifest-current.json / compare-current.py**. Its compilation and initial
manifests are immutable; later ledger-note and reviewed host-pin transitions are
recorded separately. Older ecl-case-order/Player global freezes are historical;
do not weaken them or overwrite their before/support caches.

| Carrier below build/gpt-6.1-sol/anm-full-parent-bindings-20261024 | Purpose |
| --- | --- |
| `maintained-current` | Actual maintained ten-input graph; ANM9960/all350/95 and ECL6995 |
| `child91-argument-record` | Selected private witness; all352 public owners raw-equal to maintained |
| `maintained-player-17` | Actual ANM recompile in Player17/52 graph; Player1380/all49 |

The current helper also checks 36 guards4618/all121, ECL's complete171-byte callee
and16 guards1617/all21, and Player's8 guards1753/all99. Other351/352 and518/519
public owners remain raw-identical in the source checkpoint's peer proofs.
Static/unowned fragments and runtime are outside those proofs. No independent
whole-graph cold pair or exact acceptance exists for this nonexact parent.

## Recovery, archives and validation

Read AGENTS.md, [RE_WORKFLOW.md](RE_WORKFLOW.md), [ORACLES.md](ORACLES.md), relevant
source/ledger rows and live status/diffs before changing state. Preserve all nine
unrelated untracked D3D files. Only original Japanese1.00a resources/th10.exe is
valid, SHA-256 2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.

```sh
git status --short --branch
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --execute
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/report-reconstruction-status.py
scripts/repo-python .analysis/gpt-6.1-sol/anm-full-parent-bindings-20261024/compare-current.py
```

These replay retained actual-source artifacts, not a new cold build. The
2026-10-24 Web preflight reverified the fixed target, VC7.1 build-6030 smoke and
tracking/status; Factory-native Ghidra `check` passed at 14:17 UTC for
`target:th10-main`. Retained replay reconfirmed ANM 9960/9964 with all 350
fields, 95 local destinations and 36 guards; ECL 6995/7020 with all 189 fields,
62 local destinations, its complete 171-byte callee and 16 guards; Player
1380/1435. This adds no exact acceptance.

Re-attest Ghidra before new target analysis using the Factory-native transport
in AGENTS.md. Current work uses registered repository `th10`; the coordinator
alone schedules native builds serially on one CPU at niceness 10 and owns final
gates/commits. Read-only analysis and disjoint documentation can be delegated.
Preserve Ghidra read-only/noanalysis/rollback and bounded CPU/heap settings.
Full mapped text 401000..465A80 inclusive: 412289 bytes, SHA-256
cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f.

[Latest cleanup](evidence/handoff-request-cleanup-20261024/cleanup-summary.json):
15 closed images / 355 files,32.70MB→0.53MB;32.17MB reclaimed; all23681 protected
hashes agree during compaction. Plans/producers/logs remain, but closed private
sources/images/reports require restoration. [Artifact index](RE_ARTIFACTS.md)
lists every archive and retained Stage/Enemy/older caller context. Ignored archives
are not in a Git clone; transfer the workspace and inventories when required.
[Validation](evidence/handoff-request-cleanup-20261024/validation-summary.json)
records that historical cleanup's focused checks and the earlier source
checkpoint's 89+22-stage CI coverage. MIDI checkpoint **45c16aa** later passed
all 111 stages; Options **1e35737** had focused validation at its own checkpoint.
Five-factor D3D checkpoint **6b3b5e2** independently passed all 111 stages,
including the retained Options changes, with frozen inputs preserved.
The new real-callee source/tooling has its own subsequent 112-stage full-CI
pass at 18:28:40 UTC, exit 0, and its own new-source ten-TU regression pass.

## First exact-first pilot result: MIDI

The [MIDI pilot](evidence/midi-exact-first-20261024/README.md) cold-reproduces
the old incomplete parser baseline, then removes one unsupported source-only
clock helper boundary at its three actual uses. Complete parser call/data
inventory recovers 37 fields, but the owner is 1690 versus 1670 bytes and remains
nonexact. Tick stays481/473; five guards remain exact378/all5. No exact ledger
or source coverage credit is added. Source and fixture changes are scoped to
Midi.cpp and its host fixture; no profile, ABI or comparator relaxation.

The +20-byte extent difference includes loop alignment and skip jumps; it does
not mean only20 bytes mismatch. Do not resume an uninformed register/alignment
panel. Require new target-backed evidence before the next model, otherwise move
to the queued complete representative. Preserve old/updated source receipts and
the nine unrelated D3D files. See the pilot packet for actual validation scope.

Full MIDI checkpoint **45c16aa** CI completed all 111 declared stages with
exit 0. The source and fixture tested are that checkpoint's inputs, not the
later Options change or current dirty Main.cpp. Receipt and log:
.analysis/gpt-dots/midi-exact-first-20261024/root-ci.log and root-ci.exit.
The RPC timeout was reconciled from its completed receipt, not rerun. Subsequent
documentation records that result; link/progress/whitespace checks were repeated.

## Options WORD-consumer checkpoint

See the [complete bounded receipt](evidence/options-input-word-20261024/README.md).
Validation includes2909 host cases,14 detected negative controls, full
Refresh3999/all112, all333 public-owner diagnostics and independent whole-.text
preservation outside UpdateOptions. This is focused modified-owner validation,
not a new full111-stage CI run. No new exact bytes or source owners are claimed.

## Current real D3D callees

[Durable result and copied canonical receipts](evidence/d3d-real-callees-20261024/README.md).
Current Main.cpp SHA-256:
`34af603b4fbfa66a9a94802c3383dcd108e80a57b8f3de7d27513a275654b577`.

The same three-TU graph-only baseline preceded implementation. Independent
full replay matches helper 135/all 8, wrapper 108/all 11 and real Flush
133/all 4. A separate v04 width audit accounts for the inlined SetViewport
eight-byte cleanup and preserves flag/stack checks; the original unresolved
audit is retained. D3D is 1238/1159 and does not directly call the wrapper.

Fresh canonical unit `main-clear-present-twice` passed at 18:05:14 UTC.
The strict `static_callee_unit` capability is now applied with 110 maintained
tests passing. A fresh `main-set-screen-color` cold build passed at 18:15:52
UTC, wrapper 108/all 11 plus recursive same-image helper 135/all 8, with all
frozen inputs preserved. Both complete owners are now credited, totaling
243 authored exact bytes; no D3D parent credit is added.

New-source ten-TU regression passed at 18:11:20 UTC: six complete owners,
1436 bytes / all 69 fields. Receipt:
`.analysis/gpt-dots/d3d-callee-regression-20261024/receipt.json`.
Full CI independently passed all 112 stages at 18:28:40 UTC, exit 0, with all
frozen inputs unchanged. Logs and hash receipts remain under
`.analysis/gpt-dots/d3d-callees-checkpoint-20261024/`; the durable
[evidence summary](evidence/d3d-real-callees-20261024/validation-summary.json)
records the scope. Only CI-result documentation changed afterward and was
rechecked. Runtime, shared-global identity and parent closure remain open.

## Historical D3D checkpoint: five-factor source 6b3b5e2

See the [durable D3D receipt](evidence/d3d-exact-first-20261024/README.md).

Historical source tested by the 17:32:16 UTC regression and checkpointed as
6b3b5e2, `src/Main.cpp` SHA-256:
`3bbb030a9d3f9d1961ebbfcfbccb5c2dbf080cf9dc68f8d129f9362d7cfb8de7`.
The completed trials below are structural inventories, not ordinary-byte
equality scores or canonical exact acceptance:

- Fresh baseline: 1051 candidate bytes versus target 1159, 84 versus 100 address
  fields, and 28 versus 29 local branches.
- Tangent-only model: one pi literal gains its float suffix and reproduces the
  target tangent-input qword payload `0000004052c1d03f`. The candidate extent
  remains 1051 bytes; this does not repair the error-call model.
- API-only model: 14 real GameErrorContext calls (13 Log, one Fatal) replace
  the facades, producing 1125 bytes / 98 fields. All 14 receiver, format and
  cleanup checks pass. Complete real Log (201 bytes / all 12 fields) and Fatal
  (201 bytes / all 11 fields) peer guards are exact; these are not new D3D credit.
- In the API-only/combined checkpoint, two fewer field sites were localized
  to memory-OR versus load/store and a common post-log load; one fewer branch
  was fallthrough, not missing effects. This historical localization does not
  explain away the later 30/29 branches or waive complete comparison gates.
- Combined model: one build completed with exit 0 at 17:11:22 UTC. All eight
  factor predicates pass: 14 real API edges with receiver/format/varargs/cleanup
  checks, both complete 201-byte peer guards, no facades, and the exact target
  tangent payload. The owner remains 1125/1159 bytes, 98/100 fields and 28/29
  branches, with full-owner exactness false and zero exact credit.
- Stdcall declaration-only model: one trial completed at 17:17:45 UTC with
  exit 0 and all twelve predicates passing. Actual `YGXI` decoration, the
  unchanged sole `FF000000` argument and removal of caller `ADD ESP, 4` are
  observed. The owner is 1122/1159 bytes, 98/100 fields and 28/29 branches;
  previous API edges, both 201-byte guards and tangent payload still pass.
  The explicit unresolved nonexecutable data anchor remains: this does not
  implement or close the real 108-byte callee.
- Byte-flag model: compiled once, then the original postbuild audit failed on
  one unreachable identity-LEA alignment instruction. Original `bool.json`
  and exit 1 are preserved. Separate v03 validation audits all 1150 bytes,
  passes ten flag predicates, rejects four negative controls and preserves
  eleven whole guards (2330 bytes / all 121 fields). It is not a compiler rerun
  or a rewritten successful producer receipt.
- Camera-sign model: completed once at 17:30:01 UTC with exit 0. All 29
  predicates pass, including positive 240 then division/FCHS, exact tangent
  input, three BYTE flag accesses, stdcall cleanup, fourteen API edges and
  eleven whole guards (2330 bytes / all 121 fields). Final owner 1152/1159,
  98/100 fields and 30/29 branches remains nonexact. These are counts/extents,
  not seven mismatching bytes or fractions of bindings passed.
- Fresh ten-TU canonical regression completed at 17:32:16 UTC with exit 0:
  ReleaseSurfaces 43, ServiceScreenshots 111, Present 236, TakeScreenshot 504,
  CaptureToTexture 240 and ScreenshotWorker 302; total 1436 bytes / all 69
  fields. This is separate existing-owner regression, not new exact credit.
  Full target-independent CI subsequently passed all 111 stages at 17:42:01
  UTC, exit 0, with all frozen inputs unchanged. No runtime pass is claimed.

Ignored receipt roots and exact files:
- `.analysis/gpt-dots/d3d-tangent-precision-20261024/`:
  `baseline-audit.json`, `candidate-audit.json`, `baseline.json`,
  `candidate.json`, and their `.log` / `.exit` files.
- `.analysis/gpt-dots/d3d-error-context-api-20261024/`:
  `api-only-owner-audit.json`, `api-only-edge-audit.json`,
  `api-only-independent-replay.json`, `api-only.json`, `api-only.log`,
  `api-only.exit`, and `root-application.json`. API-only source SHA-256:
  `320e4f1c8e2bc4eb98a03359d0d5eb60ac5f37d9bdfd2d579aa01d4375ec17e8`.
- `.analysis/gpt-dots/d3d-api-tangent-combined-20261024/`:
  `combined.json`, `combined-owner-audit.json`, `combined-edge-audit.json`,
  `combined.log`, `combined.exit`, `model.json`, `root-application.json`,
  `api-baseline-replay.json`, and `remaining-localization.json`.
  The completed receipt reports `supported-combined-factors-only`,
  `acceptance_authority=none`, and `exactness_credit=0`.
- `.analysis/gpt-dots/d3d-color-stdcall-20261024/`: `stdcall.json`,
  `stdcall-color-audit.json`, `stdcall-owner-audit.json`,
  `stdcall-edge-audit.json`, `stdcall.log`, and `stdcall.exit`.
  Image SHA-256:
  `2a694826f3268139414bcb34834e08eaedd89799e12127e0a2607f22ca824fcb`.
  The receipt states `supported-declaration-cleanup-only` and
  `color_callee_closure=false`; exactness credit remains zero.
- `.analysis/gpt-dots/d3d-byte-flag-20261024/`: original `bool.json` /
  `bool.exit` and separate `bool-supplemental-validation-v03.json`.
  Supplemental receipt SHA-256:
  `028d15b0f2b5024a194a39f3d048541b3f605f74ae1059ed667dffa505c482d1`.
- `.analysis/gpt-dots/d3d-camera-sign-order-20261024/sign.json`, with
  `sign.log`, `sign.exit` and the complete owner/flag/guard audits.
  Final image SHA-256:
  `248410d9b91a6b23ce38b7626b528e3a78c06ce8d21b32cbc0ae3109c2bd0e06`.
- `.analysis/gpt-dots/d3d-main-regression-20261024/receipt.json`:
  completed six-owner canonical regression; not a full-CI receipt.
- `.analysis/gpt-dots/d3d-checkpoint-20261024/`: `root-ci.log`,
  `root-ci.exit`, `ci-inputs.json`, `post-ci-inputs.log` and
  `checkpoint-validation.json`. All 111 stages passed; source and protected
  inputs were unchanged. Only CI-result documentation was edited afterward
  and rechecked separately.

This pilot is distinct from the stopped adapter-format experiment. Preserve
all nine untracked files under `docs/evidence/d3d-adapter-format/` without
resuming or incorporating that work. The historical five-factor checkpoint
added zero exact bytes and zero source owners. The later real-callee recovery
above separately adds 243 exact callee bytes and two source-present owners.
The D3D parent and runtime/shared-global closure remain open.
