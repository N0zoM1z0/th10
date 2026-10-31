# Complete configuration reset: 125-byte exact owner — 2026-10-31

`GameConfigurationView::ResetDefaults @ 0x00418C40..0x00418CBC`
(125 bytes, inclusive end) is now **canonical linked-PE exact**. This
adds **one reviewed authored owner and 125 exact authored code bytes**.
The verified target is original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

## Actual target-bound compiler context

The source remains the ordinary `void GameConfigurationView::ResetDefaults()`
member in `src/GameManagerLifecycle.cpp`, with no invented return value or
private register annotation. The exact probe uses the actual caller
`MainLoadConfiguration` in `src/MainConfiguration.cpp` as its sole `/GL /GS`
entry, with `GameManagerLifecycle.cpp` as an actual `/GL` support source.
VC7.1 SP1 build 6030, the existing LINK profile and the unresolved-data
anchor harness remain unchanged. The complete function comes from the unique
`GameManagerLifecycle.cpp` PDB section contribution; no prefix, tail, byte
patch or compiler-generated filler is admitted.

The source change moves the existing `colorMode = 0` store directly after the
object clear and before the nearby numeric defaults. The older order is
**116/125 nonexact** when rebuilt in that *same* two-source graph, with
ordinary differences at offsets `+1`, `+2`, `+3`, `+D`, `+14`, `+18`,
`+1A`, `+1D`, `+20`. Restoring the observed assignment order gives
**125/125 exact**. This is machine scheduling evidence rather than proof of
the original C++ statement order or physical source-file ownership.

All five absolute reference fields are independently bound to the real
`g_DefaultGameKeyBindings` base `0x00474E88`: fields at offsets
`+0x3C`, `+0x47`, `+0x50`, `+0x59`, `+0x63` respectively address
base+0, +4, +8, +12 and +16. The original candidate manifest incorrectly
put the already-added target address *and* the offset in the same row;
its initial **121/125** comparison was a field-declaration error, not a
codegen mismatch. All five corrected field identities and the complete
body now pass the unmodified strict comparator, including local control
transfers and termination.

## Scoped independent acceptance

Two independent targeted canonical cold replays, selecting **only**
`game-configuration-reset-defaults`, pass all 125 bytes and all five fields.
Their separately produced PDB GUIDs are
`57716569e0d782499a63c5085b7e9a45` and
`4bd7a67415afcc489694307b434786cf`. The corresponding ignored reports
are `.analysis/gpt-web/game-reset-cold-1.json` and
`.analysis/gpt-web/game-reset-cold-2.json`; the old-order controlled
comparison is `.analysis/gpt-web/game-reset-old-compare.json`.

```sh
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/replay-exact-units.py --unit game-configuration-reset-defaults --json
scripts/repo-python scripts/ci.py
git diff --check
```

Only this genuine two-input compiler graph is rebuilt; the 28-input WinMain
historical producer, unrelated owners and full cold replay are not needed
for acceptance. No data-owner, complete product, semantic runtime, original
class/source provenance or portability closure follows from these bytes.
