# TH10 exact reconstruction handoff

Updated 2026-09-22. This is a current recovery snapshot, not a chronological
session log. Historical target facts belong in docs/KNOWLEDGE_BASE.md and
accepted implementation history belongs in Git.

## Authority and recovery

- Target: original Japanese TH10 v1.00a.
- Canonical executable: resources/th10.exe.
- Required SHA-256:
  2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.
- Branch: main.
- Code checkpoint underlying the current giant-owner diagnostics:
  b3a74c1 gpt-web: restore ECL start-subroutine member shape.
- Checkpoint commit prefix: gpt-web:.
- Decompiler output, adjacent games, ignored build products and .analysis/
  artifacts are hypothesis/evidence only. They never establish exactness by
  themselves.

Before editing:

    git status --short --branch
    git diff --check
    scripts/repo-python scripts/verify-target.py
    scripts/repo-python scripts/verify-toolchain.py --execute
    scripts/repo-python scripts/validate-tracking.py --require-target
    scripts/repo-python scripts/report-reconstruction-status.py

For Factory work, also require a passing th10-ghidra preflight for the
attested target before treating new disassembly/decompilation as evidence.

Do not copy static repository-wide inventory counts from this file into a new
report. Regenerate them with the tracking scripts because source-present and
exact-unit counts change independently of the three giant owners.

## Active exact frontiers

The three large owners remain non-exact. Equal size, equal table order,
equal selector bytes, or source/semantic coverage is not exactness.

| Owner | Target | Current retained diagnostic | Status |
| --- | ---: | --- | --- |
| EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770 | 14,416 bytes | 14,228 bytes; pre-table 13,572/13,760; 746/11,556 normalized comparable bytes; selector 181/181; physical order matches; typed 0x1A8 speed fields, rank-float shared tails and target 0x119 positive-value tests retained | non-exact |
| AnmRenderManagerView::ExecuteScript @ 0x0043EE30 | 9,587-byte executable owner | selected source-shape diagnostic: contribution 9,960 with pre-table 9,584/9,588 and 705/8,608 agreement; exact-creator split context separately yields 10,040 with pre-table 9,664 | non-exact; context-sensitive |
| EclVmContext::Run @ 0x0044E1A0 | 7,020 bytes | 7,020/7,020; pre-table 6,692/6,692; 945/6,264 normalized agreement, normalization incomplete | non-exact |

Use the per-owner sections below for the selected context and open problems. Do
not mix measurements from different LTCG support graphs.

## Enemy dispatcher: current recovery point

Use only the current target-correct source checkpoint at HEAD. Historical Enemy
measurements in KNOWLEDGE_BASE and Git explain how the source got here; they are
not alternate baselines to restore.

Selected focused diagnostic:

- `.analysis/gpt-web-enemy-rankspeed-typed-fields-probe.json`
- `.analysis/gpt-web-enemy-rankspeed-typed-fields-layout.json`

These linked-PE reports are diagnostics, not exactness authority.

| Measure | Selected candidate | Target |
| --- | ---: | ---: |
| Complete contribution | 14,228 | 14,416 |
| Pre-table span | 13,572 | 13,760 |
| Stack frame allocation | 0x2C4 | 0x2C4 |
| Normalized comparable bytes | 746 / 11,556 | 11,556 / 11,556 |
| Selector bytes | 181 / 181 | 181 / 181 |
| Physical selector-group order | matches | matches |
| Suffix | 43 | 43 |
| START_SPELL physical group | 214 | 219 |

Current retained source facts:

- Dispatcher float reads use the generic `EclVmContext::ReadFloat` boundary;
  target `0x00412A60` is an unreferenced adapter. Do not restore the older
  explicit `EclVmContext *context` local merely because it scored 696/11,536;
  that variant introduced false adapter calls.
- Opcode `0x1A8` writes through the recovered `EnemyBulletPatternView` array.
  Its four float operands have natural case-local source lifetimes, and target
  stores prove typed float speed fields at pattern `+0x18` and `+0x1C`.
  Maintaining those fields as an overlay on the existing 0x210-byte words view
  makes VC7.1 form the destination in the target shape (`LEA` of scaled index
  plus runtime, then stores at `+0x2DC/+0x2E0`). Under the selected linked
  context this keeps the owner at 14,228/14,416, changes the case from 171 to
  173 bytes versus target 206, and raises normalized agreement from 735 to
  746/11,556. The target still homes the three saved float operands at
  ESP+0x10/+0x30/+0x18 while the candidate uses +0x18/+0x40/+0x38; it also
  consumes and rematerializes the rank factor where the candidate keeps it live
  on x87. This remains a non-exact checkpoint. The raw-offset spelling is
  superseded.
- Rank-float selection uses the two target-observed shared
  ReadFloat/ResolveFloat/store tails. Do not restore a private third 0x15E tail.
- Opcode `0x119` uses the target's strict-positive tests (`0.0 < value`). This
  changed the broad heuristic score from the preceding rank-tail checkpoint's
  731 to 730/11,556 at that checkpoint while fixing unordered/NaN behavior and
  branch shape. The later retained 0x1A8 source work supersedes that whole-owner
  score; the old 731 score is not a better checkpoint.

