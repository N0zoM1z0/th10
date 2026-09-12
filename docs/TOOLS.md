# Tool routing

| Need | Command or provider | Authority |
| --- | --- | --- |
| Verify the private target | `python3 scripts/verify-target.py` | Target identity, PE structure, and Rich record stream |
| Verify shared analyzer surfaces | `python3 scripts/verify-analysis-tools.py` | Selected Ghidra/JDK file identity |
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

The original executable is copied once from the Windows installation to the
ignored `resources/th10.exe`. Normal Factory work neither searches nor mounts
the Windows drive.
