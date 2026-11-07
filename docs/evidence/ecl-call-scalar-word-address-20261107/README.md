# ECL StartSubroutine: native scalar payload words after counted names

Original target: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Original `EclVmContext::StartSubroutine @0x0044DF70` owns **550 bytes**;
the current pinned VC7.1 SP1 `/GL` two-TU compiler produces a **551-byte
NONEXACT** candidate with four candidate call fields. Its large 7,020-byte
`EclVmContext::Run` caller remains **6,997/7,020** independently
original-bound bytes with **189 original linkage fields**.

## Maintained natural source recovery

A CALL instruction contains a variable-length counted subroutine name
followed by tagged eight-byte argument descriptors. The old float and
integer branches both used
`(*callerInstruction)->scalarOperands[valueOffset / sizeof(int)].integer`.
But `scalarOperands[1]` is only the starting word overlay of the serialized
instruction, **not** an allocated C++ array of runtime-indexed elements.
An argument after the first one can therefore cause undefined C++ array
pointer arithmetic even when the original x86 simply forms one 32-bit
byte address. Both branches now reuse the already-defined `OperandWord`
helper with the same unsigned `valueOffset / sizeof(int)` index. The
helper computes `instruction + offsetof(scalarOperands) + 4*index` with
native modulo-32-bit address words.

This retains the real encoded operand count, saved frame and stack slots,
source/destination type tags, arithmetic, evaluation order and calling
conventions. No new range checks, allocation, fake ABI or target bytes are
introduced. Behavior outside actual original input bounds remains
unknown; the casts intentionally model the 32-bit Win32 target.

## Five compiled native experiments and installed-source guards

The current two-TU graph was compiled and linked for five separate source
forms. Typed `OperandWord` for both branches, direct unsigned word-address
calculation, byte-addressed `operands` cursor, and only the float branch
all left the complete **551-byte** StartSubroutine PDB contribution and
all four candidate field positions **identical** to the current baseline.
The raw `int*`/masked-byte-address variation also retained size and field
positions but introduced **76 ordinary-byte differences** within the
551-byte candidate, so it was rejected. The maintained source selects the
simpler typed shared-helper form and was then compiled from disk again.

The **full original-target** 7,020-byte `Run` remained **6,997/7,020**
with all **189 original fields** and the exact 171-byte float-value callee
in every successful variant and in the installed source. Nine selected
already-accepted ECL units were cold replayed from the installed source:
`ecl-vm-host-run`, `ecl-vm-read-float`, `ecl-vm-read-int`,
`ecl-vm-read-float-value`, `ecl-vm-read-int-value`, `ecl-vm-stack-pop`,
`ecl-vm-resolve-float`, `ecl-vm-find-thread`, `ecl-vm-spawn-thread`.
All **1,161/1,161** independently original-bound bytes passed.

This is a large-parent-preserving source-semantic change, **not** a newly
exact StartSubroutine or Run. The four StartSubroutine fields reported here
are **candidate geometry**, not independently original-bound acceptance.
No all-unit cold replay, whole-game product build or runtime test ran.

Ignored private native reproduction:
`.analysis/gpt-web/20261107-turn2/ecl-call-payload-trials.py`,
`ecl-call-payload-results.json`, `ecl-call-installed-guards.json`,
`ecl-call-installed-run.json`, and compiled trial PE/MAP/PDB below
`build/gpt-web/20261107-turn2-ecl-call/`.
