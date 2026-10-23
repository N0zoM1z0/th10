# Player startup: real dependencies and shared storage

2026-10-23, starting 13047bf. PlayerInitialize now uses its real Chain, resource,
error-log and loaded-shot-data dependencies. Its complete linked candidate is
1435 bytes, the target size, but remains nonexact: independent replay of all49
address fields matches1123 bytes and leaves312 differences. No exact ledger or
canonical recipe changes. Windows product/runtime and original ownership remain
open.

## Target evidence and source changes

Read-only/noanalysis Ghidra plus raw target decoding confirm the startup's actual
calls to PreloadAnm447280, GameErrorContext::Log44B810, Chain allocation449ED0,
calc/draw insertion449AE0/449B70 and the existing shared VM initializer404F30.
The original declaration-only interfaces could not supply the real linked ABIs.

The required .sht loader426520/229 now has natural source in Player.cpp. It loads
through the existing FileSystem::OpenFile, preserves the four observed diagonal-
speed writes and fixes descriptor offsets and callback indices in place. Its
sole observed caller and the real linked graph agree on ESI Player/EAX filename.
The four callback-table bases are target observations; lengths, initialization
owners and the +2C callback's meaning remain unknown. Its native229-byte size
does not establish exactness: float instruction order and alignment still differ.
This source closure is a dependency of the large owner, not a small exact claim.
TH08/TH09 loaders supplied hypotheses only; TH10's four writes and offsets were
checked locally rather than inherited from either game.

Existing GameScoreState storage is bound at474C40. Its unknown28 byte region
covers the selection words at+28/+2C. Reading those words through references to
that existing storage restores target-like reloads across float stores; the
separate extern scalar spelling cached the character index. This supports an
aggregate-alias hypothesis, not an original class or global ownership claim.
No shared header or data definition changes.

Eight existing AnmFloat3 value operations recover the target's genuine12-byte
temporary frame. Chaining each extent's x/y assignment then repairs exactly six
target bytes, with no target-equal losses. The source adds neither padding nor
dummy writes. New layout assertions retain the existing node views and selection
storage offset. [The maintained patch](maintained.patch) records the full change.

## Complete-parent evidence and controls

The [evidence](evidence.json) pins sources, profiles, actual compilations, complete
PDB owners and six extent-store repairs. [All312 differences and49 bindings](parent-differences.json)
are retained. Target destinations are derived from reviewed calls/data operands,
canonical shared-storage bindings and exact runtime identity, independently of
candidate byte positions. Both complete destination inventories agree.

An address inside GameScoreState's diagnostic anchor can numerically equal a
neighboring unrelated anchor. Such fields bind GameScoreState+28/+2C explicitly;
they must not be relabeled as that neighbor merely because MAP gives its address.
No ordinary bytes or local control destinations are masked by the full comparison.

| Actual private trial | Complete parent / result |
| --- | --- |
| Real resource/error bindings, loader still opaque,15 inputs | 1401; only one shared log call |
| Also real loader/FileSystem/archive support,17 inputs | 1415; target-like loader ABI and both log calls |
| Read selection through existing shared storage | 1435; full bound comparison1117/1435 |
| Chain extent x/y stores, then readable selection references | 1435;1123/1435; six repairs/no losses |
| Node assignment reorder | Four changed bytes remain different; no target repairs or losses |
| Partial node views / flag snapshot | Complete parent neutral after49 independent bindings |
| History pointer loop / option-value locals | 1435;1104 /1115 matching; rejected |
| Reordered index expression / loader multiplication / Player-only Ob1 | Complete parent neutral after49 bindings |
| Existing timer members, before/after selection repair | 1421/1439; regressive; not adopted |

Private and maintained builds reproduce all519 uniquely enumerated complete
public PDB owners raw. This does not cover static/unowned fragments or prove
target equality. Private trials compile one Player input and reuse pinned support
objects; some early trials also compile the newly added real supports. These are
not independent whole-graph cold pairs. Source/profiles remain hash-pinned.

## Validation and resume

The maintained17-input image passes seven full shared-dependency contracts:
1480 bytes/all99 fields, including FileSystem::OpenFile, Log, PreloadAnm and the
four Chain owners. A separate current-source cold rebuild of the original
canonical14-input recipe passes all four Chain contracts838/all58 in one build.
Neither comparison grants Player exactness. No full CI or runtime is claimed.
See [validation](validation-summary.json) for the actual completed scope.

Private recovery is `.analysis/gpt-6.1-sol/player-services-20261023`.
Use manifest-final.json, stage-common-final.py, probe-maintained.py and
bound-parent-final.py for current-source work. Historical manifest/producer pins
are retained; do not restore old maintained source or weaken them. Keep
`build/gpt-6.1-sol/player-services-20261023/maintained-17`, the full-services17
support cache and prior-chain-canonical. The cleanup inventory documents removed
duplicate supports and their retained identical replacements.

Next attack the complete312-byte residual: callback setup/VM receiver, history
addressing, first data-pointer scheduling, one commuted float operand and the
tail timer/index schedule. Preserve all1435 bytes and all49 bindings. The shared
VM initializer is still native200/target202 with EDX/ESI receiver; prior caller/
formal controls are historical exclusions, not proof of its original signature.
Do not replace the large-owner objective with a standalone loader exact claim.
