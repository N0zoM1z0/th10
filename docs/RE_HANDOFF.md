# TH10 reconstruction — current agent handoff

**Updated 2026-11-11.** Read [AGENTS.md](../AGENTS.md), then the selected owner's
source, ledger rows and comparison recipe. The latest exact source checkpoint
is `558e58a`; later storage/documentation cleanup does not change exact credit.

## Resume and trust sources

Start with `git status --short --branch` and `git log -8 --oneline`.
Preserve and continue any dirty/untracked work. Keep commits local; never push.

The only target is the Japanese TH10 v1.00a executable at `resources/th10.exe`,
487,936 bytes, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Use pinned VC7.1 SP1 and the source-specific /GL graph. Git does not carry
the ignored target, private toolchain, Ghidra project, reference checkout,
current native contexts. [RE_ARTIFACTS.md](RE_ARTIFACTS.md) lists the retained
private inputs and five unresolved compiler contexts.

The authority for accepted exactness is `config/matches.csv`; generated
[PROGRESS.md](PROGRESS.md) is the count authority. A `functions.csv` status,
local PDB similarity or candidate-only relocation normalization is insufficient.
A new exact owner needs the complete target function extent, all original-bound
addresses/local destinations, correct compiler graph and reproducible selected
cold links. **Never run the historical full all-unit cold replay.**
Prefer `scripts/repo-python scripts/replay-exact-units.py --unit UNIT --json`
for affected source and its known exact consumers. Run lightweight CI only at
a source/metadata checkpoint. No compile/replay is necessary for docs-only edits.

## Canonical checkpoint

At `558e58a`, `matches.csv` and `PROGRESS.md` agree:

| Metric | Current |
| --- | ---: |
| Exact functions | **1,514** |
| Exact codegen bytes | **199,496** |
| Authored-origin exact functions | **601** |
| Authored-origin exact bytes | **129,209** |
| Authored source-present nonexact backlog | **149** |

Recent accepted owners include Ending LoadAnimation **126/126**
(`ending-text-load-animation`, seven target-bound fields), Sound LoadSoundSample
**684/684** (`sound-player-load-sound-sample`, 25 fields), and the two input
merge functions **37/37** and **43/43** (origin indeterminate).

Ending needed two natural C++ fixes: obtain the auxiliary VM ID **after**
resource publication to emit the in-place flags AND, then directly read the
serialized script DWORD so VC7.1 selects target ECX/ADD. Two independent selected
source cold replays passed all seven address fields.
EndingTextVmView::Run remains **1568/1568** and its 18-byte callback exact.

Sound's 684-byte match required the natural FindWaveChunk argument order
`(data, chunkSize, remaining, tag)` in the WinMain /GL graph. Its 71-byte
scanner and 57-byte setup thread also remain exact.

Earlier accepted representatives: ANM LoadTexture **243/243**, LoadTextureRegion
**320/320**, loaded-script start **203/203 at 0x43E710** (selected two-TU
context only), loaded VM InitializeVm **217/217**, enemy laser update callback
**91/91**, ReplayManager::SaveReplay **1681/1681**. Do not double-count these.

## High-value nonexact owners

These are *selected current-source comparisons*, not proof of a runnable game.
Changing a LTCG graph can change both registers and function extent.

| Owner / maintained source | Candidate | Remaining issue |
| --- | --- | --- |
| ANM ExecuteScript `0x43EE30`, `src/AnmManager.cpp` | **9962/9964**, 350 original fields | Opcode91 renderLayer target EDX vs candidate ECX at +0x113/+0x115. Aggregate, volatile, expression and parameter-order controls have repeatedly been neutral or regressed. |
| ECL Run `0x44E1A0`, `src/EclVm.cpp` | **6997/7020**, 189 fields | 23 byte differences: early stack addressing, x87 length/point-angle homes, FORMAT cursor/scratch/argument slots. Use full Run diagnostic, not 146-byte host entry as a substitute. |
| Bullet UpdateAbsoluteDirectionChange `0x4078E0` | **333/335**, 8 fields | EDX target vs EAX candidate at +0xD7/+0xDD. Speed/timer/alias/union/angle-lifetime trials failed. |
| Sound PreloadBgm `0x43D7D0` | **365/369**, 10 fields | Four local register bytes around ReadFile; local/output/reordered wrapper variants failed. |
| Memory-file Seek `0x4364F0` | **100/107**, zero fields | Register selection and FILE_END/FILE_CURRENT success-tail sharing. 14 early-return and other operand-order trials failed. |
| ECL AddScriptData `0x450220` | **510/589**, 2 CRT calls | Old table-count lifetime and register schedule; typed DWORD index fixes are not new byte credit. |
| Statistics factory `0x42AF20` | 98-byte candidate / 97 target | Extra push into private constructor; void/placement-new variants failed; constructor also 137/132. |
| Bullet SpawnSingleBullet `0x4067D0` | 1504-byte candidate / 1413 target | Incomplete private ABI/large source graph; 194-byte SpawnBulletPattern parent exact. |
| Spell HUD StartSpell `0x409280` | 2252 / 2432 target | Seven-stage state machine; selected two-TU leaf exact does not mean larger graph exact. |
| Enemy dispatcher / Player update / GameManager update | historical 14352/14416, 3108/3112, 1538/1544 | Rebuild the current graph before trusting old scores. |

