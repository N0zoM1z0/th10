# TH10 Exact-Reconstruction Handoff

## Checkpoint state

- Repository `th10`, branch `main`, target `target:th10-main`, analysis provider `th10-ghidra`.
- Current packet base: `92db5f4 gpt-5.6-sol: recover ANM script variables`, branch `main`.
- Completed session checkpoint: `bd9b2e3 gpt-5.6-sol: promote exact PbgFile accessors`.
- Completed session checkpoint: `9b5e7eb gpt-5.6-sol: add exact replay workflow`.
- Completed session checkpoint: `4ed34ee gpt-5.6-sol: promote exact PbgArchive lifecycles`.
- Completed session checkpoint: `621add9 gpt-5.6-sol: promote exact LZSS state reset`.
- Completed session checkpoint: `88800b6 gpt-5.6-sol: report exact reconstruction backlog`.
- Completed session checkpoint: `96f17c6 gpt-5.6-sol: harden COFF extent diagnostics`.
- Completed session checkpoint: `d59f975 gpt-5.6-sol: batch probe exact backlog`.
- Completed session checkpoint: `26af714 gpt-5.6-sol: add LTCG linked-image diagnostics`.
- Completed session checkpoint: `f5a0c4f gpt-5.6-sol: add canonical LTCG replay`.
- Completed session checkpoint: `40c96ec gpt-5.6-sol: promote exact PbgArchive LTCG helpers`.
- Completed session checkpoint: `e4c9077 gpt-5.6-sol: promote exact LZSS LTCG helpers`.
- Completed session checkpoint: `37ebb83 gpt-5.6-sol: promote remaining exact LTCG helpers`.
- Completed session checkpoint: `ead21f9 gpt-5.6-sol: rank exact reconstruction backlog`.
- Completed session checkpoint: `d4af1a5 gpt-5.6-sol: recover ECL core lifecycles`.
- Completed session checkpoint: `e2fecf3 gpt-5.6-sol: recover ECL host hierarchy`.
- Completed session checkpoint: `2dbce7d gpt-5.6-sol: recover ANM VM lifecycle`.
- Completed session checkpoint: `260ac75 gpt-5.6-sol: recover ANM manager core`.
- Completed session checkpoint: `3e6de60 gpt-5.6-sol: recover ASCII text pipeline`.
- Completed session checkpoint: `e239b4b gpt-5.6-sol: recover ANM render buffer core`.
- Completed session checkpoint: `591f59b gpt-5.6-sol: recover ANM draw core`.
- Completed session checkpoint: `3616aeb gpt-5.6-sol: recover ANM camera projection`.
- Completed session checkpoint: `1923623 gpt-5.6-sol: recover ANM rotated draw modes`.
- Completed session checkpoint: `56ac2e8 gpt-5.6-sol: recover ANM projected 3D quad`.
- Completed session checkpoint: `9ad868a gpt-5.6-sol: recover ANM mode dispatch`.
- Completed session checkpoint: `fc3dce0 gpt-5.6-sol: recover ANM projected photo blend`.
- Completed session checkpoint: `f6d88dd gpt-5.6-sol: recover ANM direct 3D`.
- Completed session checkpoint: `09e4da8 gpt-5.6-sol: recover ANM generated geometry`.
- Completed session checkpoint: `2d8c48b gpt-5.6-sol: recover ANM radial trail and RNG`.
- Completed session checkpoint: `92db5f4 gpt-5.6-sol: recover ANM script variables`.
- Planned current checkpoint subject: `gpt-5.6-sol: reconstruct ANM script executor`. Final commit hash is intentionally not self-recorded before the commit exists; recover it from live Git after checkpoint.
- Recovery continued from the clean ANM script-variable checkpoint. The ignored private target, existing `.analysis/`, toolchain, Wine prefix, Ghidra project, and build caches were preserved.
- Current campaign: `.analysis/gpt-5.6-sol/20260915-anm-executor/`. The earlier script-variable, radial-trail, generated-geometry, ANM direct-3D, mode-7, projected, draw, manager, VM, ECL host, ECL lifecycle, backlog-ranking, final-structural, Lzss, PbgArchive, canonical-replay, linked-diagnostic, and earlier session campaigns are checkpointed separately; earlier `gpt-web` campaigns remain ignored evidence and were not treated as current authority without replay.
- This session has not pushed. The exact-reconstruction campaign remains active/incomplete.

## Recovery and authority

The session inspected branch/HEAD/history, complete tracked/untracked state, and the prior handoff. Committed base `92db5f4...` was adopted as live authority.

All requested repository and Factory guidance was re-read from the live repository shell before tracked reconstruction work. No requested path was missing.

Fresh repository preflight passed at the start of the current ANM reconstruction session:

- `python3 scripts/verify-target.py`
- `python3 scripts/verify-toolchain.py --execute`
- `python3 scripts/validate-tracking.py --require-target`
- `python3 scripts/report-reconstruction-status.py`

The execute toolchain path passed pinned VC7.1 SP1 build6030 normal COFF, C++ `/GL`, resource and PE32-i386 link smoke through headless Wine. Native `th10-ghidra` `check {}` passed for `target:th10-main` with `provider_transport=factory-native-command`. This packet combines that provider's read-only decompilation, disassembly, calls, and xrefs with direct hash-attested target disassembly and compiler evidence.

