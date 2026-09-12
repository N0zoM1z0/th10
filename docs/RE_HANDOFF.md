# TH10 exact reconstruction handoff

## Session status

Campaign remains **active / incomplete**. This session reviewed the shared replay initializer hard packet and deliberately expanded the replay denominator with six Ghidra-missed raw bodies. No canonical exact function, whole-product closure, runtime closure, semantic reconstruction, or port readiness is claimed.

Repository selection is `th10`; analysis provider is `th10-ghidra`; target identity is `target:th10-main`. The private operator-supplied target remains `/home/pentester/coding/codex_ida/th10-reconstruction/th10/resources/th10.exe` and was not modified, replaced, moved, or committed.

## Recovery state

Session starting HEAD: `2054cfff86b6cb3d3e42af29e4e4678d984cdb97`

Starting branch/upstream state:

- branch: `main`
- upstream: `origin/main`
- relation: ahead 2 / behind 0
- staged: 0
- unstaged: 0
- untracked: 0
- conflicts: 0

The live starting checkpoint already contained the completed `gpt-web: reconstruct TH10 replay lifecycle` work. The tracked handoff in that checkpoint still described its own pre-commit audit state, but the repository itself was clean and the replay-lifecycle commit was present, so there was no recoverable dirty work.

Preserved ignored/private/generated state:

- `resources/th10.exe`: private target; preserve and exclude.
- `.tools/`: shared tool selectors and Wine state; preserve.
- `ghidra-project/`: target-bound provider state; preserve.
- `build/`: generated/open build skeleton output; preserve.
- pre-existing `.analysis/` campaigns: preserve unless proven current-session scratch.

No unrelated or unknown tracked/untracked work was reset, overwritten, deleted, or staged.

## Guidance and mandatory preflight

Before edits, the session fully reread through the repository runner:

- `AGENTS.md`
- `docs/RE_HANDOFF.md`
- `docs/RE_WORKFLOW.md`
- `docs/ORACLES.md`
- `docs/TOOLS.md`
- Factory `gpt-web-reconstruction-session-v5.json`
- Factory `worktree-recovery-and-analysis-artifacts-v1.json`
- Factory `ontology.md`
- Factory `verification-planes.md`
- Factory `agent-autonomy.md`
- Factory `new-game-bootstrap.md`
- Factory `worktree-recovery-and-analysis-artifacts.md`
- Factory `semantic-reconstruction.md`
- `.agents/skills/th10-reconstruction/SKILL.md`

One guidance-page transport failure occurred while paging durable read-only output; live repository status was re-read before resuming. No required guidance path was missing.

Mandatory entry preflight passed on the live starting checkpoint:

- `python3 scripts/verify-target.py`
- `python3 scripts/verify-toolchain.py --execute`
- `python3 scripts/validate-tracking.py --require-target`
- `python3 scripts/report-reconstruction-status.py`
- `python3 scripts/ci.py`

Target identity remained:

- size: 487,936 bytes
- SHA-256: `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`
- MD5: `7dc488d82c81dd4aee4ba098b8804d83`
- PE32 i386, four sections
- image base: `0x00400000`
- entry: `0x004537DC`
- Rich checksum: `0x3152A748`
- dominant build: 6030

The pinned VC7.1 SP1 toolchain execute smoke passed normal C/C++ COFF, C++ LTCG, resource compilation, and PE32 i386 linking.

Ghidra operation schemas were rediscovered before target-dependent work. Mandatory `check {}` passed with:

- `attestation.status=passed`
- `target_identity_id=target:th10-main`
- `provider_transport=factory-native-command`
- target hash/layout/entry correct
- `exactness_credit=none`

Every useful analysis result in this packet carried the same passed target/native-transport attestation and no exactness credit.

## Packet selection

Hard packet: **shared replay initializer and directly connected raw callback context**.

Primary reviewed extent:

- `0x00428F60-0x0042949F` — 1,344 bytes — previously `unknown/review`

This was selected because it is the central mode/path owner called by reviewed `ReplayManager::Create`, controls recording versus playback versus load-only initialization, publishes the global replay owner, owns initial stage/frame/header setup, restores load-side state, and installs three callback chains. It is materially larger and more ownership-sensitive than an easy leaf. The previous `0x00429240` subfunction hypothesis is invalid; current target analysis proves `0x00428F60-0x0042949F` is one physical function.

