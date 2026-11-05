# TH10 ECL Run: arithmetic opcode stack words now use shared 32-bit scalar storage

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The nine independent arithmetic opcode cases `ADD/SUBTRACT/MULTIPLY/DIVIDE/MODULO_INT`
and `ADD/SUBTRACT/MULTIPLY/DIVIDE_FLOAT` each pop two *four-byte* words,
modify the left word, and push the resulting word with the appropriate
value type tag. The existing `EclVmScalar` union was already proven to
represent a single 32-bit VM stack word by its tagged Push/Pop, integer/
float operand readers and CALL argument conversions, but these nine cases
still declared untyped `int`/`float` locals independently.

The maintained `EclVmContext::Run` now uses `EclVmScalar right`/`left` and
accesses `.integer`/`.real` at **all 18 pop destinations, nine actual
calculations and nine push sites**, keeping the previous operation order,
stack tag, native 4-byte size, stack address, x87 arithmetic and control
flow. This is a typed native stack-word recovery rather than an additional
buffer, a new ABI, or an inferred original source spelling.

## Complete original-target compiler evidence

A fresh pinned VC7.1 SP1 **two-source `/GL`** graph recompiles
`src/EclVm.cpp` and `src/Enemy.cpp`. Its whole PDB-owned
`EclVmContext::Run @0x0044E1A0` remains **7,020 bytes** and satisfies
**all 189 independently original-bound fields**, including original branch
and table destinations. Ordinary target code matches **6,997/7,020** bytes;
the same 23 unresolved bytes remain, so the boss is **NONEXACT** with no
new ledger credit. The independently complete 551-byte `StartSubroutine`
source candidate retains *all four* original native call sites
(`+0x9E/+0xD2/+0xE8/+0x1DE` candidate positions) and zero candidate
ordinary code regressions versus the preceding pinned source graph.

Selected cold original-target replay of the actual stack helpers
`ecl-vm-stack-pop` **122/122** and `ecl-vm-stack-push` **96/96** passes
**218/218 previously exact bytes**. All seven lightweight repository CI
checks and `git diff --check` pass. No full historical cold replay; no
whole-game product, runtime, portability or original-private-ABI claim.

Private reproducibility:
`.analysis/gpt-web/20261106-ecl-arithmetic-scalars/verify.py`,
`build/gpt-web/20261106-ecl-arithmetic-scalars/current/native-result.json`,
`.analysis/gpt-web/20261106-ecl-arithmetic-scalars/selected.json`.

Current `src/EclVm.cpp` SHA-256: `468d49050f66e9969525027faed0724764fee70416c597359d19dd584fe2a364`.