Canonical exact dependencies already protected around this owner include
`EnemyMarkPendingInterrupt` (65 bytes), `EnemySoundQueueView::QueueSoundSample`
(123), `QueueSoundCue` (149), `EnemyDropVectorView::FromAngleRadii` (30), and
`EnemyRuntimeView::EnemyRuntimeView` (145). The ANM render-layer creator family
also remains independently exact in its canonical context. Re-run the focused
exact-unit gate after changing this translation unit; giant-owner diagnostics do
not override accepted small exact units.

Current open problems, in priority order:

1. **Private integer-reader ABI.** Target `ReadIntArgument @ 0x00412A00` is a
   16-byte adapter with private `EAX=runtime`, `ECX=index`. The maintained
   callgraph has the target-observed 30 direct adapter calls and a 16-byte
   candidate adapter, but its private registers are reversed. This difference
   propagates into rank/difficulty integer cases and whole-function coloring.
2. **Shared-tail physical ownership.** `AIM_BULLET_AT_PLAYER 0x1AE` reaches the
   same float Resolve/store tail as rank/difficulty selection in the target;
   semantic routing is recovered, but VC7.1 still places the shared block under
   the wrong physical group. Moving equivalent C labels alone does not change
   this.
3. **`SET_BULLET_COUNT_BY_DIFFICULTY 0x1B4`.** Target has two local difficulty
   trees whose branches select immediate indices before one generic ReadInt join
   per tree; the second join also changes the owner register lifetime. The
   retained group is 140/151 bytes. Several locally closer 128..226-byte source
   variants worsen whole-owner agreement and are superseded.
4. **FireLaser cross-owner LTCG ABI.** Target EnemyFireLaser @ 0x0041C510
   consistently receives ESI=manager, EDI=request, plus stack type across ten
   calls in six owners. Five caller owners are outside the focused dispatcher
   graph. The first recovered support owner is now
   EnemyLaserVectorView::FromAngleMagnitude @ 0x0041F800: its maintained
   src/EnemyLaser.cpp body is canonical exact 30/30 in two independent cold
   linked-PE replays. A diagnostic reconstruction of the central laser creation
   seam makes the dispatcher 14,336/14,416 and naturally recovers ESI=manager,
   but still passes request on the stack and type in EAX, so that seam is not
   retained as exact/source truth yet. Do not fake the remaining ESI/EDI
   convention with a dispatcher-only member or synthetic dependency; continue
   reconstructing the real caller graph.
5. **Whole-function spill/stack coloring.** Large laser/spawn scratch objects are
   uniformly eight bytes below target stack offsets despite the correct 0x2C4
   frame and understood object sizes/order. At dispatcher entry the target
   homes runtime/current-instruction/opcode state at ESP+0x30/+0x1C/+0x10,
   whereas the current candidate uses +0x40/+0x14/+0x1C. In 0x1A8 the target
   later reuses the runtime's +0x30 home for a saved float operand; the
   case-local checkpoint similarly reuses the candidate runtime's +0x40 home.
   This ties the remaining rank-speed stack mismatch directly to whole-owner
   lifetime allocation. Treat it as a lifetime/spill symptom, not missing
   padding or dummy locals. Simple declaration permutations and `#pragma
   var_order` are byte-identical to the baseline and are closed directions.
   The PLAY_ANM 0x107 three-byte gap is another symptom of the same
   entry-register state problem.

Known negative/superseded experiment classes that should not be repeated without
new target evidence:

- volatile/dummy owner reloads, fake dependencies, padding, or dummy locals;
- direct exposure of recovered Spawn/EndSpell/screen-effect bodies to the
  dispatcher solely to steer LTCG;
- the 2026-09-22 EndSpell/GUI full-seam experiments, which produced only
  14,180/14,416 and at best 669/11,568 (later variants also had incomplete
  normalization), well below the retained checkpoint;
- changing `EnemyRuntimeView::owner` from the full host pointer to the base ECL
  host type solely for alias coloring; it produced byte-identical owner metrics;
- source-order-only moves of spawn cases or shared-tail labels;
- replacing all ReadInt calls with direct generic reads, or forcing a synthetic
  local/static/private wrapper ABI;
- memcpy/aggregate rewrites chosen only because a short local sequence looks
  closer; several caused unrelated cross-case folding and worse whole-owner
  diagnostics;
- changing accepted exact ANM creator ABI/TU shape to improve this giant owner.

Useful supporting owners remain non-exact: `EnemySpawn @ 0x0040CFB0` is
577/577 in its focused diagnostic but not byte-exact, and
`EnemyFullObjectView::EnemyFullObjectView @ 0x0040D830` is 672/673 with
137/653 normalized comparable bytes.

