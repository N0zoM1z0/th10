# TH10 DirectInput8 parent COM-release owner @ 0x00421D20 — 21/21 raw exact

Original TH10 JP v1.00a target, SHA-256 pinned at `config/target.toml`.
The complete **21-byte** native `0x00421D20..0x00421D34` has a plain RET,
no hidden/data bytes and no code relocation fields. The target's actual
`InputInitializeDevices` (0x0043B8D0) calls it after an error in keyboard
cooperative-level initialization. The receiver arrives privately in ESI.

The maintained type `InputDeviceOwnerView` at `src/InputDirectInputInit.cpp`
has a validated `directInput8` pointer at offset `+0x0C`. The newly recovered
`ReleaseDirectInput8()` member reads exactly that pointer, skips when null,
otherwise loads the COM vtable, calls its **slot +8** (Release) with the
interface pointer as stack argument, and clears owner `+0x0C` to NULL. The
full original instruction sequence is:

```
mov eax, [esi+0Ch]
test eax,eax
je ret
mov ecx,[eax]
push eax
call dword ptr [ecx+8]
mov dword ptr [esi+0Ch],0
ret
```

This method replaces the old undefined `InputReleaseDevices421D20`
facade at the actual failure branch of `InputInitializeDevices`. It is
**not** a duplicate shadow class or an isolated dead leaf.

## Canonical codegen proof

A scoped pinned VC7.1 SP1 `/GL` link retains the real 439-byte
`InputInitializeDevices` as entry and generates the 21-byte member's
receiver in native ESI, with a uniquely owned PDB contribution in the
unchanged `src/InputDirectInputInit.cpp`. Canonical unit
`input-directinput-release-00421d20` has **no** relocation fields;
all **21/21 bytes are raw-identical** to the original executable.
Two zero-reuse cold builds returned different PE SHA-256 and PDB GUIDs,
each passing the unchanged full-owner strict comparison. Receipts:
`.analysis/gpt-web/20261103/input-release-cold{1,2}.json`.

The 439-byte input initializer itself remains source-present **NONEXACT**.
Reproducing this referenced 21-byte helper does not prove full DirectInput
startup or retail runtime equivalence. Original source naming, physical
translation unit and ABI annotation remain unestablished. Only relevant
input-device and callback exact units are cold-replayed; no full product
or historical replay was performed.
