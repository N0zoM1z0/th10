# A006/A007 bounded runtime Cue findings

## Actual observed difference

The unique genuine QueueSoundCue call has a different bounded caller setup in A007:

- **A006:** the modeled ESI carrier equals the diagnostic sound-owner symbol address; EBX is established as source ID 28; one top stack word is prepared. The unordered four-instruction corridor contains three MOVs and one PUSH.
- **A007:** the modeled ECX carrier equals the diagnostic sound-owner symbol address; two top stack words are prepared. The unordered four-instruction corridor contains two MOVs and two PUSHes. The bounded model does not establish EBX as ID 28.

Both corridors stop at the preceding opaque call. **Neither corridor establishes the prepared field 0x3B4 value's origin, or identifies an incoming base as the bullet. Both full combined carrier/field patterns remain unproved.** A false fixed predicate means the bounded symbolic model did not establish it; it is not proof that the concrete value is different.

This establishes a changed bounded caller setup. It does not establish formal callee argument consumption, a calling convention, return cleanup, real sound storage, runtime sound semantics, or the cause of all seven bytes of the complete-owner change. The immediate-successor categories are syntactic only. No further expansion is planned for this diagnostic.

## Complete-owner context

The actual final replay revalidated both complete runtime owners and closed their CFGs: A006 885 bytes/228 instructions and A007 878 bytes/225 instructions, with no unclassified bytes. Each retains 20 named direct calls, including exactly one QueueSoundCue call. Whole owner hashes remain d1fe8601b9f3e2d742b5f35a99e88e63a5d7096cb23a28dc92830c1aece47853 and a6de1b825ce0a215431f5823b7c40e2c992c0721c6f2f30ec7f9c86fa49395f7 respectively.

Unique Cue MAP/PDB/producer metadata passed without a callee-body read. Both receiver identities used independently pinned generated alias sources and exact consistent source/numbered-anchor MAP pairs. A diagnostic data anchor is not real owner storage or a runnable implementation. No anchor data was read.

## Receipt and frozen source

- Actual final command: be76b7ea9cff49059734cbb0205457a7, completed successfully, clean before/after, unchanged production HEAD 75536f63312688610002392067ab8604cd33844b, no output truncation.
- Frozen executed source: inspect_cue_preparation.py, SHA256 2c493306e1dfb983fbc00b23eadf21ffbe1562c7801d3c189f78504900c9cd49. Independent review approved this exact source; 208 authored synthetic tests pass.
- Sanitized final receipt: remote-cue-preparation-receipt.json, SHA256 820fc611a53c0c79419cb2e224fa4189ce67cf227ca04cbec462134443c6bae7.
- Earlier first-stage command 137a843e28104982a03d7587a47f9dd0 established the three-owner aggregate/callee metadata. This final replay narrowed its body reads to the two already authorized runtime owners.

## Premises and stopping point

The model assumes immutable caller code, ordinary flat32 execution and normally returning opaque calls; a valid, nonwrapping private stack disjoint from input memory and diagnostic storage; stable readable source memory; and a valid x87 stack with sufficient free capacity for every modeled FLD and no stack faults. It claims no bitwise or NaN equivalence. The observed four-instruction corridors required only MOV/PUSH forms; their value-origin gap remains unresolved.

The source delta and unchanged runtime source's existing Cue call provide a concrete shared linkage path. The observed setup difference can inform a separately reviewed compiler-boundary hypothesis, but does not authorize or validate one. A007's failed unchanged 885-byte guard remains failed. No owner is cropped or combined, no size-based exactness credit is granted, and no source/configuration/acceptance changes follow from this report. Native bytes, disassembly, decompilation and ordered traces remain remote.

Package: final diagnostic source, authored tests, scope plan, these findings, sanitized receipt and manifest only. No caches, intermediate source copies or native payloads.
