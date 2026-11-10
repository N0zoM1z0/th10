# TH10 retained inputs and working artifacts

## What to keep

- Original target and resource inputs, pinned `.tools/` and this game's Wine prefix.
- `ghidra-project/`, `_reference/`, maintained source, canonical ledgers and comparison scripts.
- `build/match-linked/`, which supplies the current canonical native comparison graphs.
- Concrete findings and useful failed approaches in [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md),
  with current work in [RE_HANDOFF.md](RE_HANDOFF.md).
- Unrelated D3D adapter work, including its private directories and retained notes.

Two files under `docs/evidence/` remain actual inputs:
`anm-loaded-start-script-wave11-20261109/compiler-context.cpp` is compiled by the
accepted loaded-script unit, and `anm-child-request-record/binding-contract.json`
is read by the current ANM experiment driver. Their directory name does not make
them disposable historical reports.

## Current unresolved compiler contexts

These five directories under `build/gpt-web/` retain the useful native inputs
and outputs for distinct open problems:

| Owner | Directory |
| --- | --- |
| ANM ExecuteScript | `20261109/anm-execute-child91-aggregate/` |
| ECL Run | `20261109/ecl-run-length-pointer-order/pointer-y-first/` |
| Enemy runtime and motion | `20261108-enemy-runtime-aggregate-sum/` |
| Enemy dispatcher | `20261108-enemy-dispatch-rank-tail-final/` |
| Player HUD loader graph | `20261109/playerhud-restored-natural-loader/` |

Four focused ANM/ECL/Bullet/Seek trial drivers remain in `.analysis/gpt-web/`;
the current handoff names them. Replace or delete a context when its problem is
resolved or it no longer helps the next experiment.

## Cleanup policy

Historical archives, duplicate trial snapshots, old evidence reports, cleanup
audits and dated handoffs do not need permanent retention. Keep their useful
findings and reproducible current comparison commands. A citation alone is not
a reason to keep an old file. Do not add replacement archives, per-file deletion
manifests, hash registries or recurring cleanup reports.

The November 10 cleanup removed the historical private archives and obsolete
reports/experiments, releasing approximately **515 MiB**. No replacement backup
was made. Tracked-file history remains in Git. Moving to another machine still
requires transferring the original/private inputs and current retained contexts
separately from the repository clone.
