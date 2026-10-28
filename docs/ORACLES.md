# Oracle policy

An Oracle is a replayable mechanism that can falsify a bounded claim. No single
view supplies every kind of truth.

| Claim | Minimum useful Oracle | Acceptance boundary |
| --- | --- | --- |
| Target identity | size, SHA-256/MD5, PE structure | exact manifest agreement |
| Ghidra routing | Ghidra metadata, entry point, distributed mapped bytes | exact target agreement before analysis |
| Boundary/ownership | target control flow, references, tables, adjacent bytes | complete reviewed extent; unknown if ambiguous |
| Compiler generation | PE/Rich evidence plus hash/banner-pinned headless compiler probe | observed generation/build and supported artifact families, not per-unit flags |
| Source presence | maintained mapping and source file | no exactness credit |
| Normal-COFF codegen exactness | cold pinned historical-compiler object build, complete bytes and relocations | zero difference for full accepted extent |
| LTCG structural diagnosis | pinned compiler/linker, PE-bound PDB contribution extent, complete linked bytes and resolved-field inventory | routes canonical-unit review; no exactness credit |
| LTCG codegen exactness | cold pinned compiler/linker, PE/map/PDB-bound complete contribution, exhaustive decoded link-field manifest and target replay | zero difference for the full accepted extent; diagnostic image remains non-product |
| Native product closure | cold compile/link graph, imports/resources/data owners | no missing required production owner |
| Runtime semantics | deterministic scenarios against original and native products | both Oracles agree on declared observables |
| Port behavior | native baseline plus port scenario | separate portability claim |

Ghidra is the semantic-analysis provider, not an independent exactness Oracle.
Decompiler text, names, types, and candidate extents remain provisional. A
successful build or visually plausible runtime also cannot waive missing bytes,
relocations, data owners, or scenario observables.

Full mapped-text hashing uses the inclusive manifest range401000..465A80:
412289 bytes (64A81h), SHA-256
cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f.
Derive the size from text_virtual_size or end-minus-start-plus-one. Older local
review scripts hashed64A80h bytes, yielding
c1611c9becb832a4e2ba456afc58218fb5b8fafd6c85d19e3addbebb0c946e0a;
that is a prefix ending465A7F, not a complete mapped-text digest. The shared
ghidra.py distributed attestation already uses the inclusive end and its final
16-byte probe covers465A80. The2026-10-20 result-row review separately reran
the complete corrected memory hash with read-only rollback.

The toolchain smoke proves that normal C/C++ COFF, C++ `/GL`, resources, a
PE32 link, and PE/PDB contribution binding execute under headless Wine. It
grants no TH10 source, profile, ownership, or exactness credit. The
normal-COFF comparator is intentionally
narrow because the target's Rich stream also contains LTCG C++ inputs. It
derives each candidate function extent from a COFF function-definition record
or a single-function COMDAT code section; a caller-supplied target size cannot
widen the candidate past its own section.

The `artifact_kind = "coff-archive"` lane is reserved for source-less static
runtime bodies whose complete normal-COFF member is independently hash-attested
from the pinned VC7.1 archive. Its unit records the exact archive member,
symbol extent, every COFF relocation and the target address resolved for each
relocation; cold replay extracts the member afresh and applies those target
values before comparing the full extent. This grants byte exactness only for
that bounded library body. It does not create a maintained source mapping,
change the library/excluded origin, or establish whole-product closure.

The LTCG diagnostic compiles one selected maintained source and may also
compile explicitly named support sources before linking them together. Support
sources use the same `/GL` profile by default. A selected LTCG support can use
an explicit `support_profiles` source-to-switch-array override; each override
must retain `/GL`, cannot name a normal support, and participates in build and
replay grouping. These reproduced profiles do not identify original production
flags. A reviewed production partition may instead require a declared normal
non-`/GL` support subset. This lets the linker
observe real cross-TU definitions and the selected optimizer visibility.
Remaining external symbols receive unique
non-runnable data anchors. The linker's PDB
supplies each contribution extent. PE CodeView GUID/age, map timestamp/image
base, public start, and PE section layout must all agree. Link-address fields
are enumerated and masked only for structural comparison. Because the harness
is incomplete and does not establish production ownership or link context, its
results always carry `acceptance_authority=none`.

