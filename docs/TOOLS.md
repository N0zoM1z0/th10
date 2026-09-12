# Tool routing

| Need | Command or provider | Authority |
| --- | --- | --- |
| Verify the private target | `python3 scripts/verify-target.py` | Target identity, PE structure, and Rich record stream |
| Verify shared analyzer surfaces | `python3 scripts/verify-analysis-tools.py` | Selected Ghidra/JDK file identity |
| Attest and execute VC7.1 SP1 | `python3 scripts/verify-toolchain.py --execute` | Hash/banner identity plus headless normal-COFF, LTCG, resource, and PE32-link smoke |
| Compile a source/profile hypothesis | `scripts/compile-probe.sh SOURCE OUTPUT.obj FLAG...` | Compiler observation only |
| Build a canonical normal-COFF unit | `python3 scripts/build-match-unit.py --unit NAME` | Forced compile, no exactness by itself |
| Compare a canonical normal-COFF unit | `python3 scripts/compare-coff-function.py --unit NAME --json` | Complete target bytes and declared relocation replay |
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
normal C, normal C++, and LTCG C++ inputs. The current exact comparator accepts
only `artifact_kind = "coff"` units and rejects `/GL`; an LTCG hypothesis
must remain non-accepted until a linked-image extent Oracle is implemented.

The original executable is copied once from the Windows installation to the
ignored `resources/th10.exe`. Normal Factory work neither searches nor mounts
the Windows drive.
