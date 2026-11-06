# Exact native word wrapping in faith decay and timer publication

The verified Japanese TH10 1.00a original uses ordinary i386 word arithmetic
for `GameScoreStateView::DecayFaith @0x00412E70` and
`GameScoreStateView::SetTimerCurrent @0x0042A930`. The first divides an award
by ten, subtracts that amount modulo 2^32, and compares the **signed** stored
faith with the minimum 5000. The latter sets `timer.previous = value - 1`
modulo 2^32, including `value == INT_MIN`, then publishes current and float
subframe. Both previously used signed C++ arithmetic that is undefined on
overflow. The new source expresses the low-word operations as unsigned
subtractions and reinterprets the resulting native word through the pinned
VC7.1 i386 signed result representation. No timer field or ABI has changed.

## Two independent targeted native groups

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit item-faith-decay --unit game-score-set-timer-current --json
```

The selected `/GL` cold replays pass **46/46** (zero fields) and **59/59**
(one independently target-bound `g_PlayerTimerScale` address) = **105/105
exact bytes**, across two distinct compiler graphs. These are existing exact
owners, not new ledger credit. The first group uses the authentic adjacent
item polar helper, the second the actual replay initializer and its supporting
TU. The same-image checks of the earlier `AddRank` and `AddFaith` in the
newly compiled ItemHelpers artifact also pass **42/42 and 46/46**.

The private original-bound receipts are under
`.analysis/gpt-web/20261106-chat-th10/score-decay-timer-trial.json`,
`score-rank-same-image.json` and `score-faith-same-image.json`. The pinned source
SHA-256 is `b029539ab91eba926525a1de3dcb6baaa05d3b6c53e181702b325b8a4ad32fcc`.
This changes native semantics at the `INT_MIN` boundary without adding an
unreviewed control-flow branch, target-byte patch, artificial helper, whole
product or runtime claim.
