# TH10 reconstruction — next-agent continuation brief

**Updated 2026-11-06.** This is the **current operational handoff**, not a
chronological trial log. Check live `git status`, `git log`, source hashes and
the generated [progress ledger](PROGRESS.md) before acting. Historical
source-epoch notes belong in [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) and
`docs/evidence/`; a recorded image/size from an old test does **not** certify
the current source. Older versions of this handoff remain in Git history.

## Start on the real worktree

```sh
git status --short --branch && git log -8 --oneline
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/ci.py
```

- Only ignored/read-only `resources/th10.exe`, Japanese TH10 v1.00a,
  **487,936 bytes**, SHA-256
  `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`,
  is the original target. Keep `AGENTS.md` rules, pinned VC7.1 SP1 and
  actual LTCG `/GL` profiles. **Never push.**
- Every dirty change is part of this worktree: inspect and continue it;
  never reset it simply because an earlier note says the tree was clean.
- Make substantive source fixes in both large interpreters and useful leaves.
  Commits are `gpt-web: ...`, with repo author and co-author trailer.
- Accept **only complete, independently original-bound exact** code, all
  fields and branch/table targets, from a reproducible *current-source* cold
  build. Matching size, 100% normalized candidate-vs-candidate code and
  repeated guard replays confer **zero new exact credit**. Build only affected
  units using explicit `--unit` selectors. **Never cold replay all units.**

## Resume at this checkpoint

Latest accepted checkpoint: `AnmRenderManagerView::RequestCaptureRectangle`
162/162/all six original fields from two independent real three-TU cold links.
The preceding ECL, Enemy ECL, ANM and bullet word-arithmetic commits remain
source-semantic improvements with selected exact guards, not new whole-owner
credit. Always use live Git status/log; the tree may be dirty during work.

- Choose one bounded owner with a new concrete lead. The Init two-load order
  mismatch is reproduced in real 26/36-input contexts; missing file-system context
  and scalar/typed-view spelling controls are disproved leads and parked.
- ANM's opcode 91 two-byte mismatch and ECL's 23 ordinary differences remain
  open. Their current whole-function witnesses are retained below.
- Score/rank 32-bit wrapping now matches original signed-clamp semantics with
  both existing exact leaf owners preserved (see
  [word arithmetic](evidence/item-score-word-wrap-20261106/README.md)).
- Faith decay and timer previous-frame publication now likewise avoid
  signed-overflow UB, with selected original-bound native 46/46 and 59/59
  comparisons ([follow-on proof](evidence/item-faith-decay-timer-wrap-20261106/README.md)).
- Reference source is pinned at `d1f3121`; its synthetic stand-in/escape
  contexts are not local acceptance recipes. Save1681 and Ending1568 are
  already credited; do not count their guard replays as new progress.
- The FrontEnd capture owner now exact (162/162 original fields six): the
  complete result-menu caller graph needs real `AnmManager.cpp` as a support
  translation unit. Reordering destination/source rect stores alone gives
  161/162, not exact; actual FindVm linkage restores final EDX. See
  [capture proof](evidence/anm-capture-rectangle-exact-20261106/README.md).
- No full historical cold replay is needed for handoff. Rebuild only selected
  changed owners and affected exact interfaces as work resumes.

## Acceptance ledger — generated truth, not a runnable-game claim

As of this source checkpoint, [PROGRESS.md](PROGRESS.md) reports **1,829
reviewed function boundaries**, **1,309 source-present mappings**,
**1,495 complete exact functions / 195,244 original bytes**, including
**586 confirmed-authored exact / 125,154 bytes**. There are **164 authored
source-present nonexact** functions. Original author/source provenance is
not interchangeable with a reconstructed function's exact codegen. Product
build, actual game execution, semantic dual-Oracle and portability are
separate later work, **not established** by these figures.

Recent *accepted* full owners (replay only one affected cohort when needed):

