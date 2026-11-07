# Game statistics loader: original 32-bit allocation-word shift

Target: original Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The complete original `GameStatisticsOwner::Load42B030` is **423 bytes**.
At `0x0042B071` the original i386 instruction is `SHL EAX,2`: the value
from the score-file header's 32-bit uncompressed-size field is shifted as
one native dword before passing it to the allocation call. The previous
C++ wrote `header->uncompressedSize << 2`, where the declared `int` may be
negative or overflow on a shift, making this expression undefined in C++.
The maintained source now shifts `static_cast<unsigned int>(...) << 2`,
retaining the original modulo-2^32 result, allocation size, operand read,
and file/record processing order. It introduces no additional size bound,
allocation behavior, error path, or independent data owner. Signed 32-bit
conversion and whole-system behavior outside this pinned VC7.1 i386 build
are not claimed.

## Actual current-source original-target validation

With the actual edited `src/GameStatisticsLoad.cpp`, the pinned VC7.1 SP1
statistics caller graph was cold-compiled under its declared six-source
`/GL /GS /EHsc /Gz` configuration. Only the three affected canonical units
were selected:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit game-statistics-load \
  --unit game-statistics-finalize \
  --unit game-statistics-initialize-tail --json
```

All three strict complete owners pass against the *original* executable,
including their independently bound linkage fields: loader **423/423**,
finalizer **564/564**, tail initializer **142/142** (total **1,129/1,129**).
This is a regression-preserving native semantic correction, **zero new
exactness credit**: all three units were accepted previously. Constructor,
factory and 274-byte record initializer remain NONEXACT. The graph proves
neither a runnable complete game nor original physical TU provenance.

The selected cold JSON/logs are ignored local evidence at
`.analysis/gpt-web/20261107-stat-shift-selected.json` and `.err`;
retained actual source PE/MAP/PDB is under `build/match-linked/GameStatisticsTail/`.
