# TH10 ECL CALL argument descriptors: two typed tags and scalar stride

Pinned original Japanese TH10 v1.00a executable SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The CALL `EclVmContext::StartSubroutine` helper consumes a variable-length
sequence of **8-byte encoded argument descriptors**, separately distinct
from FORMAT's own scalar records. The native reads identify source scalar
type at record +0, requested destination type at record +1, and the 32-bit
value at +4; two intervening bytes remain unclassified. This is backed by
the complete target instruction decoder and existing maintained operand
read/use order, not an invented original C++ class name.

`EclVmCallArgumentView` now has offset/size compile-time assertions for these
four facts. The actual StartSubroutine loop advances both its signed metadata
byte cursor and unsigned scalar byte cursor using the checked record size,
with an explicit `int` conversion to preserve the original metadata signed
arithmetic. Source/destination tag reads use the *typed* fields, but each
reconstitutes the pointer at the point of access. This preserves the real
intervening `ReadIntValue`/`ReadFloatValue` calls, stack handling and the
native pointer register lifetime. It does **not** make the CALL argument
packet a fixed array, allocate new state, mask any bytes or change bounds.

## Rejected pointer-lifetime alternative

A genuine VC7.1 compile of one `const EclVmCallArgumentView *descriptor`
local carried across both argument-service calls kept the 551-byte
StartSubroutine owner but moved its four linked call fields from offsets
`158, 210, 232, 478` to `157, 197, 217, 474`. That candidate was rejected,
and the source restored to per-access typed pointer expressions, which keep
all four fields at their previous positions. Rejected codegen was not
misrepresented as a source-exact match.

## Full original-bound and companion evidence

An actual pinned VC7.1 SP1 `/GL` two-source ECL VM/Enemy graph recompiled
from the installed changed source confirms these *entire* PDB owners:
`StartSubroutine` **551 candidate bytes** (original **550**), four identical
linked fields and zero ordinary candidate-to-candidate differences;
`EclVmContext::Run` **7,020 candidate bytes**, 189 identical candidate
field positions/types, zero ordinary candidate differences; and
`FindSubroutine` **136 bytes** unchanged. The full interpreter additionally
passes its independent **189-field original-target contract** (including
local targets) at **6,997/7,020**, still with 23 ordinary differing target
bytes. Neither large interpreter nor 550-byte original StartSubroutine is
newly exact.

Selected original-bound cold replay on the final installed source passes
`ecl-vm-host-run` 146, `ecl-vm-read-int` 144,
`ecl-vm-find-subroutine` 136, `ecl-vm-stack-push` 96 and
`ecl-vm-stack-pop` 122 — **644/644 previously accepted bytes**. Zero new
exact credit. Full candidate comparisons, source pins and target mismatch
inventory are in `native-result.json`; ignored PE/MAP/PDB under
`build/gpt-web/20261105-ecl-call-argument/per-access-tags/`.

No historical full cold replay, product closure or runtime-equivalence proof.
