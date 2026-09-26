# TH10 exact reconstruction handoff

Updated 2026-09-26. This file is the live recovery snapshot for ongoing exact
reconstruction. It is deliberately short: chronological experiments and durable
negative results belong in `docs/KNOWLEDGE_BASE.md` and Git history.

## Authority and recovery

- Target: original Japanese TH10 v1.00a.
- Canonical executable: `resources/th10.exe`.
- Required SHA-256:
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
- Work from the live Git worktree, including dirty changes. Do not assume dirty
  edits belong to another session; inspect and either finish, record, or revert
  them deliberately.
- Treat decompiler output, adjacent games, ignored build products and
  `.analysis/` as hypothesis/evidence only. Exactness authority is the canonical
  match ledger plus its replayable units.
- Never use an old candidate address or normalized score after a support source
  changes. Regenerate the selected link graph first.
- Commit substantive progress with `gpt-web: ...`.

Before reconstruction work:

    git status --short --branch
    git diff --check
    scripts/repo-python scripts/verify-target.py
    scripts/repo-python scripts/verify-toolchain.py --execute
    scripts/repo-python scripts/validate-tracking.py --require-target
    scripts/repo-python scripts/report-reconstruction-status.py

For target disassembly/decompilation, also require a passing Factory
`th10-ghidra` check for the registered target.

## Current roadmap

Use this order unless new target evidence changes the dependency graph:

1. **ECL Run first.** Keep the 7,020/7,020 physical owner and exact surrounding
   seams while attacking x87 stack homes, format opcode 0x1E and private register
   allocation. Work on `StartSubroutine` and `ReadInt` when they explain caller
   allocation; do not reduce the task to leaf-only work.
2. **ANM ExecuteScript second.** Keep all 92 physical groups, 85 EDI restores,
   the recovered shared-tail order and current POSITION/POSITION_TIME structure.
   Continue on local lifetimes, interpolation stack homes and remaining block
   placement.
3. **Enemy dispatcher third.** Rebuild a fresh diagnostic from current HEAD before
   evaluating any change. Then address ECL reader conventions, laser construction
   and whole-function stack allocation. Old dispatcher scores are not a baseline.
4. **Close smaller Bullet/ECL backlog in parallel.** Exact leaves are useful when
   they constrain a giant owner, but do not avoid the giant owners. After a batch,
   run focused exact replays; at milestones run full replay/CI.

## Active giant-owner frontiers

All three owners below remain non-exact. Equal size, equal table order or a high
normalized byte score is not exactness.

| Owner | Current live frontier |
| --- | --- |
| `EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770` | Target owner is 14,416 bytes. **No current candidate score is retained** because ECL support changed after the last selected dispatcher graph. Rebuild before work resumes. |
| `AnmRenderManagerView::ExecuteScript @ 0x0043EE30` | ANM-066: 9,960-byte PDB contribution; 812/8,608 normalized comparable bytes; pre-table 9,584/9,588; 92/92 physical groups in target order; all 85 `OR EDI,-1` restores present. |
| `EclVmContext::Run @ 0x0044E1A0` | ECLVM-036: 7,020/7,020; pre-table 6,692/6,692; 975/6,264 normalized comparable bytes; target physical group order retained. |

The compact local summaries are under `.analysis/gpt-web/current/`. They are
convenience snapshots, not acceptance authority.

## ECL runner: current recovery point

Current retained source facts:

- `Run` remains 7,020/7,020 with a 6,692/6,692 pre-table span and target physical
  opcode-group order.
- `EclVmContext::StartSubroutine @ 0x0044DF70` is 533/550 in the selected graph.
  Delaying the local host cache until after saving/publishing `activeContext`
  naturally restores the target private receiver seam: destination arrives in
  EAX, the helper saves ESI and begins `MOV ESI,EAX`.
- Both target callers consequently emit `MOV EAX,ESI` before `StartSubroutine`.
  `EclVmHost::SpawnThread @ 0x004500D0` is now canonical exact at 142/142 and
  must be protected.
- `ReadInt @ 0x0044FDB0` remains 144/144 with four ordinary comparable bytes
  open. The target reuses ESI for the second typed-stack decrement; the current
  candidate materializes ECX instead.
