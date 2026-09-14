# TH10 Exact-Reconstruction Handoff

## Checkpoint state

- Repository `th10`, branch `main`.
- Session starting/pre-commit HEAD: `9f6cd941d089eca7d06df48ea6a47ae6a2d75eec` (`gpt-web: reconstruct TH10 enemy manager lifecycle`).
- Planned checkpoint subject: `gpt-web: reconstruct TH10 enemy ECL dispatch seam`.
- The final checkpoint hash is intentionally not self-recorded before the commit exists; recover it from live Git and the ignored campaign manifest.
- No push was performed or requested.
- Entry was clean: staged 0 / unstaged 0 / untracked 0 / conflicts 0, `main` ahead 11 / behind 0 relative to `origin/main`. No recoverable, unrelated, or unknown dirty path existed at entry.
- Entry `.analysis/`: 168 files / 1,491,112 regular bytes / 1912 KiB allocated / 0 files over 64 MiB.
- Campaign: `.analysis/gpt-web/20260914-enemy-dispatcher/`.

## Recovery, guidance, and authority

The session called `factory_describe`, `factory_list_repositories`, and `factory_get_repository_status(th10)`, then independently inspected branch, HEAD, recent commits, porcelain-v2 state, staged/unstaged diffs, untracked files, relevant ignored state, build state, `.analysis/`, and this handoff. The first repository-status call was a transport failure with no status result; the retry and repository-runner inspection both confirmed the clean checkpoint. No failed command was treated as rollback evidence.

All requested repository and Factory guidance/contracts were re-read completely through `factory_repository_run_shell`: `AGENTS.md`, `docs/RE_HANDOFF.md`, `docs/RE_WORKFLOW.md`, `docs/ORACLES.md`, `docs/TOOLS.md`, both reconstruction contracts, and the Factory ontology, verification-planes, autonomy, bootstrap, and analysis-artifact documents. No requested path was missing.

Entry repository preflights passed:

- `python3 scripts/verify-target.py`
- `python3 scripts/verify-toolchain.py --execute`
- `python3 scripts/validate-tracking.py --require-target`
- `python3 scripts/report-reconstruction-status.py`
- `python3 scripts/ci.py`
- `python3 scripts/build-match-unit.py --check`
- `python3 scripts/build.py --check`

The pinned VC7.1 SP1 build6030 execute preflight again passed normal COFF, C++ `/GL`, resource, and PE32 i386 smoke under the headless-Wine wrapper. Native `th10-ghidra` operation schemas were rediscovered before use. Fresh `check {}` passed for `target:th10-main` with `attestation.provider_transport=factory-native-command`, and every useful Ghidra result used below carried the same passed target/transport attestation. A few provider/Factory calls suffered transport failures; they produced no semantic evidence and were retried only after live-state reasoning.

