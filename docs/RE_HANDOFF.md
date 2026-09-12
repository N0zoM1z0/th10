# TH10 reverse-engineering handoff

## Phase and authority

This repository remains in the **exact source reconstruction** phase for TH10
v1.00a original Japanese. The private operator-supplied target remains
`resources/th10.exe`; it is ignored, was not modified, copied, relocated, or
staged, and no `TH10_TARGET_PATH` override or `/mnt` search was used.

Accuracy continues to dominate completeness. Source presence, compilation,
normal-COFF exactness, LTCG hypotheses, whole-build closure, runtime validation,
and Factory acceptance are separate states. Ghidra evidence has
`exactness_credit=none`.

## Live recovery state at session entry

The live repository was re-read rather than relying on the prior browser state.
The session started from:

- branch: `main`
- HEAD: `4009080d08242afefcec0d5973c4ecb9003b8e4d`
- subject: `gpt-web: reconstruct TH10 replay loader`
- upstream: `origin/main = c10c371ed3b09451ef1076f6ec1e99d87f174d22`
- relation: ahead 4 / behind 0
- dirty counts: 0 staged / 0 unstaged / 0 untracked / 0 conflicts

The current HEAD was already a clean replay-loader checkpoint, so no interrupted
tracked work required recovery. Preserved ignored/private/generated state was
classified as follows:

- `resources/th10.exe`: private target, preserve/exclude;
- `.tools/`: shared immutable selectors plus mutable Wine/tool state, preserve;
- `ghidra-project/`: provider state, preserve;
- prior `.analysis/`: checkpointed/reproducible/shared/legacy evidence, preserve
  except proven current-session scratch;
- `build/`: generated/open whole-build output, preserve.

Session-entry `.analysis` size was **135,581 bytes**. `build/` was approximately
5.0 MiB. No unrelated or unknown tracked/untracked work was reset, overwritten,
deleted, staged, or incorporated.

## Guidance and mandatory preflight

Before editing, the session fully re-read the repository guidance, both Factory
contracts, all prompt-named Factory docs, Factory
`docs/semantic-reconstruction.md`, and the repository-local
`.agents/skills/th10-reconstruction/SKILL.md`. No required guidance path was
missing.

The mandatory entry preflight was then re-run and passed:

- `python3 scripts/verify-target.py`
- `python3 scripts/verify-toolchain.py --execute`
- `python3 scripts/validate-tracking.py --require-target`
- `python3 scripts/report-reconstruction-status.py`
- `python3 scripts/ci.py`

The target remains:

- size: 487,936 bytes
- SHA256: `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`
- MD5: `7dc488d82c81dd4aee4ba098b8804d83`
- PE i386, 4 sections
- image base `0x00400000`
- entry point `0x004537DC`
- Rich checksum `0x3152A748`
- dominant compiler/linker build 6030

The pinned VC7.1 SP1 execute preflight passed normal-COFF, LTCG, resource, and
PE32-link smoke tests.

`th10-ghidra` operation schemas were rediscovered before use. The first explicit
`check {}` call failed at the Factory transport layer; live repository status was
re-read and remained unchanged before retry. The retry passed with:

- `attestation.status = passed`
- `target_identity_id = target:th10-main`
- `provider_transport = factory-native-command`
- correct target hashes/layout/entry
- `exactness_credit = none`

The entry Truth snapshot refresh was unavailable because another Factory
operation owned `<operator-path>`. No older accepted snapshot is reused as
proof for this packet.

## Hard packet selection

This packet deliberately attacked the connected replay stage-transition owner
rather than a small leaf:

- target extent: `0x0042A450-0x0042A695`
- ledger size: 582 bytes
- entry state: `unknown/review`, no maintained source
- Ghidra: `body_min=0x0042A450`, `body_max=0x0042A695`,
  `body_addresses=579`

The observable goals were to reconcile the 582-versus-579 discrepancy, recover
the private machine boundary, close the record/playback state-copy directions,
identify which existing replay layouts/helpers it owns versus merely calls, and
obtain faithful VC7.1 normal/LTCG feedback without source-shape fishing.

