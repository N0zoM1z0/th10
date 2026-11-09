# TH10 quick resume — 2026-11-09

This is the current short handoff. Read AGENTS.md, then RE_HANDOFF.md,
PROGRESS.md and RE_ARTIFACTS.md. Use KNOWLEDGE_BASE.md for older source-epoch
findings, not as a live scoreboard.

## Current checkpoint

- Target: original Japanese TH10 v1.00a resources/th10.exe, 487,936 bytes,
  SHA-256 2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.
- Source checkpoint: 2833424 — gpt-web: recover GameUI transition VM initialization.
  Its parent is e0c9337 — gpt-web: preserve ANM initializer ABI across LTCG.
- Branch is main; local commits are not pushed. Never push without an explicit
  request. The source checkpoint was committed after a successful focused
  23-TU incremental link; no full historical cold replay was run.
- Toolchain: pinned VC7.1 SP1, real /GL graph and TH10-specific profiles.
  Preserve the private .tools/, Ghidra project, _reference/, original target,
  and canonical build/match-linked/ graph.
- Current canonical ledger (PROGRESS.md): **1,505 exact functions /
  197,532 exact bytes**, including **595 authored exact / 127,416 bytes**.
  The authored source-present nonexact backlog is **155**. These are ledger
  counts, not runtime or full-rebuild claims.

## Latest ANM and GameUI work

### ABI corrections and recovered state

- AnmRenderManagerView::AddVmVariant0 @ 0x004489D0 returns AnmVmIdView
  by value. The original uses the hidden structure-return buffer in EAX;
  Ghidra's in_EAX is not evidence for an explicit int *resultId parameter.
  Keep the natural by-value C++ declaration. The experimental out-parameter
  conversion was rejected and reverted.
- GameUiVmTransitionView::Initialize @ 0x00420F30 is a member initializer
  with a screen-space position argument. GameUiTransitionInitializeSecondary
  @ 0x00421180 takes an owner and a position. Both create three root VMs
  from the transition ANM at owner offset +0x3C8, using scripts 0, 1 and 2,
  then propagate the position to all three IDs.
- The original owner class name remains unidentified. The maintained
  GameUiVmTransitionView is descriptive; its transition ANM pointer at
  +0x3C8 and state at +0x6FC are target-proven.
- The secondary initializer resets the renderer's transition viewport when
  captureAnmIndex < 0: offsets +0x2C..+0x48 receive
  0, 0, 0x280, 0x1E0, 0, 0, 0x280, 0x1E0. The maintained field names are
  provisional except for the measured offsets and values.

### Latest focused diagnostic

The last successful incremental diagnostic used the pinned 23-TU graph. Its
driver and output are retained:

- Driver: .analysis/gpt-web/20261108-current/relink-anm-initialize-vm-gameui-incremental.py
- Result: .analysis/gpt-web/20261108-current/anm-initialize-vm-gameui-results.json
- Candidate PE/MAP: build/gpt-web/20261108-anm-initialize-vm-gameui/source.exe
  and source.map
- Candidate image SHA-256:
  9611c0749d462ea0a81ccb11ba66a1876d701d73e694597696f30b28c8fd7261

| Function | Target bytes | Candidate bytes | Ordinary-byte matches | Candidate fields |
| --- | ---: | ---: | ---: | ---: |
| AnmLoadedView::InitializeVm @ 0x449870 | 218 | 207 | 35/203 | 1 |
| AnmVmView::InitializeForLoadedScript @ 0x404F30 | 202 | 200 | 24/192 | 2 |
| AnmLoadedView::InitializeAndExecuteScriptIndex @ 0x43E710 | 203 | 197 | 17/181 | 4 |
| GameUiVmTransitionView::Initialize @ 0x420F30 | 309 | 307 | 63/219 | 22 |
| GameUiTransitionInitializeSecondary @ 0x421180 | 369 | 369 | 30/277 | 23 |
| EnemyBeginSpell @ 0x409280 | 2,432 | 2,432 | 1,352/2,000 | 108 |
| AnmVmView::Initialize @ 0x401DE0 | 273 | 273 | 273/273 | 0 |