Observable outcomes sought were complete physical boundary, machine ABI, mode/path behavior, replay/header layout refinements, chain-element layout, callback entry routing, natural maintainable source, early fixed normal/LTCG compiler feedback, and denominator expansion where raw target code was missed by Ghidra.

## `ReplayManager::Initialize` target result

Physical target extent is closed:

- previous body ends at `0x00428F59`
- `0x00428F5A-0x00428F5F`: six bytes of `CC` padding
- initializer: `0x00428F60-0x0042949F`, exactly 1,344 bytes
- final instruction at `0x0042949F`: plain `RET`
- reviewed `ReplayManager::~ReplayManager` begins immediately at `0x004294A0`

The sole caller in reviewed `ReplayManager::Create` proves the private machine boundary:

- EBX: `ReplayManager` owner
- EAX: replay mode
- ECX: replay path
- no stack argument consumed by the callee
- plain `RET`

The maintained C++ remains the natural `int ReplayManager::Initialize(int mode, const char *path)`. EBX/EAX/ECX are machine/interprocedural observations, not a source calling-convention claim.

### Mode 0: recording

Target behavior:

1. stores mode at manager `+0x10`
2. publishes the manager through global replay-owner storage
3. clears current-stage frame-list storage and allocates a new `0x6284` frame block
4. allocates/zeros a `0x24` replay file header
5. writes magic `0x72303174`, version `5`, and dword `0x100` at file-header `+0x10`
6. allocates/zeros a `0x64` replay-data header
7. allocates/zeros a `0x1C4` current-stage header
8. copies target-observed character/shot/difficulty globals to replay-data `+0x50/+0x54/+0x58`
9. when runtime storage exists, copies exactly `0x34` bytes from runtime `+0x24` into replay-data `+0x14`
10. captures current stage/seed and target-observed stage globals into the stage header
11. updates bit 0 at stage-header `+0x1C0`; when the source flag is nonzero, zeros dwords `+0x24/+0x28`
12. stores the same target value at stage-header `+0x1B4` and replay-data `+0x60`
13. creates three 0x24 callback-chain elements and inserts them at calc priorities `0x0B`, `0x1B`, and draw priority `5`
14. stores current stage in manager `+0x1D0`
15. returns `0`

### Mode 1: playback

Target behavior:

1. publishes the manager through global replay-owner storage
2. calls `0x0042A200` with the replay path; failure returns `-1`
3. copies replay-data `+0x14..+0x47` back to runtime `+0x24..+0x57`
4. selects the current stage's 0x24 manager stage-state entry
5. resets record cursor to record start, record index to zero, and FPS cursor to FPS start
6. restores character/shot/difficulty and target-observed stage globals from replay/stage headers
7. restores the stage seed and clears its companion counter
8. applies stage-header `+0x14 * 10` through target helper `0x00418B80` and stage-header `+0x18` through target helper `0x0042A930`
9. restores dwords from stage-header `+0x1C/+0x20/+0x1B4/+0x1B8`
10. installs the same three callback-chain elements
11. stores `-1` at manager active-stage `+0x1D0`
12. returns `0`

### Mode 2: load-only

The target calls `0x0042A200`. It returns `0` on loader success and `-1` on loader failure. It does not publish the replay manager or install the three callback chains in this branch.

Other mode values only store the mode and return `0`.

## Maintained replay layout refinements

`src/ReplayManager.hpp` now records target-supported layout information without asserting original identifiers:

- `ReplayFileHeader +0x10`: maintained `gameVersion` dword; target recording initialization writes `0x100`
- `ReplayDataHeader +0x14..+0x47`: maintained 0x34-byte `runtimeSnapshot`
- `ReplayDataHeader +0x60`: one target-observed dword, semantic name still unknown
- `ReplayStageDataHeader` scalar target fields at `+0x0C`, `+0x10`, `+0x14`, `+0x18`, `+0x1C`, `+0x20`, `+0x24`, `+0x28`, `+0x1B4`, `+0x1B8`
- stage-header bit 0 at `+0x1C0`

All existing size constraints remain:

- replay file header: `0x24`
- replay data header: `0x64`
- stage header: `0x1C4`
- replay manager: `0x2D4`

## 0x24 replay chain-element view

Initializer construction and the target calc/draw insertion helpers establish a maintained 0x24-byte element view:

