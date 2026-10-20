# TH10 reconstruction handoff

Updated 2026-10-20. This file describes the current checkpoint. Historical
experiments remain in Git, the linked evidence packs and
[KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md); they are not a live work queue.

## Resume safely

- Target only original Japanese TH10 v1.00a at ignored `resources/th10.exe`:
  SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
- Recover tracked and untracked work before editing. The pre-existing untracked
  `docs/evidence/d3d-adapter-format/` is independent, unadopted work; preserve it.
- Use `scripts/repo-python`. Inspect `AGENTS.md`, [RE_WORKFLOW.md](RE_WORKFLOW.md)
  and [ORACLES.md](ORACLES.md) before target-dependent work.
- Current operator-authorized checkpoints use `gpt-6.1-sol: ...` and are pushed.
  This local session uses Bash and Ghidra headless. Factory transport rules
  apply when using that transport; they do not describe these local results.
- Serialize native builds at nice10 or lower priority. Ghidra review uses
  read-only/noanalysis, rollback, one CPU and 768 MiB. Verify disk identity and
  complete mapped text before/after temporary metadata work:
  401000..465A80 inclusive, 412289 bytes, SHA-256
  `cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
  The old 64A80-byte hash is a truncated prefix, not complete mapped text.

```sh
git status --short --branch
git diff --check
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --execute
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/report-reconstruction-status.py
```

## Current checkpoint

The operator has ended the 95% campaign and requested cleanup and handoff.
There is no active autonomous reconstruction campaign. Large functions remain
the preference when reconstruction resumes; select one bounded unit then.

Live totals are generated in [PROGRESS.md](PROGRESS.md): 1775 reviewed
candidates, 734 authored functions owning 273274 bytes, 1074 source mappings,
1206 canonical exact functions overall. Authored exact is **370 functions,
67505/273274 bytes (24.70%)**. The 224 mapped authored nonexact functions,
523 indeterminate origins and unresolved inventory gaps remain open; the
authored denominator is provisional.

The final source change closes RGB Evaluate441600/820. Two independent
five-TU /GL /GS canonical colds reproduce all17 target fields. Only the new
`src/AnmInt3Scale.cpp` support TU uses /Ob1; its migrated56-byte operator
retains existing credit. Twelve same-image existing guards retain4579 bytes
and183 fields. Each image therefore passes14 complete owners5455 bytes/all203
fields. Actual-source host checks pass13 scenarios,9 strict mutants and
ASan/UBSan/leaks. See [RGB proof](evidence/anm-rgb-interpolation/README.md).
Earlier Float2/Float3 and alpha exact units remain intact in these guards.
Original physical source/TU/profile identity is still unknown. All97 standard
CI steps pass after the final source freeze; current raw inputs and native
artifact hashes remain unchanged. Cleanup removes about977 MB of verified
obsolete outputs; historical source/report evidence remains.

The whole Windows i386 product graph is open. Diagnostic anchor images are
not runnable products; host tests are not original/native-product execution.
Semantic reconstruction and portability have not started.

## Open large owners

These are scoped historical diagnostics, not exact claims. Rebuild the actual
graph after any dependency changes before comparing scores.

| Owner | Last measured context | Remaining work |
| --- | --- | --- |
| ANM ExecuteScript43EE30/9964 | Current RGB five-TU carrier:9948 bytes, frameFC,89 target-sized groups, absolute gap18, aligned7400/7995; pre-table delta−16 | Nonexact. Unequal physical intervals61(+1),60(−7),2(−10; common updates/alignment included). Private registers, SCALE_TIME and common copy/flags remain open. |
| ECL Run44E1A0/7020 | Disabled-trace/shared-advance-base checkpoint:7020 bytes,6236/6264 ordinary positions;60 jump destinations and88 selector bytes retained | 28 ordinary differences, stack/register choices and incomplete selector normalization. Cache predates current shared-header inputs. [Evidence](evidence/ecl-run-disabled-trace/README.md). |
| Enemy DispatchEclInstruction40E770/14416 | Earlier eight-TU /GL /GS polar checkpoint:14524 bytes, frame2C4,181 selector bytes and108 groups | Argument adapters, polar/storage scheduling, spawn/laser lifetimes and stack homes. Historical context, not a current build. See knowledge-base polar and dispatcher entries. |

Do not repeat restored API/order/inlining controls solely to chase a score.
Gain new TH10 caller, lifetime, partition or profile evidence. Keep target
observations, cross-game hypotheses, compiler observations and unknowns distinct.

## Reproduction and retained local state

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit anm-evaluate-rgb-interpolation --unit anm-int3-scale --json
scripts/repo-python scripts/test-anm-rgb-interpolation.py
scripts/repo-python scripts/report-anm-execute-table.py --check
```

Replay only the necessary affected units; no full historical replay was run
for this checkpoint. Existing guard recipes retain their original contexts;
the RGB pack records explicit additional same-image guard overrides. The
scale recipe alone was migrated to the five-TU graph.

The retained current ANM carrier is `build/match-linked/AnmRgbInterpolation`.
Verify all12 raw project-input hashes and four artifact hashes against
`docs/evidence/anm-rgb-interpolation/native-summary.json` cold2 before reuse.
Older ANM carrier/source hashes are historical. The replaced
`AnmSetupVertexBuffer` carrier and nine discovery build directories are pruned.
Cold1 output was superseded by cold2; both complete reports and identities
remain in the private pack. Any rebuild replaces this cache and its pins.

Private review/recovery material is under
`.analysis/gpt-6.1-sol/th10-anm-rgb-value-graph-20261020/`: final reports,
source pins, target/Ghidra review, compact controls and verified source deltas.
Read its `RECOVERY.md` before using old runners: they can restore the old
starting source in `finally`; do not rerun them over accepted work.

The issue reference remains an ignored shallow sparse checkout at
`_reference/th10-decomp`, levzzz5154/th10-decomp,
commit `c6791d82dc9f02e55eca45a5314f900a85481efd` (MIT).
Adjacent TH08/TH09/TH095 and reference source are hypotheses only.

## Checkpoint completion

Focused native and host results are in the RGB proof. Run the standard gates
before the next commit; regenerate progress after ledger changes:

```sh
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py
scripts/repo-python scripts/progress.py
scripts/repo-python scripts/ci.py
git diff --check
```

Clean only inspected, attributable artifacts after retaining reproducible
evidence. Preserve unknown work, D3D evidence, private target, databases,
toolchains and reference checkout. Final validation and cleanup totals are in
[the RGB validation summary](evidence/anm-rgb-interpolation/validation-summary.json).
