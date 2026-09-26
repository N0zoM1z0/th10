# TH10 exact reconstruction handoff

Updated 2026-09-26. This is a current recovery snapshot, not a chronological
session log. Historical target facts belong in docs/KNOWLEDGE_BASE.md and
accepted implementation history belongs in Git.

## Authority and recovery

- Target: original Japanese TH10 v1.00a.
- Canonical executable: resources/th10.exe.
- Required SHA-256:
  2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.
- Branch: main.
- Treat the repository Git HEAD as the current source checkpoint; do not
  rely on a commit hash copied into this handoff. Check it with
  `git log -1 --oneline` before editing.
- Giant-owner diagnostics below belong to explicitly selected LTCG support
  graphs and source checkpoints. Regenerate a focused probe before using an
  old measurement as live feedback.
- Commit substantive checkpoints with the operator-specified prefix; use
  `gpt-web: ...` only when the operator has not supplied one. Push when the
  operator has authorized it for the active work.
- Decompiler output, adjacent games, ignored build products and .analysis/
  artifacts are hypothesis/evidence only. They never establish exactness by
  themselves. `build/` and `.analysis/` are disposable scratch and may be
  absent after cleanup; regenerate focused probes when fresh diagnostics are needed.

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
exact-unit counts change independently of the three giant owners. The tracked
candidate boundaries are reviewed, but some origins remain indeterminate and
unresolved `.text` gaps leave the final authored denominator open. Whole-product
Windows i386 closure is still open; semantic and port stages have not started.

## Active exact frontiers

The three large owners remain non-exact. Equal size, equal table order,
equal selector bytes, or source/semantic coverage is not exactness.

| Owner | Target | Last recorded diagnostic context (replay required) | Status |
| --- | ---: | --- | --- |
| EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770 | 14,416 bytes | 14,232 bytes and 720/11,556 comparable bytes in the selected four-source graph **before** ECL support changed at `57099cf`; refresh before comparing HEAD | non-exact |
| AnmRenderManagerView::ExecuteScript @ 0x0043EE30 | 9,587-byte executable owner | current EDI-tail checkpoint: 9,944-byte PDB contribution, 444/8,592 comparable bytes; 9,568/9,588 pre-table span; all 85 target `OR EDI,-1` restores reproduced; 92 physical selector groups retain target order | non-exact; context-sensitive |
| EclVmContext::Run @ 0x0044E1A0 | 7,020 bytes | current `57099cf` diagnostic: 7,020/7,020; pre-table 6,692/6,692; 946/6,264 comparable bytes, normalization incomplete | non-exact |

Use the per-owner sections below for the selected context and open problems. Do
not mix measurements from different LTCG support graphs.

The Bullet runtime/collision pair is canonical exact in a mixed profile; the
spawn and transform owners remain separate non-exact work. Do not reuse old
all-`/GL` Bullet measurements after the normal non-`/GL` transform partition.

## Backlog routing

The canonical exact register is `config/matches.csv` plus
`config/match-units.toml`; source presence and boundary/origin claims live in
`config/functions.csv` and their separate ledgers. Do not maintain a second
"completed helper" list here. At the 2026-09-26 preflight the ledgers contain
1,320 reviewed candidates, 402 source mappings, and 250 exact units; regenerate
these counts with `report-reconstruction-status.py` on every new session.

Select open authored, source-present work with
`scripts/repo-python scripts/report-exact-backlog.py`. These families are
currently useful routing points, not an exhaustive or size-ranked task list:

- Bullet: `AdvanceTransformProgram @ 0x00406D90` (1,454 target bytes),
  `SpawnSingleBullet @ 0x004067D0` (1,413), `SpawnBulletPattern @ 0x004073E0`
  (194), `UpdateAbsoluteDirectionChange @ 0x004078E0` (335), and
  `UpdateVerticalWrap @ 0x00407E40` (172). Their source mappings remain
  non-exact. The 885-byte `BulletUpdateRuntime` and 316-byte Player collision
  callee are already canonical exact; older diagnostic comparisons of those
  two are superseded. Read the relevant `BULLET-*` knowledge rows and ledger
  notes before changing any shared support graph.
- Enemy: `EnemyCancelBulletRecord @ 0x00408030` (199 target bytes) and the
  dispatcher below remain non-exact. The 16-byte `ReadIntArgument @ 0x00412A00`
  is origin-indeterminate and has a private-register mismatch in the selected
  diagnostic; do not label it authored merely because source is present.