- Format opcode 0x1E and the four float arithmetic cases are allocator-coupled.
  Target-shaped format guards can make all four arithmetic spans target-sized
  while worsening the total owner; do not tune those cases independently.

Closed ECL directions that should not be repeated without new evidence:

- moving `argumentIndex` before the initial stack-reservation branch creates a
  tempting 1,169/6,264 `Run` score but breaks the target helper ABI (`RET 4`,
  500-byte helper) and regresses `SpawnThread`;
- merely splitting `StartSubroutine` into another `/GL` translation unit does
  not recover the private receiver seam;
- guarded/do-while/goto rewrites of the StartSubroutine argument loop shrink the
  helper toward ~521 bytes and regress it;
- stack-object aliases and explicit/specialized `ReadInt` pop expansions cause
  major codegen regressions;
- format-parser declaration permutations and direct arithmetic-local ordering
  probes do not independently solve the whole-function allocator problem.

Focused ECL diagnostic:

    analysis_dir=.analysis/gpt-web/current-run
    mkdir -p "$analysis_dir"
    scripts/repo-python scripts/probe-ltcg-backlog.py \
      --source src/EclVm.cpp \
      --entry 'src/EclVm.cpp=EclVmHost::Run' \
      --support 'src/EclVm.cpp=src/Enemy.cpp' \
      --json > "$analysis_dir/probe.json"

Read the fresh `candidate_address` for target `0x0044E1A0`, then run:

    scripts/repo-python scripts/report-ecl-vm-table.py \
      --candidate build/probe-ltcg/src_EclVm.cpp/source.exe \
      --candidate-function-address "$candidate_address" \
      --json > "$analysis_dir/layout.json"

After any ECL edit:

    scripts/repo-python scripts/replay-exact-units.py --source src/EclVm.cpp

## ANM executor: current recovery point

ANM-066 is the retained source-shape checkpoint:

- selected direct-entry `/GL /GS` graph with `RandomMath.cpp` support;
- 9,960-byte PDB contribution versus the 9,587-byte target executable owner;
- 812/8,608 normalized comparable bytes;
- pre-table span 9,584/9,588;
- 92/92 physical selector groups in target order;
- all 85 target `OR EDI,-1` restores reproduced;
- shared advance tail emits target order `MOVZX size; ADD; STORE; OR EDI,-1`;
- POSITION is 171/171 and the following NOP/interrupt-plus-alternate region is
  184/184;
- expanding the POSITION_TIME initial-position ternary into explicit `if/else`
  is the allocator change that moved the pre-table deficit from 36 bytes to 4;
- POSITION_TIME is still 347/344 and the following COLOR1_TIME region 159/173,
  so the remaining four-byte aggregate deficit is not padding evidence.

Do not regress VM-id semantics or exact render-layer creators to raw integer
APIs just to change layout. The current open classes are POSITION/interpolation
float stack homes, remaining block placement, END/stop ownership and private
helper ABI effects.

Recent negative ANM experiments that are closed absent new evidence include:

- moving `savedGameSpeed` declaration scope alone;
- explicit POSITION `z/y/x` locals;
- introducing a noinline interpolation `ResetTimer` helper;
- aggregate final-position temporaries that expand the pre-table past target.

Focused ANM diagnostic:

    analysis_dir=.analysis/gpt-web/current-anm
    mkdir -p "$analysis_dir"
    scripts/repo-python scripts/probe-ltcg-backlog.py \
      --source src/AnmManager.cpp \
      --entry 'src/AnmManager.cpp=AnmRenderManagerView::ExecuteScript' \
      --support 'src/AnmManager.cpp=src/RandomMath.cpp' \
      --profile-flag=/GS \
      --json > "$analysis_dir/probe.json"

Read the fresh candidate address for target `0x0043EE30`, then use
`scripts/report-anm-execute-table.py` on the fresh linked image. Compare physical
case groups and tails, not only contribution size.

After any ANM edit:

    scripts/repo-python scripts/replay-exact-units.py --source src/AnmManager.cpp

## Enemy dispatcher: rebuild before judging

The target owner is `0x0040E770`, 14,416 bytes. Maintained source covers the
0x100..0x1B4 dispatcher, but the previously recorded candidate score used older
ECL support and is intentionally omitted from the live frontier.