## `0x0042A450-0x0042A695` — maintained `ReplayManager::BeginStage`

### Physical extent and the 579/582 discrepancy

Direct target disassembly closes the physical body at exactly 582 bytes:

- preceding callback ends at `0x0042A44B`;
- `0x0042A44C-0x0042A44F`: CC padding;
- stage-transition body starts at `0x0042A450`;
- record-mode return: `0x0042A5A2`;
- playback/final return: `0x0042A695`;
- `0x0042A696-0x0042A69F`: CC padding;
- independent next code begins at `0x0042A6A0`.

Ghidra's 579 `body_addresses` count is not a shorter function. At
`0x0042A60B` the target jumps to `0x0042A610`, retaining but skipping the
three-byte alignment instruction `lea ecx,[ecx+0]` at
`0x0042A60D-0x0042A60F`. Thus the reachable-CFG count is 579 while the
physical owned extent remains 582 bytes.

### Private machine boundary

Ghidra reports one caller function, `0x00418190`, and target xrefs show two
actual call sites:

- `0x00418278`
- `0x00418477`

Both caller paths load `EBX` from global replay-owner storage `0x00477838`
immediately before the stage-transition call sequence. The target function uses
no stack arguments and returns with plain `RET`.

Maintained machine-boundary description:

**live-in EBX = ReplayManager owner / RET**

This is an optimizer/private ABI observation, not a source-level calling-
convention claim. The maintained source uses an ordinary no-argument member
method `ReplayManager::BeginStage()`.

### Direct callee cohort

The target body calls exactly five modeled helpers:

- `0x0042AA50`: allocates/initializes/inserts one 0x6284 replay frame block;
- `0x0042AB20`: unlinks/frees all frame blocks for one stage;
- `0x00428E10`: restores the two stage position values into runtime state and
  marks four runtime slots dirty;
- `0x00426F70`: large runtime/player rebuild owner;
- `0x00424D90`: runtime reset owner.

The frame-list behavior had already been independently recovered and is used by
maintained `ReplayAllocateFrameData` / `ReplayClearStageFrameData` interfaces.
The latter three target candidates remain **unknown/review** in this packet.
Descriptive call interfaces were added only so the reviewed stage owner can be
written naturally; those declarations do not promote helper origin, source/TU,
physical ownership, or exactness.

### Record mode

After setting flag bit 1 on each non-null maintained replay chain element, mode
0 performs the current-stage transition:

1. Select `stageHeaders[g_ReplayCurrentStage]`.
2. Clear all existing 0x6284 frame blocks for the stage.
3. Allocate a new frame block and store its list node as `currentFrameLink`.
4. When the target stage-mode flag is zero, refresh stage-header fields
   `+0x0C/+0x10/+0x14/+0x18/+0x1C/+0x20/+0x1B4` from the already reviewed
   replay globals.
5. Always copy runtime `+0x3CC/+0x3D0` to stage header `+0x24/+0x28`.
6. Copy exactly 0x108 bytes from runtime `+0x436C` to stage header `+0x2C`.
7. For four runtime slots at `runtime + 0x32D4 + i*0x98`, snapshot four pairs:
   - slot `+0x00/+0x04` -> header `+0x134 + i*8`;
   - slot `+0x08/+0x0C` -> header `+0x154 + i*8`;
   - slot `+0x10/+0x14` -> header `+0x174 + i*8`;
   - slot `+0x18/+0x1C` -> header `+0x194 + i*8`.
8. Store the reviewed `+0x1B8` replay global, set `activeStage`, and copy runtime
   `+0x4474` to stage header `+0x1BC`.
9. Reset replay `frameCounter` to zero.

### Playback mode

Mode 1 selects `stageStates[g_ReplayCurrentStage].header`, sets `activeStage`,
and performs the inverse state restoration:

1. Restore header `+0x24/+0x28` through target helper `0x00428E10`.
2. Copy the 0x108-byte block from stage header `+0x2C` back to runtime
   `+0x436C`.
