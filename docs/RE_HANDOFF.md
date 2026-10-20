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
- Current operator-authorized checkpoints use English `gpt-6.1-sol: ...`
  messages and are pushed to `origin/main` after validation.
  The operator requests direct local tools, not Factory MCP. This session uses
  Bash and, when needed, read-only Ghidra headless.
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

The operator has reopened reconstruction for both large and small functions,
including ANM ExecuteScript, ECL Run and Enemy DispatchEclInstruction. This work is active;
none of the three is exact yet. Re-evaluate old source/API assumptions against
fresh target and compiler evidence. The first bounded changes and measurements
are recorded in [large-owner call order](evidence/large-owner-call-order/README.md).

Live totals are generated in [PROGRESS.md](PROGRESS.md): 1775 reviewed
candidates, 734 authored functions owning 273274 bytes, 1074 source mappings,
1210 canonical exact functions overall. Authored exact is **374 functions,
68052/273274 bytes (24.90%)**. The 220 mapped authored nonexact functions,
523 indeterminate origins and unresolved inventory gaps remain open; the
authored denominator is provisional.

The latest bounded checkpoint closes HintReadLine419F40/260 through the real
four-TU Hint/Main/FileSystem /GL /GS graph. Ordinary stdcall source recovers
both RET12 exits. Two full colds replay six complete owners986 bytes/all36
fields, including the existing OpenFile360/all26 guard. All18 source pins and
six owner hashes agree. The host parser suite explicitly models x87 evaluation
and still kills22 strict mutants; the original game control word/runtime remain
unknown. Packed BGRA storage reduces the nonexact constructor72->51 bytes;
float-expression source and the dependency context reduce LoadHintFile to1533
versus1498. See [line-reader proof](evidence/hint-read-line/README.md).
The current carrier is `build/match-linked/HintReadLine`; prior HintLoad pins
are historical. The constructor's origin stays indeterminate with no credit.

The preceding checkpoint closes HintSplitLine41A050/75. An explicit
colonless early return recovers target branch layout. Two full cold builds
replay the complete static owner and all4 fields, plus three existing guards
291 bytes/all2 fields. Explicit static tail JMP binding additionally requires
the whole raw-equal trim callee and maintained target source/name mapping;
the original static CALL default remains unchanged. See
[split proof](evidence/hint-split-tail/README.md). The canonical HintLoad
carrier and14 source pins in this proof supersede the prior helper carrier.
The ECL two-float value-object control grows Run7020->7024/frame108->110;
it changes the second spill to10C instead of target14 and is restored.

The preceding checkpoint closes HintTrimWhitespace41A0A0/114 and
HintLookupValue4198C0/98 through the real LoadHintFile two-TU graph. Two
independent cold builds reproduce both complete static owners raw-equal and
retain ClearHintLists79/all2 fields. Natural stdcall lookup source recovers
the target callee cleanup; whitespace source already matched. Host parser
checks pass with22 strict mutants. See [helper proof](evidence/hint-small-helpers/README.md).
Seven restored negative large-owner build directories were pruned (5,399,291
bytes); their source controls and reports remain. The three large owners
remain nonexact.

The preceding checkpoint closes RGB Evaluate441600/820. Two independent
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
| ANM ExecuteScript43EE30/9964 | Fresh five-TU cold:9964 bytes, frameFC,90 target-sized groups, absolute gap2, aligned7478/7995; pre-table delta0 | Nonexact. Scale method parameter order repairs the SCALE_TIME interval; BGR snapshots remove8 differences. Unequal intervals61(+1),2(−1). Registers, flag scheduling and common tail remain open. |
| ECL Run44E1A0/7020 | Fresh two-TU replay; noncanonical strict recipe validates all189 address fields and6992/7020 bytes | Exactly28 ordinary differences remain in stack/register choices. Explicit trailing-data declaration closes diagnostic selector coverage; no canonical exact claim. [Current evidence](evidence/large-owner-call-order/README.md). |
| Enemy DispatchEclInstruction40E770/14416 | Fresh eight-TU baseline:14524 bytes, frame2C4,181 selector bytes and108 groups | Strict-less polar branch restores the target condition/successor order; unaligned whole-owner scores hide this local change. Adapters, storage, lifetimes and stack homes remain open. |

Old rejected controls are scoped observations, not proof that an API or source
shape is correct. Require a concrete target hypothesis and compare corresponding
instructions; an unchanged unaligned score does not establish code neutrality.
Gain new TH10 caller, lifetime, partition or profile evidence. Keep target
observations, cross-game hypotheses, compiler observations and unknowns distinct.

## Reproduction and retained local state

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit anm-evaluate-rgb-interpolation --unit anm-int3-scale --json
scripts/repo-python scripts/test-anm-rgb-interpolation.py
scripts/repo-python scripts/report-anm-execute-table.py --check
```

The latest operator instruction explicitly requests only necessary affected
replays, serialized resource-conscious builds and periodic artifact cleanup.
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
