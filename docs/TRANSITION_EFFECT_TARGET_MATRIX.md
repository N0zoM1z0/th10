# Transition effect family: read-only decision

## Recommendation

Implement a complete bounded transition-effect owner, including all nine initializer modes, the six update bodies, five draw bodies, real node cleanup, and factory. This is about 2.27 KiB of substantive target code, plus the 36-byte initializer selector table, before small adapters. Kind-5-only code is acceptable only as an explicitly incomplete semantic experiment; it cannot honestly replace the shared factory/initializer or claim Run's asynchronous lifetime closed.

There is no existing canonical effect type in inspected sources. A new evidence-backed 0x44 view is possible, but two words remain wholly semantically unknown and global physical storage identity remains open. Do not force the six-logical-argument factory into target EBX+five-stack ABI. No production patch is recommended before deciding how to represent unknown fields and testing all nine modes and lifetime contracts. Existing scheduler dispatchers are also absent, so even complete effect source alone cannot establish real engine lifecycle execution.

## Target-bound matrix

All ends below are inclusive; decoded RET followed by CC separates every newly recovered contiguous body. Starts for the eleven callbacks are additionally attested by initializer registration immediates. Extents are provisional complete code boundaries, not authorship/exactness credit.

| Address–end | Bytes | Role and dependency |
|---|---:|---|
| 43BD40–43BD9C | 93 | Kinds0/3 falling alpha; startup abort; Tick404ED0, conversion463B2C |
| 43C1A0–43C225 | 134 | Kinds0/5 full-screen fade; FlushVertexBuffer442F50, device SetViewport slot+BC, DrawColoredRectangle43BDA0 |
| 43C230–43C2B4 | 133 | Kinds2/5 rising alpha; startup abort; GM pause flags; Tick and conversion |
| 43C2C0–43C303 | 68 | Kinds2/3 playfield rectangle, alpha OR value0 |
| 43C310–43C3BE | 175 | Kinds6/7 persistent dim then externally triggered eight-frame release; inlined timer tick |
| 43C3C0–43C403 | 68 | Kind6 full-screen rectangle without viewport/flush reset |
| 43C410–43C453 | 68 | Kind7 playfield rectangle; byte-equivalent behavior to43C2C0 but distinct body |
| 43C460–43C4F9 | 154 | Kind4 repeated alpha pulses, counts down value0; returns0 on finish/abort, unlike the other updates |
| 43C500–43C549 | 74 | Kind4 playfield rectangle; RGB from value1 maskedFFFFFF, computed alpha |
| 43C550–43C706 | 439 | Kind1 screen shake with linearly varying amplitude and four inlined RNG advances |
| 43C710–43C86C | 349 | Kind8 screen shake with attack/hold/decay envelope; real AnmRandomU32InRange43CB80 twice |
| 43C870–43C889 | 26 | ECX deletion callback, null-check, cleanup43CB00, free4524A1; returns0; origin indeterminate |
| 43C890–43C8AA | 27 | ESI receiver scalar-deleting-style adapter; cleanup then optional free based on stack bit1; RET4; origin not established |
| 43C8B0–43C901 | 82 | Allocate0x44, timer-constructor flag clear, zero17 words, flags|2, initialize; EBX priority+five stack args; RET20 |
| 43C910–43C921 | 18 | ESI explicit null-safe cleanup/free entry; plain RET; do not claim separate authored work |
| 43C930–43C968 | 57 | EAX receiver release trigger: +2C=1, timer SetCurrent(0) inline |
| 43C970–43CAD4 | 357 | ESI receiver/EBX priority, full9kind initialization; RET20 |
| 43CAD8–43CAFB | 36 data | Nine selector pointers; separate table ownership obligation |
| 43CB00–43CB7E | 127 | Actual two-node cleanup; stack receiver RET4; real missing authored ledger candidate |

Substantive factory+initializer+11callbacks+cleanup total2321 bytes. Delete callback/adapter/release entry/source ownership are separate decisions; totals must not become credit or padding targets.

## Complete initializer behavior

Unsigned kind check0..8; invalid kind skips registration and then dereferences updateNode. Do not invent successful invalid-kind handling.

