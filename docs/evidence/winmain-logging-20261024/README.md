# WinMain logging recovery, 2026-10-24

## Measured scope

Checkpoint after **a454afc**: the independently admitted Flush owner
is now **canonically exact, 79 bytes / all 6 fields**, from the fresh cold build
completed at **19:37:59 UTC**. Admission added 79 reviewed authored bytes before
exact credit; canonical replay then added 79 exact bytes. This batch therefore
adds **79 numerator bytes, 79 denominator bytes and one source owner**.
WinMain remains **1405 versus 1389 bytes, nonexact**, with zero parent exact credit.
Full CI independently passed all **112 stages** for this revision at
**19:55:46 UTC**, exit 0, with all frozen inputs preserved. This does not turn
the separate pre-existing automatic-boundary divergence into a passed check.

| Completed factor | Complete WinMain extent | Observation |
| --- | --- | --- |
| Two real startup/restart Log calls | 1327 bytes | Both actual Log edges have the real receiver, original format, one stack argument and caller cleanup. Reset and Flush facades were preserved. |
| Restart buffer reset stores | 1343 bytes | BufferEnd receives the buffer pointer; the first buffer byte becomes zero. Fatal is not reset. Original audit failed; separate supplemental proof passes. |
| Natural GameErrorContext::Flush member | 1405 bytes | Shutdown operation naturally inlines into WinMain; a separate complete 79-byte member is also retained. No forced inline/noinline or ABI spelling. |

The reused ten-TU baseline was 1311 bytes. These are owner extents, **not
mismatching-byte counts or fractions passed**. Full instruction, field, call and
local-target inventories were retained; WinMain still lacks a complete accepted
semantic binding contract, so no normalized equality score is claimed.

Every trial preserves eight complete guards totaling **1679 bytes / all 88
fields**, plus real Log and Fatal totaling **402 bytes / all 23 fields**.
The member trial also preserves real WriteFile **218 bytes / all 20 fields**
and the exact separator, caption and path payloads, including terminators.
These are same-artifact regression checks, not new owner credit or recertification
of different build graphs.

## Failed audit and independent supplemental evidence

The reset trial compiled, but its original post-build audit exited **1** because
it recognized only a literal-zero byte store, not the emitted store from BL.
Original `trial-receipt.json` (`completed: false`), log and exit file remain
unchanged. The separate `supplemental-v2/receipt.json` passes on that same image:
a complete caller CFG and explicit callee/nonvolatile-register checks establish
**BL = 0** at the byte store. It does not rewrite the original failure or prove
unresolved external runtime behavior.

The member's retained **79-byte / all 6-field** body independently compares
full-equal, with all three local destinations preserved. Its distinct target
boundary and authored origin at `0x438950..0x43899E` were reviewed and accepted
**before compilation**, independently of equality. The review examined complete
control flow, separation, references and application-specific logging behavior;
empty Ghidra xrefs alone were not treated as proof. Single-owner admission and
source mapping preceded canonical exact credit. Every prior ledger row was
byte-preserved when the owner was added. Original class/method identity,
physical TU and runtime/shared-global closure remain unproven.

## Canonical acceptance and remaining checks

[Fresh canonical Flush receipt](canonical-flush-replay.json) is a byte-for-byte
copy of `winmain-flush-member-20261024/root-canonical-flush.json`:
2918 bytes, SHA-256
`81f801b6e9df68b6cedcb1ba1106bd7b2051f0c225c5649e64f8c92237bb1304`.
Unit `game-error-flush` binds the complete 79-byte owner and all six fields to
image `3d255c626d212190409285d08d1792df65f591cb8842a41dc42871faea7697ba`.
Frozen inputs were preserved; the same fresh ten-TU image preserves six
canonical guard owners totaling **1436 bytes / all 69 fields**. The old Log
canonical recipe separately cold-passed **201 bytes / all 12 fields**. The old
Fatal recipe also cold-passed **201/all 11**; the original three-TU color
recipe cold-passed wrapper **108/all 11** and helper **135/all 8** with source
hashes unchanged. Receipts are under
`.analysis/gpt-dots/winmain-canonical-peer-regression-20261024/`. Full CI passed
all 112 stages; logs and hash receipts are retained under
`.analysis/gpt-dots/winmain-logging-checkpoint-20261024/`. See the durable
[validation summary](validation-summary.json). Only CI-result documentation
changed afterward and received separate hygiene/link/whitespace checks.

