# TH10 exact reconstruction handoff

Updated 2026-09-21. This is a current recovery snapshot, not a chronological
session log. Historical target facts belong in docs/KNOWLEDGE_BASE.md and
accepted implementation history belongs in Git.

## Authority and recovery

- Target: original Japanese TH10 v1.00a.
- Canonical executable: resources/th10.exe.
- Required SHA-256:
  2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.
- Branch: main.
- Enemy base checkpoint used for the current recovery:
  c09a251 gpt-web: refresh Enemy handoff and cleanup.
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

The three large owners remain non-exact. Equal size, equal table order or
semantic coverage is not exactness.

| Owner | Target owner | Current focus |
| --- | ---: | --- |
| EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770 | 14,416 bytes | whole-function register allocation, private operand-helper allocation, laser/rank shared tails |
| AnmRenderManagerView::ExecuteScript @ 0x0043EE30 | 9,587-byte executable owner | block placement, float temporary homes, child-helper/private ABI effects |
| EclVmContext::Run @ 0x0044E1A0 | 7,020 bytes | arithmetic stack homes, format-parser registers, remaining ReadInt register differences |

## Enemy dispatcher: current recovery point

The latest retained diagnostic is
.analysis/enemy-readfloat-context-local-repro-probe.json. It is intentionally a
focused linked-PE diagnostic, not exactness evidence.

| Measure | Latest candidate | Target |
| --- | ---: | ---: |
| Complete contribution | 14,292 | 14,416 |
| Pre-table span | 13,636 | 13,760 |
| Stack frame allocation | 0x2C4 | 0x2C4 |
| Normalized comparable bytes | 696 / 11,536 | 11,536 / 11,536 |
| Selector bytes | 181 / 181 | 181 / 181 |
| Physical selector-group order | matches | matches |
| START_SPELL physical group | 214 | 219 |

The candidate is still non-exact. The current checkpoint adds a second
target-proven callee-clean ABI repair. EnemySetScreenShake at 0x0043C8B0 ends
with RET 0x14, and the dispatcher call at 0x00410DAD performs no caller
cleanup. The maintained declaration had been caller-clean, which emitted an
extra add esp,0x14. Declaring it __stdcall removes that false cleanup and
raises normalized whole-owner agreement from 657/11,544 to 680/11,544. The
candidate contribution becomes 14,292 and its pre-table span 13,636; the
181-byte selector and physical case order remain target-equal. The preceding
EnemyBeginSpell __stdcall correction remains retained as commit 1840f6d.

The preceding spell-scratch checkpoint remains important: attested TH10 Ghidra
decompilation of opcodes 0x156/0x15C/0x165-0x167 shows instruction +0x1C
flowing through the shared float scratch and restores the target 0x2C4 frame.
Do not re-normalize that field to an unsigned integer merely to improve total
size.

Recent retained Enemy checkpoints:

| Commit | Retained result |
| --- | --- |
| 59c542e | corrects projected spawn origin/value flow |
| cdcb9aa | reconstructs the item-drop helper instead of keeping an external placeholder |
| 114cc83 | restores the target-shaped item-drop polar vector fsincos helper |
| 8b5a2cb | preserves the target-observed owner lifetime in the shared float-store tail |
| 271b254 | restores target spell scratch lifetime and the 0x2C4 dispatcher frame |
| 1840f6d | restores the target callee-clean EnemyBeginSpell ABI; 608 -> 657 normalized matching bytes |
| 02486a7 | restores the target callee-clean EnemySetScreenShake ABI; 657 -> 680 normalized matching bytes |
| ca8b589 | promotes EnemyMarkPendingInterrupt (65 bytes) and EnemySoundQueueView::QueueSoundSample (123 bytes) to canonical exact linked-PE units; the dispatcher owner remains non-exact |
| 92646bd | promotes EnemySoundQueueView::QueueSoundCue (149 bytes) to canonical exact linked-PE; the dispatcher owner remains non-exact |
| e73bde8 | promotes EnemyDropVectorView::FromAngleRadii (30 bytes) to canonical raw-equal exact; the dispatcher owner remains non-exact |
| 9007f2d | restores target-observed EnemySpawn timer/flag/item-drop/switch source shape; focused Spawn becomes 577/577 but remains non-exact |
| 5691f40 | promotes EnemyRuntimeView::EnemyRuntimeView (145 bytes) to canonical exact; the full Enemy constructor improves to 672/673 but remains non-exact |
| c317b0f | refines Enemy ReadFloatArgument lifetime; dispatcher stays 14,292/14,416 while normalized agreement improves 680 -> 696; selector/order remain exact |