- `+0x00`: dword priority
- `+0x04`: flag storage; bit 0 is cleared during construction and set before insertion for heap-created elements; bit 1 is cleared before insertion
- `+0x08`: primary callback
- `+0x0C`: lifetime callback slot
- `+0x10`: lifetime callback slot
- `+0x14`: embedded 0x0C intrusive list node; construction stores the element itself as node value
- `+0x20`: replay-manager argument

This layout is TH10 target-local. Committed TH08 chain source was consulted only after recovery as source-family corroboration; its chain element is 0x20 bytes and was not transferred to TH10.

Maintained source uses a descriptive `ReplayChainElement` and descriptive `ReplayAddToCalcChain` / `ReplayAddToDrawChain` interfaces. These names do not assert original identifiers, TU ownership, or source-level ABI of target helpers `0x00449AE0/0x00449B70`.

## Denominator expansion: six Ghidra-missed raw replay bodies

The packet added six new candidates because their physical bodies are independently CC-delimited, while keeping all six `unknown/review` with no source-present credit:

- `0x00429A30-0x00429A69` — 58 bytes. Frame-control-shaped replay body using owner through EAX. No modeled entry xref or raw little-endian VA reference. It closely duplicates active callback logic at `0x0042A3E0`, so retained source body versus optimizer clone/adapter is unresolved.
- `0x00429A70-0x00429B25` — 182 bytes. Replay FPS-display body reached by direct jump from `0x0042A430`. No raw literal entry address was observed.
- `0x00429B30-0x00429B53` — 36 bytes. Retained replay helper with no modeled entry xref; source origin and machine-call entry remain unknown.
- `0x0042A3D0-0x0042A3D9` — 10 bytes. Active first calc callback; forwards ECX owner to reviewed `ReplayManager::ProcessFrame`.
- `0x0042A3E0-0x0042A423` — 68 bytes. Active second calc callback; playback/frame-skip gating logic.
- `0x0042A430-0x0042A44B` — 28 bytes. Active draw callback wrapper; runtime gate then tail-jumps to `0x00429A70` with owner moved to EDX.

Initializer stores the three `0x0042A3xx` addresses as data pointers in both recording and playback chain setup. Source-written callback versus compiler-generated/LTCG adapter/clone origin cannot currently be decided. No adjacent name or origin was transferred.

## Maintained source

Tracked source changes are confined to `src/ReplayManager.cpp` and `src/ReplayManager.hpp`:

- implements natural `ReplayManager::Initialize`
- refines only target-observed replay/header fields and offsets
- adds a target-supported maintained `ReplayRuntimeState` snapshot view
- adds a target-supported maintained 0x24 `ReplayChainElement` view
- narrows replay manager chain pointers from `void *` to the maintained forward-declared chain type
- declares descriptive external interfaces for independently observed stage-timer, chain insertion, and callback seams

No target bytes, fake returns, inert padding, source-level private register annotations, target patching, or Oracle-specific equality tricks were added.

Original TH10 identifier spelling, original translation-unit owner, physical data owner of the descriptive globals, production per-function compiler profile, and normal-COFF versus LTCG ownership remain unknown.

## Compiler feedback

Pinned VC7.1 SP1 build-6030 fixed normal profile:

`/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc`

Result: compile passed.

The real normal `/Gy` COMDAT section for:

`?Initialize@ReplayManager@@QAEHHPBD@Z`

is `0x53B` = **1,339 bytes**. The target physical function is **1,344 bytes**, so the fixed normal object is not a physical extent match.

The same maintained source plus `/GL` also compiles successfully. This preserves an LTCG/interprocedural hypothesis but supplies no target-bound exactness authority.

A separate `compare-coff-function.py` diagnostic used the complete 1,344-byte target window and returned:

- result: mismatch
- matched comparable bytes: 55
- comparable bytes: 1,060
- acceptance authority: none

The comparator's `object_size=1344` is the caller-supplied window for this old-symbol diagnostic and is **not** treated as the owned object extent. The independently inspected normal COMDAT section is 1,339 bytes.

No canonical match unit or `config/matches.csv` exact row was added.

## Adjacent-game hypothesis use

Only committed adjacent content was used, and only after TH10-local behavior/layout recovery:

