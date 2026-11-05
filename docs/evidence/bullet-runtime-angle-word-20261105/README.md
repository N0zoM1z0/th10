# TH10 runtime bullet angle: overlapping binary32 and raw word

Original: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The original 335-byte `BulletRuntimeView::UpdateAbsoluteDirectionChange`
operates on the stored angle at full Bullet +0x3E4. The float angle is
also forwarded to a vector update as raw IEEE-754 bits. The maintained
`BulletRuntimeView` now models these **same four bytes** as overlapping
`float angle` and `int angleBits`, with `offsetof`/word-size assertions.
This is an overlay on existing storage, not another allocated field.
The absolute-direction writer now assigns `angleBits` from the previously
verified transform `state.value1Bits` and reads `angleBits` to form the
outgoing binary32 argument; it does not introduce a float-to-int numeric
conversion or alter other transform computations.

A fresh pinned VC7.1 SP1 `/GL /GS` **seven-translation-unit** compile
from installed modified source produced the complete 335-byte PDB-owned
absolute-direction function with the same 8 field offsets/types seen in
the prior graph. Against the retail target, exactly two **ordinary**
bytes remain mismatched: +0xD7 and +0xDD (register EAX vs target EDX
when forwarding the angle). The 148-byte `UpdateHorizontalWrap`
producer retains its 8 recorded fields and zero ordinary target
mismatches. These field-geometry/ordinary-byte observations are bounded
native diagnostics, not a newly independently target-bound complete
field-identity acceptance for the 335-byte owner. It remains **NONEXACT**.

Selected **original-target** canonical replays of the installed changed
source passed `bullet-update-horizontal-wrap` 148/148 and
`bullet-update-deceleration` 164/164, **312/312 previously exact bytes**.
No new exact credit. Native source pins and comparison details are in
`native-result.json`; ignored executable, objects and PDB live under
`build/gpt-web/20261105-continuation-bullet/runtime-angle-overlay/`.
No historical full cold replay, complete game build or runtime claim.
