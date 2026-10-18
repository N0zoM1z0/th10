# Bullet transform native experiments — 2026-10-18

## Result and decision

The two-site angle expansion experiment was not adopted. A003 produced a complete 1596-byte transform contribution, up from A002's 1476 bytes and above the target's 1454 bytes. The increase is 120 bytes versus baseline; the target excess is 142 bytes. Both runs retained the canonical normalized exact 885/885-byte runtime guard. Production remained clean at e94559cc95d583b702660b72bbc605e5dfc5743a with input pins valid. No production source, match-unit or acceptance ledger change follows.

A003 used one canonical cold link and completed its runner in 79.209 seconds (the outer command includes preparation overhead). Command: repository-command:c63842fcba7c410899230148295c07fe. The sole overlay expanded the artificial static angle helper's expressions at its two source call sites, SHA256 da1d560622c704618397099410ec680d08349f891dc2a87cc3d46bcea653af3f. Separate source checks covered 40 float cases and 11 mutation controls. Those finite checks do not establish target equivalence.

Full owner contribution measurements include inline tables/data without cropping. Transform exactness was not tested in either run, and no transform acceptance credit is claimed. Size alone would not establish exactness even if it matched. No game/runtime execution occurred.

## Audit and baseline

[BULLET_CHILD_TRANSFORM_AUDIT.md](BULLET_CHILD_TRANSFORM_AUDIT.md) preserves the reviewed conditional descriptor-field and live post-spawn cursor findings, their explicit premises, and open obligations. Those findings do not prove full transform-program equivalence.

A002 command repository-command:ce709107cc9a4d9bbe127bd446ce05cd completed one cold link in 74.824 seconds. Its complete transform contribution was 1476 bytes, 22 above target. All 83 raw source inputs and tool/config pins were checked before/after. Its canonical runtime comparison normalized exactly 885/885 bytes; raw linked bytes differ under relocation.

Subsequent read-only A002 normal-object metadata identified unique transform/helper contributions of 1476/68 bytes. Two named helper-target REL32 fields had preceding E8 forms; zero other named helper references were found. This is bounded relocation/source-ownership evidence. An E8 form may be embedded data or part of another instruction: instruction boundaries and control flow were not proved. Alongside the two authored source calls, it motivated the conditional A003 experiment.

## Operational recovery

A001 is preserved as a terminal failed run. It entered cold_link but failed primary compilation with “wine: could not exec wineserver”, producing no primary object or linked artifacts. This does not establish that a compiler process never launched.

Version-only probes succeeded with and without the introduced 2 GiB RLIMIT_AS cap. An existing-prefix winepath probe failed with that cap and succeeded with inherited unlimited address space; prefix registry hashes remained unchanged. Recovery stopped imposing that virtual-address cap, retaining CPU 120 seconds, wall 150 seconds, one CPU and nice +10. The inherited unlimited limit was required and left unchanged. Virtual-address capacity differs from resident RAM; no measured-RAM claim is made. Shared-server reuse was a probe caveat, so the observation is operational evidence rather than an isolated causal theorem.

Recovery also preserved separate worker receipts and worker/supervisor/cleanup failure classes. No failed run was restarted or rewritten.

## Fixed scope and retained evidence

Primary BulletRuntime.cpp; ordered supports BulletManager.cpp, BulletTransform.cpp, PlayerCollision.cpp, AnmManager.cpp, EnemyEclDispatcher.cpp and Enemy.cpp. Only BulletTransform.cpp was normal support (support-01-BulletTransform.normal.obj); the rest retained /GL. Canonical flags, entry and link harness were unchanged. A003 changed only the isolated candidate's pinned BulletTransform.cpp; production always retained the original base hash.

Evidence is in [evidence/bullet-transform-native-2026-10-18](evidence/bullet-transform-native-2026-10-18). One full A003 manifest retains shared source/closure/config data; the compact baseline references those identical sections. The exact A002 manifest can be reconstructed from a hash-checked delta. Source-only recipes/tests are in [../probes/bullet-transform/native-experiment](../probes/bullet-transform/native-experiment). Historical packets retain their original pins and paths; archival execution_ready fields do not authorize further execution. Native artifacts and raw logs remain outside this checkpoint.
