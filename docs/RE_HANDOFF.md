# TH10 reconstruction — next agent handoff

**Updated 2026-11-11.** This is the short continuation brief, not a chronological
experiment log. Read [AGENTS.md](../AGENTS.md) for the exactness, source and
storage rules. Consult [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) by function/address
for detailed observations and rejected approaches; its older dated paragraphs
describe their own source epochs, not the current score.

## First actions and authorities

1. Run `git status --short --branch` and inspect **all** dirty/untracked work;
   preserve and take responsibility for it. Inspect `git log -8 --oneline`.
2. Verify only the original Japanese TH10 v1.00a target:
   `scripts/repo-python scripts/verify-target.py`. The ignored, uncommitted
   `resources/th10.exe` must have SHA-256
   `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
3. Use the pinned VC7.1 SP1 toolchain, source-specific `/GL`/`/GS` profiles
   and real source-link dependencies. Inspect
   `scripts/repo-python scripts/verify-toolchain.py --check` if setup changes.
4. The authority for accepted exact units is `config/matches.csv` and
   `config/match-units.toml`; [PROGRESS.md](PROGRESS.md) is **generated** from
   the canonical ledgers. `config/functions.csv` and matching candidate sizes
   alone never establish exactness.

**Snapshot of generated progress:** 1,520 exact functions / 200,064 codegen
bytes; 607 authored-origin exact functions / 129,777 bytes; 143 authored
source-present functions remain nonexact. Regenerate/check the numbers from
`docs/PROGRESS.md`, not this prose. All commits use `gpt-web: ...` with the
name/email/trailer specified in `AGENTS.md`; **do not push**.

Never run the full historical/all-unit cold replay. Build and compare one
complete affected owner, all original-bound relocations and its known exact
consumers. Use explicit `--unit` selectors only:

```bash
scripts/repo-python scripts/replay-exact-units.py --unit bullet-manager-draw-bucket --json
scripts/repo-python scripts/replay-exact-units.py --unit enemy-read-dialog --json
scripts/repo-python scripts/replay-exact-units.py --unit gui-update-stage-elements --json
scripts/repo-python scripts/progress.py --check
scripts/repo-python scripts/build-match-unit.py --check
git diff --check
```

The three replays above are **examples**, not a mandatory session-entry suite.
For a prose-only cleanup no compiler, CI or replay is needed. Wine/link.exe
occasionally fails with `LNK1104`/page faults; a failed link is neither an
exact result nor evidence of changed target bytes. Retry only the affected
selected unit after inspecting the error.

## Recently accepted exact functions — do not reopen as nonexact

| Owner | Canonical unit | Proven scope |
| --- | --- | --- |
| `ResultMenuSynchronizeScore @0x00421F60` | `result-menu-score-synchronize` | **28/28**, three original-bound score-state DIR32s (+4, +0, +0). Native GUI stored-score reread restores EAX receiver/ECX score and MOV EAX,ECX. Two independent cold builds pass; related result-menu guards total **354/354**. The large 3,227-byte parent remains nonexact. |
| `ReleaseModeOwners @0x004203F0` | `main-release-mode-owners` | **128/128**, all 15 original-bound globals/cleanup/destructor/delete fields in current 30-input WinMain lifecycle graph. Five explicit natural nullable owner checks retain ESI and exact teardown order; existing real `MainSupervisorView::ReleaseGame` caller also passes **293/293** and 25 fields, **421/421** scoped. |
| `EnemyEclHostBaseView::SetSubroutine @0x00450700` | `enemy-ecl-host-set-subroutine` | **30/30**, one original-bound REL32 to the genuine exact 136-byte `EclVmScriptDatabase::FindSubroutine` in the two-TU Enemy/EclVm graph. Scoped volatile database-field read restores native EAX database / ECX name and two independent context reloads; two independent cold builds pass. Target 2,412-byte EnemyRuntimeUpdate remains nonexact. |
| `ScriptViewerBombOwnerView::EmitAuxiliary @0x00405AC0` | `script-viewer-bomb-emit-auxiliary` | **124/124**, six target-bound fields in the real six-source Bomb-update callback /GL /GS /EHs graph; target ESI receiver, separate bullet-cancel branches and volatile reread of spell 109. Existing laser cancel guard **76/76** also cold-passes. Two independent colds pass; the 223-byte Bomb Update parent remains nonexact. |
| `BulletManagerView::DrawBucket @0x004066E0` | `bullet-manager-draw-bucket` | **134/134**, five target-bound fields; two independent VC7.1 `/GL` cold links of real `BulletDraw.cpp` + `AnmManager.cpp`; true EAX/ECX Draw ABI. Source uses two `_ReadWriteBarrier`s and a volatile flags DWORD read, with no ASM/padding. |
| `EnemyReadDialog @0x00415DB0` | `enemy-read-dialog` | **124/124**, nine ctor/global/EH fields; two independent cold links of the real six-TU `/GL /GS /EHsc` graph and complete static PDB ownership. A scoped volatile read restores the global index publication order. |

These are **six accepted authored owners** (not just positional candidate
matches). Their accepted units and all ledger rows are already committed.
Useful existing exact guards: `anm-render-draw` **216/216**, `bullet-manager-draw-callback`
**44/44**, `gui-update-stage-elements` **3836/3836** (72 fields),
`script-viewer-popup-constructor` **134/134**, and `result-menu-close`
**89/89**. Other accepted large representatives include
`ReplayManager::SaveReplay` **1681/1681** and EndingTextVmView::Run **1568/1568**.
Do not infer a nonexact parent is solved from an exact callback/leaf.

## High-value nonexact work (source/context-sensitive)

The figures below describe **specific selected compiler graphs**, not a
universal size for every LTCG graph. Rebuild the current source before using
a historical numerical frontier as an acceptance decision.

| Original function / source | Target and selected candidate | Specific next hypothesis or blocker |
| --- | --- | --- |
| ANM `ExecuteScript @0x43EE30`, `src/AnmManager.cpp` | Target **9,964**, 9,962 matching bytes, **350** fields | Opcode 91 child VM layer: target EDX vs candidate ECX at +0x113/+0x115. Aggregate, source-order, reference, union, volatile, explicit script resolution and signed/field-order variants do not close the gap. All 350 original-bound fields remain proved; see pinned ANM context. |
| ECL `EclVmContext::Run @0x44E1A0`, `src/EclVm.cpp` | Target **7,020**, 6,997 matched, **189** fields | 23 bytes: early stack/ESI register homes, x87 LENGTH_SQUARED/POINT_ANGLE and FORMAT local slots. Run the *complete* owner diagnostic; a host adapter is not a substitute. |
| GUI `GuiMessageVmView::Run @0x415E90`, `src/Gui.cpp` / `Gui.hpp` | Target **4,480**, selected candidate **4,492** | Script integer offsets, activeSide addresses, profile counters and instruction cursor now use native DWORD semantics; 22 GUI-owned functions/566 fields were preserved. The 9-byte `GuiMessageInstructionNext` owner must remain separately emitted. |
| Enemy ECL dispatcher, `src/EnemyEclDispatcher.cpp` | Target/candidate extent **14,416** in selected 11-support graph, **800** source field identities | Still nonexact vs original. Native index, resource, callback, dialog and 8-bit spell-key fixes are already installed and byte-neutral; do not misread unchanged candidate code as target exactness. |
| Player update, `src/Player.cpp` | `PlayerUpdateCallbackBody`: target **3,112** / candidate **3,100**; movement target **1,656** / candidate **1,616** | Natural 32-bit history, movement, transition and damage operations are installed; further work needs concrete ABI/stack/codegen differences, not more unsigned spelling sweeps. |
| GameManager update, `src/GameManager.cpp` | Target/candidate extent **1,544** in selected GameStageSlots graph | Stats index and frame DWORD arithmetic now correct; remaining register/layout frontier. |
| `ResultMenuPrepareScore @0x00423570` (`src/FrontEnd.cpp`) | Target **377** / candidate **377** bytes; **99/106** normalized original instructions aligned (previous 372 bytes / 87/106). Native success-first/failure-tail and eight-character do/while trim installed; all 18 candidate fields present. **NONEXACT**: remaining 31 ordinary bytes differ only at `+0xA4..+0xC5` in cursor/keyboard high-bound register/CFG. Eight scoped original-bound exact guards pass **690/690**, without full cold replay. |
| FrontEnd result-menu and music-room update, `src/FrontEnd.cpp` | `ResultMenuUpdateResult`: target **3,227**; **3,036** in `/GL` FrontEnd + ResultMenuLifecycle, **3,196** in `/GL /GS /EHs` FrontEnd + VmIdInterrupt, **3,208** in the native VM-ID + Enemy sound-callee graph after recovering GUI score leaf (target 3,227, 763/907 aligned; previous same graph 3,216/762); prior three-source graph 3,216/695; `FrontEndUpdateMusicRoom` **2,596** in a prior graph | Split native state-11 replay naming and state-12 score entry, plus alphabet/space, horizontal-key branches, pointer/countdown replay-file release, native choice-one/two switch, and state-10 trailing-space do/while, branch-local replay-data timestamp pointers state-13 three-option switch and state-7 result-path join, state-12 nonnegative-first backspace/negative exit, and state-11 zero-name early return (installed; 763 normalized ordered matches in the real AnmVmId + Enemy sound-callee graph against 907 target instructions after exact score-leaf recovery). `/GS` restores the target cookie prologue; genuine `QueueSoundSample` WPO support restores EDI sound-ID calls, including state 6. All candidate contexts remain **nonexact**; avoid comparing size across different graphs as an exactness metric. The genuine ReplayManager callee recovers EDX replay-path forwarding but makes the parent 3,264/3,227 (748/907), so it is diagnostic only. Target decompile independently confirms **64-byte** replayPath and separate DWORD stack slots; 65/68-byte fake buffer-size controls improve CFG alignment but are explicitly rejected. Broadening the real selected-VM-ID lifetime restores 0x4C frame but introduces a non-target copy; use this as a frame diagnostic, not a source patch. Selected Score28, Close89, ResultMenuUpdate206, PendingThree65, PendingSix65, Callback10, Capture162 and true AnmVmId SetInterrupt65 exact cold replays pass (690/690). |
| Bullet absolute direction, `src/BulletManager.cpp` | **333/335**, eight fields | Target EDX vs candidate EAX at +0xD7/+0xDD; speed/angle lifetime, volatile, union and alias trials neutral/regressed. |
| Sound `PreloadBgm`, `src/Sound.cpp` | **365/369**, ten fields | Four register bytes near `ReadFile`; extra formatting pointers/reg-lifetime trials failed. |
| `CMemoryPbgFile::Seek @0x4364F0`, `src/ResFile.cpp` | **100/107**, zero fields | FILE_CURRENT should share the FILE_END EAX tail; signed/unsigned operands and direct-return forms did not fix register allocation. |
| Transition shake `@0x43C710`, `src/TransitionEffect.cpp` | Target **349** / candidate **350** | Separating the null/flags checks improved positional ordinary bytes **181 → 207/285**, but remaining phase ordering is nonexact; five neighboring transition units replayed exact. |
| Popup factory `GameCreateOwner42B660`, `src/ScriptViewerPopupCreate.cpp` | Target/candidate extent **71**, still nonexact | Genuine ctor/init/dtor and five fields present; ESI-vs-EBX retention plus allocation/cleanup path. New/placement/volatile variants regressed. Its 134-byte ctor is separately exact. |
| Bomb `ScriptViewerBombOwnerView::Update @0x405750`, `src/ScriptViewerBombCreate.cpp` | Target **223** / current six-source native candidate **223**; **63/72** target normalized instructions aligned, only **nine non-field byte differences** | Installed native `vmId2C` reads and volatile NULL zero-publication restore ESI-zero entry, target stack scheduling and null-path stores (prior 220 bytes, 51/72). Remaining differences are EBP manager/EBX VM register coloring versus EBX/EBP in the candidate. Five affected exact original-bound guards cold-pass **379/379**. |
| Popup Draw and callback, `src/ScriptViewerPopupCreate.cpp` | Draw target **540** / selected candidate **537**; callback target/candidate **10** | The target callback saves EDI and calls an EDI-receiver Draw, while the four-TU candidate saves EBX and uses an EBX-receiver Draw. Changing record/player declaration order or index signedness is codegen-neutral; hoisting `drawVm` regresses to 526/7. Keep real caller/receiver work in scope alongside leaves. |

**False positive to avoid:** `EntityStartScript_00404F30` showed 194/194
comparable bytes in normal COFF, but its maintained function uses
`__declspec(naked)`/hand-written assembly. **Do not add exact credit** on
that evidence. Recover the ESI VM / EAX script / resource-stack private ABI
naturally in its full source graph.

**Known comparison caveats:** The legacy `result-menu-update` unit's declared
one-byte `data_spans` conflicts with the current Capstone normalization; do
not silently delete/relax its physical-owner contract. In one 15-TU graph,
`result-menu-open-game-over` has shifted fields after +0xD2; a controlled
same-support-objects FrontEnd **before/after** relink proved identical 411-byte
candidate code and all 28 candidate field identities, so that mismatch
predates the later FrontEnd DWORD fixes. It is not an accepted exact replay
in that graph.

## Efficient continuation and retained private inputs

Select one large and one small owner with an offset-level hypothesis. Read
its source, target bytes, unit recipe, and the relevant knowledge section;
compile a *small* number of natural C/C++ forms, compare the **complete**
PDB-owned byte extent and **all** original-bound fields, and commit a useful
source checkpoint. Avoid repeating source-form grids already rejected.
Portable source edits and canonical contracts are tracked; compiler fixtures,
original binary and private diagnostics are not.

Retain `build/match-linked/`, the Ghidra project, target, `_reference/`,
`.tools/` including this game's Wine prefix, and the **five narrowly pinned
unresolved contexts** named in [RE_ARTIFACTS.md](RE_ARTIFACTS.md). Historical
`build/gpt-web` variants and duplicate scratch C++ outputs were intentionally
pruned for storage; do not assume a date-stamped path is still on disk.
Four focused private trial drivers remain in `.analysis/gpt-web/`:

- `20261110-anm-child91-aggregate-initializers.py`
- `20261110-ecl-format-cursor-allocation-schedule.py`
- `20261110-bullet-absolute-speed-liveness.py`
- `20261110-seek-tail-control-flow-trials.py`

See the unit manifest for source/caller graphs and reproduce any missing
historical probe from current source. Do not delete or alter the immutable
target, canonical ledgers, analyzer project, Wine prefix or unrelated D3D work.
