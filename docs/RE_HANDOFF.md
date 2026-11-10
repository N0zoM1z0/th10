# TH10 reconstruction — current agent handoff

**Updated 2026-11-10.** Read [AGENTS.md](../AGENTS.md), then the selected owner's
source, ledger rows and comparison recipe. The latest exact source checkpoint
is `558e58a`; later storage/documentation cleanup does not change exact credit.

## Resume and trust sources

Start with `git status --short --branch` and `git log -8 --oneline`.
Preserve and continue any dirty/untracked work. Keep commits local; never push.

The only target is the Japanese TH10 v1.00a executable at `resources/th10.exe`,
487,936 bytes, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Use pinned VC7.1 SP1 and the source-specific /GL graph. Git does not carry
the ignored target, private toolchain, Ghidra project, reference checkout,
current native contexts. [RE_ARTIFACTS.md](RE_ARTIFACTS.md) lists the retained
private inputs and five unresolved compiler contexts.

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

Recent accepted owners include Ending LoadAnimation **126/126**
(`ending-text-load-animation`, seven target-bound fields), Sound LoadSoundSample
**684/684** (`sound-player-load-sound-sample`, 25 fields), and the two input
merge functions **37/37** and **43/43** (origin indeterminate).

Ending needed two natural C++ fixes: obtain the auxiliary VM ID **after**
resource publication to emit the in-place flags AND, then directly read the
serialized script DWORD so VC7.1 selects target ECX/ADD. Two independent selected
source cold replays passed all seven address fields.
EndingTextVmView::Run remains **1568/1568** and its 18-byte callback exact.

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

## Metadata follow-up

`validate-tracking.py` reports an existing name mismatch at `0x00401DE0`:
`functions.csv` says `AnmVmInitialize`, while the source mapping says
`AnmVmView::Initialize`. This cleanup leaves those names and all exact extents
unchanged; reconcile them against the maintained source and unit when next
working on that owner.

## Next useful experiments and checks

1. Choose a complete **boss and/or leaf** with a specific offset-level
   prediction; inspect current source, `config/functions.csv`, original
   disassembly, match unit and relevant `_reference/th10-decomp` hypothesis.
   Never treat the reference project or a previous assumption as target truth.
2. Try a *few* natural C/C++ expressions, keeping source variants, scripts,
   temporary diagnostics under ignored `.analysis/gpt-web/` and scratch
   compiler outputs under `build/gpt-web/`. Avoid redoing known neutral sweeps.
3. Install a promising variant, replay **only affected exact units** and
   original-bound complete nonexact parents. Record useful findings and remove
   superseded trial files. New acceptance needs independently
   confirmed exact source-linked builds. Commit real progress as
   `gpt-web: ...` with AGENTS.md's author/trailer. Never push unrequested.
4. Wine/link.exe occasionally emits LNK1104 or a page-fault message against
   different support objects. A failed link is **not** a byte mismatch and
   supplies **no** exactness receipt. Retry an isolated selected unit serially.

Useful focused private drivers (not Git-tracked):
`.analysis/gpt-web/20261110-anm-child91-aggregate-initializers.py`;
`.analysis/gpt-web/20261110-ecl-format-cursor-allocation-schedule.py`;
`.analysis/gpt-web/20261110-bullet-absolute-speed-liveness.py`;
`.analysis/gpt-web/20261110-seek-tail-control-flow-trials.py`.

Historical archives, old reports and duplicate handoffs have been removed.
Use current unit recipes to reproduce accepted comparisons; retain only the
specific unresolved contexts listed in [RE_ARTIFACTS.md](RE_ARTIFACTS.md).