LTCG can change a function's private register ABI according to its real caller.
When the default first-backlog-function entry suppresses that context, pass a
reviewed source function explicitly with
`--entry SOURCE=SOURCE_NAME`. The tool resolves the source name to one external
VC7 symbol, rejects absent or ambiguous entries, and records the selection in
JSON. The entry must be real maintained source; it is still a diagnostic link
choice and grants no ownership or exactness credit by itself.

When a candidate depends on a separately maintained translation unit, pass
`--support SOURCE=SUPPORT_SOURCE`. A target-supported normal-COFF partition
may additionally use `--normal-support SOURCE=SUPPORT_SOURCE`. Support sources
must be repository-local files, must be unique, and never change which PDB
contribution owns the function under comparison. A canonical linked unit
records the same context with `support_sources = [...]` and, when needed,
`normal_support_sources = [...]`; build grouping and cold replay include both.

A canonical `artifact_kind = "linked-pe"` unit uses the same cold compiler and
linker surfaces but adds a target-bound acceptance contract. The comparator
requires one uniquely named `source.ltcg.obj` PDB contribution, checks its full
size, requires the pinned Capstone decoder to consume the instruction extent,
and accepts an undecoded suffix only when every byte is covered by complete,
authoritative PE base-relocation fields or explicitly reviewed trailing data
annotations. This supports PDB code contributions ending in absolute jump tables
and selectors while rejecting unannotated undecoded bytes. The comparator
rejects any undeclared
base-relocation or external control-flow field, validates each declared semantic
symbol through the linker map or its unique data-anchor alias, and rewrites that
field to the reviewed target address before comparing all bytes. A zero-field
unit must therefore be raw-equal. Acceptance establishes only reproducibility
of that bounded function in the declared LTCG context; it does not make the
anchor image runnable or establish production ownership, translation-unit
partition, link order, data ownership, or whole-product closure.

An explicitly local address must preserve its destination offset within the
compared contribution. After accounting for the declared addend, both semantic
bases must stay inside their respective contributions and have equal relative
offsets. Being internal does not authorize normalizing a different code label,
table slot or data position. The synthetic swapped-table and shifted-base
controls in `scripts/test-linked-local-fields.py` guard this requirement.
See the [easing extent and local-field audit](evidence/easing-tail-ownership/README.md).

When decoding stops in an owned absolute jump table, complete authoritative PE
DIR32 fields may bridge the gap to an explicitly declared unlinked data suffix.
Those annotations remain contiguous, trailing, disjoint from decoded bytes and
all link fields; partial words and relative fields cannot fill coverage gaps.
Field inventory and semantic binding remain exhaustive, and every data byte is
still compared. ECL Run additionally validates its reviewed60-entry table and88
selectors before deriving an annotation. See the
[table coverage controls](evidence/ecl-current-dependencies/README.md).

`diagnose-ecl-run-native.py --context-manifest PATH --unit NAME` selects an
explicit diagnostic build context without changing canonical recipes. Target
bindings and callee templates always come from canonical match-units.toml;
the context cannot replace them. Both files are fingerprinted and must remain
unchanged during the comparison. The complete callee,189 fields/62 local
destinations and table/selector coverage requirements remain unchanged.
Artifact producer/source pins are separate evidence: the command does not
assert that an existing image was built from the supplied context or current
source. See the [real scale support proof](evidence/enemy-real-scale-support/README.md).

