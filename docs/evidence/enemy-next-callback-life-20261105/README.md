# Enemy callback life remaining: one bound runtime field across four subsystems

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The runtime DWORD at `EnemyRuntimeView+0x13c8` (full enemy `+0x2404`)
was named `value13C8`. `EnemyResolveRuntimeCallback` sets it to the
current enemy life minus the next active `lifeThreshold`; the ECL SET_LIFE
opcode initializes it to full life, and both `EnemyRuntimeUpdate` and the
GUI boss-position interrupt schedule compare that same field against
thresholds. These code paths justify the descriptive name
`lifeToNextCallback`, **not** a claim to know ZUN's original identifier.

The complete maintained Enemy ECL dispatcher, Enemy lifecycle, callback
resolver, GUI HUD and layout asserts now share that single typed member.
`Enemy.hpp` also supplies the missing forward declaration for its existing
`AnmLoadedView` pointer members. Without it, standalone compilation of
`EnemyCallback.cpp` failed with undeclared-type and negative-subscript
errors. With the declaration, the file compiles as a standalone VC7.1
normal COFF translation unit.

## Native checks (current source)

- A fresh, bounded **11-translation-unit** pinned VC7.1 SP1 `/GL` build
  produces the 14,352-byte complete PDB ECL dispatcher candidate with all
  **716 candidate linkage fields** identical in offset/type to the preceding
  source-pinned graph. Its normalized ordinary code has **zero candidate
  differences**. EnemyBeginSpell (2,432/108 fields), death finalizer
  (162/9), viewport (330/10), and full enemy constructor (672/5) likewise
  preserve whole candidate owner size, field geometry and ordinary bytes.
- `EnemyCallback.cpp` compiles separately under the pinned normal COFF
  flags after the header declaration fix.
- Selected strict original-bound `gui-score-add` native replay: **46/46
  exact**, not an all-unit replay. Repository CI's seven lightweight checks
  and `git diff --check` pass.

Private proof: `build/gpt-web/20261105-next-callback-life/current/`
(`native-result.json`), reproducible command
`.analysis/gpt-web/20261105-next-boss/verify-callback-life.py`, and
`gui-score-selected-replay.json` / `callback-compile.log` under the same
private `.analysis` directory.

**No new exact credit.** The enemy ECL original is **14,416** bytes, not the
current 14,352-byte candidate. Current candidate-to-candidate equality is
only a regression proof, not an independent target-byte match. No product
runtime, original source spelling, or portability claim follows.