Other current near-matches: Text InvertAlpha **112/157** zero fields; ANM
SetAndExecuteScriptIdx **249/271 ordinary** in a 287-byte candidate.
Recent source changes in Bullet/ECL/FileSystem/Statistics/Text/Transition
principally repaired i386 integer semantics while preserving exact guards;
they did **not** increase canonical counts.

## 2026-11-10 native VM slot words and ledger repair

Commit `d9769d08` reconciles four stale source name mappings at
`0x00401DE0`, `0x0043E710`, `0x00404F30`, and `0x00409280` with the
maintained source declarations, and restores the required address ordering of
one misplaced `matches.csv` row. Strict target/tracking validation now passes:
**1,829** boundaries, **1,309** mappings and **1,514** exact owners; no
exact ledger credit was added.

The unchecked `managedVmIds` accesses in `EnemyRuntimeView::DispatchEclInstruction`
for `SET_MAIN_ANM_SCRIPT` and `PLAY_SELECTED_ANM` now use `EnemyNativeWordIndex`
for nine reads/writes/address uses. This preserves the retail 32-bit scaled
address word even when the script index exceeds the declared C++ array.
An unchanged baseline and the revised TU were separately compiled into the
same pinned 11-support /GL graph. Their **entire 14,416-byte dispatcher** and
**all 22 source-owned functions** are raw-identical; another actual-source
recompile confirmed the installed comment-bearing source still has that exact
candidate body hash. Five selected original-bound exact leaves pass a fresh
cold replay: `GetVm` 21, `ClearFlag2` 67, `Release` 21, queue sample 123 and
queue cue 149 (**381/381** across three selected artifacts). This is source
semantics/codegen preservation, **not** a newly exact boss or game completion.

A follow-on resource-table fix changes all seven dynamic `loadedAnmResources`
reads in the same boss to `EnemyNativeWordIndex`: the four-slot typed array
is accessed with script-provided resource selectors, which the native caller
uses as flat 32-bit addresses. Independent current- and selected-resource
source variants separately preserve the entire **14,416-byte** candidate
and all **22/22** source-owned functions. The installed source was recompiled
once more to confirm identical complete candidate hashes. Four additional
selected exact source/callee replays pass `CreateVmVariant0` 73,
`CreateVmVariant2` 73, queue sample 123, queue cue 149 (**418/418** in two
artifacts). The dispatcher remains NONEXACT; no complete-product claims.

New ANM child-91 and ECL Run register/lifetime/source-order controls were also
compiled against their complete 9,964- and 7,020-byte owners. No new exact
match was found; rejected source variants were not installed. Their active
frontiers remain those in the table above. No full cold replay was run.

## 2026-11-11 current-source VM flag loop checkpoint

The two bounded ten-element `SET_FLAGS` / `CLEAR_FLAGS` loops in
`src/EnemyEclDispatcher.cpp` now use the same native 32-bit VM word index
helper as the script-driven VM operations. Independent pre/post compiles of
the complete dispatcher produce **14,416/14,416 identical bytes** and all
**22/22** source-owned functions unchanged in the pinned 11-support /GL
context. Focused selected cold replays pass `anm-vm-id-set-flag2`,
`anm-vm-id-clear-flag2`, and `enemy-apply-bullet-clear` (**178/178 bytes**).
No new exact owner is claimed; the dispatcher remains NONEXACT.

A separate current-source investigation of the 107-byte
`CMemoryPbgFile::Seek @0x004364F0` confirms **100/107** ordinary bytes,
zero relocation fields, and seven register/branch encoding differences at
`+0x49,+0x51,+0x55,+0x59,+0x5E,+0x61,+0x63`. Four valid new source variants
were no better; changing FILE_BEGIN to an early return worsened the whole
control-flow layout. The original branch from FILE_CURRENT goes to FILE_END's
EAX/EDX addition at `+0x2A`; the candidate jumps into FILE_BEGIN's equivalent
tail at `+0x60`. Further work needs a specific compiler register-lifetime
hypothesis, not the already rejected direct-return/operand-order controls.

