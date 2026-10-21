# Chain calculation and drawing dispatchers

Reviewed2026-10-22 from `e558d61`. Maintained source now implements the two frame
scheduler calls already declared in Main. Both complete owners remain nonexact.
Two recovered switch tables also correct the old RET-only boundaries. This adds
**two source mappings and zero exact functions/bytes**. [Evidence](evidence.json)
contains target observations, native contexts and both compiler source controls;
[validation](validation-summary.json) records replay, host and final CI scope.

## Target ownership and ABI

Only original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`, is used.
Read-only/noanalysis Ghidra sessions use rollback, one CPU and768 MiB heap.
The full412289 mapped bytes401000..465A80 inclusive retain SHA-256
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`
before and after review. Maintained ExportArchitecture also refreshes body ranges.
Ghidra names, signatures and bodies remain provisional; raw target instructions
independently establish the following claims.

| Owner | Through RET4 | Owned alignment | Local table | Complete owned extent |
| --- | ---: | ---: | --- | --- |
|Calc449C00|273 bytes|3 bytes|449D14,8 words|308 bytes through449D33|
|Draw449D40|241 bytes|3 bytes|449E34,6 words|268 bytes through449E4B|

Calc's273 bytes include its eight-byte internal alignment region omitted from
Ghidra's sparse body. Each post-RET alignment is the three-byte LEA padding
before the table. Unsigned guards accept indices0..7 and0..5 respectively;
every table destination is inside that owner's complete code. Each table has
only its own indexed-jump Ghidra reference, and CC padding follows the last
word before the next owner. The target has **no PE base-relocation directory**;
these are reviewed absolute local destinations, not a target-relocation claim.
The initial relocation-directory diagnostic failed before producing evidence;
the corrected review uses guards, raw words, xrefs and neighboring boundaries.

Both entries receive one owner pointer on the stack, end in RET4 and explicitly
return their processed count in EAX. Main pushes the owner before43942F/4394D8;
its first result controls exit/restart, while it ignores the drawing result.
The old unannotated declarations, including drawing's void return, are corrected
to `int __stdcall`. This expresses the observed physical interface, without
claiming the original source annotation, free-function/method spelling or TU.
No duplicate global definition is introduced.

The boundary correction adds **62 owned bytes** (56 table bytes and6 alignment
bytes), with no exact credit. Authored denominator becomes734/273579, authored
source610/247329, nonexact source213 and source-absent124. Total mappings1094;
exact authored397/72387 and overall1238/138897 remain unchanged. Unknown origins
remain524. These are provisional owned-extent metrics, not whole-game coverage.

The fresh before/after automatic boundary audit retains zero geometry drift.
Unowned indexed-control tables decrease18->16. Global manual/automatic metadata
drift remains open:1114->1116 rows, unchanged49 state and107 confidence differences;
its ordinary full check still fails first at401100 provenance. Two new manual
ownership notes do not waive or rewrite that separate global audit.

## Target-observed dispatch protocol

Both begin at the first next link, owner+18h/+3Ch, and cache the next node before
invoking a callback. Null callbacks are skipped without counting. A nonnull
callback with enable **mask0x2 (bit1)** clear is skipped but counted. Before an
enabled callback, the dispatcher leaves the shared critical section and
decrements its nesting byte; the callback receives its argument in ECX. It then
reenters and increments the byte before interpreting the result.

| Callback result | Calculation | Drawing |
| --- | --- | --- |
|0|Unregister only; count and advance cached next|Same|
|1 / other, including negative values|Count and advance|Same|
|2|Retry same element, rechecking enable mask|Same|
|3|Replace count with1 and break|Same|
|4|Replace count with0 and break|Same|
|5|Replace count with-1 and break|Same|
|6|Restart at first element and reset count|Default count/advance|
|7|Invoke optional delete callback while locked; no implicit unregister|Default count/advance|

