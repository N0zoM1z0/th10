# TH10 handoff — 2026-10-31

## Entry and current status

Start with [AGENTS.md](../AGENTS.md), this brief and the relevant source/ledger
rows. [RE_WORKFLOW.md](RE_WORKFLOW.md) and [ORACLES.md](ORACLES.md) describe the
selected lane. This checkpoint adds176 exact replay-helper bytes and repairs
the complete582-byte BeginStage source to566 bytes. The parent and its139-byte
allocator remain nonexact. Existing canonical recipes are unchanged.
Compiler profiles and the comparator remain unchanged. No experiment is running.

[PROGRESS.md](PROGRESS.md) is the generated count authority. At this checkpoint:

| Measure | Count |
| --- | ---: |
| Reviewed authored owned bytes exact | 104,828 / 275,863 |
| Authored exact owners | 511 |
| Total canonical exact owners / codegen bytes | 1,388 / 172,932 |
| Source mappings / authored mappings / authored exact backlog | 1,217 / 699 / 188 |
| Reviewed function boundaries / authored functions | 1,819 / 747 |

The authored inventory remains provisional. Complete exact codegen does not
establish original names, class/TU/data ownership, product closure, runtime
semantics or portability. Those phases remain open.

## Latest source checkpoint: complete Replay stage helpers

[The complete proof](evidence/replay-stage-helpers-20261031/README.md) adds
ReplayClearStageFrameData42AB2093/all1 and ReplayRestoreStagePosition428E1083/all2:
176 new exact authored bytes. Both meaningful helper operands are reviewed;
Ghidra stack-only arity is provisional. Natural two-input source derives target
ECX/EAX and EAX/ECX private interfaces without register declarations. Existing
pair assignment preserves both coordinate copies before x87 conversion.

The582-byte BeginStage owner evolves1000 to568 with actual helpers/index rereads/
post-loop flags,565 with real frame construction/callee-pop,572 with shared
GameScoreState aliasing and566 with a stage-index snapshot. Both counted loops
and their base offsets, alternating recording loads/stores and selection reads
inside playback are recovered. The former reset facade binds actual424D90.
All five real callee identities remain in order. Current native566/582 has24
fields/nine locals versus25/ten: Player reload, loop registers/alignment and
final index scheduling remain unlike. No fragment credit.

Frame allocation42AA50 has two meaningful inputs: ESI stage and stack manager,
RET4. The genuine6284-byte constructor preserves the null branch, full reset,
cursors and intrusive-link initialization. Complete139/139 remains NONEXACT:
early fps-pointer LEA precedes the recordEnd store. Store-order, memcpy-pair and
Player-member controls remain unequal/neutral, are restored and parked. Do not
repeat them without distinguishing evidence or infer arity from stack words.

Two independent29-input canonical colds reproduce full176/all3. Both frozen
images pass24 existing contracts3120/all113; stage destructor460/499 explicitly
rejects. The first scoped run cold-passes existing cleanup66 and callback7 in
their unchanged28-input recipes. No recipe migration or full-cohort replay.
Actual29-input GameManager1538/1544 and cleanup1108/1105 remain nonexact;
BeginSpell2424/2432, stage reset184/214, embedded200/202 and binder197/203 remain
unlike. No shared Player header, comparator or profile change is retained.

Producer: build/match-linked/ReplayStageHelpers. Current76 transitive pins and
independent target inventories are in the proof; frozen colds/final-source
context are in the private replay-stage-20261031 campaign. Older28-input source
freezes now predate ReplayManager changes; check pins before reuse. Three new
source mappings retain existing authored origins and reviewed extents, with no
denominator change. Original class/TU/data ownership and later phases stay open.
Concrete next question: target Player reload and private register propagation
require genuine alias/visibility evidence; fake clobbers and guessed volatile
storage remain invalid. Allocator's only adjacent LEA/store mismatch is separate.

## Previous source checkpoint: complete GameManager call binding repair

