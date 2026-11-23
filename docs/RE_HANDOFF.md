# TH10 reconstruction — next-agent handoff

**Updated 2026-11-23.** Start with [AGENTS.md](../AGENTS.md), this brief, and the relevant source and ledger rows. The current phase is natural C/C++ reconstruction with targeted VC7.1 codegen comparison. Product closure, semantic runtime validation and portability are later phases. Counts come from the generated [PROGRESS.md](PROGRESS.md).

## Checkpoint and worktree

- Branch `main`, local commits only. Do not push.
- The original target is the private, ignored `resources/th10.exe`, Japanese TH10 v1.00a, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`. Keep it read-only and uncommitted.
- `config/match-units.toml` holds the recipes, `config/matches.csv` is the canonical exact ledger, and `config/frontiers.toml` is a diagnostic ratchet only. A local strict comparison is not a canonical cold replay.
- The superseded raw-offset `AdvanceTransformProgram` experiment only reached 0x640.

## Recent reconstruction

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

All three LTCG convention questions share one pattern: a register choice that depends on which callees and callers are visible in the link. th09 broke similar cases by tracing C2 inside the compiler; under `/GL` the backend runs inside `link.exe`. Use `scripts/diagnostics/triage/idiff.py` for fast alignment, and change one graph input at a time.

Never use inline assembly, naked functions, fake ABI declarations, stand-in callee bodies, copied target bytes, padding, inert locals or comparator cropping. An exact leaf does not make its parent exact.

## Open ANM script owner

`ExecuteScript` at 0x0043EE30 compares 9,962/9,964 bytes in the current
GameManager graph and the earlier player-collision graph. All 350 fields
and 95 fixed local destinations bind. C2 considers both ECX and EDX for
opcode 91's layer at global allocation and selects ECX. Separating the
integer accessor or the interpreter into its own `/GL` source leaves
that choice in place. Reopen when a source or real-callee change can
alter the layer's allocation while preserving the late resource load
and both shared tails. See the [knowledge base](KNOWLEDGE_BASE.md) for
the measured compiler entry points.

## Strategy for the remaining authored bytes

`_reference/th10-decomp` has MSVC C++ bodies for the remaining authored
owners. Use its statement order, case structure and helper visibility as
hypotheses and verify them in the maintained TH10 graph. Stand-in callee
bodies, context callers, `#pragma code_seg` placement and assembly
fallbacks are inadmissible here.

`FrontEndUpdateMusicRoom` now has the complete 2,796-byte extent.
`Select(0)`, the browse cursor pointer and the duration-first interpolation
call recover the corresponding code sequences. The exact interpolation
helper keeps its 206 bytes. The remaining phase 1 and 2 table entries
are two bytes early, and the prologue and parser still differ.

`EclVmContext::Run` compares 6,997/7,020 bytes in the current GameManager
graph with 189 bound fields. Twelve residual bytes come from the
equal-weight integer add and subtract result words taking reversed stack
slots. The 6030 sort model reproduces the order of all 65 stack colours
from the trace. Raising the add result's weight from six to seven in the
model exchanges just those two colours. Changing case order moved local
destinations, and the scoped declaration and scalar-form trials preserved
all 23 differences. Reopen with an operand-lifetime change that can affect
the weights before sorting. The model and comparison recipes are in
`.analysis/ecl-run/results.txt`.

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
