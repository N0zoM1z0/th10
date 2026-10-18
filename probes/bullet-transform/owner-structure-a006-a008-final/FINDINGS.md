# Final A006/A008 generated-owner findings

## Result and provenance

Actual parent-run command `repository-command:e691beb3d0a24ffda8148f0b424e00dc` completed successfully, with unchanged clean HEAD `e534a6f6c77115017e613721353308a41cde9b6a`, valid post-run pins, no build and no target/runtime execution. Its full 21,520-byte sanitized stdout was reassembled from pages without truncation. The executed collector SHA is `d83174351cca49ac99628920e5247c0161f66ca3598ba88ea040850f2da094ca`; it is frozen. The accompanying receipt SHA is `589503d3d3ab55fbf31f4e3a3f425baa700afebd93bf26c2367d11663b79d4df`.

All three complete generated-owner attribution and mapping gates passed before slicing, followed by exact owner hashes before decoding. Native image/MAP/PDB/object and original manifest pins are unchanged. There is no target-owner expansion or callee-body expansion. The A008 transform is independently authorized as its whole 1612-byte owner.

## Complete-owner accounting

- A006 runtime: all 885 bytes are recovered instructions; 228 instructions, 254 CFG edges, 20 named direct calls, 3 returns and no unclassified bytes.
- A008 runtime: all 884 bytes are recovered instructions; the same 228 instructions by count, 254 CFG edges, 20 named direct calls, 3 returns and no unclassified bytes. Its unordered named-call counts equal A006's.
- A008 transform: 1612 = 1461 recovered instruction bytes + 128 remap bytes + 20 pointer bytes + 3 unclassified bytes. It has 332 instructions, 376 CFG edges, 16 named direct calls and one return. Calls include two genuine QueueSoundSample and one genuine QueueSoundCue. No direct call remains unresolved.

A008 transform's five selected dispatch destinations close inside the owner; full-CFG guard no-bypass and code/table disjointness pass. Instruction bytes form a contiguous prefix; tables form a contiguous trailing suffix, with pointer table before remap and tables ending at owner end. These are accounting predicates only. The three remaining bytes stay uninterpreted and unclassified; no fill claim is made.

A008's experimental mandatory full-885 guard failed in its original build. This diagnostic does not revise that result or grant any exactness credit. Equal instruction/call counts do not prove equal instruction behavior or identify the source of the one-byte owner difference.

## Bounded Cue comparison

Each runtime has a uniquely attributed direct call to genuine QueueSoundCue. Both bounded four-instruction corridors stop at an opaque call. The finite model establishes the diagnostic receiver-symbol address in ESI, source sound ID 28 in EBX and one prepared pre-call stack word in each build. A008's alias and anchor source hashes match the approved A007 pair and pass fresh exact MAP pairing.

Neither corridor establishes the prepared value's origin from incoming-base field 0x3B4. Intended bullet identity remains unknown, and both full combined patterns remain unestablished. Matching bounded Cue predicates does not establish identical whole-call behavior. A diagnostic anchor address is not real sound storage.

## Bounded Advance comparison and return syntax

Both unique AdvanceTransformProgram call preparations comprise exactly two ordinary instructions starting at a genuine merge boundary and ending immediately before the call. Their preceding merged state is not resolved. Under the explicit valid, writable, nonwrapping private-stack premise:

- A006: ECX equals corridor-entry EBP; one pre-call stack word is prepared. That word's equality to corridor-entry EBP is not established. The unordered instruction families are one MOV and one PUSH.
- A008: two pre-call stack words are prepared; the top word equals corridor-entry EBP. Equality of both words to that origin is not established, and ECX equality to that origin is not established. The unordered families are two PUSH instructions.

These are corridor-entry value-origin predicates. They do not identify an object, receiver, formal parameter, function-entry value or the intended bullet. A false predicate records failure to establish an identity; it is not a proof that concrete values differ.

The sole recovered A008 transform return has exact near-RET syntax with an explicit additional-four-byte immediate. This is an instruction-syntax category only. It does not establish function-wide stack balance, which prepared word is consumed, caller cleanup, saved registers, a formal calling convention or the complete cause of the owner-byte difference.

## Stop and remaining proof gap

The compiler-context issue is parked. No further local ABI proof expansion, unknown-X-origin investigation, callee-body read, native build, padding/size tuning or translation-unit/profile sweep is authorized by this packet. A later corrective experiment needs a separate source-backed hypothesis and review.

Remaining unknowns include the merged incoming object's identity, formal argument consumption, complete stack/clobber behavior, runtime sound semantics, and the full cause of the one-byte runtime mismatch. This evidence does not authorize production source/configuration adoption or acceptance changes. Owners are never cropped or combined with helpers for scoring.

## Package validation

261 authored synthetic tests passed independently before execution. The package contains only the frozen collector, tests, scope, these findings and the sanitized actual receipt, plus their manifest. Native bytes, disassembly, decompilation, ordered traces, callsite addresses and numeric register values are absent. Cache directories and intermediate files are excluded.
