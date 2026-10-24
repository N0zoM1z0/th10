# Reconstruction workflow

## Session entry

First recover the current worktree instead of assuming it is clean. Read the
handoff, inspect status and diffs (including untracked work), then attest the
disk target, Ghidra target, ledgers, and live status using the commands in
`AGENTS.md`. Run repository Python through `scripts/repo-python`; it verifies
the four pinned Capstone component hashes before selecting an interpreter, so
Web and local shells cannot silently use different decoders. Stop if any
identity differs.

## Exact-first operating method

The operator-approved [2026-10-24 exact-first method](TH10_EXACT_FIRST_METHOD_20261024.md)
governs candidate selection and experiment order. The goal is **98% of reviewed
authored owned bytes exact**, not source coverage or approximate byte equality.
Select representative medium/large owners by comparability, concrete semantic
uncertainty and family reuse, not tiny-leaf counts or nearest byte distance.

Recover natural low-level C++ first. Honest provisional offset/numeric names
are acceptable; recover actual widths, signedness, captures/lifetimes, rounding,
control flow and callee declarations before imposing readable classes or APIs.
Make the first complete-owner comparison early. Use directed native controls
only for concrete ambiguities, not a large test matrix before that comparison.
After stable exactness, improve readability and replay the affected complete
owner and required contract after each refactor.

Each trial tests one falsifiable coherent model against a distinct baseline.
Preserve full extents, all fields, fixed local targets and required complete
callees; structural failures are diagnostic, never acceptance. Park repeated
byte-identical spelling panels with a bounded receipt and switch representative,
without a magic trial count or global exhaustion requirement. ANM's four-byte
and ECL's 25-byte cases reopen only with new target/def-use/lifetime/callee/context
evidence and a specific prediction. Current source is already compiler-oriented;
readability alone and a wrong compiler/TU are not established causes.

Report new whole-owner exact bytes separately from source coverage and useful
diagnostic progress. Never credit a fragment or convert a near-match score into
an exact owner. Preserve TH10 LTCG `/GL`; TH105 successes transfer experimental
discipline, not VC8 flags or data. Do not use fake ABI, padding, copied bytes,
allocation-forcing register hints or weakened comparators.

Current Web/Factory work uses registered repository `th10` and local
`gpt-dots: ...` commits, never push. The coordinator alone runs native builds,
serially on one CPU at niceness 10, and owns final gates/commits. Disjoint
documentation and read-only analysis may proceed separately. Historical
direct-local-only/no-subagent instructions do not govern this session.

## Bounded exact-reconstruction loop

1. Keep entry, physical boundary, origin, and source presence independent. Run
   `scripts/repo-python scripts/ghidra.py architecture`, then
   `scripts/repo-python scripts/report-boundary-inventory.py --check-ledger` before selecting
   work from a broad or newly imported inventory. Ledger writes/checks require
   that fresh ignored Ghidra body-range export. Use `--write-ledger` only
   after reviewing the target-bound report and any changes to candidate rows.
   The report's gap and target-reference lists are triage evidence: inspect a
   candidate's bytes, references, tails, tables, padding, and Ghidra body ranges
   before adding an entry or promoting it to `reviewed`.
   Its indexed-indirect-control list is also a boundary queue. A table base
   beyond a candidate's recorded end often reveals an omitted compiler-owned
   switch table, but review the selector domain, full table length, all
   destinations and neighboring padding before extending the owner.
2. Select one candidate from the ledger; use
   `scripts/repo-python scripts/report-exact-backlog.py` for the reviewed authored,
   source-present queue. Before handing a subsystem to exact-codegen work, use
   `scripts/repo-python scripts/report-source-completeness.py --module NAME
   --require-complete` to fail closed if any reviewed authored owner still lacks
   a maintained source mapping. This checks source presence only and grants no
   exactness credit. Use `scripts/repo-python scripts/rank-exact-backlog.py` when a
   fresh, target-bound comparison of the normal-COFF and LTCG lanes would help
   select a bounded source/codegen experiment. Its score is triage only and
   grants no exactness or physical-owner credit. Never select only by an
   attractive decompiler name. For semantic core recovery, use
   `scripts/repo-python scripts/rank-core-backlog.py`; its size and Ghidra connectivity score
   only orders source-absent candidates and grants no semantic, origin, or
   boundary claim.
3. Reconcile its entry, exits, tails, tables, padding, references, and physical
   ownership against target bytes.
4. Classify origin independently: authored game code, compiler-generated,
   library, import thunk, data, padding, or still unknown. Use `review` for an
   unexamined tracked entry and `indeterminate` only after an individual
   target/Ghidra review records why source authorship remains unresolved. An
   indeterminate entry is reviewed, but contributes neither to authored nor
   excluded counts; it keeps the authored exact denominator provisional.
5. For authored code, recover ABI, types, object layout, globals, constants,
   side effects, and error behavior using bounded Ghidra queries and raw target
   evidence. Keep hypotheses explicitly falsifiable.