Target-backed constraints to preserve when the dispatcher campaign resumes:

- float cases call generic `EclVmContext::ReadFloat`; the old 0x00412A60 adapter
  is not a dispatcher call seam;
- rank-float selection uses shared ResolveFloat/store tails;
- opcode 0x1A8 writes through the typed 0x210-byte bullet-pattern record;
- target `ReadIntArgument @ 0x00412A00` uses private EAX=runtime, ECX=index;
- opcode 0x1B4 contains two distinct difficulty-selection trees with one generic
  integer-reader join per tree;
- central `EnemyFireLaser @ 0x0041C510` uses ESI=manager, EDI=request and one
  stack type argument (`RET 4`);
- entry scratch and stack-home lifetime are whole-owner problems. Do not use
  dummy padding, fake volatile dependencies or byte patches.

Fresh dispatcher diagnostic:

    analysis_dir=.analysis/gpt-web/current-enemy
    mkdir -p "$analysis_dir"
    scripts/repo-python scripts/probe-ltcg-backlog.py \
      --source src/EnemyEclDispatcher.cpp \
      --entry 'src/EnemyEclDispatcher.cpp=EnemyRuntimeView::DispatchEclInstruction' \
      --support 'src/EnemyEclDispatcher.cpp=src/EclVm.cpp' \
      --support 'src/EnemyEclDispatcher.cpp=src/AnmManager.cpp' \
      --support 'src/EnemyEclDispatcher.cpp=src/AnmVmCreate.cpp' \
      --profile-flag=/GS \
      --json > "$analysis_dir/probe.json"

Use the fresh candidate address with `scripts/report-ecl-dispatch-table.py`.
Never hard-code an address from an old linked image.

## Smaller backlog routing

Use the generated backlog instead of copying a long static task list here:

    scripts/repo-python scripts/report-exact-backlog.py
    scripts/repo-python scripts/rank-exact-backlog.py --source src/EclVm.cpp
    scripts/repo-python scripts/rank-exact-backlog.py --source src/BulletManager.cpp
    scripts/repo-python scripts/rank-exact-backlog.py --source src/BulletTransform.cpp

Important current routing facts:

- ECL `SpawnThread` is exact; do not list it as backlog again.
- The Bullet runtime/collision pair already has canonical exact coverage; spawn,
  transform and several direction/wrap helpers remain open.
- Smaller exact units should be pursued when they constrain call graphs or close
  real source-present backlog, but not as a substitute for the three giant owners.

## Regression gates and checkpoint discipline

Protect exact units sourced from every translation unit you edit. At minimum:

    scripts/repo-python scripts/replay-exact-units.py --source src/EclVm.cpp
    scripts/repo-python scripts/replay-exact-units.py --source src/AnmManager.cpp
    scripts/repo-python scripts/replay-exact-units.py --source src/EnemyEclDispatcher.cpp

At a broader milestone run:

    scripts/repo-python scripts/verify-target.py
    scripts/repo-python scripts/verify-toolchain.py --execute
    scripts/repo-python scripts/validate-tracking.py --require-target
    scripts/repo-python scripts/report-reconstruction-status.py
    scripts/repo-python scripts/ci.py
    git diff --check

Commit substantive, verified progress promptly with `gpt-web: ...`.

## Local analysis retention

`.analysis/` is disposable scratch. The cleanup on 2026-09-26 reduced the
current `gpt-web` campaign from 106 experiment directories / ~27MB to compact
current-frontier summaries under:

- `.analysis/gpt-web/current/anm-execute.json`
- `.analysis/gpt-web/current/ecl-run.json`
- `.analysis/gpt-web/current/README.txt`

Those summaries record metrics and regeneration commands only. They are not
exactness authority. Reproducible PE/PDB/MAP files, source snapshots, permutation
matrices and superseded negative probes were removed after their conclusions had
been committed to Git/KNOWLEDGE_BASE.

Historical `.analysis/...` paths in `docs/KNOWLEDGE_BASE.md` are provenance
labels and may no longer exist. If an old result becomes relevant again,
regenerate it from current source rather than relying on the old candidate image.
Do not delete the canonical target, pinned toolchain or provider-owned Ghidra
state as part of scratch cleanup.
