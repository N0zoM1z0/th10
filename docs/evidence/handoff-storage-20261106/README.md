# 2026-11-06 next-agent safe storage cleanup — verified restorable archive

This is a **new, separate cleanup event**, not a restatement of the
2026-11-04/05 historical cleanup totals. Its target is the real TH10
repository with Japanese v1.00a original SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
No tracked sources, ledgers, canonical PE producers, or target binaries
were deleted or changed as part of the cleanup.

## What was archived and pruned

The retained private machine manifest explicitly names **367 ignored and
regenerable experiment-native files** (`.exe`, `.obj`, `.pdb`, `.map`, etc.)
from **24 failed/superseded test prefixes under `build/gpt-web/`**, totaling
**24,327,745 raw payload bytes**. These are *not* canonical owner artifacts.
The 24 directory names and exact per-file paths/sizes/SHA-256 hashes are
in [the machine receipt](cleanup-summary.json) and the ignored manifest
`.analysis/maintenance-next-agent-20261106/native-pruned-files-20261106.jsonl`.
All `.analysis/gpt-web/` probe scripts, JSON trial results and source
snapshots remain in place. The only other deletions were **247 regenerable
`.pyc` cache files / 4,581,025 content bytes** under `build/pycache`.

The files were written to a **real selectively restorable tar.zst archive**:

    .analysis/archive/gpt-web-rejected-native-20261106.tar.zst

The archive has **3,158,740 compressed bytes** and SHA-256
`cd9d92d88606ca46060eed0154c140d3971e6bfdcb6436b9db05df31a3992ed5`.
The decompressed archive stream was read independently, every one of its
**367/367 regular members** compared with the original per-file SHA-256
and size, and a sample `source.map` was extracted into isolated scratch,
independently rehashed and removed. All 367 expanded sources were rehashed
*again* immediately before unlink. This archive, not merely the filename
manifest, is the actual recovery medium.

The first cleanup attempt stopped **before deleting anything** when the
single-file restoration smoke test left empty scratch directories. That
smoke cleanup path was corrected; the existing verified archive was
**never overwritten**. The successful resumed attempt reverified every
member and performed the guarded unlink. Its reproducible private script
and outputs are `.analysis/maintenance-next-agent-20261106/prune.py` and
`prune.log`. Do not rerun the delete script against an already pruned tree.

## Measured disk blocks and scope

The directly measured `build/` + `.analysis/` allocated-block difference
**after the new archive had been created** was **30,265,344 bytes** (~28.9
MiB). This includes the 367 expanded native artifacts and the regenerated
Python cache removed. The archive itself occupies about 3.2 MB; after
accounting for this new archive, the approximate initial-to-final net
saving is **27 MB**. Those are two different baselines; do **not** add
historical cleanup savings or claim the 30 MB figure includes new archive
creation. Complete before/after component byte counts are in
[cleanup-summary.json](cleanup-summary.json).

## Preserve/restore guidance for the next agent

**Left intact:** `resources/th10.exe`, `.tools/`, `ghidra-project/`,
`_reference/`, `resources/private/`, the full `build/match-linked/` canonical
producer tree, all older `.analysis/archive/` archives,
`.analysis/gpt-web/` scripts/receipts, `docs/evidence/`, and the currently
pinned whole-owner ANM, ECL and Enemy source graphs:

- `build/gpt-web/20261106-handoff-anm-wrap/current/`
- `build/gpt-web/20261106-ecl-store-word/current/`
- `build/gpt-web/20261106-enemy-bullet-spawn-alias/current/`

To inspect and selectively restore a pruned member, **to an isolated
scratch location**, not on top of a freshly built current-source graph:

```sh
zstd -t .analysis/archive/gpt-web-rejected-native-20261106.tar.zst
zstd -dc .analysis/archive/gpt-web-rejected-native-20261106.tar.zst |
  tar -tf - | grep '20261106-enemy-constructor-sentinel' | head
mkdir -p /tmp/th10-restore-rejected
zstd -dc .analysis/archive/gpt-web-rejected-native-20261106.tar.zst |
  tar -xf - -C /tmp/th10-restore-rejected -- \
    build/gpt-web/20261106-enemy-constructor-sentinel/sentinel-first/source.map
```

Check the selected file SHA-256 and size against the ignored JSONL
manifest **before use**; never mix files from an old source epoch with
new current source compilation. The original executable, toolchain,
archive and Ghidra database are **ignored/private** and require an
authorized separate transfer; Git alone does not contain them. See the
[current artifact policy](../../RE_ARTIFACTS.md) and concise
[agent handoff](../../RE_HANDOFF.md).