3. Restore runtime `+0x4474` from stage header `+0x1BC`.
4. Call the connected large rebuild owner `0x00426F70`.
5. Restore the four groups of four 8-byte pairs into the four 0x98-byte runtime
   slots.
6. For the target condition `shotType + character * 3 == 5`, override each
   slot's `+0x18/+0x1C` pair with the first pair (`+0x00/+0x04`) from the
   stage header. The character/shot interpretation is independently supported
   by the reviewed initializer's replay-data `+0x50/+0x54` stores.
7. Clear every runtime slot `+0x58` reset field.
8. Call target reset owner `0x00424D90`.
9. Reset replay `frameCounter` to zero.

Other mode values only reach the common frame-counter reset after the chain-flag
updates.

## Maintained source and layout refinement

Updated:

- `src/ReplayManager.hpp`
- `src/ReplayManager.cpp`

Added maintained member:

`void ReplayManager::BeginStage();`

The existing 0x1C4 `ReplayStageDataHeader` size is unchanged. Its formerly
opaque `+0x02C..+0x1B3` region is refined only to target-observed granularity:

- `runtimeSnapshot[0x108]` at `+0x02C`;
- four 4-entry arrays of 8-byte pairs at `+0x134/+0x154/+0x174/+0x194`;
- `unknown1BC` as the observed dword at `+0x1BC`.

A local maintained `ReplayStageRuntime` view exposes only offsets directly used
by this target body:

- coordinates at `+0x3CC/+0x3D0`;
- four 0x98-byte slots beginning `+0x32D4`, with reset field `+0x58`;
- 0x108-byte replay snapshot at `+0x436C`;
- one dword at `+0x4474`.

The original runtime type, member names, global-data owner, and source/TU owner
remain unknown. The source contains no target addresses, target bytes, fake
returns, inert padding, inline assembly, or private-register calling-convention
tricks.

## Adjacent-game hypothesis discipline

Adjacent repositories were consulted only after the TH10 boundary, ABI, and
state-copy behavior were established.

TH095:

- committed HEAD `a7e340d3ff00e81b56584bc03288953f754b1faf`
- branch `main`, observed ahead 6 / behind 0
- unrelated untracked files existed:
  `EnemyManagerUpdate.i`, `config/runtime-scenarios.json`, `droid.resume.txt`,
  `scripts/runtime-diff.py`
- only committed ReplayManager content was read
- it did not provide a direct equivalent for this merged TH10 stage-transition
  owner.

TH09:

- committed HEAD `d5f16f39f24fa51728e7e458bbe83bd16a4219b0`
- branch `main`, observed ahead 3 / behind 0
- unrelated tracked modification existed at `src/AsciiManagerMenu.cpp`
- only committed `HEAD:src/ReplayInput.cpp` was read
- its separate `BeginRecordingStage` / `BeginPlaybackStage` methods corroborate
  the stage-start source family and motivated the neutral maintained TH10 name
  `ReplayManager::BeginStage` after TH10-local recovery.

TH08 was not materially consulted for this packet. No adjacent address, extent,
layout, ABI, ownership, exactness, completion, or uncommitted content was
transferred.

## VC7.1 SP1 diagnostic feedback

Fixed normal profile:

`/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc`

The same tracked `src/ReplayManager.cpp` compiles successfully under both the
fixed normal profile and the same profile with `/GL`.

Normal symbol:

`?BeginStage@ReplayManager@@QAEXXZ`

Its actual normal `/Gy` COFF symbol is in 1-based section 19, corresponding to
objdump section index 18. The true owned section extent is:

**0x415 = 1,045 bytes**

Target physical extent:

**582 bytes**

Thus fixed normal COFF is not a complete physical match. A separate 582-byte
`compare-coff-function.py` diagnostic gives:

- result: `mismatch`
- comparable bytes: 506
- matched comparable bytes: 24
- acceptance authority: `none`

