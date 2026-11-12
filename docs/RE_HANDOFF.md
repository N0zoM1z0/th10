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
The existing canonical `Player::CalculateDamageToTarget` **1,170-byte**
owner remains exact after replacing two type-punned unsigned-pointer sums
(hit accumulator and global score) with native unsigned-DWORD calculations.
Its 28 fields and all bytes cold-pass, alongside the **91-byte** shot-bounds
helper: selected **1,261/1,261** bytes, **32** fields and two artifacts.
This preserves source fidelity without increasing the exact numerator.

Do not infer a nonexact parent is solved from an exact callback/leaf.

The FrontEnd cursor member API now returns the published index from the
already exact 33-byte `FrontEndCursorSetCurrent` physical helper. After that
header change, 10 selected exact cursor/result-menu owners cold-pass
**737/737** bytes and 28 fields (seven artifacts); full 377-byte score
preparation and 3,228-byte result-menu parent remain nonexact.

The ECL `AddScriptData` insertion-shift decrement now uses modulo-DWORD
arithmetic and `memcpy` publication instead of an incompatible unsigned-int
pointer. Its two-TU 589-byte candidate and the 7,020-byte Run parent are
**ordinary-byte unchanged**; both remain NONEXACT. Four selected ECL exact
cold units pass **410/410** bytes and three target-bound fields.

## High-value nonexact work (source/context-sensitive)

The figures below describe **specific selected compiler graphs**, not a
universal size for every LTCG graph. Rebuild the current source before using
a historical numerical frontier as an acceptance decision.

