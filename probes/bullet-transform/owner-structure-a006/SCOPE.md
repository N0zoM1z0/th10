# A006 final bounded structure and named-callee replay

Parent-only execution against the existing `.analysis/bullet1454-native-baseline-20261018-a006` artifacts. Production HEAD remains 75536f63312688610002392067ab8604cd33844b. Complete generated owner remains 1596 bytes with SHA256 f570270d682599a17e29e90025ff2dd32a3690a66b4d3949bb600fc38048e41e. Native image/MAP/PDB/normal-object, overlay, tool, manifest and runner pins are unchanged from the actual A006 receipt. The already executed first-stage collector and A005 package remain frozen.

## Replayed gates

Preserve complete-owner unique MAP/PDB/contribution/normal-COFF identity checks, pinned Capstone 5.0.6, strict recursive control-flow decoding and finite normal-effect allowlist, exact guarded 128-byte remap recognizer, derived pointer extent capped at 20 bytes, and full recovered CFG guard/no-bypass and code/table disjointness checks. Unknown instruction effects, hidden/unsupported prefixes, external branches, overlap, malformed ownership or unexpected dispatches remain unresolved. No helper body is read, decoded or hashed.

The source rederives all actual accounting. It does not reinterpret the 3 unclassified bytes as instructions or padding. The four layout predicates remain accounting metadata and grant no exactness credit.

## Newly pinned generated aliases

Pin link-aliases.rsp SHA256 7af855ebef42efc8d424897afd358bcfff13b212aacdec1f4f9339920ad30e56 and link-anchors.c SHA256 97d1508d5b5d2d5e0511e73e05adb49178e3424d162f13343896db92c3f2561b, observed by actual first-stage command b5ac45dd9c9d43839fb0ee2cda5f560b. Check both before parsing and during final input revalidation.

Retain the bounded exact ASCII/LF generated grammar: 203 sorted unique source names, sequential distinct numbered anchors, and matching four-byte zero-initialized C declarations. Every anchor has a unique readable, non-executable virtual MAP extent and the link-anchors.obj producer. Four-byte anchor extents must not overlap. No anchor bytes are read; virtual-only/BSS storage is valid.

For each recovered direct CALL, accept a generated source identity only when the complete same-address MAP set consists of exactly its declared source symbol and numbered anchor, with identical section, offset, flags and producer strings. The source name must occur only once globally. Missing, additional, duplicate or conflicting rows remain unresolved. A literal function requires one globally unique symbol and one same-address row, matching code/executable PE/MAP identity and virtual/raw coverage. Other ambiguities remain unresolved, and indirect calls prevent a complete named census.

Emit bounded unordered source-name/count dictionaries, aggregate unresolved counts and narrowly qualified exact-helper recovered-call absence only when all recovered calls are direct and named. Anchors are diagnostic data placeholders, never callable implementations. Names/counts do not prove arguments, call order, dynamic behavior, helper semantic absence or callee correctness.

## Three-byte gap proposal

Within the already loaded complete owner, use the rederived partition to report only count/hash, contiguity, placement between the instruction prefix and table suffix, adjacent unconditional control terminator, and absence of recovered CFG entries. Do not inspect or decode the gap contents, read another image range, apply a fill rule or change its unclassified status. This is the stopping point for gap classification.

## Constraints and testing

No build, target execution, remote calls by the author, runtime tracing, native payload export, body inspection, owner cropping, helper-size sums, scoring, acceptance/configuration changes or production edits. Fixed counts, hashes, semantic gates and sanitized symbol metadata are the only exported results.

163 authored synthetic tests cover the retained decoder/closure/ownership checks and new hash pins, exact alias pairs, inconsistent/duplicate/extra rows, virtual-only anchors, literal identities, indirect/unresolved/helper calls, census bounds and gap metadata. Parent invokes the independently approved exact script through the pinned repository Python with `--repo /home/pentester/coding/codex_ida/th10-reconstruction/th10`.
