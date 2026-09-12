# TH10 exact reconstruction handoff

## Session status

Exact reconstruction remains active and incomplete. This checkpoint adds maintained replay-loader source and reviews its connected stateful replay-read seam. It does not claim canonical function exactness, original translation-unit ownership, whole-product build closure, runtime closure, semantic reconstruction readiness, or port readiness.

## Recovery state

Session starting checkpoint:

- branch: `main`
- HEAD: `63ae175fd9f850a9a66bddf7e0bcbc539c53cbf7`
- upstream: `origin/main` at `c10c371ed3b09451ef1076f6ec1e99d87f174d22`
- relation: ahead 3 / behind 0
- staged / unstaged / untracked / conflicts: 0 / 0 / 0 / 0

Full recovery inspection covered recent commits, porcelain-v2 state, staged and unstaged diffs, untracked paths, ignored analysis/build/tool/provider state, and this handoff. No interrupted tracked work was present.

Preserved/excluded state:

- `resources/th10.exe`: operator-supplied private target; preserved, ignored, never modified/staged/committed/relocated.
- `.tools/`: shared pinned toolchain/Wine state; preserved.
- `ghidra-project/`: provider state; preserved.
- `build/`: reproducible/open generated build state; preserved.
- pre-existing `.analysis/`: legacy/shared scratch; preserved except current-session files with explicit ownership.

Entry `.analysis` size was 130,223 bytes.

## Guidance and mandatory preflight

The session reread to EOF before editing:

- repo `AGENTS.md`
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
- repo-local `.agents/skills/th10-reconstruction/SKILL.md`

Required repo preflights passed on the starting checkpoint:

- `python3 scripts/verify-target.py`
- `python3 scripts/verify-toolchain.py --execute`
- `python3 scripts/validate-tracking.py --require-target`
- `python3 scripts/report-reconstruction-status.py`
- `python3 scripts/ci.py`

Target identity remained:

- size 487,936
- SHA256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`
- MD5 `7dc488d82c81dd4aee4ba098b8804d83`
- PE32 i386, image base `0x00400000`, entry `0x004537DC`
- Rich checksum `0x3152A748`, dominant build 6030

The pinned VC7.1 SP1 candidate again passed normal C/C++ COFF, `/GL` C++, resource, and PE32 link smoke.

`factory_list_analysis_operations(th10-ghidra)` was rediscovered. Fresh `check {}` passed exact `target:th10-main`, `attestation.status=passed`, and `provider_transport=factory-native-command`; all Ghidra results retain `exactness_credit=none`.

The entry Truth Kernel refresh was unavailable because another Factory operation owned shared `<operator-path>`. No older snapshot was reused as packet evidence.

## Hard packet selection

Selected packet:

- `0x0042A200-0x0042A3C1`, 450 bytes, previously `unknown/review`
- connected read-seam callees `0x0044B6B0`, `0x0044B790`, and `0x0044B7E0`, all previously `unknown/review`

This is a central replay-owner packet rather than a function-count win: it connects the reviewed initializer/static load paths to disk/archive acquisition, replay header validation, two-pass decrypt, LZSS decompression, and stage-state cursor reconstruction. The observable outcome was to close its physical extent/private ABI/resource ownership, add natural maintained source if supported, and review only the necessary read-seam callees without forcing exactness.

## `ReplayManager::LoadReplay` target result

Physical extent is closed directly from target bytes:

- preceding replay-save body ends at `0x0042A1F0`
- `0x0042A1F1-0x0042A1FF`: CC padding
- loader: `0x0042A200-0x0042A3C1`, exactly 450 bytes
- tail performs the security-cookie check and ends with `ret 4` at `0x0042A3BF-0x0042A3C1`
- `0x0042A3C2-0x0042A3CF`: CC padding

All three direct calls establish the private machine boundary:

- `0x00429257`
- `0x0042948C`
- `0x00429765`

Each supplies the `ReplayManager` owner through live-in ESI and pushes one replay-path argument. Maintained source remains an ordinary member function; the private ESI lowering is not promoted to a source calling convention.

Observed behavior:

1. Copy the replay path into object storage at `+0x1D4`.
2. Test byte `0x00474CA0` bit `0x20` to choose disk versus archive-backed loading.
3. Disk path:
   - format `replay/%s`;
   - call reviewed file-existence helper;
   - open the stateful replay read handle;
   - read a 0x24-byte file header;
   - require magic `0x72303174` and version word `5`;
   - read `compressedPayloadSize` bytes;
   - close the shared handle/critical-section state.
4. Archive-selected path:
   - call reviewed `FileSystem::OpenFile(path, &size, 0)`;
   - store that returned allocation as `fileHeader`;
   - use `fileHeader + 0x24` as compressed data.
5. Allocate `decompressedPayloadSize` bytes.
6. Apply reviewed `FileSystem::Decrypt` first with `(seed, increment, chunk) = (0xAA,0xE1,0x400)`, then `(0x3D,0x7A,0x80)`, each with `maxBytes == compressedPayloadSize`.
7. Call reviewed `DecompressData` into the maintained decompressed allocation.
8. Set replay-data base at decompressed `+0x00`; stage data begins at `+0x64`.
9. Preserve the target quirk: if `stageCount >= 8`, use loop bound **6**, not 8.
10. For each stage header, index the 0x24 stage-state array by signed `stageIndex`, store header pointer, set record start to header `+0x1C4`, and FPS start to `recordStart + recordCount * 6`; advance to the next stage by `0x1C4 + payloadSize`.
11. Free the separate compressed allocation only on the disk path.
12. Return zero on success; the explicit disk existence/open/header-validation failures return `-1`.

The target does not add malloc/decompress failure guards in this body; maintained source does not invent them.

## Connected stateful replay-read seam

Three necessary callees were reviewed from TH10 target behavior and boundaries. They are authored/source-shaped but remain source-unmapped in this packet.

### `0x0044B6B0-0x0044B738` — maintained `ReplayFile::Open`

- one stack path / `ret 4`
- `0x0044B739-0x0044B73F` is CC padding
- enters shared file critical section at `0x004922A4`
- increments shared active-file count byte `0x0049231E`
- opens GENERIC_READ / FILE_SHARE_READ / OPEN_EXISTING with normal + sequential-scan flags
- stores handle at `0x00474C38`
- failure path formats/frees the system error message, leaves the critical section, decrements the count, and returns `-1`
- success returns zero while intentionally retaining the critical section until close

### `0x0044B790-0x0044B7D9` — maintained `ReplayFile::Read`

- private live-in EDI byte count / plain `ret`
- `0x0044B7DA-0x0044B7DF` is CC padding
- invalid shared handle returns null
- allocates exactly EDI bytes
- allocation failure closes the handle and returns null
- otherwise reads that byte count from the shared handle and returns the heap buffer

The EDI machine input is not promoted to a source calling convention.

### `0x0044B7E0-0x0044B804` — maintained `FileSystem::CloseWriteFile`

- no arguments / plain `ret`
- CC padding begins at `0x0044B805`
- closes a valid shared handle
- leaves the shared critical section
- decrements the active-file count
- returns zero

The replay-save writer contains equivalent close behavior inline; committed adjacent source corroborates the retained source-level name but does not prove TH10 TU ownership.

Attested Ghidra caller review reports the replay loader as the sole modeled direct caller of these three retained bodies.

## Maintained source

Changed source:

- `src/ReplayManager.cpp`
- `src/FileSystem.hpp`

`src/ReplayManager.cpp` now contains a natural maintained `ReplayManager::LoadReplay` implementation matching the target-observed disk/archive split, header checks, transforms, decompression, stage cursor reconstruction, and disk-only compressed-buffer free.

The target byte at `0x00474CA0` is represented only as a neutral maintained flags byte with a descriptive `0x20` constant; the containing original object/field identity is not claimed.

`ReplayFile::Open` and `ReplayFile::Read` are declared as descriptive interfaces for the reviewed external bodies but are not implemented or source-mapped here. `FileSystem.hpp` adds declarations for the already-reviewed `FileSystem::OpenFile` and `CheckIfFileAlreadyExists` interfaces used by the loader.

## Compiler feedback

Pinned fixed profile:

`/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc`

Final maintained `src/ReplayManager.cpp` compiles successfully with that normal profile and with the same source plus `/GL`.

Normal COFF symbol:

`?LoadReplay@ReplayManager@@QAEHPBD@Z`

True normal `/Gy` owned COMDAT:

- `0x199` = **409 bytes**
- 16 relocations

Target physical extent:

- **450 bytes**

Therefore the fixed normal object is not a complete physical match. A separate 450-byte `compare-coff-function.py` diagnostic reports:

- `result=mismatch`
- 386 comparable bytes
- 31 matched comparable bytes
- `acceptance_authority=none`

The comparator's reported `object_size=450` is the caller-supplied comparison window, not the real object-owned extent. `/GL` compilation preserves a possible LTCG/interprocedural owner, but there is no target-bound linked-image exactness workflow proving that owner. No canonical match unit or exact row was created.

## Adjacent-game hypothesis use

Adjacent content was consulted only after TH10-local behavior was established and only from committed HEAD content.

- TH08: HEAD `a45e99fb1942714e6edded20847e32a654d56f97`, branch `port/portable-64bit`, clean.
- TH09: HEAD `851666f089a4b1fb97865de2ef7b477b2726fed3`, branch `main`, ahead 2 with unrelated tracked dirty work. Only committed `HEAD:` content was read.
- TH095: HEAD `823134eba906abf1a9744400fd42e29b1edc465d`, branch `main`, ahead 4 with unrelated untracked `EnemyManagerUpdate.i`, `config/runtime-scenarios.json`, `droid.resume.txt`, and `scripts/runtime-diff.py`. Only committed `HEAD:` content was read.

Committed TH095 corroborates the `LoadReplay`, `ReplayFile::Open`, `ReplayFile::Read`, and `FileSystem::CloseWriteFile` source-family/naming shape. No adjacent address, extent, layout, ABI, physical owner, exactness, or completion claim was transferred.

## Ledger delta

Entry:

- candidates: 1,236
- pending: 1,148
- authored: 81
- authored bytes: 18,236
- exclusions: 7
- source-present: 54
- canonical exact: 0

Current:

- candidates: **1,236**
- pending: **1,144**
- authored: **85**
- authored bytes: **18,934**
- exclusions: **7**
- source-present: **55**
- canonical exact: **0**

Packet delta:

- candidates +0
- pending -4
- authored +4
- authored bytes +698
- exclusions +0
- source mappings +1
- exact +0

Promoted/reviewed rows:

- `0x0042A200` -> authored `ReplayManager::LoadReplay`, source `src/ReplayManager.cpp`
- `0x0044B6B0` -> authored `ReplayFile::Open`, source blank
- `0x0044B790` -> authored `ReplayFile::Read`, source blank
- `0x0044B7E0` -> authored `FileSystem::CloseWriteFile`, source blank

No changes were made to `config/matches.csv`, `config/match-units.toml`, or `config/build.toml`.

## Validation planes

Before the initial checkpoint, the checkpoint-ready tracked source passed:

- pinned VC7.1 SP1 fixed-profile normal `ReplayManager.cpp` compilation
- the same source plus `/GL`
- true normal `ReplayManager::LoadReplay` COMDAT re-observed as 409 bytes with 16 relocations
- 450-byte target comparator diagnostic re-observed as mismatch at 31/386 comparable bytes with `acceptance_authority=none`
- `python3 scripts/verify-target.py`
- `python3 scripts/verify-toolchain.py --check`
- `python3 scripts/validate-tracking.py --require-target`
- `python3 scripts/progress.py --check`
- `python3 scripts/build-match-unit.py --check` -> 0 units
- `python3 scripts/build.py --check` -> graph explicitly open
- `python3 scripts/report-reconstruction-status.py`
- `python3 scripts/ci.py`
- `git diff --check`
- no diff in `config/matches.csv`, `config/match-units.toml`, or `config/build.toml`

Honest `python3 scripts/build.py` returned expected RC=2 with compile flags, TU partition, libraries, resources, and link order still unknown. Live status was reread after the nonzero diagnostic and showed only the intended staged packet.

The complete working diff and complete cached diff were audited to EOF before commit. Index state was exactly ten staged paths, zero unstaged, zero untracked, and each staged blob hash matched the audited working-tree blob. Staged-state Ghidra `check {}` passed at `2026-09-12T18:19:27.884955Z` for exact `target:th10-main` / `factory-native-command` with `exactness_credit=none`.

Initial local checkpoint:

- commit: `704b0c286240f84e927c8f564a29ad96e3b231da`
- subject: `gpt-web: reconstruct TH10 replay loader`
- branch: `main`
- relation after checkpoint: ahead 4 / behind 0
- staged / unstaged / untracked / conflicts: 0 / 0 / 0 / 0
- pushed by this session: **false**

The same cold validation was rerun on committed `704b0c286240f84e927c8f564a29ad96e3b231da` and passed again: normal and `/GL` compile, 409-byte/16-relocation normal COMDAT, 31/386 mismatch diagnostic, target/toolchain/tracking/progress/match-unit/build-graph/status/public-CI checks, `git diff --check`, exact ten-file commit set, and honest whole-build RC=2/open. A fresh post-commit Ghidra `check {}` passed at `2026-09-12T18:20:12.876331Z` with the exact target/native transport and no exactness credit.

State separation remains:

- source presence: loader yes; three connected read helpers no
- canonical function exactness: none
- whole Windows i386 build: open
- runtime validation: not started
- semantic/port stages: not started
- Ghidra evidence: attested provisional analysis only, exactness credit none
- Truth Kernel acceptance for this packet: unavailable/not observed

## Truth Kernel

Both entry and post-checkpoint accepted-snapshot refreshes were unavailable because another Factory operation owned shared `<operator-path>`. The post-checkpoint failure was followed by a live repository-status reread. No accepted or rejected loader/read-seam/exactness claim was observed in this session, and no older snapshot is reused as proof.

## `.analysis` lifecycle

- entry size: 130,223 bytes
- current-session peak: 200,120 bytes
- final ignored closeout size before the documentation-only amend: 135,581 bytes
- current campaign: `.analysis/gpt-web/20260913-replay-loader/`
- retained: `compiler-shape-report.txt` (3,912 bytes) and `manifest.json` (1,446 bytes at the initial-checkpoint binding)
- removed: reproducible current-session normal `/GL` objects, PDB paths, and detailed comparator JSON
- no current-session artifact exceeds 64 MiB
- no legacy/shared/unknown analysis state was removed

The ignored manifest records the logical checkpoint subject, `pushed=false`, post-commit cold validation, post-commit Ghidra attestation, Truth unavailability, and exactness `none`. After this documentation-only amend, its two 40-character commit fields are rebound to the authoritative final HEAD; the exact final HEAD and final `.analysis` byte count are recorded there and in the operator-facing final report rather than self-referencing the tracked handoff.

## Transport/recovery events

Factory transport was intermittent. Failures occurred during initial Factory discovery, long guidance reads, several Ghidra/disassembly queries, source-edit commands, durable-output reads, and one post-Truth status read. After every failed/timed-out/non-transactional command, live repository status was reread before retrying or continuing. Persistent-write failures were classified by live dirty counts before any retry. No unknown/unrelated path was reset, overwritten, deleted, or staged to obtain a clean tree.

## Checkpoint closeout

This tracked handoff is the documentation-only closeout amend of the single logical packet whose initial checkpoint is `704b0c286240f84e927c8f564a29ad96e3b231da` and whose subject remains `gpt-web: reconstruct TH10 replay loader`. Embedding the amended commit's own hash in this file would be self-referential; the authoritative final HEAD is therefore recorded after amend in the ignored campaign manifest and operator-facing final report. Nothing is pushed.

## Next preferred hard packet

Primary next packet:

`0x0042A450-0x0042A695` — ledger 582 bytes, currently `unknown/review`.

Fresh attested Ghidra reports 579 body addresses and one caller at `0x00418190`; this discrepancy means its physical boundary is **not reviewed yet**. Provisional decompilation shows a live-in EBX replay owner and a central stage-transition/state-copy role:

- marks up to three chain elements for removal/disable;
- record mode selects the current stage, frees/reallocates frame storage, snapshots runtime/player state into the 0x1C4 stage header, and updates active stage;
- playback mode restores the selected stage snapshot into runtime/player state, calls connected restore/reset helpers, clears four runtime state slots, and resets the replay frame counter.

This is the next hard route because it connects the reviewed loader/initializer/lifecycle source to the stage-header fields already recovered from save/load, not because it is an easy remaining function. The next session must first reconcile raw entry/exit, CC padding, the ledger 582 versus Ghidra 579-body-address discrepancy, private ABI, chain ownership, and the large runtime-copy extents before any authored/source promotion.