- ANM: `AnmVmView::InitializeForLoadedScript @ 0x00404F30` (202) and the
  executor below remain non-exact. Adjacent small helpers have their own
  canonical status in the match register.
- ECL: `StartSubroutine @ 0x0044DF70` (550), `SpawnThread @ 0x004500D0`
  (142), `ReadInt @ 0x0044FDB0` (144), and the runner below remain non-exact.

Historical candidate lengths and failed source variants are in
`docs/KNOWLEDGE_BASE.md` and Git. Regenerate one focused, source-bound probe
before treating any old diagnostic as a live baseline. A near match is not an
exact match.

## Enemy dispatcher: current recovery point

`EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770` covers the target
14,416-byte executable owner and opcodes 0x100..0x1B4. Maintained source is
present, but exact codegen is open. In the selected four-source `/GL /GS`
graph run before the ECL operand-index refinement at commit `57099cf`, the
candidate was 14,232 bytes with 720/11,556 normalized comparable bytes. Its
pre-table span was 13,576 versus target 13,760; selector bytes were 181/181
and physical case order matched. **This is a historical graph checkpoint, not
a fresh measurement of HEAD:** `src/EclVm.cpp` is a support input and changed
at `57099cf`. Rebuild the graph before using candidate offsets or scores.

Target-backed source facts to preserve:

- Float arguments call generic `EclVmContext::ReadFloat`; the 0x00412A60
  adapter is unreferenced in the target. Rank-float selection has two shared
  ResolveFloat/store tails, including the path reached from opcode 0x1AE.
- Opcode 0x1A8 stores two speeds through the typed 0x210-byte
  `EnemyBulletPatternView` row at offsets +0x18/+0x1C. Its current source
  keeps natural case-local float operands; target and candidate still differ
  in rank-factor lifetime and whole-function stack homes.
- The target `ReadIntArgument @ 0x00412A00` adapter takes EAX=runtime and
  ECX=index; the selected candidate reverses them. This private ABI is a
  cross-case register-coloring issue, not a reason to invent a wrapper.
- Target opcode 0x1B4 has two difficulty-index selection trees and one
  generic integer-reader join per tree. Branch-local direct-call and
  `return`-to-`break` variants worsened the whole owner and were reverted.
- The target central `EnemyFireLaser @ 0x0041C510` uses ESI=manager,
  EDI=request, and one stack type argument with `RET 4`. All ten caller sites
  are source-mapped, but the selected natural source still has a private ABI
  mismatch. Constructor lifetime/register allocation remains open.
- Entry spill homes and large scratch lifetimes differ despite a matching
  0x2C4 frame in the recorded graph. Do not add dummy locals, padding,
  forced volatile reads, or false dependencies to manufacture alignment.

Accepted exact units sourced from `EnemyEclDispatcher.cpp` and the
independent 145-byte `EnemyRuntimeView` constructor must be protected by
focused canonical replay after edits. Historical case sizes and failed
variants are in `ENEMY-*` knowledge rows; they are not alternate baselines.

### Focused Enemy probe

Set `analysis_dir` to a new `.analysis/<agent>/<campaign>` directory and create
its manifest before writing these outputs, per the Factory artifact policy.

    scripts/repo-python scripts/probe-ltcg-backlog.py --source src/EnemyEclDispatcher.cpp --entry 'src/EnemyEclDispatcher.cpp=EnemyRuntimeView::DispatchEclInstruction' --support 'src/EnemyEclDispatcher.cpp=src/EclVm.cpp' --support 'src/EnemyEclDispatcher.cpp=src/AnmManager.cpp' --support 'src/EnemyEclDispatcher.cpp=src/AnmVmCreate.cpp' --profile-flag=/GS --json > "$analysis_dir/enemy-probe.json"

Read `sources[].functions[]` in `enemy-probe.json`. Find the entry whose
`address` is `0x0040E770` and set `candidate_address` to its
`candidate_address` value. Require that field to be present; an unresolved
probe cannot supply the candidate layout. Then run:

    scripts/repo-python scripts/report-ecl-dispatch-table.py --candidate build/probe-ltcg/src_EnemyEclDispatcher.cpp/source.exe --candidate-function-address "$candidate_address" --json > "$analysis_dir/enemy-layout.json"

