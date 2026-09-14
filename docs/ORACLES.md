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

The toolchain smoke proves that normal C/C++ COFF, C++ `/GL`, resources, a
PE32 link, and PE/PDB contribution binding execute under headless Wine. It
grants no TH10 source, profile, ownership, or exactness credit. The
normal-COFF comparator is intentionally
narrow because the target's Rich stream also contains LTCG C++ inputs. It
derives each candidate function extent from a COFF function-definition record
or a single-function COMDAT code section; a caller-supplied target size cannot
widen the candidate past its own section.

The LTCG diagnostic compiles one selected maintained source and links it with
unique non-runnable data anchors for missing external symbols. The linker's PDB
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

A canonical `artifact_kind = "linked-pe"` unit uses the same cold compiler and
linker surfaces but adds a target-bound acceptance contract. The comparator
requires one uniquely named `source.ltcg.obj` PDB contribution, checks its full
size, requires the pinned Capstone decoder to consume the whole extent, rejects
any undeclared base-relocation or external control-flow field, validates each
declared semantic symbol through the linker map or its unique data-anchor
alias, and rewrites that field to the reviewed target address before comparing
all bytes. A zero-field unit must therefore be raw-equal. Acceptance establishes
only reproducibility of that bounded function in the declared LTCG context; it
does not make the anchor image runnable or establish production ownership,
translation-unit partition, link order, data ownership, or whole-product
closure.

If a required input, extent, tool profile, or observable cannot be established,
record `unknown` or an open candidate. Accuracy takes precedence over apparent
completion.
