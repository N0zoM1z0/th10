# TH10 reconstruction handoff

Read [AGENTS.md](../AGENTS.md), then this brief and the relevant source.
[PROGRESS.md](PROGRESS.md) is generated from the ledgers;
[matches.csv](../config/matches.csv) is the exact-count authority.

## Product build, headless Wine verification, and Position Independence

The whole-program VC7.1 SP1 `/GL` + `/LTCG` build is now configured in
[config/build.toml](../config/build.toml) and driven by
[scripts/build.py](../scripts/build.py), producing both
`build/windows-i386/th10.exe` (standard link, base `0x00400000`) and
`build/windows-i386/th10-reordered.exe` (reversed `.obj` order, base
`0x00500000`) with undecorated `d3d9.dll` / `d3dx9_31.dll` imports, Win32
dialog resource [src/th10.rc](../src/th10.rc), cross-TU bridge
[src/ProductBridge.cpp](../src/ProductBridge.cpp), pure C++ strings/tables
([src/ProductStrings.cpp](../src/ProductStrings.cpp),
[src/ProductGlobals.cpp](../src/ProductGlobals.cpp)), and zero-initialized
`.bss` layout [src/ProductBssLayout.asm](../src/ProductBssLayout.asm).

Reproduce both builds and run the Position Independence verifier:

```sh
scripts/repo-python scripts/build.py --check
scripts/repo-python scripts/build.py
scripts/repo-python scripts/build.py --reverse-link
scripts/repo-python scripts/verify-pi.py
```

See [POSITION_INDEPENDENCE.md](POSITION_INDEPENDENCE.md) for the full PI
verification results, the duplicate-storage and symbol-collision bugs fixed
during integration, and the headless Wine/Xvfb comparison against
`resources/th10.exe` (all three binaries load Direct3D9/DirectInput8/DirectSound,
create the `東方風神録 〜 Mountain of Faith. ver 1.00a` window, and reach the
identical missing-`th10.dat` teardown point `GameFinalizeStatistics42B1E0 + 0x0`).
Full interactive gameplay testing remains blocked only on supplying a private
`th10.dat` archive.

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
