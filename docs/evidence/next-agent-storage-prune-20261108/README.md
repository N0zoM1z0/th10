# TH10 2026-11-08 next-agent native-trial storage reclamation

The operator requested handoff preparation, dated-report corrections, and
space reclamation. No C/C++ source, canonical ledger row, target executable,
or tracked native artifact was modified by this storage-only cleanup.

## Actual measured one-time deletion

A reviewed one-time script
[`scripts/prune-next-agent-native-20261108.py`](../../../scripts/prune-next-agent-native-20261108.py)
was committed *before* applying. It defaults to dry-run, and `--apply`
requires a clean Git worktree and refuses to run a second time. It has
an explicit allowlist of **57 old exploratory/rejected/superseded families**
under the ignored `build/gpt-web/`, not a blanket `build/` delete. Only
regenerable `.obj`, `.exe`, and `.pdb` binaries were eligible. Accepted
new Sound/text cold witnesses, the current ANM baseline, all source snapshots,
trial scripts, JSON/MAP/log evidence, and the entire canonical graph were
excluded.

| Item | Measured result |
| --- | ---: |
| Reviewed root families | 57 |
| Removed native experiment files | **3,786** |
| Removed binary payload | **268,831,202 bytes** |
| `build/gpt-web/` before | **572,641,280 allocated bytes** |
| `build/gpt-web/` after | **297,750,528 allocated bytes** |
| **Disk allocation reclaimed** | **274,890,752 bytes (about 262.2 MiB)** |

**No replacement archive was created.** This avoided increasing storage
consumption; the files were rebuildable diagnostics, not original inputs.
The private ignored deletion inventory is
`.analysis/maintenance-next-agent-20261108/removed-native.jsonl`, **3,786**
records, each with relative path, payload bytes, allocated bytes and SHA-256.
Manifest SHA-256:
`a309a6e1c748e2a388a4d5c1f3a0f54f8f3600018e1cb27edc187a0c652a539f`.
The full private receipt, including protected witness identities,
protected directory allocation and each allowlisted-family count, is
`.analysis/maintenance-next-agent-20261108/cleanup-result.json`.
The tracked [summary](cleanup-summary.json) records the measured figures
for normal Git-clone handoff but not the private source or binary bytes.

## Immutable or protected resources

The cleanup verified before and after SHA-256 for the original
`resources/th10.exe` and representative canonical SoundInitBuffers,
TextRenderMainHybrid, EclVmHostRunEnemy and BulletManagerUpdateCallback
PEs. It also verified identical directory allocations for all of
`build/match-linked/`, `.analysis/archive/`, `.tools/`, `_reference/`, and
`ghidra-project/` and did not modify them. After deletion, independently
original-bound comparisons on retained canonical graphs still report:

| Original-bound owner | Complete bytes and fields |
| --- | ---: |
| SoundPlayerView::OpenDiskBgm | 241/241, 10 fields |
| SoundPlayerView::LoadBgm | 298/298, 12 fields |
| SoundPlayerView::ProcessQueues | 1,452/1,452, 43 fields |
| TextHelperView::RenderTextToTexture | 554/554, 35 fields |
| BulletManagerView::UpdateBullets | 249/249, 6 fields |
| EnemySpawn | 577/577, 12 fields |
| EclVmScriptDatabase::FindSubroutine | 136/136, 0 fields |

These are **retained already-exact guard checks**, not newly admitted exact
owners; no fresh historical all-unit cold replay was run. The full compiler
build graph, native i386 product runtime and original TU authorship remain
separate and unresolved questions.

## Rebuilding an old experiment

1. Find the trial driver and source snapshots in ignored
   `.analysis/gpt-web/`; get the relevant old source/header hashes and
   original parameter/profile from the dated evidence.
2. Verify the original target and the pinned VC7.1 SP1 toolchain. Do not
   confuse an old fixed-COFF result with a real LTCG caller graph.
3. Re-run the **selected** experiment into a new private output directory.
   Do not overwrite `build/match-linked/` or claim the old SHA as your new
   PDB/PE identity.
4. For actual source changes, cold replay only the affected canonical
   `--unit` recipes and check their complete target-bound fields.

A SHA record proves what was deleted; it is **not a restore medium**.
Dated evidence links to the pruned `build/gpt-web/<family>/*.pdb` or
`*.exe` are historical reproduction paths, **not proof those binaries
remain present**. Avoid treating them as current evidence until rebuilt.
See the updated [artifact policy](../../RE_ARTIFACTS.md) and
[current next-agent resume](../../NEXT_AGENT_20261108.md).
