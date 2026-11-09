# TH10 reconstruction — current agent handoff

Updated 2026-11-09, after `91bf803` (ANM exact-ledger reconciliation).
This is the **short authoritative continuation brief**. The dated
[2026-11-09 historical snapshot](RE_HANDOFF_HISTORY_20261109.md) holds the
old chronological work log, superseded source hashes and rejected trials.
Do not treat that snapshot or isolated `build/gpt-web/` binaries as current.

## First actions and truth sources

1. In this live TH10 worktree, inspect `git status --short --branch` and
   `git log -8 --oneline` before touching files. The ECL squared-length
   experiment formerly left dirty is saved in `b707b92` and explicitly
   **NONEXACT**; handle *any new dirty work* directly, never discard it
   unexamined. Do not push without permission.
2. Read [AGENTS.md](../AGENTS.md), the relevant source and the corresponding
   rows in `config/functions.csv` and `config/matches.csv`. The latter is
   the **accepted exact** ledger; a source function's `status=exact` alone
   does not grant acceptance. Generated [PROGRESS.md](PROGRESS.md) is the
   count authority.
3. The only supported executable is the original Japanese TH10 v1.00a
   `resources/th10.exe`, 487,936 bytes, SHA-256
   `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
   Use pinned MSVC 7.1 SP1, the real TH10 `/GL` graphs and local scripts.
4. Only compile/compare **selected affected units**. Never launch the
   historical all-unit cold replay. A complete PDB/COFF function extent,
   every original-bound field, local tables, real ABI and independent
   source-context reproducibility are required before an exact admission.

Suggested first commands:

    git status --short --branch
    git log -8 --oneline
    scripts/repo-python scripts/verify-target.py
    scripts/repo-python scripts/verify-toolchain.py --check
    scripts/repo-python scripts/ci.py

No compile, Ghidra session, receipt refresh or cold replay is needed for
ordinary documentation-only edits.

## Accepted exact progress

As of `91bf803`, canonical `config/matches.csv` and regenerated
`docs/PROGRESS.md` agree: **1,510 exact functions / 198,606 exact
bytes**, of which **599 authored-origin functions / 128,399 bytes**.
These are ledger counts, not a rebuilt runnable TH10 executable.

Useful current-source exact functions:

| Target | Complete extent | Selected proof / important scope |
| --- | ---: | --- |
| `0x00446C70` ANM LoadTexture | **243/243** | `anm-resource-load-texture`; 8 bound fields |
| `0x00446D70` ANM LoadTextureRegion | **320/320** | `anm-resource-load-texture-region`; 8 bound fields |
| `0x0041C480` enemy laser update callback | **91/91** | `enemy-laser-update-callback-bitfield`; native flags, 6 fields |
| `0x0043E710` loaded-script start | **203/203** | `anm-loaded-start-script-wave11`; selected **two-TU only**, 4 fields |
| `0x00449870` loaded VM initializer | **217/217** | `anm-loaded-initialize-vm`; 1 field |
| `0x00401DE0` VM fastcall initializer | **273/273 raw** | `anm-vm-initialize-private` |
| `0x00447AE0` DrawTextRight | **204/204** | `anm-text-right-gs` |

The `0x0043E710` and `0x00449870` rows were previously marked exact in
`functions.csv` but omitted from `matches.csv`. Both have **two fresh,
complete selected-only links** and are now formally admitted; see
[reconciliation evidence](evidence/anm-handoff-ledger-reconciliation-20261109/README.md).

The ANM texture upload pair shares an inlined typed THTX accessor; its three
previously exact texture creators remain **132/132, 264/264 and 41/41**
in the same graph (five distinct owners, 1,000 bytes total). Laser draw
callback **46/46** and laser point accumulator **54/54** remain exact in
their affected source graph. Earlier large exact examples include
ReplayManager::SaveReplay **1681/1681** and EndingTextVmView::Run
**1568/1568**. See [ANM texture](evidence/anm-texture-load-exact-20261109/README.md),
[laser update](evidence/enemy-laser-update-bitfields-20261109/README.md),
and the selected function ledger.

## Important nonexact large owners — do not substitute leaf work

The figures below are bounded **last observed** linked-source contexts.
They are not current full-product validation, and changing the LTCG graph
may change both size and register scheduling.

| Target / source | Latest supported candidate | Main blocker / next action |
| --- | --- | --- |
| ANM `ExecuteScript @0x0043EE30` — `src/AnmManager.cpp` | 9,964/9,964, **9962/9964** with 350 bound target fields | Opcode 91's `renderLayer` uses ECX in candidate vs EDX retail at offsets `+0x113,+0x115`. Test a new *compiler-liveness* hypothesis, not another already-failed inline/volatile/aggregate/bitfield alias |
| ECL `Run @0x0044E1A0` — `src/EclVm.cpp` | 7,020/7,020, **6997/7020**, 189 fields | 23 differences: early integer stack/receiver registers, x87 length/point-angle, FORMAT stack homes `+0x14/+0x18`. The saved output-pointer-order change is **not** byte progress |
| Spell HUD `StartSpell @0x00409280` — `src/PlayerHudStartSpell.cpp` | 2,252/2,432, **322/1920 ordinary**, 83 *candidate-only* relocation fields | Seven-stage logic present, but ABI and LTCG TU partition unresolved. Genuine name draw calls exact `DrawTextRight @0x447AE0`; fabricated `FormatRightAligned` facade removed |
| Enemy ECL dispatcher — `src/EnemyEclDispatcher.cpp` | ~14,352/14,416 in a historical selected graph, **NONEXACT** | Opcode/instruction views and private registers; recheck current source epoch before relying on any score |
| Player update callback body `0x00425730` — `src/Player.cpp` | historical 3108/3112, **NONEXACT** | state machine call context and register allocation |
| GameManager update body `0x00418190` — `src/GameManager.cpp` | historical 1538/1544, **NONEXACT** | `EnemyManager+0x10` is now typed `specialEnemySlots[0]`, **not** active-enemy count |

**Critical source-context trap:** `LoadScript_0043E710` is exact at 203
bytes in its **selected two-TU graph**, but emitted **216 bytes** in a
larger seven-TU spell-HUD diagnostic. Likewise a 202-byte script starter
`0x00404F30` with ordinary bytes matching under candidate-only fields is
**not formally accepted**. Never promote these candidate findings to a
whole `StartSpell` exact claim.

Focused ANM baseline is retained under
`.analysis/gpt-web/20261109/anm-child91-natural-aliases.py`.
ECL source-epoch diagnostic:
`.analysis/gpt-web/20261109/ecl-run-handoff-dirty-source.json`,
with target/byte rationale in
[ECL squared-length note](evidence/ecl-run-length-squared-20261109/README.md).
Spell HUD focused driver:
`.analysis/gpt-web/20261109/relink-playerhud-restored-natural-loader.py`;
read [spell-HUD evidence](evidence/player-hud-start-spell-20261109/README.md).

## Focused experiment loop and useful leads

Use `scripts/repo-python scripts/build-match-unit.py --unit NAME`,
then `scripts/repo-python scripts/compare-linked-function.py --unit NAME
--json`. For the nonexact ECL owner use
`scripts/diagnose-ecl-run-native.py --unit ecl-vm-host-run`; a nonzero
exit is expected because the **complete owner remains nonexact**.
Selected-unit rebuilds, not full replay, are enough for a new experiment.

Avoid repeating proven byte-neutral variants: ECL pointer-first and swapped
square expressions stayed **6997/7020**; ANM child-91 bitfield/record
accessors stayed **9962/9964** (some variants regressed or moved the
350-field geometry). Memory-file Seek `0x4364F0` remains **100/107**
under a pinned normal-COFF candidate; reference-repo natural goto layouts
regressed. Laser `0x41C480` is **already exact** and needs no new leaf
credit.

The `_reference/th10-decomp` tree is a source of hypotheses, **not**
authoritative layout, ABI, source partition, or executable bytes. Prefer
retail instructions and current native comparisons; retain failed source
variants in ignored `.analysis/` when they materially explain the next
attempt.

## Artifacts and delivery

Tracked `docs/evidence/` receipts and the original Git history travel
with a clone. The **ignored** original executable, `.tools/` compiler,
`ghidra-project/`, `_reference/`, historical `.analysis/archive/`,
private scratch drivers and canonical `build/match-linked/` **do not**
travel with Git; transfer them separately if the new agent uses another
machine. The latest audited ignored-artifact cleanup reclaimed **71.18 MiB**
of allocation by removing 1,090 rebuildable trial PE/OBJ/PDB and
Python-bytecode files. Disk pruning, retained witness roots and
private audit locations are documented in
[RE_ARTIFACTS.md](RE_ARTIFACTS.md). Keep useful MAP/log/JSON results,
selected native witness graphs and the canonical match graph.

Commit meaningful verified work in English as `gpt-web: ...`, using
the author/trailer specified in `AGENTS.md`; **never push** without an
explicit request. For historical detailed hypotheses see
[2026-11-09 archived handoff](RE_HANDOFF_HISTORY_20261109.md),
[KNOWLEDGE_BASE.md](KNOWLEDGE_BASE.md) and individual evidence pages.
