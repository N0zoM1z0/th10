# TH10 reconstruction — next-agent handoff

**Updated 2026-11-22.** Start with [AGENTS.md](../AGENTS.md), this brief, and the relevant source and ledger rows. The current phase is natural C/C++ reconstruction with targeted VC7.1 codegen comparison. Product closure, semantic runtime validation and portability are later phases. Counts come from the generated [PROGRESS.md](PROGRESS.md).

## Checkpoint and worktree

- Branch `main` at `9957cd86` (`gpt-web: tune transform program register lifetimes`); `origin/main` points at the same commit. Do not push.
- Uncommitted: `src/BulletTransform.cpp` holds a raw-offset record-access experiment for `AdvanceTransformProgram` (byte offsets computed from `offsetof`, the record held as `float *`, and a separate `dispatchKind`). It still builds to 0x640 and fails the extent gate, so it is not an improvement. Revert it unless it is a deliberate base for the next attempt. `src/BulletManager.cpp` shows as modified only because the working copy has LF line endings and `.gitattributes` wants CRLF; its content is unchanged.
- The original target is the private, ignored `resources/th10.exe`, Japanese TH10 v1.00a, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`. Keep it read-only and uncommitted.
- `config/match-units.toml` holds the recipes, `config/matches.csv` is the canonical exact ledger, and `config/frontiers.toml` is a diagnostic ratchet only. A local strict comparison is not a canonical cold replay.

## Bullet runtime owners

Measured on 2026-11-22 against the current worktree:

| Target owner | Unit | Current result | Ledger status |
| --- | --- | --- | --- |
| `0x00408210` `BulletManagerView::CancelBulletsInArea` | `bullet-cancel-in-area` | Strict 585/585, 15/15 fields | **Promotion candidate.** Still `candidate = true` with no `matches.csv` row. |
| `0x004078E0` `BulletRuntimeView::UpdateAbsoluteDirectionChange` | `bullet-update-absolute-direction` | 333/335 (+0xD7, +0xDD: target EDX, candidate EAX) | Nonexact; recorded in `frontiers.toml` |
| `0x004067D0` `BulletSpawnDescriptorView::SpawnSingleBullet` | `bullet-spawn-single-exact` | Complete 1,472-byte extent (1,413 code + 59 table); 1,418/1,472 matched, 44/44 fields | Nonexact |
| `0x00406D90` `BulletRuntimeView::AdvanceTransformProgram` | `bullet-transform-program` | PDB contribution 0x640 vs manifest 0x644; not comparable | Nonexact |

Exact and accepted in `matches.csv`: `BulletManagerUpdateCallback` (40), `UpdateBullets` (249), `SpawnBulletPattern` (194), `UpdateVectorAcceleration` (305), `UpdatePolarAcceleration` (209), `UpdateAimedDirectionChange` (431), `UpdateHorizontalWrap` (148), `UpdateHoming` (306), `EnemyCancelBulletRecord` (199) and `AnmVmView::StartScript` (202). When you change their shared source, rebuild only the affected units.

### Next actions

1. **Promote `CancelBulletsInArea`.** Run `scripts/repo-python scripts/replay-exact-units.py --unit bullet-cancel-in-area --json`. If the cold replay passes, add the `matches.csv` row, drop `candidate = true`, and regenerate progress. The size thresholds written as `width <= 16`, then `width <= 32`, then the large palette are what reproduce the target's two x87 `jp` branches.
2. **`SpawnSingleBullet`.** 54 ordinary bytes remain, concentrated in effect-script register allocation and table dispatch (first difference at +0x3EE) and in the post-spawn manager-cursor/return tail. The `StartScript` context, state-2/backstep order, the three `4.0f` references and the collision x87 store order already match.
3. **`AdvanceTransformProgram`.** The extent gate has to close first. Do not lower `compare_size` or shrink the selector span. The target's first loop block is:

   ```asm
   mov edx, [ebp+45Ch]
   lea eax, [edx+edx*2]
   mov ecx, [ebp+eax*8+474h]
   lea esi, [ebp+eax*8+464h]
   ```

   Candidates so far either swap EAX/ECX between the scaled index and the opcode, or form the record pointer too late. These attempts landed at 0x640, 0x63C or 0x648 without resolving it: raw byte-offset arithmetic, direct indexing, volatile opcode loads, declaration order, register hints, the `9957cd86` lifetime tuning, a conditional record pointer, and the current uncommitted raw-offset variant. Untried directions are natural local lifetimes around the switch, the jump-table/switch structure itself, and child descriptor initialization order. The child-pattern call at +0x498 binds `?SpawnBulletPattern@BulletSpawnDescriptorView@@QAEHPAUBulletManagerView@@@Z`.

   Runtime layout: 18 records with stride 0x18 at +0x464, kind at record +0x10, allow-while-active at +0x14, active flags at +0x43C, cursor at +0x45C, and nine extension-state slots from +0x614. The 0x644 extent covers 0x5AE code bytes, five local switch-table destinations at +0x5B0..+0x5C0, and a 128-byte selector span at +0x5C4.

4. **`UpdateAbsoluteDirectionChange`.** Direct union reads, local union temporaries, volatile reads, register hints and local-order pragmas are all byte-neutral or worse. Park it until a new observation distinguishes a model.

Never use inline assembly, naked functions, fake ABI declarations, copied target bytes, padding, inert locals or comparator cropping. An exact leaf does not make its parent exact.

`AnmVmView::StartScript(int, AnmLoadedView *, void *)` in `src/AnmVmStartScript.cpp` is the maintained owner. The global `StartEntityScript_00404f30` wrapper remains as an adapter for source-order callers. Its target-private ABI takes the VM in ESI and the script index in EAX, passes the loaded resource on the stack, and returns with `RET 4`.

## Commands

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/build-match-unit.py --unit UNIT      # serial; ~3 min each under nice 10
scripts/repo-python scripts/compare-linked-function.py --unit UNIT --json
scripts/repo-python scripts/check-frontier.py NAME               # diagnostic ratchet
scripts/repo-python scripts/replay-exact-units.py --unit UNIT --json   # only for promotion
scripts/repo-python scripts/progress.py                          # when ledgers change
scripts/repo-python scripts/ci.py && git diff --check
```

`entity-start-script` and `bullet-transform-program` share `build/match-linked/BulletTransformProgramNormal`, so rebuild the selected recipe before trusting that directory's PE/MAP/PDB. Do not run the full historical cold replay for bounded work.

## Retained context

Keep `resources/th10.exe`, `.tools/` and the TH10 Wine prefix, `ghidra-project/`, `_reference/`, canonical ledgers, `docs/evidence/`, the selected original Ghidra exports in `.analysis/`, configured `build/match-linked/` outputs, and unresolved `build/gpt-web/` contexts. See [RE_ARTIFACTS.md](RE_ARTIFACTS.md). `_reference/th10-decomp` is a separate historical reference, not a build or status authority.
