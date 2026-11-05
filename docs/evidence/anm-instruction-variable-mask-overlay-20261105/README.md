# TH10 ANM 16-bit variable argument mask: shared raw/typed instruction view

Original input: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`AnmRawInstructionView` encodes its 16-bit variable-argument selection
word at offset +6 of the instruction header. The independent variable
pointer resolver already uses `AnmVariableArgumentMaskView` to model this
bitmap. `src/AnmManager.hpp` now overlays the original scalar
`variableMask` with `AnmVariableArgumentMaskView variableArgs` in an
anonymous union and asserts that **both members start at offset +6**, and
the typed mask is exactly two bytes. The ANM `ExecuteScript` value-loader
macros read `currentInstruction->variableArgs.bits` instead of spelling
the raw mask word twice. No new storage or different flag semantics.

An alternative `__forceinline` boolean mask accessor was genuinely
compiled first, and **rejected**: it expanded the complete big VM
executor to 10,252 bytes with 349 linked fields, against the original
9,964 bytes and 350 target-bound fields. It is not in the committed source.

The retained *field overlay* was recompiled under the genuine VC7.1 SP1
`/GL /GS` five-source graph. A native PDB whole-owner comparison with the
**350 independently original-target-bound positions**, including local
branch/table destinations, gives `ExecuteScript` **9,962/9,964 bytes**;
only opcode 91 register-encoding bytes `+0x113` and `+0x115` differ (target
EDX, compiled ECX). Full `VmInitialize` 273 and
`SetAndExecuteScriptIdx` 287 PDB-owned candidate extents persist. The
main 9,964-byte executor remains **NONEXACT**; no new ledger credit.

Six explicitly selected existing original-target exact units passed
**1,323/1,323 bytes**: ANM `GetIntVar` 144, `GetFloatVar` 424,
`GetIntVarPtr` 132, `GetFloatVarPtr` 156, VM `Initialize` 273 and
`SetAndExecuteScriptIdx` 194. The tracked original target and installed
source/header SHA-256 values are recorded in `native-result.json`, with
ignored genuine linked PE/PDB/MAP witness under
`build/gpt-web/20261105-anm-flags-halfword/current-5-tu/`.

No full historical cold replay, original source identity claim, finished
whole-game Windows executable, or runtime-equivalence claim.
