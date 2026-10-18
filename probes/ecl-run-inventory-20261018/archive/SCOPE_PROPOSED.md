# Run7020 bounded discrepancy inventory — proposed, not executable

Status: source-only scope proposal. Independent reviewer accepts this bounded scope subject to exact implementation review and parent execution approval. No remote execution, build, mutation, or implementation authorized by this document. Parent executes only a separately reviewed, pinned verifier. Independent scope reviewer: /root/review_missing_input_contracts.

## Question and useful decision

The two whole-owner hashes already establish raw inequality. Raw totals alone locate its coarse partition but do not identify a new source property. In the same bounded read, structural triage can establish whether any fixed-position prefix mismatches lie outside the candidate's **canonical recognized field union**. The union covers PE DIR32 relocations and external direct-control immediate fields recognized by the pinned linked_code_fields implementation. It is not proof of all possible linker influence, matching instruction semantics, or provenance of each differing byte. Target-side relocation or other linkage effects remain possible outside the candidate union; residual differences are not automatically source-semantic differences.

A positive outside-field mismatch count warrants a later separately reviewed source-level investigation of other differences. Zero does not prove linkage explains the discrepancy, semantic equivalence, or exactness. A nonzero relative-table destination mismatch count indicates that physical case destinations differ relative to their respective owner starts; it prevents assuming same-offset case correspondence. No individual offsets or native details are exported.

## Frozen inputs and attribution

Use the actual unchanged diagnostic baseline from command feeeb3abe6ee47d7a38fc05aedbb57dd, not a cancelled/unbuilt candidate. Source HEAD a4edda20e434cd40ee99ea3b6b68673a80c3e3be must remain clean before and after. Candidate owner is exactly 7020 bytes, SHA256 98f613dc9838da5b5879304e255c32409b8a0dbb9141f24a7c6d1cd66e42ce39; target owner is exactly 7020 bytes, SHA256 b802e20de41d15a9bd59d3b7110f99840e56e5d40f27a2cda159b883d9fe3667. Both hashes are mandatory gates, not merely output values.

Reuse the strict Run ownership, PE/map timestamp/base agreement, PDB CodeView GUID/age, unique exact source.ltcg.obj contribution/module, virtual/raw/file bounds, no interior function symbols or overlapping contribution, exact root/target paths and symlink rejection from op82_baseline_order.py SHA256 782dfc0d32d6b708f158a47e0314725a3148517ddae74c0b40cdc1df50f7c6e5. Preserve all source/config/tool/decoder pins, baseline artifact pins, frozen manifest/runner pins, and candidate copied source/tool pins. Recheck every input after measurement and recheck clean HEAD. Helpers/alias provenance may be retained as preexisting guards but no helper bytes are read or analyzed; no local call-order proof is run or claimed.

## Exact whole-byte accounting

Each owner partitions without gap or overlap into prefix6692, table240, selector88. Prefix is initially a suffix-derived positional partition, not a proven code classification; keep assumed_prefix_partition true even after full linear decode validates syntactic coverage; decoding does not establish code/data classification or reachability. It includes the two trailing bytes attributed to alignment by the pinned target layout; no bytes are dropped. Compare equal-length corresponding owner positions without rewriting, shifting, searching for alignments, or masking. Compute raw byte mismatch counts for each partition and whole owner. Require their sum to equal whole mismatch count and partition lengths to sum to7020. No percentage, score, normalized hash, normalized exactness, case-credit sum, or adoption authority.

## Bounded structural triage

Use pinned canonical linked_code_fields only on candidate prefix6692. Do not use normalization_complete as a substitute for successful decode: decoded_bytes must equal6692. Independently perform a contiguous full Capstone walk of both prefixes, including alignment, with no skipped byte, positive instruction sizes, and exact end. At most6692 instructions per prefix,6692 fields/relocations intersecting each prefix,60 table entries and88 selectors; reject budget exhaustion. Fail closed on decode ambiguity or undecoded remainder.

Canonical returned fields must have positive width, lie wholly in prefix, and be pairwise disjoint. Reconstruct the canonical union from authoritative intersecting PE DIR32 relocations and decoded external immediate CALL/JUMP fields; require exact correspondence, including duplicate/overlap rejection. Require a DIR32 field to be the exact four-byte encoded immediate or displacement extent within one decoded instruction; every relative control field must be the exact encoded immediate extent of its own instruction. No arbitrary operand is added. Relocations crossing prefix boundaries fail triage. Internal branches are not masked. No native fields, operands, registers, instruction text, addresses, offsets, relocation arrays or ordered traces leave the remote.

Calculate only recognized_field_byte_count, prefix_mismatch_inside_recognized_fields, prefix_mismatch_outside_recognized_fields; require inside+outside==raw_prefix_mismatch and field+nonfield==6692. These are raw fixed-position counts classified by candidate field membership. Do not modify either byte buffer. Set explicit structural_triage_only=true and linkage_exhaustiveness_proved=false. Full decoding is syntactic coverage, not reachability or semantic proof.

## Minimal table-shape diagnostic

Retain target source/selector/active-default coverage and candidate suffix/table-shape validation. Decode both prefixes completely as above. Require all60 destinations unique, within the executable portion before trailing alignment, and at decoded instruction starts. Compare all60 destination-minus-own-origin integers in original table-index correspondence internally. Export only relative_destination_difference_count in0..60 and relative_destinations_all_equal. No destination list, source order, per-opcode spans, instruction counts by opcode, or correspondence inference is emitted. This does not establish dispatcher association. Per-opcode mapping is expressly deferred.

## Output and failure policy

Fixed schema only: boolean flags, bounded counts and preapproved hashes. No arbitrary strings, exceptions, paths or native-derived text. At most8192 output bytes. Suppress imported tool stdout/stderr and errors. Raw counts have raw_inventory_valid; triage counts have structural_triage_valid; table count has relative_table_comparison_valid. Invalid subresults use zero placeholders but their validity flag remains false. A failed global provenance/pre/postpin/resource gate invalidates every measurement and clears measurement counts. Preserve failure booleans without publishing partial evidence as valid.

## Resources and stop

Read-only, one CPU, nice increment10, wall58seconds (parent timeout60), CPU soft29/hard30seconds, address space768MiB, file size0, no pycache. No native execution, compiler, build driver, new baseline, subprocess sweep, external CodexCLI, assembly export, register experiments, padding changes, profile/TU sweep or mutation. Preserve bounded parser subprocess execution as in reviewed attribution implementation.

Stop after one successful reviewed measurement, or on any input/provenance/resource/decode/field-shape failure. No automatic relaxed fallback, no case sweep, no further decode scope, and no rebuild. Report the observed new structural property or precise unavailable validity class to parent. Any next measurement requires a new reviewed scope and must be motivated by this result.

## Required source-only tests before execution review

Synthetic inputs only: exact whole partition accounting; one mutation in each region; interior/exterior field mismatches; empty/full field unions; malformed or partial decode; crossing, duplicate, overlapping, zero-width and non-operand fields; missing canonical field; relocation/direct-control overlap; missing input/postpin failure clears counts; target-owner hash mismatch; instruction-start/interior/alignment/out-of-range/duplicate destination; distinct owner origins with equal relative destinations; relative mismatch count bounds; nonzero case offsets never exported; fixed-output schema/size and accidental-text suppression. Existing frozen verifier remains untouched.
