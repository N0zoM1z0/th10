# Enemy ECL boss: native DWORD pattern-clear cursor

Original Japanese TH10 1.00a SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`EnemyRuntimeView::DispatchEclInstruction @0x0040E770` is a **14,416-byte**
original function. It remains **NONEXACT**. The currently compiled
11-source VC7.1 SP1 `/GL /GS` linked PDB candidate is **14,352 bytes**.

Opcode `INITIALIZE_BULLET_PATTERN` receives an unchecked script index,
computes a 0x210-byte record address by native 32-bit multiply/add, and
clears exactly 0x84 four-byte words. The former C++ advanced the record
cursor with `puVar5 = puVar5 + 1`, which forms an out-of-array pointer for
an invalid script index. The maintained source now advances the existing
cursor by `sizeof(unsigned int)` using native unsigned 32-bit address
arithmetic and a pointer cast. This models the Win32 word stride without
changing the clear count, zero publication, pattern initialization, or
adding a bounds repair. Original assembly/source spelling remains unknown.

## Full 11-source native experiments, not historical full cold replay

The initial baseline was rebuilt from the **current** source epoch,
recompiling the three changed support inputs (`AnmManager.cpp`,
`EclVm.cpp`, `EnemyEclDispatcher.cpp`) and reusing only source- and
header-hash-matched objects from the previous eleven-input graph.
Four independent forms of the loop increment were then compiled and
linked into that pinned graph:

| Variant | Complete boss candidate differences | Fields |
| --- | ---: | ---: |
| Explicit native word and typed stride (installed) | 0 | 716 |
| Native `EnemyNativeWordIndex` pointer advance | 1 at +0x271F | 716 |
| Literal unsigned 4-byte address advance | 0 | 716 |
| Named unsigned next-word local | 0 | 716 |

All variants retained a 14,352-byte candidate extent, identical field
geometry and unchanged sizes/fields in four additional complete PDB
owners. The typed helper variant changed the SIB register encoding at
+0x271F from `0x31` to `0x0E` in an otherwise unchanged instruction.
This change was **not** assumed to improve the real target and was not
selected. The original image's independently decoded nearby matching
sequence uses `0x31`. Resource-bank address variants were independently
tried: three forms also changed that same candidate byte and were left
uninstalled.

The **installed source** was freshly compiled in the same complete graph:

| Current PDB owner | Candidate bytes | Candidate fields | Ordinary changes from current baseline |
| --- | ---: | ---: | ---: |
| Dispatcher | 14,352 | 716 | 0 |
| EnemyBeginSpell | 2,432 | 108 | 0 |
| EnemyFinalizeDeath | 162 | 9 | 0 |
| EnemyConfigureViewportOwner | 330 | 10 | 0 |
| Enemy full-object constructor | 672 | 5 | 0 |

These 716 dispatcher fields are **candidate-only**, not independently
original-bound fields, and there is still a **64-byte** target size gap.
A separate current-source selected canonical original-bound replay passes
five existing Enemy ECL argument helpers **87/87 bytes**, preserving their
real target bindings. The repository's seven lightweight CI stages pass.
No new exact credit, final game binary or runtime proof is claimed.

Reproduction of local targeted checks:

```sh
scripts/repo-python .analysis/gpt-web/20261107-active/enemy-live-graph.py current
scripts/repo-python .analysis/gpt-web/20261107-active/enemy-live-graph.py installed-clear-step
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-read-int-argument --unit enemy-read-float-argument \
  --unit enemy-resolve-float-argument --unit enemy-read-raw-int-argument \
  --unit enemy-read-raw-float-argument --json
scripts/repo-python scripts/ci.py
```

Ignored evidence in `.analysis/gpt-web/20261107-active/` and native
PE/MAP/PDB under `build/gpt-web/20261107-active-enemy-graph/`.
