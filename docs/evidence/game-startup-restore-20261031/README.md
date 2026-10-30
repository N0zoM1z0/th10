# Game startup dependencies and complete initialization — 2026-10-31

Three complete authored owners add **582 exact bytes**: Replay stage restoration
356/all27, ReplayManager::Create185/all12 and dummy command41/all5. Existing
Enemy timer/list reset58/all1 retains its credit after source consolidation.
The complete1009-byte GameManager initialization remains **1023/1009, NONEXACT**.
No boundary, origin or authored-denominator change is made.

## Complete restoration and factory

Target42A6A0..42A803 constructs a zeroed1C4-byte stage header in record mode,
publishes it, reads the stage index again, snapshots the actual RNG seed, resets
the generation count and copies the Main new-session bit. Playback resets the
three stage cursors/count, restores seed/count, score/power/faith/timer/lives/
rank values and reads the final stage header again. Other modes do nothing.

Natural source uses the existing RngView4918B0, GameScoreState474C40 and Main
491C28 views. Independent scalar seed/count/session aliases hoist or sink
operations incorrectly. Actual aggregate storage plus the observed count-reset
order recover every byte. The real stage-header constructor retains nullable
construction before the following unchecked use. The complete356-byte owner
has27 fields/four fixed local branches and the private EBX manager/plain RET.
No separate constructor credit is claimed.

Target429610..4296C8 has two RET8 exits, unlike the former cdecl declaration.
Correct ordinary two-parameter stdcall source recovers the complete185-byte
factory and its caller's stack cleanup. It retains null construction, the
unconditional Initialize call, real destructor/delete failure cleanup and both
returns. All12 fields and four locals pass. The existing allocation-EH binding
also proves the complete21-byte generated region, one-state FuncInfo, saved
EBP-10 allocation and actual scalar-delete/frame-handler identities. These are
metadata/address checks; no injected allocation failure or extra EH credit.

The41-byte420C00..420C28 helper chooses actual SoundPlayer QueueCommand opcode4
or3 for "dummy", passes argument0 and returns the observed zero result. Its
five fields, one local branch and both returns match completely. A scoped real
call boundary preserves the target startup call, as does the existing pattern
used for the final28-byte MarkGameChains call.

## Real startup bindings and layout repair

The complete1009-byte startup is compared throughout. Its repaired source uses
actual Chain::AllocElem/AddToCalcChain/AddToDrawChain, ScriptViewerCreateGui,
GuiView::LoadData, FrontEndLoadMusic, the restored Replay helper and the existing
MarkGameChains definition. Actual EnemyLaserManager source joins the graph with
its pinned profile. The simple-class fastcall draw-member representation is
asserted at four bytes and transferred through a union to the existing generic
Chain callback interface; no synthetic callback shim or fake register ABI.

Known Main and score offsets are reused. A minimum startup state view covers
score state plus the observed+5C counter/+60 runtime flags. This is a shared
address view, not an original allocation/class ownership claim. Owner-relative
statistics reads preserve the existing record schema's effective+18/+1D values
and+4C8 session counter while removing biased base-address arithmetic.

The old unasserted FPS view placed its doubles at+28/+30 under default alignment;
target stores are+24/+2C. Reusing existing packed ReplayTimingSource fixes both
real destination offsets. Main worker writes now name stopRequested/running at
+638/+63C, and the two still-unreconstructed state helpers receive the actual
Main address. Their names/semantics remain provisional.

Adding the full legacy leaf TU fails with four duplicate Enemy accessor symbols;
no forced-multiple-definition link is used. Only the required58-byte417800 timer/
list reset moves from DecomphelpLeafAccessors.cpp to Enemy.cpp. Existing timer
and list views replace numeric pointer casts, and the real g_PlayerTimerScale
symbol replaces the fixed476F78 literal. Full58/all1 remains exact, with zero
new credit. Only its canonical recipe/source mapping migrates to this graph.
Other legacy leaves remain unchanged; no broad legacy cohort is replayed.

## Full parent and controlled comparisons

| Model | Complete startup /1009 | Restoration /356 |
| --- | ---: | ---: |
| Entry30-input context |1024, nonexact|source absent|
| Complete helper with genuine header construction |1039, nonexact|356, nonexact|
| Actual RNG/Main storage and seed-reset order |1039, nonexact|356, exact|
| Actual Chain registration |1038, nonexact|356, exact|
| Actual Create RET8 |1029, nonexact|356, exact|
| Actual GUI/music/mark calls and dummy command |1024, nonexact|356, exact|
| Known startup state views |1034, nonexact|356, exact|
| Minimum shared state range |1029, nonexact|356, exact|
| Owner-relative statistics |1027, nonexact|356, exact|
| Final actual factories, timing/worker repair and typed timer |1023, nonexact|356, exact|

Independent complete inventories retain102 address fields in each parent,
30 direct calls with equal actual identity counts, and41 native versus40 target
local branches. Call/block order, scheduling and fixed destinations still differ;
no window, length-only or partial-parent credit. Gui loader289/289 remains
nonexact. BeginStage566/582, frame allocator139/139, Replay Initialize1316/1344,
GameManager update1538/1544 and cleanup1107/1105 remain unequal in this graph.
The timing fix is source/layout progress, not runtime validation.

## Scoped acceptance

Two independent zero-reuse32-input canonical colds pass all640 bytes/all45 fields
across the three new owners and migrated58-byte owner. Both frozen images pass
36 selected existing contracts4541/all159. Both report the complete rejections:
Chain insertion129/131 twice, GUI factory68/136, laser factory field positions
and Stage destructor460/499. These are separate declared contexts; none is waived.
The first scoped run additionally cold-passes existing GameManager update7,
OnDraw31 and MarkGameChains28 using their unchanged recipes. Static ownership
is accepted at its original producer path; the frozen-image guard selection
excludes Mark because moving the image intentionally fails its strict producer
path check. No static owner selection is weakened. No full cohort runs.

[results.json](results.json) retains79 active source/header pins, complete target/native
inventories, actual call bindings, EH evidence, controls, failed duplicate-link
receipt and independent PE/PDB identities. Jobs are serial CPU0/nice10, bounded
and logged below the private replay-restore-20261031 analysis/build campaign.
Producer: `build/match-linked/GameStartupRestore`. The first second-cold attempt
fails before comparison when winepath crashes and pollutes one linker filename.
Its terminal report is retained with zero credit. Required VC7.1 normal/LTCG/
resource/PE32 execution smoke passes, then only the four selected new/migrated
units cold-rebuild again. Source, profiles and contracts stay unchanged. Existing profiles, the real
InitializeVm entry and the comparator remain unchanged.

```sh
scripts/repo-python scripts/replay-exact-units.py --unit replay-restore-stage-state --unit replay-manager-create --unit game-prepare-dummy-music --unit game-decomphelp-fulltu-00417800 --unit game-manager-update-callback --unit game-manager-mark-chains --unit decomphelp-game-manager-on-draw --json
scripts/repo-python scripts/ci.py
git diff --check
```

Authored exact becomes106091/275863 across516 owners; canonical174205/1394.
Mappings1220/authored702 and backlog186 keep source presence separate. Original
names/class/TU/data ownership, product closure, runtime and portability stay open.