Target remains the ignored operator file `resources/th10.exe`: size 487,936, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`, MD5 `7dc488d82c81dd4aee4ba098b8804d83`, PE32 i386 base `0x00400000`, entry `0x004537DC`, dominant Rich build6030. It was not modified, moved, staged or committed. `/mnt` was not searched and `TH10_TARGET_PATH` was not set.

## Current packet: ANM script executor and interpolation core

`AnmRenderManagerView::ExecuteScript @ 0x0043EE30-0x004413A2` is now a complete
maintained source body rather than a declaration. The target instruction header
is eight bytes (`opcode`, `size`, `time`, `variableMask`) followed by typed
arguments. The executor implements every target opcode from `-1` through `92`,
including interrupt save/restore, variable reads and writable operands,
arithmetic/comparison/jump behavior, RNG and trigonometry, sprite/transform and
render flags, child-VM creation, callback setup, and the end-of-frame position,
rotation, scale, UV, color, alpha and timer updates. The absolute opcode table
at `0x004413A4` remains separate from the 9,587-byte physical contribution.

The executor review replaces most of the VM's opaque middle with target-bound
fields. The 0x3AC-byte VM now carries seven typed interpolation slots from
`+0x070` through `+0x233`, three matrices at `+0x23C/+0x27C/+0x2BC`, two BGRA
colors, script locals and counters, three position vectors, generated-geometry
state, interrupt return state, resource/script/sprite pointers and update/draw
callbacks. `AnmLoadedView::SetSprite` establishes the loaded sprite stride,
texture dimensions, UV rectangle, scale and texture-matrix writes. Four child
creation variants and VM-id lookup remain declared seams because their bodies
have not yet been reconstructed.

Fifteen connected helper bodies are also source-present: the four interpolation
evaluators, scalar interpolation curve, integer-triplet scaling, five setup
functions, intrusive child-node insertion, sprite binding, angular wrapping and
timer addition. All interpolation modes `0..17` are represented, including the
state-mutating add/accelerate modes and component-truncating Hermite color path.
The primary-alpha setup remains non-exact because the current partial executor
context registerizes its mode in EDX while the target keeps that argument on the
stack; the mismatch is retained rather than hidden with an artificial shim.

Supplying `RandomMath.cpp` beside this real executor entry also closes almost
all of the radial-trail initializer's former context gap. Its linked PDB
contribution is now exactly 803 bytes against the 803-byte target and differs
in only eight ordinary bytes: one independent-load scheduling choice in the
inlined RNG expression and the operand order of one commutative Y sum. Scoping
the 12-byte direction temporary inside the loop and writing vertex Z before UV
recover the target's reused stack slot and remove 18 earlier differences. This
is a strong near match, but it has no exactness credit.

The real executor `/GL` context produces eight new canonical exact units:
child-node insertion (23 bytes), scale setup (136), sprite binding (356),
secondary-alpha setup (116), both color setups (208 each), signed-pi angular
addition (94), and timer addition (88). Two independent cold artifact builds
replay all **8 functions / 1,229 bytes** exactly, including every declared
constant, runtime-helper and global-speed field. In particular, writing
`duration` before `mode` recovers the secondary-alpha private ABI, endpoint
value construction recovers the two color helpers, and the target-observed
callee-cleans `RET 8` establishes `AddNormalizeAngle` as `__stdcall`.

Current tracking contains **1,298** candidates, **210** authored functions,
**182** source mappings, and **90 canonical exact functions / 9,557 bytes**.
The authored source backlog is **88**. The `src/AnmManager.cpp` canonical set is
**40 functions / 7,566 bytes** and passes a complete cold replay across eight
independent artifact contexts. The full executor itself remains non-exact: the
current `/GL` PDB contribution is 8,548 bytes versus the 9,587-byte target, so
source presence, semantic completeness and exact code generation remain
separate claims.

The next ANM frontier is to reconstruct the four child-VM creators and VM-id
lookup used by opcodes 69-72, then re-probe the executor, `GetFloatVar`, and the
eight-byte radial-trail near match in that richer caller graph.

## Completed packet: ANM radial trail and RNG

The pulsing radial-trail family at `0x004452F0-0x00445898` is now
source-present. Its 0x4B0-byte VM-owned payload contains 33 textured vertices,
33 radii, 33 radial velocities, a two-float UV velocity and a trailing dword.
The initializer frees/replaces VM `generatedVertices`, installs callbacks at
VM `+0x398/+0x39C`, seeds a center plus 31 radial vertices, and uses RNG owner
`0x004918B0`. The updater scrolls both U and V with the X velocity, applies the
target's 33-vertex wrap loops, advances radii, rebuilds the ring with the
angle/magnitude helper, clears ring alpha and copies vertex 1 into closing
vertex 32. The draw callback submits the 33-vertex textured fan.

`UpdatePulsingRadialTrail` has the exact 598-byte target extent and matches
524/526 comparable bytes under normal and linked compiler probes; only two
commutative X/Y position-sum displacement bytes differ. In the single-source
LTCG diagnostic the initializer was only 572 bytes because unresolved RNG
methods could not be inlined. New multi-source `/GL` support lets the probe add
`src/RandomMath.cpp`; the initializer then becomes 804 bytes versus target 803,
but remains non-exact because its real caller is the source-absent 9,587-byte
ANM script executor at `0x0043EE30`, which supplies the target's private EDI VM
receiver.

Two trail units are canonical exact in that multi-source context:
`DrawPulsingRadialTrail` (25 bytes and two linked fields) and
`AnmFloat3View::FromAngleMagnitude` (30 relocation-free bytes). The recovered
RNG recurrence independently produces four normal-COFF exact units:
`GetRandomU16` (39), `GetRandomU32` (78), `GetRandomF32` (109), and
`GetRandomF32Signed` (115), totaling 341 bytes and five constant relocations.
Boundary review also added Ghidra-missed, unreferenced bodies at `0x004452E0`,
`0x004458A0`, `0x0044B9B0`, and `0x0044BA30`; only `0x0044B9B0` has sufficient
source/compiler evidence for authored exact promotion.

Current tracking contains **1,298** candidates, **190** authored functions,
**162** source mappings, and **79 canonical exact functions / 7,896 bytes**.
The authored source backlog is **79**. The full `src/AnmManager.cpp` canonical
set is **29 functions / 5,905 bytes** across six artifact contexts; its cold
replay passes with the trail artifact compiling both ANM and RNG inputs.

The next high-value ANM frontier is the script executor at `0x0043EE30`, which
owns the private trail-initializer call context, or the following generated
effect families beginning at `0x00445900`.

## Completed packet: ANM generated geometry

The full `0x00444B10-0x004451B2` generated-geometry corridor is now
source-present as eight distinct functions. Horizontal and vertical strip
initializers fill alternating UV rows or columns with the VM primary color and
unit RHW; the adjacent color helper updates a caller-supplied 0x1C-stride
range. Dispatcher mode 9 reaches the visible/alpha-gated textured strip
submitter with `generatedVertices` and twice `generatedVertexCount`.

The corridor also contains a batched supplied-quad path, two 0x14-stride
untextured diffuse submitters for triangle strips and fans, and a textured fan
submitter used by the 33-vertex callback at `0x00445880`. The untextured paths
select diffuse-only texture operations, disable Z writes for submission, then
invalidate the renderer's shader/color/blend/Z caches and restore normal
modulate/texture arguments. D3D9 vtable slots and all state/FVF/primitive
values are represented explicitly in the target-bound device view.

Ghidra originally omitted the independently CC-delimited bodies at
`0x00444CB0`, `0x00444DC0`, `0x00444E60`, and `0x00444FA0`; all four are now
included in the function denominator. Ghidra and raw scans find target callers
only for the dispatcher strip path and the 33-vertex textured fan. Names for
the retained no-xref bodies are descriptive or adjacent-supported rather than
original-symbol claims.

Three units are canonical exact after two cold linked-image replays:
`InitializeHorizontalTextureStrip` (201 bytes),
`InitializeVerticalTextureStrip` (201), and `SetGeneratedVertexColor` (34).
They add 436 exact bytes and have no linked fields. `DrawGeneratedVertices`
and `DrawTexturedTriangleFan` reproduce their complete 224/211-byte extents
apart from the same 13-byte texture-bind scheduling block seen in Draw3D.
`QueueSpriteQuad` and both 313-byte untextured submitters reproduce every
instruction and linked field, while the candidate PDB contributions own two
extra trailing `CC` bytes; strict extent policy keeps these five non-exact.

At checkpoint `09e4da8`, tracking contained 1,293 candidates, 154 source
mappings and 73 canonical exact functions / 7,500 bytes. The next frontier at
that checkpoint was the generated radial-trail family now covered above.

## Completed packet: ANM direct 3D

`SetRenderStateForVm3D @ 0x004423E0-0x00442597` is reconstructed from
TH10-local target evidence. It is the direct-3D sibling of the batched 2D
state helper: blend changes select inverse-source-alpha for mode zero and one
for modes one and two; the selected primary/secondary VM color is optionally
multiplied by the renderer color and published as D3D9 texture factor; and the
same point/linear sampler cache is maintained. Every incompatible state change
flushes pending 2D sprites, and the helper increments the per-frame state count.

`Draw3D @ 0x00444760-0x00444B00` rejects hidden, disabled and zero-alpha VMs,
flushes the 2D batch, updates scale and X/Y/Z rotations, and applies anchor
translation from `spriteOffset + preservedPosition + position`. It installs
the world transform, binds the sprite texture, optionally refreshes the
texture transform from UV scroll, and submits the renderer's four 0x14-byte
vertices as a two-primitive triangle strip. TH10 binds the texture before
refreshing the sprite/UV cache, repeats the `uvScrollX != 0` test in the
texture-transform condition, and writes world Z first without then with
`spriteOffset.z`; source retains those target-observed details rather than
normalizing them from adjacent code.

Pinned VC7.1 `/GL` reproduces both complete target contribution extents: 440
bytes for the state helper and 929 for the draw owner. The state helper matches
396/408 comparable non-linkage bytes and Draw3D matches 816/829. Each remaining
gap is one equal-length instruction-scheduling region around a D3D9 COM call:
12 bytes for texture-factor state and 13 bytes for texture binding. Natural
assignment-expression, explicit-local and inline member-wrapper probes do not
change those regions. They remain source-present and non-exact; no artificial
dependency or assembly constraint was added.

Current tracking contains **1,289** candidates, **174** authored functions,
**146** source mappings, and **70 canonical exact functions / 7,064 bytes**.
The authored source backlog is **72**. The full `src/AnmManager.cpp` canonical
set remains **24 functions / 5,414 bytes** across five artifact contexts.

The next ANM frontier is the generated-vertex initialization/submission
corridor at `0x00444B10`, `0x00444BE0`, `0x00444CE0`, and `0x004450E0`.

## Completed packet: ANM projected photo blend

`DrawMode7 @ 0x004445C0-0x00444751` is reconstructed from TH10-local target
evidence. After `Project3DQuad`, it transforms the four renderer-local
0x14-byte source vertices beginning at renderer `+0x3ADA78` through cached
world matrix `+0x3AD0F0`. Each resulting XYZ is measured independently from
camera global `0x00491D7C`.

Within the near distance at `0x00491E78`, a vertex keeps the VM's selected
primary or secondary color. Across the near/far interval it interpolates B/G/R
toward float channels at `0x00491E80/84/88` and preserves VM alpha. At or
beyond the far endpoint, it takes dword color `0x00491E90` and then restores VM
alpha. Unlike mode 6, this path neither applies renderer mix color nor rejects
a fully blended quad. It submits through `DrawInner(vm, 2)` and restores all
four shared RHW fields to one.

The dispatcher pushes VM then renderer for mode 7; the callee owns both stack
arguments and returns with `RET 8`. Its source view now names the four source
vertices and extends the photo-blend state through the far color, with compile-
time offset and size checks. TH095 supplied a source-family hypothesis only;
all accepted offsets, arithmetic and ABI facts are TH10-local.

Pinned VC7.1 `/GL` in the real two-VM caller context produces a 404-byte PDB
contribution versus the 402-byte target. Refining the initial 479-byte adjacent-
shaped candidate to TH10 float-channel arithmetic, pointer induction and a
12-byte live color/range/conversion aggregate closes the stack frame exactly at
`0x4C` and aligns the full transform/distance/control-flow core. The remaining
two-byte extent difference comes from the candidate retaining the shared-color
base with positive channel offsets and an adjusted loop comparison, while the
target retains an alpha pointer and offsets `-3..0`. It remains source-present
and non-exact without artificial codegen constraints.

Current tracking contains **1,289** candidates, **172** authored functions,
**144** source mappings, and **70 canonical exact functions / 7,064 bytes**.
The authored source backlog is **70**. The full `src/AnmManager.cpp` canonical
set remains **24 functions / 5,414 bytes** across five artifact contexts.

The next ANM frontier is `Draw3D @ 0x00444760`, followed by the generated-
vertex initialization/submission corridor at `0x00444B10-0x004450E0`.

## Completed packet: ANM mode dispatch

`DrawMode6 @ 0x00443FB0-0x00444232` is now reconstructed from TH10-local
evidence. It first uses the camera-facing projector, computes the distance from
`position + preservedPosition + spriteOffset` to background-camera global
`0x00491D7C`, and optionally applies the renderer color multiplier. Beyond the
near distance at `0x00491E78`, it interpolates color toward float B/G/R globals
at `0x00491E80/84/88` and alpha toward zero across the configured near/far
interval. A completed fade returns `-1`; otherwise all four shared vertices
receive the computed color and `DrawInner(vm, 2)` submits the quad.

The total `Draw @ 0x004451C0` owner is also source-present. Direct target and
PDB review correct its physical extent from Ghidra's 173-byte code-only body to
216 bytes through `0x00445297`: code ends at `0x0044526C`, a three-byte
alignment instruction follows, and `0x00445270-0x00445297` is the owned
ten-entry absolute jump table. It checks visible, draw-enabled and primary-alpha
state separately, extracts render mode from VM flag bits 22-25, and dispatches
modes 0-9 across the recovered draw family. Mode 9 exposes VM `+0x30C` as a
generated-vertex count and passes twice that count with the VM vertex pointer.

A real target caller at `0x00409230` draws two adjacent 0x3AC-byte VMs through
the global renderer when owner flag bit zero is set. Using this callback as the
pinned VC7.1 `/GL` entry reproduces both its complete 50-byte contribution and
the private 60-byte `DrawProjected3DQuad` contribution exactly, including all
ten declared linked fields. `DrawMode6` remains a close natural-source
candidate at 645 bytes versus target 643: its EDI/ESI private ABI is correct,
but the compiler candidate uses a `0x34` stack frame where the target uses
`0x20`. `Draw` remains non-exact at 208 versus the 216-byte physical owner
until its unrecovered mode-7, direct-3D, and generated-vertex callees are
restored in the same LTCG context.

This packet also fixes the linked-image Oracle for code contributions with
inline absolute tables. PE base relocations now override only overlapping
Capstone-derived false relative fields, while every other overlap still fails
closed. This lets switch-heavy ANM and ECL contributions be probed without
misclassifying jump-table data as code.

Current tracking contains **1,289** candidates, **171** authored functions,
**143** source mappings, and **70 canonical exact functions / 7,064 bytes**.
The authored source backlog is **69**. The full current `src/AnmManager.cpp`
exact set is **24 functions / 5,414 bytes** across five artifact contexts.

The next ANM frontier is mode 7 at `0x004445C0`, followed by the direct-3D and
generated-vertex owners reached by dispatcher modes 8 and 9.

## Completed packet: ANM rotated draw modes

Direct target boundary review splits the former `0x004436C0-0x00443B5E`
candidate at the `CC` byte at `0x0044390F`. The two resulting 591-byte bodies
are `0x004436C0-0x0044390E` and `0x00443910-0x00443B5E`. Ghidra xrefs and raw
control flow prove that `Draw @ 0x004451C0` render-mode cases 1 and 3 tail-jump
to the two entries at `0x00445203` and `0x00445263`. The dispatcher extracts
the four-bit mode from VM flags 22-25 after checking visible, draw-enabled and
primary-alpha state.

Both bodies are instruction-identical apart from their two address-dependent
REL32 displacements. Each handles zero Z rotation through
`DrawNoRotationNoRound`, uses one x87 `FSINCOS`, accumulates all three VM
position vectors, scales sprite width/height, applies the anchor modes in flag
bits 18-21, writes four rotated XY pairs plus common accumulated Z, and enters
`DrawInner(vm, 0)`. Maintained source therefore supplies `Draw2D` for mode 1
and a distinct `Draw2DRotatedOrAxisAligned` method for mode 3. The latter name
is corroborated by TH08's same mode table and remains a descriptive hypothesis;
all behavior, boundaries, ABIs and duplicate-body facts come from TH10.

Pinned VC7.1 SP1 build6030 `/GL` produces complete 591-byte PDB contributions
for both methods. Each probe matches 497/507 comparable bytes; the only ten
non-linkage differences are one equal-length scheduling choice at offsets
`+0x54..+0x5E`, where the target completes the horizontal anchor mask before
loading the first Y-offset operand. No exact promotion is made. Natural
calculation-order and aggregate-layout experiments either preserved those ten
differences or moved farther from the target; no inert dependency or padding
was introduced.

Current tracking contains **1,289** candidates, **166** authored functions,
**140** source mappings, and **68 canonical exact functions / 6,954 bytes**.
The authored source backlog is **68**. The full current `src/AnmManager.cpp`
exact set is **22 functions / 5,304 bytes** across four artifact contexts.

The photo-color owner and real dispatcher context identified here are completed
by the current packet above. Mode 7 and the direct-3D/generated-vertex paths
remain the open continuation.

## Completed packet: ANM projected 3D quad

`Project3DQuad @ 0x00444240-0x00444571` is recovered from TH10-local target
evidence. It establishes VM matrices at `+0x23C/+0x27C`, flag bit 14 as the
matrix-recalculation suppressor, and the renderer's cached world matrix at
`+0x3AD0F0`. Dirty transforms copy the base matrix, apply scale to `_11/_22`,
then compose nonzero X/Y/Z rotations through D3DX. Anchor bits 18-21 select
the local `-256/-128/0/128/256` corner coordinates before four projections
through the active viewport owner.

TH10 adds all three VM position vectors to existing world X/Y translation but
replaces world Z with their sum. This differs from the first adjacent-game
source-shape hypothesis and accounts for the only four bytes in the initial
822-byte candidate beyond the 818-byte target. With the corrected assignment,
the canonical `/GL` unit reproduces the complete PDB contribution and all 21
link fields exactly.

`DrawProjected3DQuad @ 0x00444580-0x004445BB` is source-present. It projects,
calls `DrawInner(vm, 0)`, restores the four shared RHW values to one, and
returns the draw result. Its standalone compiler entry emitted a conventional
72-byte wrapper. The subsequently recovered real two-VM draw callback supplies
the private ESI/EDI context and reproduces the complete 60-byte target form;
the current packet promotes it to canonical exact.

## Completed packet: ANM camera-facing projection

The camera-facing placement owner at `0x00443B60-0x00443F76` is now recovered
from TH10-local target evidence. It reads VM Z rotation at `+0x2C`, accumulates
`position + preservedPosition + spriteOffset`, builds a translated identity
world matrix, and calls the imported `D3DXVec3Project` thunk at `0x0045218E`
twice through the active viewport owner at `0x00491FAC`. The owner layout is
camera-right `+0x30`, view matrix `+0x4C`, projection matrix `+0x8C`, and D3D9
viewport `+0xCC`; all four offsets have compile-time checks.

The first projection supplies a zero vector and rejects projected Z outside
`[0,1]`. The second projects camera-right, and the screen-space distance between
the results supplies half-scale for sprite width/height. The same VM flag bits
18-21 select horizontal and vertical anchor modes before the two-dimensional
rotation is applied. TH10 writes `projectedPosition.z` to all four shared
vertices; this target-local behavior differs from the adjacent TH095 source
shape and was retained from TH10 evidence.

Pinned VC7.1 SP1 build6030 `/GL` in the regular-text entry context reproduces
the complete 1,047-byte PDB contribution and all 25 viewport, import, constant,
and shared-quad fields exactly. The 33-byte `DrawCameraFacingQuad` wrapper is
also exact with both REL32 calls replayed to `ProjectCameraFacingQuad` and
`DrawInner`. The compiler's local symbol ordering affects LTCG stack coloring:
renaming the two 12-byte projection temporaries alone produced eight stack-
displacement differences, while the retained source form reproduces all 1,047
bytes. This is compiler evidence, not an inferred target layout.

Direct target review also recovered a Ghidra-missed 50-byte
`TranslateRotation` body at `0x00443680-0x004436B1`. Its 50 instruction bytes
are raw-equal to current source, but the candidate PDB contribution includes
two trailing `CC` bytes and is therefore conservatively source-present/non-
exact under the current complete-contribution Oracle. The following rotated
draw range was subsequently split and reconstructed in the current packet.

## Completed packet: ANM shared draw core

The common renderer path at `0x00442670` is now reconstructed together with
its 25-byte color mixer at `0x004423C0`, 201-byte render-state owner at
`0x004425A0`, and axis-aligned callers at `0x00443080/0x00443290`. Direct TH10
evidence establishes screen shake at renderer `+0x5C/+0x60`, the texture/blend/
shader/filter cache at `+0x3ADA64..+0x3ADA6E`, mix color at `+0x732458`, VM UV
scroll at `+0x54/+0x58`, primary/secondary colors at `+0x2FC/+0x300`, and
sprite texture/UV/dimensions at `+0x04/+0x20..+0x34`. Every named layout field
has a compile-time offset check or belongs to an already size-checked view.

`DrawInner` adds screen shake to four shared vertices, optionally performs the
D3D9 half-pixel `FRNDINT - 0.5f` sequence, assigns sprite UVs, computes four
bounds, culls against the unsigned active viewport at global `0x00491FAC`,
flushes on texture/shader changes, applies primary or secondary VM color and
optional per-channel renderer mixing, updates render state, and expands the
quad into the packed buffer. `SetRenderStateForVm` handles destination blend
and point/linear min/mag filtering through D3D9 vtable slots `+0xE4/+0x114`;
the texture change uses `SetTexture` at `+0x104`.

Both axis-aligned draw owners implement the three horizontal and vertical
anchor modes from VM flag bits 18-21. The rounded path floors centered axes and
enters `DrawInner(vm, 1)`; the no-round path preserves sub-pixel coordinates
and enters with zero. TH10 has one target-local quirk: no-round writes vertex Z
from `spriteOffset.y (+0x350) + preservedPosition.z + position.z`, while the
rounded sibling reads `spriteOffset.z (+0x354)`. Maintained source preserves
the observed difference rather than normalizing it from adjacent games.

Pinned VC7.1 `/GL` in the real regular-text draw entry context reproduces all
five complete PDB contributions exactly: color mix 25 bytes/no fields, state
owner 201 bytes/six fields, `DrawInner` 1,111 bytes/91 fields, rounded axis draw
515 bytes/29 fields, and no-round axis draw 486 bytes/29 fields. Two cold
canonical passes replay all **2,338 bytes and 155 linked fields** with zero
differences. The full current `src/AnmManager.cpp` exact set is **19 functions /
3,406 bytes** across four artifact contexts.

Current tracking contains **1,286** candidates, **161** authored functions,
**133** source mappings, and **65 canonical exact functions / 5,056 bytes**.
The authored source backlog is **64**. This packet adds five source mappings
and five exact functions without adding speculative denominator entries.

The next ANM frontier is the neighboring transformed-quad corridor beginning at
`0x00443B60`; keep its rotation/projection semantics provisional until its own
TH10 matrix, caller, and compiler evidence is complete.

## Completed packet: ANM render buffer core

The actual ANM renderer reached through pointer `0x00491C10` owns a shared
`0x20000`-entry vertex buffer. TH10 directly establishes its `0x1C`-byte packed
vertex size, sprite count `+0x3ADAC8`, buffer base `+0x3ADACC`, end/start cursors
`+0x72DACC/+0x72DAD0`, and flush count `+0x58`. The complete buffer span is
therefore the observed `0x380000` bytes rather than an adjacent-game layout.

Maintained source covers the 29-byte clear owner at `0x00442F30`, 133-byte
flush at `0x00442F50`, and 157-byte quad-to-triangle expansion at `0x00442FE0`.
The clear resets count and cursors; the add path copies corners `0/1/2/1/2/3`,
advances by six vertices, and increments the count; flush configures D3D9
diffuse arguments and FVF, submits two triangle-list primitives per sprite,
advances the submitted range, clears the count, and increments the frame tally.

All three are canonical exact linked-PE units in the real regular-text draw
context: clear and add are complete raw-equal PDB contributions with no linked
fields; flush contains exactly four DIR32 fields, all replayed to D3D9 device
global `0x00491C30`. Two independent cold builds reproduce all **319 bytes**.
The pinned VC7.1 SDK lacks D3D9 headers, so `src/D3d9View.hpp` supplies the
minimal target-used COM prefix with compile-time checks for vtable slots
`+0x10C`, `+0x14C`, and `+0x164`.

Current tracking contains **1,286** candidates, **156** authored functions,
**128** source mappings, and **60 canonical exact functions / 2,718 bytes**.
The authored source backlog remains **64**. The current `AnmManager.cpp` source
replays fourteen exact units across **1,068 bytes** in four artifact contexts.

The next draw-core seam is the paired axis-aligned placement owners at
`0x00443080` and `0x00443290`. Their existing TH10 evidence already establishes
VM position/offset/scale fields and the horizontal/vertical anchor bits; their
final call reaches the common render-state/draw owner at `0x00442670`.

## Completed packet: ASCII text pipeline and ANM-owner correction

TH10-local queue append, formatting, glyph lookup, viewport, callback, and draw
evidence corrects the preceding checkpoint's owner hypothesis. The `0x89AC`-
byte polymorphic object at global `0x004776E0` is the ASCII text manager. It
contains 256 regular and 64 GUI records of `0x68` bytes, queue counts at
`+0x896C/+0x8970`, text color/scale/viewport/shadow/spacing state, an ASCII ANM
resource at `+0x8994`, and two embedded `0x3AC` ANM VMs. The true ANM renderer is
a distinct object published through the pointer at `0x00491C10`.

Maintained source now expresses the complete regular and GUI record layouts,
queue reset, both append paths, three variadic formatters, and the two large text
renderers at `0x00401760` and `0x00401A50`. The renderers cover both `0x62`-glyph
banks, `0x44`-byte sprite records, line and space advance, regular-text shadow,
viewport transitions, vertex-buffer flushes, and rounded versus non-rounded ANM
draw calls. Each named field and helper target is directly visible in TH10. The
TH095 ASCII implementation supplied only a source-shape hypothesis.

The retained reset at `0x004014D0`, regular append at `0x00401530`, and GUI
append at `0x004015C0` are canonical exact linked-PE units across **267 bytes**.
They are complete, raw-equal PDB-owned contributions with no linked fields in
the real `AsciiManagerCreate` LTCG context. Together with the four renamed exact
units from `260ac75`, the current source file replays eleven exact units across
**749 bytes**. The three formatters and two renderers remain source-present
non-exact work; no semantic recovery claim has been promoted to exactness.

Current tracking contains **1,285** candidates, **153** authored functions,
**125** source mappings, and **57 canonical exact functions / 2,399 bytes**.
The authored source backlog is **64**. Relative to `260ac75`, this packet adds
eight authored functions, eight source mappings, three denominator candidates,
and three exact functions / 267 bytes while correcting the manager owner across
source, ledgers, exact manifests, and documentation.

The following renderer-buffer packet takes up the real owner at `0x00491C10`
and closes its clear, flush, and quad-expansion seam before the neighboring draw
helpers at `0x00443080` and `0x00443290`.

## Completed packet: ANM VM lifecycle

Direct TH10 constructor, destructor and reset evidence establishes a `0x3AC`-
byte ANM VM. Its constructor runs nine `0x14` timer-like member constructors,
clears exactly `0xEB` dwords, and marks the active sprite index at `+0x384` as
invalid. Its destructor owns and clears the optional generated-vertex pointer at
`+0x358`. The reusable reset at `0x00401DE0` preserves `+0x20` and
`+0x340..+0x348`, clears the VM, restores list sentinels, scale, matrix, timer,
flags and seven observed state fields, and writes `-1` at `+0x2FC`.

Maintained `AnmVmView`, `AnmVmTimerView`, and `AnmMatrixView` source expresses
these facts with compile-time size and offset checks. Denominator review also
adds two CC-delimited helpers missed by Ghidra: the 55-byte matrix identity body
at `0x00401F00` and the 15-byte timer initializer at `0x00401F40`. Their logic
is independently visible inline in the reusable reset.

Two cold canonical passes reproduce four complete functions across **192
bytes**: the 34-byte destructor and 88-byte constructor as normal COFF, and the
55-byte matrix and 15-byte timer helpers as raw-equal linked-PE PDB
contributions with no linked fields. The 273-byte reset compiles to the same
extent under normal COFF but remains non-exact because the bounded harnesses do
not recover the target's private EDX receiver and register/save ordering. In the
new explicit manager-factory link context it is 271 bytes.
The emitted seven-byte timer constructor shape also occurs at two target
locations already associated with other or unresolved owners; it receives no
ANM mapping or exactness credit.

Current tracking contains **1,279** candidates, **137** authored functions,
**109** source mappings, and **50 canonical exact functions / 1,842 bytes**.
The authored source backlog is **55**. Relative to the preceding checkpoint,
this packet adds five authored functions / 465 bytes, five source mappings, two
denominator candidates, and four exact functions / 192 bytes.

## Completed packet: ECL host hierarchy

Direct TH10 evidence closes the previously unresolved class boundary. The
six-slot vtable at `0x0046D0D8` contains neutral dispatch/int/int-pointer/float/
float-pointer methods followed by scalar deleting destructor `0x0040C7B0`.
The derived table at `0x0046D0C0` replaces those five methods and ends in the
Enemy deleting destructor at `0x0040CC50`. Construction at `0x0040C710` and
`0x0040D830`, destruction at `0x0040DAE0`, and the inline base tail through
`0x0040DC5F` independently show a `0x103C` polymorphic ECL host followed by the
Enemy runtime tail. The manager's indirect call through slot `+0x14` with flag
one is virtual deletion.

Maintained source now expresses that evidence as an actual
`EnemyEclHostBaseView` base and `EnemyFullObjectView` derived class, including
real constructors/destructors, `new`, virtual `delete`, and a base-typed operand
resolver in the embedded context. Compile-time layout assertions preserve base
size `0x103C`, full size `0x2518`, active context `+0x004`, embedded context
`+0x008`, allocation list `+0x1034`, and runtime tail `+0x103C`.

Five neutral virtuals are canonical exact normal-COFF units across 27 bytes.
The 21-byte base constructor and 47-byte base destructor are canonical exact
linked-PE units; their complete PDB-owned extents declare the base-vtable field,
the constructor's one DIR32 field, and the destructor's two `free` REL32 fields.
Two independent cold builds replay all seven new units across **95 bytes**.
All 20 Enemy exact units replay together across **511 bytes** after refreshing
only VC7 compiler-local resolver label names. Base reset and the large derived
constructor/destructor remain non-exact.

Repository exact coverage is now **46 functions / 1,650 bytes**. This bounded
evidence establishes source-level class shape and the declared function bodies;
it does not establish production TU partition or whole-product link closure.

## Completed packet: ECL core lifecycle and resource base

Direct TH10 review establishes a reusable ECL host around the full object's
embedded `0x1018` context. The two callback-subroutine transition paths call a
45-byte allocation-list release at `0x0040C6E0` and then the 71-byte execution
reset at `0x0040C730`. Maintained `EnemyRuntimeUpdate` now expresses both calls.
The release helper is canonical exact; reset is source-present and remains
non-exact at 69 linked bytes versus 71 target bytes.

The neighboring ECL resource base is now source-complete for its reviewed
lifecycle seam. Base vtable slot one is a five-byte default `LoadPackage`
returning zero; the five-byte indexed getter returns one of 32 script pointers
from `+0x0C`; the 34-byte constructor clears the complete `0x1098` object and
installs vtable `0x0046D0F0`; and the 36-byte destructor restores that vtable,
frees lookup owner `+0x8C`, and clears it. The embedded-context constructor
independently clears the two storage cursors at context `+0x1008/+0x100C`.

Two cold linked builds replay all six newly exact functions across **140
bytes**. The three nonzero-field units declare both release calls, the base
vtable fields, and the direct free call exhaustively; omitted-field negative
manifests are rejected. Repository exact coverage is now **39 functions / 1,555
bytes**. Production TU partition and linked-image ownership remain unknown.

## Completed packet: dual-codegen exact-backlog ranking

`scripts/rank-exact-backlog.py` now turns the two existing batch probes into one
fresh selection tool. It obtains the current authored backlog once, runs both
normal COFF and LTCG diagnostics, requires both reports to match every current
address/name/source/size key exactly, and rejects a run if the target, ledgers,
tools, or maintained source tree changes between the two probes. Its explicit
distance combines relative complete-extent size delta with the mismatch ratio
over non-link-field bytes. Profile recommendation and `ltcg_gain` remain triage
signals with `acceptance_authority=none`.

A full target-bound run covers all **53** current authored backlog functions.
Both lanes report **53 mismatches**, zero structural-exact candidates, and zero
unresolved keys. The leading bounded experiments are:

1. `Lzss::AddString`, LTCG: candidate/target **512/516 bytes**, **51/408**
   comparable mismatches, 27 linked fields, distance **0.133**;
2. `CPbgFile::Write`, normal COFF: **67/63**, **10/59**, one relocation,
   distance **0.233**;
3. `EnemyManagerCreate`, LTCG: **94/95**, **22/75**, five linked fields,
   distance **0.304**.

The ranking does not make any function exact or determine its production
profile/physical owner. It gives later sessions a reproducible reason to choose
a source/codegen experiment and preserves both lanes' raw metrics in JSON.

## Completed packet: final structural-exact LTCG promotions

The final seven unreviewed fields from the original linked diagnostic are now
bound to TH10-local targets. The Enemy link uses `g_EnemyManager` at
`0x00477704`, derived/base ECL-resource vtables at `0x0046D0B4` and
`0x0046D0F0`, and the already reviewed `free` entry at `0x00452422`. The
PbgFile link uses the reviewed `strchr` entry at `0x00452730`, complete
45-byte `strrchr` entry at `0x00452930`, and the `GetModuleFileNameA` IAT slot
at `0x004660DC`.

Two independent cold linked builds reproduce:

- `EnemyManagerView::EnemyManagerView`: **29/29 bytes**, one DIR32 field;
- `EnemyEclResourceView::EnemyEclResourceView`: **34/34 bytes**, one DIR32;
- `EnemyEclResourceView::~EnemyEclResourceView`: **36/36 bytes**, one DIR32
  and one REL32;
- `CPbgFile::GetFullFilePath`: **121/121 bytes**, one DIR32 and two REL32.

Each complete `source.ltcg.obj` PDB contribution decodes without gaps and has
exactly the declared fields. Replaying all seven fields leaves zero differences
across all **220 bytes**. Negative manifests omitting the Enemy global DIR32 or
the PbgFile `strrchr` REL32 are rejected. The `strrchr` boundary is independently
reviewed and classified library/CRT; `strchr` keeps origin/boundary state open
because its optimized body shares a backward return tail outside the current
Ghidra candidate. A fresh Enemy/PbgFile backlog probe reports 18 mismatches and
zero structural-exact candidates. All 13 structural candidates / 715 bytes from
the original full LTCG diagnostic have now been promoted through canonical
replay; none remains a diagnostic-only exact candidate.

## Completed packet: canonical Lzss linked fields

All 24 linked fields across the four structural-exact Lzss candidates resolve
to the already target-proven `g_LzssTree` owner at `0x00477858`. Their only
addends are `+0` (`parent`), `+4` (`left`), `+8` (`right`), and `+0x18008`
(`tree[0x2000].right`). The last address is four bytes inside the final
0x0C-byte node of the independently bounded `0x2001`-node / `0x1800C`-byte
array; it is not promoted as a separate global.

Two independent cold linked builds reproduce:

- `Lzss::InitTree`: **36/36 bytes**, four DIR32 fields;
- `Lzss::ContractNode`: **79/79 bytes**, eight DIR32 fields;
- `Lzss::ReplaceNode`: **117/117 bytes**, nine DIR32 fields;
- `Lzss::FindNextNode`: **49/49 bytes**, three DIR32 fields.

Each complete `source.ltcg.obj` PDB contribution decodes without gaps and has
exactly the declared field offsets, types and widths. Replaying the reviewed
tree base/addends leaves zero differences across all **281 bytes**. Together
with the normal-COFF exact `InitEncoderState`, this seam has five exact helpers
/ 323 bytes in two explicitly separate artifact contexts. The result does not
establish a production source/data TU, physical compiler owner, or product
closure. A fresh Lzss backlog probe after the promotions reports only three
mismatches and no remaining structural-exact candidate for this source.

## Completed packet: canonical PbgArchive linked fields

The remaining three structural-exact PbgArchive helpers each have one external
REL32 field. Direct hash-attested target review closes `0x00452706-0x00452717`
as an 18-byte CRT `malloc` wrapper with 94 direct call sites, and
`0x0046054E-0x004605B6` as a 105-byte CRT `_stricmp` wrapper with 11 direct
call sites. Both boundaries end at RET and meet either CC padding or the next
independent CRT function. Their behavior and surrounding CRT owners justify
library/CRT exclusion; the exact original CRT library members remain unknown.

Two independent cold canonical linked builds reproduce all three functions:

- `PbgArchive::GetEntryDecompressedSize`: **51/51 bytes**, one `__stricmp`
  REL32 at `+0x15` replayed to `0x0046054E`;
- `PbgArchive::FindEntry`: **53/53 bytes**, one `__stricmp` REL32 at `+0x18`
  replayed to `0x0046054E`;
- `PbgArchive::CopyFileName`: **47/47 bytes**, one `_malloc` REL32 at `+0x11`
  replayed to `0x00452706`.

All three complete `source.ltcg.obj` PDB contributions decode without gaps and
contain exactly their declared fields. The 151 replayed bytes have zero
differences. Together with the two zero-field pointer helpers, this shared
PbgArchive `/GL` context now has five linked-PE exact functions / 214 bytes.
The diagnostic anchor image remains non-runnable and receives no product or
physical-owner credit. A fresh Factory Ghidra operation-discovery attempt was
unavailable because the native implementation differed from its operator
binding; it returned no semantic evidence, so this packet relies on direct
target and compiler/linker evidence only. A fresh PbgArchive backlog probe after
the promotions reports only seven mismatches and no remaining structural-exact
candidate for this source.

## Completed packet: canonical LTCG exact replay

The exact-unit graph now supports `artifact_kind="linked-pe"` alongside normal
COFF. Shared `scripts/ltcg_link.py` owns the fixed cold `/GL` compile and link
context. `build-match-unit.py` validates the artifact-specific manifest and
builds one image/map/PDB set per shared context. `compare-linked-function.py`
then rebinds PE/map/PDB identity, selects one unique `source.ltcg.obj` PDB
contribution, requires its full manifest size, verifies the pinned Capstone
5.0.6 component hashes, decodes the entire extent, rejects missing or extra
linked fields, validates declared semantic symbols through map publics or the
generated unique anchor aliases, replays target addresses, and compares every
byte. `replay-exact-units.py` cold-builds and checks both artifact kinds.

Two independent cold canonical builds reproduce
`PbgArchive::SeekPastInt` at `0x00435270` across **12/12 bytes** and
`PbgArchive::SeekPastString` at `0x00435280` across **51/51 bytes**. Their
complete PDB-owned extents decode without gaps, contain zero linked fields, and
are raw-equal to all **63 target bytes**. A separate focused `/OPT:REF` test
emitted a 16-byte ordinary stack-ABI `SeekPastInt`; the declared `/OPT:NOREF`
source context is required to recover the target's 12-byte private-register
form. These two accepted claims bind only the declared bounded LTCG context.
The non-runnable anchor image is not a reconstructed product, and production
TU partition, physical owner, link order, data owners, and whole-build closure
remain unknown.

## Completed packet: linked LTCG backlog diagnostics

The repository now parses MSF 7/PDB DBI section contributions directly and joins them to VC7.1 linker-map publics and PE raw bytes. It fails closed unless the PDB RSDS GUID/age matches the PE CodeView record and map timestamp, image base, public start, and section layout all agree. `verify-toolchain.py --execute` exercises this PE/PDB extent path in addition to the existing compiler, resource, and link smoke.

`scripts/probe-ltcg-backlog.py` cold-compiles one source at a time with fixed `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL`, then performs an iterative diagnostic link. Missing externals receive distinct non-runnable data anchors, allowing every linked field to retain a unique candidate-symbol identity without manufacturing executable stubs. PE HIGHLOW fields and decoded external control-flow operands are masked only for structural comparison. The harness, every result, and the report itself explicitly carry no acceptance authority.

The complete authored backlog scan covers all **66 functions across nine sources**. It found **13 structural-exact diagnostics / 715 bytes** and 53 mismatches: three Enemy lifecycle bodies, four Lzss tree helpers, five PbgArchive helpers, and `CPbgFile::GetFullFilePath`. The two PbgArchive pointer-advance helpers have no linked fields and reproduce all **63 raw bytes**. The other 11 candidates contain 34 resolved fields requiring explicit symbol/target review before any canonical linked unit can exist. Two independent cold PbgArchive passes reproduced the same result/extent/field signature. At checkpoint `26af714` all 13 remained diagnostics; later canonical packets have promoted the five PbgArchive and four Lzss candidates, leaving four structural candidates under review.

## Completed packet: batch normal-COFF backlog diagnostics

Checkpoint `d59f975` added `scripts/probe-exact-backlog.py`. It cold-compiles each selected source once under fixed `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc`, enumerates strict COFF functions, joins conservative source-name hints only when one external symbol matches, and runs every authored source backlog entry through target-bound probe comparison. Unresolved mappings, true size/byte mismatches and structural-exact results are separate states; the entire report has `acceptance_authority=none`.

The complete current run covers all **66 authored, source-present, non-exact functions across nine sources**. All 66 map uniquely to a COFF symbol and all 66 are real mismatches; there are zero unresolved and zero structural-exact results. Thus the fixed normal profile has no additional exact unit waiting for ledger-only promotion. This does not reject alternate normal profiles or LTCG ownership.

Probe relocation reporting now also handles a candidate relocation that crosses a shorter target extent: it retains the object addend, marks target encoding/candidate unavailable with `target_extent_complete=false`, and reports a normal size mismatch instead of aborting the batch. Canonical comparison remains strict. No source behavior, exact ledger row, or whole-build state changed.

## Completed packet: strict COFF extent diagnostics

Checkpoint `96f17c6` added decorated-symbol/extent/relocation enumeration and removed the unsafe probe fallback that could slice a caller-requested target length beyond the candidate COMDAT. Missing candidate bytes are now explicit differences. The corrected diagnostic reports `PbgArchive::GetEntryDecompressedSize` as a true 25-byte object against the 51-byte target, and all **20 canonical units / 700 bytes** still cold-replay exact under the strict extent rule.

## Completed packet: exact-work backlog reporting

Checkpoint `88800b6` added `scripts/report-exact-backlog.py`. It joins and validates the function, origin, source-mapping and exact ledgers, with source/module/state filters and JSON output. The 90 source mappings split into **20 canonical exact**, **66 authored non-exact functions / 26,540 bytes**, and **four origin-review Player callbacks / 38 bytes**. Progress and live status now expose the authored/origin-pending split.

## Completed packet: canonical LZSS state-reset backlog

Checkpoint `621add9` promoted the reviewed `Lzss::InitEncoderState` body at `0x00435FD0-0x00435FF9`. Two cold builds reproduced **42/42 bytes and three DIR32 relocations**: dictionary base `0x0048F868`, plus `g_LzssTree` base `0x00477858` with object addends `+4` and `+0x18010`. Ghidra still misses the standalone helper; raw target code and the encoder's inline reset establish its extent and behavior.

## Completed packet: canonical PbgArchive lifecycle backlog

Checkpoint `4ed34ee` promoted `PbgArchive::PbgArchive`, `PbgArchiveEntry::~PbgArchiveEntry`, and `PbgArchiveEntry::PbgArchiveEntry`. Two cold shared-object passes reproduced **51/51 bytes and one REL32 `free` relocation**. Fresh Ghidra review modeled only the entry destructor; raw boundaries and reviewed vector-constructor references remain the evidence for the two inventory-missed constructors.

## Completed packet: exact replay workflow and ResFile backlog

Checkpoint `9b5e7eb` promoted six `CMemoryPbgFile` methods across **210/210 bytes and four relocations** and added `scripts/replay-exact-units.py`. The tool cold-builds one shared object per source/profile and strictly compares every selected unit. The then-current 16 canonical units used three shared outputs instead of compiling the same three translation units 16 times. Manifest validation rejects malformed extents/relocations, overlapping target ranges and inconsistent output sharing; ledger validation joins canonical units to exact, source, function and origin records and requires both exact-unit sets to agree.

## Completed packet: canonical PbgFile exact backlog

Checkpoint `bd9b2e3` promoted six already reviewed PbgFile methods. Current `src/PbgFile.cpp` compiled from absent output paths twice and replayed exact across **220/220 bytes and nine DIR32 relocations**: destructor 46 bytes/three relocations, `Close` 34/one, `Read` 57/one, `Tell` 25/one, `GetSize` 21/one, and `Seek` 37/one. It also extended diagnostic probe output with target-derived relocation candidates; these remain non-authoritative until a reviewed canonical unit passes.

## Completed previous hard packet: Enemy ECL dispatcher context seam

This packet deliberately stayed on the central 14,416-byte `EnemyRuntimeView::DispatchEclInstruction` owner at `0x0040E770-0x00411FBF` instead of pivoting to easier leaf functions. It bounded opcodes `0x15D-0x164`, the nested difficulty tables, and the adjacent integer argument seam. The observable outcome was structural: recover context ownership/ABI and denominator gaps needed for a natural future dispatcher, without writing a partial `default`-return switch and falsely promoting the dispatcher to source-present.

Direct TH10 table decoding maps the eight selected opcodes to case entries `0x004118BB`, `0x004118E2`, `0x0041198B`, `0x004119FC`, `0x00411A3E`, `0x00411B23`, `0x0041009B`, and `0x004100EA`. `0x15F` uses rank interpolation with target constants `1024.0f` and `1/2048.0f`; `0x162` performs the integer analogue `(high-low)*(rank+0x400)/0x800 + low` with the target signed truncation-toward-zero correction. Difficulty cases reach five-entry tables at `0x00411F98` and `0x00411FAC`. These observations are provisional semantic/target evidence, not exactness credit.

The strongest ownership result is the ECL execution context embedded in full Enemy `+0x008..+0x101F`, exactly `0x1018` bytes, with the active-context pointer at full `+0x004`. Context `+0x004` is the current ECL instruction; its target-observed header has opcode at `+0x004`, variable-operand mask at `+0x008`, and dword operands at `+0x010`. Generic typed-operand helpers use context `+0x1008` as operand-stack state, `+0x100C` in local-storage addressing, and context `+0x1014` as the operand-resolver owner. Constructor machine code stores the full Enemy itself at full `+0x101C` = context `+0x1014`, directly binding the generic context to the already-reviewed Enemy operand vtable. Maintained `src/Enemy.hpp/.cpp` now express this exact-size context/instruction view while preserving all full-object offsets.

`0x00412A00-0x00412A0F` and `0x00412A10-0x00412A5A` are now boundary/ABI-reviewed integer argument wrappers. Both take `EnemyRuntimeView*` in EAX and argument index in ECX; the first forwards to the generic int reader and the second resolves a writable int argument address. Complete target scans find all 30 and 9 direct calls respectively inside the central dispatcher, but whether these are source-written helpers or compiler/LTCG outlines remains unknown, so origin stays `unknown/review` and neither is source-present.

Raw corridor review also found three initial-inventory misses: `0x00412A60-0x00412A6D` (14B), `0x00412A70-0x00412A7D` (14B), and `0x00412A80-0x00412A90` (17B). They are CC-delimited helper-shaped bodies adjacent to the seam, but fresh Ghidra xrefs plus complete direct-REL32 and absolute-pointer scans find no target references. They are therefore added only as new `unknown/review` candidates; authored/source/exact denominators do not increase.

Adjacent-game hypothesis provenance for this packet: clean committed TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97`; clean committed TH09 HEAD `f1d4c1e332828a0cb8b9308b61b79ce77f86e4d0`; TH095 live tree was dirty (4 paths), so only committed HEAD `dee282c33dbb75ca0545d3115cd421a6e4f6a7bd` was consulted. Adjacent source supplied only ECL operand source-family hypotheses after TH10-local recovery; no adjacent addresses, opcodes, layouts, owners, or exactness were transferred.