For an isolated private ECL input, `--ecl-source PATH` selects its PDB source
owner explicitly. The file must exist inside the repository and occur exactly
once among the context's primary/support inputs; the default remains
`src/EclVm.cpp`. The selected source and both manifests must remain unchanged
during diagnosis. The reported source hash is an observation, not proof that
the artifact was built from that input. Target bindings, the full callee,
189 fields/62 local destinations and table/selector coverage are unchanged.
See [source routing and layout controls](evidence/ecl-run-layout-controls/README.md).

A direct REL32 call to a static callee may explicitly declare both
`static_source` and `static_size`. This opt-in derives the selected object from
the unit's source/support list and uses the existing qualified MAP/PDB owner
checks. In the default lane, the callee must have a complete reviewed target
boundary, dense zero-field decoding and raw equality across its whole extent; the decoded
caller must CALL its exact entry. Partial bodies, linked child fields, interior
entries, JMPs and implicit public-to-static fallback are rejected. This binds
the caller field without adding duplicate credit for the callee. See the
[script browser lifecycle evidence](evidence/script-viewer-native-lifecycle/README.md).

A field-bearing static CALL may additionally opt in with
`static_callee_unit = "unit-name"`. The referenced unit must be a complete,
qualified static owner with the same entry, source, symbol and extent as the
linkage, and the identical primary/support/profile/link/entry/harness graph and
output directory. Its maintained source/name mapping and reviewed target
boundary must agree. The canonical engine recursively proves every callee
field, fixed local destination and ordinary byte in the same PE/MAP/PDB;
artifact hashes and PDB identity must remain unchanged throughout that proof.
The caller still must CALL the exact entry and compare every owned byte.
Unknown references, cycles, partial or different-artifact proofs, ambiguous
owners, interior entries and tail-jump use of this option are rejected. The
report retains the complete nested proof rather than granting extra callee
credit through the caller. Without this key, the legacy zero-field lane is
unchanged. See the [D3D real-callee proof](evidence/d3d-real-callees-20261024/README.md).

A static REL32 tail jump uses the same whole raw-equal zero-field callee checks
and additionally declares `static_tail_jump = true`. This explicit lane requires
a decoded unconditional JMP to the qualified entry, a complete reviewed target
boundary and a matching maintained target source/name mapping. CALLs, conditional
branches, interior entries, missing selectors and nonliteral opt-in values are
rejected. Without this flag, the original static CALL policy is unchanged.
Every caller byte is still compared; the flag grants no original source-ABI or
native-runtime claim. The Hint split helper uses this for its two trim exits.

Target origin is recorded independently from codegen exactness. A canonical
unit requires a maintained source mapping and reviewed target boundary, but an
unknown authored-versus-outlined origin remains unknown after an exact replay.
The exact claim covers only the complete target extent in the declared source
and compiler context.

An absolute DIR32 address of an ordinary TU-static C++ function uses a separate
explicit descriptor: `static_address_source`, `static_address_candidate_size`
and `static_address_target_size`. Both extents must be complete dense code;
the target must have a reviewed boundary and a maintained source mapping whose
TU and unqualified name agree with the exact qualified MAP/PDB symbol. The
field must name the candidate function entry, with no addend, local mode or
implicit static lookup. Source-to-object selection comes from the fixed build
graph. Different declared candidate/target sizes are permitted because this is
**address binding only**, not callee byte equality. The report labels that scope
explicitly; the enclosing parent still replays every field and compares every
owned byte. Callback bytes receive no credit from that parent replay. This lane
does not weaken either direct static CALL lane above: both require a complete
callee byte proof, not just an address. See the
[transition initializer proof](evidence/effect-initializer/README.md).

If a required input, extent, tool profile, or observable cannot be established,
record `unknown` or an open candidate. Accuracy takes precedence over apparent
completion.

## TH10 authored-byte metric

