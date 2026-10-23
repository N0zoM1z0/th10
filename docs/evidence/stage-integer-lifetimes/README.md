# Stage dispatcher integer-lifetime controls

2026-10-23, source checkpoint 2b03d2b. Recover the four completed controls before
handoff; no new reconstruction experiment or maintained source change. The
complete StageExecuteScript 403C80/1868 remains nonexact.

## Observations and source forms

Target opcode 12 starts 4041CE with a DL load/store; the maintained native starts
40175E with CL. Both inline the complete timer SetCurrent(0) sequence here.
Target opcode 13 at 404202 loads EAX and uses the five-byte accumulator store to
4923A8. Maintained 401792 uses **EDX**, producing an eleven-byte group versus ten.
The former handoff's ECX observation was incorrect and is corrected.

These target observations are from the preceding direct readOnly/noanalysis
Ghidra review and hash-verified target, with full mapped 412289-byte digest
cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f.
They describe code generation, without proving original types, ownership or ABI.

| Actual control | Case matched / comparable | Complete parent versus maintained baseline |
| --- | --- | --- |
| Compute opcode 12's used timer receiver before the mode store | 1443/1619 | All 1868 raw bytes equal |
| Share one int operand local between 12/13, assigned only in each selected case | 1439/1619 | Four ordinary byte differences |
| Opcode 13 calls an ordinary static void global-store helper | 1443/1619 | Equal after 20 independently identified address-field replays |
| Opcode 13 copies the same four-byte word with memcpy | 1443/1619 | All 1868 raw bytes equal |

All four preserve 1868 bytes, the complete 15-entry table/domain/physical order,
gap 2 and 13/15 target-sized groups. All 11 complete guard contracts pass 987 bytes
and all 36 guard fields in each image. Each control compiles only StageScript,
retains /GL /GS and reuses eight pinned game objects plus two harness inputs.
The helper remains a real source function; no target bytes, padding, fake
results or ABI annotations are introduced. Its 20 changed fields bind identical
semantic symbols or identical local destination offsets before replay. No
ordinary bytes are normalized. This proves native peer equality only.

The source-declared `global-forceinline` control was not executed. The global-copy
launch lost its original exec handle; its final source-asserted report, log and
all artifact hashes are complete and no producer remains live. A direct process
exit code for that run is not claimed. The other three prior terminal exits
were observed as 0. Details and all ordinary differences are in
[evidence.json](evidence.json); adjacent patches preserve each actual source form.

## Closure

Before cleanup, a fresh replay of the current canonical Stage image passes all
11 whole guards covering 987 bytes/all 36 fields. The 1868-byte parent hash remains
c1984eb7ab1703cc428d6659507d841f3ae7e7ebe04cfa094e8f07bd18685b26.
No new game compilation is needed. No control is adopted and no exact claim or
ledger count changes. Hash verification precedes removal of all 60 private native files,
2705612 bytes, retaining source copies, patches, producers and reports. The
[cleanup receipt](../handoff-cleanup-20261023/README.md) records restoration and
preserved inputs. Next try a different value lifetime or remaining spline
schedule; do not repeat these controls or infer whole-parent equality to target.
