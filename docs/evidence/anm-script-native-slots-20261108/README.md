# ANM script selection: native unchecked script-table word addresses

The three loaded-ANM script transition entrypoints read their script-indexed
pointer slots without validating the index. On original TH10 i386, the slot
address is a 32-bit table base plus index times four; an out-of-range index
is still addressed as a Win32 machine word. The maintained C++ now shares
`AnmScriptAtWord` for all five script-table reads in
`SetAndExecuteScriptIndex`, `InitializeAndExecuteScriptIndex`, and
`SetAndExecuteScriptIdx`. This avoids forming C++ out-of-array pointers in
the unchecked path while retaining the original indexed loads and control flow.

## Full owner and affected exact comparison

The complete 9,964-byte `AnmRenderManagerView::ExecuteScript` candidate,
including all 350 independently target-bound fields, retains **9,962/9,964**
original bytes. The complete 287-byte `SetAndExecuteScriptIdx` candidate,
with four original-bound fields, retains **265/287** bytes. Neither owner
is yet exact. In a selected current-source cold replay, the already accepted
`anm-loaded-set-and-execute-script-index`,
`anm-set-vm-script-index-and-execute`, and
`anm-evaluate-rgb-interpolation` remain respectively **194/194**, **36/36**,
and **820/820** exact. The first two span actual consumers of script
selection; the last protects the five-TU ANM compiler context used to check
the large parent. `scripts/ci.py` and `git diff --check` also passed.

The 9,964-byte parent's only outstanding ordinary differences remain the
opcode-91 EDX/ECX load and push at `+0x113,+0x115`. Ten separate new natural
source experiments on the 287-byte script-index switch (script pointer
qualifiers, index lifetime, guards, ordering) did not change its 22-byte
register-selection frontier; they were restored. Seven focused opcode-91
caller source experiments likewise failed to remove the two-byte difference.
The live private trial receipts are under `.analysis/gpt-web/20261108-current/`;
the cold replay is `anm-slot-cold.json` and the two complete original-bound
results are `anm-word-slot-original-bound.json` there.

Reproduce only the touched exact units:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit anm-loaded-set-and-execute-script-index \
  --unit anm-set-vm-script-index-and-execute \
  --unit anm-evaluate-rgb-interpolation --json
```