Pinned VC7.1 SP1 build6030 compiles the refined Enemy source under fixed normal and `/GL`. A separately compiled clean-HEAD baseline and the refined normal object emit byte-identical 586-byte `EnemyConstruct` COMDAT contents with the same four relocation sites; both fresh target diagnostics are non-exact at `63/657` comparable bytes. The old `64/657` note is superseded. This is structural source feedback only, not constructor or dispatcher exactness.

The first cold canonical replay after adding the header declarations correctly failed on the int lvalue resolver because VC7.1 renumbered compiler-internal `$L...` symbols by exactly +37. Clean-HEAD versus revised COFF symbol review proved every int/float resolver jump-table/case label kept the same section offset, and the canonical relocation offsets, types and target VAs are unchanged. `config/match-units.toml` therefore refreshes only those generated object-symbol names. A second cold deletion/rebuild/replay then returns exact 6/6, 11/11, 80/80 and 80/80 for all four configured units. The failed receipt is retained as recovery evidence; exactness was not assumed across the manifest update.

## Completed hard packet: Enemy ECL operands

The packet reconstructed the four vtable-owned operand methods immediately following the previously reviewed 14,416-byte Enemy ECL dispatcher physical owner. Their boundaries/origin were already reviewed authored-game denominator entries at packet entry; this packet adds maintained source and exactness evidence rather than inflating the authored denominator.