| Original function / source | Target and selected candidate | Specific next hypothesis or blocker |
| --- | --- | --- |
| ANM `ExecuteScript @0x43EE30`, `src/AnmManager.cpp` | Target **9,964**, 9,962 matching bytes, **350** fields | Count-minus-one helper now uses target-compiler-defined signed 32-bit conversion after unsigned DWORD decrement, avoiding the unrelated-type pointer lvalue; parent unchanged and four selected exact leafs cold-pass **372/372** bytes/31 fields. Opcode 91 child VM layer: target EDX vs candidate ECX at +0x113/+0x115. Aggregate, source-order, reference, union, volatile, explicit script resolution and signed/field-order variants do not close the gap. Native unsigned JUMP_DEC publication now uses the actual `int *` target and preserves the same complete 9,962/9,964, 350-field comparison. Four selected affected exact leaves cold-pass 372/372 bytes (31 fields); see pinned ANM context. |
| ECL `EclVmContext::Run @0x44E1A0`, `src/EclVm.cpp` | Target **7,020**, 6,997 matched, **189** fields | 23 bytes: early stack/ESI register homes, x87 LENGTH_SQUARED/POINT_ANGLE and FORMAT local slots. Run the *complete* owner diagnostic; a host adapter is not a substitute. |
| GUI `GuiMessageVmView::Run @0x415E90`, `src/Gui.cpp` / `Gui.hpp` | Target **4,480**, selected candidate **4,492** | Script integer offsets, activeSide addresses, profile counters and instruction cursor now use native DWORD semantics; 22 GUI-owned functions/566 fields were preserved. The 9-byte `GuiMessageInstructionNext` owner must remain separately emitted. |
| Enemy ECL dispatcher, `src/EnemyEclDispatcher.cpp` | Target/candidate extent **14,416** in selected 11-support graph, **800** source field identities | Still nonexact vs original. Native index, resource, callback, dialog and 8-bit spell-key fixes are already installed and byte-neutral; do not misread unchanged candidate code as target exactness. |
| Player update, `src/Player.cpp` | `PlayerUpdateCallbackBody`: target **3,112** / candidate **3,100**; movement target **1,656** / candidate **1,616** | Natural 32-bit history, movement, transition and damage operations are installed; further work needs concrete ABI/stack/codegen differences, not more unsigned spelling sweeps. |
| Player damage `CalculateDamageToTarget @0x00428630`, `src/Player.cpp` | **Already exact**, complete **1,170 bytes** and all **28** fields in its 17-input `/GL` graph | Replaced two incompatible `unsigned int*` accumulator aliases with natural native unsigned 32-bit additions and signed-field stores. Focused original-bound cold replay preserves existing 1,170-byte damage owner and 67-byte angle leaf, **1,237/1,237** and all **31** fields, two artifacts. No new exact credit. |
| Player shot loop `PlayerUpdateShots @0x00428280`, `src/Player.cpp` | Target **876** / complete twelve-source candidate **876** bytes, **22** candidate fields; 40 ordinary differences, beginning at a moved LEA/jump near `+0x111`; **NONEXACT**. | The target `INC EDX` at `+0x324` requires a native low-32-bit timer wrap: source now uses unsigned arithmetic before signed-field publication, with no change to candidate bytes or fields. Two related selected original-bound exact units remain **158/158**, seven fields; avoid full replay. |
| `InitializeGameStatisticsRecord @0x0042ACB0` (`src/GameStatisticsRecordDefaults.cpp`) | Target/candidate full **274-byte** six-TU score graph with **25 ordinary differences**, eight candidate fields; remains **NONEXACT** | Default name uses two separate four-byte `memcpy`s and a late ninth byte. Native score and extra DWORD stores now also use byte-safe four-byte `memcpy`s (compiler emits identical full owner; no pointer alias assumption). Related exact Load423, Finalize564 and InitializeTail142 cold replay **1,129/1,129**, all **36** fields, one artifact. No full cold replay. |
| GameManager update, `src/GameManager.cpp` | Target/candidate extent **1,544** in selected 23-input GameStageSlots graph; **114** candidate fields, still **NONEXACT** | Restored both native low-DWORD frame-counter increments via unsigned addition and `memcpy` bit-pattern publication, avoiding type aliasing; complete candidate has **0 ordinary-byte changes** relative to previous same-image graph. Exact callback **7/7** and faith decay **65/65** selected cold replay **72/72**, two target-bound fields; remaining register/layout frontier. |
| `ResultMenuPrepareScore @0x00423570` (`src/FrontEnd.cpp`) | Target **377** / candidate **377** bytes; **99/106** normalized original instructions aligned (previous 372 bytes / 87/106). Native success-first/failure-tail and eight-character do/while trim installed; all 18 candidate fields present. **NONEXACT**: remaining 31 ordinary bytes differ only at `+0xA4..+0xC5` in cursor/keyboard high-bound register/CFG. Eight scoped original-bound exact guards pass **690/690**, without full cold replay. |
| Result-menu state machine `ResultMenuUpdateResult @0x4236F0`, `src/FrontEnd.cpp` | Original **3,227** bytes. Current **3,228**-byte four-TU `/GL /GS /EHs` candidate with genuine VM-ID/sound callees: **784/907** normalized ordered instructions, **173** detected candidate fields. Previous checkpoints: 3,208/763 → 3,216/775 → 3,224/781. | Native **0x4C** frame restored with function-scope selected VM-ID; states 10/11 use native 16-bit `AX` input-result tests, and state-12 up/down/left likewise. Widening the state-12 edit to all four inputs shifts CFG/registers and regresses. Remaining state-7 preparation placement, state-8 VM-ID copy, state-10/11 naming and replay loop code, and state-12/13 control-flow differences are still **NONEXACT**. Score sync **28/28** exact, PrepareScore full **377**-byte candidate but **31** nonfield differences. Three successive checkpoints each passed eight selected original-bound cold exact units **529/529** with **28** fields; no full cold replay. |
| Bullet absolute direction, `src/BulletManager.cpp` | **333/335** ordinary-plus-linkage bytes, eight candidate fields, **NONEXACT** | The `int2` cycle increment now uses native unsigned 32-bit wrap without aliasing, retaining the unchanged full 335-byte graph. Remaining +0xD7/+0xDD EDX-vs-EAX load/push difference; additional size/output lifetime and counter variants neutral or regressed. Selected relative/aimed direction cold exact **769/769** with 20 original-bound fields. |
| Text `InvertAlpha @0x00437160`, `src/TextRenderer.cpp` | Target/candidate full **157**, ordinary bytes **112/157**, **NONEXACT** | Replaced signed-index unsigned-pointer alias increments with unsigned DWORD addition plus exact signed-bit publication via a local helper. Fresh normal COFF is **byte-identical** to prior candidate; three related exact units pass selected cold **1,125/1,125**, three fields, two artifacts. No full replay. |
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
