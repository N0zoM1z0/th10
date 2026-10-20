# ECL Run table coverage and current dependencies, 2026-10-21

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Starting checkpoint: `ccd47a6`. No production source change or new exact credit.
The three original large owners remain nonexact.

## Repaired diagnostic coverage

A current-dependency source investigation exposed a false rejection in the
complete Run diagnostic. Moving a preceding callee changes absolute jump-table
addresses. Capstone may then stop at byte6692, rather than accidentally decoding
table data through byte7019. The old fixed one-byte selector annotation cannot
cover that case.

Run owns6692 pre-table bytes, sixty four-byte absolute jump entries, then88
selector bytes:7020 total. The diagnostic now validates both unique dispatch
references, all sixty contiguous PE fields, their code destinations and the
selector domain. It annotates only the remaining undecoded selector suffix.
Stopping inside the executable body or changing the reviewed table layout is
unsupported. The189-field/62-local binding contract remains unchanged.

The linked comparator can now combine complete authoritative PE DIR32 fields
with explicit trailing unlinked data annotations. Annotations remain contiguous,
inside the full extent, disjoint from decoded bytes and linked fields. Missing
coverage, partial words and REL32 fields cannot bridge a gap. Actual fields
must still equal the declared manifest and pass every semantic destination
check. Every owned byte is compared; annotations do not mask data.

Sixteen target-independent tests cover the coverage rule and Run table layout,
including a synthetic PE/MAP/PDB full-owner comparison. Changing its unlinked
byte produces23/24, not a pass. Real diagnostic controls operate only on a
temporary copied native image: a selector change inside the valid domain adds
one ordinary difference (6994->6993/7020), a shifted local table destination is
rejected, and an incomplete selector annotation is rejected. Original target
and native carrier hashes remain unchanged. See [controls](oracle-controls.json).

## Fresh compiler measurements

The [native receipt](native-summary.json) records a full two-TU /GL cold build
of EclVm.cpp plus Enemy.cpp, with the real EnemyRuntimeUpdate entry. All
recursive input pins include the latest ANM header. Run retains7020 bytes and
exactly26 ordinary differences after all189 fields are bound. Sixty-two local
fields and all unlinked selector bytes participate. Its baseline decoder still
stops at7019; the repaired annotation is one byte in this particular image.

Fifteen complete existing owners retain1587 bytes/all21 fields in this actual
image, including HostRun146, SpawnThread142, four stack operations and
FindSubroutine136. These are additional same-image guards. Existing canonical
recipes and credit are unchanged. The only canonical data_spans user, EnemySpawn,
cold-replays all577 bytes/all12 fields in its14-TU graph; see
[its receipt](canonical-enemy-spawn.json). Production inputs are restored and
their frozen hashes match. No complete historical exact-unit replay was run.

Fresh related-owner extents supersede stale notes: StartSubroutine is551/550,
not the old522/550 measurement. AddScriptData remains547/589. These are complete
PDB extents and diagnostic observations, not exact percentages or partial credit.
The [target facts](target-facts.json) include read-only/noanalysis local Ghidra
review, which confirms StartSubroutine's contiguous550-byte
body and its two callers at44E360/450154. Complete inclusive412289-byte mapped
text is equal before/after review, with transaction rollback, maxcpu1/heap768m.

## Source controls, all restored

| Control | Complete Run differences | StartSubroutine bytes |
| --- | ---: | ---: |
| Production baseline | 26 | 551 |
| Explicit StopThread context local | 26 | 551 |
| Two shared float operands | 30 | 551 |
| Shared length Y / angle DY | 26 | 551 |
| Explicit-context static Run | 26 | 551 |
| Char stack storage | 26 | 551 |
| Integer conversion first | 26 | 543 |
| Above, primitive float temporary | 26 | 584 |
| Argument pointer initialized first | 26 | 551 |
| Integer-first + typed destination slots | 26 | 543 |
| Integer-first + primitive integer temporary | 26 | 568 |
| Destination-type switch | 26 | 551 |

Typed destination slots also initialize the argument pointer first. All other
controls start from baseline except the explicitly combined integer-first variants.
The explicit-context Run has the same complete raw owner hash as baseline;
this does not identify the original C++ declaration. Char storage does not
establish original element signedness. Shared floats remove two angle-slot
differences but introduce six raw-negation slot differences. The residual
+0129/+012B instructions belong to StopThread after FindThread; StopAllThreads
is a separate retained CALL and is not the cause of that interval.

Target StartSubroutine initializes the destination pointer before the value
cursor and puts integer-to-float conversion before the direct integer copy.
Reordering declarations fixes the initial instruction schedule but changes
other register choices. Reversing the conversion branch enables tail merging;
primitive scalar variants diverge further. These are scoped compiler results,
not global prohibitions or proof of original source types. All source/API/type
controls are restored, and their input snapshots remain in the private pack.

## Reproduction

```sh
scripts/repo-python scripts/build-match-unit.py --unit ecl-vm-host-run
scripts/repo-python scripts/diagnose-ecl-run-native.py
scripts/repo-python scripts/test-linked-data-coverage.py
```

The diagnostic returns1 for the ordinary mismatch,0 only for full equality,
and2 for unsupported/unbound evidence. It never grants canonical exact credit.
Private runners and source controls are under
`.analysis/gpt-6.1-sol/ecl-run-thread-stack-20261021/`; runners do not restore
source. The retained current baseline carrier is named in the receipt. Compare
all input/artifact pins before reuse. Superseded owned outputs are pruned after
retaining compact evidence. Product, runtime, semantic and portability gates
remain open. Final checks and cleanup are in [validation](validation-summary.json).