The comparator's `object_size=582` is the caller-supplied diagnostic window, not
the true object-owned section size. No source-shape fishing was performed after
this decisive mismatch. `/GL` compilation preserves possible LTCG/interprocedural
context, but no target-bound linked-image extent workflow proves its physical
owner or exactness.

No canonical match unit or exact row was added.

## Ledger result

Entry state:

- candidates: 1,236
- pending: 1,144
- reviewed authored: 85 / 18,934 bytes
- exclusions: 7
- source-present: 55
- canonical exact: 0

Current packet state after tracked edits:

- candidates: **1,236**
- pending: **1,143**
- reviewed authored: **86 / 19,516 bytes**
- exclusions: **7**
- source-present: **56**
- canonical exact: **0 functions / 0 bytes**
- canonical normal-COFF match units: **0**

Packet delta:

- candidates: +0
- pending: -1
- reviewed authored: +1 / +582 bytes
- source mappings: +1
- exclusions: +0
- exact: +0

`0x0042A450` is now `authored_game`, subsystem `Replay`, mapped to
`src/ReplayManager.cpp` as maintained `ReplayManager::BeginStage`.

The directly called `0x00428E10`, `0x00426F70`, and `0x00424D90` candidates are
unchanged and remain `unknown/review`.

No changes were made to:

- `config/matches.csv`
- `config/match-units.toml`
- `config/build.toml`

## Cold validation completed before checkpoint

The final tracked source state was rebuilt from scratch in a temporary directory
and again passed normal and `/GL` compilation. The true normal BeginStage section
again measured 1,045 bytes and the target diagnostic again reproduced the exact
24/506 mismatch.

The following checks passed on the final pre-checkpoint tracked state:

- `python3 scripts/verify-target.py`
- `python3 scripts/verify-toolchain.py --check`
- `python3 scripts/validate-tracking.py --require-target`
- `python3 scripts/progress.py --check`
- `python3 scripts/build-match-unit.py --check`
- `python3 scripts/build.py --check`
- `python3 scripts/report-reconstruction-status.py`
- `python3 scripts/ci.py`
- `git diff --check`

The honest `python3 scripts/build.py` invocation returned expected **RC=2**:
compile flags, translation-unit partition, production libraries/resources, and
link order remain unknown. Whole Windows i386 build closure therefore remains
**open**.

Current independent states:

- source presence for `0x0042A450`: reviewed/present;
- canonical exactness: none;
- normal-COFF physical match: no;
- LTCG physical owner/exactness: unknown;
- whole Windows i386 build: open;
- runtime validation: not started;
- semantic reconstruction / ports: not started;
- Factory acceptance for this packet: not observed.

## `.analysis` lifecycle

Session entry `.analysis`: **135,581 bytes**.

Current campaign:

`.analysis/gpt-web/20260913-replay-stage-transition/`

Peak after normal/LTCG objects, detailed comparator output, compact report, and
manifest: **213,188 bytes**.

After compacting evidence and deleting only reproducible current-session
objects/detailed comparator scratch, `.analysis` measured **141,577 bytes**.
The campaign currently retains only:

- `compiler-shape-report.txt` — 5,132 bytes
- `manifest.json` — current ignored campaign metadata

No current-campaign file exceeded 64 MiB. No legacy/shared analysis state,
private target, Ghidra project, toolchain, Wine prefix, or another process's
artifact was removed.

## Transport/recovery events

Factory transport failures occurred during:

- the first live repository-status attempts at session start;
- the first mandatory Ghidra `check {}`;
- the initial Truth snapshot refresh (shared operator lock rather than a result);
- one combined adjacent-repository query;
- one TH09 committed-source read;
- initial campaign-manifest creation;
- one small mapping-format read.

For every failed or potentially persistent command, live repository status was
re-read before retry. No unclassified filesystem or Git side effect was found.
The failed first campaign-manifest write was explicitly checked and had not
created the campaign directory before retry.

## Next preferred hard packet

Primary next packet:

**`0x00426F70-0x0042792D` — 2,494-byte runtime/player rebuild owner, currently
`unknown/review`.**

Why this is the preferred continuation rather than an easy leaf:

- it is a direct callee of reviewed `ReplayManager::BeginStage` playback restore;
- Ghidra currently reports 2,471 body addresses against the 2,494-byte ledger
  span, so its physical boundary/unreachable-code ownership still needs explicit
  reconciliation;
- it has five modeled callers: `0x00418190`, `0x0041AFD0`, `0x004247F0`,
  `0x00425730`, and this packet's `0x0042A450`;
- it rebuilds multiple 0x98-byte runtime/player slots and interacts with object
  lists/resource creation, making it materially more central and structurally
  difficult than the adjacent replay leaves.

Next packet goals:

1. Re-run mandatory recovery/preflights and fresh Ghidra attestation.
2. Reconcile `0x00426F70-0x0042792D` entry/tails/padding and the 23-byte
   ledger-versus-CFG-address difference using raw target disassembly.
3. Recover every real caller ABI and determine whether the target boundary is a
   conventional stack parameter or optimizer-private owner path.
4. Separate runtime/player-owned state from called resource/object-manager
   behavior; do not transfer replay ownership merely because BeginStage calls it.
5. Use TH10-local offsets/control flow before consulting committed adjacent game
   source for naming/source shape.
6. Compile a natural source hypothesis under fixed normal and `/GL`; keep
   exactness unknown unless a complete canonical target-bound normal-COFF unit
   reaches repeatable zero difference.

Secondary connected routes are `0x00424D90-0x00424EA0` (273-byte runtime reset
owner) and `0x0042A6A0-0x0042A803` (356-byte replay-adjacent body with a separate
caller), but neither should displace the 2,494-byte central owner merely because
it is smaller.

## Checkpoint closeout

The source/ledger packet was checkpointed locally as:

- initial checkpoint: `d2316fbb848be9ffaa1d84db19bf37451051b750`
- subject: `gpt-web: reconstruct TH10 replay stage transition`
- branch after the initial checkpoint: `main`
- upstream: `origin/main = c10c371ed3b09451ef1076f6ec1e99d87f174d22`
- relation after the initial checkpoint: ahead 5 / behind 0
- dirty counts after the initial checkpoint: 0 staged / 0 unstaged / 0 untracked / 0 conflicts
- pushed by this session: **false**

The committed source was then recompiled from scratch under both the fixed normal
profile and the same `/GL` profile. The normal `BeginStage` COMDAT remained
1,045 bytes, the target diagnostic remained the same 24/506 mismatch with
`acceptance_authority=none`, all target/toolchain/tracking/progress/match-graph/
build-graph/status/public-CI checks passed, and the honest whole-build invocation
again returned expected RC=2/open. The committed file set remained exactly the
10 intended packet paths.

The first post-checkpoint Ghidra refresh failed at the Factory transport layer.
Live repository status was re-read and remained the same clean checkpoint before
retry. The retry passed with exact `target:th10-main`,
`factory-native-command`, and `exactness_credit=none`, observed at
`2026-09-12T18:56:58.454584Z`.

A post-checkpoint Truth Kernel snapshot refresh then succeeded. Its accepted
source commit remained `9f7886f72adcd1880c05e34c62408cf9b1dad966` and the registry contained only
one accepted fact/claim: `target_attested:target:th10-main`. It contained no
accepted boundary, source-presence, compile, ownership, codegen, exactness,
whole-build, or runtime claim for this replay-stage-transition packet. This
snapshot is therefore recorded only as an independent acceptance-plane
observation, not as packet acceptance.

At that point `.analysis` measured 141,577 bytes. The packet campaign retained
only its compact compiler/ABI report and ignored manifest; all current-session
objects and detailed comparator scratch had already been removed as reproducible
current-session artifacts.

This handoff closeout is the only tracked post-checkpoint edit. It is amended
into the same logical local checkpoint; the authoritative amended commit hash is
recorded in the ignored campaign manifest and the operator-facing final report,
rather than self-referencing it inside this tracked file. No source, ledger,
exactness, build-graph, or packet conclusion changes during that amend. Nothing
is pushed.
