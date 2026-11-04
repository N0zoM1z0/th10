# TH10 option-trail body: separate initial X/Y loads (2026-11-04)

Original SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The entire target `0x00427960..0x00427AC6` is a **359-byte** physical owner,
including the retained unreachable LEA sled. The separate 12-byte
`PlayerOptionTrailCallback` wrapper supplies its option pointer in private
ESI. Original source name/TU and authored-versus-compiler ancestry are open.

The maintained `PlayerOptionTrailCallbackBody` already used the correct
Y-anchored history pointer, reaching 355 source PDB bytes and 140/339
ordinary preliminary matching bytes in the actual three-TU VC7.1 `/GL`
`RebuildPlayerOptions` callback graph. A fresh direct disassembly comparison
shows target initial history-sample loads of X and then Y, followed by
separate option `replayPair0.x`/`.y` stores. The previous C++ source used
`option->replayPair0 = history[index]`, invoking the maintained pair copy
operator; the actual optimizer scheduled the loads differently.

The new code uses a `const PlayerPositionPair *` into the very same existing
history sample and two ordinary member assignments in X-then-Y order.
This does **not** change the sample index, source pointer, 8-byte layout,
field values, or existing zero/nonzero option-mode branches. This change
was tested against two distinct other source hypotheses, neither retained:
moving option-mode retrieval after the copy emitted 371 bytes and
30/339 ordinary matches, and caching the sample index emitted 311 bytes
and 16/339. All temporary variants were restored.

With the retained change, actual VC7.1 SP1 `/GL` compilation in the real
`Player.cpp` + `AnmVmId.cpp` + `AnmManager.cpp` graph gives a full PDB
contribution **357 bytes against 359 target**. Relocation-aware preliminary
ordinary equality advances **140/339 to 155/339** (15 more matching byte
positions). The five candidate fields remain separately visible; **none of
these size-mismatched comparisons is canonical exact**, and the original
source ABI/production TU have not been established.

The affected canonical exact owners were cold-replayed explicitly in this
same source graph and still pass **123/123 bytes**: the 12-byte trail
callback, 12-byte special callback, and 99-byte special callback body.
The source remains **NONEXACT** and no `config/matches.csv` row has been added.

Reproduce only affected exact units (never full cohort):

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit player-option-trail-callback \
  --unit player-option-special-callback \
  --unit player-option-special-body --json
scripts/repo-python scripts/ci.py
```

Ignored actual build/diagnostic receipts:
`.analysis/gpt-web/20261104-next/player-shape-controls.json`,
`player-split-cold-guards.json`, and `player-split-current.json`.
This narrows a complete *candidate* codegen frontier, not product/runtime,
portability, or complete source-ancestry acceptance.
