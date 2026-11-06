# ECL script name table: preserve original 32-bit count arithmetic

Target: TH10 Japanese 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`EclVmScriptDatabase::AddScriptData @0x00450220` owns the complete
589-byte SCPT file registration and sorted name merge. The observed
original uses native 32-bit `ADD` and `SUB` to grow the name count and
recover the count of retained records. The former natural C++ `int`
expressions had signed-overflow undefined behavior, while the original
x86 low-word arithmetic always wraps. The inline count-reference helper
now adds the `unsigned int` word representations before publishing the
same bit pattern to the signed counter; the retained-record count likewise
uses an unsigned 32-bit subtraction before its signed loop/memcpy use.
This does *not* change an invalid file's other unguarded bounds behavior,
add a file-count check, or widen the actual 32-bit layout.

## Fresh complete-function native probes

Three temporary source forms for the count helper (`unsigned` sum,
unsigned word alias, explicit sum local) and a combined count/subtraction
control all compiled with pinned VC7.1 SP1 `/GL` using the original
`EnemyRuntimeUpdate` entry and actual two translation units
(`EclVm.cpp`, `Enemy.cpp`). All four preserved the full 589-byte
contribution with the original two call relocation positions and the
previous **510/589** ordinary bytes. The installed final code was then
**cold-recompiled from both translation units** and verified separately:

- `AddScriptData`: **589 candidate / 589 target**, **510 matching bytes**,
  both independently original-bound `malloc`/`free` REL32 fields unchanged.
- `EclVmContext::Run`: **7,020 / 7,020**, **6,997 matching bytes**, **189
  independently original-bound local/external/table fields** unchanged;
  all 88 selector bytes remain covered.
- Same-image `EclVmContext::ReadFloatValue`: **171/171** original-bound exact.

Selected independent cold replay, without rebuilding the entire cohort:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-find-subroutine \
  --unit ecl-vm-read-int-value \
  --unit ecl-vm-read-float-value --json
```

Result: **exact**, **431/431 original bytes** in one selected compiler
artifact. The three accepted units already existed; **zero new exact
credit** is claimed. The large Run and AddScriptData owners remain
**NONEXACT**. Their current-source trial programs, PE/MAP/PDB, strict
comparison and selected-cold receipts remain ignored under
`.analysis/gpt-web/20261106-chat-th10-count-more/` and
`build/gpt-web/20261106-ecl-count-reference/gptweb-count-words-current/`.
Source SHA-256: `3b4af08fec32d38a1323575365bdbbb9e211afef25adfc1d0ad6de881ebc8c7b`.

This is a source-level native semantics and complete-codegen preservation
checkpoint, not a product/runtime or original-physical-TU claim.
