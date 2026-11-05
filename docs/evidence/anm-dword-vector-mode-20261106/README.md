# TH10 ANM complete executor: direct DWORD state and timed-vector mode operands

Target: Japanese TH10 v1.00a SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The original ANM 8-byte instruction header prefixes a variable-sized
operand stream. Unlike the five single-byte flag setters, the `VISIBLE`,
`BLEND_MODE`, `Z_WRITE_DISABLE`, `FLAG13`, `POINT_TEXTURE_FILTER`,
`RENDER_MODE` and immediate alpha setter opcodes read a **full four-byte
integer** at the operand origin. `POSITION_TIME` and `ROTATION_TIME` read
a duration with the ordinary variable-operand helper, but their *mode* is
an entire second **32-bit** encoded word, unlike the one-byte modes
previously recovered for timed color/alpha/scale.

The maintained `AnmRawInstructionView` now overlays the same operand bytes
with `AnmDwordStateOperandView` (one 32-bit value at full instruction +0x08)
and `AnmTimedVectorModeOperandsView` (duration/encoded mode at +0x08/+0x0C).
Compile-time offsets and extents prove the data layout. Exactly **nine**
actual accesses in the complete `ExecuteScript` use the new views. No
allocation, altered evaluation order, bounds check, coercion or historical
original source names are asserted.

## Real bounded current-source original-bound proof

A **fresh five-TU pinned VC7.1 SP1 `/GL /GS`** source compile of
`AnmManager.cpp`, `RandomMath.cpp`, `AnmVmCreate.cpp`, `AnmVmId.cpp`,
and `AnmInt3Scale.cpp` (`/Ob1` last) reproduces the complete PDB-owned
**9,964-byte** ANM `ExecuteScript`. All **350 independently original-bound
fields**, including **95 local branch destinations**, pass; **9,962/9,964**
original code bytes match. The only two residual non-field opcode-91
register differences at `+0x113/+0x115` remain. This large owner is still
**NONEXACT** and earns zero new canonical bytes.

The explicitly selected original-bound exact leaf guards
`anm-get-int-var` (**144/144**) and `anm-get-float-var-ptr`
(**156/156**) pass **300/300** bytes in the affected source graph.
Default seven-stage `scripts/ci.py` and `git diff --check` pass. No
historical complete cold replay, product build, or runtime validation.

Private reproducibility:
`.analysis/gpt-web/20261106-anm-dword-vector/verify.py`,
`build/gpt-web/20261106-anm-dword-vector/current/gptweb-proof.json`,
`.analysis/gpt-web/20261106-anm-dword-vector/selected.json`.

Current source SHA-256: `AnmManager.cpp` `b4aefba570e98e5753bce501d7f38954de9c34080818d260c575be703f17c2f5`, `AnmManager.hpp` `267be9344aa302eef89f111aba8f621ebe9618fa0f8ff50c0c28a9e3b03d9818`.
