# D3D exact-first pilot, 2026-10-24

## Scope and acceptance boundary

This packet records the baseline and bounded tangent, API, combination,
stdcall declaration, byte-flag and camera-sign trials for InitD3DRendering.
The byte-flag native build completed, but its original postbuild audit failed;
a separate existing-image supplemental validation is recorded without rewriting
that failure. It follows the [exact-first method](../../TH10_EXACT_FIRST_METHOD_20261024.md);
it is not the stopped adapter-format experiment.

The target is original Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Its complete reviewed owner is 1159 bytes, with 100 address fields and 29 local
branches. Each candidate is independently decoded and inventoried from its
PE/MAP/PDB-owned contribution; shifted candidate offsets are not interpreted
using target field positions.

Every trial and supplemental validation grants zero exactness credit. The
final five-factor source produces **1152 versus 1159 bytes and is nonexact**,
with 98/100 address fields and 30/29 local branches. These are extents and
inventory counts, not seven mismatching bytes or fractions of bindings passed.
No ordinary-byte exact score,
new exact ledger row, new source-present owner, native product, data ownership
or runtime claim is established. Complete extent, semantic-field, fixed local
target, callee and cold-replay gates in [ORACLES.md](../../ORACLES.md) remain intact.

## Fixed context and completed experiments

The freeze files identify source checkpoint
`1e357372d88a9762b45feabfbc9f553cc24cdad3`, actual `WinMain@16` entry,
`src/Main.cpp` primary and `src/GameErrorContext.cpp` support. They retain the
VC7.1 build-6030 LTCG lane with primary/support switches
`/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL /GS`.
The diagnostic unresolved-symbol anchors do not make the image a runnable
product or prove original production TU/profile ownership. No VC8 flags,
forced register choices, fake ABI, inert padding or comparator relaxation are
part of these bounded trials.

| Trial | Candidate bytes / target bytes | Fields / target fields | Local branches / target branches | Result |
| --- | --- | --- | --- | --- |
| Fresh baseline | 1051 / 1159 | 84 / 100 | 28 / 29 | Structural rejection; wrong tangent input and facade error calls |
| Tangent-only | 1051 / 1159 | 84 / 100 | 28 / 29 | Target tangent input recovered; owner still rejected |
| API-only | 1125 / 1159 | 98 / 100 | 28 / 29 | Real error-context edges recovered; tangent input unchanged and wrong |
| Combined API/tangent | 1125 / 1159 | 98 / 100 | 28 / 29 | All eight factor predicates pass; whole-owner exactness remains false |
| Stdcall declaration-only | 1122 / 1159 | 98 / 100 | 28 / 29 | All twelve predicates pass; actual color callee closure remains false |
| Byte-flag | 1150 / 1159 | 98 / 100 | 30 / 29 | Original postbuild audit failed; separate v03 existing-image validation passes |
| Camera-sign order | 1152 / 1159 | 98 / 100 | 30 / 29 | All 29 predicates pass; whole-owner exactness remains false |

### Tangent-only model

The one source change is the float suffix in `3.1415927f / 12.0`.
The division operand, `tan(double)` call, output cast and surrounding division
remain unchanged. The target loads the double input value
0.2617993950843811, represented by the semantic qword payload
`0000004052c1d03f`; the completed candidate reproduces it.

This repairs an actual constant-precision observation, not owner layout.
The complete candidate code hash is identical to the baseline code hash:
`928a14da02fed0ebb95b25edbadedd9fb4186ee5127bd3dc30620c34618d6cfb`.
The constant payload differs outside that code contribution. Code identity
between two candidates is not target equality or proof of all data ownership.
The receipt labels the result `supported-qword-only`.

The source was restored to the frozen baseline before the API-only trial.
`root-factor-reset.json` preserves both the tangent candidate and restored
baseline identities; the factors were not silently conflated.

### API-only model and independent guards

Fourteen facade calls become the actual GameErrorContext members: thirteen Log
calls and one Fatal call. Target bytes and branch roles pair each format
identity; the audit checks the real entry, context receiver, variadic payload
shape and cleanup at all fourteen sites. There are no facade edges left.
Names alone do not establish the edge identities.

The complete real Log guard is exact across 201 bytes and all 12 fields; the
complete real Fatal guard is exact across 201 bytes and all 11 fields. These
peer checks independently establish those bounded guard results, not an exact
InitD3DRendering parent or new credit for already retained callees.
`api-only-independent-replay.json` repeats the API-edge review without compiling.
The API-only receipt labels the result `supported-api-edges-only`.

### Combination

