# Host sanitizer portability follow-up

Historical test record: the construction/lifecycle host suites cited here were
retired on 2026-10-28. Commands for deleted scripts apply to commit `47d9993`,
not the current tree. Their results are historical evidence, not requirements
for exact reconstruction; follow [the current workflow](RE_WORKFLOW.md).

The initially applied fresh v2 remote ASan+UBSan campaign failed: command
c902306744714edca1799035dab714a0 (2026-10-17 05:41:35–05:42:16 UTC)
timed out after 30 seconds while running the `sides` selection. This is not a
passing run. Mutants, lifecycle and constructor checks had not started in that
remote sequence. The original generated inputs, PIE executable and log remain
under `.analysis/gui-message-run-v2/host-sanitize` and
`.analysis/gui-message-run-v2/checks/host-sanitize.log`.

Parent-owned bounded retry command47dfc593d039414797e8bfdef8beff95 ran the
same retained executable, SHA256
d33abf4f5914de34915053e26a2bc37de43308b1e645498243ba5a27f19ef31f,
and `sides` exited0 in0.01 seconds without a source or binary change. Remote
GCC is8.4.0; the original local passing campaign used GCC14.2.0. Read-only
query e7268dab1f5945ac826f3b950e75586d confirmed matching generated Run,
dependency bodies and compared canonical header hashes; source extraction
drift was not observed. These observations are consistent with intermittent
ASan/PIE startup instability, but do not prove that as the root cause or
establish absence of undefined behavior.

Five further parent-owned bounded same-binary probes are recorded in
`checks/pie-sides-five-probes-summary.json`: one timed out at3.073 seconds
with repeated AddressSanitizer:DEADLYSIGNAL, then four passed unchanged in
0.023–0.035 seconds. This establishes nondeterminism, not a diagnosed source
loop or a proven ASan root cause. Twenty local repeated runs of the frozen
sanitized binary also passed (0.023–0.220 seconds); no local fixture fault
was reproduced.

This separate host-only patch adds `-fno-pie -no-pie` to sanitizer compiler/link
commands in the new Run and lifecycle drivers, following the existing
`test-gui-lifecycle.py` host convention. It changes no production source,
native VC7.1 flags, unsanitized command, timeout, CHECK expectation or strict
failure classification. Sanitizers remain enabled with fatal errors; the
existing explicit LeakSanitizer exclusion is unchanged.

Parent-owned command 232def64e5734e84a5b8004a3d28be08 (2026-10-17
05:45:34–05:46:51 UTC) completed all four remote sequences with exit 0:
13 non-PIE ASan+UBSan positives (ELF EXEC verified); 13 unsanitized positives
plus 28 strictly classified mutants; the actual constructor/factory→WAIT→END→
six-ID destructor bridge and strict sixth-deletion control; and the existing
constructor baseline. The last is not another remote 36-mutant run. Fresh local
36 behavioral plus 1 static constructor controls remain separate evidence.

Generated logs/exit receipts remain local ignored artifacts under
.analysis/gui-message-run-v2/checks/{host-nonpie,host-mutants,lifecycle-nonpie,
constructor}.{log,exit}; they are not committed evidence files. The original
PIE failure and same-binary probes remain preserved. Successful non-PIE runs
do not retroactively convert that failure into a pass or diagnose its cause.
LeakSanitizer remains explicitly disabled in these two new sanitizer drivers;
no leak-freedom claim follows. The patch itself changed only host sanitizer flags.
