# Complete ANM parent bindings and child request record

2026-10-24, starting checkpoint 3d3639d. The actual maintained ExecuteScript
now compares **9960/9964 bytes**, with all 350 independently bound fields and
95 unchanged local/table destinations. Four ordinary bytes remain different;
no exact credit is granted. Header layouts, ABI declarations, compiler profiles,
canonical recipes, boundary/origin classifications and exact counts are unchanged.

## Maintained source change

The two flip cases use a fully initialized four-byte flag value whose axis group
occupies bits 9/10. XOR 1/2 in that group preserves the other bits; the final
word still sets update bit 3 and scale still multiplies by -1. The two-bit view
is local to this source and asserts no original VM declaration or data owner.
Adjacent TH09 suggested the grouping only; every TH10 mask comes from TH10.

Child91 captures its actual script and layer in a two-member argument record,
then consumes both in the unchanged CreateVmVariant2 call. No unused members,
dummy inputs, inert stores, fake returns, padding, register directives or ABI
annotations are introduced. The record recovers the target's push-script-before-
load-file scheduling. Its layer still uses ECX where target uses EDX.
The getter remains a read-only VM operand resolver and is evaluated once.

The selected private model and a distinct compile of the actual maintained file
produce all 352 complete public PDB functions raw-identically. Against the prior
ten-input image, only this parent changes ordinary bytes; the other 351 are
raw-identical. The case diagnostic improves 7541/7995, 90/92 sized groups, gap 2
to 7990/7994, 92/92, gap 0. This case score grants no independent credit.
See the [full parent proof](current-parent-proof.json) and
[preservation scope](preservation-summary.json).

## Independent full target-field contract

The [contract](binding-contract.json) covers the complete reviewed owner
43EE30..44151B: 9588 decoded bytes, including one NOP, then 94 absolute table
words. The target-only decoder independently enumerates fields before comparing
its slots/types/widths to the candidate. All 350 agree. Every local destination
is independently read from target instructions/table words and must retain the
same relative offset; no moved local label is normalized away.

235 occurrences reuse independent canonical semantic bindings. Fifteen vector
component references explicitly use the full 12-byte zero-vector or offset-delta
base plus addend 0/4/8. Four-byte diagnostic anchors can overlap neighboring
names; those neighbors never replace the genuine vector base. Twelve zero-vector
loads serve four tangent copies; three delta loads serve flag13 offset updates.
The target vectors are zero-filled writable image memory, corroborated by fresh
Ghidra reads, not invented file-backed bytes or recovered production data owners.

Five remaining call occurrences use three newly reviewed callee identities:
timer Decrement, interpolation ResetTimer and CRT __CIfmod. Together with the two
vector bases these are the five formerly unresolved semantic identities.
[The identity audit](identity-audit.json) records their actual evidence and limits:

- Decrement's complete target 20-byte body negates the incoming EAX integer,
  converts it to float and invokes canonical Add at44BF40/88 with ESI timer.
  The native68-byte helper inlines the Add work and remains nonexact. Its source
  role/private incoming values establish an address identity, not child equality.
- ResetTimer's complete50-byte native body matches the reviewed4050D0 body
  after its single independently canonical476F78 game-speed pointer is replayed.
  This grants no duplicate helper credit or original class/TU identity.
- __CIfmod uses pinned libcmt.lib member87fmod.obj, SHA-256
  28e8c7d48b2645df3cc8ad4a5343d3ad1c2f326e1b71ada86059877e94901f51.
  COFF definition auxiliaries independently supply10/10/30-byte functions in
  the complete50-byte text section. All26 nonrelocation bytes match; both wrapper
  references select the80-byte descriptor at477010. Its16 nonpointer bytes
  identify fmod, all16 pointer slots preserve repeated-symbol identities, and
  fFMOD points to the complete kernel at463BC4. The known adjust_fdiv field also
  agrees with a separate canonical unit. This corroborates the caller's CIfmod
  identity; no standalone CRT match, new source mapping or library-owner claim.

The unchanged strict linked comparator replays every declared field and compares
every byte. The structural350-field inventory alone was insufficient; the first
complete bound replay gives9952/9964, followed by9960 with the request record.
There is no target byte copy, changed masking policy or relaxed comparator.

## Corrected residual attribution

| Offset, hexadecimal | Remaining difference |
| --- | --- |
| +113, +115 | Child91 layer MOV/PUSH uses ECX instead of EDX |
| +249A, +24A0 | Second radial vertex Z translation swaps position.z and positionOffset.z loads |

The old cleanup handoff called the latter pair slow Tick. Fresh full-offset
disassembly/Ghidra instead places them at4412C8/4412CE, inside the radial loop.
That attribution was wrong; the actual baseline slow Tick is already equal.
This correction changes the next investigation and invalidates a timer-only
interpretation of the earlier neutral sum controls. The borrowed-layer control
does alter real slow-Tick bytes at+2548..+254B, demonstrating that the two regions
must be kept distinct.

