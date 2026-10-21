# ECL thread operands and shared node lifetimes

Started 2026-10-22 at `bd9be46`. Eleven private natural-source controls test the
remaining early Run register choices in the current ten-TU context. None is
adopted. Maintained game code, headers, recipes, mappings and exact counts remain
unchanged. [Evidence and all source patches](evidence.json) preserve the inputs.

Fresh target/Ghidra review observes opcode10's indexed raw ID in EDX, the outer
skip-argument PUSH1 before ReadIntValue, and the host reload after that call.
STOP_THREAD uses EAX for the node's context. The native baseline instead uses
EDI for the ID and EDX for that context. Calls, fields and instruction positions
otherwise agree in these spans. Ghidra names and source types remain hypotheses.

The unmodified maintained diagnostic reproduces the complete7020-byte Run with
27 ordinary differences, all189 fields/62 local destinations and the complete
171-byte ReadFloatValue callee exact. Current32 source/header and15 native pins
still equal the [raw-NEG checkpoint](../ecl-raw-negation/native-summary.json).
No full cold is performed here: each private control recompiles only ECL,
reuses nine hash-verified game objects and relinks. Each image passes35 complete
guards for4424 bytes/all118 fields, with all table words/selectors still checked.

| Source model | Run differences | Complete outcome |
| --- | ---: | --- |
| Signed nonnegative index; live instruction cursor; shared context pointer; stopped-context reference |27 each|All three large raw native bodies equal baseline |
| Named operand-word pointer; indexing words from the instruction base |31 each|Identical Run outputs; four new differences in opcode10, no baseline difference repaired |
| Shared thread node in all four cases, with/without shared contexts |27 each|Repairs+146E but adds+AF in RETURN |
| Share node for three leaf operations, two flag operations, or stop/control pair |27 each|Same complete target-replayed Run as the preceding shared-node pair |

The five shared-node variants exchange the base/index order of two unscaled
SIB encodings. They recover target+146E while reversing RETURN's+AF. Their
complete target-replayed Run hashes agree with each other, but differ from the
baseline at exactly those two bytes. Thus an unchanged difference count does
not imply an unchanged body. ANM9964 and Enemy14352 remain raw-identical in all
eleven images. No partial exact credit follows from the locally repaired byte.

The private diagnostic adapter changes only two source-path selectors and ROOT
discovery so the PDB owner names the actual private ECL file. Comparator,
canonical target bindings, full callee,189 fields/62 locals and table/selector
coverage rules remain unchanged. Baseline/final maintained-source checks use the
unmodified tool. The exact source spellings, lifetimes and original physical TU
remain unknown.

An earlier statement that a separate StopThread context-local form was
unexecuted described only the thread-expression campaign. Older
[residual controls](../ecl-run-disabled-trace/residual-controls.json) and
[current-dependency controls](../ecl-current-dependencies/README.md) had already
executed it; the latter retained26 in its historical two-TU context. These older
scores are not current-source replays. This batch tests a context reference,
not another standalone context-pointer declaration.

The limited adjacent-game identifier search finds no matching thread operation
in the selected TH08/TH09/TH095 ECL sources. This is neither an exhaustive
architecture claim nor TH10 behavioral evidence.

Private recovery: `.analysis/gpt-6.1-sol/ecl-thread-registers-20261022/RECOVERY.md`.
Retain the current canonical baseline, smallest shared-flags failure and compact
patches/reports. The evidence records final validation and classified cleanup.
Windows product/runtime closure remains open; semantic reconstruction and porting
have not started.