The candidate-field counts above are not independently original-bound
relocation contracts. These are one diagnostic link, not canonical exact
receipts and not new exact credit. The loop-based GameUI VM creation attempt
was worse (194/309 and 250/369); the current source uses explicit three-script
C++ sequences.

EnemyBeginSpell scored 1,403/2,000 in the older 12-TU Enemy/ECL graph and
1,352/2,000 in this 23-TU ANM/GameUI graph. The link contexts differ, so do
not treat those figures as a direct source regression. The function remains
NONEXACT.

## Major open work — do not focus only on leaves

1. Continue the large ANM path: AnmLoadedView::InitializeVm,
   InitializeForLoadedScript, InitializeAndExecuteScriptIndex, and the
   9,964-byte ExecuteScript owner. Resolve real caller context and private
   ABI from target disassembly; do not invent signatures from Ghidra's
   provisional decompilation.
2. Continue the large EnemyBeginSpell @ 0x00409280 owner and its real
   ANM/stage setup. Its target argument order is (state, spellId, name, value);
   the four-argument callee uses RET 0x10. Keep this target evidence while
   investigating remaining source/codegen differences.
3. Keep the 7,020-byte ECL Run @ 0x0044E1A0 and 14,416-byte Enemy ECL
   dispatcher in the large-owner frontier. The existing handoff and
   knowledge-base evidence give their current source-epoch diagnostics.
4. Use useful leaves as ABI/callee evidence for these large owners, not as a
   substitute for attacking them.

## First commands and workflow

- git status --short --branch
- git log -8 --oneline
- scripts/repo-python scripts/verify-target.py
- scripts/repo-python scripts/verify-toolchain.py --check
- scripts/repo-python scripts/ci.py

Preserve dirty work. Inspect diffs before edits; do not reset or stash
blindly. Commit meaningful progress locally as gpt-web: ...; never push.
Do not run the full historical cold replay. For source changes, compile and
compare the affected owner and only affected exact consumers using the
existing selected-unit scripts.

Exact credit still requires the complete target-owned extent, independently
original-bound fields/relocations, and two independent current-source cold
builds. A successful compile, matching length, or candidate-only field mask
is not exact acceptance.

## Storage and retained evidence

The selective 2026-11-09 cleanup leaves build/gpt-web/ at
**313,556,992 allocated bytes (299.03 MiB)**, down from 540,475,392 bytes;
**226,918,400 allocated bytes** were reclaimed. It removed 2,622 obsolete
.obj/.exe/.pdb files (222,426,440 payload bytes) from rejected trial roots
and 290 .pyc cache files (5,243,328 payload bytes).

The following eight diagnostic roots remain intact:
- enemy-runtime-full-ltcg-callers/
- 20261108-enemy-runtime-aggregate-sum/
- 20261108-enemy-dispatch-rank-tail-final/
- 20261108-anm-initialize-vm-gameui/
- 20261108-anm-initialize-vm-gameui-callers/
- 20261108-enemy-begin-spell-20261105-full-snapshot/
- 20261108-anm-binder-complete-real-callers/
- 20261108-anm-binder-real-callers/

Nested installed-cold, accepted-exact and canonical-cold directories,
plus root-level .obj inputs, were preserved. MAP/LOG/RSP/JSON/source
snapshots and experiment drivers remain. The original EXE, .tools/,
_reference/, ghidra-project/, build/match-linked/, and every
.analysis/archive/ archive were preserved and checked unchanged.

The private deletion audit is
.analysis/maintenance-next-agent-20261109/removed-files.jsonl
(SHA-256 3b9f4f3346ccca96e1bd0556dc48e1c0fb70be44b719b222d8686f0ee99c5bdc);
the adjacent cleanup-summary.json records before/after measurements.
No backup archive was created: the manifest is an audit, not a restore medium.