[The complete proof](evidence/game-manager-bindings-20261031/README.md) adds
GameClearLinkedObjects41777066/all1 exact in GameObjectReset.cpp. It captures next
before virtual slot4, rereads links to unlink, performs scalar delete and advances
through captured next. Existing laser views supply the layout; original ownership
remains unknown. Target GameManager has two actual calls to this complete owner.

Six GameManager facades now name actual pool reset, enemy clear, music, stage
destructor, transition factory and Main mode functions. Main mode is not a sound
call; demo operands are priority43/kind5/60/three zeros. The terminal cleanup's
417FAC edge also confirms real EnemyManagerClear. Four actual existing TUs plus
the new cleanup join the previous23-input graph, with their pinned profiles.

The complete parent evolves1544 to1602 (pool reset inlined),1528 (real pool call
boundary),1533 (three final flag checks) and1535 (sound selection branches).
Final1535/1544 remains NONEXACT:114 fields,59 locals versus target60, all36 actual
callee identities in order. The27-byte final gate is raw-equal and the31-byte
sound gate has four bound fields/15 ordinary bytes equal; no fragment credit.
Typed stage delete is fully byte/field neutral and restored. Terminal cleanup
remains1108/1105. Replay1000/582, stage destructor460/499, reset184/214 and the
embedded/binder interfaces remain unequal. Begin stays2424/2432.

Current-source single-TU OnUpdate rejects10 bytes: ECX moves to EBX and its core
returns plain RET. Only game-manager-update-callback migrates to the actual28-input
GameObjectReset recipe, restoring7/all1 with stack/RET4 core. Its indeterminate
origin and existing credit remain unchanged. Failed native image/report retained.

Two independent28-input canonical colds, with75 transitive pins, pass new66/all1
and restored7/all1. First also cold-passes unchanged transition119 and delay92
recipes. Both28-input images pass23 existing contracts3054/all112; the full stage
destructor rejection460/499 remains explicit. Header Tick's PDB owner is primary
Enemy in this graph. Comparator/profiles unchanged; no full-cohort replay.
Producer: build/match-linked/GameObjectReset. Frozen colds and current baseline
context are under the private game-manager-bindings-20261031 campaign. Earlier
GameManager source freezes are historical; check pins before reuse.

Concrete coupled lead: target Replay BeginStage582 has a counted playback pair
loop and four resetFlag stores after it; current1000-byte native playback is
unrolled with resets inside the iterations. Target also rereads g_Player after
position/rebuild calls and g_CurrentStage across stage helpers. Review the actual
helper operands/bodies before changing declarations; live registers can carry
meaningful inputs. Do not infer a one-argument helper from stack usage alone.

## Previous source checkpoint: stage animation reset interfaces

[The complete proof](evidence/stage-animation-slots-20261031/README.md) adds
GameEnterMenuState404530119/all2 exact and complete nonexact
GamePrepareAnimationSlots404450214 source in StageAnimationSlots.cpp. Both are
actual GameManager dependencies; their former unresolved anchors disappear.
The transition uses the stage-compatible timer30/flags2 layout and the real
priority15/kind2 effect factory. Original names/classes/TU/storage remain unknown.

With actual TransitionEffect but missing Chain, the caller118/factory81 use
kindEBX instead of target priorityEBX. Adding existing Chain.cpp alone, reusing22
verified inputs, recovers caller119/all2 and factory82/all2. No source/profile
change or synthetic caller supplies that register choice. One local real factory
boundary preserves the observed call; TransitionEffect/Chain use their existing
canonical factory profiles. The new recipe retains the real InitializeVm entry.

Two independent23-input canonical colds, with68 transitive source/header pins,
pass the full119 bytes and both fields. Fourteen same-image existing contracts
pass2417/all89 in each; the affected canonical stage-delay92 also cold-passes.
Only game-enter-menu-state is new exact credit. No boundary or denominator change.
Producer: build/match-linked/GameStageSlots; frozen images and logs are under the
private anm-embedded-context-20261031 campaign. Other canonical graphs are unchanged.

