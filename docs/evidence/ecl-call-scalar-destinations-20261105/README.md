# TH10 ECL CALL: typed scalar destinations without changing helper codegen

The original is Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The `EclVmContext::StartSubroutine` CALL loop writes source `f`/`g` and
integer operands to the callee's byte-addressed stack. Those writes were
previously expressed as four separate casts to `int*` and `float*` of the
same `argument` stack slot. The ECL reader and lvalue resolver already
modelled the exact 32-bit word through the `EclVmScalar` union, including
binary32 integer-versus-float bit interpretation. The four CALL destination
assignments now use the same typed scalar union at the actual address.
Conversion, argument order, source/destination tag decisions, byte strides,
stack lifetimes, and original error branches are unchanged. The original
source spelling is not asserted.

## Complete native comparison, not new exact credit

The pinned VC7.1 SP1 `/GL` two-source `EclVm.cpp` + `Enemy.cpp` native link
still emits the complete **551-byte** `StartSubroutine` PDB contribution
(original target **550** bytes). All **four** link fields remain at the
same offsets **0x9E, 0xD2, 0xE8, 0x1DE** and the remaining normalized
candidate code is **551/551 byte-identical** to the preceding current-source
candidate. This is deliberately checked independently of `Run`, because a
previous descriptor-pointer caching trial preserved `Run` while moving all
four `StartSubroutine` call sites.

The complete **7,020-byte** `EclVmContext::Run` also preserves its **189**
independently original-bound fields and **6,997/7,020** matching ordinary
bytes. All **23** unresolved original differences remain, so `Run` and
`StartSubroutine` are both still **NONEXACT**, and neither gains any match
ledger credit. The affected `ReadIntValue` and `ReadFloatValue` exact leaf
producer was cold-replayed **only** with those two explicit selectors:
**124 + 171 = 295/295** target bytes exact. Seven lightweight CI gates and
`git diff --check` pass.

Reproduction: ignored `.analysis/gpt-web/20261105-ecl-call-stack-slot/verify.py`,
`build/gpt-web/20261105-ecl-call-stack-slot/current/native-result.json`,
and `.analysis/gpt-web/20261105-ecl-call-stack-slot/selected-exact.json`.
This is scoped source/codegen evidence, not full product or runtime closure.
