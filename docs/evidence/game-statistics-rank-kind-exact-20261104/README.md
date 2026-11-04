# TH10 75-byte game-statistics rank-kind counter — canonical exact

The target is the original Japanese TH10 v1.00a,
SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The CC-delimited native function at `0x004088C0..0x0040890A` is exactly
**75 bytes**. Its private calling convention ignores ECX and compares the
signed integer in EDX against 110 signed bytes, checking **five per loop**
and incrementing a count on each match, for 22 records. It returns the count
in EAX. The bytes `0x004743C0..` are the same externally owned table that
`src/GameStatisticsRecordDefaults.cpp` already consumes; this change does
**not** copy, redefine or assert an original physical array allocation.
Native incoming caller and authored/generated origin remain unproven.

## Full native PDB/PE comparison

`src/GameStatisticsRankCount.cpp` uses the genuine signed table view and
an ordinary C++ `do`/`while`. A complete pinned VC7.1 SP1 `/GL` link reports
one **75-byte PDB function contribution** and **75 decoded code bytes**.
Five independent `DIR32` locations in the native owner bind to the real
rank-kind base `0x004743C0` and offsets `+1..+4`:

| Field offset | Original address | Meaning |
| --- | --- | --- |
| +0x08 | `0x004743C0` | column 0 |
| +0x14 | `0x004743C1` | column 1 |
| +0x20 | `0x004743C2` | column 2 |
| +0x2C | `0x004743C3` | column 3 |
| +0x38 | `0x004743C4` | column 4 |

All **75/75 bytes, five/five fields, and the complete PDB owner** pass the
canonical comparator with no differences. Two independent selected cold
builds produced distinct PE hashes and PDB GUIDs:

| Cold | PE SHA-256 | PDB GUID |
| --- | --- | --- |
| 1 | `51349d0114e3f526c934ade1f9a790e7e56d904b55f20d702f91c520ddc3f7d6` | `99b89a03bb01ca48bd4d41c61b4c62a8` |
| 2 | `e9f1b066601dfcc5f150331bdabfbbffe3e66de74b3741eb6b89d102e7038b5f` | `00bd19f7f5101e429a234ff77593757f` |

**Source-shape falsification:** a natural `for (index=0; index<110;
index+=5)` with the same five comparisons emitted **135 bytes** because
MSVC automatically unrolled *two* iterations; the source restored as
`do { ...; index += 5; } while (index < 110)` generates the native 75-byte
single iteration. Both compile graphs were executed; this difference is a
verified optimizer effect, not an inferred source spelling.

Reproduce only this complete owner:

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/replay-exact-units.py \
  --unit game-statistics-rank-kind-count-004088c0 --json
scripts/repo-python scripts/ci.py
```

Raw private logs/receipts remain ignored under
`.analysis/gpt-web/20261104/rank-count-*`. The original executable,
production source TU, native product, and runtime/portability claims remain
outside this acceptance.
