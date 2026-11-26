# TH10 reconstruction handoff

Read [AGENTS.md](../AGENTS.md), then this brief and the relevant source.
[PROGRESS.md](PROGRESS.md) is generated from the ledgers;
[matches.csv](../config/matches.csv) is the exact-count authority.

## Continue with StartSubroutine

The next owner is `EclVmContext::StartSubroutine @0x0044DF70`, 550 bytes in
`src/EclVm.cpp`. The operator resumed it after the other ready owners, then
requested cleanup and a handoff. No reconstruction job is running.

The committed source is the current cold-compared 542/550 baseline. Its
complete extent and four REL32 fields bind; eight ordinary bytes still
have different registers. Neither the unsigned float-index snapshot nor
the shared tag/word union improved it, so both trials were discarded.

The live `build/match-linked/EclVmHostRunEnemy` PE and objects were last
produced by the discarded tag/word trial. Its owner bytes and fields are
byte-neutral against the baseline, but rebuild from the restored source
before further compiler observations or warm probes. The baseline report is
`.analysis/ecl-start-subroutine/resumed-current-cold-compare.json`.

Use the two-input /GL recipe `ecl-vm-start-subroutine`, rooted at the real
EnemyRuntimeUpdate. Verify the original Japanese v1.00a input and toolchain
at session entry. Select an available CPU; CPU 2 was used here. Keep native
jobs serial at nice 10 and retain their logs and actual exit codes below
`.analysis/`. Run the comparison only after a successful build:

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
timeout 900s nice -n 10 taskset -c 2 scripts/repo-python scripts/build-match-unit.py --unit ecl-vm-start-subroutine
scripts/repo-python scripts/compare-linked-function.py --unit ecl-vm-start-subroutine --json
```

The expected diagnostic result is a 550-byte candidate with 542 matching
bytes and four fields. It is not an accepted match.

| Remaining offsets | Native | Target |
| --- | --- | --- |
| `+0xC7`, `+0xCE` | Float word in ECX | EDX |
| `+0xF3`, `+0xF7`, `+0xFD`, `+0x100` | Cursor EAX, instruction ECX | Cursor ECX, instruction EAX |
| `+0x105`, `+0x10F` | Count EAX | ECX |

The integer path matches through `+0xC5`. Float-word preference and latch
ordering are separate problems. Equal-priority latch candidates are ordered
by instruction-walk fields 106/105/104; reversing source reads fixes their
register roles but breaks the schedule. The count's EAX choice is independent.
The current baseline needs no integer-store compiler barrier. Keep the raw
float bit carrier and post-resolver instruction reloads.

[KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) retains the C2 findings and failed
controls in its StartSubroutine entry. Three read-only trace families remain
under `.analysis/ecl-start-subroutine/`: `terra-float-costs2`,
`terra-latch-tie-write`, and `terra-latch-count-scores`. The last traces an
older volatile-count control. Debugger priority changes are not acceptance
evidence. The warm relink helper remains `.analysis/update-replay/relink.py`.

## Completed work

Use the canonical ledgers and recipes rather than old scratch snapshots.
Recent source/context repairs are summarized in the knowledge base:

| Scope | Canonical units |
| --- | --- |
| Natural VM initializer member | `anm-vm-initialize-private`, `entity-start-script` |
| Popup insertion, resource reload and factory | `script-viewer-emit-value-popup`, `script-viewer-popup-initialize`, `script-viewer-popup-create` |
| Real file/archive consumers | `game-statistics-constructor`, `game-statistics-create` |
| Manager list traversal and EH | `anm-render-manager-destructor` |
| Typed laser construction | `enemy-laser-spawn` |
| Key binding and complete draw table | `frontend-assign-keyconfig-binding`, `frontend-draw-dispatch` |
| Player option-state construction | `player-constructor` |
| Spell creation appearance input | `enemy-start-spell` |

The old ASCII initialization call-binding issue is resolved by the natural
VM member. A separate retained `gui-message-destructor` image in
GameCleanupContext was reported at 89/97; a control from before the GUI
work reproduced it. Its older canonical receipt remains distinct from that
retained-image issue. This cleanup does not establish a new comparison for it.

[RE_ARTIFACTS.md](RE_ARTIFACTS.md) describes protected inputs, dirty historical
worktrees and the reduced private context. No game code was rebuilt for
this cleanup. Continue with the single owner above; replay affected consumers
only after a source change reaches a complete exact result.
