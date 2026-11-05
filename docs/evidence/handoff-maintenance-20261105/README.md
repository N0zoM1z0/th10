# TH10 2026-11-05 next-agent maintenance: corrected state and audited cleanup

This record accompanies [the corrected current handoff](../../RE_HANDOFF.md)
and [private artifact/restore instructions](../../RE_ARTIFACTS.md).
The latest *source-only* checkpoint before these documentation changes was
`77dc82a`; use actual Git HEAD and source hashes on the next resume.
This task made **no changes to code generation or canonical exact credit**.

## Corrections, with current-source checks

- **`EnemyBeginSpell` is no longer 2,424-byte candidate:** the real current
  full 11-translation-unit VC7.1 `/GL` owner emits **2,432 bytes**, exactly
  its original physical extent, with **108 candidate linked fields** and
  seven stage-switch targets recovered by two child-node owner reloads.
  This is still **NONEXACT**, and equality in size does not prove target
  field acceptance. The previous `f91f0e8` handoff checkpoint is historical.
- The **14,416-byte** original ECL `DispatchEclInstruction` still has a
  **14,352-byte** current native candidate and 716 candidate linked fields.
  A fresh 11-source graph was compiled from the installed source and is
  retained at `build/gpt-web/20261105-handoff-current/complete-ecl/`.
  Its unchanged physical size does not mean 14,352 matching target bytes.
- `AnmRenderManagerView::ExecuteScript` remains **9,962/9,964 original
  bytes**, with 350 independently target-bound fields (95 local), the two
  remaining byte differences at +0x113/+0x115 and **no new exact credit**.
  `EclVmContext::Run` remains NONEXACT at 7,020 candidate bytes, 189
  candidate fields and 23 ordinary target differences. The latest ECL
  `FORMAT` source includes typed *numeric* operand reads, not only the tag.
- Citable function counts remain **1,482 exact / 189,797 exact codegen
  bytes**, not sums of near-match reports; `docs/PROGRESS.md` is the
  generated authority. The older 1,478 / 189,217 values were superseded.
- The earlier November 5 statement that only 13 old witnesses (~9 MB)
  were expanded became stale after many new recompiles. The second cleanup
  below retains the original 13 plus nine selected newer witness prefixes.

[Source-hash-pinned current native result](current-native-frontiers.json)
records the full current ECL graph, including the older-candidate comparison
at ECL `+0x271F`. **Do not** treat that historical candidate difference as
an original target-bound exact result.

## Audited second cleanup of ignored compiler experiments

- Before cleanup, ignored `build/gpt-web` held **1,892 regular files**:
  **1,581** prunable candidates and **311** retained witnesses. This is a
  file inventory, not a target-function count. Every prunable member was
  independently hashed by original relative path and size.
- Their original content totaled **72,321,019 bytes**. A local
  **3,292,104-byte** zstd tar was created, then decompressed and checked
  member by member against **every** manifest SHA-256 and size before any
  file was deleted. Each original was hashed again just before unlinking.
- After guarded pruning, **311 expanded witness files** remain
  (**14,079,534 payload bytes**). The 247 regenerable Python `.pyc` files
  under `build/pycache` were also deleted; no source was deleted.
- Measured reduction in `build` plus `.analysis`, including the newly
  created archive, was **77,815,808 bytes (~74.2 MiB)**. This is the
  actual combined *before-to-after* disk-block difference; do not add the
  earlier cleanup's measured savings to this one without distinct baselines.

New private archive (ignored and **not** included in Git):

    .analysis/archive/gpt-web-extra-graphs-20261105.tar.zst

SHA-256:

    b62afedcc09bc2f4e0dbe0724824afa19d71642df4e2fc95828b9f630c50301c

Private original-path/size/SHA-256 manifest:

    .analysis/maintenance-handoff-20261105/extra-graphs-manifest.jsonl.gz

Manifest SHA-256:

    1b94eaa3df6851aaf972595b717b86cae9c2726e1993f98057a27400c8fcc2ce

For an example selective restoration without overwriting existing witnesses:

```sh
zstd -t .analysis/archive/gpt-web-extra-graphs-20261105.tar.zst
zstd -dc .analysis/archive/gpt-web-extra-graphs-20261105.tar.zst |
  tar -tf - | grep '20261105-anm-layer-probe' | head
# Extract a specific member to a scratch directory, NOT over current build:
mkdir -p /tmp/th10-restore-check
zstd -dc .analysis/archive/gpt-web-extra-graphs-20261105.tar.zst |
  tar -xf - -C /tmp/th10-restore-check -- \
    build/gpt-web/20261105-anm-layer-probe/volatile-layer-value/link-aliases.rsp
```

A real selective extraction of that member into a temporary directory was
performed and verified against the private manifest. All 1,581 archived
regular members were fully checked. The exact guarded plan, archive,
verification and original-source-hash recheck scripts are preserved under
ignored `.analysis/maintenance-handoff-20261105/`.

**Never delete or assume transferred by Git:** `resources/th10.exe`, `.tools/`,
`_reference/`, Ghidra project, prior archives, original research receipts,
`build/match-linked/` canonical producers or tracked files. A new machine
needs these protected private inputs transferred separately; the archives
are not downloadable from a plain Git clone. The tracked
[cleanup summary](cleanup-summary.json) lists all retained witness prefixes,
verified archive identifiers and measured disk totals.
