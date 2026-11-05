# TH10 ECL runtime active-context time scalar across full Enemy dispatcher

Original executable: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The target `EnemyRuntimeView::DispatchEclInstruction` begins at `0x0040E770`
and occupies **14,416 bytes**, while the current complete source candidate
is 14,352 bytes and remains **NONEXACT**.

`ENEMY_ECL_SUBTRACT_DIFFICULTY_VALUE` retrieves one difficulty-selected
integer operand, converts it to a float, and subtracts that value from the
*first word* of the active ECL execution context. The old source loaded an
anonymous `float *` from `reinterpret_cast<int>(owner) + 4`, which hid both
which pointer was read and which scalar was updated. Independent original
context layout evidence already fixes the active-context pointer at full
Enemy +0x004 and the generic `EclVmContext::currentTime` float at context
+0x000. The Enemy-specific `EnemyEclContextView` now exposes a precisely
overlapping `float currentTime` and previous raw `int value00` at +0, with
an `offsetof` compile-time assertion. Existing five integer-zero reset
paths preserve their original bit stores. The boss's read/modify/write now
uses `&owner->activeEclContext->currentTime` in the **same scoped local**,
retaining the native pointer and operand lifetimes. No additional range,
allocation, or timing guards were inferred.

A fresh pinned VC7.1 SP1 11-translation-unit ECL graph was **fully compiled**
from the changed public header and installed source (no stale support
objects); its complete PDB owner comparisons against a source-pinned
pre-change graph report:

- `DispatchEclInstruction`: **14,352/14,352 candidate bytes**, **716**
  unchanged candidate field positions/types, **zero** ordinary candidate
  byte differences. It is **64 bytes shorter than the target**, thus NONEXACT.
- `EnemyBeginSpell`: 2,432/2,432 candidate bytes, 108 unchanged fields.
  Equal physical size does not prove target exactness.
- Enemy death finalizer: 162 bytes, nine unchanged fields.
- Viewport owner: 330 bytes, ten unchanged fields.
- `EnemyFullObjectView` constructor: 672 bytes, five unchanged fields,
  versus original 673 bytes; still NONEXACT.

Seven explicitly selected original-bound complete-function cold regressions
from installed source all passed: `enemy-runtime-constructor` 145,
`enemy-ecl-dispatch-forwarder` 11, `enemy-read-int-argument` 16,
`enemy-read-raw-float-argument` 26, `enemy-configure-interrupt` 48,
`enemy-ecl-resource-load-package` 239 and `ecl-vm-read-int` 144;
**629/629 previously exact target bytes preserved**. There is **zero new
codegen-exact credit**. The entire graph is not a reconstructed game product
or runtime semantic proof.

Current source, header and tool-linked evidence are in `native-result.json`;
ignored full native PE/MAP/PDB under
`build/gpt-web/20261105-ecl-context-time/fresh-11-tu/`. Only this affected
ECL graph and named exact units were built; no all-unit cold replay.