Reset remains184/214 with private EDI/plain RET versus target stack/RET4. Its
actual-entry control204/214 is also nonexact and is not retained. GameManager
remains1544/1544 nonexact. Begin2424/2432, embedded200/202 and binder197/203 are
unchanged in this actual23-input context. Do not repeat the actual-entry or old
formal-order/reset-boundary controls without new evidence. Unowned4415C0/41BE40
raw call hints remain unpromoted; Ghidra has no function descriptors or known
entry references there. No artificial callers were added. Earlier20-input
freezes now predate the GameManager declaration/comment update; check pins before
reusing them, and do not migrate their recipes merely to include these sources.

## Previous source checkpoint: complete spell-start stage lifecycle

[The complete review](evidence/enemy-spell-stage-20261031/README.md) retains a
natural two-input lifecycle helper used by all nine stage choices. The full
EnemyBeginSpell candidate changes2476 to2424/2432, still **NONEXACT**. The
intermediate2432-byte result was accidental length agreement: layer-first source
recovers the target memory OR and removes eight bytes. Stage1 arm order and
stage7 condition/resource order now follow target. Existing insertion pragma,
four actual stack arguments and RET16 are retained; no new ABI/inline controls.

Independent full inventories bind108 fields each, including all41 actual calls,
and validate81 direct locals plus seven table destinations. All nine selected
stage paths carry the target resource/VM/script/flag/layer/registration events.
The stage code is639 bytes with59 equally positioned bound fields and48 ordinary
differences in14 ESI-versus-EDX receiver LEAs/eight adjacent PUSH schedules. Its
cases are nine bytes early and table eight early; local offsets are not masked.
Prefix strings/registers and child searches remain unlike. No fragment credit.

An incremental and a zero-reuse20-input cold reproduce the same complete owner,
with64 current source/header pins. Twelve same-image canonical contracts pass
2243/all86; independent selected sound123 and shield36 colds pass159. No ledger
input changes. Current frozen artifacts are in the private enemy-spell-begin-
20261031 campaign; older20-input freezes now predate this Dispatcher.cpp change.

Typed statistics, global/local reset-boundary and flag-bitfield controls are
restored and parked. Initialize remains273 structurally equal, embedded reset
200/202 and front binder197/203. Do not repeat these or the historical child-return
helpers without new evidence. The stage residual is now localized to the genuine
embedded initializer's private interface and associated caller scheduling; fake
unused parameters/clobbers remain invalid.

## Previous source checkpoint: spell finish, power and notice boundary

[The complete proof](evidence/gui-spell-notice-20261031/README.md) adds
EnemyEndSpell399/all20 fields and AddPower166/all4 fields: **565 new exact
authored bytes**. Two independent20-input colds reproduce both; the first also
passes affected AddRank42. Twelve same-image owners2243/all86 pass in both.

GuiSpellNotice.cpp completes the actual ECL spell-end interface and seven-case
GUI notice source. Existing statistics records[group]/records[6] and a90-byte
spell-record view recover the whole399-byte owner. Full ID clearing and one
local genuine creator boundary recover AddPower224 to166. No shared header,
ANM source, profile or comparator change is retained.

Notice4172E0 owns640 code bytes plus its immediately following28-byte seven-way
table through41757B. The old640-byte boundary omitted that owned table; the
reviewed authored denominator increases28, with zero credit from that correction.
Complete notice remains660/668, NONEXACT. Its frame28, actual EAX kind/two stack
values/RET8 and all cases are maintained. Broad clears merge tails to576;
flags/word-copy/whole-ID/named-script controls do not close the owner and are
restored. Remaining leads are the loop load/OR/store versus memory OR, digit
ID publication register and clear/resource-load schedules. No partial-case credit.

Actual EnemyRuntime dispatcher14348/14416 retains one direct EndSpell call;
Item update2208/2320 remains nonexact. New explicit canonical selectors are
enemy-end-spell and game-score-add-power. Producer is
`build/match-linked/GuiSpellNotice`; frozen colds and current64 source/header
pins remain under the private gui-spell-notice-20261031 campaign. Old score
contexts now predate the ItemManager.cpp AddPower update; recheck inputs before
reuse. Both existing score/life owners remain exact in the current20-input image.

