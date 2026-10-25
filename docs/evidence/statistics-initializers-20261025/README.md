# Statistics initializer pilot and exact tail, 2026-10-25

Target: original Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Started e5c4129, tracked clean, protected nine files preserved. Target, executable
toolchain, tracking/status and Factory-native Ghidra check passed.

## Scope and result

Tail42ADD0 is142 dense code bytes through42AE5D. Record42ACB0 is274 bytes
through42ADC1. Pair416; the430-byte span includes14 intervening INT3 padding,
not owned code. Prior ledger replay header/payload names were incorrect: these
are TH10 score/statistics defaults. Boundaries/origins/denominator unchanged.

Three isolated complete record models291/258/274 all remain **nonexact**.
The continuous ranking cursor and typed90-byte consumed view recover natural
register/stride structure; moving the +5 zero store after strcpy/+10 clear
recovers source ordering but leaves a different cursor lifetime. No flag,
padding, register/ABI forcing, noinline or spelling panels follow.

Only the tail is admitted: **+142 authored exact/source bytes, +1 owner**.
Totals: exact76347/273658 across418 owners; source625 owners/250034 bytes. A fourth clean tail-only graph leaves record declared,
with no record source or data table definition. Tail142/all7 fields/all3 locals,
loader423/all5/all18 locals, five FileSystem/Decompress guards1078/all60 and all
four real RNG bodies341/all5 pass. A separate canonical cold link reproduces all
of these. Ctor137/132 and factory98/97 remain nonexact; no credit for either.
The prior loader canonical image and every earlier proof remain intact; the new
graph uses a distinct `build/match-linked/GameStatisticsTail` output directory.

## Natural source and field ownership

The tail clears448 bytes, writes ST5453/version0/size448, copies eight spaces
plusNUL at+C, and writes512 real RNG ushort results at+46..445. Last2 bytes stay
zero. Existing RngView and genuine RandomMath.cpp /GL support provide the inline
seed advance; savedSeed stays untouched and generationCount advances512 modulo
2^32. Ordinary C++ naturally yields private EDX; record remains ordinary /Gz
RET4. No copied RNG implementation or fake state is introduced.

Seven tail fields bind string46E354 at addends0/4/8 and g_RngView4918B0 at
addends0/4/0/4. These are semantic bases, not nearest-address guesses. Diagnostic
four-byte anchors overlap: RNG+4 coincides with the next anchor. This does NOT
establish runnable state or physical storage ownership. Nine string bytes are consumed; original physical ownership/extent is unproved,
including subsequent padding.

Record audits retain all8 target/candidate fields and all fixed local targets.
Target has no PE relocation inventory: the audited target fields are explicit,
not inferred as zero from an empty automatic field list. Default string46E858
has nine consumed bytes. Table4743C0 has110 signed consumed bytes, independently
verified against each candidate, and is shared with Fn004088C0. Original table
physical/declaration extent and two following zeros are unproved; no production
data owner is allocated or claimed. Record writes5x10 scores descending1M by100k,
then110 index/kind pairs at624/628 with90 stride. Typed opaque views are proposed
access models, not original source declarations.

## Validation and reproduction

Canonical commands: `scripts/repo-python scripts/build-match-unit.py --unit game-statistics-initialize-tail`,
then `scripts/repo-python scripts/compare-linked-function.py --unit game-statistics-initialize-tail --json`
and the loader unit comparison in the same image. Native jobs are serialized,
one CPU nice10, bounded, with exit logs and frozen inputs; no retries were needed.
Independent source/complete-owner/field/local review passed before admission.
Host `scripts/test-statistics-tail.py` tests3856 seed cases with full448-byte
expected output, surrounding guards, seed/savedSeed/counter including wraparound,
using complete extracted production tail and RNG methods with ASan/UBSan/LSan.
Only x86 pointer-bearing owner assertions are omitted in the host fixture.
This is not original-target runtime, ABI, or native-product validation.

All114 full-CI stages pass; all1619 frozen inputs including protected9 unchanged.
Terminal exit0 was recovered10:18:12 UTC after RPC timeouts without duplicate
execution. Prior loader56 retained artifacts also hash-unchanged. One interim tracking check correctly failed while the new
canonical unit awaited its exact ledger row; retained terminal evidence is not
relabeled as a pass. Original record proposals, images and decoded inventories
stay ignored; tracked inventories contain extents/hashes/fields/locals only.
No private executable, toolchain or database is committed.