The combined source adds only the already tested tangent suffix to the API-only
source. Its completed build/diagnostic ends at 17:11:22 UTC with producer exit 0.
All eight predicates pass: fourteen real entries, fourteen context receivers,
fourteen format/varargs/cleanup checks, no facades, both complete peer guards,
the baseline tangent mismatch and the corrected target-equal tangent payload.

The combined code contribution remains raw-identical to the API-only owner:
`8feca35ea84fa29ca9c22affe1ddbc4b46426ac89cb394b4741a3fec524c31e4`.
Its receipt says `supported-combined-factors-only`, not owner acceptance.

### Subsequent stdcall declaration-only model

A separate one-declaration change marks MainSetScreenColor as `__stdcall`;
it does not implement or replace the real 108-byte target callee. The root trial
completed once at 17:17:45 UTC with exit 0. Actual `YGXI` decoration is present,
the sole `FF000000` color argument is unchanged, and the caller's extra
`ADD ESP, 4` is absent. The candidate remains 1122 versus 1159 bytes, with
98/100 fields and 28/29 local branches.

All twelve predicates pass, including the previous fourteen real API edges,
both complete 201-byte Log/Fatal guards and target-equal tangent payload.
The destination remains an explicit unresolved nonexecutable data anchor,
not an implemented or accepted real callee. The receipt states
`supported-declaration-cleanup-only` and `color_callee_closure=false`.
This is a supported caller-declaration correction, not full callee closure
or whole-owner exactness.

### Byte-flag model: original failure and separate validation

The renderer-selection local changes from int to bool. The native image was
compiled once and owns 1150 bytes; the original postbuild reachability assertion
then failed on one unreachable identity-LEA alignment instruction.
The original `bool.json` remains `completed=false`, with `bool.exit=1`.
It is not relabeled as a successful producer and no second native compile was
used to hide the audit failure.

The separate `bool-supplemental-validation-v03.json` replays that existing image.
It accounts for all 1150 bytes, validates all ten flag predicates and all three
BYTE accesses to the same local, and preserves both return stack balances.
The skipped identity-LEA is accepted only as specifically verified alignment.
Four in-memory negative controls reject a nonidentity LEA, incoming edges to
its start or interior, and unaccounted unreachable code. No compared bytes are
discarded or masked, and the whole owner remains nonexact.

All eleven complete same-image guards remain exact: 2330 bytes / all 121 fields.
This is guard replay in the diagnostic graph, not canonical graph recertification
or new exact credit. Previous API, tangent and declaration observations remain.

### Camera-sign order and final measured source

The separate sign-expression trial compiled once and completed at 17:30:01 UTC
with exit 0. It recovers positive 240 followed by division and FCHS, rather than
moving the sign into the dividend. All 29 predicates pass, including unchanged
target tangent input, no forced tangent float spill, three BYTE flag accesses,
stdcall caller cleanup, fourteen real API edges, and all eleven complete guards
(2330 bytes / all 121 fields). Unknown x87 control-word and runtime consequences
remain outside this observation.

The final source SHA-256 is
`3bbb030a9d3f9d1961ebbfcfbccb5c2dbf080cf9dc68f8d129f9362d7cfb8de7`.
Its parent is 1152/1159 bytes with 98/100 fields and 30/29 branches.
The receipt says `supported-negate-after-division-only`; full-owner exactness
and actual color callee closure remain false.

## Localized remaining differences

The read-only `remaining-localization.json` is bound to API-only source and
image, not silently repinned to the combination. The identical API-only and
combined code hashes explain why it applies to that historical pair. It is not
silently promoted into a complete explanation of the later byte-flag/sign
owners, whose 30-versus-29 branch inventory remains nonexact.

- The initial flags update uses a target load/OR/store but one candidate memory
  OR. Both execute one flags read and write; the candidate has one fewer static
  address field.
- Target reference/software success paths each load flags after their Log.
  The candidate shares that post-log load between the paths. Exactly one load
  executes on either path; no intervening call or flags write was found. This
  accounts for the other field-count reduction.
- After setting disableVsync on the nonzero relative-path branch, the target
  jumps to immediate-mode setup; the candidate falls through to it. The
  alternative branch retains its condition. This accounts for the one fewer
  static branch, not a missing API failure path.
- The Fatal/null-test/Release/store-null/return-one block is present in both
  images but placed differently. Its movement is not the missing branch or
  either missing field site.

These are dataflow/CFG explanations, not a license to normalize changed local
offsets, remove compared bytes or infer full semantic equivalence. Every
ordinary byte and complete target contract still has to pass.

## Source, image and receipt identities

The following SHA-256 identities bind historical artifacts, not whichever
source happens to be dirty later. The producer JSON also retains the freeze
identity, native command/exit logs, PE/MAP/PDB bindings and artifact hashes.