The delete callback may itself perform recursive cleanup. The dispatcher does
not read its freed element afterward. This does not make arbitrary mutations or
freed cached-next pointers safe. Invalid owner/node/callback domains receive no
invented success guards. TH09's runner suggests a control-flow hypothesis, but
its0x20-byte layout, callback ABI and missing TH10-specific cases are not TH10
proof. Literal result values avoid importing its semantic enum names.

## Native compiler observations

One actual-source Main/Chain/GameErrorContext full three-TU `/GL /GS` cold emits
Calc428 and Draw272. Calc includes an inlined unregistration path. The existing
canonical14-TU `/GL /GS /EHs` caller graph emits Calc316 and Draw272, with a real
unregister call. Both visible sources and flags differ between these contexts;
the reason for the changed inlining is not isolated. Neither identifies the
original partition or production profile. Complete PDB contributions remain
larger than target308/268, so no exactness or masked ordinary-byte score is claimed.

Two private controls each recompile one Chain object and reuse13 verified game
objects; they are **not full cold builds**. Postfix instead of prefix counter
updates leaves both complete raw bodies unchanged. Moving case4 before case3
changes four raw byte positions per body: two return constants and two linked
table destinations. Sizes and aligned raw totals stay unchanged; the second
control is not raw-neutral. Neither is adopted, and source spelling remains
unknown. Raw-body fingerprints were recovered from retained PE bytes after an
image-hash/body-hash metadata labeling error, without a compiler rerun.

Two full canonical14-TU cold groups preserve **four existing exact units/282
bytes**: Unregister114, RemoveAll82, RegisterCalc43 and RegisterDraw43, with every
declared field replayed. This is affected-unit validation, not all-consumer or
whole-product closure. The first diagnostic carrier was created under the prior
campaign's build prefix by its template; its actual path and inputs are recorded,
and no existing path was overwritten. Later producer output roots are corrected.

The existing ten-TU [large-owner pins](../chain-source-closure/native-summary.json)
remain current: all32 inputs and15 artifacts are freshly hash-checked unchanged.
Chain.hpp is unchanged, and the two changed game CPPs are outside that graph.
No new large cold,35-guard replay or Run measurement is claimed. The prior ANM,
ECL and Enemy measurements therefore retain their original validated scope.

## Actual-source host validation

The transition fixture now invokes maintained Calc directly in lifetime0/7,
replacing its former0/1/7 subset model. Its obsolete subset-status assertion is
superseded by two actual processed-count assertions; existing effect, constructor,
allocation and cleanup checks remain. A new scenario executes42 dispatcher calls
with literal statuses0..7,8,-1,INT_MIN/INT_MAX; null/disabled counting; retry-mask
recheck; restart; forced results after prior processed nodes; drawing defaults;
locked deletion/unlocked callbacks; and cached-next insertion across passes.

The complete sanitized baseline passes **1375 assertions**. Exactly one of seven
active strict LF dependency pins, Chain.cpp, changes; the other six and the
CRLF-to-LF-only policy remain unchanged. All19 EOL guards pass. Eight generated
faulty-code copies each compile and fail only its predeclared assertion/exit86;
compile errors, signals, wrong assertions, noise and survivors fail the control
run. The reusable runner is `scripts/test-chain-dispatcher-controls.py`, included
as one new CI group. It does not mutate canonical inputs or relax their pins.

Native layout/private ABI, Windows lock implementation/concurrency, renderer,
assets and whole-game runtime remain outside these host observations. The known
kind8 floating-point gap and constructor/lifetime limitations remain visible.
The491BE4 source storage alias, remaining ownership/release inventory, data owners
and faithful Windows product closure remain open. Semantic reconstruction and
porting have not started. Private recovery:
`.analysis/gpt-6.1-sol/chain-dispatchers-20261022/RECOVERY.md`.

Final validation passes108 complete CI groups with all368 maintained source/test/
config raw inputs frozen throughout. Classified cleanup removes15845902 logical
file bytes from nine owned reproducible trees and one superseded dump. Current
canonical carriers, the three-TU comparison, one minimal failed native control
and compact evidence remain. Nine independent D3D files and provider/legacy state
are preserved.
