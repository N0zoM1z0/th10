# Cleanup and handoff receipt — 2026-10-23

The operator requested code/document cleanup and reduced local artifact volume
before handoff. Starting source checkpoint 2b03d2b is retained. Maintained game
code, headers, compiler profiles, build recipes and exact ledgers are unchanged.
No additional reconstruction experiment was started. The campaign remains
active-incomplete and no native producer is left running.

## Documentation and recovered work

The live [handoff](../../RE_HANDOFF.md) now contains current metrics, retained
inputs, outstanding large-function work, exclusions and restoration commands.
All 26 historical sections were moved verbatim to
[the dated history](../../RE_HANDOFF_HISTORY_20261023.md), retaining their relative
links. Historical progress and operator preferences are explicitly identified
as historical. The latest Stage EDX-versus-EAX observation corrects the old ECX
note. Four previously completed
[Stage controls](../stage-integer-lifetimes/README.md) are recovered and rejected
or classified as neutral, without changing the maintained source.

## Space reclaimed

| Disposition | Files | Before bytes | After bytes | Reclaimed bytes |
| --- | --- | --- | --- | --- |
| Losslessly archive historical compiler diagnostics | 329 | 190634458 | 12619788 | 178014670 |
| Remove redundant completed Stage native outputs | 60 | 2705612 | 0 | 2705612 |
| Total | 389 | 193340070 | 12619788 | 180720282 |

This is 180.7 MB decimal, about 172.3 MiB, excluding block rounding and small new
documentation/inventory files. No unknown-origin or provider state is removed.

The [archive inventory](archive-inventory.json) pins every original path, length
and SHA-256 plus the archive digest. Only historical compiler/Oracle report
JSONs of at least 256 KiB under the two identified campaign roots are selected.
Input manifests, source controls, boundary inventories and current Stage/ANM
replay inputs remain expanded. Every archive member was read back and verified
for its exact length and SHA-256 before any expanded file was removed.

Private archive:
`.analysis/handoff-cleanup-20261023/historical-codegen-reports.tar.gz`

Restore from the repository root before rerunning a historical consumer:

```sh
tar -xzf .analysis/handoff-cleanup-20261023/historical-codegen-reports.tar.gz -C .
```

To restore one report, append its exact original `.analysis/...json` path from
the inventory. The earlier GUI archive remains untouched; its separate
[receipt](../handoff-cleanup-20261022/README.md) records restoration.

The [carrier inventory](carrier-inventory.json) records all 60 deleted files,
verified against the original producer hashes before removal. Their source
copies, four patches, compact results and producers remain expanded. The
cleanup scripts and protection manifest remain under
`.analysis/handoff-cleanup-20261023` as ignored local recovery state.

## Verification and retained inputs

All 1568 protected file hashes are checked before and after cleanup: maintained
code/tests/config, current canonical linked outputs, the retained scoped-rank/
shared-aim Enemy candidate, original executable, previous GUI archive and all
nine unrelated D3D files. The D3D directory remains untracked and unstaged.
Toolchains, Wine state, Ghidra database, game data, other private candidates and
unclassified legacy artifacts are retained.

Fresh cached canonical Stage replay passes 11 complete guards 987/all 36 and keeps
the full 1868-byte parent. The four completed private Stage images are compared
across their complete parent extents and exhaustive equal field inventories
before deletion. Target verification, executed toolchain smoke, toolchain check,
tracking, progress generation, documentation links and whitespace pass. Full CI
is not repeated for this documentation/artifact-only batch under the operator's
focused-validation preference. No new game compile, exact credit, product
closure or runtime validation is claimed. Final details are in
[validation-summary.json](validation-summary.json).
