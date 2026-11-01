# ANM child91 static ABI correction — 2026-11-01

## Result

The source declaration for `AnmLoadedView::CreateVmVariant2` was corrected
from an implicit non-static member call to a static `__stdcall` function with
an explicit `AnmLoadedView *file` first argument:

```cpp
static AnmVmIdView __stdcall CreateVmVariant2(
    AnmLoadedView *file, int scriptIndex, unsigned int renderLayer);
```

The target call at `0x0043EF55` supplies three stack arguments (file, script,
layer), an EDI hidden-result pointer, and expects `RET 0xC`. This is not a
normal `__thiscall` member invocation. The body calls `file->InitializeVm`,
allocates through the global render manager, sets the render layer and flag,
and inserts through `AddVmVariant2`.

## Focused native evidence

A fresh selected VC7.1 SP1 /GL linked-PE build using the maintained ANM executor
translation unit and the current four support objects produced:

- `CreateVmVariant2`: complete PDB contribution **73/73**.
- All five declared target link fields: exact after target binding.
- The generated symbol is
  `?CreateVmVariant2@AnmLoadedView@@SG?AUAnmVmIdView@@PAU1@HI@Z`.
- The emitted function ends in `RET 0xC` and has the correct static `__stdcall`
  stack interface.

The focused artifact was compared against target `0x00448E30`, extent 73, with
the existing strict linked-PE comparator. The result was `exact`, 73/73, with
complete normalization and no remaining differences.

## Large caller remains open

The complete `AnmRenderManagerView::ExecuteScript` target extent remains
**9,962/9,964** in the current selected build. The only remaining differences
are at offsets `+0x113` and `+0x115`: target loads the child layer into EDX and
pushes EDX, while the candidate loads it into ECX and pushes ECX. The static
ABI correction fixes the source-level call contract and the exact leaf body;
it does not by itself solve those two caller code-generation bytes.

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

No byte patch, inline assembly, forced register annotation, padding, or ABI
shim was used in the committed correction. Further work should preserve the
73-byte exact leaf and continue exploring natural caller code generation.

## Scope and limitations

This is a focused linked-PE code-generation proof, not a Factory acceptance
receipt, original source/TU provenance claim, runtime claim, or whole-image
claim. The complete 9,964-byte parent remains nonexact. The historical
`anm-child-request-record` note describing the old member-call spelling is
superseded for this one child91 ABI; its independent operand/field evidence
remains useful.
