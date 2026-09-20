# TH10 exact reconstruction handoff

Updated 2026-09-20. This is the current recovery snapshot, not a chronological
session log. Historical target facts belong in docs/KNOWLEDGE_BASE.md;
accepted implementation history belongs in Git.

## Authority and recovery

- Target: original Japanese TH10 v1.00a.
- Canonical executable: resources/th10.exe.
- Required SHA-256:
  2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.
- Branch: main.
- Current checkpoint when this handoff was refreshed:
  89bd4b8 gpt-web: recover Enemy pending interrupt helper.
- Current checkpoint commit prefix: gpt-web:.
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

## Repository status

The reviewed inventory currently reports:

| Measure | Current value |
| --- | ---: |
| Function candidates | 1,317 |
| Boundary reviewed | 1,317 |
| Origin reviewed | 1,317 |
| Authored / excluded / indeterminate | 709 / 518 / 90 |
| Source-present mappings | 311 |
| Canonical exact functions | 163 |
| Canonical exact authored bytes | tracked by scripts/progress.py |
| Authored source-present exact backlog | 142 |

The whole Windows i386 product build is still open. Semantic reconstruction and
portability have not started.

## Active exact frontiers

The three large owners remain non-exact. Do not infer exactness from equal
size, equal selector/table order, or semantic coverage.

| Owner | Target owner | Current focus |
| --- | ---: | --- |
| EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770 | 14,416 bytes | private operand-helper allocation, spawn topology, several laser/rank intervals, shared-tail placement |
| AnmRenderManagerView::ExecuteScript @ 0x0043EE30 | 9,587-byte executable owner | block placement, float temporary homes, child-helper/private ABI effects |
| EclVmContext::Run @ 0x0044E1A0 | 7,020 bytes | arithmetic stack homes, format-parser register allocation, remaining ReadInt register differences |

Regenerate candidate metrics before relying on old numeric handoff values. The
9/19 probe/layout files were intentionally removed during housekeeping.

## Enemy dispatcher: current recovery point

Recent retained checkpoints materially relevant to the current call graph:

| Commit | Retained result |
| --- | --- |
| 89bd4b8 | recovers the Enemy pending-interrupt helper |
| 9cf1845 | recovers the Enemy raw sound-sample queue helper |
| 82729a0 | recovers the ANM VM visibility helper used by Enemy/GUI paths |
| 6ad01a2 | preserves the target-observed CreateVmVariant0 function boundary |
| 54ff159 | restores AnmVmIdView value-type API and the target-shaped FindVm ABI chain |
| 2b02c56 | recovers Enemy main-ANM creation through the real ANM type |
| 87ea4fd | restores ANM VM-id lifecycle helpers |
| d52d8c8 | recovers Enemy laser-position helper |
| e4655a6 | aggregates Enemy world-position sum |
| 914a8fd / 49a73ab | improve Enemy circle/motion layout |
| 3204f09 / 1c5d7eb | recover bullet-pattern cancel paths |

Important target-shaped helper results already established in the maintained
source/call graph:

- AnmRenderManagerView::FindVm uses the 4-byte AnmVmIdView value API and,
  in the full LTCG context, takes manager in EDX and id on the stack with
  RET 4.
- AnmVmIdView::GetVm, ClearFlag2, Release, and the visibility helper
  follow the target VM-id lifecycle family.
- AnmLoadedView::CreateVmVariant0 is kept out of line because the target owns
  a distinct function boundary at 0x00448D00.
- the raw sound-sample queue helper and Enemy pending-interrupt helper have been
  recovered as separate target-shaped helpers.

The dispatcher frame has previously been brought to the target 0x2C4.
Remaining differences are dominated by whole-function allocation and CFG
placement, not missing switch selectors. Avoid padding, inline assembly,
volatile-only shaping or fake dependencies.

### Focused Enemy probe

    scripts/repo-python scripts/probe-ltcg-backlog.py       --source src/EnemyEclDispatcher.cpp       --entry 'src/EnemyEclDispatcher.cpp=EnemyRuntimeView::DispatchEclInstruction'       --support 'src/EnemyEclDispatcher.cpp=src/EclVm.cpp'       --support 'src/EnemyEclDispatcher.cpp=src/AnmManager.cpp'       --profile-flag=/GS --json > .analysis/enemy-probe.json

    scripts/repo-python scripts/report-ecl-dispatch-table.py       --candidate build/probe-ltcg/src_EnemyEclDispatcher.cpp/source.exe       --candidate-function-address <read-from-probe> --json       > .analysis/enemy-layout.json

Read the linked entry address from the probe report. Do not hard-code an old
candidate address.

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
