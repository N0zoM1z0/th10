# TH10 — next agent's quick resume (2026-11-08)

**Read this first**, then [the source-pinned handoff](RE_HANDOFF.md), the
[selected canonical unit](../config/match-units.toml), and the specific
[evidence](evidence/) for the function you plan to change. This is a live
handoff of the source state checked after source commit `c76397f`;
historical notes may
refer to different source and compiler epochs. Read `git status` and the
actual current source rather than trusting any dated candidate score.

## What is established

- **Original:** Japanese TH10 v1.00a `resources/th10.exe`, 487,936 bytes,
  SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
  This original is read-only, ignored by Git, and **not** a rebuilt product.
- **Compiler:** pinned MSVC 7.1 SP1 i386 via `scripts/repo-python` and the
  real TU-specific `/GL` recipes. Ordinary COFF and linked LTCG **are not
  interchangeable**: original register ABI and stack slot scheduling
  depend on the actual caller graph.
- **Ledger (verified 2026-11-08):** **1,501 complete exact functions**,
  **197,060 exact original bytes**; **591 confirmed authored exact**,
  **126,944 authored exact bytes**, **159 authored source-present nonexact**.
  [PROGRESS.md](PROGRESS.md) is *generated*: do not hand-edit its totals.
- **Newest genuinely admitted exact owners:**
  `SoundPlayerView::OpenDiskBgm @0x0043D690`, **241/241 and 10/10 original
  fields**, [sound proof](evidence/sound-open-disk-bgm-early-guards-20261108/README.md);
  `TextHelperView::RenderTextToTexture @0x00437DB0`, **554/554 and 35/35
  original fields**, [text proof](evidence/text-render-main-stack-scope-20261108/README.md).
  Nearby source-semantic commits preserve already-exact functions but do
  **not** increase ledger counts.
- **Not established:** matching original C++ source spellings/TUs, a complete
  native i386 product, runtime-equivalence, or portability.

## Resume protocol

```sh
git status --short --branch && git log -8 --oneline
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-handoff-pins.py
scripts/repo-python scripts/ci.py
```

Treat **dirty modifications as the current work to continue**; do not blindly
reset, stash or discard them. Only `gpt-web: ...` commit subjects are used for
new source/evidence progress. Do not push without a separate instruction.

Before an original-codegen claim, compare the **complete** PDB/COFF owner,
actual ABI, all original-bound DIR32/REL32 fields and addends, local jump
and table destinations, and original bytes in the correct graph. Require two
independent current-source cold builds for *new* exact ledger admission.
Same-length functions and candidate-only relocation geometry are **not**
exact proof. Recompute SHA pins if changing actual source files. Experiment
scripts under ignored `.analysis/gpt-web/` are recipes, not authoritative
current PDBs.

**Do not run full historical cold replay.** Name just affected units:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit sound-open-disk-bgm --unit sound-load-bgm \
  --unit sound-process-queues --json
scripts/repo-python scripts/progress.py --check
```

For docs/storage-only changes, `scripts/repo-python scripts/ci.py` and
`git diff --check` suffice. `build/match-linked/` is the retained canonical
build graph; it does not prove a runnable reconstruction.

## Highest-value unresolved work

| Function / original address | Original / maintained size | Current validated limit |
| --- | ---: | --- |
| ANM `ExecuteScript @0x0043EE30` | 9,964 / 9,964 | **9,962/9,964**, all **350** original fields; opcode 91 EDX vs ECX at `+0x113,+0x115` |
| ECL `Run @0x0044E1A0` | 7,020 / 7,020 | **6,997/7,020**, all **189** original fields; x87/FORMAT stack homes and control-flow bytes |
| Enemy ECL dispatcher `@0x0040E770` | 14,416 / 14,416 | NONEXACT; **1,252/11,536** comparable bytes, 720 candidate linkage fields (not independently original-bound); 181-byte opcode map matches retail; rank-five float result tail restored; [proof](evidence/enemy-ecl-rank-five-tail-20261108/README.md) |
| EnemyAdvanceMotion `@0x0044C2A0` | 171 / 168 | NONEXACT; **133/140** ordinary bytes, all **7** original-bound fields; final stack cleanup differs at `+0xA1..+0xA7`; [proof](evidence/enemy-advance-motion-x87-polar-20261108/README.md) |
| EnemyRuntimeUpdate `@0x0040DC80` | 2,412 / 2,412 | NONEXACT; **1,582/2,136** comparable bytes, 69 candidate linkage fields; X/Y/Z base-before-offset x87 order; Z spill now matches `ESP+0x24`; remaining differences start at `+0x49D`; [proof](evidence/enemy-runtime-world-vector-aggregate-20261108/README.md) |
| Bullet `UpdateAbsoluteDirectionChange @0x004078E0` | 335 / 335 | **333/335**, all 8 original-bound fields |
| Sound `PreloadBgm @0x0043D7D0` | 369 / 369 | **365/369**, all 10 original-bound fields |
| Memory PBG `Seek @0x004364F0` | 107 / 107 | **100/107** with zero relocations |
| Text `RenderTextToTextureAlternate @0x00437FE0` | 509 / 509 | **193/381** non-relocation-byte match; still nonexact |

For other real bosses (Replay, Player, stage, laser, game runtime), see
[RE_HANDOFF.md](RE_HANDOFF.md) and its evidence links. **Old candidate
reports can be wrong.** Verify source epoch and target disassembly first.

## Storage and continuation

**Retain** original EXE, pinned `.tools/`, `_reference/`, `ghidra-project/`,
all `.analysis/archive/` compressed history, tracked sources/config/docs,
and all `build/match-linked/` canonical images. These private inputs are
ignored and need separate transfer if the next agent runs on another
machine. They do **not** accompany an ordinary Git clone.

Two bounded cleanups have pruned regenerable native trial binaries from
`build/gpt-web/`. The first removed 3,786 files from 57 roots (274,890,752
allocated bytes). The second removed **12,051** `.obj/.exe/.pdb` files from
448 explicitly reviewed roots, reclaiming **886,214,656 allocated bytes
(about 845.2 MiB)**. `build/gpt-web/` is now **262,840,320 bytes
(250.7 MiB)**, down from 1,149,054,976 bytes before the second pass.
Three selected enemy witness graphs, 12 nested `installed-cold` native
files and three root-level `.obj` inputs remain. `.analysis/gpt-web/`
drivers/source snapshots/JSON remain; retained `build/gpt-web/` MAPs/logs
remain. See the tracked
[second-pass cleanup evidence](evidence/next-agent-storage-prune-20261108b/README.md)
and [artifact registry](RE_ARTIFACTS.md). The private per-file SHA manifest
is `.analysis/maintenance-next-agent-20261108b/removed-native.jsonl`; it is
an audit, not a backup. **Both one-time `--apply` scripts have already
run; do not rerun them.** Historical PDBs deleted in these passes must be
rebuilt from their retained driver/source snapshot before use.
