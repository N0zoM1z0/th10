# Reconstruction workflow

## Current scope

As of 2026-10-28, work directly with local tools on exact reconstruction.
[AGENTS.md](../AGENTS.md) is the operational authority. No Factory provider,
receipt, coordinator or Factory-document prerequisite applies. Use
`gpt-6.1-sol:` local commits; never push.

The objective is at least 98% of reviewed authored owned bytes exact. Advance
complete medium/large representatives and useful leaves. Natural low-level
source and provisional names are acceptable; compare a complete owner early.

## Bounded exact loop

1. Recover status/diff and read the current handoff plus relevant source and
   ledger rows. Use `scripts/repo-python`; verify the target before target work.
   Run toolchain execution smoke only on setup/change or a tool problem. Attest
   local Ghidra when using it, not for unrelated maintenance.
2. Select one owner with a usable complete comparison and a concrete unresolved
   source/codegen question. Backlog/ranking/boundary reports are optional triage
   tools. A whole subsystem's source-completeness report is not an entry gate.
3. Review target entry, exits, tables, alignment, ownership, ABI and actual
   callees. Keep authorship and source presence separate from exactness.
4. Recover a complete natural implementation and compile it early with the
   pinned compiler. Keep compiler/linker jobs serial, one CPU at nice10, bounded
   and logged. For normal COFF, compare its entire extent and relocations. For
   LTCG, retain `/GL` and use PE/MAP/PDB-bound linked extents and every resolved
   field. Archive units use the separately pinned archive-member lane.
5. Investigate one evidence-backed model at a time. Compare all bytes, field
   bindings, local destinations and required callees under [Oracle policy](ORACLES.md).
   A structural rejection or near match grants no partial exact credit. Park
   repeated byte-neutral spelling controls until a new observation distinguishes
   a model. Do not manufacture ABI, returns, padding or copied target bytes.
6. Accept a canonical match only after complete zero-difference comparison and
   cold reproducibility. Cold-replay newly accepted and modified exact owners
   with explicit `--unit` selectors; review affected nonexact bodies separately.
   Do not rebuild the historical cohort or every consumer of a shared header.
7. Update affected ledgers, durable knowledge and handoff. Regenerate progress
   if its inputs changed. Run the lightweight CI below, inspect the diff and
   commit a coherent local checkpoint. Report new complete-owner exact bytes
   separately from maintained source and nonexact diagnostic progress.

## Validation budget

Ordinary exact work uses the native complete-byte Oracle. Do not add host
allocation-failure/null-pointer scenarios, injected exceptions, lifecycle
simulations, sanitizer/leak sweeps or mutation campaigns to its acceptance
requirements. Preserve actual target error/cleanup branches in the source and
byte comparison. Use a small directed probe only if a specific exact mismatch
needs it, then return to the complete comparison.

`scripts/repo-python scripts/ci.py` runs seven lightweight repository checks:
Python syntax, doc references, ledgers, toolchain declarations, canonical match
graph, generated progress and whitespace. It runs no host C++ fixtures or
compiler/linker jobs. If comparison infrastructure changes, explicitly select
its relevant self-test, for example:

```sh
scripts/repo-python scripts/ci.py --only 'Validate internal linked destinations'
```

The retained Oracle self-tests protect byte/field/owner acceptance, including
EH address binding; they are not game failure-path tests. Historical host suites
outside CI are optional reference tools, not checkpoint gates. Seven redundant
construction/lifecycle suites and four fixtures were removed on 2026-10-28.
Their earlier results remain historical observations, recoverable at commit
`47d9993`; old evidence commands do not impose current requirements.

A docs/test-maintenance batch needs no game rebuild, Ghidra call, cold replay,
whole-build graph validation or unchanged progress regeneration. Run only the
lightweight CI and directly affected infrastructure self-tests.

## Later phases and artifacts

After exact reconstruction, establish faithful Windows i386 product closure,
then semantic reconstruction against both original and native runtime Oracles,
then portability. Whole-product build, runtime and semantic validation belong
to those phases; they are not prerequisites for an exact-owner checkpoint.
Exact functions alone do not establish a runnable product.

Use `.analysis/` for working evidence, `build/` for generated artifacts and
tracked ledgers/docs for accepted facts. Retain what reproduces an unresolved
claim and remove superseded dumps, logs and builds. Inspect recovered dirty
artifacts before cleanup. Never commit private binaries/data, databases or tools.