### Focused Enemy probe

    scripts/repo-python scripts/probe-ltcg-backlog.py --source src/EnemyEclDispatcher.cpp --entry 'src/EnemyEclDispatcher.cpp=EnemyRuntimeView::DispatchEclInstruction' --support 'src/EnemyEclDispatcher.cpp=src/EclVm.cpp' --support 'src/EnemyEclDispatcher.cpp=src/AnmManager.cpp' --support 'src/EnemyEclDispatcher.cpp=src/AnmVmCreate.cpp' --profile-flag=/GS --json > .analysis/enemy-probe.json

    scripts/repo-python scripts/report-ecl-dispatch-table.py --candidate build/probe-ltcg/src_EnemyEclDispatcher.cpp/source.exe --candidate-function-address <read-from-probe> --json > .analysis/enemy-layout.json

Read the linked entry address from the probe report. Do not hard-code an old
candidate address. When the worktree contains unrelated edits, use committed
HEAD snapshots for support translation units rather than silently incorporating
dirty source.

## ANM executor: recovery point

The maintained source covers opcodes -1..92 and remains non-exact. Two
diagnostic contexts are intentionally kept distinct:

- the selected source-shape checkpoint is contribution 9,960 with pre-table
  9,584 versus target 9,588 and 705/8,608 agreement;
- the exact-creator split ExecuteScript context is 10,040 bytes with pre-table
  9,664. It is useful for understanding caller/TU effects, not as a replacement
  baseline.

Both retain all 92 physical groups in target order. Do not compare their total
sizes as if they were the same link graph.

The four render-layer creators Variant0/2/1/3 are independently canonical
exact 73-byte units with the target EDI hidden-return ABI. Variant0 remains a
natural AnmLoadedView member, but its maintained definition lives in
src/AnmVmCreate.cpp; that is a maintained TU boundary, not an original
filename claim.

Open executor problems are still block placement, float temporary homes,
END/stop ownership, and child/helper private ABI effects. FindVm/VM-id value
semantics should not be regressed to raw integer APIs merely to change layout.
After a focused probe, use scripts/report-anm-execute-table.py and compare
physical groups, not just contribution size.

## Generic ECL runner: recovery point

The current b3a74c1 checkpoint restores target 0x0044DF70 as
EclVmContext::StartSubroutine(caller, firstArgument) rather than a
three-argument global helper. The member spelling naturally reproduces the
target-observed RET 8 exits: the receiver is private and only the two explicit
arguments are callee-popped.

In the selected real-host /GL diagnostic:

| Measure | Candidate | Target |
| --- | ---: | ---: |
| Complete Run contribution | 7,020 | 7,020 |
| Pre-table span | 6,692 | 6,692 |
| Normalized comparable bytes | 945 / 6,264 | 6,264 / 6,264 |
| Physical opcode-group order | matches | matches |

Normalization is still incomplete, so equal contribution/pre-table/order is
not exactness. StartSubroutine itself remains 522/550 and SpawnThread remains
close but non-exact. The open classes are x87 arithmetic stack homes, format
opcode 0x1E metadata/cursor registers, and remaining private register choices
around ReadInt/stack operations. All accepted EclVm exact units must continue
to replay zero-difference while iterating on Run.

## Exact regression gates

While iterating on the three large owners, replay exact units sourced from the
changed translation units:

    scripts/repo-python scripts/replay-exact-units.py       --source src/EnemyEclDispatcher.cpp

    scripts/repo-python scripts/replay-exact-units.py       --source src/EclVm.cpp

    scripts/repo-python scripts/replay-exact-units.py       --source src/AnmManager.cpp

Before a checkpoint:

    scripts/repo-python scripts/verify-toolchain.py --check
    scripts/repo-python scripts/validate-tracking.py
    scripts/repo-python scripts/progress.py
    scripts/repo-python scripts/ci.py
    git diff --check

Checkpoint commits use gpt-web: .... Do not push from the reconstruction
factory.

## Local analysis retention

.analysis/ is disposable scratch and is never exactness authority.
KNOWLEDGE_BASE.md may retain historical .analysis/... provenance paths even
after those local files are pruned.

Keep only:

- one current probe/layout pair per active giant owner;
- scratch needed for an unresolved experiment that will be resumed immediately;
- small provider/boundary caches that are expensive to regenerate locally.

Delete after a checkpoint:

- superseded probe/layout JSON and duplicate whole-owner scans;
- copied source snapshots, temporary support TUs, patches and disassembly dumps
  whose conclusion is already in tracked docs or Git;
- old PDB/map/PE/COFF products and historical linked-image snapshots;
- interrupted/zero-byte outputs;
- negative experiments invalidated by later source/call-graph changes.

At the current checkpoint, the useful top-level retained comparisons are:

- `enemy-exp-119-positive-comparisons-{probe,layout}.json` plus the small
  `enemy-119-positive-focused-replay.json` regression report;
- `anm-variant0-member-split-{probe,layout}.json` for the still-relevant
  exact-creator split context;
- `ecl-start-member-cached-host-{probe,layout}.json`.

Historical paths named by KNOWLEDGE_BASE are provenance labels and may be absent
after their conclusions have been compacted into tracked evidence. Do not treat
a missing historical scratch file as loss of exactness authority.

Do not preserve scratch merely because an old KNOWLEDGE_BASE row names it.
Accepted exactness lives in config/match-units.toml, config/matches.csv,
tracked ledgers, and successful canonical replay, not in .analysis/.
