# Independent native member-constructor baseline review

2026-10-17. Read-only inspection of parent-built `build/diagnostic-gui-message-member-ehsc`; no reviewer build, remote source edit, executable run or acceptance action. This is the specifically authorized isolated `/EHsc` diagnostic. Canonical configuration remains unchanged; the earlier unchanged-profile recommendation does not prohibit this disclosed one-profile delta.

## Result first

All three intended owners are retained naturally, including the real static caller without noinline. All three are NON-EXACT in this baseline. Constructor and helper lengths differ; the equal-length caller has genuine instruction ordering differences. The natural construction-failure storage cleanup topology is emitted, but this anchored diagnostic cannot establish runnable EH or target execution.

| Owner | Native complete extent | Target complete extent | Finding |
|---|---|---|---|
| GuiMessageVmView(record) | 415470–41572D,702 bytes | 415B00–415DAA,683 bytes | Non-exact; actual member constructor retained |
| EnemyKillAll | 4152D0–415463,404 bytes | 40E6A0–40E755,182 bytes | Non-exact; world-effect dependency substantially inlined |
| EnemyReadDialog | 401830–4018AB,124 bytes | 415DB0–415E2B,124 bytes | Non-exact despite equal length; naturally retained static caller |

Constructor/helper identities come from map publics joined with complete PDB section contributions and validated PE/PDB identity. Caller is in the map's **Static symbols** section at section1+830. A same-start PDB code contribution establishes size124 and `source.ltcg.obj` ownership; the public-only inventory correctly leaves it unowned, not absent. Full code through each contribution's RET was decoded. LLVM18 pdbutil's optional symbol dump crashes on this older PDB's type stream; it supplied no symbol evidence. The existing parser's PDB contribution data plus static map identity is the successful bounded fallback. No map-neighbor subtraction was used as the caller extent.

## Artifact identity

Independently verified hashes:

- PE: 8dc9005019bc43f5cbec6b38bf3b93c491c2e8a80f43639c4913da7de6c98ec6
- Map: 2dbccbac523c537214de0df7d0bf9e9ddbb5a3cfffd1379e09dc7fcdd0a88a0d
- PDB: 119aac0e4d8bddad6352fe144f5a28691e355a5e2901a9a6e7bd36c5153e1fde
- Constructor raw contribution: 045e56b5f18f21fe2daab79dc2e1ee9c4efbc9b7d1007b9e3aba79dfb4c420fb
- Helper raw contribution: 5fff8b161069c89aa304921f1fdf4b9035c43f6d466be3f825582c686cd12436

member-build.json identifies six real source TUs, canonical enemy-mark-pending-interrupt profile plus only /EHsc, unresolved-symbol-data-anchors-v1 harness, and linked-diagnostic-only/no acceptance authority. Enemy.cpp and EnemyCallback.cpp are not part of this graph. The review queries left parent source/index status unchanged.

## Caller, actual root and cleanup

The actual ECL route at404E2A loads canonical GUI pointer into EDI;404E30 moves index into ESI;404E32 calls401830. Later bullet-clear/laser-clear/helper work remains in that opcode after return. This is the real source call graph, not a synthetic constructor root.

Caller401830 has target-compatible private live-ins EDI=GuiView and ESI=index, no stack parameters, RET. It establishes EH registration, requests90 bytes from operator-new anchor417000, saves the allocation in its local slot, tests it, sets state0, and only then loads gui.messageFile+9EBC and table[index*8+4]. It pushes record and allocation and calls the actual constructor415470. The null branch still produces zero then reaches the unconditional messageIndex write; null behavior remains outside the valid execution domain.

Native caller then loads g_EnemyDialogIndex at401879 **before** publishing gui.message at40187F and writing messageIndex at401885. Target does its publication/index stores at415DF9/415DFF **before** loading the global index at415E01. This six-byte load relocation within the instruction sequence is a real non-exact difference after address rebinding, not just different link addresses. Both store the new global index before conditionally clearing dialog state. Do not add volatile/opaque ordering mechanisms merely to force this schedule.

