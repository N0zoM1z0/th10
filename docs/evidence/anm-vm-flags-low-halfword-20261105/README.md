# TH10 ANM VM flags low-halfword: two complete owner writes

Verified Japanese TH10 v1.00a target SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The ANM VM stores one 32-bit flags word at offset `+0x35C`. The real
`AnmVmView::Initialize` and `AnmLoadedView::SetAndExecuteScriptIdx`
write **only the low 16 bits** of that word to `7`; they do not overwrite
the high 16 bits. The maintained view now overlaps the existing `flags35C`
word and named bitfields with `flagsLow16` at `+0x35C` and `flagsHigh16` at
`+0x35E`. Compiler-time `offsetof`/size assertions bind the native layout.
Two formerly opaque `unsigned short *` pointer casts in real C++ source now
write `vm->flagsLow16 = 7`. This adds no invented initialization, flags,
or original source-name claim.

A genuine **five-translation-unit** VC7.1 SP1 `/GL /GS` link freshly
compiled the modified `AnmManager.cpp`, `RandomMath.cpp`,
`AnmVmCreate.cpp`, `AnmVmId.cpp`, and `AnmInt3Scale.cpp` with the
registered `/Ob1` override for the final support TU. Its **complete**
`AnmRenderManagerView::ExecuteScript` PDB contribution remains 9,964 bytes.
The **independent original-target strict comparison** binds all **350**
linkage fields and their local destinations; **9,962/9,964** bytes match.
Only `+0x113` (ECX-vs-EDX load) and `+0x115` (ECX-vs-EDX push) in
opcode 91 remain different. This large function is **NONEXACT**.

Three selected canonical original-bound cold replays from the modified
source are exact: `anm-vm-constructor` **88/88**,
`anm-vm-initialize-private` **273/273**, and
`anm-loaded-set-and-execute-script-index` **194/194**. They preserve
**555/555 already credited bytes**, with **zero new exact codegen credit**.
The separate `SetAndExecuteScriptIdx` 287-byte candidate is not promoted.

Machine-readable current source/header pins and field/byte comparisons are
in `native-result.json`; the native PE/MAP/PDB remains in ignored
`build/gpt-web/20261105-anm-flags-halfword/current-5-tu/`.
No full historical cold replay, native product link or runtime claim.
