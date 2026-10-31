# TH10 handoff — 2026-10-31

## Start here

Follow [AGENTS.md](../AGENTS.md), then read the selected owner's source and
ledger rows. [RE_WORKFLOW.md](RE_WORKFLOW.md) defines the bounded exact loop;
[ORACLES.md](ORACLES.md) defines acceptance. Call local tools directly.
There is no running compiler/linker job or unfinished source edit.

[PROGRESS.md](PROGRESS.md) is generated from the ledgers and is the count authority:

| Measure | Current count |
| --- | ---: |
| Reviewed authored owned bytes exact | 113,492 / 275,890 |
| Authored exact owners | 527 |
| Total canonical exact owners / codegen bytes | 1,407 / 181,626 |
| Source mappings / authored mappings / authored exact backlog | 1,221 / 703 / 176 |
| Reviewed boundaries / authored functions | 1,819 / 747 |

The denominator and source/origin inventory remain provisional. Exact codegen
proves neither original names/class/TU ownership nor product/runtime/portability.

## Latest reviewed authored exact checkpoint

[AsciiManagerCreate @401440](evidence/ascii-factory-exact-20261031/README.md) is now complete **69/69 exact** (five allocation/constructor/initializer/destructor/deallocation REL32 fields) in the genuine three-input ASCII initialization graph. The old 70-byte one-TU candidate had the wrong destructor private receiver; neither source ABI nor factory code needed alteration. Two independent selected colds preserve the new69 and previous initializer330 + high-draw10, **409/409 exact** across34 fields. Reviewed authored exact progress rises to113,492/275,890 across527 owners.


[AsciiManagerView::OnDrawHighPriority](evidence/ascii-high-draw-exact-20261031/README.md) is now full **10/10 exact** with its real `AsciiManagerDrawGuiStrings` REL32, recovered by a natural VM pointer in the genuine GUI renderer. The selected 3-TU `AsciiManagerCreate` source graph yields the target EBX receiver; two independent selected cold links preserve new callback10 and prior Initialize330, **340/340** across all29 fields. The larger 570-byte renderer stays **NONEXACT** despite improving from54/458 to160/458 comparable positions. This new callback raises reviewed authored exact to113,492/275,890 across527 owners.


[ResultMenuUpdate @4223F0](evidence/result-menu-update-exact-20261031/README.md) is now complete **206/206 exact**, including the formerly excluded 27 bytes of physical jump/selector tables. All16 external/local fields and five table destinations bind in two independent selected cold links. The previous provisional boundary stopped at the RET after 179 bytes; the corrected extent ends at 4224BD, two CC bytes before the next owner. Exact authored progress increases to 113,492/275,890 and one new owner; the reviewed denominator is still provisional.


[GameConfigurationView::ResetDefaults @418C40](evidence/game-configuration-reset-exact-20261031/README.md)
now passes all **125 bytes and all five fields** from its real
MainLoadConfiguration caller graph. Moving the existing `colorMode = 0`
assignment ahead of numeric defaults recovers the target codegen; the prior
order was 116/125 nonexact in the same graph. An incorrect double-addend
manifest was independently repaired. Two **selected** cold replays pass;
no historical whole-cohort replay is required. Current reviewed authored
exact total is 113,492/275,890 across 526 owners; the denominator remains
provisional.

Sound PreloadBgm has a separate [bounded nonexact correction](evidence/sound-preload-reg-contract-20261031/README.md)
that recovers the real GetFmtIndexByName ECX input; do not admit its
369-byte owner merely because it has a full-size candidate.

## Recent Hint exact source checkpoint

[Hint frame Update419650..41988A](evidence/hint-frame-exact-20261031/README.md)
is complete571/all17 exact, committed in `9bd5b01`. It has169 instructions,
23 fixed local branches, EBX owner and RET at23A. Repeating the width-array
expression at its three genuine uses makes ordinary CSE/loop reduction select
width base17C; the target store/reloads, manager field72 and alignment follow.
No extra induction pointer, register forcing, padding, header or flag changes.

The existing update callback10/all1 remains exact and origin-indeterminate.
Warm and zero-reuse eight-input cold agree; independent canonical selects only
`hint-frame-update` and `hint-update-callback`,581/all18 in one producer.
Same-image factory/draw guards also pass. Actual ConsumeEntries remains
1308/1317 NONEXACT and passes its previous full proof in both final images.

Current complete build/cache context:
`.analysis/gpt-6.1-sol/hint-frame-loop-20261031/final-cold-context.json`.
Its36 source/header pins match this checkpoint. Check the actual graph,
profiles, objects and image hashes before incremental reuse. The normal
canonical producer is `build/match-linked/HintLifecycle`.

The complete 9,964-byte ANM `ExecuteScript` now strictly reproduces **9,962 bytes** using a local timer-read helper and a scoped radial Z read. Independent five-source cold and all350 field bindings confirm the remaining two bytes; see [the large-owner proof](evidence/anm-executor-two-byte-frontier-20261031/README.md). Exact totals are unchanged because the whole owner still rejects.

## Open owners worth resuming

These are complete measurements at the linked proof's source epoch, not fresh
comparisons of every owner against today's source. Choose one bounded owner.