## Completed controls and preservation

[The control record](control-summary.json) describes23 actual private ANM compiles,
plus actual maintained ANM compiles in the ten-input and Player17-input graphs.
An additional eleven-object real-Player caller relink compiles no game source;
it retains9952 and passes36 guards/ECL6995. No full independent cold pair occurs.

Three returned-ID construction models regress or fail the fixed field inventory.
Four whole-vector spellings, two vector-copy accumulations, uniform records in
all four child cases, five record construction/storage forms, two XY/depth
translations, one borrowed-layer form and four immutable/value-copy record forms
do not beat9960. In particular whole-vector reversal fixes Z but reverses X/Y;
separating XY/depth changes stack homes or the full extent. The scalar constituent
differences are not independently patchable bytes. Prepared work is not counted
as execution; all23 recorded controls reached terminal source/guard/route checks.

All36 same-image guards pass4618 bytes/all121 fields. ECL remains6995/7020 with
all189 fields/62 locals, complete171-byte callee and all16 guards1617/all21.
The actual Player17-input/52-project-file graph separately recompiles this ANM
source, reuses16 pinned game objects and preserves Player1380/1435/all49 plus
eight guards1753/all99. Across519 public owners, the other518 are raw-identical.
Old global ECL/Player manifests are left immutable; use the new actual-source
current manifest/helper rather than loosening their now-historical source pins.

## Host input pin and CI recovery

The first CI invocation completes89 stages, then stops at stage90 because the
transition-effect host suite pins the entire old AnmManager.cpp. Its only
extracted body from that file is AnmRandomU32InRange, which is unchanged.
[The review](host-pin-review.json) verifies an exact full-file inverse back to
the old source, the identical extracted helper and all six other dependencies.
The entire regenerated host harness and every extracted dependency hash also
match the preceding passing CI, independently retained in its cleanup receipts.

Only the declared LF-normalized file hash is refreshed, with a reviewed-update
record. Historical raw provenance, strict normalization, rejection behavior and
all test code remain unchanged. The failed CI log is retained. Resume explicitly
reuses the89 completed stages with unchanged relevant inputs and executes90..111,
including the host baseline and all19 EOL/pin negatives. This is combined111-stage
coverage, not a claim that the first full invocation returned zero.

## Resume

Current producers, source snapshots, target-field inventory and complete reports
live under `.analysis/gpt-6.1-sol/anm-full-parent-bindings-20261024`.
Expanded carriers are the selected child91-argument-record, maintained-current
and maintained-player-17 below the corresponding build root. See the
[artifact index](../../RE_ARTIFACTS.md) for archived closed controls.

```sh
scripts/repo-python .analysis/gpt-6.1-sol/anm-full-parent-bindings-20261024/compare-current.py
```

This pins actual maintained sources/artifacts and all contracts. Its original
compilation manifest remains immutable; the current manifest separately records
the later noncompiled function-ledger note and reviewed host-input-pin changes. The helper replays retained
images and grants no independent-cold, exact-acceptance or runtime credit.
Continue on the complete large parent and its two residual source regions.
Original record/class spelling, private register-allocation cause, production
TU/profile/data owners, native product and runtime remain open. Semantic
reconstruction and portability have not started.

[Validation](validation-summary.json) and [cleanup](cleanup-summary.json) record
the completed checkpoint gates and bounded artifact handling.

## Closed-control compaction

Twenty-three closed images/private proposals/bulky diagnostics,573 files, are
losslessly archived:53.87MB become0.59MB,reclaiming53.29MB. Every member is read
back and checked for length/SHA-256 before removal. All23680 protected hashes
agree during compaction. The selected private, actual maintained and actual
Player17 carriers remain expanded, as do all earlier before/support inputs,
source plans/producers/logs, target/Ghidra/CRT binding evidence and archives.
All nine unrelated D3D files remain untracked/unstaged/hash-unchanged.

```sh
tar -tJf .analysis/anm-child-request-cleanup-20261024/closed-controls.tar.xz
tar -xJf .analysis/anm-child-request-cleanup-20261024/closed-controls.tar.xz -C . EXACT_MEMBER_PATH
```

The archives/inventories are ignored workspace state. Restore required private
inputs/reports before reusing an old producer; never reset current maintained
source or weaken historical pins merely to rerun an old image.

Successful CI host outputs and Python bytecode are subsequently cleared as
[reproducible caches](ci-cache-cleanup.json); their manifests/hashes are retained
compressed locally. This removes10.24MB of generated payload and no native
reconstruction carrier. It includes new validation output and is separate from
the closed-control archive saving.
