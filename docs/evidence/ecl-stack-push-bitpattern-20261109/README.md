# ECL VM stack Push: preserve native type/value counter words

Original: Japanese TH10 v1.00a, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`EclVmStackView::Push` at `0x004505B0` is an already exact **96-byte** owner. The original program adds the type-record width (4) and the script-provided signed value byte count to its stack-top slot as native 32-bit words. The old maintained C++ computed unsigned sums and then cast each result to `int`; out-of-signed-range results depend on implementation-defined conversion even though VC7.1 uses the desired low bits.

The two publication steps now keep the unsigned modulo-32 sums in `typeTopBits` and `valueTopBits` and use ordinary, intrinsic-optimized `memcpy(&stackTop, &bits, sizeof(stackTop))` to publish their exact four-byte representations. This is not a byte-copy of any original executable code. It neither adds bounds checks nor alters the type tag stored in the prefix, value memcpy, signed initial capacity test, or return behavior. The initial capacity test's original implementation-defined signed projection **remains open**: changing it to a separate bitcopy drastically perturbs native codegen, and no unsupported equivalence is claimed.

Six actual VC7.1 SP1 `/GL` source variants under the EclVm/Enemy graph separately tested capacity-check, type advancement, value advancement, the two stores together and all three combined. Both store changes separately and jointly retain a **96/96** exact Push contribution, no relocation fields. The real **7,020-byte** `EclVmContext::Run` owner retains its **189** candidate field positions and **23** ordinary byte differences; the already exact 64-byte EnterFrame remains byte-identical. The capacity-check bitcopy instead makes Push **95 bytes** and Run **6,752 bytes**, so it was rejected rather than installed.

Rebuild **only** the affected owners from the selected native source and compare the original-bound big interpreter separately:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-stack-push --unit ecl-vm-stack-enter-frame \
  --unit ecl-vm-stack-pop --unit ecl-vm-read-float-value \
  --unit ecl-vm-host-run --json
scripts/repo-python scripts/diagnose-ecl-run-native.py --unit ecl-vm-host-run
scripts/repo-python scripts/ci.py
```

The full Run diagnostic still returns status 1 because Run is nonexact. Zero new exact ledger credit, native runtime, portability or whole-product closure follows from this source refinement.
