# TH10 reconstruction — agent handoff

**Updated 2026-11-16.** Current checkpoint: `02513fe2` (`gpt-web: split ECL
Return pop address schedules`). The worktree was clean when this handoff was
prepared. This brief is the resume point; [AGENTS.md](../AGENTS.md) is the
workflow authority and [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) holds durable
reverse-engineering findings.

## First actions

- Inspect `git status --short --branch`, `git diff` and `git log -8 --oneline`.
  Preserve any work already present; do not reset or push.
- For target-dependent work, run `scripts/repo-python scripts/verify-target.py`.
  The only valid target is the original Japanese TH10 v1.00a executable.
- Exact ownership and acceptance are defined by `config/matches.csv`,
  `config/match-units.toml` and `config/functions.csv`. `PROGRESS.md` is
  generated; do not hand-edit it.
- Follow `AGENTS.md` for commit identity and format: `gpt-web: ...`, author
  `N0zoM1z0 <1705004398@qq.com>`, and the specified `Co-authored-by` trailer.
  Never push.

## Freshly measured resume state

On 2026-11-16, the `player-handle-collision-hit` 36-support-TU graph was
relinked after explicitly recompiling both `src/AnmManager.cpp` and
`src/EclVm.cpp`. The `PlayerHandleCollisionHit` owner remains exact at
627/627 bytes. This refreshed image is in
`build/match-linked/ClaudeReplayInitLoadScript/`.

| Open owner | Current measurement | Next useful action |
| --- | --- | --- |
| `EnemyRuntimeView::DispatchEclInstruction @0x40E770` | 14,416 bytes; the best recorded 12-TU `/GS` diagnostic is 1,545/11,568 comparable bytes, not an exact owner | Continue with `enemy-dispatch-ecl`; prioritize the rank bands, operand-register schedule and x87 homes |
| `AnmRenderManagerView::ExecuteScript @0x43EE30` | Full 9,964-byte extent; two residual bytes at +0x113/+0x115. Retail uses `MOV EDX,[EBP+20h]; PUSH EDX`; candidate uses ECX. Confirmed after a fresh `AnmManager.cpp` support rebuild | Isolate the opcode-91 call-site register lifetime without changing adjacent opcode bodies |
| `EclVmContext::Run @0x44E1A0` | Full 7,020-byte extent; 23 residual bytes after a fresh `EclVm.cpp` support rebuild. The count matches the recorded 23, but offsets moved, so the frontier ratchet reports `REGRESSED` | Reconcile the new offsets against the old residual set; do not use `--update` simply to accept moved mismatches |
| `GuiMessageVmView::Run @0x415E90` | 4,480-byte target; latest recorded candidate 4,463 bytes and 1,047/1,219 normalized instructions | Continue with `gui-message-run-candidate`; see the owner note in the knowledge base |

`EclVmStackView::Pop @0x450610` is a useful exact leaf guard (122/122 bytes).
The ECL `Push`, `EnterFrame` and `LeaveFrame` leaves and the read/resolve
helpers are tracked as separate exact units; preserve them when changing the
interpreter. The previous isolated `Run` probe also matched the two Return-pop
SIB schedules and the `POINT_ANGLE`/`LENGTH_SQUARED` x87 sequences; recheck
those details against the refreshed linked image before treating them as
current facts.

## Rebuild and comparison loop

1. Pick one owner from the open-owner table and inspect its source, target
   bytes, ledger row and current frontier entry.
2. For a large owner, use an existing graph that includes its TU. The most
   useful current graph for Player/Enemy/ANM/ECL work is
   `player-handle-collision-hit`, output to
   `build/match-linked/ClaudeReplayInitLoadScript/`; it contains
   `AnmLoadedStartScript.cpp` and the broad 36-support-TU context.
3. For fast triage, use
   `scripts/repo-python scripts/diagnostics/triage/quick-relink.py UNIT
   --dirty src/Changed.cpp`. **List every changed TU.** If a shared header
   changed, list every affected TU that must be recompiled. The script reuses
   all other cached objects, so omitting a dirty source can silently test an
   old object. `check-frontier.py --no-build` is meaningful only after the
   relevant graph has been refreshed.
4. Compare the complete owner with
   `scripts/repo-python scripts/compare-linked-function.py --unit UNIT --json`;
   use `scripts/repo-python scripts/check-frontier.py --no-build --show NAME`
   for an already refreshed nonexact frontier. A frontier pass means “no
   regression against its recorded residual set”, not “exact”.
5. Only after a candidate is exact, run
   `scripts/repo-python scripts/replay-exact-units.py --unit UNIT --json`.
   Replay only the new owner and specifically affected exact consumers; do not
   run the full historical cohort merely because a source or header is shared.
6. Update the canonical ledgers and generated progress only when the owner
   passes complete comparison and replay. Keep near matches in the frontier.

Use `scripts/repo-python` for repository Python. Run
`scripts/repo-python scripts/ci.py --full` when shared tooling changes, not as
a substitute for owner comparison. The default lightweight CI and
`git diff --check` are sufficient for documentation-only work.

## Useful graph and unresolved structural levers

- `player-handle-collision-hit` is the broad current graph for Player, Enemy,
  FrontEnd, Chain, ANM and ECL support. Its entry symbol is
  `AnmLoadedView::InitializeVm`; the output directory is
  `build/match-linked/ClaudeReplayInitLoadScript/`.
- Check current `link-aliases.rsp` files for dangling declarations. A missing
  definition can turn a real member call into an anchored stack-ABI call.
  Map each symbol to its retail callee before replacing the declaration.
- Laser storage/view inheritance still has a known vtable/virtual-method
  integration problem; see the laser notes in `KNOWLEDGE_BASE.md` before
  changing the type-0, type-1 or line owners.
- `AnmVmIdView` default construction and the `AnmVmInitialize` declaration
  remain cross-owner ABI/codegen levers. Do not change them without identifying
  and checking the affected exact consumers.
- Chain allocator/constructor and popup-owner issues remain documented in the
  knowledge base; their older measurements are leads, not current acceptance.

## Private inputs and storage

Preserve `resources/th10.exe`, `.tools/` (especially this game's Wine prefix),
`ghidra-project/`, `_reference/`, canonical ledgers, maintained source/scripts,
`build/match/`, `build/match-linked/`, the five retained `build/gpt-web/`
diagnostic contexts and unrelated `.analysis/d3d-*` work. See
[RE_ARTIFACTS.md](RE_ARTIFACTS.md) for the precise retained set and cleanup
policy. Historical trial outputs and scratch reports are not required to resume.