Target identity remains size 487,936, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`, MD5 `7dc488d82c81dd4aee4ba098b8804d83`, PE32 i386 base `0x00400000`, entry `0x004537DC`. The ignored operator file `resources/th10.exe` was not modified, moved, staged, or committed; `/mnt` was not searched and `TH10_TARGET_PATH` was not set.

## Hard packet: Enemy ECL dispatch seam

This session deliberately attacked the previously handed-off central Enemy dispatcher instead of selecting easy leaves. The initial bounded packet was `0x0040E760` plus Ghidra's provisional `0x0040E770-0x00411D2E` 13,759-byte function. The observable goals were to reconcile vtable ownership, machine ABI, full source-owned physical extent, compiler switch data, source feasibility, and exactness independently.

### Vtable ownership correction

Direct target `.rdata`, constructor, destructor, and base-constructor evidence corrects the previous handoff's “twelve-slot primary Enemy vtable” interpretation. There are two distinct six-slot tables:

- derived Enemy ECL-evaluation vtable `0x0046D0C0-0x0046D0D4`:
  `0x0040E760`, `0x00411FC0`, `0x00412300`, `0x00412350`, `0x004126D0`, `0x0040CC50`;
- separate base-state vtable `0x0046D0D8-0x0046D0EC`:
  `0x0040C5E0`, `0x0040C690`, `0x0040C6A0`, `0x0040C6B0`, `0x0040C6C0`, `0x0040C7B0`.

`EnemyConstruct` writes `0x0046D0C0` at full-object `+0x00`, teardown restores it before the base destructor path, and the base constructor at `0x0040C710` writes `0x0046D0D8`. The easy base stubs remain a denominator gap but were intentionally not selected merely to increase function count.

### Slot zero and runtime dispatcher

Raw target bytes close `0x0040E760-0x0040E76A` as an 11-byte forwarding entry followed by CC padding through `0x0040E76F`:

- `ADD ECX,0x103C`
- tail `JMP 0x0040E770`.

Ghidra does not model `0x0040E760` as a function; its only attested reference is the derived vtable slot at `0x0046D0C0`. The `+0x103C` runtime tail is constructor-cleared data with no target vptr, so this is not an automatic multiple-inheritance adjustment to another polymorphic base. A source-level forwarding expression or equivalent logic is required; the original class/identifier spelling remains unknown.

`0x0040E770` has machine boundary `ECX EnemyRuntimeView* / RET`. It immediately recovers the full owner from runtime `+0x14D8`, reads the current ECL instruction, and dispatches opcodes `0x100-0x1B4` across Enemy-specific spawn, motion/interpolation, hitbox/flags/life, item/drop, difficulty/rank, VM/effect, and child operations. Shared successful paths return zero; target-observed conditional instruction paths can return `-1`. It is maintained descriptively as `EnemyRuntimeView::DispatchEclInstruction`; this is not a claim of the original identifier.

Most importantly, the source-owned physical extent is **not** Ghidra's 13,759 reachable-code bytes. The reviewed owner is `0x0040E770-0x00411FBF` = **14,416 bytes**:

- executable code through final `RET` at `0x00411D2E`;
- one-byte alignment at `0x00411D2F`;
- `0x00411D30-0x00411EDF`: directly indexed 108-entry jump table, 432 bytes;
- `0x00411EE0-0x00411F94`: directly indexed 181-byte opcode-to-case selector;
- `0x00411F95-0x00411F97`: three-byte alignment;
- `0x00411F98-0x00411FAB`: directly indexed five-entry nested jump table;
- `0x00411FAC-0x00411FBF`: second directly indexed five-entry nested jump table.

The next independent virtual method begins exactly at `0x00411FC0`. The maintained source declares the runtime method but does not implement this 14,416-byte owner; source presence and exactness remain unknown for it.

### Derived operand interface denominator

The same derived vtable supplied four additional Ghidra-missed authored candidates whose code/table boundaries are closed by raw target control/data references. All four remain source-absent and non-exact:

| Address / reviewed span | Maintained descriptive name | Bytes | Boundary / role |
| --- | --- | ---: | --- |
| `0x00411FC0-0x004122F3` | `EnemyFullObjectView::ReadIntOperand` | 820 | ECX full Enemy + one stack operand / `RET 4`; 51-entry selector table included |
| `0x00412300-0x0041234F` | `EnemyFullObjectView::ResolveIntOperand` | 80 | ECX full Enemy + one stack operand / `RET 4`; four writable int lvalues at `+0x1138..+0x1144` |
| `0x00412350-0x004126C3` | `EnemyFullObjectView::ReadFloatOperand` | 884 | ECX full Enemy + one stack operand / `RET 4`; 51-entry selector table included, float in ST0 |
| `0x004126D0-0x0041271F` | `EnemyFullObjectView::ResolveFloatOperand` | 80 | ECX full Enemy + one stack operand / `RET 4`; four writable float lvalues at `+0x1148..+0x1154` |

These functions were added to the reviewed authored denominator without inventing source implementations.

## Natural VC7 forwarder and canonical exactness

A campaign-only source-shape probe first established that an ordinary VC7.1 virtual forwarder with a member at `+0x103C` naturally emits exactly `ADD ECX,0x103C; JMP member_method`, with one REL32 relocation and no assembly or byte embedding. Maintained source therefore adds only the natural forwarding method:

```cpp
int EnemyFullObjectView::DispatchEclInstruction()
{
    return runtime.DispatchEclInstruction();
}
```

The full maintained `src/Enemy.cpp` builds successfully under fixed normal `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc` and the same profile plus `/GL`. Its normal COMDAT for `?DispatchEclInstruction@EnemyFullObjectView@@QAEHXZ` is exactly 11 bytes; all seven non-relocation bytes match the target and the sole REL32 is at offset `+7` against the unresolved runtime method.

Canonical unit `enemy-ecl-dispatch-forwarder` explicitly declares `artifact_kind="coff"`, target `0x0040E760`, size 11, and replays that REL32 to reviewed target `0x0040E770`. The generated object was deleted and rebuilt from scratch twice; both target-bound comparisons returned **exact 11/11 bytes**, one relocation replayed, zero mismatch, and identical JSON SHA-256 `2393f11ec0cc7434e7d0e2613abc3c38217151bddc9a315ed0a00400b2853db0`. Only after those two independent replays was `config/matches.csv` promoted.

Because `src/Enemy.cpp` also owns the pre-existing `EnemyManagerDrawCallback` exact unit, that unit was deleted/rebuilt under the modified source and remains **exact 6/6 bytes, zero relocations**.

No exact credit is assigned to the 14,416-byte runtime dispatcher or the four operand methods. Successful `/GL` compilation is only production-optimizer-context evidence and is not a standalone exact Oracle.

## Adjacent-game hypothesis discipline

Committed adjacent content was consulted only after TH10-local vtable and ECL evidence had established the seam:

- TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97`, committed source only;
- TH09 HEAD `3a08724b6b321596905ad083cd72cf0751b09989`; no useful ECL implementation was transferred;
- TH095 HEAD `3c54464e6d567a37219b31c4ee8bc49e033b61c1`; unrelated untracked `EnemyManagerUpdate.i`, `config/runtime-scenarios.json`, `droid.resume.txt`, and `scripts/runtime-diff.py` were observed in status but not read or used.