- TH08: HEAD `a45e99fb1942714e6edded20847e32a654d56f97`, branch `port/portable-64bit`, clean. Committed ReplayManager/Chain source used only for natural abstraction/name-family corroboration.
- TH09: HEAD `851666f089a4b1fb97865de2ef7b477b2726fed3`, branch `main`, ahead 2, observed clean status. No committed ReplayManager implementation was used.
- TH095: HEAD `478cea6eaac2ce7f0382559f59b6d611333562c5`, branch `main`, ahead 2. Unrelated untracked `EnemyManagerUpdate.i`, `config/runtime-scenarios.json`, `droid.resume.txt`, and `scripts/runtime-diff.py` were preserved and not read. Only committed ReplayManager source was consulted.

No adjacent address, target extent, data owner, ABI, exactness, or completion claim was transferred to TH10.

## Target source-family evidence

Attested target string search retains `.\\src\\game\\replay.cpp` labels including:

- `ReplayInf`
- `ReplayFileHeaderInf`
- `ReplayDataHeaderInf`
- `ReplayStageDataHeaderInf`
- `ReplayFrameDataInf`

These support replay source-family context but have no direct code xref binding `0x00428F60` to an original source identifier or physical TU.

## Ledger delta

Session entry on `2054cfff...`:

- candidates: 1,230
- origin/boundary pending: 1,143
- reviewed authored: 80
- authored bytes: 16,892
- exclusions: 7
- source-present mappings: 53
- canonical exact: 0 functions / 0 bytes

Current packet state:

- candidates: **1,236**
- origin/boundary pending: **1,148**
- reviewed authored: **81**
- authored bytes: **18,236**
- exclusions: **7**
- source-present mappings: **54**
- canonical exact: **0 functions / 0 bytes**
- canonical normal-COFF match units: **0**

Packet delta:

- candidates: +6
- pending: +5 net
- reviewed authored: +1
- authored bytes: +1,344
- exclusions: +0
- source mappings: +1
- exact: +0

The pending count rises because denominator expansion added six unknown/review raw bodies while only the initializer left pending status.

`config/matches.csv`, `config/match-units.toml`, and `config/build.toml` have no packet diff.

## Validation planes

Final tracked-source cold gate before checkpoint passed:

- fixed normal ReplayManager compile
- same source plus `/GL`
- true normal initializer COMDAT section re-observed as 1,339 bytes
- 1,344-byte comparator diagnostic re-observed as mismatch 55/1,060, authority none
- `python3 scripts/verify-target.py`
- `python3 scripts/verify-toolchain.py --check`
- `python3 scripts/validate-tracking.py --require-target`
- `python3 scripts/progress.py --check`
- `python3 scripts/build-match-unit.py --check` -> 0 units
- `python3 scripts/build.py --check` -> target-bound build skeleton explicitly open
- `python3 scripts/report-reconstruction-status.py`
- `python3 scripts/ci.py`
- `git diff --check`
- no diff in exact/build control files

Honest `python3 scripts/build.py` returned expected **RC=2**:

`whole build unavailable: compile flags, TU partition, libraries, resources, and link order remain unknown`

Verification states remain separate:

- source presence: initializer yes
- canonical exactness: none
- whole Windows i386 build closure: open
- runtime validation: not started
- semantic/port stages: not started

## Truth Kernel

A pre-packet accepted-snapshot refresh was unavailable because another Factory operation owned `<operator-path>`; live repository status was reread before continuing.

A later snapshot refresh encountered Factory transport failure; live status was again reread and no result was inferred.

No current-session Truth snapshot is available as proof for this initializer packet. No old snapshot is reused as evidence. No accepted or rejected initializer/callback/exactness claim is reported from this session.

## `.analysis` lifecycle

Session entry `.analysis`: **123,044 bytes**.

Current campaign:

`.analysis/gpt-web/20260913-replay-initialize/`

Peak after normal/LTCG objects, comparator JSON, compact report, and manifest: **180,178 bytes**.

Post-cleanup `.analysis` before ignored closeout metadata: **129,764 bytes**. Final closeout size after manifest binding: **130,223 bytes**.

Retained current-campaign artifacts:

- `compiler-shape-report.txt` — 5,177 bytes
- `manifest.json` — current ignored campaign manifest

Deleted current-session reproducible scratch after compact evidence was retained:

- `replay-init-normal.obj`
- `replay-init-ltcg.obj`
- `initialize-normal-compare.json`
- `initialize-normal-compare.stderr`

Final cold-gate compile/comparator artifacts were command-local `/tmp` files and were automatically removed.

