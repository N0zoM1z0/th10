# TH10 per-corner colored quad renderer, 0x0043BFA0 (501 bytes)

Target: original Japanese TH10 v1.00a, SHA-256 defined in
`config/target.toml`. The reviewed authored body is exactly
`0x0043BFA0..0x0043C194`, followed by CC padding, with a `RET 0x10`.
The source mapping was absent before this checkpoint; the neighboring
0x0043BDA0 483-byte `DrawColoredRectangle` had been reconstructed exact.

## Independently checked target behavior

We decoded the entire target body and compared it with both the native
sibling and the target-hash-attested read-only Ghidra decompilation.

- The receiver is read through live-in **EDI** after the genuine ANM manager
  `FlushVertexBuffer` callee at `0x00442F50`. The rectangle contains four
  floating coordinates at offsets 0,4,8,12.
- Four 20-byte `XYZRHW | DIFFUSE` vertices are constructed using the same
  temporary `AnmFloat3View` XYZ layout as the already-exact 483-byte sibling,
  and every W is set to exactly 1.0f. The native stack frame is `0x5C`.
- Unlike the sibling, four distinct dword stack parameters are copied into
  vertex color fields in array order. The actual native reads refer to the
  first four caller words and its epilogue is `RET 16`. The private EDI
  transport of the rectangle is an observed ABI, not an assumed ordinary
  source calling convention.
- The four initial texture stage-state calls select diffuse alpha/color,
  then render-state dest blend=6, FVF=0x44, and `DrawPrimitiveUP` as a
  2-primitive triangle strip with 20-byte vertex stride. The six cached
  ANM render-state fields are invalidated exactly as in the sibling.
- Four final stage-state calls restore modulation/texture argument setup.
  Target calls are through the actual device vtable, not fake stubs.

`AnmFloatRectView::DrawGradient` is the maintained descriptive declaration
in `src/AnmRectangle.cpp/.hpp`. The original name/TU/caller identity is
still open. We did not modify the previously accepted 483-byte function.

**Current-source supersession (2026-11-06):** A genuine 14-TU pinned VC7.1 SP1 LTCG graph, unlike the isolated compile described below, naturally reproduces the original complete **501-byte** owner. An independent newly registered target-bound canonical cold replay passes **501/501 bytes and all 14 independently target-bound fields**. See the [accepted exact proof](../anm-gradient-rect-501-exact-20261106/README.md). Everything below is historical isolated-context evidence, **not the current acceptance status**.

## Codegen status and narrow guard

The isolated pinned VC7.1 `/GL` source build/link gives **503 bytes** against
501 target bytes. This is **NONEXACT**: the compiler emits an ordinary member
receiver transfer at entry, whereas the native body consumes EDI as a private
live-in. Its preliminary comparator reports 24/445 ordinary comparable bytes
and 14 candidate fields, not an independently bound complete proof. Matching
PDB size closely does not establish field binding or equality; therefore no
canonical `config/matches.csv` credit is claimed.

Original direct code xrefs are not established for this owner; one should not
invent a fake caller just to force EDI. The existing natural member source
retains all operations and the four argument colors while preserving this
open private ABI. A targeted cold replay guards the already-exact rectangle
and associated caller. The private diagnostic output is
`.analysis/gpt-web/20261103/rectangle-gradient-first.log`.
