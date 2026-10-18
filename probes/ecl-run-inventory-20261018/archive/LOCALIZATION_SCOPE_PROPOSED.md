# Source-only proposal: localize139 residual positions

Status: independent scope agreement received; implementation has not begun. No native access, remote execution or build. Exact implementation review is still required; parent alone may execute a later exactly reviewed verifier. A docs-only checkpoint may change HEAD; wait for parent-supplied new HEAD and confirmed unchanged source/native pins before freezing a successor.

## Motivation

Actual inventory command e8404d04f7554c088119f7331971d970 established139 differing prefix positions outside recognized candidate fields, with all60 relative table destinations equal. A single same-input read-only localization could identify a physical region deserving source-level investigation, rather than proposing another source spelling for already-matching local op82 ordering. The original inventory alone does not localize these139 positions or prove op82's whole physical span matches.

## Preserve and tighten all gates

Retain entire7020 owners/hashes, strict Run attribution/PDB identity, target and candidate source/tool/config/artifact pins, parent-reviewed HEAD, symlink refusal, complete linear decoding, canonical candidate field validation, all postpins and immutable diagnostic interpretation. Recompute raw totals and field residual on the same buffers and require their prior fixed values787/607/180/0 and468/139; require recognized field coverage516. Require all60 own-origin relative table destinations equal, unique, before trailing alignment and at decoded instruction starts on both sides. Pin any new HEAD only from verified parent metadata; old a4ed receipt remains the artifact/source baseline, not a license to ignore repository changes.

Require identical selectors and exact source coverage. Strengthen to prove the59 active opcode selector values form a permutation of table indices0..58; selector59 has29 source opcode values and denotes the single shared default destination. If this one-to-one binding fails, stop rather than silently duplicating or guessing interval attribution.

## Complete physical partition, internal only

Internally sort the60 distinct relative destinations. Form initial interval[0,first destination) and one half-open interval per destination, ending at the next destination or prefix end6692 for the last. Initial interval may be empty. Require strict destination ordering, all boundaries decoded instruction starts except the exact prefix end, no overlap/gap and total coverage6692. Include both trailing alignment bytes in the final physical span; never discard them or count them separately twice. Require no recognized field crosses any interval boundary. The60 relative destinations are equal across images, so compare identical fixed-position intervals; perform no search, shifting, alignment optimization, normalized rewriting or semantic matching.

Labels are lexical, conditional physical table-entry spans only. Each opcode/count record means residual bytes in its selector-associated physical interval, not bytes semantically owned or executed by that opcode. Reordering a list for presentation does not establish runtime dispatcher association, code/data classification, reachability, case-body ownership or equivalence. A span can contain shared continuation or epilogue material; its chosen opcode label only identifies the pinned table-entry starting it.

## Minimal output

Keep global raw totals, residual totals, validities and preapproved hashes from the first inventory. Add only:
- localization_valid Boolean
- initial_span_outside_candidate_fields_difference_count
- shared_default_span_outside_candidate_fields_difference_count
- exactly59 records sorted by public source opcode value, each containing that opcode value and its outside-recognized-candidate-fields difference count

Do not export physical span order, offsets, native addresses, lengths, native bytes, instruction/register data, field lists or traces. Default is counted once, not repeated29 times. Require initial count + shared default count +59 active span counts ==139. Keep whole raw owner total787 separately and explicitly: localization is not a score or per-case byte-credit sum. No per-opcode normalized/raw exactness booleans, ranking, or source adoption claim. Counts identify candidate investigation regions only. Keep output within8192 bytes; if schema exceeds the cap during source-only tests, reduce redundant static metadata only after review, never truncate result records or increase limit unreviewed.

## Failure, bounds and stopping

If any localization gate fails, suppress every localized count and record, set localization_valid=false and stop; globally invalid provenance/postpins suppress all measurements. No permissive fallback or partial successful region list. Preserve oneCPU/nice10, CPU soft29/hard30, wall58/outer60,768MiB address space, zero file writes/bytecode and fixed count/Boolean/hash output. No build/native execution or expanded disassembly scope beyond the already reviewed two6692-byte prefixes. No external CodexCLI, assembly/padding/register/profile/TU experiments.

Stop after one reviewed successful localization or any failure. Then choose at most a source-only investigation motivated by a nonzero region; do not automatically patch, sweep opcodes or rerun measurements. Future source changes/builds require a separate bounded proposal and review.

## Synthetic tests required later

Permutation/default alias rejection; destination equality and instruction-boundary rejection; empty initial interval; alignment bytes accounted in final interval; exact partition6692; field-boundary crossing rejection; all139 differences placed in initial/default/one active/multiple spans; sum139 and global787 preserved; opcode-sorted presentation independent of physical order; shared default counted once; no native-order/offset/length leakage; localization and global postpin failures suppress outputs; updated HEAD/source distinction enforced; fixed schema complete under8192 bytes.
