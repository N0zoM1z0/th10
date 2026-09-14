# Tool routing

| Need | Command or provider | Authority |
| --- | --- | --- |
| Verify the private target | `python3 scripts/verify-target.py` | Target identity, PE structure, and Rich record stream |
| Verify shared analyzer surfaces | `python3 scripts/verify-analysis-tools.py` | Selected Ghidra/JDK file identity |
| Attest and execute VC7.1 SP1 | `python3 scripts/verify-toolchain.py --execute` | Hash/banner identity plus headless normal-COFF, LTCG, resource, PE32 link, and PE/PDB extent smoke |
| Compile a source/profile hypothesis | `scripts/compile-probe.sh SOURCE OUTPUT.obj FLAG...` | Compiler observation only |
| Compare a normal-COFF source probe | `python3 scripts/compare-coff-function.py OBJECT SYMBOL ADDRESS SIZE --json` | Relocation-masked diagnostic plus target-derived relocation candidates; no acceptance authority |
| Enumerate normal-COFF functions | `python3 scripts/compare-coff-function.py OBJECT --list-functions [--contains TEXT] [--json]` | Exact decorated symbol, section extent and relocation count for probe setup; no acceptance authority |
| Build a canonical compiler unit | `python3 scripts/build-match-unit.py --unit NAME` | Cold normal-COFF compile or `/GL` compile/link, no exactness by itself |
| Compare a canonical normal-COFF unit | `python3 scripts/compare-coff-function.py --unit NAME --json` | Complete target bytes and declared relocation replay |
| Compare a canonical linked-PE unit | `python3 scripts/compare-linked-function.py --unit NAME --json` | PE/map/PDB-bound complete extent, exhaustive linked-field validation and target replay |
| Cold-replay canonical exact units | `python3 scripts/replay-exact-units.py [--source SOURCE | --unit NAME]` | One cold build per shared artifact context followed by strict comparison of every selected COFF or linked-PE unit |
| List the source-present exact backlog | `python3 scripts/report-exact-backlog.py [--source SOURCE] [--module MODULE] [--state authored\|origin-review\|excluded\|all] [--json]` | Triage-only joined view of non-exact source mappings; default excludes origin-pending entries |
| Rank the exact backlog in both codegen lanes | `python3 scripts/rank-exact-backlog.py [--source SOURCE] [--limit N] [--json]` | Fresh normal-COFF and LTCG probes joined to the same current backlog and ranked by explicit size/byte distance; no acceptance authority |
| Batch-probe the authored exact backlog | `python3 scripts/probe-exact-backlog.py [--source SOURCE] [--show RESULT] [--json]` | One cold normal-COFF compile per source plus strict diagnostic comparison; no acceptance authority |
| Inspect linked function extents | `python3 scripts/inspect-linked-functions.py IMAGE MAP PDB [--object TEXT] [--contains TEXT] [--json]` | PE/map/PDB-bound public functions whose sizes come from DBI section contributions; no acceptance authority |
| Batch-probe LTCG backlog | `python3 scripts/probe-ltcg-backlog.py [--source SOURCE] [--entry SOURCE=NAME] [--support SOURCE=SUPPORT_SOURCE] [--show RESULT] [--json]` | Cold one- or multi-source `/GL` compile and diagnostic link, PDB-owned extents, and target structural comparison; no acceptance authority |
| Initialize private Ghidra project | `python3 scripts/ghidra.py import` | Operator-only bootstrap; provisional inventory |
| Attest private Ghidra project | `python3 scripts/ghidra.py check` | Target/project binding, no exactness credit |
| Discover/call Ghidra | Factory provider `th10-ghidra` | Target-attested provisional semantic analysis |
| Validate ledgers | `python3 scripts/validate-tracking.py --require-target` | Internal consistency and target binding |
| Report progress | `python3 scripts/report-reconstruction-status.py` | Ledger-derived status |
| Validate public checkout | `python3 scripts/ci.py` | Small target-independent checks |

The public game repository contains no MCP server and has no game-specific
public URL. GPT-web uses the one shared Factory MCP and selects repository
`th10` plus provider `th10-ghidra`.

Ghidra 12.1.3 and Temurin JDK 21.0.12.1 are installed once in private
Factory-managed storage. The ignored `.tools/ghidra` and `.tools/jdk` links
select those immutable shared installations. The mutable project remains
game-specific at `ghidra-project/TH10`.

VC7.1 SP1 is likewise one immutable shared payload selected by the ignored
`.tools/msvc710-sp1` link. The tracked lock binds repository commit
`cf62606064633dd8441aa2feffe34792099cc366`, the compiler/linker build-6030
banners, and all code-generation/resource executables used by the smoke path.
The upstream repository declares no license; it is an operator-supplied
compatibility input and is never redistributed here.

Wine is a shared host runtime, but `.tools/wineprefix-msvc710-sp1` belongs
only to TH10. `scripts/run-headless-wine.sh` always launches Windows commands
under Xvfb with a win32 prefix and disables Gecko/Mono prompts. Set
`TH10_MSVC71_ROOT`, `TH10_WINEPREFIX`, or `WINE` only for an explicit
local override; receipts bind those selector values and tool files.

The target is not a uniform standalone-COFF build. Rich product IDs establish
normal C, normal C++, and LTCG C++ inputs. The standalone comparator accepts
only `artifact_kind = "coff"` units and rejects `/GL`. The linked LTCG probe
recovers exact candidate extents from a PDB bound to the PE by its CodeView
GUID/age and to the linker map by timestamp, image base, public start, and PE
section layout. Its unresolved-symbol data-anchor harness is intentionally
non-runnable, and probe results have no acceptance authority. A reviewed
`artifact_kind = "linked-pe"` unit can claim bounded exactness only through the
separate canonical comparator: its PDB contribution must cover the complete
declared extent, the hash-pinned Capstone decoder must enumerate every linked
field, the manifest must bind every field to a semantic target, and target
replay must leave zero differences. The linked harness never receives product
closure or runtime credit.
Probe-mode relocation targets are decoded from the selected target window to
make a prospective canonical manifest reviewable. They remain candidates until
the symbol meaning, complete extent, and relocation ownership are reviewed and
the resulting manifest unit replays with zero differences. Candidate code size
comes only from its COFF function definition or a single-function COMDAT code
section; requested target length never extends the candidate into the next
section. Probe records produced before this invariant was added must be rerun
before their object-size or comparable-byte fields are used as current
evidence.

The original executable is copied once from the Windows installation to the
ignored `resources/th10.exe`. Normal Factory work neither searches nor mounts
the Windows drive.