### Fresh baseline

- Source SHA-256:
  `9cce8257321bde23fdb88b6f42f7412a3b6a425a138f7bda0482796fd300cd8e`.
- Image SHA-256:
  `ada06fa89e9cd1f14d8fdc2f893b2ac48d610c149ed69cb5264261d3017f97d6`.
- Producer receipt: `.analysis/gpt-dots/d3d-tangent-precision-20261024/baseline.json`.
- Receipt SHA-256:
  `b7476f693dcc2362e01cb6fe0d1dbd24d248860889382b8370d6ae69c7e10dda`.

### Tangent-only

- Source SHA-256:
  `e42b6a133303f0ac955d2d5dce1346f54d18b2ac2603cfa9f691f24602b26679`.
- Image SHA-256:
  `6b4732ff75b26d73ca31da245a5f8fab67d7f5c3c2be8a4449fbd1b797d6d457`.
- Producer receipt: `.analysis/gpt-dots/d3d-tangent-precision-20261024/candidate.json`.
- Receipt SHA-256:
  `ded00e1c05e99e890d4feb960b95178712089eb3ed646bf4e02563419d20f272`.

### API-only

- Source SHA-256:
  `320e4f1c8e2bc4eb98a03359d0d5eb60ac5f37d9bdfd2d579aa01d4375ec17e8`.
- Image SHA-256:
  `310d3d2a523bd7e5c4594eb597e40f06fc981919aa441b204ecabc9aa2c9b00f`.
- Producer receipt: `.analysis/gpt-dots/d3d-error-context-api-20261024/api-only.json`.
- Receipt SHA-256:
  `6c5bdd6576895305792b5420229579e1c0e96ddaa4551a2a069ba9b5bee26e97`.

### Combined API/tangent

- Source SHA-256:
  `cc07328ee32de6e91092f80a4df816270e99f98c008abe05d525d1187b057fe2`.
- Image SHA-256:
  `7c0b6ab41f9486de8af5794f2798509cb16d8afc1943332912bde37d3921fad8`.
- Producer receipt: `.analysis/gpt-dots/d3d-api-tangent-combined-20261024/combined.json`.
- Receipt SHA-256:
  `580eab44d6b8a4c91e0fee3e0411fa6060a2feb331fee7538c772e8994a5c421`.

### Stdcall declaration-only

- Source SHA-256:
  `a3a84959b99c6480c7764dc21cfcdf7ac168c740e924051b4fda91744bcc84ee`.
- Image SHA-256:
  `2a694826f3268139414bcb34834e08eaedd89799e12127e0a2607f22ca824fcb`.
- Producer receipt: `.analysis/gpt-dots/d3d-color-stdcall-20261024/stdcall.json`.
- Receipt SHA-256:
  `6e3e4ab258266e2b597339b2eef29ed596968c5c544eb79d4138d7dd5e28e5a2`.

### Byte-flag build and supplemental validation

- Source SHA-256:
  `4d5eb653eba01986023334cabfd3bac6bbcaa12b0346d0487d41e18767661a0a`.
- Image SHA-256:
  `a3e3c47398fa951d383eaa91fcf0f6b8089fb84d5c47b58af99d835f096ec460`.
- Original failed producer: `.analysis/gpt-dots/d3d-byte-flag-20261024/bool.json`.
- Original failed receipt SHA-256:
  `a7e22da971d66902c99f49efebd64c4592a066c7331e81734f88045d767d78e7`.
- Separate validation:
  `.analysis/gpt-dots/d3d-byte-flag-20261024/bool-supplemental-validation-v03.json`.
- Supplemental receipt SHA-256:
  `028d15b0f2b5024a194a39f3d048541b3f605f74ae1059ed667dffa505c482d1`.

### Camera-sign order

- Source SHA-256:
  `3bbb030a9d3f9d1961ebbfcfbccb5c2dbf080cf9dc68f8d129f9362d7cfb8de7`.
- Image SHA-256:
  `248410d9b91a6b23ce38b7626b528e3a78c06ce8d21b32cbc0ae3109c2bd0e06`.
- Producer receipt: `.analysis/gpt-dots/d3d-camera-sign-order-20261024/sign.json`.
- Receipt SHA-256:
  `508df5f3f371401a55f1bd275d0f58ccab00bafe80267b28f6b1080455577322`.

## Retained evidence and reproduction boundary

All paths below are ignored working evidence, not files supplied by a Git clone:

- `.analysis/gpt-dots/d3d-tangent-precision-20261024/`: `freeze.json`,
  `HYPOTHESIS.md`, `produce.py`, `audit.py`, `baseline.json`,
  `candidate.json`, `baseline-audit.json`, `candidate-audit.json`,
  `root-factor-reset.json`, and the corresponding logs/exit files.
