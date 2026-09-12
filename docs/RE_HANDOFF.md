# TH10 exact-reconstruction handoff

## Session status

Exact reconstruction remains **active and incomplete**. This checkpoint advances the replay lifecycle/recording-owner seam; it is not a whole-executable, whole-build, runtime, semantic, or port completion claim.

Repository selection for this work is fixed to:

- repository: `th10`
- analysis provider: `th10-ghidra`
- target identity: `target:th10-main`
- private target: `resources/th10.exe` at the repository-default operator path
- toolchain candidate: pinned VC7.1 SP1-era build 6030 environment

The private target remained ignored and was never modified, relocated, staged, or committed. No push was performed.

## Recovery and starting state

This session began from live HEAD:

- `7a133c895c56e8a8aef2cece4279ee575afb1823`
- branch `main`
- upstream `origin/main`
- ahead 1 / behind 0
- 0 staged / 0 unstaged / 0 untracked / 0 conflicts

The full recovery gate inspected recent history, porcelain-v2 status, staged and unstaged diffs, untracked paths, handoff state, and relevant ignored state. There was no interrupted tracked/untracked work to recover.

Ignored state was preserved and excluded from Git work:

- `resources/th10.exe`: operator-supplied private target
- `.tools/`: shared pinned toolchain and Wine state
- `ghidra-project/`: target-bound provider state
- `build/`: generated/open build state
- `.analysis/`: prior/shared/legacy plus bounded current-session scratch

The session-entry `.analysis/` size was **294,912 bytes**. Pre-existing ignored content was treated as preserve/unknown-or-shared rather than current-session-owned.

All required repository guidance, both Factory contracts, all required Factory docs, `semantic-reconstruction.md`, and `.agents/skills/th10-reconstruction/SKILL.md` were re-read from the live repository/Factory filesystem before source edits. Several long-output Factory transport failures occurred while reading or querying; after each failure live repository status was reread before retry or continuation.

## Mandatory preflight

Entry preflights passed:

- `python3 scripts/verify-target.py`
- `python3 scripts/verify-toolchain.py --execute`
- `python3 scripts/validate-tracking.py --require-target`
- `python3 scripts/report-reconstruction-status.py`
- `python3 scripts/ci.py`

Target identity remained:

- size 487,936
- SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`
- MD5 `7dc488d82c81dd4aee4ba098b8804d83`
- PE32 i386, 4 sections, image base `0x00400000`, entry `0x004537DC`
- Rich checksum `0x3152A748`, dominant build 6030

`th10-ghidra` operations were rediscovered from their schemas. The mandatory `check {}` passed with exact `target:th10-main`, `attestation.status=passed`, and `attestation.provider_transport=factory-native-command`. Every useful Ghidra result used in this packet carried the same passed target/native-transport attestation and `exactness_credit=none`.

## Hard packet: replay lifecycle and recording owner

The packet was selected from the live prior handoff because it surrounds the already reviewed 1,681-byte replay writer, closes the 0x2D4 replay-object lifetime, and resolves the recording/playback state that feeds that writer. It is a hard owner/layout frontier rather than a smallest-function or easy-exact strategy.

Reviewed authored target bodies:

- `0x004294A0-0x0042960E`, 367 bytes: maintained `ReplayManager::~ReplayManager`
- `0x00429610-0x004296C8`, 185 bytes: maintained `ReplayManager::Create`
- `0x004296F0-0x004297A4`, 181 bytes: maintained `ReplayManager::Load`
- `0x004297B0-0x004297C1`, 18 bytes: maintained `ReplayManager::Destroy`
- `0x004297D0-0x00429A23`, 596 bytes: maintained `ReplayManager::ProcessFrame`

A sixth body was discovered by physical boundary review even though it was absent from the initial Ghidra candidate denominator:

- `0x004296D0-0x004296E8`, 25 bytes: compiler-generated scalar-deleting-destructor shape, classified as an exclusion rather than authored game code.

The five maintained names are descriptive reconstruction names chosen only after TH10-local behavior/calls/layout were recovered. Committed adjacent source was used later for naming/source-shape corroboration. No original identifier, decorated symbol, translation-unit owner, source calling convention, or physical compiler owner is claimed.

### Physical boundaries and machine ABI

Observed boundary facts:

- `0x004294A0` ends in `ret 4`; one `CC` byte separates it from `0x00429610`.
- `0x00429610` ends in `ret 8`; `CC` padding continues through `0x004296CF`.
- `0x004296D0-0x004296E8` is independently CC-delimited and ends in `ret 4`.
- `0x004296F0` ends in `ret 4`; padding continues through `0x004297AF`.
- `0x004297B0` ends in plain `ret`; padding continues through `0x004297CF`.
- `0x004297D0` ends in plain `ret` at `0x00429A23`.

The per-frame callback wrapper at `0x0042A3D0` receives the replay owner in ECX, moves it to EDI, calls `0x004297D0`, restores EDI, and returns. The two normal business-path callers of `0x004297B0` load the object being replaced into ESI before calling the null-safe destroy body. These are machine-ABI observations only; maintained source intentionally remains ordinary C++.

### Destructor / factories / destroy helper

`0x004294A0` frees the replay file-header allocation, clears/frees each of eight stage frame lists, releases replay payload and stage-header allocations, cuts three chain elements when present, clears the global replay owner when it refers to this object, and runs destructor logic over eight 0x24 stage-state objects beginning at `+0xA0`.

`0x00429610` allocates a 0x2D4 object, constructs the eight 0x24 stage states, clears the object, invokes the shared mode/path initializer at `0x00428F60`, and destroys/frees the object on initialization failure.

`0x004296F0` performs the same allocation/construction family, establishes mode 2, invokes the replay-load path, and destroys/frees the object on load failure.

`0x004297B0` is a null-safe delete body. Its two reviewed UI/menu call sites first remove an old replay object from a slot, call this body, then save and reload the same slot. This normal business-path use plus committed adjacent corroboration supports the maintained `ReplayManager::Destroy` name without proving the original source symbol.

### Newly discovered compiler-generated body

`0x004296D0-0x004296E8`:

- calls the reviewed replay-manager destructor;
- tests deleting flag bit 0;
- conditionally frees the object;
- returns the original object pointer;
- ends in `ret 4`.

No direct Ghidra xref and no whole-target raw little-endian VA reference were observed for this entry. A natural pinned VC7.1 non-virtual class with ordinary `delete` expressions automatically emits `??_GReplayManager@@QAEPAXI@Z`. That compiler behavior corroborates the target body's scalar-deleting-destructor origin class. The target and normal-object shapes still differ, so this creates an exclusion only, not an exact result.

### ReplayManager target-local layout

The allocation/clear sites establish an exact maintained object size of **0x2D4**. Fields supported directly by TH10 accesses are:

- `+0x08`: chain pointer used by destructor
- `+0x0C`: chain pointer used by destructor
- `+0x10`: replay mode
- `+0x14`: `ReplayFileHeader *`
- `+0x18`: replay-data allocation/base
- `+0x1C`: eight stage-header pointers
- `+0x3C`: eight 0x0C intrusive frame-list roots
- `+0x9C`: current recording-frame link
- `+0xA0`: eight 0x24 stage states
- `+0x1C0`: decompressed replay payload/base used by load-side code
- `+0x1C4`: replay FPS byte
- `+0x1C8`: frame counter
- `+0x1CC`: chain pointer
- `+0x1D0`: active stage
- `+0x1D4`: 0x100-byte replay path through object end

Each 0x24 stage state contains:

- record-stream start/cursor at `+0x00/+0x04`
- FPS-stream start/cursor at `+0x08/+0x0C`
- stage-header pointer at `+0x10`
- record index at `+0x14`
- embedded 0x0C intrusive node at `+0x18`

Raw target bodies at `0x0042AC20` and `0x0042AC60` provide constructor/destructor behavior for this 0x24 state even though those raw labels are not promoted by this packet.

### Runtime recording frame layout correction

The previous handoff conservatively stopped at a 0x6278 payload view. Connected allocation/free analysis now closes the runtime object:

- 3,600 six-byte records through `+0x545F`
- record-end pointer `+0x5460`
- 3,600 one-byte FPS samples starting `+0x5464`
- FPS-end pointer `+0x6274`
- embedded 0x0C intrusive node at `+0x6278/+0x627C/+0x6280`
- total allocation: **0x6284**

`0x0042AA50` allocates and initializes this exact 0x6284 block; `0x0042AB20` walks/unlinks/frees the per-stage intrusive lists. SaveReplay serializes the record/sample streams, not the embedded runtime list node.

### ProcessFrame behavior

`0x004297D0` is the target-local record/playback per-frame dispatcher.

Record mode:

- carries previous/current input state and masks live input with `0x01F7`;
- applies the observed `0x0200` hold behavior and clamps its counter at 8 while adding bit `0x0004`;
- calls the reviewed input-source updater;
- every 30 frames samples FPS using target constants `0.5f` and `256.0f`, writes one byte, and advances the FPS end cursor;
- appends three u16 values as one six-byte replay record;
- when the current frame block fills, obtains another 0x6284 block and replaces the manager's `+0x9C` current link.

Playback mode:

- uses `+0x1D0` as the selected stage;
- consumes six-byte records through that stage's 0x24 state;
- updates replay FPS from the current FPS byte every frame while advancing the FPS cursor every 30 frames;
- clears current input/auxiliary/flag values when the record stream is exhausted or no active stage is selected;
- advances stage-record and manager-frame counters.

## Maintained source

`src/ReplayManager.hpp` now replaces the previous conservative/partly incorrect replay-list view with target-supported types and offsets:

- `ReplayRecData`: three u16 values, size 6
- `ReplayListNode`: value/next/previous, size 0x0C
- `ReplayFrameData`: runtime size 0x6284 with embedded node at 0x6278
- `ReplayStageState`: size 0x24 with target-supported cursor/header/index/node layout
- `ReplayManager`: size 0x2D4 with compile-time assertions for the reviewed field offsets

`src/ReplayManager.cpp` adds natural implementations of:

- `ReplayStageState` constructor/destructor
- `ReplayManager` constructor/destructor
- `ReplayManager::Create`
- `ReplayManager::Load`
- `ReplayManager::Destroy`
- `ReplayManager::ProcessFrame`

It also updates the existing SaveReplay traversal to consume the target-observed intrusive frame-list roots and node values.

The source intentionally leaves connected helper implementations such as the shared initializer, load body, frame allocator/free helpers, input updater, and chain operations as descriptive externals when their independent origin/source ownership has not been reviewed. No target bytes, fake returns, inert padding, target patching, or private-register source ABI tricks were added.

## Adjacent-game hypothesis provenance

Only committed adjacent content was used, and only after TH10-local behavior had been recovered.

- TH08: HEAD `a45e99fb1942714e6edded20847e32a654d56f97`, branch `port/portable-64bit`, clean. Committed ReplayManager source supplied naming-family corroboration; its object layout materially differs.
- TH09: HEAD `5b77d78f57bb0cdb6b5d7eaaf8091c0084736be7`, branch `main`, ahead 1, clean in the observed status. No TH09 address/layout was transferred.
- TH095: HEAD `4c9cfb5085f34feabbb30a3cbff624eb11444450`, branch `main`. Unrelated tracked and untracked work was present and preserved; none of that uncommitted content was read. Only `git show HEAD:src/ReplayManager.hpp/.cpp` was consulted. Committed TH095 contains the analogous `Initialize`, `LoadReplay`, writer, destructor, `Create`, `Load`, `Destroy`, and `ProcessFrame` family, but its manager size/layout differs materially.

No adjacent address, target extent, data ownership, TU ownership, ABI, exactness, or completion claim was transferred into TH10.

## Compiler and exactness feedback

Final maintained `src/ReplayManager.cpp` compiles successfully with pinned VC7.1 SP1 build 6030 using:

- fixed normal profile `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc`
- the same source/profile plus `/GL`

True normal `/Gy` text-section extents from the final source are:

| Body | Normal section | Target |
| --- | ---: | ---: |
| `ReplayManager::~ReplayManager` | 199 | 367 |
| `ReplayManager::Create` | 80 | 185 |
| synthesized scalar deleting destructor | 30 | 25 |
| `ReplayManager::Load` | 77 | 181 |
| `ReplayManager::Destroy` | 27 | 18 |
| `ReplayManager::ProcessFrame` | 456 | 596 |

The standalone normal source therefore has real extent/codegen mismatches. `/GL` feasibility preserves an LTCG/interprocedural hypothesis but has no target-bound linked-image extent authority.

An important diagnostic correction was made during this packet: probe-mode `scripts/compare-coff-function.py` can use its caller-supplied expected size as the extraction window when an old VC7.1 function symbol lacks an auxiliary definition extent. A probe JSON whose `object_size` equals the supplied target size is therefore not, by itself, evidence of the true COFF-owned function extent. This packet uses `/Gy` section sizes above for the normal extent diagnostic. The misleading fixed-window JSONs were treated as reproducible scratch and removed after the conclusion was recorded.

No canonical match unit or exact ledger row was added. `config/matches.csv`, `config/match-units.toml`, and `config/build.toml` remain unchanged.

## Durable ledger delta

Starting status:

- candidates: 1,229
- pending origin/boundary review: 1,148
- reviewed authored: 75 functions / 15,545 bytes
- exclusions: 6
- source-present mappings: 48
- canonical exact: 0 functions / 0 bytes

Current post-packet status before checkpoint:

- candidates: **1,230**
- pending: **1,143**
- reviewed authored: **80 functions / 16,892 bytes**
- exclusions: **7**
- source-present mappings: **53**
- canonical exact: **0 functions / 0 bytes**
- canonical normal-COFF match units: **0**
- whole Windows i386 build: **open**
- runtime validation: **not started**
- semantic reconstruction / ports: **not started**

Packet delta:

- candidates: +1
- pending: -5
- reviewed authored: +5 functions / +1,347 bytes
- exclusions: +1
- source-present mappings: +5
- exact: +0

The new denominator candidate is the 25-byte compiler-generated body at `0x004296D0`; it is not counted as authored code.

## Tracked paths in this packet

Intended tracked paths are:

- `config/function-origins.csv`
- `config/functions.csv`
- `config/implemented.csv`
- `config/reccmp-functions.csv`
- `docs/KNOWLEDGE_BASE.md`
- `docs/PROGRESS.md`
- `docs/RE_HANDOFF.md`
- `resources/progress.svg`
- `src/ReplayManager.cpp`
- `src/ReplayManager.hpp`

No exact/build control-plane file is intended to change.

## Verification planes

The pre-checkpoint cold gate has passed on the final maintained source and ledgers:

- entry `verify-target.py`, `verify-toolchain.py --execute`, tracking/status, and public-CI preflight
- mandatory native Ghidra target/provider check
- final-source normal VC7.1 ReplayManager compile
- final-source ReplayManager `/GL` compile
- independent `/Gy` section-size reproduction for the six lifecycle/compiler bodies
- `verify-target.py`
- `verify-toolchain.py --check`
- `validate-tracking.py --require-target`
- `progress.py --check`
- `build-match-unit.py --check` with zero canonical match units
- `build.py --check`, which correctly reports the build graph open
- honest `build.py`, which returns the expected RC=2 because compile flags, translation-unit partition, libraries, resources, and link order remain unknown
- reconstruction-status generation
- public CI
- `git diff --check`
- exact/build control-plane diff check: `config/matches.csv`, `config/match-units.toml`, and `config/build.toml` are unchanged

A fresh dirty-state Ghidra `check {}` also passed after the cold gate with exact `target:th10-main`, `attestation.status=passed`, `provider_transport=factory-native-command`, and `exactness_credit=none`; its observed timestamp was `2026-09-13T07:18:26.448020Z`.

The remaining pre-checkpoint work is repository diff/index audit and staging. After commit, rerun the cold gate, Ghidra attestation, and independent Truth snapshot when available. Source presence, exactness, whole-build closure, runtime validation, and Truth acceptance remain separate states.

## Truth Kernel

The current-session pre-checkpoint accepted snapshot succeeded. Its source commit is `9f7886f72adcd1880c05e34c62408cf9b1dad966` and the registry contains exactly one accepted fact / one accepted claim: the prior target-bound scope `target_attested:target:th10-main` produced through the TH10 Ghidra provider. It contains no replay-lifecycle source, boundary, ownership, compiler-profile, exactness, build, or runtime acceptance. Therefore this packet has **no newly observed accepted claim**.

A fresh post-commit accepted snapshot must still be attempted. This pre-checkpoint registry observation must not be reused as proof of post-commit acceptance if that later refresh is unavailable.

## Analysis artifacts

Current-session campaign:

- `.analysis/gpt-web/20260913-replay-lifecycle/`

Observed sizes:

- session-entry `.analysis/`: **294,912 bytes**
- observed session peak so far: **380,928 bytes**
- after current-session scratch cleanup: **311,296 bytes**

Retained current-session files after cleanup:

- `compiler-shape-report.txt`: 4,333 bytes
- `manifest.json`: current campaign manifest

Removed only reproducible current-session scratch:

- normal and `/GL` ReplayManager objects/PDBs
- temporary origin-probe source/objects/PDBs
- detailed fixed-window comparator JSON files

No current campaign artifact exceeded 64 MiB. No pre-existing/shared analysis tree, target, Ghidra project, toolchain, Wine prefix, or unknown artifact was removed.

## Next hard packet

Prefer the shared replay initializer centered on **`0x00428F60-0x0042949F` (1,344 bytes)**, with only its necessary connected load/recording context such as `0x0042A200` and the reviewed frame-list helpers.

Why this is the next hard frontier:

- it is the common mode/path owner used by the newly reviewed Create path;
- it establishes record/playback/load-only initialization and global-owner publication;
- it allocates and initializes the same stage headers, frame lists, stage states, and callback/chain ownership resolved in this packet;
- closing it can replace current descriptive external declarations with target-reviewed source while challenging the central owner boundary;
- it is materially larger and more connected than available score/write leaf work, so it preserves the mixed hard-frontier balance rather than optimizing function count.

Do not assume an old `0x00429240` subfunction: current Ghidra analysis identifies `0x00428F60-0x0042949F` as one function, and addresses inside that range are context within that body unless new physical evidence proves otherwise.

## Checkpoint intent

After the remaining cold/Truth/diff gates pass, create one local checkpoint with subject:

- `gpt-web: reconstruct TH10 replay lifecycle`

Do not push. Record the resulting commit hash in the ignored campaign manifest and final session report rather than attempting a self-referential tracked handoff commit hash.

This handoff is a continuation checkpoint only and does not declare the TH10 exact-reconstruction phase complete.
