# Direction-angle NaN correction — 2026-10-18

## Retained source correction

The direction-only correction is retained for its semantic evidence. Shared direction actions now test requested float0 with ordered <= -990 and >= 990 comparisons, otherwise retaining the literal angle. An unordered input therefore follows the literal path rather than calling EnemyAngleFromPlayer. The vector call site and original helper remain unchanged. Finite thresholds, signatures, timer logic and other source bytes are unchanged. The exact raw LF source SHA256 is b635d17c1f7bc76d3af8fc7b9fd4453f6c32b0e1f90fd6f718a4e6a7460516c0; the single-block reversal is recorded in evidence/direction-source-change.json under the existing dated evidence directory.

The reviewed target replay gives concrete valid index-zero witnesses for all three direction kinds, tying the NaN-store path to current-record float0 with freshness. Its explicit premises and unresolved universal dispatch/state coverage remain in [the frozen audit](../probes/bullet-transform/direction-replay/AUDIT.md). It does not prove full-transform equivalence, vector behavior, signaling-NaN exception behavior or NaN payload preservation. The frozen collector keeps its original historical HEAD/source pins and must not be silently repinned after this source integration.

The host suite passes its baseline and 28 compiled semantic mutation controls, including direction NaN/lazy-aim behavior and unchanged vector NaN aim behavior. This is finite source-behavior coverage, separate from target proof and native exactness.

## A004 failure remains terminal

A004 command repository-command:d2912f09ce6047bbb4302212ffd2a563 completed its compilation inputs but failed during linking after 79.697 seconds. Its third link log reported LNK1181 input '.obj'; no executable/MAP/PDB existed. Production remained clean and postrun pins valid. It was not restarted or rewritten.

Read-only diagnostics found all seven source objects, the anchor object/source and alias response file intact. The 203 alias lines validated, and all 12 regenerated Windows paths were exact/nonempty with unchanged prefix registry hashes. These later observations do not recover historical argv or prove an empty-path cause. The diagnostic history supports a controlled existing-object relink, not a source/profile change to address the linker failure.

## R001 guarded recovery

R001 command repository-command:9f6331a55f0f4bc787dc02be88034162 succeeded in 5.389 seconds using zero compiler calls, zero cold_link calls and exactly one linker invocation. It copied pinned existing objects/aliases into a fresh directory, validated/cached paths, and kept the canonical graph/profile/entry intact. A004's complete inventory remained unchanged.

The existing canonical runtime guard normalized exactly 885/885 bytes. Both full owner-attribution checks passed; production cleanliness and source/input postchecks passed. The transform contribution is 1540 bytes versus target 1454, an 86-byte excess, and 64 bytes larger than the 1476-byte A002 baseline. The rejected A003 two-site expansion had 1596 bytes. These are complete contributions including inline tables/data, without cropping.

The source correction is retained for the semantic reason above despite the larger native size. Transform exactness was not tested, and no transform unit or exactness credit is accepted. No game/runtime execution occurred. Native artifacts remain private outside the repository.

Resource limits were one CPU, nice+10, inherited per-process CPU 60 seconds, linker wall 90 seconds and supervisor worker wall 150 seconds, with no introduced virtual-address cap. CPU limits are not an aggregate group budget; no resident-memory measurement is claimed.

## Reproducibility

Compact A004/R001 terminal metadata lives in [the dated evidence directory](evidence/bullet-transform-native-2026-10-18). [Offline recipes](../probes/bullet-transform/native-experiment/recovery-history/README.md) reconstruct exact historical runner/manifest/overlay identities from the existing A003 packet using small hash-checked deltas. No duplicate native payload or full input manifest is needed. Production source attributes remain unchanged: the repository's existing *.cpp checkout policy may produce CRLF. The reviewed native overlay and Git blob use raw LF bytes, while the host manifest separately canonicalizes checkout line endings. Historical native recipes require the pinned raw LF source, recoverable from the integrated Git blob; a CRLF checkout is not silently treated as the same raw native input.
