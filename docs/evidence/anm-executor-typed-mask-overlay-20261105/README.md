# TH10 ANM large executor: typed variable-mask overlay in 9,964-byte owner

The only accepted retail target is Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`AnmRawInstructionView::variableMask` is the 16-bit variable-argument
selector at on-disk instruction +0x06. The independently checked
`AnmVariableArgumentMaskView` already consumes that exact word in the
`GetIntVarPtr` and `GetFloatVarPtr` helpers. The large `ExecuteScript` source
instead duplicated raw bit arithmetic inside its `GET_INT_VAR` and
`GET_FLOAT_VAR` macros. We now overlap `unsigned short variableMask` with
the **existing typed** `AnmVariableArgumentMaskView variableArgs` at that
same location; the raw scalar is retained for ABI compatibility. The
`offsetof`/size assertion covers **both views at +0x06, both 2 bytes**.
The two macros read `currentInstruction->variableArgs.bits` and preserve
the same native integer/float argument interpretation and evaluation order.
There is no extra allocation, flag mutation, implicit bounds check, or
new instruction-field size.

A rejected natural experiment used a `__forceinline` bool helper with a
stack mask temporary. Real VC7.1 compilation expanded the complete
executor from 9,964 to 10,252 bytes and changed field geometry from
350 to 349 fields, so it was **fully reverted**. The retained direct
field access restores the **9,964-byte** PDB function. A fresh actual
five-translation-unit VC7.1 SP1 `/GL /GS` link was made with the final
source/header pins, including the original `/Ob1` support override.
The **strict original-target** full comparison passes all **350**
independently bound fields and their local/table targets, and matches
**9,962/9,964 original ordinary bytes**. The two remaining byte
mismatches `+0x113`, `+0x115` are the known opcode-91 ECX/EDX layer
transport and remain **NONEXACT**. `AnmVmView::Initialize` remains a
273-byte candidate; `SetAndExecuteScriptIdx` stays 287 bytes (still
nonexact). No similarity-based exact credit has been entered.

Six explicitly selected original-bound exact replays on the modified source
passed: `anm-vm-initialize-private` 273, `anm-get-float-var` 424,
`anm-loaded-set-and-execute-script-index` 194, `anm-get-int-var` 144,
`anm-get-float-var-ptr` 156 and `anm-get-int-var-ptr` 132.
Total **1,323/1,323 previously accepted exact bytes**, no new exact
credit. The full field-and-code evidence and SHA-pinned final sources
are in `native-result.json`; actual private PE/MAP/PDB under ignored
`build/gpt-web/20261105-anm-flags-halfword/current-5-tu/`.
No all-unit historical cold replay, complete product, or runtime-equivalence
claim is made.
