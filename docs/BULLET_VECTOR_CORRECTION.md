# Vector-angle NaN correction — 2026-10-18

## Retained semantic correction

The remaining angle helper now uses ordered <= -990 and >= 990 comparisons, otherwise retaining the literal requested angle. This corrects the proved vector unordered-input path: NaN is retained without calling EnemyAngleFromPlayer. The already-corrected direction block is byte-identical. Only the helper comparison/return block changes; no ownership refactor, timer/ABI change or compilation directive is introduced.

Source SHA256 is 72aa46821fdae7f35482a30fcfe8abbd48b075a75489f593d922e9ad2ab131d1 (raw LF). The single-block reversal to prior b635 source is recorded in [vector-source-change.json](evidence/bullet-transform-native-2026-10-18/vector-source-change.json). Production source attributes are unchanged. Normal C++ checkout may use CRLF; host canonicalization is separate from native raw-byte pins.

The reviewed valid-domain vector-prefix evidence is command repository-command:6e7a037fdda14bc9914a41683e8dc1ec. Its exact collector, authored synthetic tests and [audit](../probes/bullet-transform/vector-replay/AUDIT.md) remain frozen with historical pins. Preserve its premises and domain limits: this is not universal dispatch/state or full-transform equivalence, nor a signaling-NaN/payload-preservation theorem. Host baseline and 32 compiled semantic mutation controls pass, including vector NaN literal/lazy-aim behavior and unchanged direction behavior. Finite tests and target proof remain distinct evidence.

## A005 native result

Command repository-command:23e13cd48e2e40069a852073dce3d2c0 completed one isolated canonical cold_link in 23.695 seconds. The canonical runtime guard normalized exactly 885/885 bytes. Production remained clean at 1ace9bb5c34811a6d5911864cbe75599b47966f2; all source/input postpins and required full-owner attribution gates passed.

The full transform contribution remains 1540 bytes against target 1454, with SHA256 72917a0ae941e4e20a0418c93db0bf9810446934d2acf38c7df03d9bc552fb52, identical to the direction-corrected R001 owner. This does not claim the helper, whole image or every artifact is unchanged. Tables/data are included without cropping. The 86-byte target excess remains; transform exactness was not tested and no transform acceptance credit is added. Source is retained for the semantic evidence above. No game/runtime execution occurred.

The compilation graph/profile/entry and sole normal-support boundary were unchanged. The explicit candidate-only harness delta temporarily validates windows_path results before canonical argv construction: success, nonempty single-line ASCII, no NUL and exact expected resolved path. It restores the prior callback afterward, makes no tracked driver edit and adds no retry. This fail-closed check does not prove an empty historical argument caused A004's earlier '.obj' failure. Full-build resource policy remained one CPU, nice+10, inherited per-process CPU 120 seconds, worker wall 150 seconds and no introduced AS cap.

## Archival stability

[A005 metadata](evidence/bullet-transform-native-2026-10-18/a005-result.json) retains artifact/owner/tool-packet identities. The [recipe archive](../probes/bullet-transform/native-experiment/recovery-history/README.md) now derives A004/R001 b635 and A005 72aa source bytes from the already-archived A003 da1 overlay using small hash-checked source splices. It never depends on current production source. Exact historical A004/R001 runner/manifest/overlay identities remain unchanged, and A004 failure remains separate from R001 recovery.

Reconstruction is offline only; historical ready flags do not grant execution permission. Native objects, images, PDB/MAP files and raw logs are excluded from this checkpoint.
