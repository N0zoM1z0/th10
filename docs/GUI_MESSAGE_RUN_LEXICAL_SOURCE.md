# Stage-message dispatcher: lexical source hypothesis

`GuiMessageVmView::Run` remains **decompiled, 0.00**. This change is an ordinary C++ source hypothesis, not a native matching result or runtime-closure claim.

## Target derivation

Independent instruction evidence identifies the complete target owner at `0x00415E90–0x0041700F`: 4480 bytes comprising 4306 executable bytes, two alignment bytes and 172 selector bytes. The next independent owner starts at `0x00417010`.

The source retains the 24-opcode switch and expresses the target's other selector domains lexically: seven enemy-portrait stages, seven enemy-name stages and five stage-six difficulty arms. Their target tables start at `0x00416F64`, `0x00416FC4`, `0x00416FE0` and `0x00416FFC`. Immediate script/sprite/bonus values come from the target-audited implementation. Stage seven retains its portrait variant split; character guards reject values outside 0/1. Source switches do not guarantee generated pointer tables or forbid compiler merging.

The target executes this control flow inside Run. The earlier candidate instead emitted a 1244-byte Run owner plus separate source helpers and lookup arrays; that historical result motivated examining source factoring. Disjoint helper sizes cannot be added to Run for matching credit.

## Ordinary lexical changes

Ten Run-only artificial helpers are removed after their operations are placed at their use sites:

- `CreateGuiVm`, `InterruptVm`
- `DrawMessageText`, `DrawDecodedMessageText`, `SetMessageSide`
- `SetPlayerPortraitSprites`, `SetEnemyPortraitSprites`
- `AddStageClearScore`, `IncrementStageClearCount`, `CompleteStageFromMessage`

Maintained downstream ANM, interrupt, score, music, timer, supervisor and stage-transition owners remain dependencies. `DeleteVm` and `ResolveVm` remain for other GUI owners. No downstream bodies, compiler directives, assembly, escape/retention mechanisms, profile changes or reference implementation code are imported. Production code following Run is unchanged.

## Preserved contracts

- END and pending WAIT leave without advance or tick. Completed and unknown commands use the live instruction pointer for common advance; future-time exit ticks.
- Side changes retain interrupt order, one cached manager across both position calls, and the live side read for the second position.
- Portrait changes retain find/set order and reload operands/resources before the second child operation.
- Text resolution and stale-ID clearing precede decode. Ordinary line draws retain their cached manager; alternating-line zero explicitly reloads the manager for its final draw. Color/side reads remain live.
- Completion unlock stores precede early transition. Stage-six difficulty is selected after the initial score addition. Stage-six and stage-seven effects, mode exits and count publication remain distinct. Count accesses retain signed, capped memcpy operations on live bytes. Former helper returns become outer-case breaks reaching common advance.
- Shared byte-record accessors, canonical scalar/storage contracts and downstream transition/scaled-audio implementations are unchanged.

## Validation

The former host interpreter/lifecycle and mutation suites have been removed.
Historical reports are recoverable at `e1c4075`. Current acceptance uses complete
native comparisons under [the Oracle policy](ORACLES.md).

The earlier fixed graph used EAX/plain RET where the target uses a stack
argument/RET 4. Keeping the stdcall declaration alone does not establish a
repair. The transition effect's private ABI and runtime boundaries remain
unresolved; no new native exactness is claimed here.
