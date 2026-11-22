# TH10 reconstruction — next-agent handoff

**Updated 2026-11-22.** Start with [AGENTS.md](../AGENTS.md), this brief, and the relevant source and ledger rows. The current phase is natural C/C++ reconstruction with targeted VC7.1 codegen comparison. Product closure, semantic runtime validation and portability are later phases. Counts come from the generated [PROGRESS.md](PROGRESS.md).

## Checkpoint and worktree

- Branch `main`, local commits only. Do not push.
- The original target is the private, ignored `resources/th10.exe`, Japanese TH10 v1.00a, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`. Keep it read-only and uncommitted.
- `config/match-units.toml` holds the recipes, `config/matches.csv` is the canonical exact ledger, and `config/frontiers.toml` is a diagnostic ratchet only. A local strict comparison is not a canonical cold replay.
- The worktree is clean. The superseded raw-offset `AdvanceTransformProgram` experiment from the previous session was deleted; it only reached 0x640.

## What changed on 2026-11-22

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

## Strategy for the remaining authored bytes

`_reference/th10-decomp` has MSVC C++ for all 109 authored owners that are
not exact here (128,407 bytes), excluding its GCC/Clang profiles. Audit each
one before borrowing: copy natural statement order, case structure and
helper visibility. Do not borrow its stand-in callee bodies, context callers,
`#pragma code_seg` placement or assembly fallbacks. Large near owners to try
first: `ExecuteScript` (2 bytes; keep its two merged child tails),
`EclVmContext::Run` (23 bytes), `GameResetGui` (10 bytes in the inlined life-icon loop) and
`TransitionShakeLinear` (one FIDIV placement). The reference shapes already
tried for the shake are recorded in the knowledge base.

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
