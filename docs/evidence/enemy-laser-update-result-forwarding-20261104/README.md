# TH10 laser update chain: real callback return-value forwarding

Original Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The registered update callback at `0x0041C480..0x0041C4DA` is a complete,
CC-delimited **91-byte** target. Its real manager resides in ECX on entry;
`EnemyLaserManagerStorageView::Initialize` registers the function at
`0x0041C157`. The actual `0x0041C330` linked-list updater is a separate
**274-byte** owner using private EDI for the manager. The original
class, source name and compiled source TU remain unestablished.

Direct complete native disassembly reveals a source behavior distinction:
when GameManager's real `+0x58` pause/disable flags block processing, the
callback returns constant 1. Otherwise it invokes the actual list updater.
In the speed-pause lane it saves `g_AnmGameSpeed`, zeroes that global,
invokes the updater, restores the saved speed and **returns the callee's
EAX**. In the normal-speed lane it likewise calls the updater and immediately
pops saved registers and returns the callee's EAX. The target does **not**
insert a `mov eax,1` after either update call, although the maintained
`UpdateLinkedLasers` source currently returns 1 on its own paths.

The previous `EnemyLaserUpdateCallback` implementation unconditionally
returned 1 after the update. The new natural C++ code captures the updater's
real result across the speed restoration and returns that result (or returns
the updater directly on the normal lane). It keeps the same two actual early
flag guards, speed global, call target, and owned return behavior. It does
not manufacture a result value or force a register through fake ABI types.

## Bounded pinned VC7.1 native experiments: NONEXACT

An actual pinned VC7.1 SP1 `/GL` callback-root compile/link was executed
against the **entire** 91-byte target and separately the 274-byte callee.
Before this semantic correction the native callback candidate was
**88 vs 91 PDB bytes**, 2/67 preliminary ordinary equal bytes, six
candidate address fields. With result forwarding and two explicit early
returns it is **87 vs 91 PDB bytes**, still 2/67 preliminary ordinary
equality, six candidate fields. That is **not a codegen improvement**;
full byte and field geometry still differ and no match is admitted. The
large list updater remains **275 vs 274**, with 39/254 preliminary
ordinary matching bytes and five candidate fields in the same actual
compiler graph. These are diagnostic values, not partial exactness.

Seven native variants were actually compiled: one direct result-forwarding
branch, separately spelled early returns, a named flag-mask, high-byte shift,
high-byte temporary, signed gate copy, and inverted pause branch. They
all retained a **87-byte** callback, and the parent stayed **275 bytes**.
Discarded variants were restored; the retained source is the direct
callee-result, early-guard natural formulation because it expresses the
actual target return-flow semantics.

The selected canonical cold replay after the source change passed the
existing **46-byte** laser draw callback and its distinct **43-byte** draw
list, **89/89 exact** with no added target matching credit. Other
historical exact units remain separately tracked. Neither the 91-byte
callback nor 274-byte parent is claimed byte-exact.

Only affected selected exact units need replay:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-laser-draw-callback-0041c4e0 \
  --unit enemy-laser-draw-list-0041c450 --json
scripts/repo-python scripts/ci.py
```

Private ignored local receipts:
`.analysis/gpt-web/20261104-next/laser-update-callback-inspect.log`,
`laser-update-91-return-panel.json`, `laser-update-91-gate-panel.json`,
and `laser-update-result-draw-guards.json`.
This preserves target interface semantics only; original author origin,
whole executable, runtime validation and portability remain unproved.
