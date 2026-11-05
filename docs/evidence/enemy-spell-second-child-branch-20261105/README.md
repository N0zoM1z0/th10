# TH10 2,432-byte EnemyBeginSpell: original-sized complete linked function

Original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Target `EnemyBeginSpell @ 0x00409280` owns **2,432 bytes**, including
2,403 code bytes, a 1-byte NOP and a 28-byte switch table.

Reverse-reading the **complete** original and candidate instruction
streams isolated a genuine control-flow/source-lifetime mismatch in both
sub-VM searches for scripts `0x19F` and `0x1A0`. The second lookup
previously cached `AnmVmView *child` inside the loop, leading VC7.1 to
emit a short early branch and move the seven-stage switch table up by
8 bytes. The retained source **keeps the linked-list node across the
lookup and rereads its `owner` on the matching path**, preserving the
original null lookup behavior before the same unconditional `intVar2`
store. This change is not padding, a compiler pragma, or an invented ABI.

A real pinned **eleven-source VC7.1 /GL graph** with ten source-hash-
checked reusable unchanged support objects now generates the complete
**2,432-byte** native `EnemyBeginSpell` PDB contribution (previous
candidate **2,424 bytes**). Its 108 linked address fields include the
seven code-owned switch table words at **+0x964..+0x97F**. Independently
reading all seven original and candidate table dwords relative to their
owner bases gives the **same seven local destinations**:
`+0x6E4, +0x74B, +0x796, +0x7E1, +0x82C, +0x877, +0x8BF`.

A **nonacceptance diagnostic** that zeros the 108 *candidate-identified*
linkage fields in both candidate and target windows reports **1,835/2,432
ordinary matching bytes** versus **1,235/2,424** for the prior first-
loop-only candidate. This does **not** establish target-bound external
field obligations or full exactness. The remaining differences include
compiler register lifetimes and evaluation order; no new exact credit.

Scoped canonical independent replays of the same TU's already-exact
`enemy-ecl-resource-load-package` (239 bytes),
`enemy-configure-interrupt` (48 bytes), and
`enemy-cancel-bullet-record` (199 bytes) remain exact: **486/486**.
Seven-stage repository CI and whitespace checks passed; no historical
all-unit replay or product/runtime claim. Source/checksum and exact
selected receipts in `native-result.json`; ignored local native link in
`build/gpt-web/20261105-boss-second/second-node-owner/`.
