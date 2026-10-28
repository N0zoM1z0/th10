# Result-transition source notes

The former host contract suite, synthetic dependency roles, sanitizer checks
and mutation requirements were removed on 2026-10-28. Historical source and
reports are recoverable at `e1c4075`. Current acceptance uses complete native
byte comparisons under [the Oracle policy](ORACLES.md).

The source covers `GameCompleteStageTransition`, `GameQueueScaledSoundCommand`
and the braced `GUI_MSG_COMPLETE_STAGE` region in `GuiMessageVmView::Run`.
The transition retains its direct queue calls. Complete-owner bytes and actual
callee bindings remain the evidence for exactness. Target `423370` uses receiver
EDI and plain RET; the natural `ResultMenuView*` interface alone does not prove
that ABI. Numerical width/order/conversion and storage bindings at `420C30`
remain subject to native comparison.

## Floating-point limit

Magnitude bits `0x4efdffff` and speed bits `0x3f7fffff` produce an exact quotient
`2130706431 - 1/16777215`; binary64 can round across that integer while extended
precision truncation differs. Host samples therefore do not establish general
x87 equivalence. NaN, infinity, overflowing conversions, native x87 state and
the precise runtime behavior of `0x463B2C` remain unresolved.
