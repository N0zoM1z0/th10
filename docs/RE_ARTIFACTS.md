# Reconstruction artifacts and recovery — 2026-10-31

Use [RE_HANDOFF.md](RE_HANDOFF.md) for continuation and [PROGRESS.md](PROGRESS.md)
for counts. Ignored inputs/archives are private workspace state, not Git clone
contents. Transfer archives and inventories separately when moving workspaces.
Archive existence or a familiar directory name does not make old bytes current.

## Preserved inputs and expanded state

The read-only original `resources/th10.exe`, `ghidra-project/`, pinned `.tools/`
toolchain/Wine environment, `_reference/` and unrelated D3D evidence are untouched.
Canonical outputs under `build/match-linked/` remain expanded, including the
qualified `MusicRoomHelpers` producer and `MainD3DInterface` evidence.
A recipe in `config/match-units.toml` defines an exact replay, not an old image.

The following campaigns remain expanded under both `.analysis/gpt-6.1-sol/`
and `build/gpt-6.1-sol/`:

| Campaign | Retained purpose |
| --- | --- |
| `hint-frame-loop-20261031` | Current exact frame/callback and actual nonexact consumption callee; current36-pin eight-input context |
| `gui-draw-close-20261031` | Latest complete724-byte draw candidate; final15 source/header pins match |
| `gui-state-view-20261031` | Latest4472/4480 interpreter witness; Hint.cpp pin has changed |
| `cleanup-context-20261031`, `cleanup-switch-20261031` | Complete cleanup/update residuals and real dependency/EH witnesses; source pins are historical |
| `ecl-script-data-20261030` | Actual-caller547/589 comparison; three inputs changed; pin-equal one-source entry is a different graph |
| `anm-executor-inline-context-20261030` | Complete executor/call-boundary witnesses; revalidate source epoch |
| `enemy-operands-context-20261030` | Integer-reader exact history and open float reader; revalidate source epoch |
| `ending-run-context-20261030` | Complete1568-byte Run witness; ten inputs changed since its final source context |
| `laser-draw-context-20261030` | Complete draw comparison witnesses; revalidate source epoch |

The [current pin audit](evidence/maintenance-20261031/context-audit.json) covers
319 recognized manifests before cleanup,113 of which remain expanded. Five
match their source/header hashes. This is neither complete native acceptance
nor permission to reuse arbitrary objects. Attest actual graph/source order,
profiles, object hashes, image and required ABI before incremental reuse.
Do not rewrite historical pins to make a cache appear current.

## Verified private archives

| Archive | Inventory / retained record |
| --- | --- |
| `.analysis/archive/terminal-experiments-20261031.tar.xz` |5,370 files from41 ended/superseded roots; `.analysis/maintenance-20261031/archive-inventory.jsonl.gz`; [current cleanup record](evidence/maintenance-20261031/README.md) |
| `.analysis/archive/terminal-experiments-20261030.tar.xz` |13,305 terminal files; `.analysis/maintenance-20261030/archive-inventory.json.gz`; [previous record](evidence/maintenance-20261030/cleanup-summary.json) |
| `.analysis/archive/historical-analysis-20261028.tar.zst` |Earlier26,144-file outer archive; `.analysis/handoff-20261028/historical-file-index.jsonl.gz` and cleanup-summary.json |

The new archive preserves213,017,412 payload bytes in4,162,508 archive bytes.
Every member's path/size/SHA-256 was verified, and sources were rehashed before
removing expanded copies. Its archive/inventory digests, protected-input checks
and net storage reduction are in the current cleanup record. Previous archives
and their inventories are preserved; their older verification epochs are not
presented as fresh checks. Regenerable Python cache removal is recorded separately.

Completed exact campaigns, superseded prototypes, source capsules and logs are
recoverable from these archives. Some context files inside retained campaigns
still reference archived builds. Read the inventory rather than assuming all
old paths are expanded. Never delete `.analysis/` or its archives wholesale.

## Selective restoration

List members and extract exact ignored paths from the repository root:

```sh
tar -tJf .analysis/archive/terminal-experiments-20261031.tar.xz | rg 'hint-consume-lifetimes-20261031/final-cold'
tar -xJf .analysis/archive/terminal-experiments-20261031.tar.xz -C . .analysis/gpt-6.1-sol/hint-consume-lifetimes-20261031/final-cold-context.json
```

A context alone does not restore its objects/image. Select their exact paths
from the per-file inventory, extract them too, and verify hashes. Do not extract
historical maintained source over `src/`. Restoring a historical witness and
compiling today's maintained source are different experiments.

For pre-2026-10-28 inner archives, restore the inner archive from the outer zstd
archive first, then inspect its members. This inner path was previously verified:

```sh
tar --zstd -tf .analysis/archive/historical-analysis-20261028.tar.zst
tar --zstd -xf .analysis/archive/historical-analysis-20261028.tar.zst -C . .analysis/handoff-cleanup-20261023/historical-codegen-reports.tar.gz
tar -tzf .analysis/handoff-cleanup-20261023/historical-codegen-reports.tar.gz
```

Older detailed restore tables remain in Git (`58bb846:docs/RE_ARTIFACTS.md`).
Some earlier batches intentionally deleted regenerable outputs instead of
archiving them; rebuild those only through their declared scoped recipes.
Music parser static ownership requires the actual qualified canonical producer
path; a copied snapshot is not a substitute for that producer qualification.