## Previous source checkpoint: score display and extra lives

[The complete proof](evidence/gui-score-display-exact-20261031/README.md)
adds GameUpdateGui671/all36 fields and GameAddLives141/all12 fields: **812 new
exact authored bytes**. Both complete owners and every fixed local destination
pass two independent19-input cold links. The first scoped cold additionally
passes AddRank42; both images retain the selected eight-owner1490/all57 proof.

GuiScoreDisplay.cpp completes the existing GameManager and Item caller
interfaces. Reusing GameScoreState storage, full ID clears, two local creator
call boundaries and a count-first free interface recover the actual private
ABIs without shared-header/profile/ANM-source changes. Names, original class/TU,
data ownership, product and runtime remain open.

The refreshed17-input main GUI updater baseline remains3836 bytes with four
ordinary timer differences.
Message-result/frame-assignment/frame-order controls merely move that residual
and are restored. ECL validation/release helpers, reference release, memcmp,
late cross-TU helpers and direct clear controls remain547/589 and are restored.
Do not repeat these models without new evidence.

Canonical producer: `build/match-linked/GuiScoreDisplay`. Frozen cold images,
logs and source pins remain under the private gui-score-display-20261030
analysis/build campaign. Explicit new selectors are gui-update-score-display
and game-add-lives; item-rank-add is the affected existing cold guard.

## Earlier source checkpoint: Ending

[The complete Ending proof](evidence/ending-native-record-20261030/README.md)
is the previous source checkpoint, commit `58bb846`.

- Run emits the complete 1,568-byte owner: 1,496 code/alignment plus 72 table
  bytes. All 92 field positions, 53 direct local destinations and 18 table
  destinations coincide. **54 ordinary bytes still differ: NONEXACT**.
- Native scalar `EndingRecordInt`, current instruction member accesses, real
  `ChainElem` construction and path capture before buffer clearing are retained.
  Initialize remains 584/584 and animation loading 131/126, both nonexact.
- Actual `VmIdInterrupt.cpp` restores the pending-ID EAX interface in the
  21-input diagnostic. It does not migrate the canonical 17/20-input graphs.
- Explicit canonical create/delete/text-constructor/LoadScript colds passed
  871 bytes. Same-image constructor/loader/decoder/pending passed 860/all19,
  with strict worker-member EH binding. No new exact credit was claimed.

Keep the existing profiles and checksum root. Shared-ID noinline, ordinary
release helper, returned ID, loading bit, timestamp, lookup/text and cursor
controls were restored and parked. Reopen only with new evidence and a
prediction that distinguishes the proposed source form.

Expanded diagnostic context:
`.analysis/gpt-6.1-sol/ending-run-context-20261030/final-source-context.json`.
Its70 source/header pins predate the current ItemManager.cpp caller update.
Recheck every input, object hash and image hash before incremental reuse; an
old image or matching subset is not an exactness receipt.

## Most recent unfinished investigation: ECL script database

`EclVmScriptDatabase::AddScriptData` at `0x00450220` has a complete 589-byte
target. A one-source Run-entry graph emitted 528 bytes with the wrong private
interface. Adding the **actual Enemy.cpp caller/vtable definitions**, under the
existing /GL profile and unchanged Run entry, restores ECX this, one stack
argument and RET4; the complete candidate is 547/589 and remains nonexact.
No source edit or canonical AddScriptData unit was adopted.

The scoped canonical `ecl-vm-find-subroutine` replay passed its existing 136
bytes. The two-source diagnostic also emits the same 136 bytes; this observation
adds no canonical credit. See the [maintenance record](evidence/maintenance-20261030/README.md)
for the retained graph and comparison summary.

Expanded context:
`.analysis/gpt-6.1-sol/ecl-script-data-20261030/actual-caller-context.json`.
All 17 source/header pins still match. The adjacent entry context records the
separate one-source graph; do not substitute it for the caller-bound graph.
Target/native listings and bounded probe runners remain beside the contexts.
Remaining leads concern the validation slot reload, old-table copy/free guard,
unsigned positive-count branch and insertion-loop lifetimes. The helper controls
listed above were tested and restored; no improved rewrite was retained.

