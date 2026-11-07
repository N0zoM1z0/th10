# ANM ExecuteScript JUMP_DEC: native 32-bit wrapping decrement

The original Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`,
executes opcode `ANM_OP_JUMP_DEC` inside the large original
`AnmRenderManagerView::ExecuteScript @0x0043EE30` (9,964 bytes).
That opcode decrements the selected integer lvalue as a native 32-bit word
before reading its signed result to decide whether to jump.

The previous source `--*GET_INT_VAR_PTR(0)` expresses signed C++ decrement,
which has undefined behavior at `INT_MIN` although x86 DEC wraps low word.
The installed C++ now uses the corresponding `unsigned int` word lvalue:

```cpp
--*reinterpret_cast<unsigned int *>(GET_INT_VAR_PTR(0));
if (GET_INT_VAR(0) > 0) { /* original conditional jump */ }
```

`GetIntVarPtr` is invoked at the same point, and the subsequent operand
lookup and signed test are unchanged. No extra storage, widened operation,
synthetic ABI, or bounds guard is introduced. The original signed word
and corresponding unsigned word types may alias the same four bytes.
This byte-exact native behavior is pinned to the actual 32-bit compiler;
not a claim of portable arbitrary pointer behavior or original spelling.

## Entire original-bound boss proof

Four actual source trials tested unsigned predecrement, unsigned decrement
via `-=1u`, unsigned timer tick (separately), and the combined change,
compiling `src/AnmManager.cpp` using pinned VC7.1 SP1 `/GL /GS` and a
source-hash-checked four-TU support set including the real
`AnmInt3Scale.cpp` `/Ob1` code. Each newly linked image was compared
against the *complete original 9,964-byte owner*, its **350 independently
bound original fields**, and 95 local destinations. All trials preserved
the existing **9,962/9,964** original-byte frontier. The only two remaining
differences are opcode-91 register scheduling at `+0x113` and `+0x115`;
`ExecuteScript` remains explicitly **NONEXACT**. Temporary sources were
restored between experiments. The installed unsigned `JUMP_DEC` source
matches the strictly tested `jump-dec-unsigned` variant.

Only four affected previously accepted variable-access helper functions
were original-bound cold-replayed from that installed source:
`anm-get-float-var` **424/424**, `anm-get-int-var` **144/144**,
`anm-get-float-var-ptr` **156/156**, `anm-get-int-var-ptr` **132/132**;
**856/856** total. This adds **zero** newly exact owner bytes.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit anm-get-int-var --unit anm-get-int-var-ptr \
  --unit anm-get-float-var --unit anm-get-float-var-ptr --json
```

Full target-bound trial reports and current-source selected-cold receipt:
`.analysis/gpt-web/20261107-anm-timer-jump-word/{probe.py,results.json,jump-selected.json}`;
actual pinned native linked PE/MAP/PDB per trial in
`build/gpt-web/20261107-anm-timer-jump-word/`.
No whole-game or historical all-unit replay was performed.
