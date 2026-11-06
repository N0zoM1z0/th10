# ECL Run: native signed negate and postdecrement scalar stack words

Oracle: original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The `ECL_VM_NEGATE_INT` and `ECL_VM_POST_DECREMENT_INT` opcodes use
exactly one native **32-bit signed integer VM stack value**. Their original
operations are distinct: NEGATE_INT pops a value, negates it, and pushes the
new value; POST_DECREMENT_INT reads an encoded variable, writes its
**decremented** value through ResolveInt, but pushes the **old** value.
The maintained source now uses the already layout-asserted 4-byte
`EclVmScalar.integer` for both stack payloads, matching the same scalar
wire/storage model used by arithmetic, logical and comparison opcodes.
All helper invocations, argument/tag widths, signed arithmetic, value
publication order and original control-flow branches are preserved. No
new byte buffer, calling convention, bounds behavior or private original
source identifier is invented.

## Full actual native owner + selected exact leaf evidence

Fresh pinned VC7.1 SP1 two-source `/GL` compilation of `EclVm.cpp` and
`Enemy.cpp` retains the **entire 7,020-byte** PDB-owned
`EclVmContext::Run` candidate with all **189 independent original-bound
fields** (including switch destinations) and **6,997/7,020** matching
ordinary original bytes. The same 23 differences remain: Run is
**NONEXACT** with zero new exact credit. The 551-byte `StartSubroutine`
source owner retains **four unchanged** native call field sites at
`0x9E/0xD2/0xE8/0x1DE` and identical normalized candidate bytes to the
prior source build. Its original is 550 bytes and it is also NONEXACT.

Only the directly involved existing exact functions were cold replayed:
`ecl-vm-read-int` **144/144**, `ecl-vm-stack-push` **96/96**, and
`ecl-vm-stack-pop` **122/122**, total **362/362** original-bound bytes.
Seven lightweight CI stages and whitespace pass; no historical all-unit
cold replay. Neither whole-game product, runtime equivalence, private ABI
identity nor portability is claimed.

Private repro and native receipts:
`.analysis/gpt-web/20261106-ecl-negation-increment/verify.py`,
`build/gpt-web/20261106-ecl-negation-increment/current/native-result.json`,
`.analysis/gpt-web/20261106-ecl-negation-increment/selected.json`.

Installed `src/EclVm.cpp` SHA-256: `63eb562811821baf48bb2760428a624018a8d33d54b412367e01ddace077b274`.
