# Reconstruction artifacts and recovery — 2026-10-30

This index describes private local state. Use [RE_HANDOFF.md](RE_HANDOFF.md) for
continuation and [PROGRESS.md](PROGRESS.md) for current counts. Ignored inputs,
archives and inventories are not in a Git clone; transfer them separately when
moving this workspace. Archive existence does not make an old result current.

## Expanded working state

The original read-only `resources/th10.exe`, private `ghidra-project/`, pinned
`.tools/` toolchain/Wine environment, `_reference/` checkouts and unrelated D3D
evidence are preserved. Canonical outputs under `build/match-linked/` remain
expanded. A canonical recipe in `config/match-units.toml`, not a directory name
or an old warm context, defines an exact replay.

These campaigns remain expanded under `.analysis/gpt-6.1-sol/` and the matching
`build/gpt-6.1-sol/` root:

| Campaign | Retained purpose |
| --- | --- |
| `ending-run-context-20261030` | Latest 21-input diagnostic; only its `final-source` build remains expanded |
| `enemy-operands-context-20261030` | Accepted integer reader, float candidate and canonical cold records |
| `ecl-script-data-20261030` | Unfinished complete AddScriptData comparison and actual caller graph |
| `anm-executor-inline-context-20261030` | Pin-compatible entry context; other controls remain historical |
| `gui-draw-context-20261030` | Complete draw candidate and cold record |
| `laser-draw-context-20261030` | Retained draw comparison contexts |
| `rectangle-vector-context-20261030` | Small accepted helper cold records |

The [pin audit](evidence/maintenance-20261030/context-audit.json) checks source
and header equality only. Before reuse, attest all pins, compile/link profiles,
source order, object hashes and image hash against the actual graph. Some
historical manifests within an expanded campaign still reference archived
builds. Do not rewrite their pins or assume every manifest is reusable.

The music parsers require the actual PDB producer directory
`build/match-linked/MusicRoomHelpers`; a copied or restored snapshot deliberately
fails that qualification. Rebuild the explicit selectors at their declared
producer when needed.

## Verified private archives

| Archive | Contents and inventory |
| --- | --- |
| `.analysis/archive/terminal-experiments-20261030.tar.xz` | 13,305 terminal files from older analysis/build campaigns, including superseded Ending builds; per-file path/size/SHA-256 inventory at `.analysis/maintenance-20261030/archive-inventory.json.gz` |
| `.analysis/archive/historical-analysis-20261028.tar.zst` | Earlier 26,144-file analysis archive; inventory at `.analysis/handoff-20261028/historical-file-index.jsonl.gz`, receipt at `.analysis/handoff-20261028/cleanup-summary.json` |

The new archive is 32,495,572 bytes, preserving 396,014,930 payload bytes.
Every member hash and one restored context were checked before originals were
removed; source files were rehashed immediately before unlinking. Public
[cleanup metadata](evidence/maintenance-20261030/cleanup-summary.json) pins its
archive and inventory digests. The compactor and terminal log remain in
`.analysis/maintenance-20261030/`.

The older outer archive is 123,011,695 bytes with SHA-256
`b7cf15be7f5f243e6b5bc0aeb9a0fa69924da3364dac8e27e5390aff27426e67`, rechecked
on 2026-10-30. It includes the older small ANM/ECL/Player/RNG/report archives
formerly listed here. Their receipts and inventories remain historical
provenance; **those inner archive paths are no longer expanded**.

Rebuildable `build/pycache/` contents are removed separately. No target bytes,
private database, toolchain or reference checkout was removed. Historical
savings must not be added to this batch's savings.

## Selective restoration

Run from the repository root. List archive members first, then extract only
required ignored paths. The new archive stores individual files with their
original repository-relative names. For example:

```sh
tar -tJf .analysis/archive/terminal-experiments-20261030.tar.xz
tar -xJf .analysis/archive/terminal-experiments-20261030.tar.xz -C . .analysis/gpt-6.1-sol/anm-executor-context-20261030/final-context.json
```

A context file alone does not restore its object/image inputs. Select their
exact member paths from the inventory, or restore that campaign's ignored
build subtree when its historical evidence is actually needed. Check restored
file hashes against the inventory. Do not extract old maintained source over
`src/`, relax target pins or label an old image as a new compilation.

For a pre-2026-10-28 inner archive, restore the inner archive **first** from the
outer zstd archive, then inspect and extract the desired member. This exact
inner path was checked against the outer member list:

```sh
tar --zstd -tf .analysis/archive/historical-analysis-20261028.tar.zst
tar --zstd -xf .analysis/archive/historical-analysis-20261028.tar.zst -C . .analysis/handoff-cleanup-20261023/historical-codegen-reports.tar.gz
tar -tzf .analysis/handoff-cleanup-20261023/historical-codegen-reports.tar.gz
```

The former archive-group table and individual inner restore commands are
recoverable with `git show 58bb846:docs/RE_ARTIFACTS.md`. Apply the outer restore
step above before using them. Some older cleanup batches intentionally removed
regenerable outputs rather than archiving them; these archives do not restore
such deleted builds. Rebuild maintained code through its declared recipe.

Restoring a historical source-bound witness and compiling today's source are
different experiments. Keep their records independent. Preserve the archives
and inventories while they are the only copies of useful historical evidence.