| Kind | Update | Draw | Extra |
|---:|---|---|---|
|0|43BD40|43C1A0|none|
|1|43C550|none|none|
|2|43C230|43C2C0|none|
|3|43BD40|43C2C0|alpha=255 before registration|
|4|43C460|43C500|none|
|5|43C230|43C1A0|Run route|
|6|43C310|43C3C0|none|
|7|43C310|43C410|none|
|8|43C710|none|none|

Update priority14. Draw priority is caller-supplied EBX. Register update then draw, publish each returned pointer immediately. RegisterCalc/RegisterDraw store arg=effect and node flags|2. Install deletedCallback only on updateNode after registrations. Timer initialization/reset and kind/frames/three payload writes happen after registration. This ordering exposes a partially initialized object if registration can execute concurrently; do not reorder it under a safety rationale. Target assumes registration allocation succeeds.

## Storage feasibility

Known allocation size68, all17 words zeroed by factory, then flags word0 OR2. Observed fields:
- +00 flags; only factory bit2 established here
- +04 unknown word, zeroed; no semantic name justified
- +08 update ChainElem*, +0C draw ChainElem*
- +10 kind
- +14 unknown word, zeroed; no semantic name justified
- +18 integer alpha
- +1C signed frames for fades/pulse/shake1; signed amplitude for shake8
- +20/+24/+28 three 32-bit payloads with mode-specific interpretations
- +2C release-state word for kinds6/7
- +30 existing AnmVmTimerView: previous,current,subframe,scale,flags through+40

This is a target-backed view with explicitly unknown storage, not an original class reconstruction. Do not introduce a vptr: zero-fill and all observed accesses give no evidence for one. Existing ChainElem and AnmVmTimerView cover node/timer contracts. For parameters prefer raw uint32 payload words or a documented mode union; value1 is ARGB in kind4, while shake duration interpretation is numeric. A single falsely universal color/float type is unsound.

Render rectangles are (0,0,640,480) or (32,16,416,464). Full-screen43C1A0 flushes cached manager, writes viewport X/Y/W/H at491CF4/491CF8/491CFC/491D00, then calls device491C30 vtable+BC with viewport491CF4. Existing viewport type can describe it, but alias/binding to the actual global object is not proved. Shake outputs are direct globals491E64/491E68. Existing AnmRenderManagerView has screenShakeX/Y, but an arbitrary loaded manager pointer is not automatically equivalent to these absolute globals. Reuse only after proving storage alias. Startup abort4918A4 has existing name g_GameStartupCounter (GameManager.cpp), definition/binding still requires review.

## Behavioral edges worth testing

Rising alpha43C230: frames!=0 -> alpha=trunc(subframe*255/frames) before current reaches frames, else255, clamp negative only. frames==0 leaves alpha unchanged. Expire at current>=frames+2 or nonzero startup counter. Otherwise skip Tick if nonnull GM flags58&5; null GM ticks. Return1 while active,7 on expiration.

Falling43BD40: if frames!=0 compute trunc(255-subframe*255/frames), negative clamp; expire at current>=frames, or startup counter. Kinds0/3 differ in initial alpha, not update body.

Kinds6/7: before release, alpha=trunc(subframe*128/frames) only while frames!=0 and current<=frames, then persists forever. After release, alpha=128-trunc(subframe*16), return7 once current>8. No startup/GM pause check. Release trigger resets timer0.

Kind4: alpha derives from high byte(value1), decreases across period, repeats after decrementing value0 and resetting timer. Finish and startup abort return0, not7. Preserve this distinction, including its lifetime consequence below.

Kind1: tick first, then expire at current>=frames, else interpolate value0+(value1-value0)*subframe/frames. For each axis, advance RNG twice but target uses the second 16-bit result in BOTH halves before modulo3. Four generation increments total. Standard GetRandomU32/AnmRandomU32InRange is NOT equivalent. 0/1/2 choose0/+amplitude/-amplitude. The first output preserves extended x87 amplitude while second consumes it; keep float-rounding differences explicit. Do not silently fix the apparent RNG defect.

