# Reconstruction workflow

## Session entry

First recover the current worktree instead of assuming it is clean. Read the
handoff, inspect status and diffs (including untracked work), then attest the
disk target, Ghidra target, ledgers, and live status using the commands in
`AGENTS.md`. Run repository Python through `scripts/repo-python`; it verifies
the four pinned Capstone component hashes before selecting an interpreter, so
Web and local shells cannot silently use different decoders. Stop if any
identity differs.

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
6. Place natural C/C++ in a candidate translation unit. Mark source presence
   without claiming exactness.
7. Compile the smallest useful historical-compiler probe with an explicit
   candidate profile. The pinned build-6030 tool surfaces are observed, but the
   profile and physical owner are still hypotheses. Determine whether the
   candidate belongs to a normal C/C++ COFF unit or to LTCG. For normal COFF,
   compare the complete owned extent and every relocation. For LTCG, do not
   strip `/GL` and do not use the standalone comparator. Use
   `scripts/repo-python scripts/probe-ltcg-backlog.py --source SOURCE` to recover a
   PE/PDB-bound linked extent and structural diagnostic, while keeping exactness
   unknown until review establishes every resolved field. If target calls show
   that LTCG lowers the candidate only inside a particular maintained caller,
   add `--entry SOURCE=SOURCE_NAME`; the report records and validates that
   explicit context. If cross-translation-unit inlining needs another recovered
   `/GL` input, add one or more `--support SOURCE=SUPPORT_SOURCE` arguments;
   canonical linked units preserve that ordered list as `support_sources`.
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
8. Add a canonical match unit and exact ledger row only after repeatable zero
   difference against the verified target.
9. Cold-replay affected accepted units with
   `scripts/repo-python scripts/replay-exact-units.py --source SOURCE`; use the unfiltered
   command at integration checkpoints. Then regenerate progress, update the
   handoff and per-game knowledge, inspect the final diff, and commit one
   coherent local checkpoint.

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
checkpoint. Never delete unexplained artifacts from a recovered dirty session.
