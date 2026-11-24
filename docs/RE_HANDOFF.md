# TH10 reconstruction handoff

Start with [AGENTS.md](../AGENTS.md) and the selected owner's source and
ledger rows. The current phase is natural C/C++ reconstruction with the
pinned VC7.1 SP1 build 6030 compiler and linker. [PROGRESS.md](PROGRESS.md)
reports exact coverage from the ledgers. Product closure, runtime semantics
and portability remain open.

## Worktree at handoff

- Use the original Japanese TH10 v1.00a executable at the private,
  read-only `resources/th10.exe`. The SHA-256 is
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
- `config/match-units.toml` defines the compiler recipes and
  `config/matches.csv` records accepted exact owners. The residuals in
  `config/frontiers.toml` belong to the recipes named there. Results in a
  different link graph can have different residuals.

## Recent accepted owners

The byte column gives the complete compared extent, including owned tables
and alignment where present. See the [knowledge base](KNOWLEDGE_BASE.md) for
the compiler observations behind the source changes.

| Unit | Bytes | Relevant context |
| --- | ---: | --- |
| `bullet-transform-program` | 1,604 | UpdateBullets-rooted link, 38 fields, five absolute switch entries and 128 selectors. Credits 1,454 code bytes. |
| `bullet-spawn-pattern-exact` | 194 | Same link, six fields. EAX descriptor and stack manager at both real callers. |
| `enemy-start-spell` | 2,432 | `EnemyStartSpell` link, 108 fields and eight fixed local destinations. The real dispatcher passes four stack arguments. |
| `ecl-vm-run` | 7,020 | `GameManagerUpdate` link, 189 fields and 62 fixed local destinations. The complete switch data and the context-first float reader are covered by the scoped replay. |
| `anm-execute-script` | 9,964 | `GameManagerUpdate` link, 350 fields and 95 fixed local destinations. Per-case child attachment restores allocation before C2 merges the two spawn tails. |
| `result-menu-update-result` | 3,260 | `GameManagerUpdate` link, 174 fields and eight switch entries. The two score-name copies share a source pointer. |
| `game-manager-update-body` | 1,544 | `GamePrepareAnimationSlots` link entry, 114 fields and 60 fixed local branches. |
| `game-reset-gui` | 1,965 | Same link, 52 fields and 73 fixed local branches. Both life-icon loops walk the flags word at the VM stride. |
| `gui-message-run` | 4,480 | 337 fields, including the 43 switch entries and four references to the tables. The player portrait cases set up register allocation for all 16 enemy portrait pairs. |
| `midi-process-event` | 1,670 | `MidiEventTick` link, 37 fields and all three compressed switch tables. The SysEx allocation is named before publication and read back through its slot. |
| `midi-event-tick` | 473 | Same link, 14 fields and 12 fixed branches. The next tick and parser argument use the same track entry. |
| `stage-execute-script` | 1,868 | `StageExecuteScript` link, 36 fields and 15 switch entries. Fog mode is published before the two interpolation record copies. |
| `anm-loaded-start-script-wave11` | 203 | Same link. Adding the real initializer changes the ABI of both loader and starter. |
| `item-manager-update-body` | 2,320 | Registration-rooted link, 154 fields and 73 fixed local branches. |
| `gui-update-score-display` | 671 | 36 fields. Branch-local awards retain the exact `AddLives` body. |
| `game-manager-cleanup` | 1,105 | 110 fields. The recipe links the real statistics finalizer and gives `Player.cpp` its reviewed `/EHs` profile. |
| `transition-shake-linear` | 439 | 20 fields. The wide RNG seed read and named float product restore the x87 divide. |
| `bullet-spawn-single-exact` | 1,472 | UpdateBullets-rooted link, 44 fields. Credits 1,413 code bytes. |

## Open owners

### Music room

`FrontEndUpdateMusicRoom` at `0x433EF0` compares the 2,796-byte contribution
in the `MusicRoomHelpers` graph. The current source matches 1,360 of 2,372
ordinary bytes, all 101 external bindings and 60 of 96 fixed relative
branches. The phase 1 and 2 table destinations are two bytes and one byte
early, respectively.
Sharing the first row index and y-coordinate's row-pair offset removes an
extra loop-entry jump.

