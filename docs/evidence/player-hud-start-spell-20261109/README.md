# TH10 spell-card HUD start (0x00409280)

This is the current focused source reconstruction of the 2,432-byte spell-card HUD start routine. The maintained source surface is EnemyGameStateView::StartSpell in src/PlayerHudStartSpell.cpp; the original class and translation-unit ownership remain unproven.

## Current five-TU candidate

- Target extent: 0x00409280..0x00409BFF (2,432 bytes).
- Candidate extent: 2,416 bytes (16 bytes short).
- Focused link: five source TUs, 100 unresolved-symbol anchors, 32.8 seconds.
- Candidate-only relocation fields: 108 fields / 432 bytes.
- Ordinary bytes matching at equal offsets: 531 / 1,984.
- Result: NONEXACT.

The source now expresses panel creation through the existing native operations AllocateVm, InitializeVm, and AddVmVariant0, rather than maintaining a duplicate list-registration implementation. The full stage switch, replay-gated spell-history updates, digit VM setup, managed effect, child timers, bonus calculation and stage-specific scripts are present.

The exact 273-byte VM initializer at 0x00401DE0 is independently established in ../anm-vm-initialize-fastcall-20261109/README.md. The 0x00404F30 script-start adapter is separately maintained in src/AnmVmStartScript.cpp, but remains nonexact (200/202 bytes, 23/192 ordinary bytes). Neither leaf result transfers exactness to this boss.

See focused-five-tu-report.json for the selected-function linked-PE comparison and source-pins.json for the exact source/image hashes. This is a focused replay only; no full cold replay was run.
