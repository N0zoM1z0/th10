# TH10 reconstruction handoff

Updated 2026-10-22. Exact reconstruction is active under the operator's renewed
instruction to checkpoint and push bounded work. Historical investigations belong in
Git, [the knowledge base](KNOWLEDGE_BASE.md) and linked evidence packs. Older
reports describe their own source/build context, not the current baseline.

## Resume safely

- Use only the original Japanese TH10 v1.00a, ignored `resources/th10.exe`:
  SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
- Read `AGENTS.md`, [RE_WORKFLOW.md](RE_WORKFLOW.md) and [ORACLES.md](ORACLES.md),
  inspect tracked/untracked work and review the diff before editing.
- Preserve independent, unadopted `docs/evidence/d3d-adapter-format/` (nine
  untracked files). This batch neither changes nor stages that work.
- Use `scripts/repo-python` for repository Python. The operator requests direct
  local tools, no Factory MCP. Local Ghidra review is read-only/noanalysis with
  rollback, one CPU and 768 MiB heap; serialize native builds at nice10.
- Verify disk identity and complete mapped text before/after Ghidra metadata
  work: 401000..465A80 inclusive, 412289 bytes, SHA-256
  `cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
  The older 64A80-byte digest covers a truncated prefix.
- Operator-authorized checkpoints use English `gpt-6.1-sol: ...` commits and
  push to `origin/main` after validation. Replay only necessary affected units.

```sh
git status --short --branch
git diff --check
scripts/repo-python scripts/verify-target.py
scripts/repo-python scripts/verify-toolchain.py --execute
scripts/repo-python scripts/validate-tracking.py --require-target
scripts/repo-python scripts/report-reconstruction-status.py
```

## Current source and progress

[Generated progress](PROGRESS.md): 1776 reviewed candidates, 735 confirmed
authored functions / 273576 bytes, 1085 source mappings, 1237 exact functions
overall. Authored exact is **396 functions / 72283 bytes (26.42% provisional)**.
The [SVG](../resources/progress.svg) visibly fills 135.28 of 512 pixels.
There are 206 mapped authored nonexact functions and 523 indeterminate origins;
the complete-game denominator remains unknown. This batch adds no exact credit.

The latest source change is confined to `src/AnmManager.cpp`'s render-mode9
locals and count expressions. Grouped ordinary declarations and original
vertex-count bounds recover `LEA ESI,[EAX-1]`, test/spill/FILD scheduling and
remove a delayed ESI copy. Both current ten- and eleven-TU contexts retain
9964 bytes and improve aligned ANM diagnostic agreement from 7478 to
7541/7995. No header, ABI, layout, compiler profile, entry or recipe changes.
See [radial counter evidence](evidence/anm-radial-count/README.md) and ANM-114.

The preceding source change replaces two Enemy laser pattern-copy loops with
typed-array `memcpy`: dispatcher 14464 -> 14352 bytes, aggregate group-size gap
650 -> 566. See [laser-copy evidence](evidence/enemy-laser-pattern-copy/README.md).
The latest exact checkpoint closes two 65-byte shared pending helpers through
explicit ID-value construction; see [ID-value evidence](evidence/pending-id-value/README.md).
Those histories retain their original input pins and validation scope.

## Three large owners remain nonexact

The [fresh ECL storage controls](evidence/ecl-storage-controls/README.md)
independently reproduce the two-TU cold Run baseline:7020 bytes,26 ordinary
differences, all189 fields/62 locals and the complete171-byte float callee exact.
Grouped LengthSquared declarations, earlier FORMAT counter initialization and
reversed Pop pointer additions produce the identical raw Run body. Sharing one
integer result adds a difference at+AF, rather than the larger graph's+146E.
All four controls are restored; no exact credit or maintained source changes.
The baseline and selected failure remain under
`.analysis/gpt-6.1-sol/ecl-residual-storage-20261021/` and its matching build root;
three neutral carriers are pruned (771355 file bytes). Its manifest binds the
restored source, complete input graph, results and independent D3D hashes.
Do not infer original source spelling or TU ownership from these controls.

All measurements below were refreshed under the current dependencies. Aligned
positions and equal-sized groups are diagnostics, not partial exact credit.

| Owner / target | Current measurement | Remaining work |
| --- | --- | --- |
| ANM ExecuteScript 43EE30 / 9964 | 9964 bytes; 7541/7995 aligned positions; 90/92 target-sized groups; aggregate gap 2 | Child group91, float-index temporaries49/53/54, FlipX61, common-tail2 alignment and slow Tick operand order. |
| ECL Run 44E1A0 / 7020 | 7020 bytes; 6993 matched; all189 linked fields / 62 local destinations; 171-byte float callee exact | 27 ordinary differences: thread argument, SIB, stack homes and common latch. |
| Enemy DispatchEclInstruction 40E770 / 14416 | 14352 bytes; frame2C4; 19 sentinels; 181 selectors / 108 ordered groups; 42 target-sized groups; aggregate gap566 | Laser factory private ABI, tail sharing, packet lifetime and storage/register/stack scheduling. |

Run's generic decoder-normalization flag is false at the final selector byte;
strict table/selector annotations still cover all7020 bytes, 60 table words and
88 selectors. Keep the historical separate two-TU result (26 differences)
distinct from the current 27-difference graphs.

ANM's combined radial/flag control fixes slow Tick load order but reverses the
radial Z operands; it is rejected and retained as the smallest current failure.
A one-based count control is rejected; the additional prepared repeated-expression
variant was not compiled. Earlier [residual/data audit](evidence/anm-residual-data-audit/README.md)
records other rejected forms. No original source spelling or physical TU is proven.

The current bounded data audit validates 45 ordered absolute-memory operands
inside 92 ANM spans: 21 literals, 9 canonical globals and 15 vector fields.
Ten vector-field accesses alias unrelated four-byte harness anchor names;
use reviewed 491C14/491E6C bases, actual addends and opcode scope rather than a
MAP-at-address name. This audit excludes prologue, calls, immediates, indexed
memory and tables; it is not exhaustive function normalization or data ownership.

For Enemy opcode1B4, a prior complete bounded CFG review finds three generic
ReadInt calls on every selected static return path in both images. Native
404A4C is physically in1AA and shared by1B4; target41054F/410580/4105BE stay
in1B4. The earlier two-versus-three region count measures layout. Review tail
sharing and lifetime before adding another call. Calls were assumed returning
and predicates unconstrained; argument/runtime equivalence is not claimed.
The reused iVar27 is not one stable packet lifetime. A distinct typed packet
already regressed to14476/gap662 in an older nine-TU context.

## Retained build and recovery state

| Context | Retained output | Input/artifact identity |
| --- | --- | --- |
| Actual eleven-TU large graph, including Player | `build/gpt-6.1-sol/anm-vertex-count-20261021/adopted` | 33 source/header inputs, 16 native artifacts; 46 whole guards / 5335 bytes / 142 fields |
| Canonical ten-TU graph, excluding Player | `build/match-linked/EnemyItemDrops` | 32 inputs, 15 artifacts; five selected canonical units; 35 same-image whole guards / 4424 bytes / 118 fields |
| Canonical two-TU range helper | `build/match-linked/AnmRandomU32InRange` | AnmManager + RandomMath; complete 88-byte helper raw-equal, zero fields |
| Rejected combined radial/flags control | `build/gpt-6.1-sol/anm-vertex-count-20261021/radial-declarations-flags` | Source, image/MAP/PDB and input pins retained |

Current public hashes, source overlays and target facts are in
[the radial counter pack](evidence/anm-radial-count/README.md). Private manifests,
producer scripts, reports and CI logs are under
`.analysis/gpt-6.1-sol/anm-vertex-count-20261021/`; start with `RECOVERY.md`.
Verify input/artifact hashes before reusing any cache. A rebuild replaces its
pins. Older carriers are historical after the AnmManager.cpp change.

Do not rerun the one-time `prepare-proof.py` or old experiment runners blindly:
some mutate documentation or restore an old starting source in `finally`.
Use maintained scripts and pinned overlays against their stated starting HEAD.
Historical producer-dependent carriers are retained for reproducibility.
Adjacent TH08/TH09/TH095 and ignored `_reference/th10-decomp` are hypotheses
only; that reference checkout is pinned to
`c6791d82dc9f02e55eca45a5314f900a85481efd` (MIT).

To recreate the necessary current canonical large artifact:

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit enemy-drop-item-counts --unit enemy-drop-item-counts-core \
  --unit enemy-finalize-death --unit enemy-cancel-bullet-record \
  --unit enemy-cancel-bullet-pattern --json
scripts/repo-python scripts/diagnose-ecl-run-native.py \
  --unit enemy-cancel-bullet-pattern \
  --artifact-dir build/match-linked/EnemyItemDrops
```

