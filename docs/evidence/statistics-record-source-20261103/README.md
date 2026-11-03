# TH10 statistics-record defaults @ 0x0042ACB0 — full source, NONEXACT

Original Japanese TH10 1.00a function `0x0042ACB0..0x0042ADC1` is **274 bytes** with
one stack receiver and `RET 4`. The original executable and this boundary were
reinspected by independent PE disassembly and read-only Ghidra decompilation;
this entry initializes a statistics record, **not** a replay header. Its
physical source file/name, and the two global data allocations, remain unknown.
The new maintained `src/GameStatisticsRecordDefaults.cpp` implements the
complete domain behavior with the existing 0x437C `GameStatisticsRecord` type;
`config/functions.csv`, implemented and reccmp maps now include it.

The target begins by clearing 0x10DF DWORDs (0x437C bytes) and stores magic
`0x5243`, zero version, and size `0x437C`. It then fills **five successive
batches of ten** 24-byte ranking entries starting at +0x10: scores from
1,000,000 down by 100,000 to 100,000, active flag 1, zero status and extra,
and all nine consumed name bytes from original address 0x0046E858. The first
batch ends at +0x100 and 50 entries fill through +0x4BF. It then populates
**110 signed table kinds** and their sequential indices at record offsets
+0x624/+0x628, spaced 0x90 bytes, grouped into five explicit index/kind
pairs per 0x2D0-byte iteration. The signed kind reads come from the shared
target table beginning at 0x004743C0; the function accesses all 110 entries.
This table also has a separate native user; its original physical allocation
extent has **not** been asserted or generated in this source.

The source uses local typed 24-byte score slots and 144-byte rank-kind slots
with compile-time layout checks. Direct `activeByte`/`kindData` typed cursors
in the selected version preserve the target's effective offsets and avoid
inventing extra record structure in the public `GameStatistics.hpp` layout.
No extra guards, lookup bounds, hardcoded original executable bytes, inline
assembly, or fake ABI are inserted. The actual source depends on original
data bases to be bound when establishing future canonical codegen credit.

## Bounded pinned-compiler experiments and open difference

The pinned VC7.1 SP1 `/GL` single-source diagnostic compiles and links all
274 target semantic bytes from source, but its independent PDB extent is
**302 bytes versus target 274**. The first typed-slot model matched 59/227
preliminarily comparable ordinary bytes with 12 candidate address fields;
replacing the two nested loops with target-style active-byte and rank-kind
pointers improved this to **62/227**, still **302/274**, with the same
unresolved field inventory. This experiment does **not** claim original
source-level object layout, complete 12-field target binding or exactness.
A previous archived 274-byte isolated model was also nonexact; its length
alone is not reason to promote it. Candidate logs and original typed source
are retained under `.analysis/gpt-web/20261103/stats-record/`, ignored.

No canonical match unit or `config/matches.csv` row is added. No new codegen
exact byte credit is claimed; neighboring existing exact statistics units
are separately protected with scoped cold replays, never a full replay.