Totals now: **1777 candidates and reviewed boundaries; 735 authored functions /
273658 bytes; 617 authored source owners; 412 authored exact functions / 75038
bytes; 1101 mappings; 1255 overall exact functions / 141969 bytes**.

Fresh Ghidra architecture export passed, but
`report-boundary-inventory.py --check-ledger` **failed** against pre-existing
manual-versus-automatic divergence: **48 states, 106 confidences and evidence
text**, with **zero address or extent changes**. This is not a passed check or
a text-only difference. Preserve the earlier manual evidence; do not overwrite
it with an automatic inventory to conceal the failure. The independent focused
79-byte admission and canonical replay do not resolve this global discrepancy.

## Source and artifact identities

Latest source SHA-256 values, as recorded by
`winmain-flush-member-20261024/source-model.json`:

- `src/Main.cpp`: `1cf12840976adf45f6a0bdc81b0cfa170ad6472efdbdafaf458a98b088f095ff`
- `src/GameErrorContext.hpp`: `30d19e8ce426e832bbd1b7090885c2ced61da9a6fd52f7036f86e55195cd5a37`
- `src/GameErrorContext.cpp`: `844567a9b5580e9554e4a75a9942a9bccc381e90fba3826639df0ac746e21df7`

Measured image SHA-256 values, in factor order:

1. Two Log calls: `f184b21d78ff769d6fddbb74c21c59cab6b5e8ef8b9f399fd31dbb50c4d6f9c2`
2. Reset stores: `80f5b6d3e44b8501663132bb986bea3230239e9c7b0acc92dc31bf5d685c70f0`
3. Flush member: `f353d80194f18963d1ad60740b5329837cc967efb3b4019c2707589a11f9b313`

Ignored evidence below `.analysis/gpt-dots/` is not supplied by a Git clone:

- `winmain-call-audit-20261024/`: `FIRST_FACTOR.md`, `trial-receipt.json`,
  full inventories, source application and frozen-input receipts.
- `winmain-reset-log-20261024/`: original `trial-receipt.json`,
  `trial-root.log`, `trial-root.exit`; separate `supplemental-v2/receipt.json`
  and `supplemental-v2/reaching-bl-proof.json`.
- `winmain-flush-member-20261024/`: `source-model.json`, `trial-receipt.json`,
  `trial-audit.json`, `independent-review/proof.json`,
  `root-boundary-decision-before-build.json`, `root-owner-admission.json`,
  `root-canonical-flush.json`, `root-canonical-six-guards.json` and
  `root-canonical-inputs-preserved.json`.
- `error-flush-boundary-review-20261024/`: `proof.json` is the independent
  target-bound boundary/origin review; `pre-admission-boundary-check.log`,
  `pre-admission-boundary-check.exit` and the report preserve the failed global
  automatic check separately from focused admission.

## Fresh hypothesis and acceptance checklist

The three unchecked source hypotheses below are **pending, not tried**.
Preserve a distinct baseline, one coherent falsifiable model, full-owner gates
and all required callees; do not combine these questions into a spelling panel.

- [x] Measure two real Log edges independently of reset and shutdown behavior.
- [x] Measure the two restart stores; preserve failed and supplemental receipts.
- [x] Measure natural Flush member/caller behavior and review its whole retained body.
- [x] Admit the independently reviewed complete Flush owner, then grant exact
  credit only after fresh canonical cold replay; preserve the separate global
  boundary-check failure described above.
- [ ] Post-Reset device null recheck: test whether a fresh device read/null
  condition restores the target cleanup path without changing its other effects.
- [ ] Instance publication timing: reconcile target store ordering and observable
  call boundaries before proposing a separately measured timing change.
- [ ] Allocation capture/final-null-store: recover the captured allocation result
  and final store paths, then test one target-backed lifetime/side-effect model.
- [ ] Run required regression and full CI on the frozen final checkpoint inputs;
  do not inherit a CI pass from a previous source revision.
