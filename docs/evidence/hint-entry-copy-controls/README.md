# Hint automatic-entry copy and initialization controls

2026-10-22, baseline `bdba3f7`. Natural source changes improve the selected
AddAutomaticEntry graph from210 to212 bytes against the220-byte target. It
remains nonexact and grants **zero new exact functions or bytes**. Existing
Caution104 remains exact. Authored source605/exact397/72387 bytes are unchanged.

## Target and source observations

Fresh original-target verification and local read-only/noanalysis Ghidra agree
on41A120..41A1FB/220. The complete412289-byte mapped text hash agrees before and
after the rolled-back review. Target position copying uses three integer
load/store pairs through an EAX destination, then calls strncpy with64 bytes.
The live Player position and new entry are distinct objects at the sole reviewed
caller; no arbitrary overlapping-copy contract is introduced.

Replacing the three scalar XYZ assignments with a12-byte memcpy reproduces
that instruction shape. The34-byte position/call fragment is equal after its
single independently resolved strncpy REL32 is replayed. Its target offset is
+8B and selected native offset+89. This is a fragment diagnostic, never a whole
function match or ledger credit. The target instruction sequence does not
identify the original vector type or source spelling.

The equivalent nonzero-first chapter conditional recovers the target branch
and counter-load order. Moving remaining initialization after time/scale
recovers their inlined store order while leaving the standalone51-byte
constructor raw-identical to baseline. That standalone constructor still has14
ordinary byte differences, chiefly self-link/store scheduling. Original source
order is not uniquely established by these compiler controls.

The candidate still merges the two target474C84 reads, schedules alignment
later and retains different allocation-pointer/self-link register scheduling.
Those observations do not prove that the shared global is volatile. No ABI,
flags, target bytes, layout, inert padding or fabricated side effects change.

## Independent compiler controls

Four separate full six-TU cold links retain the existing VC7.1 `/GL` recipe
and real PlayerUpdateCallback entry. Source is Player plus Hint, ItemManager,
AnmManager, AnmVmId and Gui. The controls are sequential and input-pinned:

| Source control | Entry /220 | Constructor /51 | Result |
| --- | ---: | ---: | --- |
| Position memcpy | 212 | 51 | Position/call fragment recovered |
| Add nonzero-first conditional | 212 | 51 | Branch/counter order recovered |
| Add remaining-last initialization | 212 | 51 | Selected; standalone constructor raw unchanged |
| Replace packed color with chained BGRA stores | 242 | 72 | Rejected; packed assignment restored |

Each graph passes the existing complete Caution104/all2 comparison. The enlarged
rejected graph moves linked addresses, changing some other raw bodies; no full
address-rebinding claim is made for its nonexact Player transition. The selected
graph keeps the transition782 and parent2808 raw-identical to baseline.

An additional independent canonical cold repeats all five selected native
bodies, including the212-byte allocator and51-byte constructor. Current recursive
inputs and both selected carriers are pinned in [native evidence](native-summary.json).
[Target facts](target-facts.json) keep raw target observations separate from
compiler inference. Existing lifecycle and Player state2 contracts pass,
including65 and7 strict functional controls respectively, with explicit host
seams and no TH10/native-product runtime claim. [Validation](validation-summary.json)
records the completed gates.

The prior [Player/caution checkpoint](../player-state2-hint/README.md) retains
its historical210-byte source context. Canonical ten-TU large-owner inputs are
unchanged; no broad old-unit replay is performed. The three large owners,
independent global boundary audit, native product closure and later phase gates
remain open.

## Final validation and cleanup

The frozen selected source passes all106 public CI checks. Only validation
documentation and ignored-output cleanup follow that run; final source/test and
native artifact hashes remain equal. Focused documentation, tracking, progress,
toolchain, build-graph and whitespace gates are repeated before checkpoint.
The [cleanup record](cleanup-summary.json) removes6260601 logical file bytes:
three superseded experiment carriers and four reproducible CI host-output trees.
Exact source overlays/receipts, the current two native carriers and the pinned
prior private baseline remain. Unclassified work and independent D3D files are
preserved. The overall reconstruction goal remains active.
