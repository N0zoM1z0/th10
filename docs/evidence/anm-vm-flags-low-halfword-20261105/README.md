# TH10 ANM VM flags: recover the independent low 16-bit flag overlay

Original executable: Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

`AnmVmView::flags35C` is a 32-bit overlapping flag word at VM +0x35C.
Two *separate complete* original owners explicitly write its **low 16
bits** with value `7`, without requiring a 32-bit assignment:

- `AnmVmView::Initialize @0x00401DE0`: `MOV WORD PTR [EDX+0x35C],7` at
  original `0x00401E97` (9-byte original instruction).
- `AnmLoadedView::SetAndExecuteScriptIdx @0x0043E8B0`: `MOV WORD PTR
  [EAX+0x35C],7` at original `0x0043E90A`. That owner also preserves and
  restores the whole 32-bit flag word in its other paths.

The maintained VM union now exposes `flagsLow16` and `flagsHigh16` along
with the already named whole word and bitfields. Compile-time `offsetof`
assertions fix the two halfwords at +0x35C and +0x35E. Both real source
stores use `vm->flagsLow16 = 7` rather than casting the address of the
32-bit member to `unsigned short *`. This preserves high flags in paths
where they are live, and does not change any of the 32-bit or bitfield
consumers. The original historic spelling of the fields remains unknown.

## Original-bound full-owner verification

Both changed source and header were compiled by pinned VC7.1 SP1 in their
**real five-source `/GL /GS`** ANM interpreter graph, with the established
`/Ob1` support profile on `AnmInt3Scale.cpp`. In a complete source-epoch
comparison, `ExecuteScript` retains **9,964 bytes, 350 same-position and
same-type linked fields and zero ordinary candidate differences**;
`AnmVmView::Initialize` retains **273 bytes/zero fields**, and the real
`AnmLoadedView::SetAndExecuteScriptIdx` **287 bytes/four fields**, plus
`AnmLoadedView::InitializeVm` **207 bytes/one field**. These are whole PDB
contributions, not overlapping prefixes.

An independent **target-bound strict comparison** rebinds all 350 fields,
including 95 fixed original local/data destinations. The full 9,964-byte
`ExecuteScript` remains **9,962/9,964** original bytes, with exactly the
prior two EDX-vs-ECX register-transport bytes at `+0x113` and `+0x115`;
**NONEXACT**, zero new exact credit. Two scoped canonical original-target
cold replays pass: `anm-vm-initialize-private` **273/273** and
`anm-vm-constructor` **88/88** (**361/361 previously accepted bytes**).

`native-result.json` pins source/header SHA-256, all five input sources,
original write opcodes, strict full-owner comparison and scoped exact replay.
Ignored native PE/MAP/PDB are under
`build/gpt-web/20261105-anm-vm-halfword/full-boss/`. No historical full
cold replay, complete game executable or runtime-equivalence claim.
