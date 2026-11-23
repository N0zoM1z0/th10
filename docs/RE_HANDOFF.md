# TH10 reconstruction — next-agent handoff

**Updated 2026-11-23.** Start with [AGENTS.md](../AGENTS.md), this brief, and the relevant source and ledger rows. The current phase is natural C/C++ reconstruction with targeted VC7.1 codegen comparison. Product closure, semantic runtime validation and portability are later phases. Counts come from the generated [PROGRESS.md](PROGRESS.md).

## Checkpoint and worktree

- Branch `main`, local commits only. Do not push.
- The original target is the private, ignored `resources/th10.exe`, Japanese TH10 v1.00a, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`. Keep it read-only and uncommitted.
- `config/match-units.toml` holds the recipes, `config/matches.csv` is the canonical exact ledger, and `config/frontiers.toml` is a diagnostic ratchet only. A local strict comparison is not a canonical cold replay.
- The superseded raw-offset `AdvanceTransformProgram` experiment only reached 0x640.

## Recent reconstruction

- `midi-process-event` is exact at 1,670 bytes with 37 bound fields
  and all three compressed switch tables. Naming the SysEx allocation
  before publishing it to the ring slot, then reading the slot back,
  restores the parser's register allocation with the two ordinary
  checkpoint `for` loops.
- `gui-message-run` is exact at 4,480 bytes with 337 bound fields,
  including all 43 switch entries and four fixed references to the tables.
  The two text cases inline the same helper in the order that restores the
  shared lookup schedule. Both player portrait handlers set up the scratch
  allocation for all 16 enemy portrait pairs. See the
  [compiler notes](KNOWLEDGE_BASE.md) for the code shapes and trace points.
- `midi-event-tick` is exact at 473 bytes with 14 bound fields and 12
  fixed local branches. The next event tick comes directly from the
  track array, and the parser takes the address of that same entry. The
  `MidiEventTick` link preserves the complete parser candidate.
- `game-reset-gui` is exact at 1,965 bytes with 52 bound fields and
  73 fixed local branches. Both life-icon loops walk the flags word at
  the VM stride. The non-positive count bypasses the upper-bound check
  and enters the hide loop at +0x1F0.
- `stage-execute-script` reproduces 1,868 bytes with 36 bound fields,
  including all 15 jump-table entries. Publishing the fog interpolation
  mode before the two record copies restores the register rotation in
  opcodes 10–13. The real stage-creation graph includes the script
  starter, which takes the VM in ESI and the script index in EAX.
- The 203-byte ANM script loader replays from the same StageCreate graph.
  It calls the real `ExecuteScript` and uses the shared game speed and
  renderer counter. Linking the real initializer still changes the ABI
  of both loader and starter. See the knowledge base for the measured
  StageCreate and additional-caller configurations.
- `gui-update-score-display` is exact at 671 bytes with 36 bound fields.
  Each difficulty branch awards the life after its own threshold check.
  VC7.1 merges the calls and restores the EAX-then-ECX loads. The shared
  `AddLives` body, Item update and GameManager update still match.
- `item-manager-update-body` is exact at 2,320 bytes with 154 bound fields
  and 73 fixed local branches. It compiles separately from its registration
  callback and real score helpers. Forming the distance scale from the
  collection line and playfield extent restores the x87 `FSUB` sequence.

- `game-manager-update-body` is exact at 1,544 bytes with 114 bound fields
  and 60 fixed local branches. Its real `GamePrepareAnimationSlots` link entry
  retains the stack call and restores the update context to EDI. A local
  pointer to the second VM ID closes the delayed reset's address calculation.
- `result-menu-update-result` is exact in the same selected cold link.
  It reproduces all 3,260 physical bytes, 174 bound fields and eight
  switch entries. The final score-name copy shares the source pointer
  with the first copy and indexes the saved name by the position in
  that array. [Compiler notes](KNOWLEDGE_BASE.md)
- `game-manager-cleanup` is exact at 1,105 bytes with 110 bound fields. The
  recipe links the real statistics finalizer and compiles `Player.cpp` with
  `/EHs`. The statistics declaration uses explicit `__stdcall` across
  compiler profiles. `game-statistics-finalize` also passes selected replay.
- `transition-shake-linear` is exact at 439 bytes with 20 bound fields. The
  wide RNG seed read and named float product restore the late x87 divide.
  The same selected cold link preserves `transition-effect-initialize`
  and `transition-shake-envelope`.

Read [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) sections "Write a plain `switch`", "Argument evaluation order", "`AnmVmStartScript` and `AnmVmInitialize` visibility (open)" and "Non-local regressions through calling conventions".

- `AdvanceTransformProgram` is now a single natural `switch`. The hand-built branch tree, `volatile` cursor, `register` hints and barriers are gone.
- 0x404F30 is now the free function `__stdcall AnmVmStartScript(AnmLoadedView *resource, AnmVmView *vm, int scriptIndex)`, following `_reference/th10-decomp`. All 28 callers were updated. The old member's `void *unused` parameter, which forwarded whatever a caller left in ECX, and the `StartEntityScript_00404f30` adapter are removed.
- `SpawnSingleBullet` became exact (1,472/1,472 bytes, selected cold replay) through those two changes plus a natural cursor tail.
- `BulletUpdateRuntime` (0x406240) had silently regressed. Since `e569a902`, the normal-COFF `BulletTransform.cpp` support call stripped `QueueSoundCue`'s private ABI. Its recipe now compiles all support under `/GL`, and its row was withdrawn from `matches.csv`.
- `AnmVmStartScript` lost its exact row because it was exact only through the fake parameter.

## Open bullet owners

| Target owner | Unit | Current result | Next step |
| --- | --- | --- | --- |
| `0x00406D90` `AdvanceTransformProgram` | `bullet-transform-program` | Full 0x644 extent; 395/409 aligned instructions; the only field mismatch is the manager load at +0x46A (ours +0x46E) | Child-spawn block +0x3E0..+0x495. `SpawnBulletPattern` gets its descriptor in ECX in this graph but in EAX in retail and in its own exact graph; find which graph difference causes that. |
| `0x00404F30` `AnmVmStartScript` | `entity-start-script` | 204/202: retail plus one `xor ecx,ecx` with `AnmVmInitialize` opaque; 200 bytes with it visible (VM in EDX) | Open LTCG question; see the knowledge base. Do not reintroduce a forwarded ECX parameter or a stand-in initializer. |
| `0x00406240` `BulletUpdateRuntime` | `bullet-runtime-update` | 884/885: retail `mov ecx, ebp` against our `push ebp` (the interpreter's `this` goes on the stack in this graph) | Same family as the `SpawnBulletPattern` question: LTCG convention choice in a partial graph. |
| `0x004078E0` `UpdateAbsoluteDirectionChange` | `bullet-update-absolute-direction` | 333/335 (+0xD7, +0xDD: EDX against EAX) | Parked; the reference `EaseStep` shape regressed to 320. |

All three LTCG convention questions depend on which callees and callers
are visible in the link. TH09's 13.10.3077 Enemy update matched after
making the real timer body visible and keeping an early list read that
advanced the local scratch cursor before C2 merged the two paths. Trace
both allocation passes for TH10's 13.10.6030 inside `link.exe` and
change one graph input at a time. TH09's separate Type29 draw carrier
needed to see `SetPolar2D`, while moving that helper changed 24 other
owners. Compare both sides of a proposed carrier split. Use
`scripts/diagnostics/triage/idiff.py` for alignment.

Never use inline assembly, naked functions, fake ABI declarations, stand-in callee bodies, copied target bytes, padding, inert locals or comparator cropping. An exact leaf does not make its parent exact.

## Open ANM script owner

`ExecuteScript` at 0x0043EE30 compares 9,962/9,964 bytes with 350
bound fields and 95 fixed local destinations. The maintained residual is
opcode 91's use of ECX for the layer. The retained `current-clock-pc`
probe uses a three-field opcode 88 request, an opcode 91 layer reference
and a snapshot of the instruction pointer, instruction time and timer
current value. It recovers the entire opcode 91 sequence with the scoped
interrupt scan and radial code intact. Its remaining differences are
+0x7C/+0x7F, where the timestamp has an empty affinity list and tied
global register costs put it in EAX. Preloading the opcode or splitting
the speed handling shifts the code. Reopen with a live range or real
caller context that predicts a different allocation while preserving the
folded timer operand. See the [compiler traces and controls](KNOWLEDGE_BASE.md).

## Strategy for the remaining authored bytes

`_reference/th10-decomp` has MSVC C++ bodies for the remaining authored
owners. Use its statement order, case structure and helper visibility as
hypotheses and verify them in the maintained TH10 graph. Stand-in callee
bodies, context callers, `#pragma code_seg` placement and assembly
fallbacks are inadmissible here.

