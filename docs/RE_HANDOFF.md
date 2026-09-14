# TH10 Exact-Reconstruction Handoff

## Checkpoint state

- Repository `th10`, branch `main`.
- Session starting/pre-commit HEAD: `bd86313544d196c25858c7ed9c57c81a2fab89cf` (`gpt-web: reconstruct TH10 enemy lifecycle`).
- Planned checkpoint subject: `gpt-web: reconstruct TH10 enemy manager lifecycle`.
- Final commit hash is intentionally not self-recorded before the commit exists; read live Git and the ignored campaign manifest after recovery.
- No push was performed or requested.
- Packet entry was clean: staged 0 / unstaged 0 / untracked 0 / conflicts 0, ahead 10 / behind 0 relative to `origin/main`.
- A later conversation resume found 9 unstaged tracked files, all coherently belonging to this packet; staged/untracked/conflicts remained zero. They were classified `recoverable-current-work` and finished first. No unrelated or unknown tracked/untracked paths were present.
- Entry `.analysis/`: 150 files / 1,238,917 bytes / 1628 KiB / 0 files >64 MiB. Resume-recovery `.analysis/`: 168 files / 1,662,528 bytes / 0 files >64 MiB.
- Campaign: `.analysis/gpt-web/20260914-enemy-manager-lifecycle/`.

## Recovery and authority gates

The session re-ran `factory_describe`, `factory_list_repositories`, `factory_get_repository_status(th10)`, inspected branch/HEAD/history, porcelain-v2 state, complete working/staged diffs, ignored state, `.analysis/`, and the previous handoff. All requested repository/Factory guidance and contracts were re-read through `factory_repository_run_shell`; no requested path was missing.

Repository preflight passed:

- `python3 scripts/verify-target.py`
- `python3 scripts/verify-toolchain.py --execute`
- `python3 scripts/validate-tracking.py --require-target`
- `python3 scripts/report-reconstruction-status.py`
- `python3 scripts/ci.py`
- `python3 scripts/build-match-unit.py --check`
- `python3 scripts/build.py --check`

The execute toolchain preflight again passed normal COFF, C++ `/GL`, resource, and PE32 i386 smoke under pinned VC7.1 SP1 build6030/headless Wine. Native `th10-ghidra` operation schemas were rediscovered and `check {}` passed for `target:th10-main` with `provider_transport=factory-native-command`. Every useful Ghidra result in this packet carried the same passed attestation.

Several Factory calls suffered transport/network failures with no command id, attestation, or semantic result. Those were treated as no evidence; live Git/status was inspected before retrying uncertain operations. No target/provider mismatch occurred.

