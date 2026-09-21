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
- Enemy checkpoint when this handoff was refreshed:
  76848d9 gpt-web: restore Enemy spell name length semantics.
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
build/gpt-web-enemy-checkpoint-spell-name-length/. It is intentionally a
build-only diagnostic, not exactness evidence.

| Measure | Latest candidate | Target |
| --- | ---: | ---: |
| Complete contribution | 14,336 | 14,416 |
| Pre-table delta | -80 | 0 |
| Selector bytes | 181 / 181 | 181 / 181 |
| Physical selector-group order | matches | matches |
| Sum of absolute physical-block size deltas | 572 | 0 |
| Physical blocks with equal size | 38 | all |

The candidate is still non-exact. Its exact-unit replay was not completed in
that retained packet because the local Python environment could not import the
hash-pinned Capstone 5.0.6 decoder. Treat that as an unavailable regression
check, not as a pass or failure.

Recent retained Enemy checkpoints:

| Commit | Retained result |
| --- | --- |
| 59c542e | corrects projected spawn origin/value flow |
| cdcb9aa | reconstructs the item-drop helper instead of keeping an external placeholder |
| 114cc83 | restores the target-shaped item-drop polar vector fsincos helper |
| 8b5a2cb | preserves the target-observed owner lifetime in the shared float-store tail |
| 76848d9 | treats the encrypted spell-name length at instruction +0x1C as an integer byte count |

The dispatcher frame has reached the target 0x2C4 and all 181 selectors remain
mapped in target physical order. Remaining gaps are dominated by register
allocation, helper-private ABI and shared-tail ownership, not by missing
selectors. Total contribution size is not a monotonic quality metric: the
76848d9 semantic fix increased the contribution while improving block-layout
agreement.

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
- CreateVmVariant0 now has an explicit retained boundary; verify any future
  boundary change against both the executor and external callers.

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