Two spell-path dependencies are now independently exact even though the large dispatcher is not. EnemyMarkPendingInterrupt @ 0x00409E50 replays all 65 bytes plus two linkage fields zero-difference, and EnemySoundQueueView::QueueSoundSample @ 0x0043DC90 replays all 123 bytes plus its sound-metadata field zero-difference. Both passed two independent cold canonical replays in the dispatcher /GL /GS entry context. These promotions do not change the retained 14,292 / 14,416 whole-owner diagnostic, 680 / 11,544 normalized agreement, 181/181 selector, or physical case order, and they do not justify an exact claim for DispatchEclInstruction.

The adjacent position-aware sound queue entry is exact as well: EnemySoundQueueView::QueueSoundCue @ 0x0043DD10 replays all 149 bytes plus three linkage fields zero-difference in two independent cold dispatcher-context builds. The EnemyEclDispatcher source now has four canonical exact units totaling 431 bytes. This still does not alter the retained 14,292 / 14,416 dispatcher diagnostic or justify an exact claim for the large owner.

The item-drop polar helper is now exact too. EnemyDropVectorView::FromAngleRadii @ 0x00413270 is raw byte-identical over its complete 30-byte relocation-free contribution and passed two independent cold dispatcher-context replays. The EnemyEclDispatcher source now has five canonical exact units totaling 461 bytes. The shared 14KB dispatcher owner remains non-exact and its retained whole-owner metrics are unchanged.

EnemySpawn @ 0x0040CFB0 now preserves four target-observed source-order details: timer initialization order, delayed request flag 0x40000 application, one-store item-drop remapping, and the explicit 0/20/49 death-effect switch arm. In a focused /GL /GS Spawn diagnostic this moves the candidate from 564 to 577 bytes against target 577 and raises normalized agreement from 49/533 to 86/529. Equal size is not exactness. Linking that improved Spawn body directly into the dispatcher currently worsens the large owner, so DispatchEclInstruction keeps its existing external spawn seam while the constructor/runtime-initializer dependency chain is recovered.



The Enemy runtime-tail default constructor is now independently exact. The
target EnemyRuntimeView::EnemyRuntimeView @ 0x0040CC70 is a 145-byte
EDX-receiver body generated from the maintained interpolation/bullet-pattern
member model, and the selected EnemySpawn /GL /GS context reproduces all 145
bytes zero-difference. This does not make the full object constructor exact:
EnemyFullObjectView::EnemyFullObjectView @ 0x0040D830 is currently 672/673
with 137/653 normalized comparable bytes, versus the prior 622/673 and 92/653.
The 14KB dispatcher recovery point remains independently non-exact.



A retained Enemy-dispatcher register-allocation checkpoint now makes
ReadFloatArgument bind owner->activeEclContext to an explicit
EclVmContext *context local before calling the generic reader. With the
committed Generic ECL support image, the focused /GL /GS dispatcher remains
14,292 / 14,416 bytes while normalized agreement improves from 680 / 11,544 to
696 / 11,536. The 181-byte selector, physical case order, 13,636-byte
candidate pre-table span (target 13,760), and 43-byte suffix remain unchanged.
This is still non-exact; the change redistributes several physical case
intervals and therefore should be treated as a whole-function coloring
checkpoint, not an exact-case promotion.