| Exact original owner | Strict acceptance | Evidence |
| --- | --- | --- |
| `AnmRenderManagerView::RequestCaptureRectangle @0x00424480` | 162/162, six target-bound fields; two independent real FrontEnd/VmIdInterrupt/AnmManager three-TU colds; same-image FindVm73 and result close89 exact | [capture rectangle](evidence/anm-capture-rectangle-exact-20261106/README.md) |
| `EndingTextVmView::Run @0x0040BD80` | 1568/1568; 92/92 fields; 53 direct locals and all18 owned table words; independent 21-input cold; existing guards953/all24 pass | [ending execution](evidence/ending-run-1568-exact-20261106/README.md) |
| `ReplayManager::SaveReplay @0x00429B60` | 1681/1681; 83/83 fields; independent 21-input cold; same-image replay/file guards412/all33 and shared-artifact ReadIntOperand820 pass | [replay save](evidence/replay-save-1681-exact-20261106/README.md) |
| `EnemyLaserType1View::Draw @0x0041EA40` | 173/173; 10/10 fields; independent two-input cold; same-image angle94/renderer216 pass | [type-1 draw](evidence/laser-type1-draw-173-exact-20261106/README.md) |
| `BulletRuntimeView::UpdateVerticalWrap @0x00407E40` | 172/172; 8/8 fields; independent seven-input cold with horizontal148 guard | [vertical wrap](evidence/bullet-vertical-wrap-20261106/README.md) |
| `BulletRuntimeView::UpdateVectorAcceleration @0x00407560` | 305/305; 10/10 fields; final seven-input cold with horizontal148 guard | [vector acceleration](evidence/bullet-vector-acceleration-20261106/README.md) |
| `GameLoadStartupVersion @0x00420100` | 174/174 bytes; 15/15 fields; independent final six-TU cold, real PBG path entry | [startup version](evidence/startup-version-174-exact-20261106/README.md) |
| `AnmFloatRectView::DrawGradient @0x0043BFA0` | 501/501 bytes; 14/14 source/callee/global fields, real 14-TU context | [gradient](evidence/anm-gradient-rect-501-exact-20261106/README.md) |
| `BulletManagerView::SpawnBulletPattern @0x004073E0` | 194/194; 6/6 fields; real four-TU context | [bullet](evidence/bullet-spawn-pattern-194-exact-20261106/README.md) |
| `EnemyLaserManagerStorageView::AccumulatePointChecks @0x0041C880` | 54/54 *unmasked* bytes, zero external fields; **original authored provenance indeterminate** | [laser](evidence/enemy-laser-point-checks-54-exact-20261106/README.md) |
| `SoundPlayerView::GetFmtIndexByName @0x0043D2A0` | 227/227; four bound fields | [format-name](evidence/sound-getfmt-index-227-exact-20261105/README.md) |
| `SoundDataLoaderThread @0x0043D080` | 107/107; 11 bound fields | [sound loader](evidence/sound-loader-single-index-exact-20261105/README.md) |
| `MidiOutputView::StopPlayback @0x0043AE20` | 129/129; five bound fields; separate 30-TU selected destructor guards passed | [MIDI](evidence/midi-stop-playback-single-owner-20261105/README.md) |

The gradient **503-byte isolated** and bullet-pattern **195-byte isolated**
older candidates were superseded by exact *integrated* graph results. Laser
original authorship remains unresolved, so its 54 bytes do **not** count as
confirmed authored code.

## Current large and near-exact **nonexact** frontiers

