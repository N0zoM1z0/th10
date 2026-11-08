# TH10 reconstruction agent rules

## Current direction — handoff and exact reconstruction

The current task is exact reconstruction. Target at least 98% of reviewed
authored owned bytes exact. Prioritize complete medium/large representatives
alongside useful leaves, and compare the first complete source early. Natural
low-level C/C++ and provisional numeric/offset names are acceptable.

Call local tools directly. Do not route through Factory providers, require
Factory receipts, or read Factory guidance as a session prerequisite. Use
`scripts/ghidra.py` for read-only local Ghidra analysis when needed.

Use English local commits prefixed `gpt-web:`, author
`N0zoM1z0 <1705004398@qq.com>`, with trailer
`Co-authored-by: gensokyo-dev <342580529+gensokyo-dev@users.noreply.github.com>`.
Never push. Preserve unrelated work, including D3D adapter evidence.

## Target and recovery

Only the original Japanese TH10 v1.00a executable is valid, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The canonical private input is `resources/th10.exe` under this repository.
Keep it read-only, ignored, and uncommitted. Do not search `/mnt`, add Windows
mounts, or set `TH10_TARGET_PATH` without an explicit operator override.

At session entry, inspect `git status --short --branch`, tracked/untracked work
and the diff. Recover dirty work before proceeding. Read the current handoff
and relevant source/ledger rows; consult workflow and Oracle details for the
lane being used rather than rereading every historical document.

Use `scripts/repo-python` for repository Python. Before target-dependent work,
run `scripts/repo-python scripts/verify-target.py`. Check toolchain declarations
with `scripts/repo-python scripts/verify-toolchain.py --check`; run its
`--execute` smoke on setup, toolchain changes, or a compiler/Wine problem, not
before every unit. Attest local Ghidra before using its observations; no Ghidra
call is needed for a documentation/test-maintenance batch.

## Exact work and acceptance

- Work on one bounded owner or coherent infrastructure batch and leave a
  reviewable local checkpoint. Prefer complete comparisons over speculative
  test matrices. Park repeated byte-neutral controls; reopen with new evidence
  and a falsifiable prediction.
- Keep compiler/linker jobs serial, one CPU at nice10, with bounded runtime and
  retained exit logs. Preserve TH10's pinned VC7.1 profiles and LTCG `/GL` where
  applicable; do not transplant another game's compiler flags.
- Preserve strict complete-owner bytes, every relocation/resolved field, fixed
  local destinations, required callee bindings and actual ABI. Only a complete,
  target-bound, cold-reproducible zero-difference comparison may enter
  `config/matches.csv`. Near matches remain candidates.
- Use natural source. Never copy target bytes, manufacture returns, add inert
  padding, force registers through fake ABI declarations, or patch target code.
- Replay new exact owners and affected existing exact bodies/interfaces with
  explicit `--unit` selectors. Do not replay the full historical cohort or all
  consumers merely because they share a source/header/build graph.
- Do not add or require host simulations of allocation failure, null-pointer
  paths, exception injection, ownership/lifecycle scenarios, sanitizer/leak runs,
  or mutation campaigns for ordinary exact work. The target's actual branches
  and cleanup code still belong in its complete byte comparison. A small probe
  is appropriate only when it resolves a specific current exact mismatch.
- Comparator self-tests are needed when that comparator changes, limited to the
  affected mechanism. EH address/metadata binding checks are exactness checks;
  they do not require executing failure scenarios. Historical test reports and
  archived commands are evidence, not current checkpoint requirements.

## Evidence and later phases

Keep boundary review, origin, source presence, exact codegen, product closure,
runtime validation and portability independent. Ghidra names/types/boundaries
are provisional. Record durable TH10 facts in `docs/KNOWLEDGE_BASE.md` and the
current handoff; retain compact reproducible evidence rather than test reports
unrelated to the exact claim.

Adjacent TH08/TH09/TH095 reconstructions may suggest hypotheses, never TH10
bytes, layouts, ownership or exactness. Consult them only for a concrete lead.
Unknown TH10 facts stay unknown.

Product build/runtime closure, semantic dual-Oracle work and portability are
later phases. Do not require their tests or whole-build/source-completeness
reports to finish an exact unit. Do not claim these phases from exactness.

## Agent handoff and reclaimable storage

The short, authoritative continuation brief is docs/RE_HANDOFF.md.
Its current-source six-TU score-file graph supersedes older five-TU
caller assumptions; earlier independent exact-owner receipts remain valid
for the source epoch they actually compile.
Do not append repetitive chronological exact claims to its top;
consolidate into the accepted-owners table and current open-owner
frontiers, with evidence links. Generated PROGRESS.md is the count
authority; dated notes, scratch candidates and historical source
pins must not override current ledgers. For uncertain old claims,
label their source epoch rather than silently promoting them.

When asked to save disk space, remove only verified regenerable
ignored experiment PE/OBJ/PDB and Python caches. Preserve original
target inputs, .tools, the Ghidra project, canonical
build/match-linked, historical private evidence archives and
unresolved heavyweight witness contexts. Record the deleted scope,
retained witnesses and payload bytes in docs/RE_ARTIFACTS.md.
Do not mistake a list of deleted file hashes for a restorable archive.

## Checkpoint

For source changes, finish with the affected native exact comparison. For
metadata/docs/test-only changes, do not rebuild game code. Run the lightweight
`scripts/repo-python scripts/ci.py` and `git diff --check`. Its default seven
checks cover Python syntax, docs, ledgers, toolchain declarations, canonical
match graph, generated progress and whitespace. Select affected Oracle
self-tests explicitly with `--only` when changing comparison infrastructure.
Regenerate progress only when its ledger inputs change.

Keep scratch/logs below `.analysis/`, builds below `build/`, shared-tool links
below `.tools/`, and the private database below `ghidra-project/`. Remove
superseded experiments after retaining useful evidence. Never commit original
game binaries/data, databases, toolchains, generated decompiler text,
credentials or private endpoints.

## Shared Factory runtime

Follow [the shared tools instructions](docs/SHARED_TOOLS.md). Ghidra/JDK and
Wine executables are shared through Factory; repository scripts also work
without MCP. Use `scripts/shared_runtime.py` and the existing compiler/analysis
frontends. Keep this game's prefix, analyzer project, Java home/cache, target
and compiler profile isolated. Do not install duplicate tools or inherit
another game's `WINEPREFIX`. This shared-runtime procedure supersedes older
per-repository installation instructions; pinned versions/hashes still apply.
