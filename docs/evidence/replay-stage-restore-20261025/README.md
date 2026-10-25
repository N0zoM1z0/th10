# Replay stage restoration: complete-owner storage diagnostics

All three models remain **nonexact**. No production source, source mapping,
canonical recipe, boundary, origin or exact ledger changes. Authored exact stays
**75782/273658 bytes, 416 owners**. The three candidate extents are not mismatch
counts, and the final 357/356 result is not a one-byte mismatch claim.

## Target and first complete model

The reviewed authored owner is **0x42A6A0..0x42A803, 356 bytes**. Its sole real
caller is **GameManagerInitialize at 0x417870**, callsite **0x417AFB**; an earlier
418870 transcription was incorrect. The target consumes EBX without stack
arguments. The existing ordinary free-function declaration in GameManager.cpp
is used unchanged, rather than forcing that private register ABI.

Record mode allocates and conditionally zeroes the complete 0x1C4 header,
publishes it in stageHeaders[currentStage], then reloads stage/header before
seed, low-16 stage index and packed bit0 assignment. A null allocation is still
published and subsequently dereferenced; no new successful failure path is added.
Playback resets record/fps cursors and record index, captures the header,
restores seed/counter, score and power, calls the real SetFaith, and invokes the
existing timer method, which naturally inlines. The timer converts its argument
as a signed int. Three later state stores precede a **fresh currentStage/header
reload** for final +1B8. Other modes return without these effects.

The first complete natural model uses the existing layouts and ordinary
parameter. The graph is candidate ReplayManager.cpp, GameManager.cpp and
ItemManager.cpp, in that order, all VC7.1 /GL without /GS, rooted at the real
GameManagerInitialize. It is a non-runnable diagnostic anchor image, not an
original physical-TU or product claim.

| Model | Restore / target | Real caller / target | Restore fields / locals |
| --- | --- | --- | --- |
| First complete separate scalars | 344 / 356 | 1053 / 1009 | 24 / 4 |
| Common score-state storage | 357 / 356 | 1070 / 1009 | 27 / 4 |
| Existing RNG common storage added | 357 / 356 | 1070 / 1009 | 27 / 4 |

Target has 27 address fields, four fixed local branches and two direct calls.
All emitted owners are independently PE/map/PDB-bound and densely decoded.
The first native caller passes EAX and callee moves it to ESI. Three target
current-stage read sites disappear despite explicit source reload expressions.
No partial instruction window or normal-COFF comparison is accepted.

## Evidence-grounded shared storage

Existing GameScoreStateView at 0x474C40 has extent0x5C. Target-local address
relationships place currentStage at +0x3C, score +4, power +8, faith +0xC,
timer +0x14, value30 +0x30, value50 +0x50 and rank +0x58. GUI-007 previously
showed that common-storage source can recover reload sequences; ENEMY-079
independently distinguishes stage+0x3C from chapter+0x44.

The second isolated model uses one coherent 33-file quoted-include closure.
It exposes the observed fields inside the **unchanged 0x5C extent**, updates
Replay/GameManager/Item uses consistently and leaves +0x5C final value1B8
separate. Unknown+0x38 remains bytes. Existing GameManager volatile lives
accesses retain their qualification through the same object; the restore body
gains no volatile accesses. No new register, noinline, packing or flag control
is introduced.

This model naturally recovers **EBX and all three missing current-stage reads**,
including final playback recapture. It also recovers the later playback store
sequence. Record allocation/flag scheduling and seed/counter scheduling still
differ, and the complete owner remains rejected. The result is storage/context
evidence, not proof of the original C++ class or a faithful runtime data owner.

RNG-001 and existing canonical fields independently bind RngView at0x4918B0:
unsigned16 seed, saved seed and unsigned32 generationCount at+4, total8 bytes.
The third model changes only Replay's two scalar names to these existing fields,
without changing expression order or adding a RandomMath TU. Its prediction of
seed/counter scheduling recovery is **falsified**: the full357-byte body is
identical to the second model after masking all27 address fields. This is
candidate-to-candidate diagnostic neutrality, not target equality or acceptance.
No further source-order, register, noinline or profile panel is justified.

## Field identities, guards and limits

Four-byte diagnostic anchors can overlap when object-member offsets are added.
Nearest-symbol labels are therefore not semantic identities. The shared and RNG
role-binding receipts explicitly pair **every27 field** by source base/addend
and disassembled role, including GameScoreState+0x24 and RngView+4. They do not
waive full extent, local-target or ordinary-byte requirements. Both source models
remain nonexact; no opaque alias is accepted.

Each image passes five strict same-image canonical guards: SetFaith73/all1,
SetTimerCurrent59/all1, stage delay92/all1, update callback7/all1 and mark-chains
28/all0: **259 bytes / all4 fields**. These retain existing credit only. The real
caller remains nonexact in every model and is not admitted through the child.
Full source/input/artifact freezes and the protected nine D3D files are preserved.
All cold and comparison exits are0; no native failure or transport retry occurred.

Independent final review approves documentation-only parking, not source adoption.
It checks both shared/RNG source layouts, full disassemblies, all27 explicit
field roles and four local destinations; it separately preserves2353 frozen
target/tool/source inputs and all12 native artifacts per model. All original
first/shared freezes also remain unchanged under the producer checks. Remaining
allocation/packed-flag and seed/score/counter scheduling differences are real;
no further meaningful model is identified. Final focused toolchain declaration,
tracking, progress and diff checks pass; summary.json records the scopes.
No new full-CI, runtime, original object lifetime, product or portability claim.

## Retained reproduction packet

First source is retained as first-model.patch against the baseline source.
The four shared patches apply to that first model and corresponding unchanged
baseline files; rng-model.patch then applies only the existing RNG identity.
The ignored packets are `.analysis/replay-stage-restore[-shared|-rng]-20261025`;
images are `build/replay-stage-restore-{first,shared,rng}-20261025`. Each packet
retains its frozen runner, cold metadata, complete disassembly/inventories,
source closure, guards, logs and terminal exits. Native builds were serialized,
one CPU at nice10 with180-second caps. All target reads use the canonical input
hash in summary.json. No binary or decompiler payload is committed.