The caller's EH handler4157FB loads FuncInfo41638C and jumps to CxxFrameHandler anchor417238. Metadata has magic19930520, one unwind state, map416384, zero try blocks and zero IP map. Sole entry is {-1,4157F0}. Cleanup4157F0 loads `[ebp-10h]`, pushes it, calls operator-delete anchor417004, pops ECX and returns. It has no whole-message destructor or effect rollback. State0 remains installed from before table evaluation through publication/global stores until unregister, matching target's observed state-region topology. Target equivalents are handler46582B, FuncInfo47180C, map471804, action465820 and delete4524A1.

Important boundary: `__except_list` is unresolved and bound to data anchor417240, so emitted registration uses **FS:[417240]**, while target uses FS:[0]. New/delete/CxxFrameHandler are also data anchors, not callable runtime implementations. This file must not be executed to test EH. The review establishes compiler-emitted cleanup identity and topology only; it does not prove arbitrary-SEH equivalence, exception runtime operation, or target execution.

## Constructor full-body observations

The member constructor naturally lowers to storage and record on stack, returns storage in EAX and RET8, matching the target private boundary without a forced calling convention. Prologue saves EBX in addition to target's other registers. It emits the three timer-bit clears, then explicit six ID-zero construction stores, then full90-byte clear and timer resets. Target does not retain these six pre-clear ID stores. Extra retained member construction and different register/scheduling choices already prevent exactness; the net size difference is19 bytes, not a claim that any single sequence explains all differences.

All reviewed body stages remain present: instruction store; two text resource/VM allocation/init/insertion paths; manager reload/cache pattern; four separate FindVm calls with clear-ID-then-dereference null branches; glyph16 stores; text vectors/colors; exactly2000 bullet records with state0/3 skips and stride7F0; canonical laser pointer+18, saved next+08 before slot5 call atvtable+14 with mode0; helper call through canonical enemy pointer; storage return. The maintained timer flags read remains visible before clearing and remains a representation-validity limitation, not original-source UB evidence.

## Helper full-body observations

The argument remains one stack pointer with RET4; EDI holds the traversal/timer owner. Node next is saved before effects. Flags maskC050 and bit80 override, signed death-script test, global enemy resource lookup, post-effect OR20000 and final argument timer advance remain visible. Resource comes from global41719C, not substituted with argument owner.

The world-effect creation's allocation, coordinate setup, script execution and insertion/ID handling are inlined into this helper, while target calls448DB0. This accounts for a major structural expansion, independently of instruction/register differences; no retention attribute is recommended to reverse it. The retained global resource lookup and node-next temporary are still inspectable.

Timer constants at4161D0/4161CC have bits3F7D70A4/3F8147AE (strict0.99/1.01 interval), and4161E4 is1.0f. Native x87 comparisons use the same unordered-rejecting tests. Fast path separately increments integer and float. Slow path FLD scale/FADD subframe/FST subframe retains the x87 value for the conversion call, then stores integer current, as in target. Conversion itself is an unresolved anchor; no conversion-runtime equivalence is claimed.

## Evidence and remaining gates

Read-only commands: 2c909457aefa426188bccc836e441432 (report),59fa802185ad45bf8b45e9b31595c3ff (hashes/parser),39e7e5c8a1024ad7964da293d4cc2c65 (complete main decodes/contributions; paginated tail retained). Additional EH/root outputs are local handoff evidence, not repository patch files, retained as native-member-eh.json and native-member-root.json; full main decode in native-member-decode-head.json/tail.json. These preserve exact query IDs/statuses.