| Owner | Last complete snapshot | Evidence / concrete remaining lead |
| --- | --- | --- |
| Sound PreloadBgm43D7D0 |365/369 NE|[Read-register experiment](evidence/sound-preload-barrier-20261031/README.md): ten bound fields and all 121 instructions match except four EAX/EDX register bytes; affected existing units cold-pass 1641/all |
| Hint ConsumeEntries419120 |1308/1317 NE|[Consumption proof](evidence/hint-consume-context-20261031/README.md): small-ID result/register order and common script PUSH; actual world creator116/118 remains NE |
| GUI message Run |4472/4480 NE|[Latest state view](evidence/gui-message-state-view-20261031/README.md): counter placement, cursor and remaining register/read schedules |
| Capture RequestCaptureRectangle424480 |148/162 NE|[Full strict context](evidence/capture-rectangle-schedule-20261031/README.md): real FindVm EDX callee recovered; 23 to 14 remaining store-schedule differences; two existing actual callers cold-pass 822/all |
| GUI DrawStageElements415800 |724/724 NE|[Latest arithmetic proof](evidence/gui-draw-read-order-20261031/README.md): first rectangle LEA targetD6/nativeEC, addition field targetEC/nativeE8 |
| GameManager cleanup / update |1098/1105 and1538/1544 NE|[Cleanup controls](evidence/gui-message-teardown-20261031/README.md): transition flag publication/block placement and VM-call/private helper interfaces |
| ECL AddScriptData450220 |547/589 NE|[Caller-context record](evidence/maintenance-20261030/README.md): actual Enemy caller restores ABI; validation-slot reload, old-table guard and insertion lifetimes remain open |
| Enemy float operand reader |876/884 NE|[Operand proof](evidence/enemy-operands-exact-20261030/README.md): float family/source ordering controls remain unresolved |
| ANM ExecuteScript |9962/9964 NE|[New arithmetic proof](evidence/anm-executor-two-byte-frontier-20261031/README.md): Z read and timer x87 schedule recovered; only child91 EDX/ECX push remain; all350 fields/95 locals strictly bound |
| Ending Run |1568/1568 NE|[Ending proof](evidence/ending-native-record-20261030/README.md): old complete comparison retains54 ordinary differences; current inputs have changed |

The GUI stage updater is already complete3836-byte exact; do not reopen the
old four-byte residual. See [its accepted proof](evidence/gui-update-exact-20261031/README.md).
The integer operand reader is already exact too. Use the ledgers before
selecting a similarly named old candidate.

Parked Hint models: separate width induction spills; width-derived slot indexing
adds arithmetic; local width address and owner/slot lookup forms are neutral;
typed position view selects the wrong x-based cursor. New repeated-width source
has superseded that loop frontier. For consumption, lookup-value projection and
receiver/endpoint spelling controls are neutral. Reopen only with new evidence
and a distinguishing prediction.

## Cache and artifact recovery

[RE_ARTIFACTS.md](RE_ARTIFACTS.md) lists expanded working contexts, the verified
private archives and selective restoration. Ended campaigns are archived with
per-file hashes; their old paths do not imply expanded or reusable inputs.
The current [cleanup audit](evidence/maintenance-20261031/README.md) records scope,
storage savings and source-pin mismatches. In particular:

- Latest GUI Run context differs at Hint.cpp; its4472-byte image is historical.
- ECL actual-caller context differs at AnmManager.hpp, Enemy.cpp and GameScoreState.hpp.
  The pin-equal one-source entry is a different graph with the wrong ABI.
- Cleanup/message contexts differ at Gui.cpp and Hint.cpp. Ending has ten changed inputs.
- The current Hint and latest GUI draw pins match, but matching pins alone do
  not establish graph/ABI correctness or complete native equality.

Caution424650 calls AddAutomaticEntry41A120, not ConsumeEntries or Update.
Its extra replay in older Hint campaigns was unnecessary and is not a gate.
Preserve original target, Ghidra, toolchain/Wine, reference checkouts and the
[D3D adapter evidence](evidence/d3d-adapter-format/README.md). That D3D hypothesis
is unadopted; do not rerun its consumed one-shot probe. Music parser qualification
requires the real `build/match-linked/MusicRoomHelpers` producer path.

## Validation and history

Verify the target before target work; attest Ghidra before using observations.
Keep compiler/linker jobs serial, CPU0/nice10, bounded and exit-logged. Compare
complete owners, every field, fixed locals, required callees and actual ABI.
Replay only new/affected exact owners with explicit `--unit` selectors.

Default `scripts/repo-python scripts/ci.py` runs seven lightweight checks.
Select comparator self-tests only for a changed mechanism. Historical host
failure/lifecycle/sanitizer/mutation reports are not current gates. Product
closure, semantic dual-Oracle work and portability remain later phases.
Regenerate progress when ledger inputs change; finish with `git diff --check`
and a local English `gpt-6.1-sol:` commit using the specified author/trailer.
Never push.

The previous858-line handoff is recoverable with
`git show 9bd5b01:docs/RE_HANDOFF.md`. Older detail remains in
[the dated handoff](RE_HANDOFF_HISTORY_20261030.md), linked evidence and Git.
Old sizes, open statuses, test lists and “current” cache labels do not override
current ledgers, source pins or the latest complete owner proof.
