# ECL Run: byte-offset JUMP records and counted SPAWN_THREAD_WITH_ID prefix

Verified target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The ordinary, true-conditional and false-conditional ECL JUMP opcodes share
one interpreter dispatch tail. On a successful jump, operand word 0 is a
signed **byte displacement** relative to the current serialized instruction,
and word 1 supplies the target time converted to binary32. The actual VM
source formerly addressed both only through unnamed generic operand
indexes. The new `EclVmJumpOperandsView` overlays the existing instruction
payload, with `instructionByteOffset` at payload +0 and `targetTime` at
payload +4. The common JUMP label now uses these named encoded words.

`SPAWN_THREAD_WITH_ID` independently interprets its first operand as an
inline byte count and locates a later integer argument by exactly
`(inlineBytes + 4) >> 2`; it does *not* assume the byte stream is a fixed
C++ structure or that the variable part is naturally aligned. The new
`EclVmSpawnThreadInlinePrefixView` binds count +0 and inline bytes +4 in
the same stream. Its encoded count accesses the native alias; the dynamic
shift and `ReadIntValue` call order are unchanged. Both complete-instruction
origins are asserted at +0x10 (their second fields at full +0x14), with no
new allocated fields, guessed names for opaque inline data, or fabricated ABI.

## Scoped real native original-bound evidence

One freshly compiled pinned VC7.1 SP1 **two-source** `/GL` graph
(`EclVm.cpp` and `Enemy.cpp`) retains the complete `EclVmContext::Run`
**7,020-byte** PDB contribution. The original-target strict comparator
validates **all 189** actual binding fields including original local jump
and switch destinations, and matches **6,997/7,020** ordinary bytes; the
same 23 remaining differences still make Run **NONEXACT**, with no new exact
ledger entry. `AddScriptData` remains a separate **585-byte** nonexact
candidate (target 589). The full 551-byte `StartSubroutine` candidate retains
exactly four unchanged call fields at `0x9E/0xD2/0xE8/0x1DE` and **0**
ordinary regressions from the prior current source graph; its target owner
is 550 bytes and still NONEXACT.

Selected canonical target-bound original replays of directly related
`ecl-vm-read-int` **144/144** and `ecl-vm-read-int-value` **124/124**
recompiled **268/268 exact**. Default seven lightweight CI checks and
`git diff --check` pass. No full historical cohort was replayed.
This does not establish whole-game build/runtime/portability or original
compiler-private ABI/source names.

Ignored reproducibility witnesses:
`.analysis/gpt-web/20261105-ecl-jump-thread/verify.py`,
`build/gpt-web/20261105-ecl-jump-thread/current/native-result.json`,
`.analysis/gpt-web/20261105-ecl-jump-thread/selected.json`.

Current source SHA-256: `EclVm.cpp` `d3828bf3b13739b89af895601237c4ae262d038195c7a392d2bf746476d978a5`, `EclVm.hpp` `96ed76bb161759b21915f038b958160f4e2ca3372c6bab8e136c9cf0d539a2f8`.
