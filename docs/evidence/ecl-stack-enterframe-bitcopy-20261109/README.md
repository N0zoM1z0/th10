# ECL EnterFrame: preserve the native signed stack-top bits

Original Japanese TH10 v1.00a executable SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The maintained `EclVmStackView::EnterFrame` computes `previousTop + localBytes` as the low 32-bit i386 stack word, then compares the result **as a signed int** against 0x1000. The old C++ used a cast from `unsigned int` to `int` after unsigned addition. For wraparound values outside `INT_MAX`, that cast is implementation-defined even though MSVC 7.1 implements the intended two's-complement bit pattern.

The source now computes the unsigned sum in `nativeBits` and copies its four bytes into `int localTop` through ordinary `memcpy`. This expresses the observed representation exactly without a signed out-of-range conversion. It leaves all negative/signed threshold behavior, saved frame word, stack pointer publication, pre-existing unchecked allocation semantics and subsequent 4-byte counter update unchanged. This is a fixed 32-bit i386 code model, not a general portability or bounds guarantee.

Five *real* C++ source candidates were compiled in the EclVm/Enemy `/GL` source graph: the original cast, `memcpy`, an unsigned-reference alias, an unsigned-pointer alias, and a separate writable unsigned temporary. All yielded the same 64-byte `EnterFrame` implementation, no target relocation fields, and a complete original 7,020-byte `Run` with the same 23 differing locations and 189 independently bound fields. The selected `memcpy` source does not use inline assembly, artificial ABI controls or compiler padding. The original `Pop` caller remains 122/122 exact.

Validate the current installed source using **only** affected exact units and the complete big-owner diagnostic:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-stack-enter-frame --unit ecl-vm-stack-pop \
  --unit ecl-vm-stack-push --unit ecl-vm-read-float-value \
  --unit ecl-vm-host-run --json
scripts/repo-python scripts/diagnose-ecl-run-native.py --unit ecl-vm-host-run
scripts/repo-python scripts/ci.py
```

`diagnose-ecl-run-native.py` deliberately returns nonzero while `Run` remains nonexact. No exact bytes are added to the canonical ledger by this change; the exact ECL stack owners were already admitted. Original TU provenance, native runtime tests, and complete product closure remain outside this evidence.
