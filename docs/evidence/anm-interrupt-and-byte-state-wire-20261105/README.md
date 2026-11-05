# TH10 ANM: 32-bit interrupt labels and 8-bit state-setting wire operands

Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The native ANM `ExecuteScript` interpreter consumes a variable-size
instruction payload beginning at +8. It scans for `ANM_OP_INTERRUPT_LABEL`
using the first *32-bit* payload field, comparing both the requested
interrupt number and the fallback sentinel. Five other opcodes — FLAG28,
RENDER_LAYER, USE_SECONDARY_COLOR, FLAG27 and ALTERNATE_RNG — read only the
first **byte** of their payload. This distinction matters to the original
wire format: it is not safe to reinterpret these as all DWORDs, or to
promote the mode bytes to an ordinary arithmetic conversion.

The maintained C++ now introduces `AnmInterruptLabelOperandsView` (one
signed DWORD) and `AnmByteStateOperandView` (one unsigned byte), both
beginning at the same existing instruction union's payload +0. Compile-time
assertions check the exact 4-byte/1-byte widths and complete instruction
+8 offsets; the original `intArgs`/`byteArgs` aliases remain available for
other unclassified opcodes. The entire real opcode switch uses the new
semantic members for the two label comparisons and five state stores,
without moving the original label-scan side effects or adding a new
bytecode buffer. Historical original type spellings remain unknown.

## Fresh native owner contract

Pinned VC7.1 SP1 genuine five-source `/GL /GS` compilation from current
`AnmManager.cpp`, `RandomMath.cpp`, `AnmVmCreate.cpp`, `AnmVmId.cpp` and
`AnmInt3Scale.cpp` (last under `/Ob1`). The complete
`AnmRenderManagerView::ExecuteScript @0x0043EE30` PDB owner is **9,964
bytes** and matches **9,962/9,964** original ordinary bytes. All **350
original-bound** fields, including **95 local destinations**, pass; the
only remaining two normal-byte differences are the opcode-91 ECX-versus-EDX
register bytes at +0x113 and +0x115. **NONEXACT**, no new ledger credit.

Selected original-target cold replay of `anm-get-int-var` **144/144** and
`anm-get-float-var-ptr` **156/156** passes **300/300** complete bytes,
without the historical full cohort. All seven default repository CI checks
and whitespace gate pass. No source-era name, runnable game, runtime or
portability claim follows.

Private reproducer:
`.analysis/gpt-web/20261105-anm-label-bytes/verify.py`,
`build/gpt-web/20261105-anm-label-bytes/current/gptweb-proof.json`,
`.analysis/gpt-web/20261105-anm-label-bytes/selected.json`.

Installed source SHA-256: `AnmManager.cpp` `1f4c521e8f4a70922ab022b7049231168394dd68d61fece1847d444306f0a0c2`, `AnmManager.hpp` `6b79ae190e74e054e5b6102a8deec36e09bcd0bcb61ec35f721f7e7330cd39b8`.