`MidiOutputView::LoadTrackFile` matches 442 of 448 bytes in
`midi-load-track-file` with all six fields bound. The header copies
enter scheduling in retail order, with priorities `0xF000` for EDX
and `0xF400` for ECX. The higher ECX priority reverses the pair at
+0x46. The raw length also reuses the header's stack colour.
The reader that returns the advanced cursor and writes the length
by reference restores both stack homes but folds the pointer copy
at +0x103 into the length load, leaving 446 bytes. The source and
trace commands are in `.analysis/midi-event/results.txt`.
[Compiler notes](KNOWLEDGE_BASE.md)

`FrontEndUpdateMusicRoom` now has the complete 2,796-byte extent.
`Select(0)`, the browse cursor pointer and the duration-first interpolation
call recover the corresponding code sequences. The exact interpolation
helper keeps its 206 bytes. The remaining phase 1 and 2 table entries
are two bytes early. The parser and preview setup leave the initial row
block seven bytes early. An extra entry jump and alignment account for
five of those bytes. Recheck both inlined `CreateVmVariant0` sites when
changing the loop form. The parser's `do` countdown loses both expansions
and six linker fields. The countdown `for` keeps them. At
`0x107AD5FB`, VC7.1 has split the flag OR in both factories. The row
matches retail's split sequence. In the preview, scheduling inserts the
script LEA and PUSH before the lower-weight flag store. Retail emits
a memory OR before the LEA. Reopen with a source form that changes
the script argument's scheduling while preserving both factory
expansions. In the parser, the weight-56 countdown takes ESI
before the weight-36 comment cursor reaches the allocator. Check
whether a new source form makes ESI available to the cursor.
Declaration-only controls for the interpolation and exit interrupt
helpers change their private calling conventions. The interpolation
control moves both middle table entries one byte toward retail while
passing the three register arguments on the stack. Keep those helpers
visible and check register arguments alongside table offsets.
The compiler traces and full comparison commands are in
`.analysis/music-room/results.txt`.

