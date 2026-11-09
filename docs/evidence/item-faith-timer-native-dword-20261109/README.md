# Native low-32-bit faith and timer state updates

The original Japanese TH10 v1.00a target (SHA-256 2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040) updates signed 32-bit faith and timer counters with native wraparound. GameScoreStateView::AddFaith/DecayFaith add or subtract the integer tenth of their argument and then apply the original signed upper/lower bounds. SetTimerCurrent stores the preceding frame as value-1 with the low DWORD bits preserved, including INT_MIN.

Previous C++ used an unsigned arithmetic result followed by static_cast<int>, whose out-of-range conversion is implementation-defined. The revised C++ updates the existing signed int object through its corresponding unsigned int type, a permitted lvalue alias. This retains the original low-word bits and signed range tests. No bounds clamp, input validation or register directive was added; the real counters, timer and memory layout are unchanged.

Two independently compiled real VC7.1 SP1 /GL context checks compared baseline and all-three-combined against original bytes. Under ItemHelpers, AddFaith (46/46), DecayFaith (46/46) and ItemVectorView::FromAngleMagnitude (30/30) remained target exact. Under ReplayManager::Initialize, SetTimerCurrent (59/59, one target-bound global field) and SetFaith (73/73, one target-bound field) remained exact. Both graphs used the real source and target executable. Detailed private receipts: .analysis/gpt-web/20261109/item-faith-timer-word-updates.py and build/gpt-web/20261109-live/item-faith-timer-word/results.jsonl.

Rebuild only the relevant game-score and item-manager exact functions and their direct callback consumers. This is a native C++ semantic improvement, not new codegen/exact-ledger credit.
