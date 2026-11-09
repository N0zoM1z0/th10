# Exact native recovery: ANM VM initializer (0x00401DE0)

The 273-byte VM/entity initializer is now expressed with the target's actual
register-facing source shape. The implementation lives in
src/AnmVmInitialize.cpp as a global __fastcall function. The source-facing
AnmVmView::Initialize() wrapper forwards an unused first argument in ECX
and the VM pointer in EDX.

## Verified cold replays

The affected match unit was built twice from a removed unit output directory.
Both independent cold builds report:

- Target extent: 0x00401DE0..0x00401EF0 (273 bytes).
- Candidate contribution: 273 bytes, fully decoded by Capstone.
- Normalized comparison: **273/273 exact**.
- Raw linked bytes: **equal**.
- Relocation/linkage fields: **zero**.
- First differences: none.

Receipts are preserved in report-cold-1.json and report-cold-2.json.
source-pins.json records the exact source/config inputs used by both builds.

## Source-level observations

The target's entry uses EDX as the VM pointer. It preserves the render layer
at +0x20 and position offset at +0x340, clears the complete 0x3AC-byte
object, restores those two values, then installs the default color, scale,
identity matrix, low flags and both embedded layer-list nodes.

The former ordinary member-function body placed the receiver in a different
compiler-visible ABI. Modeling the retail entry as
AnmVmInitialize(void *unused, AnmVmView *vm) and keeping it in a separate
translation unit reproduces the complete target body without hand-written
assembly or relocation normalization.

This proof is scoped to the initializer only. The adjacent script-start
helper at 0x00404F30 remains nonexact and is tracked separately; the large
EnemyBeginSpell owner also remains open.
