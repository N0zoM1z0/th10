# GUI boss life-gauge consumer of the typed Enemy baseline

Original target: Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The typed Enemy life-gauge baseline at runtime +0x13C4 was recovered
in the preceding `gpt-web: bind enemy ECL life gauge baseline to its runtime
field` commit. A cross-module build subsequently exposed two consumers in
`src/Gui.cpp` that still named the old `unknown13C4` member. Both are
now reconciled: the compile-time full-Enemy layout obligation at +0x2400,
and the actual `GuiView::UpdateStageElements` boss-life normalization,
`float(runtime.life) / float(runtime.lifeGaugeReference)`.

An actual pinned VC7.1 normal COFF source compile of `src/Gui.cpp` passed;
selected original-bound native cold replays passed
`gui-set-lives-display-count` 77/77 and `gui-score-add` 46/46,
**123/123** total. Seven-stage lightweight CI and `git diff --check`
passed. The large GUI update owner was **not** accepted or replayed as
exact: this edit changes the member name without claiming original C++
spelling, full-product closure, original executable codegen or runtime proof.
