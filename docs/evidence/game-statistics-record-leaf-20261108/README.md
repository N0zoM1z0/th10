# TH10 typed GameStatistics record accessor leaf

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The 11-byte leaf at `0x004088A0` computes an indexed record address:

```asm
imul eax, eax, 0x437c
lea  eax, [eax + ecx + 8]
ret
```

This is now expressed as ordinary typed C++:

```cpp
GameStatisticsRecord *GameStatisticsRecordAt(
    i32 index, GameStatisticsOwner *owner)
{
    return &owner->records[index];
}
```

The types are not guessed from the stride alone. `GameStatistics.hpp` already
asserts `offsetof(GameStatisticsOwner, records) == 8`,
`sizeof(GameStatisticsRecord) == 0x437c`, and the enclosing owner's x86 layout.
The resulting source directly uses the existing fixed-size record array.

The target boundary has no callers or xrefs. The receiver type is supported by
an independently recovered owner layout, but the leaf's original source owner,
source name, and authorship remain indeterminate. `GameStatisticsRecordAt` is a
descriptive name for the observed address calculation, not an ownership claim.

## Focused exact replay

Command:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit game-decomphelp-fulltu-004088a0 --json
```

Result: `exact`, 11/11 raw bytes, complete 11-byte PDB section contribution,
zero differences, and no linkage fields. The full 1,501-unit match-unit graph
also passed validation. No full cold replay, whole-image claim, or runtime
claim is made.
