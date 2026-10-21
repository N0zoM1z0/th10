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

[Generated progress](PROGRESS.md):1776 reviewed candidates,735 confirmed
authored functions/273576 bytes,1088 source mappings and1238 exact functions
overall. Authored exact is **397 functions/72387 bytes (26.46% provisional)**;
overall exact138897 bytes.605 authored functions have source,208 remain
source-present/nonexact and130 source-absent. Source-present authored bytes are
246417/273576 (90.07%).523 origins remain indeterminate; the complete-game
denominator remains unknown.

The latest [thread/register controls](evidence/ecl-thread-register-controls/README.md)
test11 private source models without changing maintained code or exact credit.
Four are raw-neutral. Two operand-pointer forms produce identical31-difference
Run bodies. Five shared-node scopes fix SIB+146E but add RETURN+AF, retaining27
and identical whole Run bodies. Their unchanged count is not raw neutrality.
All images pass35 full guards4424/all118, Run7020/all189/62locals/full171callee;
ANM and Enemy remain raw-identical. Each recompiles one ECL object and reuses
nine verified game objects; no new full cold is claimed.

Earlier standalone StopThread pointer locals were already executed in older
residual/current-dependency contexts. The prepared/unexecuted statement applied
only to the thread-expression campaign, not all history. Do not repeat that
control as new or use its old two-TU26 as a current-source score.

The latest [ECL raw-word negation correction](evidence/ecl-raw-negation/README.md)
expresses opcode55 with the existing unsigned bits member,0u-bits. Target44F851
is NEG EDX followed by Push f/4. The old reconstructed signed expression
triggers UBSan for valid negative-zero bits80000000h. The adopted source
preserves observed modulo32 arithmetic; this is not floating sign inversion.
Original source signedness remains unknown. No new mapping or exact credit.

One full actual-source canonical ten-TU cold passes five units1172 bytes. The
same image passes35 complete guards4424/all118. All three complete large native
bodies are raw-identical to the pre-edit baseline and private positive. The
unmodified maintained diagnostic retains Run7020/6993, all189 fields/62locals
and complete171-byte float callee exact. New current32-input/15-artifact hashes
are in [native evidence](evidence/ecl-raw-negation/native-summary.json).
Old radial/context receipts retain historical source/artifact meanings; their
original15 outputs were copied to
`build/gpt-6.1-sol/ecl-operand-homes-20261022/baseline-ten-tu` before rebuilding.
The older two-TU26-difference source context has not been replayed for this edit.

Nine private controls compile one ECL /GL object and reuse nine verified game
objects. Declaration/scope forms and second-temporary sharing are raw-neutral.
First-temporary sharing fixes two PointAngle homes but adds six NEG_FLOAT
ordinary differences (31 total); pair/reversed forms repeat it. Combining that
sharing with unsigned NEG still gives31 and is rejected. Private diagnostic
source routing changes only two path literals plus ROOT discovery, keeping the
core comparator/bindings/coverage unchanged. Actual-source final proof uses the
unmodified maintained tool. No original lifetime, spelling or TU claim follows.

Actual-source raw-NEG contracts pass17 literal payloads/34 invocations with
ASan/UBSan/leaks and three strict functional controls. The explicit stack seam
projects the first dword of initialized host union storage; its pointer may
widen storage. Native layout, stack conversion/underflow, private ABI and game
runtime are not proved. Existing FORMAT passes10 scenarios/eight controls with
sanitizers. A historical control-projection runner was attempted without its
required source-root argument and is not counted as passed; its old hard pins
are not refreshed by this batch.

The previous [Hint entry controls](evidence/hint-entry-copy-controls/README.md)
recover position/call, chapter-branch and initialization ordering. Allocator
212/220 and standalone constructor51/51 with14 differences remain nonexact.
The prior [Player state2 and caution work](evidence/player-state2-hint/README.md)
added three source bodies and Caution104 exact. Its canonical six-TU source
context is independent of this ECL edit. Semantic host projections do not
establish native storage/private ABI or product closure.

Global automatic/manual boundary audit drift remains open:1114 metadata rows,
49 state and107 confidence differences, zero geometry differences; first failure
is401100 provenance text. Ordinary tracking/CI success does not waive it.

## Current checkpoint validation and recovery

The thread/register native controls and **107 fresh full CI checks pass**. All351
maintained code/test paths remain unchanged through CI and equal the preceding
passed source state. Only validation documentation and classified ignored-output
cleanup follow CI.
Current32 inputs and15 canonical artifacts still equal the raw-NEG pins; the
unmodified maintained Run diagnostic freshly repeats7020/6993/27.

Current private work: `.analysis/gpt-6.1-sol/ecl-thread-registers-20261022/`;
start with RECOVERY.md and manifest.json. Retain current canonical baseline and
one minimal shared-thread-flags failure, plus exact overlays/reports/producers.
Cleanup removes14115654 logical file bytes from ten superseded controls,
four reproducible CI trees and campaign bytecode. Prior raw-NEG baseline snapshot
and historical contexts keep their recorded scopes. Nine unrelated untracked D3D
files and older unclassified
analysis/provider state remain preserved. Broad exact reconstruction stays active.

