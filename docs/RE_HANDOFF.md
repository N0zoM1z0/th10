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
  actual LTCG `/GL` profiles. **Never push** without authorization.
- Every dirty change is part of this worktree: inspect and continue it;
  never reset it simply because an earlier note says the tree was clean.
- Make substantive source fixes in both large interpreters and useful leaves.
  Commits are `gpt-web: ...`, with repo author and co-author trailer.
- Accept **only complete, independently original-bound exact** code, all
  fields and branch/table targets, from a reproducible *current-source* cold
  build. Matching size, 100% normalized candidate-vs-candidate code and
  repeated guard replays confer **zero new exact credit**. Build only affected
  units using explicit `--unit` selectors. **Never cold replay all units.**

## Acceptance ledger — generated truth, not a runnable-game claim

As of this source checkpoint, [PROGRESS.md](PROGRESS.md) reports **1,829
reviewed function boundaries**, **1,309 source-present mappings**,
**1,489 complete exact functions / 191,183 original bytes**, including
**580 confirmed-authored exact / 121,093 bytes**. There are **170 authored
source-present nonexact** functions. Original author/source provenance is
not interchangeable with a reconstructed function's exact codegen. Product
build, actual game execution, semantic dual-Oracle and portability are
separate later work, **not established** by these figures.

Recent *accepted* full owners (replay only one affected cohort when needed):

