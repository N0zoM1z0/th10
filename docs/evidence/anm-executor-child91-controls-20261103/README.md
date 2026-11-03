# TH10 9,964-byte ANM ExecuteScript — new child91 compilation controls

This records fresh, **executed** source experiments against the complete
`0x0043EE30..0x0044151B` ANM script executor, not a reconstruction exactness
claim. The original Japanese v1.00a target, compiler identity and the
existing independent [350-field binding contract](../anm-child-request-record/binding-contract.json)
were retained. All 95 local/table reference destinations remain enforced.

The maintained source already has 9,962/9,964 ordinary bytes matching and
all 350 independently bound fields equal, with two register-encoding gaps
inside child opcode 91 at +0x113/+0x115 (native EDX, candidate ECX).

Two additional natural C++ value-passing forms were tried by writing the
**actual** `src/AnmManager.cpp`, recompiling the pinned VC7.1 `/GL /GS`
primary input, reusing the unchanged hash-validated four support objects,
linking the full selected PDB owner and invoking the existing strict
350-field comparator. These are two distinct real linked executables:

| New control | Complete ordinary byte agreement | Remaining gaps |
| --- | --- | --- |
| Two-element `int` argument array | **9962/9964** | +0x113/+0x115 (ECX vs EDX) |
| Stack argument record read through a const pointer | **9960/9964** | Same +0x113/+0x115; new +0x2483/+0x248A |

The const-pointer form regressed Z-vector update scheduling and was discarded.
The array form is neutral and was also discarded. Both source edits were
reverted byte-for-byte under a `try/finally` check; neither modified the
maintained `ExecuteScript` or its exact 73-byte `CreateVmVariant2` callee.

Private reproducible controller and reports:
`.analysis/gpt-web/anm/child91-20261103-new-forms.py`,
`.analysis/gpt-web/anm/child91-20261103-new-forms-results.json`.
The real native linked-image artifacts and strict bound reports are under
`.analysis/gpt-6.1-sol/anm-executor-inline-context-20261030/` and
`build/gpt-6.1-sol/anm-executor-inline-context-20261030/`.
No full cold product replay was invoked. In particular the 9962/9964 frontier
remains **nonexact**: no exact ledger adjustment is supported.