Do not hard-code an old candidate address. When the worktree contains unrelated
edits, use committed HEAD snapshots for support translation units rather than
silently incorporating dirty source.

## ANM executor: recovery point

The maintained source covers opcodes -1..92 and remains non-exact. The current
direct-entry /GL /GS graph with RandomMath.cpp /GL support emits a 9,704-byte
PDB contribution against the 9,587-byte target executable owner, with
345/8,352 normalized comparable bytes and complete normalization. The 92
physical selector groups retain target order, but pre-table code is 9,328
versus target 9,588 bytes. The older single-source graph remains a separate
diagnostic: 9,752-byte contribution, 388/8,392 normalized comparable bytes,
and 9,376-byte pre-table code. Do not compare their scores as one link graph.

Opcode 40 now calls a maintained `AnmRandomU32InRange` helper whose two
`GetRandomU16` calls reproduce target `0x0043CB80` raw-equal (88/88 bytes) in
two independent canonical cold builds with RandomMath.cpp /GL support. The
source placement and free-function name are descriptive; original class/TU
remain unknown. In this same graph, integer-random opcode 40 is 102 versus
target 105 bytes and float-random opcode 41 is 127 versus target 130. Both
otherwise follow the target local instruction sequence through the variable
write, then omit the target's three-byte `or edi, -1` at the shared loop tail.
Opcode 41 also uses an ESP+0x10 float temporary where the target uses +0x14.
Reconcile loop EDI lifetime and stack frame from target-supported source
shape before claiming either opcode or the executor exact.

The selected pre-table spans decode completely. Target has 85 `or edi, -1`
instructions; the old baseline had two. A 2026-09-26 TH10-local source-shape
checkpoint now keeps one loop-carried interrupt sentinel, uses it for both
END/fallback `-1` comparisons, and resets it at the shared instruction-advance
tail. VC7.1 naturally tail-duplicates that assignment into exactly 85 EDI
restores while preserving all 92 physical selector groups. The pre-table span
moves from 9,328/9,588 to 9,568/9,588, leaving only a 20-byte pre-table deficit.
The linked owner is still non-exact. A follow-up layout checkpoint routes
NOP/interrupt-label through the same shared instruction-advance tail. That
recovers the target physical split around POSITION exactly: POSITION is
171/171 bytes and the following NOP/interrupt-plus-alternate-position region is
184/184, while all 85 EDI restores and all 92 physical selector groups remain.
The selected contribution is 9,928 versus target 9,587, with 391/8,576
normalized comparable bytes and a 9,552/9,588 pre-table span. The shared tail
still schedules `OR EDI,-1` before ADD/STORE in the candidate while target does
ADD/STORE first; POSITION float temporary stack homes also remain different.
Do not optimize toward total contribution size at the expense of these physical
group and instruction-order facts. The 0xFC-byte stack frame still matches
target size, while saved game speed remains ESP+0x60 versus target +0x98.

A 2026-09-26 support-graph check added `AnmVmCreate.cpp` and then
`AnmVmId.cpp` to the selected ANM/RandomMath graph. Both variants emitted a
10,040-byte executor and retained only two EDI resets, so expanding those
callee sources does not explain the sentinel lifetime. Target interrupt search
at `0x0043F646` compares the scanned opcode against DI and at `0x0043F651`
compares the fallback argument against EDI; those are actual uses of the
reset value. Treat the wider graph as a separate diagnostic from the selected
9,704-byte graph.

A fresh source-scope cold replay at the EDI-tail checkpoint reports `exact`
for all 92 canonical units sourced from `src/AnmManager.cpp`, across 17 linked
artifacts and 17,331 matched bytes. This supersedes the earlier Wine-crash-only
source replay note for regression coverage; the large executor itself remains
non-exact and is not part of those canonical exact units.

The same fresh graph naturally materializes `AnmVmTimerView::SetCurrent` as
a target-exact 57-byte private-EAX helper. Preserve that natural member path
while changing executor lifetime, block placement, or helper ownership.

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

The earlier `b3a74c1` checkpoint restored target 0x0044DF70 as
EclVmContext::StartSubroutine(caller, firstArgument) rather than a
three-argument global helper. The member spelling naturally reproduces the
target-observed RET 8 exits: the receiver is private and only the two explicit
arguments are callee-popped.

