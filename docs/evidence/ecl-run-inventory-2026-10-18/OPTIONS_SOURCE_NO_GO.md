# UpdateOptions source-only triage: parked

Date: 2026-10-18. Parent-reported clean HEAD: 65e3fd3c127c7efe190ed9ac2cd9b82d31bd2725, corroborated by the saved read-only repository receipt.

## Decision

**1128/3188 is stale. Do not initiate a missing-large-source expansion or fresh build from that diagnostic.** The latest authored checkpoint is 3276/3188 physical bytes, an 88-byte excess, still nonexact. This is historical diagnostic evidence, not a newly measured build or exact-byte credit. The remaining input/timer/adapter knowledge gap is explicitly parked. No reopening is recommended without new concrete source/call-ownership evidence.

## Durable repository provenance

The parent obtained source-only authored note matches in `remote-authored-notes.json`, command `repository-command:2c745fe0bb5a4d6ba06f580050d9bd27`. That receipt records unchanged clean HEAD above. Its `stdout.text` contains the following repository paths, hashes, and line matches:

- `docs/KNOWLEDGE_BASE.md`, SHA256 `6d8396e6f4e82bf8304c809e2ba4172e4ecec266f4832e8a1657553d6194f09b`: lines 1523–1524 record the OLD 1128/3188 controller and 2124/3999 refresh; line 1559 records four-site expansion; line 1576 records 3228/3188; line 1638 records 3280/3188 after close restoration; line 1677 records the direct-member input control; line 1745 explicitly records the LATEST 3276/3188 nonexact result. Line 1784 records the hintMode/SetHintModeSprites naming update.
- `docs/RE_HANDOFF.md`, SHA256 `4af5d1ed7c58973587450d99c0cdfeaec239cb34ce947eec9a299fbd4752144b`: line 1275 is the OLD 1128 result; line 1297 gives 3228; lines 1315–1318 give direct closure and 3280; line 1345 gives latest 3276; line 1367 explicitly parks the remaining input/timer/adapter knowledge gap.

The locally authored snapshot `/workspace/shared/input-reconstruction-plan/REMOTE_HANDOFF_SNAPSHOT.md` independently contains corresponding progression at lines 1281, 1303, 1324 and 1351. Its line numbers differ from the repository document: do not substitute those for repository citations. Lines 1348–1351 explain that direct-input binding was tested and reverted because all four sites inlined rather than retaining the target two-call boundary, and timer/create/interrupt audits found no omitted helper effects.

## Maintained source corroboration and preservation boundary

All source files used were SHA256-checked against the supplied manifest; exact pins are in `source-pins.json`. `src/FrontEnd.cpp` is `c26c9c1c00b3caa98a01fc210efdbb898fe6e8cace5a4d84a8072cd3f8a90d5d`. Canonical config SHA256 is `8811679fb37ab220216a8926a9ce46b8637335e47cea7cd8d5d8a78b6004cc88`.

- FrontEnd.cpp:564–608: existing expanded child-ID and hint-sprite helpers. Four UpdateOptions call sites exist at lines 950, 1010, 1040 and 1071. The three-way helper has six lexical lookup expressions, so source already exposes 24 across those four expansion sites; this does not assert emitted native duplication or dynamic execution count.
- FrontEnd.cpp:932–1107: controller state protocol and direct close statements are already maintained; no missing state body was identified.
- FrontEnd.cpp:1110–1229: preserve the exact RefreshOptionsDisplay owner and its shared lookup source.
- AnmVmId.cpp:106–117: typed SetSprite uses the current global manager, distinct from the cached parent manager in FindOptionsChildId. Do not replace that seam with direct VM sprite access or modify existing attributes as a tuning exercise.

## Re-entry gate

Only revisit if new evidence identifies one specific unresolved source/helper boundary under the current pinned graph. Read existing authored diagnostic summaries before requesting a new build. Do not repeat the direct-input control, broaden helper expansion, alter ABI/profile/TU context, or force allocation. The large-deficit premise has been disproved by superseding authored history.

This audit made no source/config edits, native builds, target/native artifact reads, remote calls, or external reference imports. Only this compact local report and source-pin record were written on the parent's request.
