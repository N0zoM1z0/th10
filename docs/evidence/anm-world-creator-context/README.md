# ANM world-creator context review (2026-10-19)

Scope: four complete118-byte world-coordinate VM creators and the shared
203-byte `InitializeAndExecuteScriptIndex`. **None is exact. All source/API
trials are restored; this checkpoint adds zero exact bytes.**

| Owner | Target | Complete size |
| --- | --- | ---: |
| CreateVmAtWorldVariant0 | 0x448DB0 | 118 |
| CreateVmAtWorldVariant1 | 0x449010 | 118 |
| CreateVmAtWorldVariant2 | 0x448EE0 | 118 |
| CreateVmAtWorldVariant3 | 0x449140 | 118 |
| InitializeAndExecuteScriptIndex | 0x43E710 | 203 |

## Target observations

Local hash-attested Ghidra agrees with all four full creator extents, including
three functions temporarily defined inside a rolled-back transaction under
`-readOnly`. Every creator receives position in EDI, uses three stack words
for resource, hidden ID result storage and index, and ends in RET12. The target
captures allocated VM in ESI, reads its flags, clears render layer, ORs bit30
and stores flags. It adds binary32 constants224/16 to position X/Y, copies Z,
starts the script, reloads the global manager and registers the VM in the
corresponding list. Resource/VM/index reach the shared start helper through
ECX/EAX/EBX. Source class names and original compiler/TU ownership remain unknown.

`target-facts.json` pins the target, mapped `.text` attestation, complete code
hashes and each creator's seven independently reviewed address fields. The
translation constant payloads are checked separately. Existing reviewed
boundaries/origins are unchanged. Ghidra's incomplete incoming-reference lists
are not treated as proof that a sibling has no callers.

## Complete strict candidates

`candidate-recipes.toml` is explicitly **noncanonical**. It binds all28 creator
fields and all four shared-helper fields to independently reviewed target
addresses. The existing strict linked comparator validates every field and
decodes every complete extent, but correctly returns `mismatch` for all five.
`strict-candidate-results.json` records the full results.

In the existing single-TU `/GL` screen-creator family, each creator has the
target118-byte size and private interface. Thirteen ordinary bytes differ
inside the flags/layer scheduling interval: source-generated OR/store precedes
layer zero, while the target writes layer zero between flag load and OR/store.
Full replay therefore gives105/118 bytes for each creator, with all seven
fields accounted for. This is neither exactness nor partial-owner credit.

The shared helper has the target203-byte size and202/203 strict replay bytes.
Its sole ordinary difference at +0x27 changes `MOV EDX,EDI` to `MOV ECX,EDI`
before calling Initialize. This context uses Initialize itself as the linker
entry; its emitted275-byte receiver-ECX body differs from target273/EDX.
Binding the callee's address cannot waive the unlike receiver instruction.
No whole-product claim can combine these separate-context observations.

The four existing screen creators95 each and SetupVertexBuffer472 pass
**852 complete bytes and42 target-bound fields** in that actual retained image.
Their recipes are explicitly adapted as same-image guards; canonical manifests
and existing exact totals are unchanged.

## Bounded controls

All builds are cold VC7.1 SP1 CL13.10.6030 / LINK7.10.6030 diagnostic links.
All inputs retain `/GL`; unresolved anchors make these images nonrunnable.
`native-summary.json` records the exact entry, profile, support order, frozen
CPP/quoted-header inputs, image identities and complete-owner diagnostics.

| Context/control | World creator /118 | Shared helper /203 | Result |
| --- | ---: | ---: | --- |
| Four-TU ExecuteScript `/GL /GS` | 116 | 197 | Private registers differ |
| Same, layer write moved before flags | 116 | 197 | Entire four creator bodies byte-identical; restored |
| Real EnemyKillAll plus four ANM TUs | 116 | 197 | Creator work inlined into caller; caller404/182 |
| Above, retain observed world0 call boundary | 116 | 197 | Caller183/182, creators unchanged; annotation restored |
| Existing single-TU screen-family profile/entry | 118 | 203 | Thirteen and one ordinary byte differences respectively |
| Same, actual flags local before layer write | 118 | 203 | Same diagnostic mismatch counts; restored |
| Same, existing useUnitSpeed bitfield | 118 | 203 | Same diagnostic mismatch counts; restored |
| Same profile, real screen0 creator as entry | 116 | 197 | Initialize273 raw-equal, helper coalesces VM into EDX |
| Above, ordinary resource/VM/index free helper | 116 | 197 | No improvement; all five API/source files restored |

The four-TU graph is primary AnmManager, then RandomMath, AnmVmCreate and
AnmVmId, using the already declared ExecuteScript `/GL /GS` recipe. The real
caller graph is primary GuiMessage/EnemyKillAll, then those four TUs with the
same profile. The retained single-TU recipe is copied from the existing
`anm-create-vm-screen-variant0` family: `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc
/GL`, entry `?Initialize@AnmVmView@@QAEXXZ`, no support inputs. It is an existing
bounded compiler context, not evidence for original production flags.

The MIT reference supplied source-shape hypotheses for world0 and the start
helper. Its volatile world accesses, asm interfaces and unused leading
initializer argument are not adopted. The free-helper control used only three
real operands and preserved the complete maintained body; neither class
ownership nor parameter spelling is established by the experiment.

Do not repeat these store-order, bitfield, outlining, member/free-function or
entry controls without additional TH10 source/alias/lifetime/caller evidence.
The target schedule and the coupled Initialize/start-helper interfaces remain
open. Rotate to another complete authored owner instead of granting near-match
credit or introducing dummy operands, volatile accesses or byte padding.

## Reproduction and checkpoint

Use `ltcg_link.cold_link` with the recorded single-TU profile/entry and output
directory from `candidate-recipes.toml`. Then import
`scripts/compare-linked-function.py`, set its `UNITS_MANIFEST` to this candidate
manifest and call `compare_unit` for each listed unit. All five must currently
return `mismatch`, with complete field normalization. The manifest is deliberately
outside `config/match-units.toml` and `config/matches.csv`.

Target/toolchain/tracking checks, focused strict candidates/guards, generated
progress, public CI and whitespace checks pass. Superseded controls are removed;
one ignored single-TU image remains for the unresolved claim.
Native product, runtime, dual-Oracle semantic and port gates remain open.