`EclVmContext::Run` compares 7,001/7,020 bytes in the GameManager graph
with 189 bound fields and 62 fixed destinations. The Enemy-rooted host
link adds two SIB differences. The standalone link also passes the
receiver in ECX. Keep the GameManager context for the next comparison.
Twelve differences are the swapped integer add and subtract result
homes. Both words have six references. The 65-colour sort model
exchanges the two homes if the add word gains a reference. The same
exchange occurs if the FORMAT tag cursor gains a reference and its
colour moves before the subtract word.
Splitting the reference's FORMAT specifiers shortens the owner by 64
bytes and leaves the homes reversed. Source-order and direct result
writes also preserve the tie. Reopen with a lifetime change that predicts
a different initial colour order.
Five differences are in tag addresses. The final two are the closing
time guard, whose last local allocation starts at EAX while retail
uses EDX. The model, compiler traces and recipes are in
`.analysis/ecl-run/results.txt`.

`EnemyGameStateView::StartSpell` matches 2,366/2,432 bytes in maintained
source. The retained `spell-restore-aggregate-name` probe matches
2,402/2,432 with all 108 fields and eight fixed destinations. Restoring
the name just after the aggregate copy keeps the four stack arguments
and reproduces the entry, first two copies and per-shot count. The
aggregate now needs retail's relative store and a name lifetime that
leaves the original argument in its stack slot. The indexed probe
reaches that address form with reversed SIB terms and a redundant
stack store. Both maintain the seven stage destinations. See the
[probe commands](../.analysis/start-spell/results.txt).

`InitializeGameStatisticsRecord` matches 249 of 274 bytes with all eight
DIR32 fields bound in `game-statistics-initialize-record`. The allocator
trace explains why the ninth name byte takes EDX from zero. The
reference's literal `strcpy` restores both name word stores through the
same register and gives zero EDX, then spills the outer count. Reopen
with a single-cursor source form that keeps the literal copy. See the
[knowledge base](KNOWLEDGE_BASE.md) for the measured field movement.

## Commands

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/build-match-unit.py --unit UNIT      # serial; about 3 min each under nice 10
scripts/repo-python scripts/compare-linked-function.py --unit UNIT --json
scripts/repo-python scripts/diagnostics/triage/idiff.py IMAGE SYMBOL ADDRESS SIZE
scripts/repo-python scripts/replay-exact-units.py --unit UNIT --json   # promotion; the unit must not be candidate = true
scripts/repo-python scripts/progress.py                          # when ledgers change
scripts/repo-python scripts/ci.py && git diff --check
```

`entity-start-script` and `bullet-transform-program` share `build/match-linked/BulletTransformProgramNormal`, and `bullet-runtime-update` and `player-collision-check` share `build/match-linked/BulletRuntimeUpdate`. Recipes that share a directory must stay identical. Rebuild the selected recipe before trusting a shared directory. When `src/BulletManager.cpp`, `src/BulletTransform.cpp` or `src/AnmVmStartScript.cpp` changes, replay the 22 exact units that compile them (`.analysis/startscript/replay-units.txt` holds the selector list).

## Retained context

Keep `resources/th10.exe`, `.tools/` and the TH10 Wine prefix, `ghidra-project/`, `_reference/` (`th10-decomp` was fast-forwarded on 2026-11-22), canonical ledgers, `docs/evidence/`, the selected original Ghidra exports in `.analysis/`, and configured `build/match-linked/` outputs. See [RE_ARTIFACTS.md](RE_ARTIFACTS.md). `_reference/th10-decomp` is a hypothesis source for natural C++ shapes, not a build or status authority. Its LTCG context drivers and stand-in callees are not admissible here.
