# InitializeGameStatisticsRecord: native split-name copy and field scheduling

Original Japanese TH10 v1.00a target SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The **274-byte** `InitializeGameStatisticsRecord @0x0042ACB0` first zeroes a
`GameStatisticsRecord`, sets its header, emits 5×10 ranking-score default
entries and then writes 110 signed rank-kind pairs. The nine consumed default
name bytes consist of two four-byte words plus a final single byte at
original `0x0046E858..0x0046E860`. Each entry has a disjoint extra data word,
so writing that word between the first eight and the final name byte is
semantically the same as the previous 9-byte `memcpy`.

The installed natural C++ now uses `memcpy(..., 8)`, publishes the
independent `extra` word, then writes `name[8]`, before preserving the same
status byte, score decrement and iteration. There is no fabricated input,
assembly, forced register, target-byte patch, padding or changed loop count.
The original 110-iteration rank-kind loop is unchanged and continues to
produce the original complete 274-byte size under VC7.1 SP1.

## Current-source native evidence and explicit limit

Several independent source-order, scalar/copy and record-cursor experiments
were compiled using pinned VC7.1 SP1 normal COFF. The final maintained source
was then cold-linked from **the real, complete single translation unit** using
`/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL /GS` with the actual
`InitializeGameStatisticsRecord` entry. Its PDB contribution is **274 bytes**
with **all eight original data reference identities** (name base plus +0/+4/+8,
rank-kind base plus +0/+2/+0/+0/+4). Every original encoded address is
independently verified against the original PE; each current candidate field
is checked against the real linked map symbol and addend.

| Comparison | Previous source | Installed source |
| --- | ---: | ---: |
| Whole owner size | 274 | 274 |
| Full data-reference fields | 8 | 8 |
| Exact original field *positions* | **4/8** | **7/8** |
| Differing ordinary bytes under one **fixed common exclusion mask** | **75** | **36** |
| Ordinary compared positions with that *same* mask | 230 | 230 |

The fixed 44-byte exclusion mask is the **union** of the old/new candidate
and target field positions, applied identically to both source epochs so the
75→36 comparison is meaningful; it is explicitly a **diagnostic, not canonical
acceptance masking**. The remaining incorrect data-field position is the
second default-name dword: original `+0x53`, candidate `+0x50`. Complete
original equality is therefore **REJECTED** despite correct identities and
full PDB size. The first part of the score loop still uses different
`ESI/EDI/EAX` cursor allocation. **No new exact match or byte credit**.

Proof program and result:
`.analysis/gpt-web/20261107-stat-default/verify-installed.py`,
`build/gpt-web/20261107-stat-default/installed-cold/proof.json` and its
actual PE/MAP/PDB. Baseline native image is separately preserved under
`build/gpt-web/20261106-statistics-record/final-linked/`. The unchanged
original-source input and all tested restored variants are recorded at
`.analysis/gpt-web/20261107-stat-default/{results.json,copy-results.json,cursor-results.json,name-schedule-results.json,extra-results.json}`.
Installed `src/GameStatisticsRecordDefaults.cpp` SHA-256:
`2c0c4d290118b9a65a781fef030825b07b89dde090a24cbc1252c200cf3173df`.

No all-unit cold replay was run: there is no canonical exact unit owned by
this TU. These figures do not prove original physical source spelling,
actual game runtime, source provenance or portability.
