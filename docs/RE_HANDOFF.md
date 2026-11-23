# TH10 reconstruction — next-agent handoff

**Updated 2026-11-23.** Start with [AGENTS.md](../AGENTS.md), this brief, and the relevant source and ledger rows. The current phase is natural C/C++ reconstruction with targeted VC7.1 codegen comparison. Product closure, semantic runtime validation and portability are later phases. Counts come from the generated [PROGRESS.md](PROGRESS.md).

## Checkpoint and worktree

- Branch `main`, local commits only. Do not push.
- The original target is the private, ignored `resources/th10.exe`, Japanese TH10 v1.00a, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`. Keep it read-only and uncommitted.
- `config/match-units.toml` holds the recipes, `config/matches.csv` is the canonical exact ledger, and `config/frontiers.toml` is a diagnostic ratchet only. A local strict comparison is not a canonical cold replay.
- The superseded raw-offset `AdvanceTransformProgram` experiment only reached 0x640.

## Recent reconstruction

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

## Open ANM and GUI owners

`ExecuteScript` at 0x0043EE30 currently compares 9,962/9,964 bytes in the
`player-handle-collision-hit` graph at `16572e6b`, including all 350 fields
and 95 fixed local destinations. Opcode 91's layer load uses ECX before
C2's local register pass. Later instruction scheduling preserves that
choice. See the ExecuteScript entry in the [knowledge base](KNOWLEDGE_BASE.md)
for the measured controls and compiler entry points. Reopen this owner
with a source or real-callee change that affects the layer's global live
range while preserving the late resource load and both shared tails.

`GameResetGui` at 0x00413BC0 compares 1,964/1,965 bytes with all 52 fields
bound. At +0x1D1 the target skips the hide loop's upper-bound test for a
non-positive count. The early-return candidate recovers this jump. In that
candidate C2 keeps the original count, loop countdown, and first hidden
index in three registers before block-local allocation. Moving the setter
to another TU gives the same 1,955/1,965-byte result while its standalone
77 bytes still match. Reopen with a control-flow change that lets the
upper-bound test and the hide loop share the first hidden index. The local
trace and scoped comparison commands are in `.analysis/gui-guard/results.txt`.

## Strategy for the remaining authored bytes

`GameUpdateGui` at 0x00417040 still has the full 671-byte extent. The
`AddLives` member call loads ECX before EAX at +0x286. Restore the target's
EAX-then-ECX order while preserving the member's 141-byte body and the
exact item caller. The former free-function interface gives the original
GUI order and reverses the loads in the item caller.

`_reference/th10-decomp` has MSVC C++ bodies for the remaining authored
owners. Use its statement order, case structure and helper visibility as
hypotheses and verify them in the maintained TH10 graph. Stand-in callee
bodies, context callers, `#pragma code_seg` placement and assembly
fallbacks are inadmissible here. `EclVmContext::Run` remains a close large
owner with 23 differing bytes.

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