C2 traces attribute both parity register differences to the phase 0 row
setup. In the earlier source, the second LEA for the y offset reuses EDX
and the scratch cursor stays on EAX. The shared row-pair form emits the
shift in EDX and spends the next scratch choice on a separate LEA for the
first row index. This rotates the later remainder operands to ECX and EDX.
Preserve the row-entry layout while testing a change before that choice.

The same current source in the `GameManagerUpdate` graph restores the
parser's retail ESI comment cursor and EBX countdown. The link also moves
the `OpenFile` size pointer from the retail stack slot to EBX and changes
the browse and close blocks. Adding the real `StageLifecycle.cpp` caller
alone to `MusicRoomHelpers` preserves the baseline's bytes and field
positions. The caller responsible for the observed graph difference remains
unidentified.
Keep both inlined VM factory expansions in the comparison. The preview's
flag store currently schedules after the script PUSH, while the original
memory OR precedes them. Build `front-end-music-skip-line` to refresh the
`MusicRoomHelpers` graph when its inputs change. The local
`.analysis/music-room/results.txt` has the compare and trace commands.

### Other large or close owners

| Owner | Current comparison and next action |
| --- | --- |
| `MidiOutputView::LoadTrackFile` | `midi-load-track-file` matches 442 of 448 bytes with six bound fields. The ECX header copy's larger priority reverses two loads and the length reuses the header's stack colour. The retained cursor-returning reader recovers the stack homes in a 446-byte probe. See `.analysis/midi-event/results.txt`. |
| `InitializeGameStatisticsRecord` | `game-statistics-initialize-record` matches 249 of 274 bytes with eight bound fields. The reference's literal name copy restores the word stores and zeros EDX, then spills the outer count. Investigate the lifetime of the name destination. |

### Bullet frontiers

These measurements come from the member-signature source epoch.
Retest each candidate in its reviewed link graph.

| Unit | Residual at the baseline |
| --- | --- |
| `entity-start-script` | 204 bytes against 202 with the real initializer opaque, and 200 bytes with it visible and the VM in EDX. The natural starter is now `AnmVmStartScript(resource, vm, scriptIndex)`. |
| `bullet-runtime-update` | 884 bytes against 885. The interpreter receives its pointer on the stack. Retail uses ECX. |
| `bullet-update-absolute-direction` | Both extents are 335 bytes. Only +0xD7 and +0xDD differ, with EDX in place of EAX. The reference `EaseStep` form shrinks the candidate to 320. |

The [compiler notes](KNOWLEDGE_BASE.md) record applicable methods.
The reference checkout at `~/coding/codex_ida/th09-reconstruction/th09`
also traces globals, caller visibility, stack colours and the scratch cursor
for C2 13.10.3077. For a concrete TH10 mismatch, compare one dependency or
lifetime change at a time on C2 13.10.6030. The local `_reference/th10-decomp`
sources can suggest natural C++ forms. Compare them with the original TH10
target in its reviewed graph.

## Resume commands

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
timeout -k 10s 540s taskset -c 2 nice -n 10 scripts/repo-python scripts/build-match-unit.py --unit UNIT
scripts/repo-python scripts/compare-linked-function.py --unit UNIT --json
scripts/repo-python scripts/diagnostics/triage/idiff.py IMAGE SYMBOL ADDRESS SIZE
timeout -k 10s 540s taskset -c 2 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit EXACT_UNIT --json
scripts/repo-python scripts/ci.py && git diff --check
```

Use an available CPU in place of `2` where needed. Reuse a retained linked
graph for a warm trial after checking its inputs. Select new and affected
exact owners individually for replay. Use the manifest to recover a shared
directory's recipe. A docs-only checkpoint needs whitespace checks and
no rebuild. See [RE_ARTIFACTS.md](RE_ARTIFACTS.md) for the retained local
contexts.
