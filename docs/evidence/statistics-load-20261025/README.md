# Statistics loader: first complete exact owner, 2026-10-25

**+423 authored exact bytes / one owner.** Authored exact is now
**76205/273658 bytes,417 owners**. Complete loader, constructor and factory add
**652 source-present bytes / three owners** (624/249892 total); only loader is
exact. Existing reviewed boundaries, origins and authored denominator are unchanged.

Original Japanese TH10 1.00a target SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Preflight target/toolchain execution/tracking/status and native Factory Ghidra
attestation passed. Starting HEAD7fe7b59 had no tracked changes and only the nine
unrelated D3D evidence files, preserved and excluded throughout.

## Complete natural model and boundary

Loader42B030..42B1D6 is423 dense bytes, endingRET; no table, EH region or owned
padding. The existing GameStatisticsOwner layout and slot47783C are reused,
not duplicated. Real factory42AF20 allocates1DCB4 and constructs before publication;
ctor42AE60 clears the owner, opens `scoreth10.dat` mode1, initializes the tail,
then seven records using pointer-difference indexing, and calls the loader.
Both initializer bodies142/274 remain explicitly declared dependencies only.
Prior replay-oriented ledger prose was not used to invent a replay owner.

The loader validates magic30314854 and low WORD version3, decrypts payload+18
with AC/35/10, captures input before malloc(uncompressedSize<<2), publishes the
allocation and reloads header sizes for decompression. It scans signed remaining
bytes. CR5243/version0 checks unsigned checksum4374 bytes from+8 and size437C,
then copies by index+0C into records+8; ST5453/version0 checks440 bytes and448,
then copies tail+1D86C. Known bad records advance by their stored size; unknown
magic or negative remaining enters fallback. Nonpositive exhausted payload
returns0. Fallback conditionally frees/clears header, allocates18, zeroes six
words, sets magic/version3/+C100, and returns0; loadedPayload is not cleared.
Allocation/index/length/zero-progress safety checks are deliberately not added.

## Native proof

The first full source is exact on its first comparison:423/all5 and all18 fixed
local branches. Ordinary C++ naturally produces private EBX owner, AL decrypt
seed and the checksum loop unrolling; no register hints, forced ABI, padding,
noinline, copied code, or profile sweep. Candidate profile is VC7.1 `/GL /GS
/EHsc /Gz`, grounded in the existing filesystem graph but not claimed as original
loader production flags.

A separate canonical cold build of `game-statistics-load` reproduces423/all5.
Production integration splits loader/ctor/factory into GameStatisticsLoad.cpp;
unchanged GameStatistics.cpp save is explicit support along with real FileSystem
and normal-COFF Decompress. Complete independent review confirms first and
canonical owner/fields/locals; the diagnostic image remains nonrunnable.
Five complete same-image guards pass **1078 bytes/all60 fields**:
CheckExists111, WriteFile218, OpenWrite137, ReplayOpen137, Decompress475.

Ctor138/132 and factory98/97 remain nonexact. Declared initializer boundaries
cannot reproduce their private receiver ABIs and are not closed source bodies.
Decrypt286 remains nonexact; its real identity is bound, not credited. Save564
is independently nonexact despite equal extent (record/checksum registers and
tail scheduling differ); no save promotion. Its exhaustive field/addend/local
audit was not completed, so Save is not an exact guard. Optional first-image ReplayRead and
CloseWrite selectors used different default-convention decorations and were
reported unavailable, not counted as guards or waived exact failures.

## Integration and tests

GameStatistics.cpp save body and physical owner layout are unchanged. Adding a
real constructor makes two existing host fixtures nontrivial; their test-only
no-op constructor seam explicitly keeps file loading outside the save/teardown
contracts. The save fixture's deliberate byte-pattern fill now uses fill_n;
all byte/guard assertions remain. No no-op constructor enters production.
Original host compile failure from -Wclass-memaccess is retained, not relabeled.
Corrected ASan/UBSan/LSan-enabled save229 scenarios plus44 strict mutations and
teardown70 cases plus50 strict mutations pass. RPC timeout was recovered from
terminal exit0 logs without duplicate tests.

Tracking and progress markdown/SVG pass. All113 target-independent full-CI
stages pass; all1603 frozen inputs (including protected9) are unchanged. Terminal
exit0 was recovered09:49:13 UTC after RPC timeouts, without duplicate execution.
See validation-summary.json. No product, runtime, original physical
TU/profile, initializer-source or semantic-phase closure is claimed.

## Reproduction and retention

Canonical: `scripts/repo-python scripts/build-match-unit.py --unit game-statistics-load`,
then `scripts/repo-python scripts/compare-linked-function.py --unit game-statistics-load --json`.
Run compiler jobs serially on one CPU at nice10. `first.py` preserves the original
isolated first model recipe; `canonical.py` records the independent cold proof
and refuses to overwrite it. First inputs remain under
`.analysis/gpt-dots/statistics-load-20261025/candidate/`; initial source inputs are
bound to starting HEAD and frozen hashes. Current maintained header intentionally
adds constructor/load declarations, so the original freeze is not a claim that
all current source files still equal their pre-admission hashes.

Retained artifact hashes bind the original candidate, producer logs, complete
PE/MAP/PDB images and focused validation. Raw instruction inventories stay ignored;
tracked owner inventory includes only extents/hashes/fields/local destinations.
No private target, toolchain or analysis database is committed.
