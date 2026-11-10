# TH10 reconstruction — current agent handoff

**Updated 2026-11-10 after commit `558e58a`.** This is the current continuation
brief. Read [AGENTS.md](../AGENTS.md) first. The
[November 9 handoff history](RE_HANDOFF_HISTORY_20261109.md) and
[November 9 quick resume](NEXT_AGENT_20261109.md) describe an earlier source
epoch. Their exact counts and experimental images are **historical**.

## Resume and trust sources

At this checkpoint `main` was **clean, 37 commits ahead of `origin/main`**.
Newest commit: `558e58a gpt-web: Recover exact 126-byte Ending animation loader`.
Start with `git status --short --branch` and `git log -8 --oneline`;
inspect and continue any new dirty/untracked work rather than discarding it.
Local commits have **not** been pushed.

The only target is the Japanese TH10 v1.00a executable at `resources/th10.exe`,
487,936 bytes, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Use pinned VC7.1 SP1 and the source-specific /GL graph. Git does not carry
the ignored target, private toolchain, Ghidra project, reference checkout,
native witness images or historical archives. Read [RE_ARTIFACTS.md](RE_ARTIFACTS.md)
if transferring machines.

The authority for accepted exactness is `config/matches.csv`; generated
[PROGRESS.md](PROGRESS.md) is the count authority. A `functions.csv` status,
local PDB similarity or candidate-only relocation normalization is insufficient.
A new exact owner needs the complete target function extent, all original-bound
addresses/local destinations, correct compiler graph and reproducible selected
cold links. **Never run the historical full all-unit cold replay.**
Prefer `scripts/repo-python scripts/replay-exact-units.py --unit UNIT --json`
for affected source and its known exact consumers. Run lightweight CI only at
a source/metadata checkpoint. No compile/replay is necessary for docs-only edits.

## Canonical checkpoint

At `558e58a`, `matches.csv` and `PROGRESS.md` agree:

| Metric | Current |
| --- | ---: |
| Exact functions | **1,514** |
| Exact codegen bytes | **199,496** |
| Authored-origin exact functions | **601** |
| Authored-origin exact bytes | **129,209** |
| Authored source-present nonexact backlog | **149** |

The old `91bf803` handoff was **1,510 / 198,606**. Four subsequent admitted
owners added **890 bytes** (two authored, two origin-indeterminate):

| Target | Exact size | Unit and evidence |
| --- | ---: | --- |
| Ending LoadAnimation `0x0040C3C0` | **126/126** | `ending-text-load-animation`; 7 target-bound fields; [proof](evidence/ending-loadanimation-126-exact-20261110/README.md) |
| Sound LoadSoundSample `0x0043D390` | **684/684** | `sound-player-load-sound-sample`; 25 fields; [proof](evidence/sound-sample-exact-20261109/README.md) |
| MergeDirectButton `0x0044A130` | **37/37** | `input-merge-direct-button-0044a130`; origin indeterminate |
| MergeWinMmButton `0x0044A160` | **43/43** | `input-merge-winmm-button-0044a160`; origin/private ABI provisional |

Ending needed two natural C++ fixes: obtain the auxiliary VM ID **after**
resource publication to emit the in-place flags AND, then directly read the
serialized script DWORD so VC7.1 selects target ECX/ADD. The old 131-byte
loader first became 126 bytes with 3 differences, then **126/126**.
Two independent selected source cold replays passed all seven address fields.
EndingTextVmView::Run remains **1568/1568** and its 18-byte callback exact.
Do not repeat the historical 123/126 frontier as current.

Sound's 684-byte match required the natural FindWaveChunk argument order
`(data, chunkSize, remaining, tag)` in the WinMain /GL graph. Its 71-byte
scanner and 57-byte setup thread also remain exact.

Earlier accepted representatives: ANM LoadTexture **243/243**, LoadTextureRegion
**320/320**, loaded-script start **203/203 at 0x43E710** (selected two-TU
context only), loaded VM InitializeVm **217/217**, enemy laser update callback
**91/91**, ReplayManager::SaveReplay **1681/1681**. Do not double-count these.

## High-value nonexact owners

These are *selected current-source comparisons*, not proof of a runnable game.
Changing a LTCG graph can change both registers and function extent.