## Completed validation and cleanup

The frozen code passed **105 CI checks**. One full ten-TU cold replay covers
five accepted units; an additional distinct two-TU cold covers the range
helper. These are two artifact groups, not two independent repetitions of the
complete large context. The actual-source three large native bodies reproduce
the private positive raw; Run and Enemy are unchanged by the radial edit.
Toolchain, tracking, progress, source/native pins and whitespace checks pass;
see [validation](evidence/anm-radial-count/validation-summary.json).

The first CI attempt correctly rejected the changed whole-file LF pin for
AnmManager.cpp in the TransitionEffect host fixture. Review found its extracted
AnmRandomU32InRange body and six other dependencies unchanged. The additional
88-byte native replay passed before refreshing exactly one LF pin plus review
metadata. All843 host assertions and19 EOL/pin guards then passed, followed by
full CI. Strict rejection policy, fixture algorithms and assertions are unchanged.

This checkpoint reclaims **19674975 file bytes** (about18.8 MiB): 5087074 from
superseded experiment outputs, then14587901 from reproducible CI host binaries,
Python bytecode and twelve redundant case2 text dumps. Passed logs, compact
facts, overlays, current native carriers and the selected failure remain.
The private `handoff-cleanup.json` records the latter paths, producers and sizes;
[the public cleanup record](evidence/anm-radial-count/handoff-cleanup.json)
records the retained scope. Unclassified older analysis, independent D3D work,
target, databases, toolchains, Wine/provider state and reference checkout remain.

Only documentation and ignored-artifact cleanup followed the successful full
CI; focused document/tracking/progress/toolchain checks and source/native hash
verification are repeated for handoff. No new experiment or broad replay is
needed to validate those cleanup edits. Future source work must run its affected
Oracle and finish the standard gates:

```sh
scripts/repo-python scripts/verify-toolchain.py --check
scripts/repo-python scripts/validate-tracking.py
scripts/repo-python scripts/progress.py
scripts/repo-python scripts/ci.py
git diff --check
```

The Windows i386 product graph, data owners, initialization, resources, link
order and runtime closure remain open. Diagnostic anchor images are not runnable
products. Exact reconstruction precedes faithful product closure, dual-Oracle
semantic reconstruction and portability; none of those later gates is closed.