| Target | Original vs current complete owner | What is known; next meaningful investigation |
| --- | --- | --- |
| `AnmRenderManagerView::ExecuteScript @0x0043EE30` | **9,964 / 9,964** | **9,962/9,964** original bytes, **350/350 independently original-bound fields**, 95 local targets. Opcode 91 registers at +0x113/+0x115 (original EDX, candidate ECX). Current source explicitly wraps three integer assignment and three in-place arithmetic ops; full original-bound 350-field owner stays 9962/9964 with only opcode91 register differences, and the selected four variable access guards remain 856/856 ([new proof](evidence/anm-inplace-word-wrap-20261106/README.md)). Do not retry failed aggregate, volatile, member-order or layer-lifetime permutations without a new compiler lead. [Current-source proof](evidence/anm-integer-assignment-wrap-20261106/README.md). |
| `EclVmContext::Run @0x0044E1A0` | **7,020 / 7,020** | **6,997/7,020**, **189/189 bound fields**. Remaining 23 ordinary byte differences include register/stack homes, `LENGTH_SQUARED`, `POINT_ANGLE`, FORMAT and loop/callback paths. `STORE_INT`/`STORE_FLOAT`, CALL, JUMP and stack scalar work are installed; add/subtract/multiply/negate/post-decrement now explicitly preserve 32-bit wrap without new exact credit ([proof](evidence/ecl-integer-word-wrap-20261106/README.md)). [Current full-source proof](evidence/ecl-counted-offset-cursor-20261106/README.md). |
| `EnemyRuntimeView::DispatchEclInstruction @0x0040E770` | **14,416 / 14,352** | 64-byte length gap; **716 candidate** fields. Fresh 11-TU source/PDB codegen checks are mostly candidate-vs-candidate, **not** a full independently bound target acceptance. Typed spawn record, position, word clear and bullet call are installed. Three bullet-count and one integer rank interpolation operations now use native 32-bit wrapping; real 11-TU codegen/716 fields unchanged and five related exact guards87/87 pass ([new proof](evidence/enemy-ecl-rank-word-wrap-20261106/README.md)). [Caller record](evidence/enemy-pattern-bullet-descriptor-boundary-20261106/README.md). |
| `EnemyBeginSpell @0x00409280` | **2,432 / 2,432** | **108 candidate** fields but many ordinary code differences. Source bonus arithmetic now explicitly wraps modulo 2^32 before the signed cap. Same extent is not exact. [Bonus proof](evidence/enemy-begin-spell-bonus-wrap-20261106/README.md). |
| `EclVmScriptDatabase::AddScriptData @0x00450220` | **589 / 589** | **510/589**, **2/2 original-bound fields**. Count-reference growth and read-only loop view of the actual input-parameter cursor restore0x10 frame, old-table EBP, REP MOVSD+FB and both CRT call positions. First count/fill/merge-tail register scheduling remains nonexact; no identity selector. Both count growth and old-record subtraction now use native word-wrap source expressions; full 589/2 and Run7020/189 geometry unchanged ([new proof](evidence/ecl-count-word-wrap-20261106/README.md)). Early count/whole-cursor/force-inline controls are parked. [Current proof](evidence/ecl-counted-offset-cursor-20261106/README.md). |
| `EclVmContext::StartSubroutine` | **550 / 551** | Four caller field positions preserved; direct signed/source pointer lifetime scheduling unresolved. **NONEXACT**. [CALL proof](evidence/ecl-call-counted-inline-name-20261106/README.md). |
| `BulletRuntimeView::UpdateAbsoluteDirectionChange @0x004078E0` | **335 / 335** at Vector source epoch | Last complete measurement **333/335**, **8/8 independently original-bound fields**, at BulletManager.cpp `504af18f...` before the vertical-wrap change. EAX/EDX at +0xD7/+0xDD remain the nonexact frontier; the three direction-change counter increments now preserve native unsigned 32-bit wrap with 1,394/1,394 selected exact guard bytes and the absolute owner still 333/335 ([fresh proof](evidence/bullet-direction-counter-wrap-20261106/README.md)). [Pinned proof](evidence/bullet-vector-acceleration-20261106/README.md). |
| `SoundPlayerView::LoadSoundSample` / `PreloadBgm` | **684 / 684**; **369 / 369** | Respectively **678/684** and **365/369** ordinary bytes in suitable five-TU graphs; mainly call argument register order. Many simple declaration/alias and wrapper trials already failed. [Sound wire evidence](evidence/sound-riff-chunk-prefix-20261105/README.md). |
| `PollInputFrame @0x0044A5F0` | **850 / 850** | Split immutable published return from destructive repeat cursor; fresh baseline855 shrinks to850. **807/850 bytes**, **10/10 independently bound fields**. Remaining43 ordinary differences start +0x2DD in repeat/edge registers and scheduling. Related264-byte query cold passes. XOR/countdown/wider-mask/snapshot controls give no further improvement and are restored. [Current proof](evidence/input-frame-published-mask-20261106/README.md). |
| `StageResetPublishedCameras @0x004216F0` | **465 / 533** | Restored omitted secondary/published +0xE4 dword assignments, respectively 1/0; camera layout stays 0x118. Both fresh `/GL` and normal-COFF complete owners emit 533 bytes /40 fields; vector temporary reuse and global-store scheduling remain nonexact. [Word-store proof](evidence/stage-camera-default-words-20261106/README.md). |
| `ReplayManager::Initialize @0x00428F60` | **1,344 / 1,344** in real26/36-input contexts | Construction order, real shared state/ChainElem registration and current-stage header reload restored. Full74 candidate fields; strict rejection at the selected-stage DIR32 position (+3C5 vs original+3CB), with9 diagnostic ordinary differences from two reordered loads. Create34 gives1342 because loader path promotes from stack/RET4 to ECX/RET; adding real FileSystem/PbgArchive restores1344/RET4 in36 inputs but leaves identical9 differences. Repeated scalar/typed-view controls are byte-neutral and parked. Existing Create185 now uses two real configuration-default providers; selected Create34/Save21 colds pass with no new credit. [Current source and ABI proof](evidence/replay-initialize-constructors-20261106/README.md). |
| `EnemyLaserType1View::CheckCollisionCircle @0x0041EFA0` | **1,108 / 1,178** | Real virtual slot46DA10+1C now owns the body. Unordered radial acceptance, post-add sample Z clear and radius parameter home restored; typed vectors recover0x32C frame. Current22-input real context restores the independent registration118 call; factory still expands and full30-field extent rejects original29. Request-member factory control gives157/RET8 and is restored. Existing polar30/bounds91/registration118 pass239/all4; affected Update832 and Draw173 use their own selected cold recipes. [Current proof](evidence/laser-type1-circle-owner-20261106/README.md). |
| `EnemyLaserType1View::CheckCollisionBox @0x0041EB00` | **1,171 / 1,225** in current22-input circle context | Body unchanged since the earlier four-input1209/20-field proof, which belongs to pre-circle EnemyLaser.cpp408659cf. Current broader context emits1225/20 and remains nonexact; graph-dependent sizes are not source-only regressions. Actual slot18, unordered XY rejects, post-add Z clear and0x344 frame remain. Factory type-0 branch still inlines. [Prior four-input proof](evidence/laser-type1-box-owner-20261106/README.md); [current context](evidence/laser-type1-circle-owner-20261106/README.md). |
| `EnemyLaserType0StorageView::Bounce @0x0041CFD0` | **410 / 510** at pre-box source epoch | Measured at EnemyLaser.cpp `0eb005f5...`, before the box-owner change; Bounce body is unchanged. Actual type-0 vtable46DA60+3C and Update call41D45F bind this owner. Real generated virtual ownership restores ECX/RET; typed XYZ copies and ordered Y predicates are installed. One FireLaser path inlines; factory private arguments/registers remain nonexact. Original Update-entry canonical guard1001/all31 and native polar30/sound123 pass. Bounce-as-entry is rejected (same510; Update1009). Fresh stdcall and complete manager-member controls both give factory157/RET8 with stack request; restored/parked. [Current proof](evidence/laser-bounce-virtual-owner-20261106/README.md). |
| `InitializeGameStatisticsRecord @0x0042ACB0` | **274 / 274** | Natural110-iteration kind loop shrinks fresh302/12-field source to274/8 in normal COFF and separate `/GL /GS` cold. Full address-bound diagnostic **174/274**, still NONEXACT; field positions disagree. Score copy keeps two cursors and spills group count; kind tail has two SIB differences. Literal/aggregate copies are neutral; method280 and indexed791 restore. [Current proof](evidence/statistics-record-natural-loop-20261106/README.md). |
| `TransitionShakeLinear @0x0043C550` | **439 / 439** | Fresh existing14-input `/GL` graph: **412/439**, **20/20 original-bound fields**. `FIDIV` is at +0xB6 instead of target +0xCE. Named float-product controls (outer conversion; then explicit inner/outer conversions) both move it to +0xAA and displace two fields; rejected/restored. Initializer396/all37 guard passes from the restored-source baseline image. [Reference review and pins](evidence/upstream-th10-decomp-review-20261106/README.md#local-camera-experiment). |
| `TextRenderBufferView::InvertAlpha @0x00437160` | **157 / 157** | **Current source** two-TU `/GL /GS` measures **138/157**, zero fields; EAX/ECX receiver/count/selector choice accounts for19 differences. Separate pitch capture/multiply is byte-neutral in normal COFF112/157 and GL138/157; restored. Current installed signed-overflow-free text row and geometry words preserve all ordinary codegen; fresh GL/GS still measures138/157 and selected exact text owners1799/1799 pass ([native proof](evidence/text-native-word-geometry-20261106/README.md)). An *older different source/graph* had only `JS` vs `JL` at +0x69; **that one-byte near-exact result is not current**. Private proofs: `.analysis/gpt-web/20261106-text-current-exact/` and `.analysis/gpt-web/20261106-text-alpha-owner/`. |

Updated external reference: `_reference/th10-decomp-d1f3121` preserves the
November 1 head alongside the October 18 checkout. At review start its 16
promotions mapped to 15 open local entries. SaveReplay and Ending Run are now
accepted here; 13 remain open. Prioritize the three substantial laser
cutters and ECL count-reference model using our real callees; synthetic
caller/escape/stand-in contexts confer no local exact credit. See the
[review/crosswalk](evidence/upstream-th10-decomp-review-20261106/README.md).

Other unaccepted leaves worth *new evidence-led* work: `CMemoryPbgFile::Seek`
107/107 but 100 matching ordinary bytes; `AnmLoadedView::SetAndExecuteScriptIdx`
287/287 with about 22 instruction bytes differing; `EnemyEclHostBaseView::SetSubroutine`
30/30 with register/ABI disagreement; `EnemyFullObjectView` ctor target
673 vs native 672. The 20-TU `enemy-end-spell` replay can exceed execution
window: the last two attempts produced **no receipt**, so do not report it
as having passed during that particular experiment.

## Current-source proofs and candidate epoch pins

These SHA-256 values identify the *installed source bytes*, not historical
archive images. Recheck after any source change. Current-source/replay
receipts are ignored private files, not Git attachments.

| Installed source | SHA-256 |
| --- | --- |
| `src/FrontEnd.cpp` | `0bab7e7448c0762a1410d627301652bfc8cfe22e7aaa88b76e979151cdaaf8c6` |
| `src/AnmManager.cpp` | `e4c4e24b4e7c3879e2a88265c161867a3ea8db5dcfe6d9abc6bf0d833beb0707` |
| `src/AnmManager.hpp` | `3adee93a8eaec70e0b7d469e96271126a5a631b6a44e5e17199892acd2dc9667` |
| `src/EclVm.cpp` | `3b4af08fec32d38a1323575365bdbbb9e211afef25adfc1d0ad6de881ebc8c7b` |
| `src/EclVm.hpp` | `b7f6c877aff0fb70ac53f4b55b3e5da5be0b6032532eb3b983d97604ae93fe52` |
| `src/Ending.cpp` | `ed016273f068605e633f3faaff6a89556c953ec3a30b724ec52b8c54706a23ff` |
| `src/EnemyEclDispatcher.cpp` | `7d30a9b073172e0e1867972a0530d92400a9f6f432d81c17214e815b71e31471` |
| `src/Enemy.cpp` | `c08272bf324217b9e6e4ffa91cd13fb2555aeb37b44be6231075082ebfcac5d2` |
| `src/Enemy.hpp` | `2e2168b1d84e6dab64abb51c2d6dcdf0498c6f36856c9b1a25f18637ed35eea2` |
| `src/EnemyLaser.cpp` | `7d5f898f3fe4718ec4d44637b71883f300cd9493771ff2296538baf986adead7` |
| `src/EnemyLaserType1.hpp` | `dfd675f59897f541739888e99067a4306fcbe67b6773ec184b8baca90984facc` |
| `src/LaserType1.cpp` | `b7f759886f4897a9c1e2cd8fb99007f045e932d7a3826417ecd194f9006ead51` |
| `src/ItemManager.cpp` | `b029539ab91eba926525a1de3dcb6baaa05d3b6c53e181702b325b8a4ad32fcc` |
| `src/BulletManager.cpp` | `ddb77dc65b7e1ae9629d16b64fee178cff743f3192ce95617d8dcbcf64a34a63` |
| `src/Sound.cpp` | `cb5cdabd1732dfbd93d6da73938df42dc541356fe7c34fd20a33950576b5b3b0` |
| `src/StageCameraDefaults.cpp` | `4ac0d9c1eeb14e1763f5ed642e50895e5081fdf2f41dcda819a6ca7ee7255fb8` |
| `src/StageScript.hpp` | `3a1a5402fdd969c150844102680d007f1080f6e38e44f429e70f0594b537bf5b` |
| `src/GameStartupVersion.cpp` | `125b80a073015e9d0fd2694b989a0983bf3f3777fb29634825818ced0f125b4f` |
| `src/GameStatisticsRecordDefaults.cpp` | `bd62bc5eb20ab8346d8b81d724cd6db90050ed75816e05bd406e47c3711b75d9` |
| `src/InputPolling.cpp` | `f3bb9e5230f9db7e141e187b054e9bbd68d1282c7cd27f02723176f4415d641f` |
| `src/ReplayManager.cpp` | `7c6a50d945a9c83f694bda661880d9211b29b1970c99a8a4ddd4844dab6fcaf3` |
| `src/TextRenderer.cpp` | `2e95a09f2ea5f0aef8fe3b22f487fce776ea4ae5a918606f464ebf6a4e2c0cc4` |
| `src/TransitionEffect.cpp` | `d114571cc8111ba3fa36be41ce51321e9a3747871bf34aaa9ba60002a22e315f` |

Preferred current native witnesses (only if the source pin matches):

- Ending Run exact: `build/match-linked/EndingTextRun/`,
  [complete source/image, original fields, table and guard pins](evidence/ending-run-1568-exact-20261106/native-result.json).
- Replay Save exact: `build/match-linked/EnemyReadIntOperand/` (shared real graph),
  [original acceptance](evidence/replay-save-1681-exact-20261106/native-result.json);
  [current Initialize-source selected cold pins](evidence/replay-initialize-constructors-20261106/native-result.json).
- Existing Replay Create exact: `build/match-linked/ReplayManagerCreateDefaults/`,
  current34-input recipe includes real configuration defaults and preserves185/all12/EH;
  Init remains nonexact. [Current source and context pins](evidence/replay-initialize-constructors-20261106/native-result.json).
- Type-1 Draw exact: `build/match-linked/LaserType1Draw/`,
  [current-header selected cold pins](evidence/laser-type1-circle-owner-20261106/native-result.json);
  [original source/callee proof](evidence/laser-type1-draw-173-exact-20261106/README.md).
- Type-1 circle/current box context:
  `build/gpt-web/20261106-laser-type1-circle/real-caller-context/`,
  [complete candidates, source pins and selected Update/Draw protection](evidence/laser-type1-circle-owner-20261106/native-result.json).
- Linear camera-shake candidate: `build/gpt-web/20261106-upstream-review/camera-baseline/`,
  [complete baseline, rejected controls and restored-source guard](evidence/upstream-th10-decomp-review-20261106/camera-experiments.json).
- Statistics-record candidate:
  `build/gpt-web/20261106-statistics-record/final-linked/`,
  [full original-bound diagnostic and source pins](evidence/statistics-record-natural-loop-20261106/native-result.json).
- Type-0 Bounce candidate and selected Update guard:
  `build/match-linked/LaserLineUpdate/`,
  [source/image and actual virtual-slot pins](evidence/laser-bounce-virtual-owner-20261106/native-result.json).
- Bullet vertical/horizontal wrap: canonical
  `build/match-linked/BulletHorizontalWrap/`, private
  `.analysis/gpt-web/20261106-bullet-vertical-wrap/canonical-replay.json`,
  [source/image pins](evidence/bullet-vertical-wrap-20261106/native-result.json).
- ANM five-TU whole executor:
  `build/gpt-web/20261106-handoff-anm-wrap/current/gptweb-proof.json`,
  `docs/evidence/anm-integer-assignment-wrap-20261106/README.md`.
- ECL counted-offset cursor589 and whole Run7020:
  `build/gpt-web/20261106-ecl-count-reference/final/`,
  [current source/image and complete comparisons](evidence/ecl-counted-offset-cursor-20261106/native-result.json).
- Earlier ECL two-TU `Run` source epoch (check its pins):
  `build/gpt-web/20261106-ecl-store-word/current/native-result.json`,
  `docs/evidence/ecl-stores-scalar-lvalues-20261106/README.md`.
- Enemy 11-TU dispatcher/BeginSpell:
  `build/gpt-web/20261106-enemy-bullet-spawn-alias/current/native-result.json`,
  `docs/evidence/enemy-pattern-bullet-descriptor-boundary-20261106/README.md`.

Targeted examples, **not** a request to run all units:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit anm-get-int-var --unit anm-get-int-var-ptr --json
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-read-int --unit ecl-vm-stack-pop --json
```

## Storage, evidence, and next-agent transfer

Consult [RE_ARTIFACTS.md](RE_ARTIFACTS.md) for **current** expanded/archived
storage and selective recovery. Keep the original EXE, `.tools/`,
`ghidra-project/`, `_reference/`, `.analysis/archive/`, current proof
scripts/receipts and **all `build/match-linked/` canonical artifacts**.
Deleted failed *regenerable* experiments are not missing Git source; re-run
their `.analysis/gpt-web/` driver, or restore a verified archived member
when needed. **Do not mix old PDB/MAP/OBJ with current source hashes.**
The ignored original, private toolchain, database and archives are **not in
Git**; transferring just the Git branch is insufficient for a new machine.

For documentation/metadata-only changes run `scripts/repo-python scripts/ci.py`
and `git diff --check`. No full historical cold replay is needed for
handoff documentation. Store new source results in small separate
`gpt-web:` commits, then update this concise table rather than appending
another dated mini-report. Do not claim whole-game link, actual runtime,
portability, or proof of original source author identity from exact bytes.
