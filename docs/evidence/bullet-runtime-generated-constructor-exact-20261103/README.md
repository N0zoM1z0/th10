# TH10 BulletRuntimeView implicit constructor at 0x00405D00 — 215/215 raw exact

This checkpoint independently closes the 215-byte, reviewed target
`0x00405D00..0x00405DD6` without hand-writing instructions, copying target
bytes, altering the original executable or claiming the original author
explicitly wrote this C++ constructor. The target's **source-authored vs.
compiler-generated origin remains indeterminate**, as documented before
this result. The maintained class definition is `BulletRuntimeView` in
`src/BulletManager.hpp`; the actual compiler context is the
`src/BulletPoolCreation.cpp` factory's construction of an array of 2001
such records inside the genuine BulletManager allocation.

## Original executable and type evidence

Original Japanese TH10 v1.00a direct instruction inspection confirms an
ECX receiver, explicit EBX/ESI/EDI save/restore, timer-active flag clears,
and exactly the following observable member operations:

- initialize the embedded `AnmVmView vm` at bullet offset +0x008. The
  original inline VM constructor clears the nine timer flags, then zeros
  exactly 0x3AC bytes and assigns `activeSpriteIndex` -1;
- clear the two bullet timer active flags at offsets +0x408/+0x41C;
- clear the nine extension-state timers at 0x614, stepping 0x34 bytes;
- return the original bullet pointer unmodified through EAX.

The existing typed `BulletRuntimeView`, `AnmVmView`, `AnmVmTimerView` and
`BulletExStateView` members reproduce this sequence by natural default
construction. This **215-byte** routine is a single original function,
not a whole-array helper or the distinct 34-byte optional-buffer destructor
at 0x00405DE0 (which was independently exact before this checkpoint).

## Two independent source-native codegen proofs

The selected graph is **exactly** the already-proven 182-byte
`bullet-pool-create` factory: pinned VC7.1 SP1 `/GL /GS /EHs`, primary
`BulletPoolCreation.cpp` plus unchanged real `Chain.cpp`, `AnmManager.cpp`,
`GameErrorContext.cpp`, `ScriptViewerLifecycle.cpp` support sources. The
factory remains the real LTCG entry. The compiler emits a separate,
uniquely PDB-owned `??0BulletRuntimeView@@QAE@XZ` contribution in the
primary object's linked module. It is **215 bytes**, equal to the target
physical extent, and the complete byte arrays are **raw-equal**. There are
**zero** code relocations/normalization fields: no related address is
masked, assumed or borrowed from a different source owner.

Two independent zero-reuse cold linked PEs have different SHA-256 and PDB
GUIDs. Each strict canonical comparison yields `result: exact`,
`matched_bytes: 215`, `size: 215`, `raw_equal: true` and zero differences.
Retained private local reports:
`.analysis/gpt-web/20261103/bullet-runtime-ctor-cold{1,2}.json`.

The constructor's exact codegen does not imply original C++ syntax, full
retail product/runtime behavior, or a separate exactness claim for the 2001
record vector helper (`_eh_vector_constructor_iterator_`). Existing
`BulletManagerView::Initialize` 163 bytes, `CreateBulletManager` 182 bytes,
and `BulletRuntimeView::~BulletRuntimeView` 34 bytes are independently
credited and are not counted twice. This adds **215 new exact bytes**.
Only these affected exact units are cold-replayed, never a full cohort.
