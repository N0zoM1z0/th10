# WinMM button merge: native 43-byte source recovery

Original Japanese TH10 v1.00a, SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`MergeWinMmButton @ 0x0044A160..0x0044A18A` is a complete 43-byte routine with two `RET 4` exits and **zero relocation fields**. Its private native inputs are CX signed button index, EDX output pointer, SI action, and one stack DWORD holding the WinMM buttons.

The natural `static __stdcall` C++ helper has a single stack-passed `buttons` argument after VC7.1 optimizes the other arguments to private registers. The source uses the original `1u << button` expression instead of a separate explicit `& 31` count normalization. The pinned i386 compiler emits `MOV EAX,1; SHL EAX,CL`, whose native shift hardware uses the low five bits, exactly like the original. This reconstructs the observed native behavior and removes three unwanted mask bytes; it **does not** claim defined portable C++ behavior for out-of-range shifts.

Independent compiler controls found that normal C++ calling convention emitted 39 bytes, and an explicit mask with `__stdcall` emitted 46. The selected `__stdcall` form emits **43/43** original bytes, including both returns, with no relocations. An alternative `__fastcall` source happened to produce the same bytes, so the original source declaration and the function's authorship remain unknown.

The selected native COFF unit is `input-merge-winmm-button-0044a160`. Reproduce with:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit input-merge-winmm-button-0044a160 \
  --unit input-merge-direct-button-0044a130 \
  --unit input-controller-buttons --json
```

This proof concerns one complete compiled helper, not the larger nonexact `MergeJoystickInput` or a runnable game product.
