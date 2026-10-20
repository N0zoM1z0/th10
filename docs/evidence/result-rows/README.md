# Result lists and name-entry drawing

The name/grid owner4224C0 and row owner422660 now have maintained source:
**1,466 authored owned bytes**, plus the seven-byte draw callback with origin
still indeterminate. Two independent cold replays close **404 new authored
exact bytes** and seven indeterminate exact bytes. Rows remains nonexact.

## Target and physical ownership

All observations use original Japanese TH10 1.00a, SHA-256
2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.
Local headless Ghidra ran read-only/noanalysis, with a768 MiB heap and one active
processor. Executable and complete mapped-text identity passed; temporary
metadata was rolled back and mapped bytes stayed unchanged. Final review hashes
all412289 bytes through465A80 inclusively, matching
cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f.
An earlier private script hashed one byte less; see the corrected Oracle policy.

Name entry owns404 bytes through two RET16 exits, before twelve CC bytes.
Its API is the owner plus a **by-value12-byte position**, not the decompiler's
provisional scalar/return types. Rows has1035 code/alignment bytes through RET4
at422A68. NOP422A6B, four pointers422A6C and ten selectors422A7C..422A85 extend
physical ownership to1062, correcting27 missing authored denominator bytes.
Ten CC bytes separate update callback422A90. Ghidra's1026-byte row body also
omits nine internal alignment bytes, already covered by the old1035-byte ledger.

Unsigned phase-minus10 guard9, both indexed reads, all selectors, four local
instruction destinations and neighbors establish ownership. Target PE base
relocations are absent. Draw adapter422AA0..422AA6 is installed at42217A,
pushes ECX as the row owner's stack argument and returns its result. Source
authorship versus compiler generation remains unknown. The target auditor,
scripts/report-result-rows.py, checks these observations, complete code, direct
calls, cleanup, selected sampling operands, formats/floats and physical ledgers.

## Recovered behavior

ASCII+8988 is the existing **drawShadow** queue field, not the reference
comment's game-thread busy flag. Rows enables it on the current ASCII owner,
then clears it on the current owner at exit. Replay phases10/17 draw25 slots
at15-pixel pitch, sampling each row's selection. Filled rows retain the replay
header across localtime but resample ASCII afterward; empty rows use the fixed
placeholder format.

Phases11/18 retain the selected slot, interpolate name position before frame10
and settle at224 afterward. The replay header is sampled after name entry;
the final row uses **current game stage**, not the replay completion field.
Phases12/19 draw the heading, optionally name entry and ten18-pixel score rows.
scoreUnavailable suppresses name and selected-row highlight. Timestamp tests
precede localtime; the dated branch then independently resamples profile,
character, shot and difficulty for score/name/signed suffix/stage.

Name drawing captures alphabet length once, draws the name and moves the
underline back one cell at length8. Grid layout has13 columns with18/16-pixel
horizontal/vertical pitch. The last three glyphs are81/7F/80; normal alphabet
bytes retain signed-char promotion. Position is passed by value, colors reset
to white and actual queue capacity remains256. Descriptive names/source
placement do not prove original class/TU/data ownership or construction.

## Native proof and limits

Each independent complete cold compiles FrontEnd.cpp and AnmManager.cpp with
pinned VC7.1 SP1 /GL /GS /EHs, freezing18 source/header inputs. The real draw
callback is entry. Each strict replay passes628 complete bytes/all21 fields:

| Owner | Bytes | Fields | Credit |
| --- | ---: | ---: | --- |
| ResultMenuDrawNameEntry | 404 | 16 | new authored exact |
| ResultMenuDrawRowsCallback | 7 | 1 | new indeterminate exact |
| ASCII AddFormatText | 85 | 4 | existing guard |
| ASCII AddString | 132 | 0 | existing raw-equal guard |

New units share a ResultMenuRows carrier. Guard recipes are translated only
into this diagnostic graph; canonical guard recipes and their credit remain
unchanged. The anchor image is non-runnable. The initial diagnostic incorrectly
used140 as AddString's target size; the corrected132-byte diagnostic and both
strict guard replays supersede that error.

Rows is **1034/1062 owned bytes**, nonexact. Native code ends after1005 bytes;
three alignment bytes, sixteen pointers and ten selectors complete its PDB
owner. Target has1035 code plus27 tail bytes. Incidental decoding of tables
does not establish executable code or a normalized-parent exactness claim.
A typed five-by-ten profile-array control leaves size and structural comparison
unchanged; it is discarded and earlier source restored. No forced noinline/volatile, placement pragmas,
invented register arguments or ABI/arithmetic sweeps are adopted. Further
parent work needs TH10-backed source/context and full code/data field review.

The host suite extracts all three owners, the record helper, actual ASCII
formatter and queue writer. Headers use explicit layout/prefill adaptations and
32-bit timestamps. localtime is a deterministic role whose remapping probes
sampling contracts, not reachable CRT/game callbacks. Actual ASCII helpers
contain no injected callbacks. ASan/UBSan/leak checks and14 strict named mutants
pass. Cases include short alphabets, length8, controls, wrapping, all six active
phases, frame9/10, group5/difficulty4, signed suffix, capacity saturation and
old-date/new-record sampling. Sanitizer or unrelated failures do not qualify.
Native x87/exceptional FP, invalid inputs, storage lifetime, product and runtime
validation remain open.

The MIT reference is
[levzzz5154/th10-decomp](https://github.com/levzzz5154/th10-decomp), pinned at
c6791d82dc9f02e55eca45a5314f900a85481efd in ignored _reference/th10-decomp.
Its pause/result/name recoveries supplied hypotheses; all accepted offsets,
helpers, tables and exactness are independently TH10-bound. The
[MIT attribution](../../licenses/th10-decomp-MIT.txt) is retained.
All87 public CI checks and final target/toolchain/tracking/progress/whitespace
checks pass. Pre-existing D3D evidence remains untouched.