| Exact original owner | Strict acceptance | Evidence |
| --- | --- | --- |
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
| `AnmRenderManagerView::ExecuteScript @0x0043EE30` | **9,964 / 9,964** | **9,962/9,964** original bytes, **350/350 independently original-bound fields**, 95 local targets. Opcode 91 registers at +0x113/+0x115 (original EDX, candidate ECX). Current source also explicitly wraps three integer assignment ops. Do not retry failed aggregate, volatile, member-order or layer-lifetime permutations without a new compiler lead. [Current-source proof](evidence/anm-integer-assignment-wrap-20261106/README.md). |
| `EclVmContext::Run @0x0044E1A0` | **7,020 / 7,020** | **6,997/7,020**, **189/189 bound fields**. Remaining 23 ordinary byte differences include register/stack homes, `LENGTH_SQUARED`, `POINT_ANGLE`, FORMAT and loop/callback paths. `STORE_INT`/`STORE_FLOAT`, CALL, JUMP and stack scalar work are installed. [STORE proof](evidence/ecl-stores-scalar-lvalues-20261106/README.md). |
| `EnemyRuntimeView::DispatchEclInstruction @0x0040E770` | **14,416 / 14,352** | 64-byte length gap; **716 candidate** fields. Fresh 11-TU source/PDB codegen checks are mostly candidate-vs-candidate, **not** a full independently bound target acceptance. Typed spawn record, position, word clear and bullet call are installed. [Caller record](evidence/enemy-pattern-bullet-descriptor-boundary-20261106/README.md). |
| `EnemyBeginSpell @0x00409280` | **2,432 / 2,432** | **108 candidate** fields but many ordinary code differences. Source bonus arithmetic now explicitly wraps modulo 2^32 before the signed cap. Same extent is not exact. [Bonus proof](evidence/enemy-begin-spell-bonus-wrap-20261106/README.md). |
| `EclVmScriptDatabase::AddScriptData @0x00450220` | **589 / 585** | Checked script-file slot reload after validation; `REP MOVSD` and two call fields still displaced. [SCPT proof](evidence/ecl-script-slot-reload-20261105/README.md). |
| `EclVmContext::StartSubroutine` | **550 / 551** | Four caller field positions preserved; direct signed/source pointer lifetime scheduling unresolved. **NONEXACT**. [CALL proof](evidence/ecl-call-counted-inline-name-20261106/README.md). |
| `BulletRuntimeView::UpdateAbsoluteDirectionChange @0x004078E0` | **335 / 335** | **333/335 preliminary ordinary** positions after eight *candidate* field sites; EAX/EDX at +0xD7/+0xDD. Original field identity acceptance not yet complete. [Angle proof](evidence/bullet-runtime-angle-word-20261105/README.md). |
| `SoundPlayerView::LoadSoundSample` / `PreloadBgm` | **684 / 684**; **369 / 369** | Respectively **678/684** and **365/369** ordinary bytes in suitable five-TU graphs; mainly call argument register order. Many simple declaration/alias and wrapper trials already failed. [Sound wire evidence](evidence/sound-riff-chunk-prefix-20261105/README.md). |
| `StageResetPublishedCameras @0x004216F0` | **465 / 533** | Restored omitted secondary/published +0xE4 dword assignments, respectively 1/0; camera layout stays 0x118. Both fresh `/GL` and normal-COFF complete owners emit 533 bytes /40 fields; vector temporary reuse and global-store scheduling remain nonexact. [Word-store proof](evidence/stage-camera-default-words-20261106/README.md). |
| `TextRenderBufferView::InvertAlpha @0x00437160` | **157 / 157** | **Current source** fresh two-TU `/GL /GS` measures **138/157**, zero fields. An *older different source/graph* had only `JS` vs `JL` at +0x69; **that one-byte near-exact result is not current**. Native current proof is private `.analysis/gpt-web/20261106-text-current-exact/`. |

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
| `src/AnmManager.cpp` | `94070661d61c5e144811bf220ac4cd618aac5cdfc0b3b3f7941a0ce4e12ff973` |
| `src/AnmManager.hpp` | `3adee93a8eaec70e0b7d469e96271126a5a631b6a44e5e17199892acd2dc9667` |
| `src/EclVm.cpp` | `480e2c574242417024b67b7295f5c367fc52cda59b83dd29bc704df74568a566` |
| `src/EclVm.hpp` | `b7f6c877aff0fb70ac53f4b55b3e5da5be0b6032532eb3b983d97604ae93fe52` |
| `src/EnemyEclDispatcher.cpp` | `0f496115327b594e053e3aa5e92f37addf5c968a23a613649b0b4376d5e1b034` |
| `src/Enemy.cpp` | `c08272bf324217b9e6e4ffa91cd13fb2555aeb37b44be6231075082ebfcac5d2` |
| `src/Enemy.hpp` | `2e2168b1d84e6dab64abb51c2d6dcdf0498c6f36856c9b1a25f18637ed35eea2` |
| `src/BulletManager.cpp` | `2bda79b9659785c784bb33d7c9b453c4cfefb249d37631d63cc20956d18f25aa` |
| `src/Sound.cpp` | `cb5cdabd1732dfbd93d6da73938df42dc541356fe7c34fd20a33950576b5b3b0` |
| `src/StageCameraDefaults.cpp` | `4ac0d9c1eeb14e1763f5ed642e50895e5081fdf2f41dcda819a6ca7ee7255fb8` |
| `src/StageScript.hpp` | `3a1a5402fdd969c150844102680d007f1080f6e38e44f429e70f0594b537bf5b` |
| `src/GameStartupVersion.cpp` | `125b80a073015e9d0fd2694b989a0983bf3f3777fb29634825818ced0f125b4f` |

Preferred current native witnesses (only if the source pin matches):

- ANM five-TU whole executor:
  `build/gpt-web/20261106-handoff-anm-wrap/current/gptweb-proof.json`,
  `docs/evidence/anm-integer-assignment-wrap-20261106/README.md`.
- ECL two-TU `Run`:
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

For source/evidence-only changes run `scripts/repo-python scripts/ci.py`
and `git diff --check`. No full historical cold replay is needed for
handoff documentation. Store new source results in small separate
`gpt-web:` commits, then update this concise table rather than appending
another dated mini-report. Do not claim whole-game link, actual runtime,
portability, or proof of original source author identity from exact bytes.