TH08/TH095 committed ECL source only supported the broad execute/operand-family hypothesis. No adjacent address, extent, owner, layout, name, ABI, phase state, or exactness claim was transferred. A target source-path string near `0x0046D0F8` had no attested code xref and was not used to claim translation-unit ownership.

## Ledger delta and changed tracked files

Entry ledger:

- candidates 1256
- pending 1133
- authored 112 / 36,636 bytes
- excluded 11
- source-present 85
- canonical exact 1 / 6 bytes

Current ledger:

- candidates **1261**
- pending **1132**
- authored **118 / 52,927 bytes**
- excluded **11**
- source-present **86**
- canonical exact **2 / 17 bytes**

Delta: candidates +5, pending -1, authored +6 / +16,291 bytes, excluded +0, source-present +1, exact +1 / +11 bytes. The exact percentages decrease after the large authored-boundary expansion; that is intentional and more truthful than omitting hard code from the denominator.

Tracked packet files before staging are:

- `src/Enemy.hpp`
- `src/Enemy.cpp`
- `config/functions.csv`
- `config/function-origins.csv`
- `config/implemented.csv`
- `config/reccmp-functions.csv`
- `config/match-units.toml`
- `config/matches.csv`
- `docs/KNOWLEDGE_BASE.md`
- `docs/PROGRESS.md`
- `resources/progress.svg`
- this `docs/RE_HANDOFF.md`.

`config/build.toml` remains unchanged and honestly open. Durable conclusions are recorded as `ENEMY-007` and `TOOLCHAIN-027` in `docs/KNOWLEDGE_BASE.md`; generated progress was regenerated from the ledger.

## Verification planes

