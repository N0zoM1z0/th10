# TH10 full ANM ExecuteScript: typed variable-length instruction traversal

Original target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Complete `AnmRenderManagerView::ExecuteScript` target
`0x0043EE30` is **9,964 bytes**.

The existing `AnmRawInstructionView` defines the retail script instruction
header: signed opcode `+0`, byte extent `+2`, instruction time `+4`,
argument variable mask `+6`, arguments `+8`. Native compiler assertions
now check all five offsets. Because the stream contains variable-length
records, incrementing an instruction pointer by `sizeof` is wrong: the
actual next instruction is at **current byte address + `size`**.

The maintained source now expresses **three** executor traversal sites
(normal step, interrupt-label scan, and interrupt-resume step) through
`AnmRawInstructionView::Next()`; **three** jump origins (unconditional,
conditional/decrement, and comparison jumps) use
`beginningOfScript->AtByteOffset(rawOffset)`. The source still uses exactly
the original opcode-derived byte offset, does not introduce bounds checks,
does not change the fallback sentinel handling and does not invent an
original class name or private calling convention.

## Original-target exactness check

The complete pinned VC7.1 SP1 `/GL /GS` ANM **five-source** graph was
built with the required `/Ob1` override for the existing AnmInt3Scale
support. The real linked PE and PDB were compared using the established
**350-field target-bound** manifest, including **95 local destinations**.
The maintained owner is still 9,964 bytes, with **9962/9964 exact
original-target bytes**. Exactly the *same two* original byte differences
remain at `+0x113` and `+0x115` (ECX/EDX child layer transport). All
other ordinary bytes and all 350 field/destination obligations match. This
is a complete-owner strict comparison, not an isolated snippet check.
**The large function is still NONEXACT**, and this work adds no canonical
match credit.

Selected affected existing native exact replays cover the ANM loaded VM
variant-2 creator, VM timer Add, VM timer Decrement and complete RGB
interpolation evaluation; they all remain exact. Seven-stage lightweight CI
passed. We did **not** run a full historical cold replay, nor claim game
product build/runtime closure.

The new complete PE/MAP/PDB remains ignored at
`build/gpt-web/20261105-followup-anm/typed-instruction-next/`.
Reproducer/attested result and selector list are in `native-result.json`
and `.analysis/gpt-web/20261105-followup/anm-next-strict.py`.