6. Place a coherent complete natural low-level C/C++ owner in a candidate
   translation unit. Keep provisional names where appropriate and mark source
   presence without claiming exactness. Do not require speculative class/API
   cleanup before comparison.
7. Compile the first complete owner early with an explicit candidate profile;
   use smaller directed probes only to resolve concrete blocking ambiguities.
   The pinned build-6030 tool surfaces are observed, but the profile and physical
   owner are still hypotheses. Determine whether the
   candidate belongs to a normal C/C++ COFF unit or to LTCG. For normal COFF,
   compare the complete owned extent and every relocation. For LTCG, do not
   strip `/GL` and do not use the standalone comparator. Use
   `scripts/repo-python scripts/probe-ltcg-backlog.py --source SOURCE` to recover a
   PE/PDB-bound linked extent and structural diagnostic, while keeping exactness
   unknown until review establishes every resolved field. If target calls show
   that LTCG lowers the candidate only inside a particular maintained caller,
   add `--entry SOURCE=SOURCE_NAME`; the report records and validates that
   explicit context. If cross-translation-unit inlining needs another recovered
   input, add one or more `--support SOURCE=SUPPORT_SOURCE` arguments. When
   target evidence supports a normal non-`/GL` partition for a support TU,
   select it with `--normal-support SOURCE=SUPPORT_SOURCE`. Canonical linked
   units preserve both lists as `support_sources` and `normal_support_sources`.
   A declared `support_profiles` override may narrow a selected LTCG support's
   switches while retaining `/GL`; it participates in grouping and replay.
   The RGB scale support's `/Ob1` is one bounded reproduced hypothesis, not
   evidence of original production flags. See `docs/ORACLES.md`.
   `rank-exact-backlog.py` accepts the same entry override. A canonical linked
   unit must declare `artifact_kind = "linked-pe"`, the fixed compiler/link
   profiles and harness, its unique PDB-owned symbol extent, and every semantic
   link target. `compare-linked-function.py` must decode the complete extent,
   reject any missing or extra linked field, replay all declared fields, and
   return zero differences. The resulting bounded claim does not establish the
   diagnostic image as a product or identify the production physical owner.
   Distinguish source,
   profile, boundary, ownership, optimizer, and library mismatches.
   `scripts/repo-python scripts/probe-exact-backlog.py --source SOURCE` performs the fixed
   normal-COFF batch diagnostic after deriving symbols and extents from the
   object itself; it grants no exactness credit.
   Source-less VC7.1 static-runtime members use the separate
   `artifact_kind = "coff-archive"` lane. A unit must pin the archive hash and
   member, record the complete COFF symbol extent and every relocation's
   target, and cold-extract that member before the ordinary comparator runs.
   Such a unit may close a library body in the overall byte ledger while its
   origin remains `library/exclude` and its source mapping stays empty.
8. Add a canonical match unit and exact ledger row only after repeatable zero
   difference against the verified target.
9. Under the operator-authorized accelerated scope (2026-10-05), validate the
   functions whose bodies or interfaces were modified. Cold-replay modified
   canonical units and new exact candidates with explicit `--unit` selectors;
   review changed nonexact dependencies without granting them exact credit.
   Do not routinely replay all prior functions or all consumers of a shared
   source/header merely because they share a build graph. Full owned extents,
   independent target bindings and cold reproducibility requirements for each
   claimed exact function remain unchanged. Factory receipt acceptance is a
   separate claim when that transport is used. Record the
   actual completed validation scope and any cancelled broader checks. Then
   regenerate progress, update the handoff and per-game knowledge, inspect the
   final diff, and commit one coherent local checkpoint.

## Product and semantic gates

Function exactness is followed by a separate faithful-product phase. Populate
`config/build.toml` as evidence establishes translation units, compiler
profiles, libraries, resources, initializers, data owners, and link order. A
successful link is necessary but insufficient: exercise representative
Windows i386 runtime scenarios and audit production data ownership.

Only after that native product is trustworthy should semantic reconstruction
begin. Each semantic change then has two runtime Oracles:

1. the original hash-attested TH10 target;
2. the reconstructed Windows i386 product built from the exact baseline.

Agreement between both substantially reduces false positives that a single
runtime or a premature modern port can hide. Portability work starts after this
dual-Oracle semantic baseline, in a distinct product configuration.

## Artifact lifecycle

`.analysis/` is disposable working state, not a journal. Keep small manifests,
commands, and outputs needed for unresolved evidence; convert reusable logic to
scripts and accepted facts to tracked ledgers/docs. Remove superseded dumps,
duplicate decompilations, stale logs, and obsolete build artifacts at each
checkpoint. Inspect unexplained artifacts from a recovered dirty session first; once their retained work is preserved in source or durable notes, remove the disposable copies instead of carrying them forward indefinitely.