| Owner / maintained source | Candidate | Remaining issue |
| --- | --- | --- |
| ANM ExecuteScript `0x43EE30`, `src/AnmManager.cpp` | **9962/9964**, 350 original fields | Opcode91 renderLayer target EDX vs candidate ECX at +0x113/+0x115. Aggregate, volatile, expression and parameter-order controls have repeatedly been neutral or regressed. |
| ECL Run `0x44E1A0`, `src/EclVm.cpp` | **6997/7020**, 189 fields | 23 byte differences: early stack addressing, x87 length/point-angle homes, FORMAT cursor/scratch/argument slots. Use full Run diagnostic, not 146-byte host entry as a substitute. |
| Bullet UpdateAbsoluteDirectionChange `0x4078E0` | **333/335**, 8 fields | EDX target vs EAX candidate at +0xD7/+0xDD. Speed/timer/alias/union/angle-lifetime trials failed. |
| Sound PreloadBgm `0x43D7D0` | **365/369**, 10 fields | Four local register bytes around ReadFile; local/output/reordered wrapper variants failed. |
| Memory-file Seek `0x4364F0` | **100/107**, zero fields | Register selection and FILE_END/FILE_CURRENT success-tail sharing. 14 early-return and other operand-order trials failed. |
| ECL AddScriptData `0x450220` | **510/589**, 2 CRT calls | Old table-count lifetime and register schedule; typed DWORD index fixes are not new byte credit. |
| Statistics factory `0x42AF20` | 98-byte candidate / 97 target | Extra push into private constructor; void/placement-new variants failed; constructor also 137/132. |
| Bullet SpawnSingleBullet `0x4067D0` | 1504-byte candidate / 1413 target | Incomplete private ABI/large source graph; 194-byte SpawnBulletPattern parent exact. |
| Spell HUD StartSpell `0x409280` | 2252 / 2432 target | Seven-stage state machine; selected two-TU leaf exact does not mean larger graph exact. |
| Enemy dispatcher / Player update / GameManager update | historical 14352/14416, 3108/3112, 1538/1544 | Rebuild the current graph before trusting old scores. |

Other current near-matches: Text InvertAlpha **112/157** zero fields; ANM
SetAndExecuteScriptIdx **249/271 ordinary** in a 287-byte candidate.
Recent source changes in Bullet/ECL/FileSystem/Statistics/Text/Transition
principally repaired i386 integer semantics while preserving exact guards;
they did **not** increase canonical counts.

## Next useful experiments and checks

1. Choose a complete **boss and/or leaf** with a specific offset-level
   prediction; inspect current source, `config/functions.csv`, original
   disassembly, match unit and relevant `_reference/th10-decomp` hypothesis.
   Never treat the reference project or a previous assumption as target truth.
2. Try a *few* natural C/C++ expressions, keeping source variants, scripts,
   MAP/log/JSON receipts under ignored `.analysis/gpt-web/` and scratch
   compiler outputs under `build/gpt-web/`. Avoid redoing known neutral sweeps.
3. Install a promising variant, replay **only affected exact units** and
   original-bound complete nonexact parents. New acceptance needs independently
   confirmed exact source-linked builds. Commit real progress as
   `gpt-web: ...` with AGENTS.md's author/trailer. Never push unrequested.
4. Wine/link.exe occasionally emits LNK1104 or a page-fault message against
   different support objects. A failed link is **not** a byte mismatch and
   supplies **no** exactness receipt. Retry an isolated selected unit serially.

Useful focused private drivers (not Git-tracked):
`.analysis/gpt-web/20261110-ending-script-index-expression.py`;
`.analysis/gpt-web/20261110-anm-child91-aggregate-initializers.py`;
`.analysis/gpt-web/20261110-ecl-format-cursor-allocation-schedule.py`;
`.analysis/gpt-web/20261110-bullet-absolute-speed-liveness.py`;
`.analysis/gpt-web/20261110-seek-tail-control-flow-trials.py`.

The latest November 10 storage pass removed obsolete trial MAP/log/RSP files
and repacked Git without losing objects or history, releasing about **569 MiB**
including Python caches. Source snapshots, JSON results, native witnesses,
canonical graphs and private inputs remain. [RE_ARTIFACTS.md](RE_ARTIFACTS.md)
records current storage and recovery; its earlier cleanup figures are historical.
Read the [historical handoff](RE_HANDOFF_HISTORY_20261109.md) only when its older
experiments directly inform the next test.
