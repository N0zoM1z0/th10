# Enemy dispatcher spawn-tail source checkpoint

2026-10-22. The maintained dispatcher now fills absolute-spawn request fields
locally and shares the actual EnemySpawn call with relative spawn. Its complete
linked extent grows from 14352 to 14368 bytes against target 14416: 48 bytes
short instead of 64. This is diagnostic progress, **not a new exact match**.

The original Japanese v1.00a target is SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Fresh disk verification, read-only Ghidra observation and executed toolchain
preflight passed. Full mapped text remains 412289 bytes with hash
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.

## Source and compiler observations

The adopted change duplicates the existing position.y, life, scoreReward,
itemDropType and eclVariables assignments in the absolute-spawn arm. Argument
reads and stores retain their order. Both arms reach the existing spawn call.
The unused finish-position label is removed. No storage sizes, declarations,
ABI, target bytes, compiler flags or canonical recipes change.

All controls retain the 2C4 stack frame, 19 double-sentinel comparisons, all
181 selector bytes and 108 ordered physical groups. Results are local layout
diagnostics; aligned bytes and matching group sizes do not prove exactness.

| Source control | Parent bytes | Aligned bytes | Target-sized groups | Aggregate absolute group gap |
| --- | ---: | ---: | ---: | ---: |
| Previous maintained source | 14352 | 3219/11148 | 42 | 566 |
| Duplicate full spawn tail | 14480 | 3137/11149 | 42 | 690 |
| Five local rank tails | 14536 | 3229/11153 | 42 | 660 |
| **Duplicate spawn fields (adopted)** | **14368** | **3250/11139** | **45** | **348** |
| Five ranks, shared result | 14380 | 3222/11140 | 43 | 620 |
| Spawn fields plus shared rank result | 14380 | 3273/11124 | 46 | 388 |

Groups 0x109, 0x100 and 0x101 change from 16/278/198 to 17/403/81 bytes,
matching their target sizes. The absolute spawn tail jumps back into the
relative spawn group, as observed in the target. Other nonexact groups regress:
0x118/0x11a grows 183 to 193 against 179; the spell group grows 224 to 227
against 219; some laser groups lose aligned bytes. The combined rank control
improves the spell size but worsens rank groups and aggregate gap, so it is
not adopted. The full transition is recorded in [evidence.json](evidence.json).

## Focused validation

The private candidate and fresh maintained-path rebuild each compile one primary
object and reuse nine verified support objects plus the two harness files.
They are **not two full cold builds**. Their complete raw dispatcher, ANM,
ECL and laser-factory bodies agree, including after unused-label removal.

The maintained image passes 36 complete comparisons (4618 bytes, 121 fields)
and the two existing 202/219-byte laser constructor comparisons (four fields):
38 complete guards, 5039 bytes, 125 fields. No new exact credit is claimed.
ANM remains 9964 bytes, 7541/7995 aligned, 90/92 target-sized groups. ECL remains
7020 bytes, 6993 matched and 27 ordinary differences, all 189 linkage fields,
62 local destinations and the complete 171-byte float callee checked.
The selector/source enum consistency check also passes. Full CI was not run,
following the operator's preference for focused affected replay.

## Reproduction and retained evidence

The five patches beside this file are relative to the starting source identified
in evidence.json. Private producers, overlays, reports and manifests remain in
`.analysis/gpt-6-astra-medium/enemy-shared-tails-20261022/`.
The original manifest is historical after source adoption; do not restore its
source pins or run it as a current manifest. The validation manifest also predates
the subsequent notes-only ledger update.

Before rebuilding the canonical image, all 19 old native files were hash-verified
and archived under
`build/gpt-6-astra-medium/enemy-shared-tails-20261022/baseline-before-adoption/`,
and 32 source/header inputs under the private `baseline-source/` directory.
The baseline report retains its original canonical pathname; its bytes now live
in that archive. Frame and sentinel observations were revalidated against the
archived image, not the replacement canonical image.

Keep the selected private carrier, current canonical image, archived baseline
and compact producers/reports. Four rejected carriers were hash-inventoried and
removed: 64 files, 4265482 bytes. Nine unrelated D3D evidence files remain intact.
Exact ledgers, boundary/origin decisions and product/runtime gates are unchanged.