No current-campaign artifact exceeded 64 MiB; the campaign remained far below the 256 MiB review threshold. No legacy/shared/unknown `.analysis`, target, Ghidra state, toolchain, Wine prefix, or another process's output was removed.

## Transport/recovery events

Several read-only or multi-file operations encountered Factory transport failures. Each failure was followed by a live repository status read before retrying or proceeding. Two attempted multi-file ledger writes were confirmed to have made no changes before the ledger work was retried as small per-file transactions. No failed command was assumed to have completed, and no unknown files were deleted to recover a clean tree.

## Checkpoint closeout

The complete working diff and complete staged diff were audited before commit. Staged-state Ghidra `check {}` passed with exact `target:th10-main`, `provider_transport=factory-native-command`, and `exactness_credit=none`.

Initial local checkpoint before this documentation-only closeout amend:

- commit: `c7335c68515c2e4d79aa3a06db4ae07ef375972a`
- subject: `gpt-web: reconstruct TH10 replay initializer`
- branch: `main`
- relation after the initial checkpoint: ahead 3 / behind 0
- staged / unstaged / untracked / conflicts: 0 / 0 / 0 / 0
- pushed by this session: **false**

Post-checkpoint cold validation passed again on the committed source and ledgers: fixed normal and `/GL` ReplayManager compilation, true 1,339-byte normal initializer COMDAT observation, 1,344-byte comparator-window mismatch 55/1,060 with authority none, target/toolchain/tracking/progress/match-unit/build-graph/status/public-CI checks, honest whole-build RC=2/open, and the exact expected ten-file commit set. A fresh post-checkpoint Ghidra `check {}` also passed at `2026-09-12T17:41:01.401636Z` with the exact target/native transport and no exactness credit.

The post-checkpoint Truth refresh was unavailable because another Factory operation owned `<operator-path>`. That availability failure is not an accepted or rejected packet claim; no older snapshot is reused as proof.

After compact evidence and ignored-manifest closeout, `.analysis` is **130,223 bytes**. The current campaign retains only `compiler-shape-report.txt` (5,177 bytes) and `manifest.json` (2,002 bytes); no current-campaign artifact exceeds 64 MiB.

This tracked handoff records the initial logical checkpoint hash because embedding the hash of its own documentation-only amend would be self-referential. The authoritative final HEAD is the current commit containing this closeout handoff; its exact hash is recorded in the ignored campaign manifest after the amend and in the operator-facing final report. The logical subject remains `gpt-web: reconstruct TH10 replay initializer`. Nothing was pushed by this session.

## Next preferred hard packet

**Replay load owner: `0x0042A200-0x0042A3C1` — 450 bytes, currently `unknown/review`.**

This extent is already target-bound for routing purposes:

- reviewed SaveReplay ends at `0x0042A1F0`; `0x0042A1F1-0x0042A1FF` is CC padding
- loader starts at `0x0042A200`
- loader ends with `RET 4` at `0x0042A3BF-0x0042A3C1`
- `0x0042A3C2-0x0042A3CF` is CC padding before newly tracked raw callback `0x0042A3D0`
- callers are exactly the reviewed shared initializer and reviewed static `ReplayManager::Load`

Why it is the right next hard frontier:

- it closes the maintained `ReplayManager::LoadReplay` declaration currently called by two reviewed owners
- it contains a disk-versus-archive file-source branch
- it owns the reverse `Decrypt(seed 0xAA/chunk 0x400)` then `Decrypt(seed 0x3D/chunk 0x80)` pipeline followed by reviewed `DecompressData`
- it allocates the decompressed replay payload and reconstructs up to eight stage header/record/FPS cursor views used by the reviewed initializer and ProcessFrame
- it has compiler-sensitive GS-cookie/local-buffer shape and mixed private owner/path ABI context
- it is not an easy-function/count selection

`0x0042A450-0x0042A695` is a separate 582-byte candidate with only caller `0x00418190`; do not automatically merge it into the load packet merely because it is adjacent to the callback gap. The six new raw callback/context candidates should remain unknown until source-origin/adapter evidence is strong enough to classify them.

Campaign balance since the previous hard frontier therefore remains centered on large ReplayManager ownership seams: 1,681-byte replay writer, 1,327 bytes of lifecycle/ProcessFrame authored bodies plus one compiler helper, and now the 1,344-byte shared initializer while adding six unresolved raw denominator bodies. The next 450-byte loader continues the same ownership/data-flow seam rather than selecting the easiest remaining target.
