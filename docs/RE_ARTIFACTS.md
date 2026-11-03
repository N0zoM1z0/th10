# Reconstruction artifacts and recovery — 2026-11-03

Use [RE_HANDOFF.md](RE_HANDOFF.md) for continuation and [PROGRESS.md](PROGRESS.md)
for counts. Ignored inputs/archives are private workspace state, not Git clone
contents. Transfer archives and inventories separately when moving workspaces.
Archive existence or a familiar directory name does not make old bytes current.

## Latest verified selective cleanup — 2026-11-03

A further bounded cleanup removed **543 ignored files** and reclaimed
**15,661,348 payload bytes (14.94 MiB)** or **16,596,992 disk-block bytes
(15.83 MiB)**. The deleted set consisted only of generated PE/OBJ/PDB
from superseded `build/gpt-web/` experiments and verified Python bytecode.
Candidate source snapshots, logs, `.map` and `.rsp` diagnostics remain;
**canonical `build/match-linked/` producers, target, Ghidra, `.tools`,
private archives and the D3D workspace were unchanged**.

[Cleanup scope and recovery notes](evidence/handoff-storage-20261103/README.md)
and [audited machine summary](evidence/handoff-storage-20261103/cleanup-summary.json)
list all pruned top-level roots, byte counts, protected before/after
inventories and the per-file inventory checksum. The ignored complete
per-file SHA-256 manifest remains under `.analysis/maintenance-20261103/`;
it is **not** an archive containing deleted bytes. Rebuild individual
missing experimental outputs only when needed for new work.

All **six** earlier preserved nonexact/near-exact diagnostic witnesses listed below
remain, plus these **five** more contemporary large-owner witness directories:
`build/gpt-web/game-cleanup-warm-20261102`,
`build/gpt-web/popup-browser-context-20261102`,
`build/gpt-web/bomb-full-caller-20261102`,
`build/gpt-web/bomb-laser-integrated-20261102`, and
`build/gpt-web/popup-item-same-owner-20261102`.
They are retained as historical compiler contexts only, not guarantees of
current source pin equality. Files under build/gpt-web outside those kept
roots may intentionally lack PE/PDB/OBJ after this cleanup, while preserving
their diagnostic logs, MAPs and scripts. Python-driven CI may recreate ignored bytecode caches; clearing the
regenerated `.pyc` files after final verification does not represent a second
independent storage saving and is not added to the original cleanup tally.

## Earlier cleanup and six historical witnesses — 2026-11-02

At the earlier 2026-11-02 checkpoint a targeted cleanup reclaimed **200.41 MiB** (210,141,821 payload
bytes) from ignored, regenerable build/gpt-web permutations and build/pycache.
It deleted 4,252 experimental build files plus bytecode-cache files. The
candidate build tree went from about 210 MiB to **4.9 MiB**. See the
[reconciled cleanup report](evidence/handoff-storage-20261102/README.md)
and [machine-readable summary](evidence/handoff-storage-20261102/cleanup-summary.json).

The six diagnostic build directories preserved at that earlier checkpoint are still retained:

- build/gpt-web/anm-executor-current-20261102
- build/gpt-web/anm-executor-independent-20261031
- build/gpt-web/ecl-run-stack-home-20261102/len-volatile-x
- build/gpt-web/game-owner-create-init-20261102/current
- build/gpt-web/gui-draw-20261102/baseline
- build/gpt-web/current-turn/sound-sample-captured-remaining

The 90 retained files are **candidate/near-match witnesses**, not a
complete game build. Their source/header pins and native graph must be
re-checked before use. Large source-input graphs and past per-variant
source snapshots remain under ignored .analysis/gpt-web; canonical
accepted native producers under build/match-linked are untouched.
The compressed per-file deleted inventory is ignored under
.analysis/maintenance-20261102/, and describes files to regenerate;
it is **not** a restorable archive of their contents.

The original target, Ghidra project, .tools, .analysis/archive,
build/match-linked, build/gpt-6.1-sol and D3D evidence were preserved.
No archived evidence or unique private input was deleted.

## Historical 2026-10-31 inventory (source-epoch warning)

The next table documents what was expanded **at that earlier epoch**.
Its 13/13, 14/15, 34/36 and similar source-pin fractions were
observations at the original snapshot, not measurements of the
post-2026-11-02 source. Never call them current without recomputing.

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
| hint-frame-loop-20261031 | Exact frame/callback proof; retained 36-pin context is now 34/36 after AnmManager.cpp and Hint.cpp changed. The current source was separately cold-replayed twice (581/581). |
| gui-draw-close-20261031 | 724-byte nonexact draw candidate; retained 15-pin context is now 14/15 after AnmManager.cpp changed. |
| gui-state-view-20261031 | 4472/4480 interpreter witness; retained 64-pin context is now 60/64 (AnmManager.cpp, FrontEnd.cpp, Hint.cpp, Sound.cpp changed). |
| `cleanup-context-20261031`, `cleanup-switch-20261031` | Complete cleanup/update residuals and real dependency/EH witnesses; source pins are historical |
| `ecl-script-data-20261030` | Actual-caller547/589 comparison; three inputs changed; pin-equal one-source entry is a different graph |
| anm-executor-inline-context-20261030 | Complete executor/call-boundary witnesses; post-PreloadSurface context was 13/13 at its prior check; later source changes require a fresh pin audit. |
| `enemy-operands-context-20261030` | Integer-reader exact history and open float reader; revalidate source epoch |
| ending-run-context-20261030 | Complete 1568-byte Run witness; latest final-source context is now 57/70 pins (13 changed inputs). |
| `laser-draw-context-20261030` | Complete draw comparison witnesses; revalidate source epoch |

The original pin audit (evidence/maintenance-20261031/context-audit.json)
covers 319 recognized manifests at the pre-follow-up cleanup epoch, 113 of
which remained expanded and five matched their recorded source/header hashes.
Later accepted source changes invalidate some of those pins. Current selected
campaign status is listed in the table above and in RE_HANDOFF.md. This is
neither complete native acceptance nor permission to reuse arbitrary objects.
Attest actual graph/source order, profiles, object hashes, image and required
ABI before incremental reuse.
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

## Follow-up generated-build pruning — 2026-10-31

A later cleanup removed **2,650 ignored, untracked files**
(115,052,465 payload bytes; 109.72 MiB) from
build/gpt-web/current-turn, all exploratory permutations under
build/gpt-web/newturn-front-end except final-capture-cold, build/gpt-web/newturn-ascii,
build/gpt-web/seek, and regenerable build/pycache. The current-turn research
scripts, source snapshots and receipts under .analysis/gpt-web/current-turn
were retained. Canonical build/match-linked outputs, the final capture witness
(11 files), all ten retained gpt-6.1-sol campaigns, active ANM research
contexts, private archives, original target, Ghidra database and toolchain were
not removed. Per-root counts and hashes for the retained capture witness are
recorded in the follow-up maintenance inventory and cleanup-summary.json.

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
