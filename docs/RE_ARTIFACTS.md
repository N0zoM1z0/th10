# TH10 ignored artifacts — storage and recovery

**Updated 2026-11-10 after the user-approved historical-text cleanup.**
Current exact counts belong in [PROGRESS.md](PROGRESS.md); current reconstruction
work belongs in [RE_HANDOFF.md](RE_HANDOFF.md).

## Current storage

Measured with `du -s -B1` before this documentation checkpoint:

| Path | Allocated MiB | Retained contents |
| --- | ---: | --- |
| `build/gpt-web/` | **96.16** | Source snapshots, JSON results, native witnesses and their supporting text |
| `build/match-linked/` | **132.97** | Complete canonical native graph |
| `.analysis/gpt-web/` | **174.16** | Trial drivers, source variants and diagnostics |
| `.analysis/archive/` | **206.66** | Nine historical private archives |
| `.tools/` | **583.12** | Pinned compiler and game-specific Wine prefix |
| `_reference/` | **43.77** | Reference checkout |
| `ghidra-project/` | **9.62** | Private analyzer database |
| `.git/` | **18.14** | All existing objects, references and reflogs, repacked |

## Latest cleanup and retained evidence

The user explicitly extended this pass beyond AGENTS.md's default native-file
and Python-cache scope to include historical trial MAP, log and response files.
This was a one-time scope extension; historical archives remain protected.

Deleted **12,588 ignored files** under `build/gpt-web/`:

| Type | Files | Payload bytes |
| --- | ---: | ---: |
| MAP | 2,454 | 167,725,960 |
| Log | 7,641 | 41,573,786 |
| Linker/compiler response | 2,493 | 31,806,294 |
| Total | **12,588** | **241,106,040** |

The tree fell from **374,845,440** to **100,831,232 allocated bytes**, releasing
**274,014,208 bytes (261.32 MiB)**. Current comparison configuration contains no
`build/gpt-web/` input paths. Files were checked against Git's ignored and tracked
inventories before deletion. Source snapshots, trial scripts and JSON conclusions
were retained even when the associated historical MAP/log/RSP was removed.

All previously selected witness roots, all other directories still containing
native outputs, root-level inputs and nested `installed-cold`, `accepted-exact`
and `canonical-cold` contexts were excluded. **1,945 retained context files**
were individually hashed before and after deletion. These include:

- Ending's exact 126-byte loader at
  `build/gpt-web/20261110/ending-script-index/direct-member/` and its preceding
  `ending-loadanimation-lifetimes/late-pointer/` context.
- Sound's exact 684-byte loader at
  `build/gpt-web/20261109-live/sound-sample-signature/out-before-span/`.
- The selected ANM, ECL, enemy, Player HUD and laser contexts from the earlier
  November 9/10 native-output cleanups, including the complete unresolved
  larger compiler graphs.

Separately, **19 ignored Python caches** were removed: **360,532 payload bytes**,
**409,600 allocated bytes**. Lossless Git repacking with
`git -c pack.threads=1 repack -a -d --keep-unreachable` released another
**322,158,592 allocated bytes (307.23 MiB)**. The complete set of **20,570 Git
objects**, both references and **3,451 reflog entries** matched before and after;
no history expiry or unreachable-object pruning was performed.

Together these operations released approximately **569 MiB** before the small
cleanup records and documentation checkpoint. The target still hashes to
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The canonical native graph, private archives, experiment-source tree, toolchain,
reference checkout, resources and Ghidra database retained their allocations.

The compact private audit is under
`.analysis/maintenance-storage-20261110-review/`: `text-prune-plan.json.gz`
records deleted paths and sizes; `text-prune-summary.json` records the scope,
retained roots and measurements; `protected-trial-files.json` records retained
context hashes; `storage-summary.json` records Git preservation and protected
allocations. **The deletion inventory is not a backup.** No new archive was made.

## Preserve and recover

Keep the read-only Japanese 1.00a target at `resources/th10.exe`, private resource
packs, pinned `.tools/`, game-specific Wine prefix, `ghidra-project/`, `_reference/`,
`build/match-linked/`, useful source/driver/results and unresolved native compiler
contexts. These private resources are not included in a normal Git clone.
Transfer them separately when moving machines.

For a removed trial, use its retained source and driver with the pinned compiler
in a separate output directory. Historical MAP/log/RSP paths can now be absent;
old receipts describe their original source epoch rather than current disk
contents. An old document reference alone does not make a regenerable trial
output necessary to keep. Current exactness still comes from complete selected
source comparisons and `config/matches.csv`.

## Earlier cleanup evidence

Repeated historical allocation tables have been removed from this continuation
page. Their records remain in Git history and the existing evidence:

- The preceding November 10 native cleanup removed 2,973 OBJ/EXE/PDB files,
  **178,552,229 payload bytes**, releasing **173.67 MiB**. Its private inventory
  and summary remain in `.analysis/maintenance-handoff-20261110/`.
- November 9 private records remain in
  `.analysis/maintenance-handoff-20261109/` and
  `.analysis/maintenance-next-agent-20261109/`.
- [November 8 second native cleanup](evidence/next-agent-storage-prune-20261108b/README.md),
  [first cleanup](evidence/next-agent-storage-prune-20261108/README.md),
  [November 7 cleanup](evidence/next-agent-storage-prune-20261107/cleanup-summary.json)
  and [November 6 archive provenance](evidence/handoff-storage-20261106/README.md)
  retain the earlier measurements and recovery context.

The dated one-time prune scripts under `scripts/prune-*` have already been
applied. Do not rerun their `--apply` or use their fixed inventories as general
cleanup policies. Earlier statements that every MAP/log/RSP survived describe
those earlier passes; the latest cleanup above supersedes them.

## Private archive registry

These ignored archives were verified present and had SHA-256s rechecked on
2026-11-07. They remain **unmodified**; none was duplicated in this cleanup.

| Private `.analysis/archive/` filename | SHA-256 | Provenance |
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

## Selective recovery from an archive

Inspect an archive and extract only the needed member to a private staging
directory. For example:

```sh
zstd -t .analysis/archive/gpt-web-handoff-controls-20261106.tar.zst
mkdir -p .analysis/native-restore-handoff
zstd -dc .analysis/archive/gpt-web-handoff-controls-20261106.tar.zst |
  tar -xf - -C .analysis/native-restore-handoff -- \
    build/gpt-web/20261106-replay-initialize/continues-snapshot/source.pdb
```

Check the restored member against that archive's historical manifest and source
context before using it. These archives predate later deletions and cannot
restore every removed trial. Storage/documentation changes need lightweight
`scripts/repo-python scripts/ci.py` and `git diff --check`, without game rebuilds.
