# TH10 full 7,020-byte ECL Run: typed FORMAT numeric operand words

Original is Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The actual ECL Run target starts at `0x0044E1A0` and has a **7,020-byte
complete owner**, not a partial trace.

The FORMAT opcode encodes a 4-byte leading field, a variable-length inline
format string, and a sequence of 8-byte tagged arguments. The maintained
`EclVmInstruction` now has independently offset-asserted overlapping
`integerOperands` and `floatOperands` at +0x10. The previous code
retrieved numerical FORMAT operands by casting the **whole instruction** to
a float/int dword array and indexing `inlineBytes/4 + 6 + 2*argumentIndex`.
The installed source instead reads the true typed operand array using the
exactly equivalent `inlineBytes/4 + 2 + 2*argumentIndex`. This exposes the
actual +0x10 offset and avoids duplicating raw whole-record pointer casts,
while preserving the original inline-string and number-reader lifetimes.

A pinned VC7.1 `/GL` full `src/EclVm.cpp` + `src/Enemy.cpp` link was compiled
*twice*: float operand typed alone, then both float/integer typed. In both
entire-body comparisons, the result remains **7,020 candidate bytes**
with **all 189 original target-bound linkage offsets/types** and exactly
**23 ordinary target-byte differences** at the same offsets as the prior
source epoch. Notably the earlier intuitive `argument->real` rewrite
*failed* this test (6,940 candidate bytes and 188 fields) and was not used.
This is structural source recovery without a new exact claim.

Three targeted accepted canonical replays remain exact:
`ecl-vm-host-run` **146/146**,
`ecl-vm-read-int` **144/144** and
`ecl-vm-read-float` **183/183**, totaling **473/473 previously exact bytes**.
Seven-stage lightweight repository CI and diff checks passed; the full
historical cold replay and runnable product link were not attempted.
Ignored local result:
`build/gpt-web/20261105-continuation-ecl-run/ecl-format-int-and-float-words/`.