- `.analysis/gpt-dots/d3d-error-context-api-20261024/`: `api-freeze.json`,
  `model.json`, `produce_api.py`, `owner_inventory.py`, `audit_api.py`,
  `api-only.json`, `api-only-owner-audit.json`, `api-only-edge-audit.json`,
  `api-only-independent-replay.json`, and logs/exit files.
- `.analysis/gpt-dots/d3d-api-tangent-combined-20261024/`:
  `combined-freeze.json`, `model.json`, `produce_combined.py`,
  `combined.json`, `combined-owner-audit.json`, `combined-edge-audit.json`,
  `api-baseline-replay.json`, `remaining-localization.json`,
  `combined.log`, `combined.exit`, and `root-application.json`.
- `.analysis/gpt-dots/d3d-color-stdcall-20261024/`: `color-freeze.json`,
  `model.json`, `produce_color.py`, `audit_color.py`, `stdcall.json`,
  `stdcall-color-audit.json`, `stdcall-owner-audit.json`,
  `stdcall-edge-audit.json`, `stdcall.log`, `stdcall.exit`, and
  `root-application.json`; the freeze records its exact artifact directory.
- `.analysis/gpt-dots/d3d-byte-flag-20261024/`: original `bool.json`,
  `bool.exit`, `bool.log`, `flag-freeze.json`, `bool-owner-audit.json`,
  `bool-supplemental-validation-v03.json`, `bool-flag-audit-padding-v03.json`,
  `bool-guards-replay-v02.json`, `padding-v03-controls.json`, and their auditors.
- `.analysis/gpt-dots/d3d-camera-sign-order-20261024/`: `sign-freeze.json`,
  `sign.json`, `sign-owner-audit.json`, `sign-operation-audit.json`,
  `sign-flag-audit.json`, `sign-guards.json`, `sign.log`, and `sign.exit`.

Native artifacts are below the matching `build/gpt-dots/` roots, in
`d3d-tangent-precision-20261024/baseline` and `candidate`,
`d3d-error-context-api-20261024/api-only`, and
`d3d-api-tangent-combined-20261024/combined`.
Use the retained producer/freeze pair and explicit artifact directory. Do not
overwrite historical artifacts or refresh source pins to make stale images
appear current. The coordinator owns any new serial, one-CPU, nice-10 build.

The first five producer exit files and the sign trial exit file are zero.
The byte-flag producer exit remains one after its postbuild assertion; its
separate supplemental receipt does not overwrite that history. Intermediate
unresolved-symbol link attempts are retained before the final diagnostic link
succeeds; this is not a claim that every native command returned zero.
The receipts prove only their stated diagnostic scopes.

## Canonical regression and remaining work

The separate fresh ten-TU canonical regression completed at 17:32:16 UTC with
exit 0 against the final source. All six intact owner contracts pass:
ReleaseSurfaces 43, ServiceScreenshots 111, Present 236, TakeScreenshot 504,
CaptureToTexture 240, and ScreenshotWorker 302 bytes; total 1436 bytes / all
69 fields. Receipt:
`.analysis/gpt-dots/d3d-main-regression-20261024/receipt.json`.
These existing-owner regression results are separate from the eleven same-image
guards; do not sum overlapping scopes or claim new exact bytes.

Full target-independent CI passed all 111 stages at 17:42:01 UTC with exit 0
against final Main source `3bbb030a9d3f9d1961ebbfcfbccb5c2dbf080cf9dc68f8d129f9362d7cfb8de7`.
The post-run hash check confirms every frozen source, documentation and protected
input remained unchanged. Receipts are under
`.analysis/gpt-dots/d3d-checkpoint-20261024/`: `root-ci.log`, `root-ci.exit`,
`ci-inputs.json`, `post-ci-inputs.log` and `checkpoint-validation.json`.
Only these CI-result documentation paragraphs changed afterward; documentation
hygiene, links, progress freshness and whitespace were rechecked. This does not
establish a runnable native product or new exact bytes.

The next planned work is real 108-byte and 135-byte callee closure. Establish a
fresh three-TU graph-only baseline before implementing those bodies. In
particular, the declaration-only color correction still points to a
nonexecutable data anchor; it cannot supply the actual callee's behavior.
No new source-present or exact claim exists for those planned callees.
Consult the [maintained handoff](../../RE_HANDOFF.md) for later results.

All nine untracked files under `docs/evidence/d3d-adapter-format/` belong to a
distinct stopped experiment. This packet neither edits them nor resumes,
incorporates or borrows their unfinished claims. Historical artifact directions
are not current routing instructions.