No accepted exactness credit is indicated. Do not count same caller length as exact; do not count host seams as production constructor/alias validity; do not count anchored native linkage as runtime validation. Existing view casts still have ISO alias questions despite real host lifetimes and explicit -fno-strict-aliasing. Preserve target pointer identity and separate compiler/ABI evidence rather than rewriting pointers to copies to hide the issue. Production acceptance remains subject to parent's scoped regressions and canonical exact replay, with the timer representation and runtime dependencies explicitly open.

## Bounded design-debt note: six pre-clear ID stores

The target entry415B00–415B2B clears the three timer initialization bits and immediately performs the90-byte REP STOS. There are no separate writes to six ID slots40..54 before that clear. Native415490–4154AB writes zero to those six slots before its REP STOS at4154B7. The current Gui.hpp declares `AnmVmIdView vmIds[6]`; current AnmVmId.hpp supplies `AnmVmIdView() { value = 0; }`, an integer-value constructor, four-byte storage assertion, methods and no user-defined destructor. The real record constructor's empty member-initializer list therefore invokes six default constructors before its body. This is an actual maintained-source initialization promise, not an ABI obligation inferred from the ID methods.

These six writes are ordinary nonvolatile stores, with no external call or read of the IDs before the body overwrites the same bytes. Ignoring the separately unresolved timer-read problem, removing the redundant stores is a valid optimization. Thus target absence is compatible both with dead-store elimination of zeroing member constructors and with a different original ID representation/default-construction policy. The observed compiler retained them in this precise graph; this is concrete codegen evidence, but does not establish a compiler defect, a required optimization flag, or uniquely identify the original source type. Calling it merely a scheduling problem is too strong, and declaring the member-construction model disproved is also too strong.

Documented reference evidence is narrower than a default-constructor proof. KNOWLEDGE_BASE ANM-048/049/056 records target-exact ID methods, and ANM-057 records a four-byte wrapper/temporary ABI distinction; none establishes that every containing TH10 object default-zeroed its ID members before its own clear. ANM-048 explicitly treats TH095/TH08 source organization as hypothesis references. No independently verified cross-version constructor-type evidence was available for this bounded review, and none was used to override TH10 target evidence.

The six AnmVmIdView zero assignments themselves are source-language-valid initialization and create no resource ownership. Proper member lifetime does not require zero initialization: a different, independently justified representation could have valid lifetimes with scalar values initialized later by the bulk clear. Conversely, replacing these fields with raw/trivial stand-ins solely to reduce the native size would discard maintained type/interface assumptions without proof. Keep source frozen; do not weaken shared default constructors, invent a special uninitialized-ID type or apply retention/profile changes for this observation.

This debt is separate from the timer issue. ID writes initialize a scalar without reading its old value; maintained timer `flags &= ~1u` reads indeterminate raw integer state. The actual native AND instructions do not prove the original source did so; an individual bitfield initialization remains a hypothesis. Neither constructor-seamed host success, valid outer/member lifetimes, nor removal of redundant ID stores would resolve production timer or cross-view alias legality.

Stopping condition for this follow-up: ambiguity documented, no patch or new build. Reopen only with independent TH10 evidence from ID construction in other complete owners where a following clear cannot hide initialization, or a specifically verified relevant reference type; then review shared consumer/lifetime implications as a separate bounded task. The present target absence alone does not authorize a production member-type change.

## Bounded ID-construction debt

The six pre-clear ID stores come from the maintained AnmVmIdView default
constructor's valid zero initialization. Proper member lifetime does not itself
require those zero stores, and eliminating them before the bulk clear would be
a valid optimization, separate from the unresolved timer read. Their absence in
target is compatible with optimization or a different original ID initialization
policy; this one graph does not decide between them. Existing exact ID methods
and four-byte wrapper ABI evidence do not prove universal default-zero policy.
Keep source frozen: no raw stand-in, special uninitialized type, shared constructor
weakening or profile tuning follows. Reopen only with independent TH10 owner or
verified reference-type evidence and a separate shared-consumer/lifetime review.
