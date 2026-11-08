# Enemy ECL rank-five float result tail (2026-11-08)

Target `EnemyRuntimeView::DispatchEclInstruction @ 0x0040E770` is 14,416
bytes. The maintained source's `ENEMY_ECL_SELECT_FLOAT_BY_RANK_5` case had
been tail-sharing both the rank-1 and rank-2 result paths. Reconstructing the
rank-1 path as an ordinary inline `ReadFloatArgument(1)` followed by the
native `EclVmContext::ResolveFloat(0)` store changes the source's control-flow
shape and restores the target-sized function.

The fresh targeted VC7.1 `/GL` build (the dispatcher TU plus the retained 11
support LTCG objects) produces a 14,416-byte function, versus 14,352 bytes in
the previous candidate. Its structural comparison rises from **1,079/11,552**
comparable bytes with 716 candidate linkage fields to **1,252/11,536** with 720 candidate
linkage fields (not independently original-bound). The 181-byte opcode-to-jump-table map remains byte-identical to the
retail map. This is a substantial local codegen improvement, not a complete
match: 1,252 bytes is still a small fraction of the comparable function, and
the first reported differences begin in the function prologue.

The neighboring `ENEMY_ECL_INTERPOLATE_FLOAT_BY_RANK` case (`0x15F`) remains
an explicit next target. Retail evaluates the rank factor before the high/low
float delta and spills its operands at `ESP+0x10` / `ESP+0x30`; this candidate
evaluates the delta first and uses `ESP+0x0C` / `ESP+0x28`. Do not infer exact
source provenance from the recovered extent or the map match.

## Reproduction and identities

Run `.analysis/gpt-web/20261108-current/enemy-dispatch-rank-tail-final-verify.py`.
It compiles only `src/EnemyEclDispatcher.cpp`, links the 11 retained support
objects and compares the PDB-owned dispatcher extent to the retail target; it
does not run a full cold replay. `results.json` records the final source and
candidate-image SHA-256 values, function extent, linkage-field count, map
identity, and selected opcode disassembly.
