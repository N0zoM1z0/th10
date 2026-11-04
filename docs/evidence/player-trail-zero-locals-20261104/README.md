# TH10 359-byte Player option trail: complete-length zero-mode source improvement

This checkpoint uses the pinned original Japanese TH10 v1.00a executable
(SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`).
The physical target `0x00427960..0x00427AC6` is **359 bytes**, including
its unreachable alignment sled. The 12-byte registered callback at
`0x00427950` is a separate exact function that transports its option receiver
through a private ESI live-in. The historical original C++ declaration,
production TU, and authored-versus-compiler origin remain unestablished.

## Source change and original instruction evidence

`src/Player.cpp` already carries the correct first history sample X/Y loads
and Y-anchored seven-sample interpolation loop. The original target zero-mode
branch loads `player->positionX` and `player->positionY`, reads the X/Y values
saved in the option record and writes their arithmetic differences to
`option->replayPair2`, at the real option offsets +0x44 and +0x48.
The maintained source previously computed these two differences inline.
This checkpoint gives the two original option values and the two original
player values named, scoped `const int` locals, then computes and writes both
unchanged differences. This is a natural source-level lifetime/order model,
not a fabricated register, inert padding or byte-level patch.

## Actual native full-owner comparison

The maintained VC7.1 SP1 `/GL` source graph is `Player.cpp` with real
`AnmVmId.cpp` and `AnmManager.cpp` support, using the actual
`RebuildPlayerOptions` entry. For this complete physical owner, linked PDB
contribution sizes and ordinary positional diagnostics evolve as follows:

| Maintained source shape | PDB size / target | Diagnostic ordinary identical bytes |
| --- | --- | ---: |
| Original aggregate sample and X-anchored loop | 355 / 359 | 92 / 339 |
| Y-anchored history writes | 355 / 359 | 140 / 339 |
| Separate initial sample X/Y reads | 357 / 359 | 155 / 339 |
| **Named zero-mode X/Y and position locals** | **359 / 359** | **205 / 339** |

The new source fixes the physical **PDB extent length**, but **does not yet
match the complete bytes or the original field geometry**. Five candidate
address fields are present, but independently binding the target's full
field locations, all local destinations and the actual compiler-private ABI
remains open. The ordinary positional metric is diagnostic only; the
**entire 359-byte owner remains NONEXACT**. No `config/matches.csv` row is
admitted.

Rejected native source variants were compiled, not inferred: computing
both difference locals before the writes emitted 343 bytes / 32 ordinary
matches; `register` or mutable option mode kept 357 bytes / 155 matches.
The independent original 359-byte disassembly and a fresh complete PDB
comparison were checked against the retained source.

After the source change, a new selected cold canonical replay in the actual
callback graph passes all affected established exact bodies:
`PlayerOptionTrailCallback` (12/12), `PlayerOptionSpecialCallback` (12/12),
`PlayerOptionSpecialCallbackBody` (99/99), totaling **123/123 bytes**.
No whole-history cold replay was run. Private native compiler receipts:
`.analysis/gpt-web/20261104-next/player-359-zero-branch.json`,
`player-359-zero-improvement-guards.json`, and
`player-359-zero-improvement-maintained.json`.

To repeat only affected guards:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit player-option-trail-callback \
  --unit player-option-special-callback \
  --unit player-option-special-body --json
scripts/repo-python scripts/ci.py
```

This source-codegen improvement does not establish original full product,
runtime semantics or portability.