Kind8: startup abort returns7; null GM or flags58&0x77 returns1 without tick/RNG/output changes. Otherwise Tick first; envelope uses signed comparisons against payload durations; decay conversion contains unsigned-to-float correction2^32. Two calls to genuine range3 RNG, then outputs. Terminal return7 leaves previous shake values unchanged. Do not zero shake on completion absent caller evidence.

## Lifetime: newly confirmed hazard

Target calc dispatcher449C00 releases critical section before invoking update, reacquires afterward, and dispatches return code. Verified table449D14:0->unregister current node,1->continue,2->repeat,3/4/5->terminate list with1/0/-1,6->restart list,7->call deletedCallback. It caches next node before invoking callbacks. Return7 invokes deletedCallback while holding the critical section; deleted callback calls cleanup, which re-enters that Windows critical section separately for update and draw nodes. Reentrant Windows critical sections matter; a nonrecursive host mutex deadlocks.

Cleanup43CB00 captures node+8 and current global chain before entering lock, unregisters, leaves lock/decrements nesting, then reloads receiver and node+0C AND reloads global chain before the second lock cycle. It does not clear the effect's stored node pointers. UnregisterElem clears callbacks and frees owned nodes; it does NOT invoke deletedCallback. Thus return7 causes cleanup/free once; dispatcher must not dereference current node afterward. Raw pointer double-cleanup is unsafe.

Kind4 returns0: calc dispatcher only unregisters update node, does not invoke delete callback. This leaves the effect and draw node alive, with stale updateNode pointer. Do not pretend all modes share automatic destruction. External ownership or target leak remains unresolved; implementing a nicer lifetime would change target behavior. RemoveAllFromList likewise does not invoke deletion callbacks. Real dispatchers449C00/273 and449D40/241 lack source bodies; effect tests may model their observed protocol but must not claim real-engine scheduler closure.

## Reuse, not new credit

Chain::RegisterCalc44A000/43, RegisterDraw44A030/43, AllocElem449ED0/74, AddToCalcChain449AE0, AddToDrawChain449B70 are already present. UnregisterElem449F60/114 and RemoveAllFromList449E50/82 already exact. Existing AnmVmTimerView has constructor flag clear, Tick, SetCurrent. DrawColoredRectangle43BDA0/483 exists; FlushVertexBuffer442F50/133 exists/exact. AnmRandomU32InRange43CB80/88 exists/exact and is correct for kind8 only. Reuse RngView/GetRandomU16 for kind1 only with explicitly target-matching duplicated-second-half expression, not presumed normal U32. Allocation/free CRT and float conversion runtime are not game-authored candidates.

## Scope decision and blockers

A normal C++ TransitionEffectView plus callbacks registered through existing fastcall ChainCallback is feasible as a semantic candidate. Keep one coherent owner packet, use existing timer/node/render types, and group allocation, complete initialization,11callbacks,release,cleanup together. Include small delete adapter only because real lifetime needs it; no standalone leaf-credit exercise. Preserve target invalid-kind/zero-duration behavior in preconditions rather than fabricate fallbacks.

Blocking exact/native claims: true globals/viewport/shake aliases; authentic EBX factory and ESI initializer boundary under real caller; original TU/authorship; missing real calc/draw dispatchers; mode4 external cleanup/leak; unknown +04/+14 semantics; edge numeric conversion/x87 precision. All extents/tables need canonical complete comparisons, not source-shape claims. No target execution, builds, remote writes, source edits, or new accepted credit occurred.

Evidence: hash-pinned target2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040. Read-only repository commands e33035fd91ac470aa098f5f20cef0c4e (full family decode; locally saved decode.txt), a9e85c3da9c141f0b131ad9f27d455e4 (cleanup/constants/source ledger),47e725dec8d44d4f9cf8265befd488ad (scheduler and ledger),4308b6dcc7ee47c5b03c332baaca09ab (tables). All before/after status digests identical, HEADa743734. Local source baseline50b971f inspected plus current remote StageDraw/AnmManager/Chain references. Older initializer evidence remains in message-runtime-dependency-audit/deeper-decode.txt.
