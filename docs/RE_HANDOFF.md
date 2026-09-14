# TH10 Exact-Reconstruction Handoff

## Checkpoint state

- Repository: `th10`
- Branch: `main`
- Session starting HEAD: `ded98252dbe09453cf575f27888eef432a0c92f3` (`gpt-web: reconstruct TH10 enemy runtime update`)
- Pre-commit HEAD while this handoff is written: `ded98252dbe09453cf575f27888eef432a0c92f3`
- Planned checkpoint subject: `gpt-web: reconstruct TH10 enemy lifecycle`
- Final checkpoint hash is intentionally not self-recorded here because the commit containing this file does not exist yet. Read live Git and the ignored campaign manifest after recovery.
- No push was performed or requested.
- Entry tracked state was clean: staged 0, unstaged 0, untracked 0, conflicts 0; `main` was ahead 9 / behind 0 relative to `origin/main`.
- No recoverable interrupted tracked work, unrelated tracked work, or unknown untracked paths were present at entry. Existing ignored `.analysis/`, `.tools/`, Wine/Ghidra/build state, caches, and the private target were preserved.
- Entry `.analysis/`: 134 regular files / 1,149,235 regular bytes / 1504 KiB allocated / 0 files larger than 64 MiB.
- Current campaign: `.analysis/gpt-web/20260914-enemy-lifecycle/`.

## Mandatory recovery and authority gates

The session re-ran `factory_describe`, `factory_list_repositories`, and `factory_get_repository_status(th10)`, inspected live branch/HEAD/history, porcelain-v2 state, staged/unstaged diffs, untracked paths, ignored state, `.analysis/`, and this handoff, then read every mandatory guidance file from the registered repository runner. No requested guidance path was missing.

An initial `factory_describe` transport/network failure returned no evidence; retry succeeded. Mandatory repository preflight then passed:

- `python3 scripts/verify-target.py`
- `python3 scripts/verify-toolchain.py --execute`
- `python3 scripts/validate-tracking.py --require-target`
- `python3 scripts/report-reconstruction-status.py`
- `python3 scripts/ci.py`
- `python3 scripts/build-match-unit.py --check`
- `python3 scripts/build.py --check`

The pinned candidate remains Microsoft Visual C++ .NET 2003 SP1-era build 6030. The execute preflight exercised normal COFF, `/GL`, resource compilation, and PE32 linking through the repository wrapper. The target remains the operator-supplied ignored file at the repository-default path; it was not modified, relocated, staged, or committed.

Native Ghidra operations were rediscovered from `th10-ghidra`. The initial `check {}` passed for `target:th10-main` with `attestation.provider_transport=factory-native-command`. Every useful native query in this packet carried the same passed attestation. A later pre-commit refresh had one transport/network failure with no attestation, then a retry passed for the exact target and transport. Native Ghidra evidence remains provisional with no exactness credit.

Target identity remains:

- SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`
- MD5 `7dc488d82c81dd4aee4ba098b8804d83`
- PE i386, image base `0x00400000`, entry `0x004537DC`

## Packet selection and balance

This session deliberately stayed on a hard, evidence-connected Enemy lifecycle seam rather than selecting small exact candidates. The packet began from the previous handoff's allocation/construction/update/teardown/death cohort and expanded only when TH10 direct callback registration and physical table ownership demanded it.

Final reviewed packet contains seven source-owned bodies/extents totaling **1980 target bytes**:

| Address | Maintained name | Reviewed target extent | State |
| --- | --- | ---: | --- |
| `0x0040CFB0` | `EnemySpawn` | 577 bytes through `0x0040D1F0` including owned switch data | authored/source-present, non-exact |
| `0x0040D750` | `EnemyManagerUpdate` | 166 bytes | authored/source-present, non-exact |
| `0x0040D810` | `EnemyManagerUpdateCallback` | 10 bytes | newly discovered candidate; authored/source-present, non-exact |
| `0x0040D820` | `EnemyManagerDrawCallback` | 6 bytes | newly discovered candidate; authored/source-present, canonical exact |
| `0x0040D830` | `EnemyConstruct` | 673 bytes | authored/source-present, non-exact |
| `0x0040DAE0` | `EnemyTeardown` | 386 bytes | authored/source-present, non-exact |
| `0x0040E5F0` | `EnemyFinalizeDeath` | 162 bytes | authored/source-present, non-exact |

This follows recent hard Player/Enemy central owners rather than a smallest-function strategy. The six-byte exact callback was accepted only because it naturally fell inside the selected lifecycle packet and survived a repeatable canonical COFF Oracle; it did not drive packet selection.

## Physical boundary and denominator corrections

### `EnemySpawn` `0x0040CFB0`

The imported Ghidra/ledger candidate was only 501 bytes through `0x0040D1A4`. Raw TH10 control flow proves that this omitted source-owned switch data:

- executable code ends at RET 8 at `0x0040D1A2-0x0040D1A4`;
- `0x0040D1A5-0x0040D1A7` is a three-byte alignment sled;
- `0x0040D1A8-0x0040D1BB` is a five-entry jump table directly indexed at `0x0040D12B`;
- `0x0040D1BC-0x0040D1F0` is the 53-byte selector table directly indexed at `0x0040D124`, consistent with the target `<= 0x34` selector range;
- `0x0040D1F1-0x0040D1FF` is CC padding.

The reviewed source-owned physical span is therefore 577 bytes, not 501. This expands the authored byte denominator by 76 bytes instead of hiding compiler-owned switch data outside the function row.

### Ghidra-missed registered callbacks

Target manager initialization directly stores:

- `0x0040D810` as the update-chain callback at priority `0x12`. Its 10-byte body saves EDI, copies conventional callback ECX into EDI, calls private-EDI owner `0x0040D750`, restores EDI, and returns. Padding follows at `0x0040D81A-0x0040D81F`.
- `0x0040D820` as the draw-chain callback at priority `0x14`. Its complete six-byte body is `mov eax,1; ret`; padding begins at `0x0040D826`.

Both were absent from the imported Ghidra candidate inventory and are now explicit denominator candidates. Other aligned raw bodies observed at `0x0040D200`, `0x0040D260`, `0x0040D710`, `0x0040D730`, `0x0040D800`, and `0x0040DC70` currently have no modeled Ghidra xref and no absolute-pointer occurrence in the bounded PE scan. They remain inventory gaps/unknown rather than being promoted from body shape alone.

## Recovered Enemy ownership, ABI, and layout

`EnemySpawn` consumes a 0x40-byte logical spawn request from live-in EAX plus manager/ECL-name stack values and returns with RET 8. It allocates exactly `0x2518` bytes, invokes the constructor, copies position/reward/life/item-drop/flags/parameters, initializes damage-reduction timer state, executes one runtime update, derives death-presentation state, and links the runtime's embedded list node into the manager.

`EnemyConstruct` receives the full object in live-in ESI plus one stack ECL subroutine name and returns with RET 4. TH10 clears exactly `0x537` dwords = `0x14DC` bytes beginning at full object `+0x103C`, exactly reaching the end of the `0x2518` allocation. Maintained assertions preserve `sizeof(EnemyRuntimeView)==0x14DC`, `offsetof(EnemyFullObjectView,runtime)==0x103C`, and `sizeof(EnemyFullObjectView)==0x2518`. Source intentionally does not hard-code target vtable addresses because original class hierarchy/compiler source ownership remains unresolved.

The constructor audit corrected two non-zero fields that cannot be represented merely by the runtime clear: runtime `+0x1400` and `+0x1404` are both initialized to `32.0f`.

The manager is maintained as an exact `0x68` view with target-established timer at `+0x40`, script database `+0x54`, enemy-list head/tail `+0x58/+0x5C`, active count `+0x60`, and spawn counter `+0x64`. The embedded enemy list node is exactly 12 bytes at runtime `+0x130` / full `+0x116C`.

`EnemyManagerUpdate` uses a private EDI manager live-in and plain RET. It iterates the manager list, runs `EnemyRuntimeUpdate`, clears the target flag on success, or dispatches the object's virtual scalar-deleting-destructor path. The primary target vtable slot `+0x14` points to wrapper `0x0040CC50`; that wrapper calls reviewed `EnemyTeardown` before optional object deallocation. Maintained source keeps wrapper/deallocation mechanics separate from the teardown body.

`EnemyTeardown` has one stack full-object argument and RET 4. It unlinks the manager node, clears special-manager ownership, processes **10** managed VM ids (the runtime-update owner moves only the first eight), marks resolved VM/child states for deletion, clears Player references, and releases the object's owned-allocation list. Source audit removed defensive null guards that were absent from the raw target VM-list walk.

Cross-object teardown evidence also closes Player shot targeting: `Player::shots[]` begins at Player `+0x49C`; teardown scans 128 rows beginning at Player `+0x4E8` with stride `0x5C`. `0x4E8 - 0x49C = 0x4C`, proving the formerly neutral `PlayerShotRuntimeView +0x4C` field is an `EnemyFullObjectView *trackedEnemy`. Teardown clears it when it points at the dying enemy. Player `+0x3504/+0x3508` remain the separately reviewed global tracked-enemy pointer/valid fields.

`EnemyFinalizeDeath` has one stack full-object argument and RET 4. It conditionally invokes target death-audio/effect paths, drops the direct item, processes the direct drop slot plus eleven item-count slots, clears the direct type, plays sound 10, and returns one. Helpers with private register live-ins are intentionally represented as descriptive logical interfaces rather than falsely claiming their original ABI.

## Maintained source changes

- `src/Enemy.hpp`
  - expanded exact Enemy runtime/full-object/manager/list/spawn-request views and assertions;
  - corrected managed VM id count to 10;
  - added item-drop/death fields and neutral `+0x1400/+0x1404` floats;
  - declared the seven reviewed lifecycle interfaces with comments separating maintained logical signatures from private machine ABIs.
- `src/Enemy.cpp`
  - added natural C++ lifecycle implementations for spawn, constructor, manager update, the two registered callbacks, teardown, and death finalization;
  - added only the partial helper/registry views necessary to express target-observed behavior;
  - preserved compiler/type uncertainty rather than embedding vtable addresses or target bytes.
- `src/Player.hpp`
  - renamed the target-proven Player-shot `+0x4C` field to `EnemyFullObjectView *trackedEnemy` and added its offset assertion.

No target bytes, fake returns, inert source padding, target patching, hard-coded vtable addresses, or assembly/oracle tricks were introduced.

## Compiler and exactness feedback

Final focused pinned-VC7.1 SP1 build6030 probes all pass:

- `src/Enemy.cpp` normal `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc`
- `src/Enemy.cpp` same profile plus `/GL`
- `src/Player.cpp` normal shared-layout sanity
- `src/Player.cpp` same profile plus `/GL`

Final true normal `/Gy` COMDAT and relocation-aware target diagnostics are:

| Function | Normal owned bytes | Target bytes | Comparable match | Relocs | Probe authority |
| --- | ---: | ---: | ---: | ---: | --- |
| `EnemySpawn` | 628 | 577 | 76 / 536 | 12 | none; mismatch |
| `EnemyManagerUpdate` | 163 | 166 | 11 / 142 | 6 | none; mismatch |
| `EnemyManagerUpdateCallback` | 10 | 10 | 1 / 6 | 1 | none; mismatch |
| `EnemyManagerDrawCallback` | 6 | 6 | 6 / 6 | 0 | direct probe has no acceptance authority |
| `EnemyConstruct` | 586 | 673 | 64 / 657 | 4 | none; mismatch |
| `EnemyTeardown` | 418 | 386 | 41 / 362 | 6 | none; mismatch |
| `EnemyFinalizeDeath` | 157 | 162 | 9 / 138 | 6 | none; mismatch |

The first/six other bodies are source-present but not canonical exact. Successful `/GL` compilation preserves an LTCG/interprocedural hypothesis; there is still no standalone target-bound linked-image extent Oracle for those bodies.

The draw callback is different: `config/match-units.toml` now defines `enemy-manager-draw-callback` with explicit `artifact_kind = "coff"`, the pinned normal profile, the exact symbol, target address `0x0040D820`, and complete six-byte extent. It has zero relocations. The object was deleted, rebuilt, and target-compared repeatedly, including after final source corrections and again in the pre-commit verification matrix. Every canonical replay returned `result="exact"`, `matched_bytes=6/6`, with a true six-byte `.text` section. This is the only exact promotion in this packet.

## Ledger state

Tracked ledger changes:

- `config/functions.csv`: corrected `0x0040CFB0` to 577 bytes; promoted the five existing lifecycle bodies to reviewed Enemy authored source; added `0x0040D810` and `0x0040D820` denominator candidates with target-proven callback registration and boundaries.
- `config/function-origins.csv`: seven lifecycle rows classified `authored_game / Enemy / authored / high` with evidence id `enemy-lifecycle-2026-09-14`.
- `config/implemented.csv`: seven maintained names added.
- `config/reccmp-functions.csv`: seven source mappings added.
- `config/match-units.toml`: one canonical normal-COFF unit for `EnemyManagerDrawCallback`.
- `config/matches.csv`: exactly one new canonical exact row, `0x0040D820`, size 6.
- `config/build.toml`: unchanged and honestly open.

Post-update reconstruction status before commit:

- candidates: **1251**
- pending: **1139**
- authored: **104** / **35,474 bytes**
- excluded: **8**
- source-present: **77**
- canonical exact: **1** / **6 bytes**

`docs/PROGRESS.md` and `resources/progress.svg` were regenerated from the ledger. Durable conclusions are recorded in `ENEMY-002` (refined), `ENEMY-003`, `ENEMY-004`, and `TOOLCHAIN-025` in `docs/KNOWLEDGE_BASE.md`.

## Adjacent-game hypothesis use

Adjacent repositories were consulted only after TH10-local observations suggested the Enemy family, and only for naming/source-shape hypotheses:

- TH08 observed HEAD `a45e99fb1942714e6edded20847e32a654d56f97`; clean. Committed Enemy source was consulted.
- TH09 observed HEAD `3a08724b6b321596905ad083cd72cf0751b09989`; `main` ahead 9; clean.
- TH095 observed HEAD `e446f055b295194d6b8d7632aeb0e0028adbc524`; `main` ahead 10 with unrelated untracked `EnemyManagerUpdate.i`, `config/runtime-scenarios.json`, `droid.resume.txt`, and `scripts/runtime-diff.py`. Those untracked paths were not read or used.

No adjacent address, extent, layout, ABI, owner, exactness, or completion fact was transferred. TH10 remained authoritative.

## Verification planes at pre-commit checkpoint

### Source presence

Seven lifecycle source mappings are reviewed and present. Two candidates (`0x0040D810`, `0x0040D820`) newly expand the function denominator; `EnemySpawn` expands its byte denominator by 76 bytes through source-owned switch data.

### Exactness

Only `EnemyManagerDrawCallback` at `0x0040D820` is canonical exact through a replayable complete normal-COFF unit. The other six reviewed lifecycle bodies are explicitly non-exact under the fixed normal diagnostic and remain exactness-unknown with respect to possible production/LTCG context.

### Whole build

`python3 scripts/build.py` was actually executed and returned **RC 2 / explicitly open**. Remaining unknown production inputs include compile flags, TU partition, libraries, resources, and link order. `python3 scripts/build.py --check` passes because this openness is declared honestly. Function exactness does not close the product graph.

### Runtime

Not performed. There is no closed faithful reconstructed product to launch, so no runtime claim is made.

### Factory Truth acceptance

A fresh read-only `factory_get_accepted_snapshot(th10)` succeeded before commit: sequence **0**, accepted facts **0**, accepted evidence manifest **0**. No Truth submission or replay was performed. The repository's one canonical exact ledger row and Factory accepted facts are separate states.

### Other final pre-commit checks already run

- target verification: passed
- toolchain declaration check: passed; entry `--execute` had already passed the full smoke suite
- tracking validation: passed
- reconstruction status: passed
- public CI: passed
- canonical match-unit graph: passed with one configured unit
- canonical draw callback rebuild/compare: exact 6/6, zero relocations
- build graph check: passed, explicitly open
- `git diff --check`: passed
- fresh Ghidra target/transport identity refresh: passed after one transport-only retry

These checks will be repeated against staged state as applicable, followed by cold post-commit verification.

## Scratch lifecycle

Entry `.analysis/`: 134 files / 1,149,235 bytes / 1504 KiB / 0 files >64 MiB.

After reconstruction and selective cleanup, before handoff/manifest finalization: 148 files / 1,232,646 bytes / 1616 KiB / 0 files >64 MiB. Current campaign is 14 files / 83,411 bytes and retains:

- final `Enemy.normal.obj` and `Enemy.ltcg.obj`;
- seven final per-function normal-vs-target comparator JSON files;
- `draw-callback-canonical-final.json` and `draw-callback-canonical-precommit.json`;
- honest whole-build stdout/stderr;
- campaign manifest.

Removed only known current-session reproducible scratch with final replacements: preliminary Enemy/Player layout objects, Player normal/LTCG shared-layout sanity objects, two older duplicate canonical replay JSON files, and the ignored generated match object. Legacy/unknown analysis state, provider databases, toolchains, Wine prefixes, targets, and other-process output were preserved.

## Next evidence-connected hard frontier

Do not optimize for easy exact callbacks after this packet. The next connected Enemy ownership packet should center on the **760-byte manager lifecycle/factory cohort** that remains `unclassified`:

- `0x0040D280-0x0040D3CD`, 334 bytes: manager initialization and callback allocation/registration; called by `0x0040D6B0`.
- `0x0040D530-0x0040D65B`, 300 bytes: manager teardown/resource cleanup with critical-section paths; called by `0x0040D6B0` rollback and `0x00417C80`.
- `0x0040D6B0-0x0040D70E`, 95 bytes: manager allocation/factory/rollback owner; directly calls the previous two and has caller `0x00417870`.
- `0x0040CC50-0x0040CC6E`, 31 bytes: scalar-deleting enemy wrapper already proven to call `EnemyTeardown` and optional free; include it to close compiler/type destruction ownership rather than counting it as an easy leaf.

The same audit should reconcile the nearby raw aligned inventory gaps `0x0040D200`, `0x0040D260`, `0x0040D710`, `0x0040D730`, `0x0040D800`, and `0x0040DC70` before deciding whether any belong in the candidate denominator. Their current lack of modeled xrefs/absolute-pointer hits is evidence for caution, not evidence that they are padding or library code.

The much larger unresolved `0x0040E770` dispatcher (13,759 bytes in the current frontier) remains a later hard target. The next session should continue challenging owner/boundary/denominator assumptions around the manager cohort before returning to that dispatcher.

This handoff is a checkpoint, not a completion declaration. TH10 exact reconstruction, faithful Windows i386 product closure, runtime validation, and later semantic/portability phases remain open.
