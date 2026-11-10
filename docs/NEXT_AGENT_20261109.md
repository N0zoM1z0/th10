> Historical snapshot (2026-11-09). Its 1,510 / 198,606 checkpoint is superseded by [NEXT_AGENT_20261110.md](NEXT_AGENT_20261110.md) and [RE_HANDOFF.md](RE_HANDOFF.md).

# TH10 next-agent quick resume — 2026-11-09

Use [RE_HANDOFF.md](RE_HANDOFF.md) for the authoritative detailed brief.
[AGENTS.md](../AGENTS.md) gives the current workflow rules. The longer
[historical handoff](RE_HANDOFF_HISTORY_20261109.md) contains earlier trial
notes, **not** the latest codegen scores or source hashes.

## Live checkpoint

- Worktree: `main`; check `git status --short --branch` and `git log -8
  --oneline` first. Local commits are not automatically pushed.
- Target: original Japanese TH10 1.00a, `resources/th10.exe`,
  SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
- Pinned toolchain: VC7.1 SP1 with TH10-specific LTCG `/GL`. Do not
  run any full all-function cold replay.
- Correct accepted exact count after `91bf803`: **1,510 functions /
  198,606 bytes**, from generated [PROGRESS.md](PROGRESS.md). This
  corrected a 420-byte discrepancy: previously omitted exact
  `0x43E710` (203 bytes) and `0x449870` (217 bytes) passed two
  current-source selected-only cold builds and entered `matches.csv`.
- Prior `src/EclVm.cpp` dirty source work was explicitly preserved in
  `b707b92`; the seven-kilobyte ECL Run owner remains **6997/7020**,
  nonexact. Inspect and preserve any new dirty work.

## New exact source recovery worth reusing

`LoadTexture` **243/243** and `LoadTextureRegion` **320/320** share a
typed THTX accessor that corrects retail VC7.1 MOV/LEA lowering.
`EnemyLaserUpdateCallback` **91/91** needs bitfield reads of
GameManager +0x58 rather than a cached DWORD. The restored natural
`LoadScript_0043E710` is **203/203** only in its selected two-TU
context; a seven-TU boss graph emitted 216 bytes and remains nonexact.
`AnmLoadedView::InitializeVm` is **217/217**, not the stale 218-byte
historic diagnostic. Full evidence links are in [RE_HANDOFF.md](RE_HANDOFF.md).

## Attack big owners and leaves together

- ANM ExecuteScript `0x43EE30`: **9962/9964**, 350 bound fields;
  opcode 91 ECX versus EDX is the two-byte frontier.
- ECL Run `0x44E1A0`: **6997/7020**, 189 bound fields; FORMAT and
  x87 stack-home scheduling remain open.
- Spell HUD StartSpell `0x409280`: target 2432, selected candidate
  2252, **NONEXACT**; 7-stage source is present and the name renderer
  has been corrected to exact DrawTextRight.
- Enemy ECL dispatcher target **14,416** bytes, historical candidate
  about 14,352, NONEXACT; Player update and GameManager update are
  also open. Read each current row before adopting a historical score.

## Useful commands and private files

    git status --short --branch
    git log -8 --oneline
    scripts/repo-python scripts/verify-target.py
    scripts/repo-python scripts/verify-toolchain.py --check
    scripts/repo-python scripts/ci.py

For source changes rebuild only affected `--unit` owners, compare full
target-bound extents and required exact consumers, then make
`gpt-web: ...` commits. Never push without permission.

For local/private evidence and storage pruning read
[RE_ARTIFACTS.md](RE_ARTIFACTS.md). The latest verified private
cleanup reclaimed **71.18 MiB**, without deleting evidence archives
or canonical linked graphs. Do not delete the original executable,
pinned `.tools/`, `ghidra-project/`, `_reference/`,
`.analysis/archive/` or `build/match-linked/`. A Git clone alone
does not carry these ignored resources or the private deletion audit.
