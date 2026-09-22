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
| EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770 | 14,416 bytes | 14,228 bytes; pre-table 13,572/13,760; 731/11,556 normalized comparable bytes; selector 181/181; physical order matches; rank-float routing now uses the target two shared store tails | non-exact |
| AnmRenderManagerView::ExecuteScript @ 0x0043EE30 | 9,587-byte executable owner | selected source-shape diagnostic: contribution 9,960 with pre-table 9,584/9,588 and 705/8,608 agreement; exact-creator split context separately yields 10,040 with pre-table 9,664 | non-exact; context-sensitive |
| EclVmContext::Run @ 0x0044E1A0 | 7,020 bytes | 7,020/7,020; pre-table 6,692/6,692; 945/6,264 normalized agreement, normalization incomplete | non-exact |

Use the per-owner sections below for the selected context and open problems. Do
not mix measurements from different LTCG support graphs.

## Enemy dispatcher: current recovery point

The selected current diagnostic is the rank-float shared-tail checkpoint:

- .analysis/enemy-exp-rank5-shared-tails-probe.json
- .analysis/enemy-exp-rank5-shared-tails-layout.json

These are focused linked-PE diagnostics, not exactness evidence.

| Measure | Selected candidate | Target |
| --- | ---: | ---: |
| Complete contribution | 14,228 | 14,416 |
| Pre-table span | 13,572 | 13,760 |
| Stack frame allocation | 0x2C4 | 0x2C4 |
| Normalized comparable bytes | 731 / 11,556 | 11,556 / 11,556 |
| Selector bytes | 181 / 181 | 181 / 181 |
| Physical selector-group order | matches | matches |
| Suffix | 43 | 43 |
| START_SPELL physical group | 214 | 219 |

The current source-shape checkpoint deliberately supersedes c317b0f's explicit
ReadFloatArgument context local. Target 0x00412A60 is an unreferenced 14-byte
float-reader adapter, while the dispatcher calls generic EclVmContext::ReadFloat
at 0x0044FE40 directly. The one-expression maintained wrapper reproduces that
call topology: its separate candidate adapter is 14 bytes with a tail jump and
has no dispatcher calls, while the explicit-local variant introduced four false
adapter calls. The structural correction keeps the complete contribution,
pre-table span, selector, physical group order and suffix unchanged. Its raw
normalized agreement is 680/11,544 rather than c317b0f's 696/11,536 coloring
score; do not restore the false adapter calls merely to improve that heuristic.
Opcode 0x10E is now 289/287 and 0x1B3 is 167/170, versus 231/287 and 227/170
under the explicit-local variant.

The current extension keeps that direct generic ReadFloat topology and expresses
opcode 0x1A8's two result stores through the recovered EnemyBulletPatternView
array rather than a manually reconstructed byte address. That natural typed
source shape leaves the 14,292-byte contribution, 13,636-byte pre-table span,
181/181 selector, physical order and 43-byte suffix unchanged while raising
normalized agreement from 680/11,544 to 690/11,544. The 0x1A8 physical group
changes from 172 to 171 bytes against target 206; this remains a global coloring
checkpoint, not a local exact case. All five canonical EnemyEclDispatcher exact
helper units cold-replay zero-difference for 461/461 bytes after the change.
The owner remains non-exact.

The current checkpoint additionally restores the target rank-float sharing
topology. Target 0x15E does not own a third private ReadFloat/ResolveFloat/store
return path: its >= -200 and < -400 paths select operand indices 2 and 0 into
the first rank-float tail, while the -400..-200 path selects index 1 into the
second tail also reached by 0x15D and AIM_BULLET_AT_PLAYER. Routing the
maintained source through those two existing tails removes the extra candidate
tail and raises normalized agreement from 690/11,544 to 731/11,556. The
complete contribution becomes 14,228/14,416 and the pre-table span
13,572/13,760; selector bytes remain 181/181 and physical group order remains
target-equal. The resulting 0x15D, 0x15E and 0x1AE physical intervals are
44/39, 123/169 and 116/76 candidate/target bytes respectively, so this is a
control-flow recovery checkpoint rather than a local or whole-owner exactness
claim. All five canonical EnemyEclDispatcher exact helper units cold-replay
zero-difference for 461/461 bytes after the change.

The earlier spell and screen-shake ABI fixes are already retained in Git and
KNOWLEDGE_BASE. Do not re-derive the current baseline from their older
657/11,544 or 680/11,544 measurements. In particular, target
EnemySetScreenShake @ 0x0043C8B0 consumes five stack arguments and returns
with RET 0x14; the target dispatcher additionally carries draw priority 0x31
in EBX as a private LTCG value. A six-stack-argument source prototype is not
the shipped ABI and should not be restored merely because a focused experiment
changes the whole-owner score.

Canonical exact dependencies already accepted around this owner include:

- EnemyMarkPendingInterrupt @ 0x00409E50: 65 bytes;
- EnemySoundQueueView::QueueSoundSample @ 0x0043DC90: 123 bytes;
- EnemySoundQueueView::QueueSoundCue @ 0x0043DD10: 149 bytes;
- EnemyDropVectorView::FromAngleRadii @ 0x00413270: 30 bytes;
- EnemyRuntimeView::EnemyRuntimeView @ 0x0040CC70: 145 bytes;
- ANM render-layer creators CreateVmVariant0/2/1/3: four independent 73-byte
  exact units in their canonical AnmVmCreate.cpp/ExecuteScript context.

