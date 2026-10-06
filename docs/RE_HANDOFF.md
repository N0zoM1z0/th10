# TH10 reconstruction handoff

Updated 2026-10-06. This file is the current resume brief, not an experiment
journal. Durable claim details belong in
[`KNOWLEDGE_BASE.md`](KNOWLEDGE_BASE.md), generated totals in
[`PROGRESS.md`](PROGRESS.md), and older experiment history in
[`RE_CAMPAIGN_NOTES_2026-10-02.md`](RE_CAMPAIGN_NOTES_2026-10-02.md).
Historical `.analysis/...` paths are provenance labels only; the referenced
scratch may be pruned after its conclusion is recorded in tracked state.

## Authority and current state

- Target only the original Japanese TH10 v1.00a executable at the ignored
  `resources/th10.exe`. Required SHA-256:
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
- The historical compiler candidate is VC7.1 SP1 build 6030. Use
  `scripts/repo-python` for repository Python commands and keep target,
  toolchain, boundary, origin, source-presence, codegen-exactness, whole-product,
  runtime, semantic, and portability claims separate.
- Current generated ledgers contain 1,732 reviewed candidates, 882 source
  mappings, 1,110 canonical exact functions / 111,609 exact code bytes,
  266,187 confirmed authored bytes, 45,896 authored exact bytes, and 142
  authored source-present non-exact functions. These numbers come from
  `docs/PROGRESS.md`; do not copy older checkpoint totals forward.
- Native Windows i386 product closure and runtime validation remain open.
  Semantic reconstruction and portability remain later phases. Exact functions
  alone do not establish a complete or runnable game.
- Operator-requested commits use `gpt-web: ...`. Do not push from this worktree.
- Work from the live worktree, including dirty changes. Do not assume an
  unexplained dirty edit belongs to another session: inspect it, finish or
  deliberately reject it, and checkpoint substantive retained progress.
- Current source checkpoint `3c70981` retains the ANM ExecuteScript shutdown
  dispatch placement improvement recorded as ANM-103. ExecuteScript remains
  non-exact and receives no partial exact credit.

## Active roadmap

1. **ECL Run — `EclVmContext::Run @ 0x0044E1A0`**
   - Source-present and non-exact.
   - Fresh current-source rebuilding retains 7,020/7,020 bytes and the exact
     6,692-byte pre-table extent. The spawn operand-index lifetime refinement
     recorded as ECLVM-058 raises normalized comparable agreement to
     5,863/6,264 (5,969 raw), keeps 58 active destinations aligned, and retains
     the FORMAT +2 / NOP -2 / TERMINATE +2 residual spans. Run remains non-exact.
   - Continue on x87/ESP home placement, the remaining FORMAT/shared-tail
     shape, and private general-register choices. Rebuild diagnostics from
     current source for each trial and replay only canonical exact units actually
     affected by the edit.

2. **ANM ExecuteScript — `AnmRenderManagerView::ExecuteScript @ 0x0043EE30`**
   - Source-present and non-exact.
   - Current retained linked contribution is 9,948/9,964 bytes with all 92
     dispatch groups in target physical order. ANM-103 moved the
     END/DELETE/STATIC group to its better source position without disturbing
     the accepted exact cohort.
   - Next investigate the loop-tail EDI reset/private register state and local
     or aggregate lifetimes. Preserve the 92-group ordering instead of
     re-sorting the switch around score changes.

3. **Enemy dispatcher — `EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770`**
   - Source-present and non-exact.
   - Rebuild the diagnostic baseline from current source before making changes.
     Do not steer from an old linked image or normalized score.
   - Prioritize ECL private calling conventions, laser construction, stack/frame
     layout, rank arithmetic, and x87 homes.

4. **Batch closure**
   - Interleave smaller non-exact Bullet/ECL functions with the large owners;
     do not turn the project into a tiny-leaf-only queue.
   - After a coherent batch, run the broader tracking/progress/CI checks and the
     required focused exact replays.

## Recovery and verification

Start by recovering the real worktree and attesting the local prerequisites:

```sh
git status --short --branch
git diff --check
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --execute
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/report-reconstruction-status.py
```

For a bounded source change, use the narrowest relevant diagnostic first.
Diagnostics such as `probe-ltcg-backlog.py` have no exactness authority. A
modified canonical exact source must be protected by the corresponding cold
`replay-exact-units.py --unit ...` or `--source ...` scope. Before committing a
coherent batch, run:

```sh
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py
scripts/repo-python scripts/progress.py
scripts/repo-python scripts/ci.py
git diff --check
```

Factory receipt acceptance is a separate boundary from local canonical replay.
Do not report a diagnostic size, similarity score, successful compile, or Git
commit as exactness.

## Scratch and cleanup policy

- `.analysis/` and `build/` are disposable working state. They are not a
  journal and must not be the sole home of a durable conclusion.
- Before deleting scratch from a dirty recovery, identify what it belongs to.
  Preserve retained source changes in Git and move durable conclusions into
  source, ledgers, scripts, or `KNOWLEDGE_BASE.md`; then remove superseded
  probes, dumps, logs, generated PE/PDB/OBJ files, and stale build trees.
- `ghidra-project/` is private analysis database state, not ordinary compiler
  output. Keep it unless deliberately rebuilding the local database.
- `.tools/`, `_reference/`, and `resources/th10.exe` are not generic cleanup
  targets. They contain the pinned tool environment, reference material, and
  private target respectively.
- Historical documentation may retain `.analysis/...` names as provenance even
  after the disposable files are removed. Such paths are not promises that the
  scratch still exists.