**Source presence:** the 11-byte full-object forwarding method is newly source-present. The 14,416-byte runtime dispatcher and four adjacent operand methods are reviewed authored but source-absent. Their declarations/names are descriptive interfaces, not claims that the bodies were reconstructed.

**Exactness:** the new forwarder has repeatable canonical normal-COFF target-bound exactness, 11/11 bytes with its sole REL32 replayed. The existing draw callback remains exact 6/6. Repository total is two exact functions / 17 bytes. Ghidra observations and structural probes alone received no exactness credit.

**Whole build:** an actual `python3 scripts/build.py` run returned **RC2 / explicitly open**. Missing production compiler flags, translation-unit partition, libraries, resources, and link order remain blockers. `python3 scripts/build.py --check` passes the honest open graph.

**Runtime:** not performed; there is no closed faithful reconstructed Windows i386 product to launch.

**Factory Truth:** a fresh pre-checkpoint `factory_get_accepted_snapshot(th10)` returned sequence **0**, accepted 0, evidence 0, rejected 0, pending 0. No Truth submission or replay was performed. Repository canonical exactness and Factory acceptance remain separate planes.

**Ghidra:** fresh pre-checkpoint `check {}` passed for `target:th10-main` with `provider_transport=factory-native-command` after the packet edits. No target/provider mismatch occurred.

Working-tree validation after the source/ledger edits passed target verification, toolchain declaration check, tracking/status, public CI, two-unit match-graph validation, cold rebuild/comparison of both canonical exact units, fixed normal and `/GL` compilation of `src/Enemy.cpp`, build-graph check, and `git diff --check`. The actual whole-build diagnostic returned the expected RC2/open result described above. Staged and post-commit cold validation must still be read from the live session/manifest when recovering a completed checkpoint rather than inferred from this pre-commit text.

## Scratch lifecycle

After working validation but before selective cleanup, `.analysis/` reached 194 files / 1,925,706 regular bytes / 2408 KiB, with no file over 64 MiB. Only current-session reproducible duplicates were removed: the second `Enemy.working.normal.obj` / `Enemy.working.ltcg.obj`, duplicate working canonical JSONs, and generated `build/match` objects that staged validation will cold rebuild. No legacy/unknown analysis, Ghidra state, toolchain, Wine prefix, target, or shared/provider state was deleted.

Pre-staging `.analysis/` after cleanup is **190 files / 1,846,954 regular bytes / 2320 KiB / 0 files over 64 MiB**. The campaign currently retains 22 files: final normal/`/GL` Enemy objects, the natural forwarder probe source/object/disassembly, forwarder symbol/disassembly reports, structural and repeated canonical comparator JSONs, existing exact-regression JSON, raw entry/tail/vtable/vptr/large-slot/small-slot/dispatcher target evidence, whole-build logs, and its manifest. The ignored manifest must be rebound to the actual checkpoint hash after commit.

## Next evidence-connected hard packet

Do not switch to the easy zero/NULL base virtual stubs merely to improve function count. The strongest continuation is the **derived operand-resolver cohort** immediately following the dispatcher: `0x00411FC0`, `0x00412300`, `0x00412350`, and `0x004126D0`, totaling **1,864 reviewed source-owned bytes**. Two members contain 51-way compiler switch tables and target-global/helper dependencies; the other two expose four writable integer and four writable float ECL lvalues. Recovering their natural source, layouts, and compiler-sensitive selectors will type the interface used by the still-source-absent 14,416-byte `EnemyRuntimeView::DispatchEclInstruction` and provide a stronger bridge into reconstructing that central owner.

Packet-selection balance remains hard-frontier oriented: this session attacked a 14.4 KiB central dispatcher seam, expanded the authored denominator by 16,291 bytes, and accepted only one small exact forwarding method because it was structurally connected and canonically provable. The next proposed cohort is connected, switch-heavy, and owner-sensitive rather than the easiest remaining code.

The TH10 exact-reconstruction phase remains active and incomplete. Faithful whole-product closure, runtime validation, Factory acceptance, semantic reconstruction, and portability all remain open.