| Address | Maintained source | Target bytes | Final state |
| --- | --- | ---: | --- |
| `0x00411FC0-0x004122F3` | `EnemyFullObjectView::ReadIntOperand` | 820 | source-present, non-exact |
| `0x00412300-0x0041234F` | `EnemyFullObjectView::ResolveIntOperand` | 80 | source-present, canonical exact |
| `0x00412350-0x004126C3` | `EnemyFullObjectView::ReadFloatOperand` | 884 | source-present, non-exact |
| `0x004126D0-0x0041271F` | `EnemyFullObjectView::ResolveFloatOperand` | 80 | source-present, canonical exact |

The reviewed full Enemy vtable at `0x0046D0C0` owns these entries in slots 1-4. Raw target review closes each `RET 4`, adjacent padding and each resolver jump table. Ghidra does not model the four vtable entries as standalone functions; its semantic view remains provisional and carries no exactness credit.

## Operand protocol and layout refinement

The two lvalue resolvers and the target spawn copy independently close the 0x20-byte block at full Enemy `+0x1138..+0x1157` / runtime `+0x0FC..+0x11B` as four writable integer ECL variables followed by four writable float ECL variables. Spawn executes `REP MOVSD` for exactly eight dwords from request `+0x20` to full Enemy `+0x1138`, so maintained `EnemyEclVariableView { int integers[4]; float floats[4]; }` is shared by `EnemyRuntimeView` and `EnemySpawnRequestView`. This refines the prior neutral eight-dword transport block without changing the reviewed object extents.

