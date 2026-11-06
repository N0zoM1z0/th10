# TH10 private artifacts — next-agent storage and recovery

**Current inventory snapshot: 2026-11-06.** See the authoritative
[next-agent handoff](RE_HANDOFF.md) for source SHA-256 pins and current
exactness; [PROGRESS.md](PROGRESS.md) for canonical counts. The many older
`docs/evidence/handoff-storage-*` reports are **dated historical cleanup
snapshots**, not assertions about what is expanded today. Never use an old
native PE/PDB as proof for changed source bytes.

## What must not be deleted or treated as ordinary Git files

- Original Japanese TH10 v1.00a `resources/th10.exe` (SHA-256
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`):
  ignored/read-only, never commit or replace.
- `.tools/` (~583 MiB at the snapshot): pinned VC7.1 SP1, Wine and headers.
  Without these, current native replays cannot be reproduced.
- `ghidra-project/`, `_reference/`, `resources/private/` and the whole
  `.analysis/archive/`: private analyses, original inputs or independently
  verified historical archives, **not disposable compiler cache**.
- `src/`, `config/`, `scripts/`, `docs/`, Git history and all accepted
  `build/match-linked/` canonical PE/MAP/PDB producers (~114 MiB).
- `.analysis/gpt-web/` research scripts/source snapshots/receipts and
  the current complete native ANM/ECL/Enemy witnesses documented in
  [RE_HANDOFF.md](RE_HANDOFF.md). A source pin mismatch requires rebuilding
  the scoped unit, not quietly using a stale PDB.

## What is currently expanded and what was just reclaimed

| Directory | Actual allocated storage immediately after 2026-11-06 pruning | Policy |
| --- | ---: | --- |
| `build/gpt-web/` | 68,411,392 bytes (~65.2 MiB) | Selected current witnesses and old trial JSON; failed native binaries archived when proven regenerable |
| `build/match-linked/` | 119,414,784 bytes (~113.9 MiB) | **Keep** all canonical builders |
| `.analysis/archive/` | 202,616,832 bytes (~193.2 MiB) | **Keep** all compressed historical evidence, including new archive |
| `.analysis/gpt-web/` | 44,843,008 bytes (~42.8 MiB) | **Keep** scripts, negative experiments and receipts |
| `build/pycache/` | Removed at cleanup | Regenerates when Python tools execute |

The [2026-11-06 verified cleanup](evidence/handoff-storage-20261106/README.md)
archived **367 old native experiment files (24,327,745 payload bytes)**
from 24 rejected/superseded `build/gpt-web` prefixes, then deleted only
those expanded `.exe/.pdb/.obj/.map`-family files after matching **all
367/367 archive member hashes**, verifying an actual one-member restore,
and rehashing before unlink. It also deleted **247 `.pyc` cache files**
(4,581,025 bytes). These were not source, original, canonical matches or
currently pinned whole-function graph inputs.

Immediately after archive creation, the measured `build` + `.analysis`
allocated-block reduction was **30,265,344 bytes (~28.9 MiB)**. Because
that baseline **already included the new archive**, the approximate
initial-to-final net gain after accounting for archive storage is about
**27 MB**. Do not add previous cleanup-event savings to this figure.
For all 24 directories, actual before/after disk-block measurements and
verification details see the tracked
[cleanup-summary.json](evidence/handoff-storage-20261106/cleanup-summary.json).

## Private archive registry

These ignore-listed archives are **not included in Git**. Verify actual
SHA-256 after transfer; all sizes/identities below are as of 2026-11-06.

| Private archive (`.analysis/archive/`) | SHA-256 | Provenance |
| --- | --- | --- |
| `gpt-web-rejected-native-20261106.tar.zst` | `cd9d92d88606ca46060eed0154c140d3971e6bfdcb6436b9db05df31a3992ed5` | Newly verified 367-member native trial archive, **3,158,740 bytes** |
| `gpt-web-transient-frontiers-20261105.tar.zst` | `d99f7572d6d1ab66efac495c5091c32b5b4499bc3ea9102fbd7f26b004d130a6` | [Nov 5 current-source-era cleanup](evidence/handoff-current-cleanup-20261105/README.md) |
| `gpt-web-regenerable-experiments-20261105.tar.zst` | `c3c38c20fa7c6f303911973988b4582f0d012a62f0dd2c9fbde24fcc9f03ebde` | [Earlier selected archive](evidence/handoff-storage-20261105/README.md) |
| `gpt-web-extra-graphs-20261105.tar.zst` | `b62afedcc09bc2f4e0dbe0724824afa19d71642df4e2fc95828b9f630c50301c` | [Handoff maintenance](evidence/handoff-maintenance-20261105/README.md) |
| `historical-compiler-graphs-20261104.tar.zst` | `7917cbed088dbff5d12ec914e33975c79fc9999bc6ded0266b94da0a8472e513` | [Nov 4 detailed cleanup](evidence/handoff-storage-20261104/README.md) |
| `historical-analysis-20261028.tar.zst` | `b7cf15be7f5f243e6b5bc0aeb9a0fa69924da3364dac8e27e5390aff27426e67` | Earlier private analysis source-epoch archive |
| `terminal-experiments-20261030.tar.xz` | `d1fb67af025897e2facf375b8f6e1a62646bd0fcea303a95586a791befd2179c` | Earlier terminal experiments |
| `terminal-experiments-20261031.tar.xz` | `8b2b1c573c499083cd632691d9085b5e17307dea922c9d4ae0303e698964e917` | Earlier terminal experiments |

The machine-verifiable private manifest for this latest archive is
`.analysis/maintenance-next-agent-20261106/native-pruned-files-20261106.jsonl`
(SHA-256 `b371183c1add0e8536c51fa687346d99eb6e59592b4f56d32ecd38eb02f9c194`).
The reproducible archive-and-prune driver is the retained private
`.analysis/maintenance-next-agent-20261106/prune.py`; **do not rerun it**
on a tree where its target files have already been pruned.

## Selective restoration, not replacing current native codegen

Use the actual private archive and work from the repository root:

```sh
zstd -t .analysis/archive/gpt-web-rejected-native-20261106.tar.zst
zstd -dc .analysis/archive/gpt-web-rejected-native-20261106.tar.zst |
  tar -tf - | grep '20261106-anm-op91-order' | head
mkdir -p /tmp/th10-native-restore
zstd -dc .analysis/archive/gpt-web-rejected-native-20261106.tar.zst |
  tar -xf - -C /tmp/th10-native-restore -- \
    build/gpt-web/20261106-enemy-constructor-sentinel/sentinel-first/source.map
```

Verify the restored member's hash and size against the *private manifest*.
For older compressed graphs consult their dated evidence/manifest first;
restore files **outside** live current-source artifact directories. Most
missing old negative-experiment paths can alternatively be regenerated
from the retained `.analysis/gpt-web/` trial script.

## New machine transfer and future pruning guardrails

A Git clone alone does **not** provide the legally supplied original,
compiler, Ghidra database, private archives or ignored research receipts;
arrange authorized transfer of those inputs and verify their hashes. Keep
the source epoch and canonical acceptance distinct. Do **not** run full
historical cold replay just to verify a storage cleanup. Future cleaning
should affect only proven-regenerable ignored files, with a restorable
archive, per-file checksum manifest, isolated restoration smoke test,
recorded allocated-byte savings and a tracked receipt. Never purge
`build/match-linked/`, original inputs, private archives or unresolved
critical witnesses as a shortcut.