Target remains size 487,936, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`, MD5 `7dc488d82c81dd4aee4ba098b8804d83`, PE32 i386 base `0x00400000`, entry `0x004537DC`. The ignored operator `resources/th10.exe` was not modified, moved, staged, or committed; `/mnt` was not searched and `TH10_TARGET_PATH` was not set.

## Hard packet and denominator changes

This packet deliberately continued the manager/resource ownership seam instead of selecting easy exact leaves. Direct TH10 vtable/constructor evidence expanded the initial manager cohort into the manager-owned ECL resource family.

Reviewed authored/source-present bodies total **1162 target bytes**:

| Address | Maintained name | Bytes | State |
| --- | --- | ---: | --- |
| `0x0040CD20-0x0040CD7E` | `EnemyEclResourceView::LoadFile` | 95 | source-present, non-exact |
| `0x0040D260-0x0040D27C` | `EnemyManagerView::EnemyManagerView` | 29 | new candidate, source-present, non-exact |
| `0x0040D280-0x0040D3CD` | `EnemyManagerView::Initialize` | 334 | source-present, non-exact |
| `0x0040D3D0-0x0040D3F1` | `EnemyEclResourceView::EnemyEclResourceView` | 34 | new candidate, source-present, non-exact |
| `0x0040D400-0x0040D4EE` | `EnemyEclResourceView::LoadPackage` | 239 | new candidate, source-present, non-exact |
| `0x0040D530-0x0040D65B` | `EnemyManagerView::~EnemyManagerView` | 300 | source-present, non-exact |
| `0x0040D680-0x0040D6A3` | `EnemyEclResourceView::~EnemyEclResourceView` | 36 | source-present, non-exact |
| `0x0040D6B0-0x0040D70E` | `EnemyManagerCreate` | 95 | source-present, non-exact |

Reviewed compiler-generated/excluded bodies total **89 bytes**:

- `0x0040CC50-0x0040CC6E`, 31B: Enemy scalar deleting destructor wrapper.
- `0x0040D660-0x0040D67D`, **30B**: ECL-resource scalar deleting wrapper. Final audit corrected an earlier truncated 28B window; `RET 4` occupies `0x0040D67B-0x0040D67D`, and `0x0040D67E-0x0040D67F` is padding.
- `0x0040D710-0x0040D72B`, 28B: manager scalar deleting wrapper.

`0x0040D200`, `0x0040D730`, `0x0040D800`, and `0x0040DC70` remain `unknown/review`. Their clean CC-delimited body shapes are insufficient by themselves to establish source origin.

## Manager ownership and source shape

`EnemyManagerView` is exactly `0x68` bytes: callback nodes `+0x08/+0x0C`, eight special-enemy slots `+0x10`, four effect resources `+0x30`, timer `+0x40`, ECL resource `+0x54`, list head/tail `+0x58/+0x5C`, active count `+0x60`, spawn counter `+0x64`.

`0x0040D260` receives manager in private EDX, clears timer flag bit zero at `+0x50`, zeroes all `0x1A` dwords, sets flags bit 1, publishes `g_EnemyManager`, returns the object, and ends in plain RET. The same source construction operation appears inline byte-for-byte inside `EnemyManagerCreate`, establishing authored origin beyond constructor-like shape alone.

`0x0040D280` carries manager in EBX plus one stack ECL filename and `RET 4`. It installs the primary resource, allocates/constructs the `0x1098` ECL resource, invokes its virtual file loader, registers update/draw callbacks at priorities `0x12/0x14`, and initializes the timer.

The target callback node is exactly `0x24` bytes. Source audit corrected its construction to preserve target behavior: priority zero; flags only clear bit zero; callback initially NULL; `+0x0C/+0x10` NULL; `+0x14=self`; `+0x18/+0x1C` NULL; manager owner later at `+0x20`.

`0x0040D530` carries manager in EAX and plain RET. It calls manager-clear owner `0x00409F90`, removes both callbacks under the callback critical section, frees 32 ECL script-data pointers, destroys/frees the ECL resource, clears manager `+0x54`, conditionally releases four animation-resource slots when lifecycle bits 0/3 are clear, and clears `g_EnemyManager`.

`0x0040D6B0` has one stack filename/`RET 4`: allocate `0x68`, inline manager construction, initialize, destructor/free rollback on failure, return manager on success.

## ECL resource ownership

The resource allocation is exactly `0x1098`. TH10 vtables establish a partial base/derived relationship without proving original type names:

- derived `0x0046D0B4`: `0x00450220`, package loader `0x0040D400`, file loader `0x0040CD20`;
- base `0x0046D0F0`: `0x00450220`, `0x0040C810`.

Maintained source models this with natural virtual C++ and does not embed target vptr addresses. Target teardown establishes 32 owned script-data pointers at `+0x0C..+0x88` and an owned lookup table at `+0x8C`.

Retained constructor `0x0040D3D0` pre-clears neutral dword subobjects at `+0x1090/+0x1094`, then clears the complete `0x1098`, installs the derived vptr, and returns. Maintained source uses two neutral four-byte member constructors plus a containing full-object clear; VC7.1 naturally emits the same core ordering, with only conventional ECX-to-EDX receiver setup added in fixed normal code.

`0x0040D400` is ECX-this plus one stack package pointer/`RET 4`. It parses `ANIM` magic `0x4D494E41`, loads NUL-terminated animation names starting at resource index 9, stores resources beginning at manager `effectResources[1]`, four-byte-aligns the cursor, then parses `ECLI` magic `0x494C4345` and virtually dispatches `LoadFile` for each ECL filename.

`0x0040CD20` is ECX-this plus stack filename/`RET 4`. It copies the filename into the target buffer, invokes target loader `0x0044B360`, adds/parses the returned script data, and returns 0/-1. Target calling evidence passes filename in private EAX plus two zero stack values; existing TH10-local Main evidence identifies those logical values as size-out and mode. Maintained source therefore supplies `(filename, NULL, 0)` without claiming the private EAX ABI.

`0x0040D680` carries resource in ESI, transitions to the base-state vptr, frees/nulls lookup storage at `+0x8C`, and returns. Natural derived/base VC7 destructor generation supplies the vptr transition; no target vtable bytes are hard-coded.

## Source and compiler feedback

Tracked source changes are confined to `src/Enemy.cpp` and `src/Enemy.hpp`. They add the exact callback-node partial view, the partial polymorphic ECL-resource classes, neutral constructor-visible tail dwords, resource/callback ownership views, and natural source for the eight reviewed authored bodies. No target-byte embedding, fake returns, inert padding, target patching, target-vptr constants, assembly, or Oracle-specific tricks were introduced.

Final pinned VC7.1 SP1 source compiles both fixed normal `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc` and the same profile plus `/GL`.

| Target | Normal/target bytes | Comparable match | Relocs | Result |
| --- | ---: | ---: | ---: | --- |
| `0x0040CD20` | 97 / 95 | 50 / 79 | 4 | mismatch, authority none |
| `0x0040D260` | 31 / 29 | 1 / 25 | 1 | mismatch, authority none |
| `0x0040D280` | 314 / 334 | 8 / 286 | 12 | mismatch, authority none |
| `0x0040D3D0` | 36 / 34 | 0 / 30 | 1 | mismatch, authority none |
| `0x0040D400` | 218 / 239 | 32 / 223 | 4 | mismatch, authority none |
| `0x0040D530` | 298 / 300 | 12 / 184 | 29 | mismatch, authority none |
| `0x0040D680` | 40 / 36 | 4 / 28 | 2 | mismatch, authority none |
| `0x0040D6B0` | 97 / 95 | 43 / 75 | 5 | mismatch, authority none |

Generated helpers also mismatch: normal ECL-resource deleting helper 60B versus target D660 30B (4/22, 2 relocs), manager deleting helper 30B versus target D710 28B (0/20, 2 relocs). `/GL` success preserves a possible production optimizer/LTCG context but gives no standalone exactness credit.

## Ledger and verification planes

Current ledger before commit:

- candidates **1256**
- pending **1133**
- authored **112 / 36,636 bytes**
- excluded **11**
- source-present **85**
- canonical exact **1 / 6 bytes**

Packet delta: candidates +5, pending -6, authored +8 / +1162B, excluded +3, source-present +8, exact +0. `config/matches.csv`, `config/match-units.toml`, and `config/build.toml` are unchanged. `docs/PROGRESS.md` and `resources/progress.svg` were regenerated. Durable facts are in `ENEMY-005`, `ENEMY-006`, and `TOOLCHAIN-026`; `ENEMY-004` explicitly records which older gap conclusions were superseded.

**Source presence:** eight new authored bodies mapped; three deleting wrappers excluded as compiler-generated.

**Exactness:** this packet adds no exact function. Because `src/Enemy.cpp` also owns the existing exact draw callback, `enemy-manager-draw-callback` was cold rebuilt and still compares **exact 6/6, zero relocations**. Its generated match object was then removed as reproducible scratch and must be rebuilt again at staged/post-commit validation.

**Whole build:** actual `python3 scripts/build.py` returned **RC2 / explicitly open**. Unknown production compiler flags, TU partition, libraries, resources, and link order remain blockers. `build.py --check` passes the honest open graph.

**Runtime:** not performed; there is no closed faithful reconstructed product.

**Factory Truth:** fresh `factory_get_accepted_snapshot(th10)` returned sequence **0**, accepted facts **0**, accepted evidence count **0**. No replay/submission was performed. Repository canonical exact=1 and Factory accepted=0 remain separate states.

## Adjacent hypotheses and scratch

Only committed adjacent content was consulted after TH10-local observations: TH08 `a45e99fb1942714e6edded20847e32a654d56f97` clean; TH09 `3a08724b6b321596905ad083cd72cf0751b09989` main ahead9 clean; TH095 `229999a8029b3f2810256df7074f4eb094ed7fed` main ahead13 with unrelated untracked `EnemyManagerUpdate.i`, `config/runtime-scenarios.json`, `droid.resume.txt`, `scripts/runtime-diff.py`, none of which were read or used. Adjacent source only corroborated broad source-family hypotheses; no address/layout/ABI/owner/exactness fact was transferred.

After selective cleanup, `.analysis/` is **168 files / 1,490,821 bytes / 1776 KiB / 0 files >64 MiB**. The campaign retains **18 files / 251,904 bytes**: final normal/`/GL` objects, final symbol/disassembly reports, ten final diagnostic JSONs, the draw-callback canonical regression JSON, whole-build logs, and manifest. Removed only current-session reproducible superseded probe objects/disassemblies, ten obsolete diagnostic JSONs, and the generated match object. Legacy/unknown analysis, Ghidra, toolchain, Wine, target, and shared/provider state were preserved.

## Next hard frontier

Do not pivot to easy leaves. Fresh target-attested navigation identifies `0x0040E770-0x00411D2E`, **13,759 bytes**, still `unknown/review`, as the next hard Enemy dispatcher. Ghidra reports no direct caller at its modeled entry but an attested reference from `0x0040E766`. Raw target bytes show `0x0040E760-0x0040E76A` is an 11-byte adapter (`ADD ECX,0x103C; JMP 0x0040E770`) followed by CC padding at `0x0040E76B-0x0040E76F`; earlier TH10-local vtable review places `0x0040E760` in the primary Enemy vtable, while Ghidra does not model it as a function.

The next session should re-attest Ghidra, re-read that vtable entry, reconcile the adapter's physical/source/compiler ownership and denominator treatment, then bound the 13.7 KiB dispatcher's exits, tables, relocations, and ABI before source promotion. `0x00409F90` is useful context but should not replace this central dispatcher merely because it is smaller.

This checkpoint leaves the exact-reconstruction campaign active. Faithful Windows i386 product closure, runtime validation, Factory acceptance, semantic reconstruction, and portability remain open.
