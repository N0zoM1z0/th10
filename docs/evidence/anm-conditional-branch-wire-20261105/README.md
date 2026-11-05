# TH10 ANM conditional jump family: typed comparison and byte-addressed branch payload

Japanese TH10 v1.00a original SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

In the complete ANM `ExecuteScript` interpreter, the twelve integer/float
conditional jump instructions compare their first two 32-bit operands, and
on success consume a **script byte displacement** and target timer from
payload dwords 2 and 3. The previous real implementation used unnamed
`intArgs[2]/[3]` indexes at the shared successful-branch label; the
conditionals themselves must still evaluate `GET_INT_VAR`/`GET_FLOAT_VAR`
through the encoded 16-bit variable mask. The current source now overlays
this variable-length packet with one `AnmConditionalJumpOperandsView`:
comparison lhs at +0, rhs at +4, byte displacement at +8 and target timer
at +0xC relative to the argument payload. Compile-time assertions bind
its **16-byte** extent and the displacement/time at complete instruction
+0x10/+0x14. This is a serialization recovery, not a fixed-size instruction
allocation or an assertion about historical original symbol names.

## Complete source-bound native evidence

Pinned VC7.1 SP1 five-source `/GL /GS` build of actual `AnmManager.cpp`,
`RandomMath.cpp`, `AnmVmCreate.cpp`, `AnmVmId.cpp`, `AnmInt3Scale.cpp`
(`/Ob1` last). The strict comparator includes the **entire original 9,964
byte** `AnmRenderManagerView::ExecuteScript` PDB body, **350 independent
original-bound** relocation/control fields and **95 local destinations**.
Result: correct 9,964-byte owner, **9,962/9,964** matching original ordinary
bytes and the *same* two opcode-91 register bytes +0x113/+0x115. The boss
remains **NONEXACT**, zero new match-ledger credit. This does not assume an
older record's correctness: the whole current image was recompiled and
compared against the independently specified target field contract.

The directly affected `GetIntVar` and `GetIntVarPtr` accepted leaf functions
were recompiled in a **selected** canonical graph and strictly passed
**144/144 + 132/132 = 276/276 original bytes**. `scripts/ci.py` seven checks
and `git diff --check` pass; no all-unit cold replay. No original source
identity, binary product, runtime or portability claim follows.

Private reproduction:
`.analysis/gpt-web/20261105-th10-conditional-jump/verify.py`,
`build/gpt-web/20261105-th10-conditional-jump/current/gptweb-proof.json`,
`.analysis/gpt-web/20261105-th10-conditional-jump/selected.json`.

Installed SHA-256 pins: `AnmManager.cpp` `fd976c4db1d21bf108be43f64c04bdd5ccadb74b5ea45208955fde782c26202e`, `AnmManager.hpp` `7d918c92c3bba71c89790308d3681e51880af746212cc9cb774eef461cace7ea`.
