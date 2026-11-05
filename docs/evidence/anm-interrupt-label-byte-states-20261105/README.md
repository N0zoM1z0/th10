# ANM ExecuteScript: native-width interrupt label and byte state payloads

Target is the pinned Japanese TH10 v1.00a executable, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The complete ANM `ExecuteScript` instruction stream carries an 8-byte
instruction header and a variable-length operand body. The interrupt-label
search compares the first 32-bit body operand against both the pending
interrupt and fallback sentinel. In contrast, five state opcodes
(`FLAG28`, `RENDER_LAYER`, `USE_SECONDARY_COLOR`, `FLAG27`, `ALTERNATE_RNG`)
consume exactly the first **byte** of the body. The source now defines
`AnmInterruptLabelOperandsView` and `AnmByteStateOperandView` as
same-address overlays, with size and offset assertions; the opcode switch
uses the corresponding named fields. This is a wire representation, not
an allocation, a guessed original class name, or a changed branch order.

## Native result: entire boss and selected dependent leaf guards

A fresh genuine pinned VC7.1 SP1 **five-TU** `/GL /GS` ANM graph, with
`AnmInt3Scale.cpp` under `/Ob1`, was compiled and verified against all
**350** independently original-bound fields (including **95** local
branches/jump-table targets) and the *complete 9,964-byte* PDB owner.
It retains **9,962/9,964** original ordinary bytes. The only two remaining
non-field differences are still opcode-91 `+0x113/+0x115` register choices.
`ExecuteScript` remains **NONEXACT**, so zero new match-ledger credit.

Selected canonical cold original-target replays of existing operand
helpers `anm-get-int-var` **144/144** and `anm-get-float-var-ptr`
**156/156** passed **300/300** bytes. This is only the affected helper
subset, not the full historical cold cohort. Repository lightweight CI
and whitespace validation pass. None of these checks implies full-game
build, runtime equivalence, or original private ABI knowledge.

Private reproducer and source-pinned proof:
`.analysis/gpt-web/20261105-anm-label-bytes/verify.py`,
`build/gpt-web/20261105-anm-label-bytes/current/gptweb-proof.json`,
`.analysis/gpt-web/20261105-anm-label-bytes/selected.json`.

Source SHA-256: `AnmManager.cpp` `1f4c521e8f4a70922ab022b7049231168394dd68d61fece1847d444306f0a0c2`, `AnmManager.hpp` `6b79ae190e74e054e5b6102a8deec36e09bcd0bcb61ec35f721f7e7330cd39b8`.