The existing progress metric sums `functions.csv.size` for authored rows and
`matches.csv.size` for authored exact rows. These are reviewed owned `.text`
extents, not an instruction-only count: a switch table and internal alignment
are included when target-local ownership review assigns them to that function.
The four exact ANM operand helpers, EclVm Run, Enemy dispatch, and the
ENEMY-076/PLAYER-029/ITEM-007 boundary corrections use this convention. A nearby
table with unresolved ownership remains outside the function until reviewed.

Keep executable-code and owned-data portions explicit in the evidence.
`compare_size` can cover a larger compiler artifact without crediting every
compared byte to one claim. It cannot justify ending a code boundary inside an
instruction. The indeterminate404E90 leaf's old15-byte credit/17-byte comparison
is superseded by a complete17-byte boundary and ordinary17-byte unit; see the
[RET4 extent correction](evidence/leaf-compare-extent/README.md). This is not an
instruction-only authored-byte policy. A boundary correction can increase the
authored denominator with zero exact credit.

## One-allocation EH entry binding

A linked DIR32 field may explicitly opt into `allocation_eh_handler = true`
for the supported VC7.1 single-allocation cleanup shape. Its exact parent-named
MAP static entry must join one unaliased, complete21-byte generated PDB region
in the same object/module as the already bound parent. Both images must expose
the immutable one-state FuncInfo, saved storage atEBP-10, scalar-delete action
and CxxFrameHandler dispatch protocol. Ordinary public/static selectors remain
unchanged. This supplies an address identity for the complete parent replay;
it grants no child-code, metadata ownership or EH-runtime credit. Unsupported
shapes stay unknown. See [the replay factory proof and limitations](evidence/replay-load/README.md).

## Stack-file EH entry binding and metadata extent

A DIR32 field can explicitly select `stack_file_eh_handler = true` for the
reviewed one-state automatic CPbgFile cleanup shape. The parent's unaliased
MAP handler must join one complete 18-byte generated PDB code contribution in
that same parent object/module. Its eight-byte action loads ECX from EBP-0x18
and tail-jumps to the real, qualified 46-byte CPbgFile destructor contribution;
the ten-byte entry selects FuncInfo and jumps to CxxFrameHandler. An anchor
cannot replace the destructor. Both images must expose immutable metadata and
the same one-state cleanup protocol. Allocation, static and stack-file selectors
are separate opt-ins and cannot be combined. Other stack objects, offsets,
cleanup instructions and metadata shapes remain unsupported.

The supported 0x19930520 one-state FuncInfo header is exactly seven words/28 bytes.
Compiler output places the Read header at the end of its readonly section;
GetSize's following word is the next UnwindMap state in both target and native
images. Reading an eighth word incorrectly constrains adjacent data. Both EH
readers now stop at28 bytes. Synthetic section-end and arbitrary-neighbor
positives protect that boundary; malformed headers and short ranges still fail.
This correction adds no exact credit or metadata ownership claim. The stack-file
lane likewise supplies address identity only; the enclosing owner must still
compare every byte and every field. See the
[archive size proof](evidence/filesystem-path-size/README.md).

## Ending worker-member EH address binding

The opt-in `worker_member_eh_handler = true` is restricted to the named Ending
text constructor's DIR32 handler field at+3. Its unique unaliased MAP entry
must join one complete24-byte generated PDB region in the same parent module
and object. Both images require immutable one-state28-byte FuncInfo, the exact
unwind action, saved this atEBP+4, member offsetD0, real worker destructor tail
and CxxFrameHandler dispatch. Allocation, stack-file and static selectors cannot
combine with it. The real16-byte worker destructor must match all ordinary bytes
and its two semantically bound fields; the qualified101-byte Stop contribution
and real vtable/Stop symbols cannot be replaced by anchors. Every parent byte,
field and extent still passes the normal comparator. The selector supplies
handler address identity only, with no generated-code, metadata, storage or
EH-runtime credit. See [the complete Ending proof](evidence/ending-id-storage-20261028/README.md).
