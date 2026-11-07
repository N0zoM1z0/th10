# TH10 private artifacts — next-agent storage and recovery

**Current snapshot: 2026-11-07, after source checkpoint `16b0ae5`,
subsequent handoff-documentation correction `7aeab42`, and the selected
2026-11-07 native-artifact pruning.** Use [RE_HANDOFF.md](RE_HANDOFF.md) for
current source pins/frontiers and [PROGRESS.md](PROGRESS.md) for canonical
counts. Older storage events and source epochs are historical; do not use
those figures as live disk inventories.

## Retain and transfer

- Original ignored/read-only `resources/th10.exe`, SHA-256
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
- `.tools/`, including pinned compiler/Wine/headers; `ghidra-project/`;
  `_reference/`; `resources/private/`; all existing `.analysis/archive/` files.
- Every `build/match-linked/` canonical artifact and all current unresolved
  complete witnesses listed in [RE_HANDOFF.md](RE_HANDOFF.md).
- `.analysis/gpt-web/` source snapshots, drivers, receipts and useful logs;
  unrelated D3D adapter evidence; tracked source/config/docs and Git history.

A clone supplies none of the ignored original, tools, database, archives or
research receipts. Transfer those separately and verify their hashes. An old
native image does not certify changed source, even when its directory still
exists. Do not mix old PDB/MAP/OBJ files with a current source epoch.

## Current expanded storage and the 2026-11-07 cleanup

The table below is a *measured 2026-11-07 snapshot*, not a quota guarantee.
Actual allocation varies with future selected builds and Python validation.
`build/match-linked` and source-pinned complete cold witnesses are preserved;
the compact exact ledgers are tracked and do not depend on rejected variant
binaries.

| Directory | Allocated bytes at the cleaned checkpoint | Retention |
| --- | ---: | --- |
| `build/gpt-web/` | **152,375,296** (145.3 MiB) | Rejected variants pruned; current full owner/cold witnesses retained |
| `build/match-linked/` | **127,270,912** (121.4 MiB) | Entire canonical graph preserved |
| `.analysis/archive/` | **216,694,784** (206.7 MiB) | All historical compressed evidence retained |
| `.analysis/gpt-web/` | **58,744,832** (56.0 MiB) | Private source variants, scripts, output JSON and logs retained |
| `build/pycache/` and local `__pycache__/` | **0 in the cleaned local cache roots** | Only ephemeral Python bytecode removed; regenerates automatically |

**Actual native deletion:** 1,342 ignored `.exe/.obj/.pdb` files from
**26 explicitly inspected rejected/superseded variant families**, totaling
**74,380,757 payload bytes** and **76,615,680 allocated bytes** (73.1 MiB).
This batch created **no replacement archive** so that storage is really
reclaimed. Each deletion path/size/SHA-256 is recorded in the private
`.analysis/maintenance-handoff-20261107/native-deleted.jsonl`, SHA-256
`314ef076e4ff105628ae88ab8d33f61b33bb3f71c1f547a384fb247958c8e2d9`.
The exact plan, retained variant names and one-time guarded driver remain
under `.analysis/maintenance-handoff-20261107/`. A hash list establishes
*which generated file was removed*, **not** a restorable binary backup.

To revisit a rejected variant, regenerate it with the retained
`.analysis/gpt-web/` source/driver and pinned VC7.1 compiler; do not assume
an old MAP/PDB pair is valid for a new current source. Retained MAP/log/JSON,
source snapshots and experiment scripts are **not** proof of exact acceptance.

**Local Python cache deletion:** 289 ignored `.pyc/.pyo` files, totaling
**5,033,902 payload bytes**; 60 empty cache directories removed. The measured
allocation reduction across `build/`, `scripts/` and `.analysis/gpt-web/`
was **5,894,144 bytes** (5.6 MiB). Python validation recreates bytecode;
future CI/experiments may change the live cache size. The private
`.analysis/maintenance-handoff-20261107/cache-result.json` records the
bounded before/after footprint. Cache contents are not archived or necessary
for verification.

After the final seven-stage CI, Python had automatically regenerated
**248 cache files**; these were immediately removed again and documented in
`.analysis/maintenance-handoff-20261107/cache-post-ci-result.json`.
This is **not a second independent storage saving**: it simply leaves the
working tree with no local Python bytecode at the measured checkpoint.

**Combined new cleanup:** **1,631 files**, **79,414,659 payload bytes**, and
**82,509,824 bytes** (78.7 MiB) of independently measured native-plus-cache
allocated reclamation. Accounting for the retained private cleanup manifests,
actual net allocation in the measured `build/` + `.analysis/` + `scripts/`
directories fell **82,108,416 bytes** (78.3 MiB).
The live folder snapshot after cache pruning was: `build/`
**280,932,352 bytes**, `.analysis/` **287,662,080 bytes**, and `scripts/`
**1,658,880 bytes**. All sizes are source-epoch-specific and will grow as
compilers, CI or research run. No extra archive was created.

**Directly verified exclusions:** original `resources/th10.exe`, `.tools/`,
`_reference/`, `ghidra-project/`, `.analysis/archive/`,
**all `build/match-linked/`** (including six-source statistics canonical),
ANM capture and statistics exact cold witnesses, statistics record
`installed-cold` native proof, current real-caller witness and private source
investigation. No target input, Git history, source, config, tracked evidence,
original data, scripts or archived native member was pruned.

