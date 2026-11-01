# ANM child91 member __stdcall ABI — 2026-11-01

## Selected source model

`AnmLoadedView::CreateVmVariant2` is a non-static member with `__stdcall`:

```cpp
AnmVmIdView __stdcall CreateVmVariant2(
    int scriptIndex, unsigned int renderLayer);
```

The implicit `this` pointer is stack-passed as the first of three stack
arguments. The caller also supplies `scriptIndex` and `renderLayer`; EDI holds
the hidden `AnmVmIdView` result pointer. The callee returns with `RET 0xC`.

This is a better source/ABI model than the previously tested static overload
with an explicit `AnmLoadedView *` parameter. The latter produced byte-identical
leaf code, but its `SG` symbol did not represent the target's member identity.
The selected compiler symbol is:

`?CreateVmVariant2@AnmLoadedView@@QAG?AUAnmVmIdView@@HI@Z`

## Focused native proof

A fresh selected VC7.1 SP1 `/GL` build of the maintained ANM translation unit
and current support graph produced:

- `CreateVmVariant2`: complete PDB contribution **73/73 exact**.
- All five target-bound fields: exact after normalization.
- Full PDB-owned function size: 73 bytes.
- Function epilogue: `RET 0xC`.

The strict linked-PE comparator binds the function to target `0x00448E30` and
replays all five linkage fields. No target-byte patch, inline assembly, forced
register annotation, padding, or ABI shim is used.

## ExecuteScript parent frontier

The 9,964-byte `AnmRenderManagerView::ExecuteScript` parent remains
**9,962/9,964**. Its only ordinary-byte differences are:

- `+0x113`: candidate `MOV ECX,[EBP+0x20]`, target `MOV EDX,[EBP+0x20]`.
- `+0x115`: candidate `PUSH ECX`, target `PUSH EDX`.

All 350 independently bound fields and 95 local destinations remain in the
strict parent comparison. The static-call spelling, direct-expression variants,
script-local variant, and volatile-layer controls did not improve this frontier;
some regressed other bytes and were discarded. The selected source keeps the
small child-request record because it preserves the 9,962-byte parent baseline.

## Exact target call order

The target call sequence at `0x0043EF42..0x0043EF55` is:

```asm
mov edx, [ebp+0x20]
push edx
push eax
mov eax, [ebp+0x308]
push eax
lea edi, [esp+0x100]
call 0x00448e30
```

The current candidate matches this sequence except that it uses ECX for the
layer load and push. Further work should preserve the exact 73-byte member leaf
and explore only natural caller code generation.

## Scope

This is a focused linked-PE code-generation proof, not a Factory acceptance
receipt, original source/TU provenance claim, runtime claim, or whole-image
claim. The parent remains nonexact and receives no exact credit.