## 2026-11-11 Player history loop threshold checkpoint

Both Player large update loops now compute the script-facing history
threshold with `static_cast<int>(static_cast<unsigned int>(optionCount) << 3)`.
This restores native 32-bit SHL semantics while retaining the signed loop
comparison. Selected 3-TU VC7.1 comparison keeps all 67 Player-owned bodies
and 556 field layouts/non-field bytes unchanged; six selected exact native
cold units pass **807/807** (one transient Wine link retry). Neither 1,656-
nor 3,112-byte original Player boss receives exact credit. See the knowledge
note for rejected unsigned-compare and helper variants.

## 2026-11-11 Player large-owner history indexing

`src/Player.cpp` now uses `PlayerNativeHistoryWord` for the two unchecked
`optionCount * 8` history reads in PlayerUpdateMovementAndOptions and
PlayerUpdateCallbackBody. Both express the retail low-32-bit base+stride
without C++ out-of-array pointer arithmetic. In the selected PlayerDraw
three-TU `/GL` graph, two independent source compiles preserve all **67**
complete Player-owned function bodies and **556** original candidate symbol
bindings/field positions; no non-field byte differs. Full candidate sizes
remain 1,616 and 3,100 respectively, so neither large owner becomes exact.
Six directly relevant, target-bound canonical Player cold replays pass
**807/807 bytes** across four artifacts, without a full-cohort replay.
See the top of `docs/KNOWLEDGE_BASE.md` for target applicability and scope.

## 2026-11-11 additional natural-source probes

No new exact match was found after the Enemy flag-loop checkpoint. Complete
selected ANM ExecuteScript remains **9,962/9,964** (350 original fields), ECL
Run **6,997/7,020** (189 fields), Sound PreloadBgm **365/369** (ten fields),
Memory Seek **100/107**, statistics record defaults **199/242** comparable
ordinary bytes in its 274-byte function, and Chain AllocElem **48/70** in its
74-byte function. Additional declaration, copy, volatile, barrier, constructor
and argument-order controls were compiled; none improved those whole-owner
frontiers. See the 2026-11-11 register/ABI section at the top of
`docs/KNOWLEDGE_BASE.md` for specific failed shapes and target instructions.

**Important false exact lead:** the 202-byte `EntityStartScript_00404f30`
normal-COFF probe has 194/194 ordinary bytes and both genuine callee fields,
but its maintained body is naked handwritten assembly. Do not admit it as a
natural exact owner or add a `matches.csv` row. The useful open task is to
recover the ESI-VM/EAX-script/stack-resource private ABI naturally in the
real multi-TU caller graph; the earlier natural five-TU body was 200/202.

## Next useful experiments and checks

1. Choose a complete **boss and/or leaf** with a specific offset-level
   prediction; inspect current source, `config/functions.csv`, original
   disassembly, match unit and relevant `_reference/th10-decomp` hypothesis.
   Never treat the reference project or a previous assumption as target truth.
2. Try a *few* natural C/C++ expressions, keeping source variants, scripts,
   temporary diagnostics under ignored `.analysis/gpt-web/` and scratch
   compiler outputs under `build/gpt-web/`. Avoid redoing known neutral sweeps.
3. Install a promising variant, replay **only affected exact units** and
   original-bound complete nonexact parents. Record useful findings and remove
   superseded trial files. New acceptance needs independently
   confirmed exact source-linked builds. Commit real progress as
   `gpt-web: ...` with AGENTS.md's author/trailer. Never push unrequested.
4. Wine/link.exe occasionally emits LNK1104 or a page-fault message against
   different support objects. A failed link is **not** a byte mismatch and
   supplies **no** exactness receipt. Retry an isolated selected unit serially.

Useful focused private drivers (not Git-tracked):
`.analysis/gpt-web/20261110-anm-child91-aggregate-initializers.py`;
`.analysis/gpt-web/20261110-ecl-format-cursor-allocation-schedule.py`;
`.analysis/gpt-web/20261110-bullet-absolute-speed-liveness.py`;
`.analysis/gpt-web/20261110-seek-tail-control-flow-trials.py`.

Historical archives, old reports and duplicate handoffs have been removed.
Use current unit recipes to reproduce accepted comparisons; retain only the
specific unresolved contexts listed in [RE_ARTIFACTS.md](RE_ARTIFACTS.md).
