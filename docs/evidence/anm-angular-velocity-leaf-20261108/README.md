# TH10 typed ANM angular-velocity word accessor

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The five-byte leaf at `0x00408830` performs one indexed dword read:

```asm
mov eax, dword ptr [eax + ecx*4 + 0x30]
ret
```

The maintained body now names the compatible field view:

```cpp
u32 ReadAnmVmAngularVelocityWord(i32 index, AnmVmView *vm)
{
    return reinterpret_cast<u32 *>(&vm->angularVelocity)[index];
}
```

`AnmManager.hpp` lays out `AnmVmView::angularVelocity` at `+0x30`; the
translation unit adds a compile-time `offsetof` assertion. The return remains
an opaque 32-bit word rather than a `float`, because the target returns it in
EAX and no callers exist to establish the consumer interpretation. The view
is layout-compatible with `AnmVmView`, not proof of original source ownership
or authorship.

## Focused exact replay

Command:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit game-decomphelp-fulltu-00408830 --json
```

Result: `exact`, 5/5 raw bytes, complete five-byte PDB contribution, zero
differences, and no linkage fields. No full cold replay, whole-image claim, or
runtime claim is made.