Generic ECL has a new retained owner-shape checkpoint. Target 0x0044DF70 is
now maintained as EclVmContext::StartSubroutine(caller, firstArgument), not as
a three-argument global helper. This ordinary member spelling naturally gives
the target-observed RET 8 on both exits: the receiver is private and only the
two explicit arguments are callee-popped. In the selected Run /GL diagnostic,
EclVmContext::Run stays 7,020 / 7,020 with an exact 6,692 / 6,692 pre-table
span and all 59 physical opcode groups in target order, while normalized
agreement improves from 795 / 6,264 to 945 / 6,264. SpawnThread improves to
140 / 142 and 122 / 130. StartSubroutine itself remains 522 / 550 and Run
normalization is incomplete, so these remain non-exact. All fourteen existing
canonical EclVm exact units replay zero-difference, and the Enemy dispatcher
remains at 14,292 / 14,416 with 696 / 11,536 when linked against this support.

Remaining gaps are dominated by whole-function register allocation,
helper-private ABI, /GS local placement and shared-tail ownership, not missing
selectors. In particular the candidate still keeps owner/instruction values in
EDI/ESI across the top-level switch where the target spills/reloads them, and
its large laser/spawn scratch objects sit eight bytes below the target offsets.

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

    scripts/repo-python scripts/probe-ltcg-backlog.py       --source src/EnemyEclDispatcher.cpp       --entry 'src/EnemyEclDispatcher.cpp=EnemyRuntimeView::DispatchEclInstruction'       --support 'src/EnemyEclDispatcher.cpp=src/EclVm.cpp'       --support 'src/EnemyEclDispatcher.cpp=src/AnmManager.cpp'       --profile-flag=/GS --json > .analysis/enemy-probe.json

    scripts/repo-python scripts/report-ecl-dispatch-table.py       --candidate build/probe-ltcg/src_EnemyEclDispatcher.cpp/source.exe       --candidate-function-address <read-from-probe> --json       > .analysis/enemy-layout.json

Read the linked entry address from the probe report. Do not hard-code an old
candidate address. When the worktree contains unrelated edits, use committed
HEAD snapshots for support translation units rather than silently incorporating
dirty source.

## ANM executor: recovery point

The executor source covers opcodes -1..92. Useful unresolved themes:

- END/stop physical placement still matters more than total contribution size;
- float-jump operand temporary homes remain sensitive to declaration/lifetime
  choices;
- child creation and VM helper boundaries can change the whole LTCG layout;
- FindVm/VM-id value semantics are now substantially better than the 9/19
  handoff and should not be regressed back to raw int APIs;
- the four render-layer creators Variant0/2/1/3 are now canonical exact
  73-byte units with the target EDI hidden-return ABI. Variant0 remains a
  natural AnmLoadedView member but its maintained definition lives in the
  separate src/AnmVmCreate.cpp TU; this is a maintained TU boundary, not an
  original-filename claim;
- do not infer executor exactness from those four helpers. In the
  Variant0-member-split focused ExecuteScript link, the executor is still
  10,040 bytes with 9,664 pre-table bytes versus target 9,588, although all
  92 physical groups remain in target order. The older 9,960/9,584 diagnostic
  remains useful evidence of link-context sensitivity rather than an exact
  baseline.

After an ANM probe, use scripts/report-anm-execute-table.py and compare
physical groups, not just total size.

## Generic ECL runner: recovery point

The runner has historically matched target total size/pre-table/order while
remaining byte-nonexact. The useful unresolved classes remain:

- x87 arithmetic stack-slot assignment;
- format opcode 0x1E metadata/cursor registers;
- the last register-choice differences in ReadInt.

Do not reorder physical cases merely to improve raw byte agreement.

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

.analysis/ is disposable scratch. Keep only artifacts needed for an active,
unresolved comparison. At each checkpoint remove:

- superseded probe/layout JSON;
- copied source snapshots and disassembly dumps already summarized in tracked
  docs or Git history;
- old PDB/map/PE/COFF products;
- interrupted/zero-byte outputs;
- negative experiments invalidated by later call-graph changes.

Accepted exactness lives in tracked match units and ledgers, not in
.analysis/. Historical .analysis/... paths in KNOWLEDGE_BASE.md are
provenance labels and may no longer exist locally.
