# Supervisor mode call graph — 2026-10-29

Five newly exact owners add **399 bytes:363 authored and36 origin-indeterminate**.
A previously exact23-byte stage helper is retargeted to natural typed source,
without adding bytes or mappings. Two independent canonical22-input cold links
reproduce all six selected owners, **422 bytes/all18 fields**.

| Complete owner | Bytes | Fields | Selector |
| --- | ---: | ---: | --- |
| Supervisor update `41FF80..41FFF0` | 113 | 7 | `supervisor-update-callback` |
| Indexed lock release `421C50..421C68` | 25 | 1 | `supervisor-leave-lock` |
| BGM fade maintenance `421E00..421EE0` | 225 | 4 | `sound-service-bgm-fades` |
| Nullable GameManager delete `418150..418161` | 18 | 2 | `game-manager-delete` |
| Nullable FrontEnd delete `42CDB0..42CDC1` | 18 | 2 | `frontend-delete` |
| Existing stage publication `421B70..421B86` | 23 | 2 | `game-score-select-next-stage` |

The two nullable helpers and existing stage helper retain indeterminate origin.
The other three retain their prior authored classifications. Compiler equality
does not establish original declarations, class/TU ownership or storage closure.

## Complete larger owner and boundary correction

The actual startup-factory caller4218D0 owns **666 bytes**, through421B69:
572 executable bytes, a16-slot absolute main table64 bytes, a4-slot inner table16
bytes, and a14-byte compressed index map. The old572-byte ledger ended after a
backward JMP and omitted all94 table bytes. Both table selectors and every slot
were read directly; all destinations are code instruction starts. The main
selector is bounded by15 and the inner index by13. The compressed map chooses
only the four reviewed inner targets. Six CC bytes precede independent421B70.

Every table byte was queried in the attested read-only Ghidra database. Only the
three owning indexed reads are observed. Ghidra associates the byte-map reference
with421B5B; the raw MOVZX operand and its consumed range establish the actual base
421B5C through421B69. Provisional database association does not override bytes.
The authored reviewed denominator gains94 owned data bytes; none enter exact
credit while the parent remains nonexact.

`MainSupervisorView::UpdateGameMode` now has complete natural source, including
startup failure/exit, menu transitions, game initialization/retry/stage changes,
Ending and shared lock release. The final native owner is678/666. Initial686,
then678, and grouped/fallthrough/shared-cleanup694 controls are retained privately.
The latter controls and a byte-neutral generic normal-unlock control are restored.
Target menu creation and the normal epilogue occupy a different position; stage
stores/load scheduling also differs. Park block-spelling controls pending actual
callee/context evidence. No partial parent-byte or table credit is admitted.

## Actual callback and field recovery

The parent has a real caller41FF80 and its callback address escapes at420476.
Without registration source the compiler naturally gives that callback a private
ESI input and107 bytes. Restoring complete registration makes it retain the
observed ECX input/save/restore and all113 bytes. No fake ABI or unused argument
is introduced. The actual mode callee retains EAX input/plain RET, while its full
body remains nonexact; the callback grants address/interface binding only.

Registration420470 itself uses globals and has no input. The old opaque Main
seam supplied an extra logical pointer. The source now initializes mode38C=-2,
mode390=0 and unknown398=0, publishes update/added callbacks, and registers draw
priorities1/40/50. It emits204/204 but still swaps EBX/EBP allocations relative
to the target. Size equality is not exactness; registration stays a candidate.

The fresh call chain also corrects an old navigation description:421E00 services
the actual Sound owner492590, rather than generic game-object cleanup. Its four
independent tests use fade types1/2/4/3, signed decrement and integer attenuation
formulas. Completed type1 stops only the primary DirectSound buffer; other types
clear their type without a final volume write. Capturing the current sound for
each test removes unwanted reloads within a test (273 to225), while preserving
reloads after external calls. All four SetVolume calls bind the real106-byte owner.

The retained stage helper copies GameScoreState nextStage+40 to selectedStage+3C
and publishes route474788+index*30 through477848. Those physical addresses equal
the earlier separate474C80/474C7C aliases. Its maintained void member reproduces
the target's incidental EAX descriptor pointer; no consumed return is invented.
Legacy Decomphelp source is untouched. Replacing its primary mapping/selector
grants no additional23-byte credit and does not settle source-versus-copy origin.

GameManager's existing60-byte view is shared through a header without changing
its fields. Supervisor mode fields, startup slot768, gate648, close gate63C and
clear color780 have independent offset assertions. These are target-backed partial
views, not original class-size or product-storage proofs. Opaque creation/cleanup
and unimplemented callback declarations are retained as actual unresolved seams,
not substitute function bodies. Mechanical stack-owner/RET4 contracts are checked;
callee byte equality is claimed only where its own unit passes.

## Focused native proof

Both cold builds compile all22 declared inputs with pinned VC7.1 profiles and
`/GL`; no objects are reused. Complete qualified PDB extents, every field and fixed
local destinations replay zero-difference for the six selected owners. The same
images also pass34 explicit old contracts6535/all221, including actual SetVolume,
ANM service, GameManager adapters, Ending creation/delete, Startup and Sound peers.
These are selected same-image comparisons, not full historical-cohort replay.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit supervisor-update-callback --unit supervisor-leave-lock \
  --unit sound-service-bgm-fades --unit game-manager-delete \
  --unit frontend-delete --unit game-score-select-next-stage --json
```

Private recipes, snapshots, raw boundary/ABI records, complete receipts/listings
and exit logs are under `.analysis/gpt-6.1-sol/supervisor-modes-20261029/`.
First independent witness: `build/gpt-6.1-sol/supervisor-modes-20261029/cold-a/`;
second: `build/match-linked/SupervisorModes/`. Public source/artifact identities
are in [results.json](results.json).

The mechanical case-reordering compile error was rejected before linking. An
initial cold preflight also rejected overlapping old/new stage selectors; the
old selector was removed, with its source/evidence retained, before either cold
build. Neither rejected attempt is a proof run. Lightweight CI and whitespace
checks pass; no comparator mechanism or runtime/host-scenario test changed.
