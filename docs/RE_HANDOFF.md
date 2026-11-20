# TH10 reconstruction — next-agent handoff

**Updated 2026-11-20.** Start with [AGENTS.md](../AGENTS.md), this brief, and the relevant part of [KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md). The current work is natural C/C++ exact reconstruction, not whole-product/runtime closure. Work on `main`, inspect `git status --short --branch` and `git log -5 --oneline`, commit locally with `gpt-web:`, and do not push.

## Target and source of truth

- Original Japanese TH10 v1.00a: `resources/th10.exe`, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`. Keep this private input uncommitted and unmodified.
- `config/match-units.toml` holds comparison recipes; `config/matches.csv` is the historical canonical exact ledger; `config/frontiers.toml` stores nonexact diagnostics. **A ledger entry is not automatically a fresh exact comparison against the latest source.** Do not change target sizes, relocation bindings or frontier residuals simply to obtain a pass.
- [PROGRESS.md](PROGRESS.md) is generated from those ledgers, not manually maintained. At this checkpoint it reports **1,556 canonical exact functions / 214,824 codegen bytes**, of which **640 functions / 144,495 bytes** have confirmed authored origin. Full-game origin and inventory closure remain open.

## Immediate focus: bullet transform interpreter

**`BulletRuntimeView::AdvanceTransformProgram`**, target `0x00406D90`, lives in `src/BulletTransform.cpp`. This is a substantial, **nonexact** owner, not a completed leaf. The target has `0x5AE` (1,454) code bytes and a `0x644` (1,604) byte code-and-local-table comparison extent. The current `bullet-transform-program` recipe uses VC7.1 SP1 LTCG `/GL`, a 128-byte selector data span at `+0x5C4`, and five local switch-table destinations. The old description of this function as a non-`/GL` normal-COFF owner is obsolete.

The source currently implements the 18-record transform loop, size-`0x18` records at runtime `+0x464`, kind at record `+0x10`, allow-while-active at `+0x14`, active flags at runtime `+0x43C`, and cursor at `+0x45C`. It uses a smaller switch nested inside range dispatch, the real ANM and sound-queue calls, the typed child-pattern descriptor, and nine transform state slots. In commit `306639a9`, the previously dirty code was preserved as a reviewed **work-in-progress**: there is an entry bound check and a shared loop-tail bound check; already-advanced actions skip normal advancement; a spawned child with the fade-parent flag follows an extra advance after cancelling the parent. This was checked against the target control-flow addresses, but the overall function has **not** achieved exact codegen.

Current targeted build:

```sh
scripts/repo-python scripts/build-match-unit.py --unit bullet-transform-program
scripts/repo-python scripts/compare-linked-function.py --unit bullet-transform-program --json
```

The latest candidate has a **`0x634`-byte PDB contribution versus the manifest's `0x644`**. The strict comparator reports that extent mismatch, so a byte-match percentage would be misleading. An earlier committed baseline also failed (`0x61C` versus `0x644`). Next: preserve the nested opcode order, match the first record load / EAX-ECX-ESI lifetimes, child descriptor store order, the two distinct loop tails, and the local jump-table boundaries; compare the *whole* owner rather than stopping at the first `RET`. The retail entry uses two indexed addresses (`[ebp+eax*8+0x474]` and `LEA [ebp+eax*8+0x464]`); present C++ still lowers them differently. Do not use ASM, register fakery or padding.

**Affected exact-owner caveat:** `bullet-runtime-update` has a historical `885/885` row in `config/matches.csv`, but its current mixed-profile graph produces a `0x36E` PDB contribution against manifest `0x375`. The same `0x36E` result occurs when compiling the previous committed `BulletTransform.cpp` (before the `306639a9` loop changes); it is **not established as a new regression from that dirty change**. The next agent should reconcile the complete root graph and source epoch before claiming its current exactness or changing the canonical ledger.

Adjacent previously verified independent leaves: `enemy-angle-from-player` (`Player::AngleFromPosition`, **67/67**) and `bullet-spawn-pattern-exact` (**194/194**). These are scoped historical checks; rerun them when relevant input graphs change. `EnemyRuntimeUpdate` (**2,412/2,412**) and `EnemySpawn` (**577/577**) were likewise exact in the earlier Enemy ANM-state graph. A matching callee never makes the parent exact.

## Other major frontiers

These are the last useful bounded diagnostics, not fresh acceptance receipts:

| Owner / unit | Candidate / target | Residual | Next focus |
| --- | ---: | ---: | --- |
| Enemy ECL dispatcher, `enemy-dispatch-ecl` (36-TU LTCG) | 14,452 / 14,416 | 10,414 | Large opcode scheduler, operand lifetimes and typed pattern firing; current frontier `REGRESSED` by offset placement |
| ECL interpreter, `ecl-run` | 7,020 / 7,020 | 23 | Remaining register/SIB and spill order; keep the four exact ECL stack leaves |
| ANM executor, `anm-execute-script` | 9,964 / 9,964 | 2 | Opcode 91 argument register EDX vs ECX |
| GUI message VM, `gui-message-run-candidate` | 4,464 / 4,480 | 1,935 | Portrait operand lifetimes, branch ordering |
| `GameResetGui` | 1,965 / 1,965 | 47 | Preserve the exact lives-display helper |

The Enemy dispatcher source now shares an asserted `BulletTransformRecordView` with `BulletManager`, plus the 18-record/laser layout in `EnemyBulletPatternFieldsView` and the five-word `EnemyAnmScriptStateLayout` at Enemy runtime `+0xE8..+0xF8`. Opcode `0x199` indexes 22 overlapping transform slots, not just the 18 ordinary pattern slots. The Stage camera `value0E8/value0EC` words are unrelated shake coordinates. See the Enemy/Bullet sections of the knowledge base before changing layouts; the larger dispatcher still requires its own complete strict comparison.

Other earlier exact guards include `EclVmHost::StopAllThreads` (31), ECL stack Pop/Push/EnterFrame/LeaveFrame (122/96/64/41), `PlayerHandleCollisionHit` (627), `EnemyApplyBulletClear` (44), and the reviewed Main camera owners. Their retained results belong to their original source graphs.

## Short reproducible workflow

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --check

# Current large Bullet owner: build first, then read the strict mismatch.
scripts/repo-python scripts/build-match-unit.py --unit bullet-transform-program
scripts/repo-python scripts/compare-linked-function.py --unit bullet-transform-program --json

# Only when touching their source or linked graph:
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-angle-from-player --unit bullet-spawn-pattern-exact --json

# Existing large-frontier outputs; --no-build does not recompile.
scripts/repo-python scripts/check-frontier.py --no-build ecl-run enemy-dispatch-ecl

# For documentation changes, only lightweight syntax/whitespace checks:
scripts/repo-python scripts/ci.py
git diff --check
```

For a changed LTCG source, validate its actual entry graph and guard only the relevant exact owner(s); do not run full all-unit cold replay. The `bullet-runtime-update` source-epoch mismatch above must be resolved before treating its historical exact row as a current guard. Ghidra target observations remain reproducible with `scripts/repo-python scripts/ghidra.py`; the original executable and Ghidra project must stay private.

## Storage / retained evidence

The duplicate analysis dumps, unconfigured throwaway links and stale scratch were removed without touching the target, toolchain, Ghidra project, D3D adapter evidence, the origin-review input, or any **configured** `build/match-linked/` output. See [RE_ARTIFACTS.md](RE_ARTIFACTS.md) for what remains and what can be regenerated. The current code and this brief are in Git; no deleted scratch export is needed to continue.