Both readers preserve the target's exact 51-selector domain `-10000..-9950`. TH10-local cases cover RNG U32/unit/signed-unit values, signed random angle scaling, current/offset/base motion values, Player position and angle-to-point helpers, update timer, runtime flag bit `0x10000`, the four int/four float variables, special-enemy coordinates, managed-VM id resolution followed by signed VM word `+0x38A`, rank, difficulty, velocity angle, life and difficulty equality tests.

The integer and float readers deliberately remain separate because TH10 contains type-dependent behavior elsewhere. Direct table entry sends integer selectors `-9996` and `-9976` to target `0x0041219A` while `ECX` still names this Enemy, so both readers expose this Enemy's world-Y and the integer form applies the target float-to-int helper. Only selector `-9962` enters through `0x00412192`, replaces `ECX` with the manager primary special-enemy pointer, and then falls through to `0x0041219A`; selector `-9963` resolves the corresponding special-enemy X directly. Maintained source preserves this target control flow rather than normalizing it from adjacent games.

TH10-local writers establish `0x00474C74` as a `0..4` difficulty index and `0x00474C98` as a signed rank-like value restored from replay and adjusted/clamped to `[-0x400,+0x400]`; maintained source uses descriptive names `g_EnemyDifficulty` and `g_EnemyRank`, not original-identifier claims. Helper evidence independently identifies RNG helpers at `0x0044B9E0/0x0044BB20/0x0044BB90`, Player angle-to-point helper `0x00426660`, float-to-int helper `0x00463B2C`, and VM id resolve-or-clear helper `0x00449450`. Source does not add a target-absent null guard around the VM lookup.

