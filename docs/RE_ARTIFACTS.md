# TH10 private artifacts — next-agent storage and recovery

**Current snapshot: handoff maintenance, 2026-11-06, after source checkpoint
`f3a6d2f`.** Use [RE_HANDOFF.md](RE_HANDOFF.md) for source pins/frontiers and
[PROGRESS.md](PROGRESS.md) for counts. Earlier storage reports describe their
own cleanup events; their directory sizes are not current inventories.

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

## Current expanded storage and this cleanup

| Directory | Allocated storage at this snapshot | Policy |
| --- | ---: | --- |
| `build/gpt-web/` | 115,552,256 bytes (110.2 MiB) | Current/first-exact witnesses retained; selected superseded native files archived |
| `build/match-linked/` | 126,160,896 bytes (120.3 MiB) | Keep every canonical artifact |
| `.analysis/archive/` | 216,694,784 bytes (206.7 MiB) | Keep all verified archives |
| `.analysis/gpt-web/` | 52,469,760 bytes (50.0 MiB) | Keep research source snapshots, drivers and receipts |
| `build/pycache/`, `scripts/__pycache__/` | Removed at cleanup | Regenerate on Python use |

This event removed **731 ignored `.exe/.obj/.pdb` files** from 39 explicitly
selected rejected/superseded experiment directories: **65,780,410 payload
bytes**. Their actual compressed archive is **14,075,697 bytes**;
all 731 member hashes/sizes and an isolated PDB restoration were verified
before deletion. Maps, logs, JSON, source snapshots and experiment drivers
remain. Pruned experiment directories can therefore be partial; restore their
native members before inspecting an old image. Current 26/36 replay-init,
ANM/ECL/Enemy, camera, box/circle and first-exact witnesses remain expanded.

Original Python caches removed: **531 files /
9,832,764 payload bytes**. Caches generated solely by the
maintenance validation are removed separately. Cache hashes are a deletion
inventory, **not** a restorable archive; normal Python execution regenerates
them. No compiler or game replay is needed for that regeneration.

Measured `build` + `.analysis` reduction is **63,229,952 allocated bytes
(60.3 MiB)**, including the new archive and cleanup metadata. The measurement
starts after the protection inventory and before archive creation/cache
pruning. This is one event's net result; do not add historical cleanup savings.
[Summary, selected directories and retained witnesses](evidence/agent-handoff-maintenance-20261106/cleanup-summary.json).

The earlier [Nov6 cleanup](evidence/handoff-storage-20261106/README.md)
archived 367 files/24,327,745 payload bytes and removed 247 cache files. Its
68,411,392-byte `build/gpt-web` snapshot and 28.9 MiB post-archive reduction
belong to that earlier event. They are superseded as current storage values.

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

The latest native manifest is
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

Future pruning is limited to verified regenerable ignored experiment PE/OBJ/PDB
and Python caches under [AGENTS.md](../AGENTS.md). Preserve unresolved complete
witnesses, canonical artifacts, original inputs, tools, databases and archives.
Record deleted payload, retained evidence and measured net storage. A list of
hashes alone is never an archive. Do not cold-replay the historical cohort to
validate a documentation or storage cleanup.
