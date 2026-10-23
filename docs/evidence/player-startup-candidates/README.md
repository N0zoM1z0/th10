# Player startup candidates recovered for handoff

Historical checkpoint13047bf. Current maintained work is recorded in
[the real-dependency packet](../player-startup-real-dependencies/README.md).

2026-10-23, maintained source 040f1e8. These experiments concern the complete
1435-byte PlayerInitialize owner at 0x4247F0. No candidate is adopted and no exact
ledger changes. Maintained Player remains 1116 bytes in the previously measured
17-TU graph. This packet recovers the final completed experiment before cleanup.

## Observations and measured controls

The retained target decode and read-only Ghidra review identify two allocations
at 0x42485B/0x424886 calling Chain::AllocElem at 0x449ED0, followed by insertions at
0x424879/0x4248A0 calling AddToCalcChain/AddToDrawChain at 0x449AE0/0x449B70. The
manager pointer is loaded from 0x491BE4, with priorities 0x10/0x16. The existing
Chain declarations and layout supply a natural replacement for three Player
declaration-only interfaces. This does not establish global object ownership.

The target reserves 12 scratch bytes and performs eight three-component value
calculations through that temporary storage before copying results to Player.
Its additions load the extent before the position; subtractions use position
minus extent. Its option-data accesses reload Player+0x45C after timer setup.
Existing AnmFloat3View value operators are a source-shape hypothesis for these
observations, rather than an authorization to manufacture spills or padding.

| Private trial, same 14-TU recipe | Complete parent size | Masked positional diagnostic | Scratch frame |
| --- | --- | --- | --- |
| typed-nodes: bind existing Chain allocation/insertion | 1116 | 143/1267 | absent |
| value-extent-left: add eight real Float3 value operations | 1366 | 76/1275 | 12 bytes |
| value-current-data: also reload optionData after timer setup | 1388 | 103/1275 | 12 bytes |

All three retain four complete Chain guards: 838 bytes/all 58 linked fields.
Each parent calls five real implementations: two AllocElem calls, one calc
insertion, one draw insertion and the shared VM initializer. Native AllocElem
and the shared initializer still have differing target sizes. The source
bindings do not make those callees exact.

The final value-current-data job completed normally; its formerly uncollected
log is now reconciled with its report and artifacts. Each trial compiles only
its private Player.cpp with the recorded /GL /GS profile and reuses 13 pinned
objects from the canonical Chain graph. It then relinks the image. These are
not independent full cold builds. Size proximity and masked positional scores
do not establish an exact improvement, exhaustive field binding or runtime.

## Resume route and retained inputs

The [evidence](evidence.json) pins the actual private inputs, producers, complete
PDB owners, diagnostic scores, guards, call edges and all measured artifacts.
Five adjacent patches apply independently to Player.cpp at source040f1e8:
three measured candidates, plus **value-pos-left** and **bound-services**, which
are prepared but **not executed**. The latter introduces the existing PreloadAnm
and GameErrorContext::Log implementations and requires its additional real
support source; clean up its stale interface comment before measuring it.

Private producers and inputs remain at
`.analysis/gpt-6.1-sol/player-chain-bindings-20261023`. The complete latest native
carrier is `build/gpt-6.1-sol/player-chain-bindings-20261023/value-current-data-14`.
The two earlier carriers retain their image/map/PDB and unique Player object;
only their 26 combined duplicate reused objects are removed. The cleanup receipt
names each identical retained replacement. Preserve the original manifests;
do not relax source/target pins or overwrite a completed trial directory.

Continue with the large Player owner: measure the real resource/error bindings,
then inspect complete-parent residuals. The remaining option-data interface
targets 0x426520/229; its physical ESI Player/EAX filename boundary needs actual
source/caller evidence. It is a dependency of this large-owner investigation,
not a separate small-function exact goal. Whole-parent exactness remains open.
Stage, ANM, ECL and Enemy remain available large-owner routes.
