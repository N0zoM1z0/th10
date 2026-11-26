# TH10 reconstruction handoff

Read [AGENTS.md](../AGENTS.md), then this brief and the relevant source.
[PROGRESS.md](PROGRESS.md) is generated from the ledgers;
[matches.csv](../config/matches.csv) is the exact-count authority.

## Continue with StartSubroutine

The next owner is `EclVmContext::StartSubroutine @0x0044DF70`, 550 bytes in
`src/EclVm.cpp`. The operator resumed it after the other ready owners, then
requested cleanup and a handoff. No reconstruction job is running.

The committed source is the current cold-compared 544/550 baseline. Its
complete extent and four REL32 fields bind; the float-word mismatch at
`+0xC7`/`+0xCE` is now resolved by keeping `argument` non-volatile in the loop
body and passing `instruction->floatOperands[valueOffset / sizeof(int)]`
directly to `ReadFloatValue(caller, argumentIndex, ...)` (removing the local
`EclVmScalar value` carrier and float-arm `source = caller` snapshot), while
retaining a volatile header instruction reload. Six ordinary bytes remain, all
from a single `EAX`/`ECX` swap in the loop latch (`+0xF3..+0x10F`). The live
`build/match-linked/EclVmHostRunEnemy` PE and objects were cold-built from the
committed source; the baseline report is
`.analysis/ecl-start-subroutine/cold-544-compare.json`.

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

The expected diagnostic result is a 550-byte candidate with 544 matching
bytes and four fields. It is not an accepted match.

| Remaining offsets | Native | Target |
| --- | --- | --- |
| `+0xF3`, `+0xF7`, `+0xFD`, `+0x100` | Cursor EAX, instruction ECX | Cursor ECX, instruction EAX |
| `+0x105`, `+0x10F` | Count EAX | ECX |

Everything through `+0xF1` (242 bytes, including both operand branches) and
from `+0x110` through `+0x225` (278 bytes) matches byte-for-byte. Equal-priority
latch candidates are ordered by instruction-walk fields 106/105/104; reversing
source reads fixes their register roles (`EAX`/`ECX`/`EDX`) but breaks the
schedule when both reads are volatile. A fully non-volatile IVR loop
(`stack.data[argumentOffset]` and `(metadataOffset + 4u) / 4u`) naturally
assigns the target latch registers (`ECX`/`EAX`/`EDX`/`ECX`) and float word
(`EDX`), but currently swaps `valueOffset`/`instruction` at `+0x68..+0xC6` and
reorders two latch instructions (`+0xF6`, `+0x107`).

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
