# Complete score/life source diagnostics, 2026-10-25

## First model: scope and complete results

At maintained checkpoint **ef5b9a8**, this is an **ignored combined source model
and graph expansion**, not a pure differential or maintained source admission.
Gui.cpp is an ignored copy including the complete proposal once. Maintained
GameManager.cpp, Gui.cpp, headers, configuration and metrics are unchanged.

The eight-TU order is GameManager, ScriptViewerWorker, staged Gui, AnmManager,
AnmVmCreate, AnmVmId, EnemyEclDispatcher and EclVm. The real OnUpdate entry and
standard diagnostic anchor harness remain. Base flags retain /GL without /GS;
only EnemyEclDispatcher and EclVm add /GS from the existing sound-queue context.
This union is a diagnostic hypothesis, not an original production profile.
[Summary JSON](summary.json) records the reproducible order, profiles and pins.

| Complete owner | Native / target bytes | Native fields / locals / calls |
| --- | --- | --- |
| GameManagerUpdateBody | 1559 / 1544 | 114 / 55 / 36 |
| GameUpdateGui, target 0x417040 | 734 / 671 | 39 / 24 / 15 |
| GameAddLives, target 0x4188A0 | 194 / 141 | 15 / 1 / 7 |

All three are **nonexact**. Extents and inventories are not mismatch-byte counts
or accepted bindings. The after-OR dead branch survives naturally, without a
forcing hack. CreateVmVariant0 is inlined where the target calls it; the first
43 bytes of AddLives are instruction-equivalent modulo fields, not fragment
exact credit. Independent audits found no concrete semantic defect justifying
wrapper, noinline or profile panels. No original class/TU or shared-global
closure follows: the new score-view anchor is independent, and nearest-symbol
guesses are unsafe.

## Preserved failure and successful same-image supplement

The link succeeded, but **original root.exit and cold.exit remain 1**:
serialization of the returned support_profiles failed on PosixPath dictionary
keys. The failure is after cold_link, not a native link failure. A separate
observer comparison exits 0 on the retained image; no rebuild or overwrite.
Supplemental metadata validation passes with **65 original files unchanged**.
Its metadata is explicitly reconstructed, not the missing original return;
unrecoverable cold_removed values are not invented.

In the first model, existing exact guards pass: manager 127/all 2, Tick 72/all 4, Stop 101/all 3,
SetSprite 356/all 2, CreateVmVariant0 73/all 5, GuiLives 77/all 0 and QueueSample
123/all 1. The original optional MarkVm schema failure remains: its normal-COFF
recipe was incorrectly treated as a zero-field linked recipe. A separate
supplement uses its actual relocation and proves **MarkVm 66/all 1**, calling
same-image **FindVm 73/all 0**. No comparator weakening or new credit.

- Retained image SHA-256:
  `d51ad3edc9b79539f9ff9fc167ce070f72c02e7f91109c4a62da703ac7abb8c1`
- Supplemental metadata receipt SHA-256:
  `efbc9e768c11d13a7f1df8246cfdcafb0281a5934a2753121db942129d4c7e64`

Receipts remain under `.analysis/gpt-dots/game-score-first-native-20261025`;
compact identities are in the summary. No disassembly or target payload is copied.

## Second model: shared state and real ItemManager caller

The ignored shared-model packet uses a coherent **40-file / 730068-byte**
overlay, with all quoted includes resolving inside it. It refines canonical
GameScoreState partial fields while retaining extent 0x5C and rank at 0x58;
0x5C/0x60 remain separate aliases. Existing ItemAddLives identity replaces the
independent helper, and real ItemManager (caller 0x41B704) is the ninth TU.
All TUs use the overlay; order otherwise stays unchanged, with the same base
profile and only dispatcher/EclVm adding /GS. This remains a combined
shared-layout/caller-context experiment, not maintained source admission.

Root/cold/comparison exits are **0**, and full return metadata is saved without
serialization failure. Parent remains **1559/1544**, GameUpdateGui becomes
**730/671**, and ItemAddLives **189/141**: all nonexact. All tested existing
guards except **CreateVmVariant0** pass. That optional guard is a genuine
**structural rejection: complete owner 74 bytes versus target 73**. The runner
receipt's success records its required checks, not all optional guards exact.
Do not trim the owner, waive the rejection or infer acceptance from the exit.

- Shared-model image SHA-256:
  `68c83c1ada3b327b80f81b82a430977ceedc70356eee8833beaf9091f75882d1`
- Packet: `.analysis/gpt-dots/game-score-shared-native-20261025`

Independent diagnostic review passes with originals preserved: **10 other
guards exact, CreateVmVariant0 still rejected**. Its downstream InitializeVm
changes 207/RET8 to 201/RET4 with a different natural private register/stack
ABI; replacing a one-byte push with a two-byte receiver move explains the
wrapper growth. ANM source/header are unchanged. Both scorer and ItemManager
call the same ItemAddLives using the canonical score symbol. This combined
TU/type change does not isolate causality or justify a new noinline model.
Review receipt SHA-256:
`7696c68a8437656b47b9e7455a1c920e0712ffe6b5c6a9b5879df8246400833c`.
First-model failure and supplements remain unchanged with their original scope.

## Credit and CI scope

**Zero new maintained source owners, exact bytes or denominator bytes.** The
previous 112-stage pass belongs to maintained ef5b9a8's worker checkpoint;
there is no new full-CI claim for this ignored source/graph experiment.

Neither model establishes shared runtime storage or original class/TU ownership.
No new semantic source model is supported by this review; further work requires
fresh evidence, not wrapper/noinline/profile or spelling/flag panels.