Committed adjacent source was consulted only after TH10 switch tables/storage were independently decoded. TH08 observed HEAD `a45e99fb1942714e6edded20847e32a654d56f97` was clean; adjacent material only corroborated the broad typed int/float/lvalue ECL source family. No selector, address, layout, ABI, owner or exactness fact was transferred. Dirty adjacent untracked content was not used.

## Compiler and exactness feedback

Final maintained `src/Enemy.cpp` compiles with pinned VC7.1 SP1 build6030 under fixed normal `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc` and the same profile plus `/GL`.

True final normal `/Gy` extents versus target are:

- `ReadIntOperand`: **800 / 820 bytes**; target-window diagnostic `mismatch`, **115/452 comparable bytes**, 92 object relocations, `acceptance_authority=none`.
- `ResolveIntOperand`: **80 / 80 bytes**.
- `ReadFloatOperand`: **800 / 884 bytes**; target-window diagnostic `mismatch`, **95/540 comparable bytes**, 86 object relocations, `acceptance_authority=none`.
- `ResolveFloatOperand`: **80 / 80 bytes**.

The two 80-byte lvalue resolvers each have an explicit canonical `artifact_kind="coff"` unit. Each complete owned extent carries five DIR32 relocations: one jump-table relocation plus four local case-target relocations. Current-source clean builds and repeated target-bound comparison return `result="exact"`, 80/80 bytes, with all relocation targets matching the target. These are the only new exact promotions in this packet.

