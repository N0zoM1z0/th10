# Chain allocation source and profile controls

2026-10-22, original Japanese TH10 v1.00a only. **No maintained source, canonical
recipe or exact credit changes.** [Facts](facts.json) bind current inputs,
transformations, actual linked owners and complete guard results.

## Independent target and carrier observations

The complete allocator449ED0..449F19 is74 bytes through RET4. Its sole external
CALL field is at3 and targets452493, operator new. Allocation requests24h;
construction is conditional on success, but the subsequent setter still writes
through the null result. No successful-null behavior is invented.

Target flags use MOV ECX,[EAX+4], OR ECX,1 and MOV [EAX+4],ECX around the callback
store, nine bytes. Current native uses a four-byte memory OR, accounting for
the five-byte size gap. Its constructor AND also occurs earlier than target.
These are separate remaining differences; equalizing size alone would not pass.

Fresh local Ghidra checks the74-byte allocator and59-byte owner constructor,
attesting all412289 mapped bytes before/after readOnly/noanalysis/rollback with
CPU1/heap768MiB. Names/types remain provisional. Existing source/header layouts
and callback ABI are unchanged. Adjacent TH09/TH095 constructor/bitfield source
is hypothesis material only; no adjacent layouts or flags become TH10 evidence.

All107 maintained native inputs match the prior actual Chain cold inventory,
and all23 files in its retained first-cold snapshot match their recorded hashes.
Each serial nice10 control compiles one private Chain object and reuses13 game
objects plus attested anchors. This is not a new full14-TU cold. `/GL` remains.

## Executed controls

| Control | Allocator extent | Existing complete Chain guards |
| --- | ---: | --- |
| Explicit full-word flags snapshot before callback store |69|4 exact /838 bytes /58 fields|
| Unmodified source with G7 |69|4 exact /838 bytes /58 fields|
| Ordinary callback-initialization helper |69|4 exact /838 bytes /58 fields|
| Unmodified source with Og disabled |161|All4 rejected on complete extent|
| Compound assignment to the heap bit |69|4 exact /838 bytes /58 fields|

For the four optimized forms, dense69-byte decoding observes one CALL at2.
Ignoring only its four-byte address field, each complete69-byte body is identical
to the baseline69-byte body. The callback helper moves the linked owner; its raw
hash changes only through that CALL displacement. This diagnostic establishes
no target binding or partial match for the74-byte target. All five allocator
comparisons fail closed at the PDB extent check; none passes the complete Oracle.

Og disabled produces insertion extents222/222 versus131/131 and runner
extents372/320 versus308/268. The model is rejected, rather than treating guard
failures as success or changing canonical contracts. The tests do not establish
or exclude original flags, signedness, class declarations, source spelling,
physical TU partition, optimizer phase or allocation/runtime behavior.

## Recovery and next route

The current manifest/producers and selected failure carriers are under
`.analysis/gpt-6.1-sol/chain-allocation-codegen-20261022/` and the corresponding
build campaign. Keep compact transformations and only the necessary failures;
clean duplicate outputs after facts are retained. Preserve the original Chain
cold snapshots, canonical large-function carrier, nine independent D3D files
and provider/legacy state.

Do not repeat these five forms or substitute a normal-COFF profile to force a
fit. A fresh source-presence report still finds124 absent authored mappings:
118 GameUnassigned, three FileSystem, two Player and one Startup. Rotate to
the already reviewed FileSystem4356F0/212,435800/214,4358E0/190 entries and their
current source/caller context. This queue is source-presence triage, not a
boundary, semantic, exactness or product-closure claim.

[Final validation](validation-summary.json): all109 CI groups pass with all369
frozen code/test/config inputs unchanged. Toolchain execution/check, target,
tracking, generated progress and whitespace checks pass. Cleanup removes5338060
logical bytes from21 explicit owned paths, retaining one69-byte carrier and the
161-byte extent failure; original23-file snapshot and nine D3D files are unchanged.