## Other retained frontiers

These are measurements at their linked evidence epochs, not fresh comparisons
of today's entire source tree. Relink the actual graph before resuming it.

| Owner | Last recorded native / target | Evidence |
| --- | ---: | --- |
| Enemy integer operand reader | 820 / 820, exact, all95 fields | [Complete accepted proof](evidence/enemy-operands-exact-20261030/README.md) |
| Enemy float operand reader | 876 / 884, nonexact | [Operand proof and parked controls](evidence/enemy-operands-exact-20261030/README.md) |
| Replay-save update | 1,444 / 1,460, nonexact | [Actual callee graph](evidence/replay-save-context-20261030/README.md) |
| MusicRoom update | 2,788 / 2,796, nonexact | [Complete owner review](evidence/music-room-helpers-exact-20261030/README.md) |
| GUI message Run | 4,496 / 4,480, nonexact | [Complete interpreter](evidence/gui-message-run-context-20261030/README.md) |
| GUI update | 3,836 / 3,836, four ordinary differences | [Complete updater](evidence/gui-update-context-20261030/README.md) |
| ANM ExecuteScript | 9,960 / 9,964, nonexact | [Call-boundary proof](evidence/anm-timer-boundary-20261030/README.md) |
| Stage dispatcher | 1,868 / 1,868, nonexact | [Dispatcher proof](evidence/stage-dispatch-context-20261029/README.md) |

Equal extents, normalized instructions and partial byte equality receive no
exact credit. The [dated prior handoff](RE_HANDOFF_HISTORY_20261030.md) retains
Player and other detailed leads, rejected controls and earlier source epochs.
Use its linked proofs for a selected owner; do not inherit its old “current”
labels or rerun all historical experiments.

## Cache and storage recovery

The [source-pin audit](evidence/maintenance-20261030/context-audit.json) found
570 stale contexts among 584 manifests with nonempty pin maps. Fourteen matched
source/header pins; that count says nothing about complete native acceptance.
In particular, old ANM executor `final-context.json` has a changed
`AnmManager.hpp` pin. Replay-save and music-helper colds have a changed Enemy.cpp
pin. Old Player main-callee contexts have several changed inputs. None is a
current warm-cache receipt.

[RE_ARTIFACTS.md](RE_ARTIFACTS.md) lists expanded campaigns, verified archives
and selective restoration. Current Ending/ECL/Enemy and other pin-compatible
campaigns remain expanded, together with canonical `build/match-linked/`
outputs. Historical controls, copied objects, listings and reports were
losslessly archived with per-file hashes before removing expanded copies.
Original target, Ghidra database, toolchains/Wine, reference checkouts and
[D3D adapter evidence](evidence/d3d-adapter-format/README.md) are preserved.
The D3D hypothesis remains unadopted; do not resume its consumed one-shot probe.

Static music parser selectors require their actual PDB producer directory,
`build/match-linked/MusicRoomHelpers`. A restored snapshot does not satisfy
producer qualification. Rebuild the declared graph there when needed; never
bypass the ownership check or overwrite historical pins to make a cache pass.

## Checkpoint rules

Use `scripts/repo-python`; verify the private target before target-dependent
work, check toolchain declarations and attest Ghidra before using observations.
Native jobs stay serial, CPU0/nice10, bounded and logged. Complete bytes, every field,
fixed local destinations, required callees and actual ABI remain mandatory.

Cold-replay only new/affected exact owners with explicit `--unit` selectors.
Do not replay every consumer merely because it shares a file. Ordinary exact
work requires no host failure/lifecycle/sanitizer/mutation campaign.
`config/transition-effect-host-inputs.json` remains retired provenance.

Finish with the affected native comparisons, the default seven lightweight
`scripts/ci.py` checks and `git diff --check`. Regenerate progress when ledger
inputs change. Commit locally with the specified author/coauthor and `gpt-6.1-sol:`
prefix; never push.