Useful supporting owners remain non-exact:

- EnemySpawn @ 0x0040CFB0 is target-sized at 577/577 in its focused diagnostic
  but is not byte-exact;
- EnemyFullObjectView::EnemyFullObjectView @ 0x0040D830 is 672/673 with
  137/653 normalized comparable bytes;
- directly exposing recovered Spawn/EndSpell/screen-effect bodies to the
  dispatcher changes its private register allocation and has not reproduced
  the shipped production caller graph.

The target 0x2C4 frame and shared spell scratch lifetime are target-derived.
Do not normalize instruction +0x1C back to an integer merely to improve size.
The remaining broad differences are dominated by private register allocation,
helper visibility/caller graph, shared-tail ownership, and stack slot coloring.
The large laser/spawn scratch objects remain uniformly eight bytes below their
target stack offsets even though their sizes/order are understood; current
evidence indicates this is a spill/lifetime problem, not missing padding or
missing dummy locals.

### Known negative experiments

These experiments were useful for diagnosis but should not be repeated or
restored without a new target-derived reason:

- replacing ordinary owner uses with normal runtime reloads is optimized back
  to the same code;
- volatile owner reloads release a register but greatly inflate the dispatcher
  and worsen physical layout;
- routing all direct integer reads through ReadIntArgument changes the physical
  case order and is not a valid replacement for the private ReadInt call shape;
- direct rank-threshold rewrites for the 0x1A6/0x1A9 family worsen shared-tail
  ownership even when the source condition looks closer to the decompiler;
- aggregate ANM position copies and a forced managed-VM pointer in opcode 0x103
  both worsened the linked layout;
- moving C labels/source anchors alone does not move the shared float-store
  tail; VC7.1 canonicalizes those equivalent CFGs;
- synthetic Enemy-local reconstructions of FireLaser/laser constructors do
  not reproduce the shipped private ABI. Target FireLaser @ 0x0041C510 has
  six target callers, five outside the dispatcher, so its ABI is a
  cross-owner/LTCG problem. Do not fake that context inside the dispatcher.

- splitting PlayerFloat3 vectorScratch into three scalar locals shrinks the
  owner to 13,952 bytes and worsens agreement; the eight-byte scratch offset
  is not fixed by changing aggregate spelling;
- six-stack-argument screen-shake prototypes and synthetic full screen-effect
  caller graphs do not reproduce the shipped five-stack-argument + EBX
  private ABI;
- changing the canonical exact CreateVmVariant0 member ABI/TU shape can make
  the Enemy owner look much closer in one diagnostic, but regresses the ANM
  production-context diagnostic. Do not trade an accepted exact helper
  context for a giant-owner score.

Useful target-local clues that remain open:

- PLAY_ANM 0x107 is three bytes short because the shipped dispatcher reaches
  the case with EDX holding the owner and performs mov edx,[edx+4] at the case
  entry; the current candidate reaches it with a different whole-function
  register state. This is not a local three-byte bug.
- AIM_BULLET_AT_PLAYER 0x1AE jumps into the float Resolve/store tail used by
  rank/difficulty float selection. Commit 8b5a2cb recovers the target-shaped
  EBX=owner lifetime in that tail, but physical ownership is still different.
- FireLaser @ 0x0041C510 uses ESI=manager, EDI=request and stack type in the
  shipped image. The base laser initializer @ 0x0041C030 can be reproduced as
  a 98-byte EDX-receiver body, but the real derived laser subobject
  constructors and full caller graph are still missing; raw offset-based fake
  constructors only inflate code and should not be committed.

Avoid padding, inline assembly used only for byte shaping, volatile-only
dependencies, fake data dependencies, and speculative class graphs whose only
purpose is to steer LTCG.

### Focused Enemy probe

    scripts/repo-python scripts/probe-ltcg-backlog.py --source src/EnemyEclDispatcher.cpp --entry 'src/EnemyEclDispatcher.cpp=EnemyRuntimeView::DispatchEclInstruction' --support 'src/EnemyEclDispatcher.cpp=src/EclVm.cpp' --support 'src/EnemyEclDispatcher.cpp=src/AnmManager.cpp' --support 'src/EnemyEclDispatcher.cpp=src/AnmVmCreate.cpp' --profile-flag=/GS --json > .analysis/enemy-probe.json

    scripts/repo-python scripts/report-ecl-dispatch-table.py --candidate build/probe-ltcg/src_EnemyEclDispatcher.cpp/source.exe --candidate-function-address <read-from-probe> --json > .analysis/enemy-layout.json

Read the linked entry address from the probe report. Do not hard-code an old
candidate address. When the worktree contains unrelated edits, use committed
HEAD snapshots for support translation units rather than silently
incorporating dirty source.

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

- enemy-readfloat-context-local-repro-{probe,layout}.json;
- anm-variant0-member-split-{probe,layout}.json;
- ecl-start-member-cached-host-{probe,layout}.json.

Do not preserve scratch merely because an old KNOWLEDGE_BASE row names it.
Accepted exactness lives in config/match-units.toml, config/matches.csv,
tracked ledgers, and successful canonical replay, not in .analysis/.