The two readers remain source-present/non-exact. Successful `/GL` compilation preserves a possible production LTCG/interprocedural context but gives no standalone exactness credit.

Because all current Enemy exact units share `src/Enemy.cpp`, the final integration gate deletes/rebuilds and compares every configured exact unit after the operand edits:

- `enemy-manager-draw-callback`: exact 6/6, zero relocations;
- `enemy-ecl-dispatch-forwarder`: exact 11/11, one relocation;
- `enemy-ecl-int-lvalue-resolver`: exact 80/80, five relocations;
- `enemy-ecl-float-lvalue-resolver`: exact 80/80, five relocations.

Total repository canonical exact coverage after the Enemy packet was **4 functions / 177 bytes**.

## Ledger and verification planes

Current ledger:

- candidates: **1277**
- origin/boundary pending: **1130**
- authored: **132 / 53,233 bytes**
- excluded: **15**
- source-present: **104 / 27,584 bytes**
- canonical exact: **46 / 1,650 bytes**

Current hierarchy packet delta from `d4af1a5`: **+7 functions / +95 exact bytes**, seven source mappings, and one compiler-generated deleting-destructor denominator entry. The authored source backlog remains **54 functions**. Session exact delta from `444eb1a...` is **+42 functions / +1,473 bytes**.

