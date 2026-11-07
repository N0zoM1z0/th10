# ECL AddScriptData: native 32-bit merge counters and insertion cursor

The original Japanese TH10 v1.00a target, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`,
contains the complete 589-byte `EclVmScriptDatabase::AddScriptData` at
`0x00450220`. The existing implementation merges sorted eight-byte
subroutine entries from an SCPT file, using an unchecked signed
`subroutineCount` after accumulation from a serialized file count.

The original x86 subroutine insertion shift decrements its index as a
32-bit register word, even if the signed count wraps to `INT_MIN` on a
corrupt script. The old C++ `--index` would have signed-overflow UB at
that value. The new loop assigns the result of unsigned 32-bit subtraction
back to the native signed index before preserving the same signed
comparison. The `sortedCount` and `added` increments are likewise now
explicit unsigned four-byte additions, published to the original signed
count words. The sequence, insertion limits, copy, freeing, `strcmp`,
`strlen` and SCPT cursor reads have **not** changed. There is no added
bounds repair or claim that malformed input is safe.

## Source-pinned original and neighbor comparisons

Six natural variants were compiled and linked separately in the current
ECL/Enemy **two-TU** VC7.1 SP1 `/GL` graph: word-decrement, word-sorted
increment, word-added increment, both increments, assignment-form word
-decrement, and the installed three-counter combination. Each compared
the whole `AddScriptData` original target and the complete 7,020-byte
`EclVmContext::Run`, not only selected instructions.

| Variant | AddScriptData original comparable bytes | Run original bytes / fields |
| --- | ---: | ---: |
| Original current-source baseline | 502/581 (589-byte body) | 6,997/7,020; 189/189 |
| Unsigned lvalue predecrement | **500/581**, rejected | 6,997/7,020; 189/189 |
| Word-sorted increment | 502/581 | 6,997/7,020; 189/189 |
| Word-added increment | 502/581 | 6,997/7,020; 189/189 |
| Both increments | 502/581 | 6,997/7,020; 189/189 |
| Word-decrement assignment | 502/581 | 6,997/7,020; 189/189 |
| **All three (installed)** | **502/581**, 589 bytes | **6,997/7,020**, **189/189** |

The AddScriptData full owner keeps the two original CRT call fields at
`+0x6C` and `+0x101`; the other 79 ordinary mismatches remain. The giant
Run remains **NONEXACT**, as does AddScriptData. The zero difference
relative to the previous candidate does not grant new exact codegen credit.

Fresh current-source selected canonical cold replay:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-find-subroutine --unit ecl-vm-read-float-value \
  --unit ecl-vm-host-run --unit ecl-vm-stack-pop --json
scripts/repo-python scripts/verify-handoff-pins.py
scripts/repo-python scripts/ci.py
git diff --check
```

All four previously exact original-bound owners pass **575/575** bytes in
their two selected build graphs, as does the seven-stage repository CI.
The reproduction driver, six per-variant receipts, native source pins and
selected exact receipts are kept in ignored `.analysis/gpt-web/20261107-active/`
(`ecl-record-counter-trials.py`, `ecl-record-counter-results.json`,
`ecl-record-counters-guards.json`); native linked artifacts are under
`build/gpt-web/20261107-active-ecl-counter/`.
