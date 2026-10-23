# ANM ExecuteScript translation-unit partition

2026-10-23, starting56edff0. A consistent private eleven-source /GL graph moves
the entire ExecuteScript definition byte-for-byte into a separate TU. No
maintained source, header, ABI, recipe or exact claim changes.

The original ANM core retains all other definitions in order. The executor TU
uses the same includes, matching random-helper declaration, and the identical
AnmScriptVectorView declaration/size assertion. Its entire function body is
hash-verified equal to maintained source. All signatures and flags are retained.
Two ANM objects are compiled successfully overall; nine canonical game objects
are hash-verified and reused. This is an incremental eleven-source experiment,
not a full cold or evidence of original production partitioning.

## Completed result

ANM9964, ECL7020 and Enemy14336 retain every complete peer byte after exhaustive
field replay. ANM has298 changed address fields, Enemy24, ECL none; every changed
field resolves to a common qualified map/static symbol or equal owned offset.
No unresolved field or ordinary peer byte remains. ANM stays7541/7995,90/92 sizes,
gap2: FlipX53/52, common1440/1441 and groups49/53/54 unchanged. This executor-only
split does not repair the residual. All36 whole guards4618/all121
pass; ECL remains6993/7020 with189 fields and exact171-byte callee. Eight bounded
candidate child-return paths pass. No parent or case-level exactness is granted.

## Recovery details

The first executor compile rejected a missing TU-local vector-view declaration;
no link or comparison existed for that attempt. The successful core object and
nine unchanged canonical objects were retained and hash-verified. Recovery adds
the identical declaration and compiles only the corrected executor before link.
The first wrapper still exits1 after all native comparisons: the auxiliary
source parser assumes a following member definition and rejects a standalone TU.

No native rebuild or acceptance-rule change is used to bypass that parse error.
The exact source bytes used to compile the complete function are first verified
equal to the maintained definition. The unchanged route checker then uses that explicitly bound
source view for enum/case projection and the real split image for its complete
bounded CFG checks. The collected report names the actual executor input and
records the projection. These completed checks and full peer comparison exit0;
the earlier failed steps are not relabeled as successful.

## Retention and reproduction

[Evidence](evidence.json) preserves actual source/object hashes, the eleven-source
context, failed/recovered stages, exact guard scope, whole-function projection,
peer replay and cleanup. Private logs, overlays and actual producers remain in
.analysis/gpt-6.1-sol/anm-execute-partition-20261023. probe.py records the initial
missing-declaration attempt; finish.py records the incremental recovery.

A clean reproduce.py is prepared there for a fresh unused label, for example
`reproduce.py separate-execute-replay`. It incorporates both recovered source
handling steps and compiles both ANM objects. That prepared driver has not been
executed; the observations above come from the separately recorded actual stages.
Never restore maintained source or loosen frozen hashes to rerun a historical
manifest against newer inputs.

All17 new native files were hash-verified and removed, reclaiming1098060 bytes.
Canonical artifacts,108 maintained sources, pinned headers/recipes and nine
unrelated D3D files remain unchanged. Native work is serial nice10; same-session
Ghidra verifies all412289 mapped bytes with readOnly/noanalysis/rollback,
maxcpu1/heap768m. No full CI or product/runtime proof is claimed. Exact counts
remain unchanged; investigate a distinct real caller/entry context next rather
than repeating this complete-body-preserving split.