**Source presence:** seven ECL-host class functions became source-present.

**Exactness:** repository canonical exact coverage is 46 functions / 1,650 bytes. The complete mixed set cold-replays through the configured shared artifacts across five source files. This exact lane does not establish production object ownership or whole-build closure.

**Whole build:** actual final `python3 scripts/build.py` returned **RC2 / explicitly open**. Production compiler flags, TU partition, libraries, resources and link order remain unknown. `build.py --check`, tracking, toolchain execution and public CI pass this honest open state.

**Runtime:** not performed. There is no closed faithful reconstructed Windows-i386 product to execute.

**Factory Truth:** no Factory replay or acceptance submission was performed by this session. Repository canonical exact results and Factory accepted facts remain separate states.

## Scratch and recovery artifacts

The overall recovery/resume entry inventory earlier in this conversation was **365 regular files / 4,218,280 logical bytes / 0 files >64 MiB**. At the clean `2053a39e...` hard-packet boundary, `.analysis/` contained exactly **319 regular files / 3,709,138 logical bytes / 4,464 KiB allocated / 0 files >64 MiB**. Legacy/unknown analysis, other campaigns, Ghidra state, toolchains, Wine state and the private target were preserved.

The hard campaign `.analysis/gpt-web/20260914-enemy-dispatcher-hard/` exits with **28 files / 257,221 bytes**. It retains the manifest, bounded dispatch/table/helper disassemblies, top-level and nested table maps, clean-HEAD baseline source/object evidence, final normal and `/GL` objects, constructor/read-int/read-float diagnostics, local-label shift receipt, exact replay receipts (including the intentional failed pre-refresh receipt), and whole-build logs. Superseded current-packet `Enemy.context.*` scratch and reproducible `build/match/*.obj` outputs were removed only after final receipts existed.

The previous Web checkpoint's final `.analysis/` inventory was **347 regular files / 3,966,359 logical bytes / 4,796 KiB allocated / 0 files >64 MiB**. No legacy/unknown artifact was bulk-deleted, and the ignored private `resources/th10.exe` was not modified, moved, staged or committed.

The linked-diagnostic, canonical replay, PbgArchive, Lzss, final-structural, backlog-ranking, ECL lifecycle, and current host-hierarchy campaigns each retain one compact manifest; full JSON reports, objects, linked images, maps and PDBs are reproducible below ignored `build/`. No copied target was created.

## Next hard frontier

Continue the ECL core before returning to the general ranked backlog. The recovered host hierarchy makes the 71-byte reset at `0x0040C730` the closest connected non-exact source experiment. The large derived constructor/destructor at `0x0040D830/0x0040DAE0` now have the correct source-level class ownership but retain private LTCG ABIs and real byte mismatches; use them as structural feedback rather than exact candidates without new evidence.

After the reviewed backlog is exhausted, return to the central Enemy dispatcher with a second opcode cohort that shares the typed execution context and argument wrappers. Keep `0x0040E770-0x00411FBF` source-absent until a complete maintainable switch representation is defensible. The real production link graph remains a major infrastructure gap; canonical bounded linked-image units do not close it.

The adjacent `0x00412AA0` and later ECL helper corridor is also relevant as ABI/owner context, but smaller size alone is not a reason to abandon the central owner. The three newly discovered unreferenced `0x00412A60/70/80` candidates require origin evidence before any authored promotion.

The exact-reconstruction campaign remains active/incomplete. Faithful Windows-i386 whole-build closure, runtime validation, Factory acceptance, semantic reconstruction and portability remain open.
