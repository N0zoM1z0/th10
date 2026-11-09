# ECL EnterFrame: publish the saved-frame slot as a native bit word

Original Japanese TH10 v1.00a, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The existing `EclVmStackView::EnterFrame @0x00450690` is exactly **64 bytes**. It stores the previous frame-base DWORD into the VM stack and then advances the stack top by the four-byte serialized record width. The old source calculated a modulo-32 unsigned sum and performed an implementation-defined conversion to `int` for results above `INT_MAX`. This conversion is not needed to express the original low-32-bit binary behavior.

The retained source now calculates the sum into `const unsigned int storedTopBits` and publishes its four bytes with intrinsic-optimized `memcpy(&stackTop, &storedTopBits, sizeof(stackTop))`. It does not modify the frame-base data, the pre-existing signed capacity test, the first EnterFrame signed conversion (already repaired in `a4bae34`), or any bounds. The capacity test is still expressed through the older signed cast because moving it to a new local changes VC7.1 scheduling and the original comparison; that remaining source-language question is not silently hidden.

Six real C++ source forms were compiled against the current source epoch with pinned VC7.1 `/GL`, the actual EclVm and Enemy contributions, and original executable target. The selected publication-only bitcopy preserves **64/64** EnterFrame, **96/96** Push, **7020/7020** Run extent, all **189** Run linkage-field positions, and the same **23** non-field Run differences. Two guard-restructuring variants instead emitted 63-byte EnterFrame and caused large shifts in Run, so they were rejected. No copied target bytes, inline machine code, synthetic ABI or profile tuning were used.

Reproduce with selected units rather than full cold replay:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-stack-enter-frame --unit ecl-vm-stack-push \
  --unit ecl-vm-stack-pop --unit ecl-vm-host-run --json
scripts/repo-python scripts/diagnose-ecl-run-native.py --unit ecl-vm-host-run
scripts/repo-python scripts/ci.py
```

The large Run diagnostic still exits 1 until that full function is exact. This change adds no new canonical codegen credit; native runtime, portability and full game product completion are unverified.
