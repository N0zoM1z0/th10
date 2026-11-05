# TH10 ANM JUMP/JUMP_DEC serialized operand overlays

Target: original Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

The complete `AnmRenderManagerView::ExecuteScript` at `0x0043EE30`
consumes variable-length instruction bodies. After the 8-byte common header:

- JUMP has a signed script **byte** displacement followed by an integer
  destination timer value, at payload +0 and +4.
- JUMP_DEC has a writable countdown operand at payload +0, then the same
  signed byte displacement and destination timer at payload +4 and +8.

Previously the maintained opcode switch used anonymous `intArgs[0/1/2]`
indexes at all four jump-value reads. The source now adds
`AnmJumpOperandsView` and `AnmDecrementJumpOperandsView` into the existing
instruction payload union, each checked for native offset/size, and
names the four corresponding jump fields directly. The countdown still
uses the *original* GET_INT_VAR_PTR/GET_INT_VAR masked operand helpers;
conditional branch, byte-addressed script traversal, and timer update
ordering are unchanged. These are wire-prefix interpretations, not fixed
instruction allocations, invented historical class names or new compiler
flags.

## Fresh complete original-target proof

Pinned VC7.1 SP1 genuine **five-input `/GL /GS`** source graph with
`AnmManager.cpp`, `RandomMath.cpp`, `AnmVmCreate.cpp`, `AnmVmId.cpp` and
`AnmInt3Scale.cpp` (`/Ob1` on the last) recompiled. The strict complete
**9,964-byte** target-bound PDB owner comparison establishes **350/350**
independently original-verified field identities, including **95** local
branches/table destinations; ordinary code is **9,962/9,964** original
matched. The only remaining two differences are opcode-91 register choice
bytes at +0x113 and +0x115. The ANM boss remains **NONEXACT** with no new
exact ledger credit; the payload change does not shift this frontier.

Explicit original-target selected exact leaf replays for the operand
value and writable pointer helpers passed `anm-get-int-var` **144/144**
and `anm-get-int-var-ptr` **132/132**, **276/276** total. Default seven
lightweight CI checks and `git diff --check` passed; no historical cold
full replay. This does not assert whole-game build/runtime/portability.

Private reproduction:
`.analysis/gpt-web/20261105-next-anm-jump/verify.py`,
`build/gpt-web/20261105-next-anm-jump/current/gptweb-proof.json`,
`.analysis/gpt-web/20261105-next-anm-jump/selected-exact.json`.

Source SHA-256 pins: `AnmManager.cpp` `b72c879c28d21f84b97778546f175240aafc7bc47a58f306cb2b66d7fb753fe1`; `AnmManager.hpp` `788e1937eccdac7e2d5ba1e9dfd61d69ff6a704eab946c2b05e9720240bf7325`.
