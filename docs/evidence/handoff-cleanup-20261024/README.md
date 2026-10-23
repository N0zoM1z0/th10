# Cleanup and recovered large-owner handoff — 2026-10-24

The operator requested code/document cleanup and a smaller handoff workspace.
Starting HEAD is 7dd1c6d. Maintained C/C++, headers, compiler profiles, recipes,
target identities and exact ledgers are unchanged. Reconstruction remains
active-incomplete. Commit prefix is `gpt-6-astra-medium: ... ~`; local only.

## Recovered work

[The control inventory](controls.json) records six completed ECL images and
21 completed ANM images from the preceding, undocumented private batch. Each
compiles one actual game object and reuses nine pinned game objects; no full
independent cold pair occurs. Its original manifests, plans, producers and logs
remain immutable. Recovery verifies source/object/image/map/PDB hashes and the
recorded terminal results before classifying outputs for compaction. No game
source change or new native reconstruction build is made in this cleanup.

| Actual control family | Recorded outcome |
| --- | --- |
| Five ECL inline-depth/definition controls | Full Run 6995/7020 unchanged; 9 guards 1137 / all 18; all 352 public PDB owners raw-identical |
| ECL unchanged-source `/Ob1` profile control | Run 4356; strict complete 7020-byte extent refusal; guards do not all pass |
| Seven scalar/reference/getter ANM flip controls | Full 9964-byte contribution; case 7541/7995, 90/92 sized groups, gap 2; no improvement |
| Two-bit local word/scalar-return/shared-word ANM models | Full 9964-byte contribution; case 7982/7994, 92/92, gap 0 |
| Two-bit union value returned by value | Case 7980/7994; 92/92, gap 0 |
| Eight later field/child/timer controls | Reproduce 7982/7994; 92/92, gap 0 |
| Child file reference / file snapshot | 7980/7994 with gap 0 / 7475/7995 with gap 2 |

All 21 ANM images retain 36 complete guard bodies covering 4618 bytes / all 121
fields, strict ECL 6995/7020 with 189 fields and the 171-byte callee exact, plus
the bounded child call routes.
Those checks do not grant ANM exactness or identify original classes/TUs.
Only the first seven, byte-neutral ANM controls have completed 352-owner peer
proofs; the promising two-bit candidate still needs its own complete peer audit.

## Retained ANM continuation witness

Expanded private source:
`.analysis/gpt-6.1-sol/anm-flag-return-context-20261023/flip-two-bit-local/AnmManager.cpp`.
Image/map/PDB/objects:
`build/gpt-6.1-sol/anm-flag-return-context-20261023/flip-two-bit-local`.
All its reports, guard contracts, source plan, hashes and compiler inputs remain
expanded. Its source SHA is 4d78ced7eb3a84e3b154ff0b91a8a53757852dcf141634de5389926df23dabd8;
image SHA is b5b1b67ac040b91cdcb5d0b5a4759b2bc9939fdd66b038b0469707c134defebf.

The source-local four-byte union models a fully initialized flag word with a
two-bit axis group at bits 9/10. Flip X/Y XOR 1/2 in that group, preserve all other
bits, set update bit 3 and retain the scale multiplication. No shared header,
object layout or ABI changes. Adjacent TH09's two-bit flip spelling supplied
the hypothesis; it supplies no TH10 mask, address, layout or ownership evidence.
The source view is not an assertion about the original class declaration.

[The fresh retained-carrier replay](candidate-validation.json) reproduces the
complete 9964-byte PDB contribution, 9588-byte decoded span and 94-word trailing
table. Structural inventory has 350 four-byte fields, leaving 8564 ordinary
positions; 8552 match. The 12 structural differences are +113, +115..+11D in
child91 argument preparation and +249A/+24A0 in the second radial vertex Z addition.
The earlier slow-Tick attribution was corrected by the later
[full-parent audit](../anm-child-request-record/README.md); those two offsets
are radial-vector code. Offsets are hexadecimal. This is **structural diagnosis only**; the complete
semantic address replay remains unfinished. All 350 fields need independently
justified target bindings, including 95 local/table references.
Coincident data anchors must use the proper
semantic base and addend rather than an unrelated neighboring symbol.

The previous readonly Ghidra review and pinned CRT archive scan remain private
in the same campaign. Review their five unresolved semantic identities:
timer Decrement → 428DD0 (native helper 68 bytes versus target 20; nonexact),
interpolation ResetTimer → 4050D0, zero vector → 491C14,
position-offset delta → 491E6C and CRT __CIfmod → 463BBA.
The pinned 87fmod.obj member remains at
`build/gpt-6.1-sol/anm-flag-return-context-20261023/binding-archive/87fmod.obj`.
These are binding leads, not a finished 350-field contract or helper
exactness claims. No small helper should be promoted merely to finish the parent.

Next work: independently bind every field and local destination through the
unchanged strict linked comparator, audit all complete public peers, then test
real caller visibility for child91 and genuine timer expression/context
hypotheses. If the candidate is adopted, compile actual maintained source and
refresh its provenance without weakening historical pins. Exact acceptance
additionally requires zero differences and independent complete cold builds.
The real Player caller graph is a hypothesis to inspect, not an executed control
in this batch. ECL/Enemy/Stage/Player maintained state is unchanged.

## Compaction and restoration

[The receipt](cleanup-summary.json) covers 26 closed controls / 598 files:
59130719 payload bytes become 609532 bytes, reclaiming 58521187 bytes
(58.52 decimal MB). Every archive member is read back and checked for length
and SHA-256 before its original is removed. All 23412 protected file hashes agree
before and after compaction. Allocated-space saving is recorded separately;
these numbers exclude earlier cleanups and the small new manifests/docs.

Current and selected ECL/Player carriers, canonical outputs, the selected ANM
witness, all older support caches and archives remain. All nine unrelated D3D
files remain untracked/unstaged/hash-unchanged. Original resources, tools/Wine,
the Ghidra database and unclassified legacy directories remain. No tree is
deleted by age and no broad clean/reset is used.

The private compactor, plan, protected hashes, archive inventory and archive live
under `.analysis/handoff-cleanup-20261024`. Restore only required original member
paths from the repository root before using an archived control. Plans/producers
remain expanded, but their archived private inputs/reports must be restored.

```sh
tar -tJf .analysis/handoff-cleanup-20261024/closed-ecl-anm-controls.tar.xz
tar -xJf .analysis/handoff-cleanup-20261024/closed-ecl-anm-controls.tar.xz -C . EXACT_MEMBER_PATH
```

Archives are ignored local evidence and do not travel with a Git clone. Transfer
the retained workspace plus archives/inventories when those receipts are needed.
Never restore historical maintained code into src/ or change old source pins.
See the [artifact index](../../RE_ARTIFACTS.md) and [current handoff](../../RE_HANDOFF.md).
[Validation](validation-summary.json) records the post-compaction retained ECL/Player
and ANM candidate replays, toolchain/tracking/progress/documentation checks, and
all 111 public CI stages passing. This includes host contracts and does not
constitute target runtime validation. Afterward, [CI cache cleanup](ci-cache-cleanup.json)
removes 468 known generated files: 10.24 MB of Python bytecode and four successful
host-CI output directories. Their inventories and four host receipts remain
compressed locally; reconstruction PE/map/PDB/object carriers are untouched.
The cache figure includes outputs regenerated during validation and is separate
from archive savings; it is not a net workspace reduction measurement.
No new exact, product, runtime, semantic or portability completion is claimed.
