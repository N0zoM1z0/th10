# TH10 WinMM button merge: recover original x86 low-five-bit shift count

Target: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The retained independent native helper `0x0044A160..0x0044A18A` owns
**43 bytes**; its nonnegative-button path explicitly emits `SHL EAX,CL`
at `+0x0A` (`D3 E0`), after loading the fixed one-bit mask into EAX.
On 32-bit x86 this instruction takes only the **low five bits** of CL as
its count. Negative signed-short mappings return zero with plain `RET 4`,
while positive mappings test the input WinMM 32-bit bitfield, OR the chosen
action into the caller's 16-bit output, reread the masked result and return
the zero-extended action or zero. The caller's original ABI transports the
binding in CX, output pointer in EDX, and action in SI; these are target
observations, not source calling-convention annotations.

The previous natural C++ helper `MergeWinMmButton` in `src/InputPolling.cpp`
used `1u << button`. For nonnegative mappings 32 or above, that C++
expression has undefined behavior. Although the pinned original x86
instruction deterministically masks the count, the maintained C++ source
had never expressed this *observed domain*. The implementation now reads
`1u << (static_cast<unsigned int>(button) & 31u)` after the existing
negative check, thus preserving **every valid 0..31 mapping**, the original
negative-disabled behavior, and original low-five-bit semantics for
nonnegative 32..32767. No added index guards, default button remapping or
new device data source are introduced. This is a source-level semantic
recovery, not a claim about authored C++ syntax or original TU provenance.

## Original-code and compiler boundary

The isolated real VC7.1 SP1 `/GL /GS` `PollInputFrame` graph was compiled
with this mask and checked against the original. The selected already exact
`FrontEndGetControllerState` at `0x0044A4E0` remains **264/264 canonical
exact**, with **12/12 real target-bound fields**. A separate fresh native
source experiment tested both `unsigned int(button)&31` and a short-cast
local; both compiled the unchanged 264-byte exact query owner.

However the **nonexact** `MergeJoystickInput` candidate now emits **897**
bytes versus the original **833**, whereas the earlier source had a
shorter 865-byte candidate. The frame polling candidate is **855** versus
original **850**. This is an explicit **codegen size regression** in those
unaccepted owners, even though the new source removes undefined behavior
for persisted out-of-range WinMM mappings. The independent 43-byte helper
has no proven native entry xrefs, and its source origin remains indeterminate.
**No newly exact unit, byte percentage or original production-owner claim**
results from this change. A future exactness-oriented implementation must
recover both the x86 shift semantics and the original private WPO codegen.

This change deliberately retains fidelity to original x86 behavior rather
than silently treating invalid positive mappings as zero or inventing a
32-bit bounds guard. The only scoped accepted-owner replay is:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit input-controller-buttons --json
scripts/repo-python scripts/ci.py
```

Private receipts:
`.analysis/gpt-web/20261104-next/input-winmm-shift-mask-results.json` and
`input-winmm-shift-selected-cold.json`. The source SHA at this selected
cold was `d74c1092a8e38903059f530a9c5a6e813eac98d646addab6005d5a241cfafaf1`.
No whole-image build, runtime device test or portability acceptance follows.
