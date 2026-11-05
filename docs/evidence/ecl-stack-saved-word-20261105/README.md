# TH10 ECL frame-link and preserved-word stack view

Verified original Japanese TH10 v1.00a executable SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The native ECL value stack contains two distinct four-byte representations.
Typed scalars have a 4-byte type slot (only first byte initialized), while
ENTER/LEAVE frame linkage stores a **complete signed 32-bit previous frame
base**, without a type tag. StartSubroutine also restores one saved 32-bit
word just below its previous stack top. The maintained source now shares a
small `EclVmStackSavedWordView` for those **whole-word accesses**, with
`sizeof==4` and `offsetof(value)==0` compile-time assertions. No extra
padding, default writes, range checks, or new stack allocation are added.

`EclVmStackView::EnterFrame` writes the previous frame base using this typed
view and advances the stack by its checked size; `LeaveFrame` retreats by
the same signed size and reloads that word. `EclVmContext::StartSubroutine`
uses the same 4-byte representation for its preserved word. All size
arithmetic retains explicit `int` casts; C++ `sizeof`'s unsigned type must
not silently change the original signed cursor lifetime.

## Complete native source-graph and exact-unit verification

A fresh pinned MSVC 7.1 SP1 `/GL` **two-source** EclVm/Enemy build of the
installed code compares whole PDB function contributions against a source-
pinned previous graph. All candidate linkage positions/types and ordinary
code bytes remain identical in: `EnterFrame` **64 bytes, 0 fields**;
`LeaveFrame` **41 bytes, 0 fields**; `StartSubroutine` **551 bytes, 4
fields**; and ECL `Run` **7,020 bytes, 189 fields**.

The **independent original-target** full `Run` contract also replays all
189 original-bound linkages, local destinations and selector/data bytes,
with **6,997/7,020 ordinary bytes matched** and the same 23 outstanding
differences. `StartSubroutine` is **551 candidate vs 550 original** and
remains NONEXACT; no cropped credit is inferred.

Six selected already accepted original-bound functions passed their cold
replays on this installed source: `ecl-vm-stack-enter-frame` 64,
`ecl-vm-stack-leave-frame` 41, `ecl-vm-stack-push` 96,
`ecl-vm-stack-pop` 122, `ecl-vm-read-int` 144 and `ecl-vm-host-run` 146:
**613/613 exact bytes preserved**, **zero new exact credit**.

All source/header pins, original-target comparison and selected results are
in `native-result.json`. Private native PE/MAP/PDB outputs are under
`build/gpt-web/20261105-ecl-stack-saved-frame/with-call/`. No historical
all-unit cold replay, executable product link, or runtime equivalence claim.
