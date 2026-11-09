> Historical 24-TU baseline: the current source owner has since moved to EnemyGameStateView::StartSpell in src/PlayerHudStartSpell.cpp. The current five-TU focused report is player-hud-start-spell-20261109/README.md.

# EnemyBeginSpell current-source baseline and ANM start ABI

This is a focused 24-TU incremental candidate, not an exact proof.

- Target owner: EnemyBeginSpell @ 0x00409280, 2,432 bytes.
- Candidate owner: 2,432 bytes.
- Ordinary bytes matching: 1,352 / 2,000.
- Candidate relocation fields: 108.
- Result: **NONEXACT**.

The experiment keeps the complete large boss owner in the comparison while
also including the selected ANM VM initializer and its source-level start
helper. The separate 0x00401DE0 leaf is exact (see
../anm-vm-initialize-fastcall-20261109/README.md), but that result does not
transfer to the 0x00404F30 start helper or the boss.

The start helper's current candidate is 200 bytes against the 202-byte target
and has only 23/192 ordinary bytes matching. Its VM register allocation still
does not reproduce the target's private ESI/EAX/stack entry convention.
candidate-baseline.json contains the complete selected-function comparison;
source-pins.json identifies the installed source epoch.