In the selected real-host /GL diagnostic:

| Measure | Candidate | Target |
| --- | ---: | ---: |
| Complete Run contribution | 7,020 | 7,020 |
| Pre-table span | 6,692 | 6,692 |
| Normalized comparable bytes | 946 / 6,264 | 6,264 / 6,264 |
| Physical opcode-group order | matches | matches |

Normalization is still incomplete, so equal contribution/pre-table/order is
not exactness. StartSubroutine itself remains 522/550 and SpawnThread remains
close but non-exact. The open classes are x87 arithmetic stack homes, format
opcode 0x1E metadata/cursor registers, and remaining private register choices
around ReadInt/stack operations. Fresh HEAD experiments show these are coupled:
a target-shaped positive null guard plus do/while around the format parser makes
all four float arithmetic case spans target-sized, but grows the owner to 7,024
bytes; changing the conversion branch toward the target shrinks the format case
while flipping the float stack homes back. Treat the format parser's lexical
lifetime as a whole-function allocator input rather than tuning the arithmetic
cases independently. ReadInt remains 144/144 with only four ordinary register-
encoding bytes open; stack aliases are codegen-neutral while mutable/inline pop
rewrites materially regress the helper and Run. All accepted EclVm exact units
must continue to replay zero-difference while iterating on Run.

At the `StartSubroutine` seam, target `Run` and `SpawnThread` move their
destination context from ESI into EAX before calling the 550-byte helper. The
selected candidate passes ESI directly to its 522-byte helper, accounting for
the two-byte `SpawnThread` size deficit. Refactoring the member into a natural
three-argument `__stdcall` free function preserved the candidate callee and
both callers byte-for-byte; it was reverted. Reopen this seam with evidence
about destination lifetime and register allocation, not another declaration
change.

A source refinement in `StartSubroutine` now indexes the integer operand
words with an unsigned byte offset divided by four. The target uses `SHR 2`
and a scaled dword load; the earlier byte-mask expression used `AND -4` in
the candidate. The selected diagnostic stays 522/550 bytes and `Run` stays
7,020/7,020, while helper comparable agreement rises from 73/534 to 77/534.
Declaring `StartSubroutine` no-inline made no codegen difference and was
reverted. All 14 canonical exact units from `src/EclVm.cpp` cold-replay
zero-difference (1,331 bytes across three artifacts). The helper is still
non-exact.

## Exact regression gates

While iterating on the three large owners, replay exact units sourced from the
changed translation units:

    scripts/repo-python scripts/replay-exact-units.py --source src/EnemyEclDispatcher.cpp

    scripts/repo-python scripts/replay-exact-units.py --source src/EclVm.cpp

    scripts/repo-python scripts/replay-exact-units.py --source src/AnmManager.cpp

Before a checkpoint:

    scripts/repo-python scripts/verify-toolchain.py --check
    scripts/repo-python scripts/validate-tracking.py
    scripts/repo-python scripts/progress.py
    scripts/repo-python scripts/ci.py
    git diff --check

## Local analysis retention

The 2026-09-26 maintenance pass classified the existing `build/` contents as
reproducible compiler/PDB/map/PE products and Python caches. It also classified
the `.analysis/gpt-web/` and `.analysis/gpt-6-sol/` campaigns as completed or
superseded session scratch: their current conclusions are in source, ledgers,
`docs/KNOWLEDGE_BASE.md`, and Git. Those generated outputs were pruned after
review. The four root-level `gpt-web-ecl-base*.cpp` and
`gpt-web-timer-dirty.hpp` files were old source snapshots; the
`factory-native-ghidra/` scratch directory was empty. No TH10 build or replay
process owned these paths at cleanup. There is no retained live candidate image
or trusted baseline in either campaign directory. Historical `.analysis/...`
paths in knowledge rows are provenance labels and may no longer exist;
regenerate a source-bound probe before using them as feedback.

Future experiments should create a campaign manifest before large outputs,
record source HEAD, target identity, compiler context, and cleanup disposition,
and retain only a minimal current failure reproducer when compact tracked
conclusions are insufficient. Do not treat ignored scratch as exactness
authority. The private target `resources/th10.exe`, pinned `.tools/` surface,
and `ghidra-project/` are operator/provider state and were deliberately retained.
