# One explicit destination-first source hypothesis

Status: source proposal with local host-contract validation. No maintained or production source was edited. No game/VC7.1 candidate was built, executed, adopted, or credited. The patch remains a local artifact; whole hypothetical source bytes exist only transiently for extraction/source-isolation tests. Temporary host harnesses compile the exact extracted case as described below.

Change exactly one case in EclVm.cpp: opcode ECL_VM_NORMALIZE_ANGLE gains a local block and `float *destination = ResolveFloat(0);`, then stores `EnemyWrapAngle(ReadFloat(0))` through that pointer. No header, function signature, calling convention, inlining directive, helper body, other case, build profile, or checker changes. No claim about the original source signature or private ABI is made.

Motivation: actual independently reviewed conditional receipt c40e958fba1c4367b070ef4c765b21a4 proves destination-before-read and read-before-wrap only from the verified table-selected entry to first direct escape under its explicit execution-model premises. The source reference states that order explicitly. Runtime opcode-to-entry association, recognized common advancement and complete opcode semantics remain unproved. This evidence is sufficient to motivate the isolated experiment; it does not establish a behavior defect, required production change, native equivalence, or improved byte matching.

Baseline expression does not portably specify callback evaluation order in the historical C++ language mode. Explicit sequencing therefore cannot be claimed universally equivalent for arbitrary stateful host callbacks. The two-schedule counterexample test intentionally yields different final values. It does not predict the current compiler's actual baseline schedule.

Actual maintained valid Enemy destinations are the four float-variable fields; their resolver only selects an address and the matching getter branches merely read it. Valid nonnegative frame-relative destinations similarly resolve/read the same field without mutation. For these pure valid paths, changing resolver/read order introduces no demonstrated state difference. Null destinations, stack-pop pseudo-values as destinations, RNG getter IDs without writable destinations, invalid frame offsets and exceptional/native floating-point behavior are outside this contract. No null-check or behavior repair is added.

Tests are authored Python semantic models and source-identity assertions, not compiled C++ or target tests. Eight tests pass, including 72 pure Enemy schedule combinations and 48 valid frame schedule combinations, preservation of unrelated state and float-storage bits, explicit callback instrumentation at an abstract seam, and a stateful abstract-host non-equivalence counterexample. The wrapping provider is an explicitly pure seam (identity, sign inversion or constant); these tests make no claim to reproduce native EnemyWrapAngle floating-point arithmetic. The production wrap source is only hash-checked and remains unchanged. Source isolation verifies exact one-block replacement and reconstructs the patch/hypothetical hash in memory.

Pins:

- Base EclVm.cpp: `8f236b6d2bdfa73bbf8747b16e934eb62264d2d54a811ac5f5cec789a1457efb`
- Hypothetical EclVm.cpp: `3c91d6beb3428ce9ef8da0dad5046cc6b303e2f2a28d2f07f57dbbd8c245ccf5`
- destination-first.patch: `61d07c4d18bce657cfa85d803f971e08650babb1e08ec7900f74ef5030b30e62`
- manifest.json: `0f15bf510152e870cb74cc5edf995deb4240d6efbc54e33694a52b2497c64a11`
- test_contract.py: `4aa0a80f31c0d7b4cabf62611ded9696e73d242adcf7b82f84af97298abdfa07`

Central reviewer says no further dispatcher provenance is prerequisite to proposing one separately approved exploratory build. Before any such build, parent must obtain exact experiment review covering current source/config pins, isolated application/revert, same real-host complete-owner harness, baseline/candidate controls, full 7020-byte owner/table accounting, and no automatic adoption. This packet itself grants no build authority. Native improvement remains unknown.

## Compiled host-contract supplement

At parent's explicit request, `compiled_contract.py` generated temporary C++ harnesses using the exact case extracted from the hash-checked hypothetical source. The frozen patch/hypothetical-source hashes are unchanged. Compiler: system g++ Debian 14.2.0-19, `-std=c++98 -O0 -Wall -Wextra -pedantic-errors`. This is a modern host compiler in a historical-compatible language mode, not VC7.1, x87, a native game build, ABI validation, or byte-match evidence.

The harness provides instrumented pure destination/read seams for four valid Enemy-style IDs and four frame-style offsets, plus a pure arithmetic provider seam. It checks 48 slot/value/domain combinations per positive harness, exact callback counts, explicitly required proposed-case D/R/W event order, destination float-storage bits, other cells and unrelated state. The original baseline expression is compiled separately under the same pure seams with output/count checks only; its event order is deliberately not prescribed or treated as portable.

Seven variants compiled successfully, each under a 15-second compile timeout and 2-second execution timeout, CPU10/12 seconds and 512MiB memory limits. Proposed and pure-baseline variants pass all48 checks. Five candidate mutants compile and fail with the designated semantic diagnostics: read-first → EVENT_ORDER; wrong destination → OTHER_DESTINATION; duplicate read, missing wrap, missing read → CALL_COUNT. A first diagnostic calibration expected DESTINATION_VALUE for wrong destination, but zero input leaves that selected value unchanged while corrupting the other cell; the expected diagnostic was corrected to OTHER_DESTINATION. No proposed source bytes changed. Temporary sources/binaries are removed automatically. Source hash is checked again afterward.

Host script SHA-256: `1062dc3811a27eee227cb02373dac6a6b78654776f42df88f46632f5be7d2471`.
Result SHA-256: `976a69ada41590db2b332489e64284d523608e8744908b0e7403e0164214bba8` (`compiled-contract-result.json`).

The earlier eight Python model tests remain separate evidence. This supplement validates the extracted authored case against explicit host seams; it does not prove actual game integration, arbitrary callback equivalence, production baseline compiler schedule, full opcode semantics, original signature/ABI, or native improvement.

Test-integrity gates: optimized Python is refused before source reads or compiler invocation, including direct imported execute(); two -O subprocess regressions pass. Every mutant must differ from the exact extracted proposal and from every other mutant. Control-test SHA-256: `59a9994ab2d05216b51159edec92d0c334574ecf3db33c88ba4e3cc631f64873`.
