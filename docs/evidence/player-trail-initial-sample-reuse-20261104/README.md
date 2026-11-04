# TH10 Player option trail: preserve the initial X/Y sample across zero-mode setup

Original target: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The physical Player option-trail body `0x00427960..0x00427AC6` is **359 bytes**.
This note concerns the *entire* body, not the already accepted separate
12-byte entry callback at `0x00427950`. The original private ESI option
receiver, historical C++ source declaration and production TU remain unknown.

The original zero-mode path copies two adjacent dwords from the replay
history sample into `option->replayPair0` at offsets +0x34/+0x38 and then
uses the same X/Y values when forming its offsets from the real player
position. The formerly maintained source reread the just-written option
members through `option->replayPair0.x/.y`. The revised natural C++ in
`src/Player.cpp` explicitly names the initial `const int initialX` and
`initialY` obtained from the *real* `PlayerPositionPair` and reuses those
values in the zero-mode subtraction, while retaining both original
member stores. This does not add a fake ABI, rewrite machine instructions,
change the pointer's origin, or insert inert padding. The player history
and option record have separate native field ranges; no new storage owner
is introduced. The staged volatile coordinates and typed history pointer
for the nonzero mode are left intact.

## Full native PDB comparison: real improvement, still NONEXACT

A genuinely compiled VC7.1 SP1 `/GL` three-source WPO graph using primary
`Player.cpp` and real `AnmVmId.cpp`/`AnmManager.cpp` support under
`RebuildPlayerOptions` produced the following **complete 359-byte PDB**
contribution comparisons against the **entire 359-byte original**:

| Maintained source variant | PDB/target | Ordinary positional agreement |
| --- | --- | ---: |
| Prior staged typed nonzero coordinates | 359/359 | 215/339 |
| Zero-mode reread via the history entry | 359/359 | 210/339 |
| **Named initialX/initialY reuse (retained)** | **359/359** | **245/339** |
| Temporary whole `PlayerPositionPair` copy | 359/359 | 245/339 |
| Directly subtract from history without intermediate fields | 375/359 | 46/339 |

The change improves **30 ordinary byte positions** over the previous
source. There are still five candidate address fields and many byte
mismatches; the target-bound original field geometry, full byte sequence
and private receiver are **not** proven. The 245/339 score is purely
relocation-normalized *diagnostic*, **not** canonical exact acceptance.
No `config/matches.csv` row or exact codegen credit is added.

A fresh selected canonical **cold** replay with the installed changed
source passed the three existing exact callback owners:
`player-option-trail-callback` **12/12**,
`player-option-special-callback` **12/12**, and
`player-option-special-body` **99/99**, totaling **123/123 accepted bytes**.
The selected source's SHA-256 was
`862667c43725c7422b3c3dc1060d4ebe0dfb50145f7f52574a2c60623c7858a9`.
The exact units were *regressions*, not newly accepted owners. No all-unit
cold replay ran.

Repeat only the affected accepted units:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit player-option-trail-callback \
  --unit player-option-special-callback \
  --unit player-option-special-body --json
scripts/repo-python scripts/ci.py
```

Private source and native receipts under `.analysis/gpt-web/20261104-next/`:
`player-zero-source-reuse-results.json`,
`player-zero-reuse-selected-guards.json`, and
`player-zero-reuse-maintained-proof.json`. This is complete-owner source
codegen progress, not source-era authorship, runnable product, verified
runtime behavior, or portability acceptance.