The next bounded route is a Chain source-completeness review. Maintained
Chain.cpp already defines AddToCalcChain, AddToDrawChain and AllocElem, while
449AE0/131,449B70/131 and449ED0/74 still have empty source fields. Existing
canonical caller fields bind the first two target addresses, but complete body,
ABI and behavior review is still needed before mapping or exact promotion.
Also review449AA0/59 and WinMain's callback-owner creation seam; its old scheduler
reset name and untyped free declaration are hypotheses. No source count changes
are made from this lead. Avoid another round of the same ECL scope controls.

## Three large owners remain nonexact

The historical [ECL storage controls](evidence/ecl-storage-controls/README.md)
independently reproduced their pre-edit two-TU cold Run baseline:7020 bytes,26 ordinary
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

The table retains its measurements after the unsigned-NEG ten-TU cold reproduces
all three complete native bodies raw. New32 input pins supersede the older radial
source pins. The eleven-TU context is historical after the Player and ECL edits.
Aligned positions and equal-sized groups are diagnostics, not partial exact credit.

| Owner / target | Current measurement | Remaining work |
| --- | --- | --- |
| ANM ExecuteScript 43EE30 / 9964 | 9964 bytes; 7541/7995 aligned positions; 90/92 target-sized groups; aggregate gap 2 | Child group91, float-index temporaries49/53/54, FlipX61, common-tail2 alignment and slow Tick operand order. |
| ECL Run 44E1A0 / 7020 | 7020 bytes; 6993 matched; all189 linked fields / 62 local destinations; 171-byte float callee exact | 27 ordinary differences: thread argument, SIB, stack homes and common latch. |
| Enemy DispatchEclInstruction 40E770 / 14416 | 14352 bytes; frame2C4; 19 sentinels; 181 selectors / 108 ordered groups; 42 target-sized groups; aggregate gap566 | Laser factory private ABI, tail sharing, packet lifetime and storage/register/stack scheduling. |

Run's generic decoder-normalization flag is false at the final selector byte;
strict table/selector annotations still cover all7020 bytes, 60 table words and
88 selectors. Keep the historical separate two-TU result (26 differences)
distinct from the current 27-difference graphs.

The preceding [count/residual controls](evidence/anm-count-residual-controls/README.md)
resolve the previously uncompiled expression option in their pre-edit ten-TU graph.
That legacy form and a current-layout one-based loop produce identical complete
three-owner bodies, reducing ANM7541->7475 diagnostic positions. Four source
spelling/local controls are raw-neutral. Single-axis flag splitting reverses
second-vertex Y/Z while leaving total7541 unchanged. Same-context combined flags
recover Tick order but reverse Z (7540); reversed-vector repairs regress other
components, and copy/operator+= translation grows ANM9984/gap38. All twelve are
rejected; maintained source, canonical recipes and exact statistics stay unchanged.
Each compiles one /GL object and reuses nine verified game objects, not a full
cold ten-TU build;35 whole guards4424/all118 and Run7020/all189/62locals pass.

Prior ANM private work is `.analysis/gpt-6.1-sol/anm-radial-expression-20261022/`;
read its RECOVERY/manifest. Keep the current ten-TU baseline and small combined-
flags failure, plus compact input overlays/results. Earlier
[residual/data audit](evidence/anm-residual-data-audit/README.md) and the historical
eleven-TU failure retain their original scopes. No original source spelling or
physical TU is proven, and a positional total alone cannot authorize adoption.

That control batch passed106 full CI checks with unchanged maintained game code;
the current35 whole guards pass again afterward. Only validation documentation
and ignored-output cleanup follow. The [summary](evidence/anm-count-residual-controls/summary.json)
records scope and cleanup of17132488 logical file bytes. All12 exact source
overlays, current canonical baseline and the small combined-flags failure remain.

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
| Historical eleven-TU graph, Player input changed | `build/gpt-6.1-sol/anm-vertex-count-20261021/adopted` | 33 source/header inputs, 16 native artifacts; 46 whole guards / 5335 bytes / 142 fields |
| Canonical ten-TU graph, excluding Player | `build/match-linked/EnemyItemDrops` | 32 inputs, 15 artifacts; five selected canonical units; 35 same-image whole guards / 4424 bytes / 118 fields |
| Canonical two-TU range helper | `build/match-linked/AnmRandomU32InRange` | AnmManager + RandomMath; complete 88-byte helper raw-equal, zero fields |
| Rejected combined radial/flags control | `build/gpt-6.1-sol/anm-vertex-count-20261021/radial-declarations-flags` | Source, image/MAP/PDB and input pins retained |

Current ten-TU hashes are in [the raw-NEG pack](evidence/ecl-raw-negation/native-summary.json).
Historical radial source overlays and target facts are in
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

## Prior radial checkpoint validation and cleanup

The radial checkpoint code passed **105 CI checks**. One full ten-TU cold replay covers
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

At that prior checkpoint, only documentation and ignored-artifact cleanup
followed its successful full CI; focused document/tracking/progress/toolchain checks and source/native hash
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
