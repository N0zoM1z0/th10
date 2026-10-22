# ECL raw-negation storage and geometric lifetime interaction

2026-10-22, starting 0724572. Six actual-source controls test whether the known
shared-geometric-temporary regression can be separated from NEG_FLOAT_STORAGE's
payload representation. All retain unsigned modulo negation of the raw float
word, the stack tags and widths, read ordering, FORMAT, and compiler flags.
No maintained source or exact ledger change is adopted.

| Source form | Complete Run result |
| --- | --- |
| Raw negation local becomes unsigned int | 7020 / 6993; three large raw bodies equal baseline |
| Shared first geometric float, current raw-negation union | 7020 / 6989; fresh control for this compiler graph |
| Shared first float plus unsigned int negation storage | Same three complete raw bodies as preceding control |
| Shared first float plus explicit unsigned union member addresses | Same three complete raw bodies as preceding control |
| Shared first float plus explicit stack-view pointer in raw negation | Same three complete raw bodies as preceding control |
| One scalar union shared by first geometric operands and raw negation | 7020 / 6990; different stack allocation, still worse than baseline |

Counts are extent / matching bytes after the strict declared field replay, not
exactness claims. Every result binds all 189 fields and retains the exact
171-byte ReadFloatValue callee. All six pass nine whole ECL guards, 1137 bytes
and 18 fields. ANM and Enemy complete raw bodies remain baseline-identical.

The shared-float controls repair Run+1899/+18B6 but add six differences at
+169D/+16A8/+16AC/+16B0/+16B6/+16BB. Independent aligned disassembly identifies
the new mismatch as the raw-negation temporary moving from target ESP+1C to
ESP+18 (with the corresponding four-byte PUSH adjustment). Seven other existing
stack-home differences change their candidate byte, without becoming equal.
Primitive storage, explicit member addresses and a stack-view pointer do not
remove this interaction in the tested graph.

Sharing the union itself instead repairs +181B/+1840/+1844 in LENGTH, retains
the six raw-negation regressions, and changes the same seven existing stack-home
bytes. The two POINT_ANGLE repairs no longer occur. This distinguishes actual
allocator changes from merely equal difference counts; none improves the main
27-difference baseline. Source identity and original variable lifetimes remain
unknown. The operator requested handoff after the final build had started; it
was collected to completion, and no further experiment was launched.

## Reproduction and retention

The adjacent six patches and [evidence.json](evidence.json) bind actual source
shape assertions, producer hashes, complete differences, raw owner hashes,
canonical input hashes and all 96 removed native carrier files. All new builds
compiled only ECL and reused nine hash-verified game objects plus two harness
inputs, serially at nice10 with /GL and /GS unchanged. Baseline, target/toolchain,
tracking and fresh read-only Ghidra verification preceded the trials.

Private producers, source copies and full reports remain in
.analysis/gpt-6-astra-medium/ecl-neg-storage-interaction-20261022. Native carriers
were verified against their recorded hashes and removed, reclaiming 6399286
bytes. Canonical build inputs, maintained sources and nine unrelated untracked
D3D files were verified unchanged. No full CI or product/runtime claim follows.
