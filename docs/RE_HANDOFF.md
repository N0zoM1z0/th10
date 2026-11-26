# TH10 reconstruction handoff

Read [AGENTS.md](../AGENTS.md), then this brief and the relevant source.
[PROGRESS.md](PROGRESS.md) is generated from the ledgers;
[matches.csv](../config/matches.csv) is the exact-count authority.

## Authored exact backlog complete

`EclVmContext::StartSubroutine @0x0044DF70` (550 bytes in `src/EclVm.cpp`) is
now an accepted cold-verified exact match in `ecl-vm-start-subroutine`,
completing all confirmed authored functions in the canonical ledger (see
[PROGRESS.md](PROGRESS.md) and [matches.csv](../config/matches.csv)).

The two-input `/GL` recipe `ecl-vm-start-subroutine` (`src/EclVm.cpp` +
`src/Enemy.cpp`, rooted at `EnemyRuntimeUpdate`) reproduces all 550 bytes and
all four `REL32` call fields (`ReadIntValue`, `ReadFloatValue`, `__ftol2`,
`FindSubroutine`) with zero differences; sibling exact units in the same
image (`ecl-vm-host-run` 146/146, `ecl-vm-spawn-thread` 142/142, and
`enemy-full-object-destructor` 386/386) remain exact. The cold exact report
is `.analysis/ecl-start-subroutine/cold-exact-compare.json`.

Reproduce the cold comparison with:

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
timeout 900s nice -n 10 taskset -c 2 scripts/repo-python scripts/build-match-unit.py --unit ecl-vm-start-subroutine
scripts/repo-python scripts/compare-linked-function.py --unit ecl-vm-start-subroutine --json
```

[KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) records the natural zero-`volatile`
loop-header IVR structure (`++argumentIndex, metadataOffset += 8, argumentOffset += 4`
with `static_cast<unsigned int>(metadataOffset + 4) >> 2`) that closed the
final float-word and loop-latch register differences.

## Completed work

Use the canonical ledgers and recipes rather than old scratch snapshots.
Recent source/context repairs are summarized in the knowledge base:

| Scope | Canonical units |
| --- | --- |
| ECL subroutine entry | `ecl-vm-start-subroutine` |
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