The [previous 2026-11-06 cleanup](evidence/agent-handoff-maintenance-20261106/README.md)
archived 731 superseded native files and reported an older 110.2 MiB
`build/gpt-web` snapshot. That figure is *not* current; native experiments
later rebuilt the directory. Likewise
[earlier 367-member archive](evidence/handoff-storage-20261106/README.md)
belongs to its own historical event. Both archives and their restoration
instructions remain valid and protected. The new deletion batch is separate
and intentionally unarchived.

[2026-11-07 cleanup summary](evidence/agent-handoff-maintenance-20261107/README.md)
and [machine-readable scope](evidence/agent-handoff-maintenance-20261107/cleanup-summary.json).

## Private archive registry

These ignore-listed archives are **not included in Git**. Verify actual
SHA-256 after transfer; all sizes/identities below are as of 2026-11-06.

| Private archive (`.analysis/archive/`) | SHA-256 | Provenance |
| --- | --- | --- |
| `gpt-web-handoff-controls-20261106.tar.zst` | `574dcaa16c912444b241974f8b43593f3ef5ae2f8b181e6a86064b2ba70b44ec` | Handoff controls: 731 verified native members, **14,075,697 bytes**; [cleanup receipt](evidence/agent-handoff-maintenance-20261106/README.md) |
| `gpt-web-rejected-native-20261106.tar.zst` | `cd9d92d88606ca46060eed0154c140d3971e6bfdcb6436b9db05df31a3992ed5` | Earlier verified 367-member native trial archive, **3,158,740 bytes** |
| `gpt-web-transient-frontiers-20261105.tar.zst` | `d99f7572d6d1ab66efac495c5091c32b5b4499bc3ea9102fbd7f26b004d130a6` | [Nov 5 current-source-era cleanup](evidence/handoff-current-cleanup-20261105/README.md) |
| `gpt-web-regenerable-experiments-20261105.tar.zst` | `c3c38c20fa7c6f303911973988b4582f0d012a62f0dd2c9fbde24fcc9f03ebde` | [Earlier selected archive](evidence/handoff-storage-20261105/README.md) |
| `gpt-web-extra-graphs-20261105.tar.zst` | `b62afedcc09bc2f4e0dbe0724824afa19d71642df4e2fc95828b9f630c50301c` | [Handoff maintenance](evidence/handoff-maintenance-20261105/README.md) |
| `historical-compiler-graphs-20261104.tar.zst` | `7917cbed088dbff5d12ec914e33975c79fc9999bc6ded0266b94da0a8472e513` | [Nov 4 detailed cleanup](evidence/handoff-storage-20261104/README.md) |
| `historical-analysis-20261028.tar.zst` | `b7cf15be7f5f243e6b5bc0aeb9a0fa69924da3364dac8e27e5390aff27426e67` | Earlier private analysis source-epoch archive |
| `terminal-experiments-20261030.tar.xz` | `d1fb67af025897e2facf375b8f6e1a62646bd0fcea303a95586a791befd2179c` | Earlier terminal experiments |
| `terminal-experiments-20261031.tar.xz` | `8b2b1c573c499083cd632691d9085b5e17307dea922c9d4ae0303e698964e917` | Earlier terminal experiments |

The **previous archived-member** manifest is
`.analysis/maintenance-agent-handoff-20261106/native-pruned-files.jsonl`
(SHA-256 `83f18f7cce1d2cb3d59d897ca596a0abc000775ec113dbf20b6d334fa7f48dd9`).
The private driver is `.analysis/maintenance-agent-handoff-20261106/prune-native.py`;
it is single-use and refuses an existing archive/receipt. Do not rerun cleanup
blindly. The earlier 367-member manifest/driver remain under
`.analysis/maintenance-next-agent-20261106/` for that older archive.

## Selective restoration

From the repository root, restore outside live native artifact directories:

```sh
zstd -t .analysis/archive/gpt-web-handoff-controls-20261106.tar.zst
mkdir -p .analysis/native-restore-handoff
zstd -dc .analysis/archive/gpt-web-handoff-controls-20261106.tar.zst |
  tar -xf - -C .analysis/native-restore-handoff -- \
    build/gpt-web/20261106-replay-initialize/continues-snapshot/source.pdb
```

Check the member hash/size against the private native manifest. The archive
hash is in the registry above. A restored old experiment is still bound to its
old source/profile/context, not the current code. Older archives have separate
dated manifests; consult their evidence before restoring them.

**2026-11-07 pruning has no archive and cannot be restored by these commands.**
Its private JSONL is an audit manifest, not file content; recompile from the
retained real source and experiment driver/profile. Canonical artifacts and
newest accepted cold proofs were never pruned.

Future pruning is limited to verified regenerable ignored experiment PE/OBJ/PDB
and Python caches under [AGENTS.md](../AGENTS.md). Preserve unresolved complete
witnesses, canonical artifacts, original inputs, tools, databases and archives.
Record deleted payload, retained evidence and measured net storage. A list of
hashes alone is never an archive. Do not cold-replay the historical cohort to
validate a documentation or storage cleanup.
