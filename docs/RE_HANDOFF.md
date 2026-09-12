# TH10 Reconstruction Handoff

## Session status

The exact-reconstruction campaign remains **active and incomplete**. This checkpoint reconstructs the central replay-save writer as reviewed authored/source-present code, but it does **not** establish canonical exactness, whole-product build closure, runtime closure, semantic-reconstruction readiness, port readiness, or new Truth Kernel acceptance.

Repository selection for this session:

- Repository: `th10`
- Analysis provider: `th10-ghidra`
- Target: `target:th10-main`
- Private local target: `resources/th10.exe`
- Phase: exact reconstruction with early faithful Windows i386 build feedback

The private target remained ignored and untouched. No target bytes were patched, copied into source, relocated, or committed. No push was performed.

## Recovery and starting state

Session starting source commit:

- `c10c371ed3b09451ef1076f6ec1e99d87f174d22`
- subject: `gpt-web: reconstruct TH10 FileSystem encrypt transform`
- branch: `main`
- initial observed relation: ahead 1 / behind 0 against the then-visible `origin/main`
- dirty counts: 0 staged / 0 unstaged / 0 untracked / 0 conflicts

During recovery, the remote-tracking reference was externally advanced to the same commit as local HEAD. The repository reflog recorded `origin/main` update-by-push at `2026-09-12 23:38:50 +0800`. Local HEAD did not move, this session did not push, and the live relation became ahead 0 / behind 0.

The original packet recovery gate inspected recent commits, branch/upstream state, porcelain-v2 status, staged and unstaged diffs, untracked files, the handoff, and relevant ignored state. After that browser session was interrupted before checkpointing, the next resume observed the same HEAD and upstream with **0 staged / 8 unstaged / 2 untracked / 0 conflicts**. The eight tracked edits plus `src/ReplayManager.cpp` and `src/ReplayManager.hpp` were reviewed in full together with the campaign manifest and classified as **recoverable-current-work** for this replay-save packet. No unrelated or unknown dirty path was found. The resumed session therefore completed and checkpointed this packet before starting any new reconstruction work.

Ignored/private/generated state was classified and preserved:

- `resources/th10.exe`: operator-supplied private target; preserve and exclude.
- `.tools/`: shared tool selectors and Wine/tool state; preserve and exclude.
- `ghidra-project/`: target-bound provider state; preserve and exclude.
- previous `.analysis/` campaigns: checkpointed or legacy ignored evidence; preserve.
- `build/`: generated/open build state; preserve.

`.analysis/` measured **109,361 bytes** at session entry. `build/` measured **5,031,767 bytes**.

All prompt-required repository guidance, Factory contracts/docs, the Factory-root `docs/semantic-reconstruction.md`, and `.agents/skills/th10-reconstruction/SKILL.md` were read completely before editing. The resumed recovery session reread the same complete guidance set before touching the recoverable packet. No required path was missing.

## Mandatory preflight

The following repository preflights passed before target-dependent editing:

- `python3 scripts/verify-target.py`
- `python3 scripts/verify-toolchain.py --execute`
- `python3 scripts/validate-tracking.py --require-target`
- `python3 scripts/report-reconstruction-status.py`
- `python3 scripts/ci.py`

Target identity remained:

- SHA256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`
- MD5 `7dc488d82c81dd4aee4ba098b8804d83`
- PE32 i386, 4 sections
- image base `0x00400000`
- entry point `0x004537DC`
- Rich checksum `0x3152A748`
- dominant tool build 6030

The pinned VC7.1 SP1 candidate passed its execute smoke, including normal COFF, LTCG, resource, and PE32-link checks.

`th10-ghidra` operations and schemas were rediscovered. The mandatory `check {}` passed with:

- `attestation.status = passed`
- target `target:th10-main`
- `attestation.provider_transport = factory-native-command`
- correct target hash/layout/entry
- `exactness_credit = none`

All useful Ghidra observations in this packet were target-attested and read-only. No Ghidra observation is treated as exactness evidence.

## Hard packet: replay-save writer

The selected hard frontier was:

- `0x00429B60-0x0042A1F0`, **1,681 bytes**
- initial ledger state: `unknown/review`, no source mapping, no exactness
- maintained result: reviewed `authored_game`, source-present as `ReplayManager::SaveReplay`, non-exact

This body was selected because it directly owns the write-side pipeline exposed by the previous packet: stage aggregation and packing, `CompressData`, two `FileSystem::Encrypt` passes, replay-file output, and two user-metadata blocks. It is a large central data-owner/ABI/control-flow packet, not a smallest-function or easy-exact target.

### Physical extent and machine boundary

The physical extent was closed independently from the provisional Ghidra boundary:

- `0x00429B54-0x00429B5F` is `CC` padding before the body;
- body starts at `0x00429B60`;
- terminal `ret 4` is at `0x0042A1EE-0x0042A1F0`;
- `0x0042A1F1-0x0042A1FF` is `CC` padding;
- independent replay-load body begins at `0x0042A200`.

Two direct target call sites were recovered:

- `0x00423F16`
- `0x0043399D`

Both callers load the replay owner from global `0x00477838` into ECX, place the formatted replay path in EDX, push one replay-name pointer, call `0x00429B60`, and ignore the return value. The observed machine boundary is therefore:

- ECX: replay owner
- EDX: replay path
- one stack argument: replay name
- `ret 4`

This is a machine-ABI fact only. The maintained source uses an ordinary C++ member declaration and does not claim source-level `__fastcall` or original signature lowering.

### Original replay source-family evidence

The TH10 target itself retains replay source/type strings including:

- `src/game/replay.h:129 ReplayRecDataInf`
- `src/game/replay.cpp:765 ReplayStageDataHeaderInf`
- `src/game/replay.cpp:636 ReplayFrameDataInf`
- `src/game/replay.cpp:168 ReplayInf`
- `src/game/replay.cpp:48 ReplayStageDataHeaderInf`
- adjacent `ReplayDataHeaderInf` and `ReplayFileHeaderInf` strings

These establish original replay source-family participation and retained type-label vocabulary. They do **not** by themselves assign a reviewed function to an original translation unit or prove original C++ declarations.

### Target-local layout recovery

The replay-save body and independent load body establish the following maintained views:

- `ReplayFileHeader`: size `0x24`
  - `+0x0C`: user-data offset
  - `+0x1C`: compressed payload size
  - `+0x20`: decompressed payload size
- `ReplayDataHeader`: size `0x64`
  - `+0x0C`: timestamp used by `localtime`
  - `+0x10`: score
  - `+0x48`: slow rate
  - `+0x4C`: packed stage count
  - `+0x50/+0x54/+0x58/+0x5C`: character, shot, difficulty, completion-state selectors used by metadata formatting
- `ReplayStageDataHeader`: size `0x1C4`
  - signed stage index at `+0x00`
  - six-byte record count at `+0x04`
  - variable payload size at `+0x08`
- `ReplayRecData`: size `0x06`
- `ReplayFrameData` observed storage:
  - 3,600 six-byte records: `0x5460` bytes
  - record-end pointer at `+0x5460`
  - 3,600 one-byte samples beginning at `+0x5464`
  - sample-end pointer at `+0x6274`
  - total observed size `0x6278`

The writer divides the first stream byte distance by six using the target's signed magic-multiply sequence, consistent with typed six-byte record pointer subtraction. The load body independently reconstructs the first stream end from `stageData + recordCount * 6`.

The adjacent replay-load body at `0x0042A200` independently validates the serialized layout:

- file header magic `0x72303174` (`"t10r"` in file byte order)
- version word `5`
- compressed/decompressed sizes at `+0x1C/+0x20`
- first stage header at decompressed base `+0x64`
- next stage at `current + 0x1C4 + payloadSize`

The replay object allocation site at `0x004296F0` allocates and clears exactly `0x2D4` bytes and constructs eight `0x24`-byte subobjects beginning at `+0xA0`. This packet establishes the total object size and the SaveReplay-relevant fields through `+0x9F`; maintained source deliberately keeps `+0xA0-0x2D3` as unknown storage rather than guessing the subobject semantics.

### Save pipeline behavior

The reviewed body:

1. copies the supplied replay name into the replay-data header and pads the first eight name positions with spaces;
2. creates the `replay` directory and formats `replay/%s`;
3. iterates up to eight stage slots;
4. for each present stage, resets its payload size, adds a fixed `0x1C4` stage header, walks its linked recording buffers, accumulates six-byte record count plus the one-byte sample stream, and tracks first/last present stage;
5. stores packed-stage count, target score, and slow-rate metadata;
6. allocates the full decompressed buffer, copies the `0x64` replay header, then each stage header and its variable streams;
7. calls reviewed `CompressData`;
8. applies reviewed `FileSystem::Encrypt` twice:
   - seed `0x3D`, increment `0x7A`, chunk `0x80`, `maxBytes == compressedSize`;
   - seed `0xAA`, increment `0xE1`, chunk `0x400`, `maxBytes == compressedSize`;
9. stores compressed/decompressed sizes and user-data offset in the `0x24` file header;
10. opens the replay output and writes the file header and compressed payload;
11. allocates and clears a `0xFFFF`-byte scratch buffer and emits aligned `USER` type-0 metadata containing version, name, date, character, rank, stage summary, score, and slow rate;
12. clears the same scratch and emits aligned `USER` type-1 retained comment text;
13. frees scratch, closes the write state, and returns zero.

The target contains explicit short-write cleanup paths that close the Win32 handle, leave the file-system critical section, and decrement the target write-depth byte.

`0x0044B620` was inspected only as connected context and target-locally behaves as the write-file opener: it enters the shared critical section, opens with `GENERIC_WRITE`, `CREATE_ALWAYS`, share mode 1, and normal attributes, and unwinds the write state on failure. That candidate remains independently `unknown/review`; this packet does not promote it.

## Maintained source

New maintained files:

- `src/ReplayManager.hpp`
- `src/ReplayManager.cpp`

The source uses natural C++ and target-supported replay type names while leaving unrecovered storage/fields explicitly unknown. It contains no target bytes, fake returns, inert padding, target patching, or exact-mode local arrangement.

`ReplayManager::SaveReplay` is retained as a descriptive maintained source name. The source does not claim an original decorated symbol, original translation-unit owner, production calling convention, or physical compiler owner.

A minimal timing-source view is explicitly packed to four-byte alignment because the target reads consecutive doubles at `+0x24` and `+0x2C`. The first compile attempt failed the two offset assertions under the default layout; the pack-four correction follows direct target offsets rather than suppressing the assertions.

The maintained `ReplayManager` size is asserted to `0x2D4`, but fields beginning at `+0xA0` remain an unknown byte range because their semantics are outside this packet.

## Adjacent-game hypothesis provenance

Adjacent repositories were consulted only after TH10 had established the packet boundary, machine ABI, major layout, and save behavior. Only committed content was used.

- TH08: HEAD `a45e99fb1942714e6edded20847e32a654d56f97`, branch `port/portable-64bit`, clean. Committed ReplayManager source corroborated the `SaveReplay` naming family only.
- TH09: HEAD `0c0d91323a3d37cfc84c578ffa136840b5796dbc`, branch `main`, clean. Committed replay input/source corroborated naming and linked-buffer concepts; its data format differs materially and did not define TH10 layout.
- TH095: HEAD `4c9cfb5085f34feabbb30a3cbff624eb11444450`, branch `main`, tracked clean with unrelated untracked `EnemyManagerUpdate.i`, `config/runtime-scenarios.json`, `droid.resume.txt`, and `scripts/runtime-diff.py`. None of those untracked files was read. Committed ReplayManager source corroborated the 0x24 file-header family, the same two Encrypt profiles, 0xFFFF USER scratch, two USER types, and four-byte metadata alignment only after those facts were recovered from TH10.

No adjacent address, function extent, data/object ownership, translation-unit ownership, ABI, exactness, or completion state was transferred into TH10.

## Compiler and exactness feedback

The final tracked `src/ReplayManager.cpp` compiles successfully with the pinned VC7.1 SP1 candidate under:

- fixed normal profile: `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc`;
- the same source/profile plus `/GL`.

The final target-bound normal-COFF probe reports:

- target owned extent: 1,681 bytes
- object owned extent: 1,681 bytes
- comparable non-relocation bytes: 1,465
- matching comparable bytes: 73
- result: mismatch
- acceptance authority: none

A natural source correction removed a redundant temporary `time_t`: the target directly passes replay-data `+0x0C` to `localtime`. This preserved the 1,681-byte owned extent and improved the normal diagnostic from 63/1465 to 73/1465 comparable bytes.

Target bytes visibly contain GS-cookie instrumentation. Additional **scratch-only** profile/ABI diagnostics were used to classify the mismatch, not to tune maintained source:

- tracked source plus `/GS`: 1,681 bytes, mismatch, 86/1457 comparable bytes;
- temporary source-level `__fastcall`, normal profile: 1,681 bytes, mismatch, 78/1465;
- temporary source-level `__fastcall` plus `/GS`: 1,681 bytes, mismatch, 87/1457.

None closes the target shape. The remaining mismatch is therefore not reducible to one obvious source-level calling-convention switch; GS profile, private register lowering, helper inlining, local lifetimes, LTCG/interprocedural context, and physical source ownership remain unresolved.

The maintained source remains ordinary C++. No canonical exact row or match unit was added. `/GL` compilation preserves the LTCG hypothesis but has no standalone target-bound exactness authority.

`config/matches.csv`, `config/match-units.toml`, and `config/build.toml` remain unchanged.

## Durable ledger changes

The existing candidate `0x00429B60` was promoted only after target-local boundary, caller ABI, save/load layout, output behavior, and compiler feedback were reconciled.

Tracked packet paths before the handoff update:

- `config/function-origins.csv`
- `config/functions.csv`
- `config/implemented.csv`
- `config/reccmp-functions.csv`
- `docs/KNOWLEDGE_BASE.md`
- `docs/PROGRESS.md`
- `resources/progress.svg`
- `src/ReplayManager.cpp`
- `src/ReplayManager.hpp`

This handoff is the tenth intended tracked path.

Knowledge entries added/updated include `REPLAY-001`, `REPLAY-002`, `REPLAY-003`, `TOOLCHAIN-011`, and `SOURCE-002`.

Current reconstruction status:

- tracked candidates: **1,229**
- origin/boundary pending: **1,148**
- reviewed authored: **75 functions / 15,545 bytes**
- exclusions: **6**
- source-present mappings: **48**
- canonical exact: **0 functions / 0 bytes**
- canonical match units: **0**
- whole Windows i386 build: **open**
- runtime validation: **not started**
- semantic reconstruction / ports: **not started**

Packet delta:

- candidates: +0
- pending: -1
- reviewed authored: +1 function / +1,681 bytes
- source-present: +1
- exclusions: +0
- exact: +0

## Verification planes

Focused validation after the edit passed:

- final tracked ReplayManager normal VC7.1 compile
- final tracked ReplayManager `/GL` compile
- full target-bound normal comparator: 1,681/1,681 owned bytes, mismatch 73/1465, acceptance none
- `python3 scripts/validate-tracking.py --require-target`
- `python3 scripts/progress.py --check`
- `python3 scripts/report-reconstruction-status.py`
- `git diff --check`

The resumed recovery session reran the final tracked `ReplayManager.cpp` under the fixed normal VC7.1 profile and the same profile plus `/GL`; both compiled successfully. It also reran the full target-bound normal comparator for `ReplayManager::SaveReplay`, reproducing **1,681/1,681 owned bytes** with a non-exact **73/1465** relocation-masked comparable result and no acceptance authority. The cold checkpoint gate then passed `verify-target.py`, `verify-toolchain.py --check`, tracking/progress checks, the 0-unit match graph, the explicitly open build graph, reconstruction status, public CI, and `git diff --check`. The honest `python3 scripts/build.py` returned the expected **RC=2** because compile flags, TU partition, libraries, resources, and link order remain open. `config/matches.csv`, `config/match-units.toml`, and `config/build.toml` remained unchanged.

The verification planes remain independent:

- source presence: **yes for this reviewed body**
- canonical exactness: **no**
- whole Windows i386 build closure: **open**
- runtime validation: **not started**
- Truth Kernel acceptance for this packet: **none established**

## Truth Kernel acceptance

The original packet's pre-checkpoint `factory_get_accepted_snapshot(th10)` attempt was unavailable because another Factory operation owned `<operator-path>`. On resume, a fresh accepted snapshot was successfully read. It is sourced from commit `9f7886f72adcd1880c05e34c62408cf9b1dad966` and contains exactly **one accepted fact / one accepted claim**, limited to `target_attested` for `target:th10-main`.

The accepted registry therefore contains **no replay source-presence, boundary, compiler-profile, ownership, or exactness acceptance** for this packet. The current source/ledger/compile conclusions remain independent local reconstruction evidence rather than Truth Kernel acceptance.

## Analysis artifacts

Campaign directory:

- `.analysis/gpt-web/20260912-replay-writer/`

Observed `.analysis/` sizes:

- session entry: **109,361 bytes**
- session peak: **248,963 bytes**
- after packet scratch cleanup: **114,289 bytes**

Retained compact campaign artifacts after cleanup:

- `compiler-shape-report.txt`: 3,290 bytes
- `manifest.json`: 1,638 bytes before final checkpoint metadata

Removed only current-session reproducible scratch after its conclusions were compressed into the report:

- bounded full replay-writer objdump
- normal and `/GL` probe objects
- final normal and `/GL` probe objects
- `/GS`, temporary fastcall, and fastcall-plus-`/GS` probe objects
- detailed comparator JSON/stderr
- temporary fastcall source copies

No artifact exceeded 64 MiB. No old/shared analysis state, target, Ghidra database, Wine prefix, toolchain, or another process's output was deleted.

## Engineering events and recovery handling

Several Factory transport/network failures occurred during this packet, including optional Ghidra caller/callee/string queries, adjacent committed-source reads, source patch commands, and scratch ABI/profile diagnostics. After every failed/timed-out/non-transactional operation, live repository status was reread before retry or continuation. Commands that might have written source or scratch were inspected before any retry so partial effects were not guessed.

One early campaign setup command exited nonzero because of a here-document boundary error after the manifest had already been written. Live status and the campaign file were inspected before the read-only ledger portion was rerun.

The first ReplayManager compiler probe failed legitimately on two negative-size offset assertions for the target timing view. No output object was assumed. After live-status recovery and artifact inspection, the target-observed four-byte packing was applied and both normal and `/GL` compiles passed.

These events affected availability/engineering flow only; they are not evidence for reconstruction or exactness.

## Next hard packet

Prefer the connected replay lifecycle/recording owner cohort rather than the easier score writer:

- `0x004294A0`, 367 bytes
- `0x00429610`, 185 bytes
- `0x004296F0`, 181 bytes
- `0x004297B0`, 18 bytes
- `0x004297D0`, 596 bytes

The `0x004296F0` body already provides direct target evidence for a `0x2D4` replay-object allocation and eight `0x24` subobjects at `+0xA0`; `0x004297D0` operates the recording/playback state that feeds SaveReplay. This cohort can close lifecycle ownership and replace the current conservative `+0xA0-0x2D3` unknown storage with evidence-backed fields where possible.

The 564-byte score writer at `0x0042B1E0-0x0042B413` remains a useful secondary write-pipeline comparison but is not preferred merely because it is smaller. The next packet should preserve the current hard-frontier balance by continuing the newly exposed Replay owner/data-layout seam.

## Checkpoint intent

The recovered packet passed its cold checkpoint gate. After full working-tree and staged-diff audit, create one local checkpoint with subject:

- `gpt-web: reconstruct TH10 replay save writer`

Do not push. The final commit hash is recorded in the ignored campaign manifest and session report after commit creation rather than self-referencing this tracked handoff.

This handoff is a continuation checkpoint only. It does not declare the TH10 exact-reconstruction phase complete.
