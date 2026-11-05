# TH10 ECL 4-byte stack type slots across the 7,020-byte VM interpreter

Target is the pinned Japanese TH10 v1.00a original, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The independent **96-byte** `EclVmStackView::Push` and **122-byte**
`EclVmStackView::Pop` functions prove a four-byte **type slot** before a
typed scalar stack payload. When the tag is nonzero, Push writes its type
byte at slot +0, skips **four bytes**, then copies the scalar. Pop reads
the type from slot +0 after retreating by four bytes, interpreting the
retrieved `i` or `f` scalar accordingly. The three remaining bytes of that
slot are not initialized; no invented default value or padding write was
introduced.

A native `EclVmStackTypeSlotView` now owns this four-byte wire layout.
Compile-time `sizeof` and `offsetof` assertions fix tag +0 and size 4.
Push and Pop access the actual type field and advance/retreat by the
**signed** `int` conversion of that type's size, preserving original
stack-top arithmetic. The host/VM ABI and all existing dynamic scalar
sizes remain unchanged; this is an in-memory serialized slot view rather
than a new allocated stack or historical source identifier.

## Independent complete-owner verification

A genuine pinned MSVC 7.1 SP1 selected cold replay of **four entire
original-bound functions** on the installed modified source passed:

- `ecl-vm-stack-push`: 96/96 exact
- `ecl-vm-stack-pop`: 122/122 exact
- `ecl-vm-read-int`: 144/144 exact
- `ecl-vm-host-run`: 146/146 exact

All **508/508** bytes were previously accepted; new exact credit is zero.
A separate actual **two-source** VC7.1 `/GL` ECL VM/Enemy link freshly
recompiled the complete modified 7,020-byte `EclVmContext::Run` owner,
including all its 189 independently original-target-bound address fields
and local/control-table destinations. The full strict original comparison
remains **6,997/7,020**, with exactly the **same 23 ordinary differing
bytes** as the previous complete compiler graph, and remains NONEXACT.
`EclVmScriptDatabase::FindSubroutine` remains a 136-byte candidate,
while `AddScriptData` remains a 570-byte candidate against 589 target.

The machine-readable source/header hashes, full comparison and exact replay
receipts are in `native-result.json`; ignored native PE/MAP/PDB are under
`build/gpt-web/20261105-ecl-stack-tag/full-run/`.
No full historical cold replay, product linkage or runtime guarantee.
