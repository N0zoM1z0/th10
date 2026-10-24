# TH10 exact-first method, 2026-10-24

## Decision and scope

The operator approved this method on 2026-10-24: document it, then advance the
reconstruction. The objective remains **98% of reviewed authored owned bytes
exact**, not 98% source coverage, approximate byte similarity, or a count of
small exact functions. A reviewed owner includes its assigned code, internal
alignment and owned tables; unresolved ownership stays unresolved.

This changes experiment selection and source-recovery order. It does not relax
[Oracle policy](ORACLES.md), change any accepted extent, or authorize a new
exact ledger row. At adoption, the maintained source checkpoint is
`00c7466`, the documentation baseline is `887e0bab`, and new whole-owner
exact bytes from this method remain **0**. MIDI was the first bounded
representative; its completed nonexact source checkpoint is recorded below.
The method itself remains unchanged; the MIDI and Options pilots are recorded
below and the next representative is under bounded review.

## Why the method changes

The current source is already compiler-oriented. Readability alone is not an
established cause of the residual differences, and neither a wrong compiler
nor a wrong original translation-unit partition has been proved.

- ANM's complete 9964-byte owner retains 9960 equal bytes. The four ordinary
  differences concern Child91's layer register and operand order in the second
  radial Z addition. The last 15 controls yielded 12 byte-identical results,
  two regressions and one strict structural rejection. Preserve that evidence;
  another neutral spelling panel is not progress.
- ECL's complete 7020-byte owner retains 6995 equal bytes. Its 25 differences
  are mostly stack/register/SIB choices, including 16 stack homes. Nonlocal
  coupling has been demonstrated, but it does not identify the original
  compiler/profile/TU cause.
- These near matches receive no whole-owner exact credit. That accounting
  explains zero added exact bytes despite real source, binding and diagnostic
  progress; it is not evidence that those activities did nothing.

See the [maintained handoff](RE_HANDOFF.md), the
[ANM source/proof packet](evidence/anm-child-request-record/README.md), and the
[last 15 controls](evidence/handoff-request-cleanup-20261024/README.md).

### Comparative evidence, not a compiler recipe

Existing TH105 receipts and history were reviewed at fixed commit
`4a38312`; this was **not** a fresh TH105 cold run and made no TH105 writes.
The latest 18 commits in that review added a net 17 source-present function
owners, not 17 source-adding commits, and five new exact functions / 21959
callable bytes. Relevant examples were:

- IkuObject `dd41181`: an early complete comparison covered 6229 callable
  bytes / 6551 full emitted payload bytes.
- Alice `4a38312`: 10715 callable bytes / 11072 full emitted payload bytes became exact
  after corrections to the real packet, inclusive loop endpoint, NaN guard,
  and case-998 metadata.
- Alice control `f57d98c`: changing three continuation stores from 1 to 3
  made the full 2560-byte owner exact. An apparent code-generation symptom can
  therefore still be a semantic reconstruction error.
- TH105 still parks Aya's 2-byte, Komachi's 7-byte and Sakuya's 13-byte cases.
  Its successful examples do not imply that every near match closes quickly.

Those successful selected units used VC8 non-`/GL` contexts; TH10's core
owners use VC7.1 build-6030 LTCG multi-input graphs. Transfer the discipline of
early complete comparisons and evidence-driven semantic repair, not flags,
addresses, data, layouts, ownership or exactness claims.

## Operational loop

1. **Choose a representative owner.** Prefer a medium/large complete owner
   with an attainable comparison contract, a concrete semantic uncertainty,
   and useful family reuse. Size and existing byte distance are observations,
   not the selection rule. Do not farm tiny leaves or always select the
   nearest-to-exact function.
2. **Recover natural low-level C++.** Provisional numeric/offset names are
   acceptable when they honestly express current knowledge. Recover real
   operand widths and signedness, capture points and lifetimes, memory effects,
   floating-point rounding, control flow, packet/state values, and actual callee
   declarations. Do not first impose an attractive class hierarchy or abstract
   API that hides the target's operations.
3. **Compare the first complete owner early.** Once a coherent complete
   implementation and a usable target-bound contract exist, compile it in the
   correct lane and compare its whole extent. Do not postpone this behind a
   large native-control matrix. Directed native controls answer concrete
   ambiguities that block that model; they are not a substitute for a full
   owner comparison.
4. **Run one falsifiable coherent model per trial.** State the evidence,
   changed source meaning or context, predicted complete-owner effect, and
   rejection condition before compiling. A model may require coordinated
   edits, but it must not bundle unrelated guesses. Keep a distinct baseline
   and record the actual build graph and tool/profile pins.
5. **Diagnose with all gates intact.** Compare the complete owned extent and
   every declared field, fixed local destination and required complete callee.
   Check affected guards. A size, binding, decode, local-target, or callee
   failure is diagnostic evidence, never acceptance. A closer prefix or case
   fragment is not exact credit. Retain TH10's `/GL` where applicable.
6. **Accept only whole-owner exactness.** An apparently exact trial still
   requires canonical cold reproducibility and the existing full comparison
   contract. Keep Factory acceptance separate when that transport is used.
   Retained-artifact replay, a successful build and a local commit each have
   zero new exactness authority by themselves.
7. **Make readable source after stable exactness.** Improve names, types,
   classes and interfaces only when supported. Replay the affected complete
   owner and its required contract after every refactor; revert or isolate a
   refactor that loses exactness. Readability cleanup is not permission to
   invent an ABI or change semantics, and it does not open the later product,
   dual-runtime semantic or portability phase gates.

## Bounded investigation and parking

ANM's four-byte and ECL's 25-byte cases remain preserved, not abandoned. Park
repeated neutral source-spelling panels. Reopen one only with new TH10 target,
def-use, lifetime, callee or actual caller/TU-context evidence that produces a
specific prediction and a distinct baseline/control.

When successive valid controls are byte-identical and add no discriminatory
evidence, retain a bounded receipt, state what the controls did and did not
exclude, and switch to another representative. There is no magic trial count
and no requirement to exhaust every spelling, type name, flag or hypothetical
TU globally. A new semantic observation can justify returning immediately.

Do not disguise a structural rejection as a regression score or keep changing
the comparator until it accepts a preferred source. No copied target bytes,
fake ABI/returns, inert padding, register hints used merely to force allocation,
or weakened masks/extent/local-target/callee rules are permitted.

## Hypothesis checklist

Record this small packet before an experiment:

- Owner ID, reviewed target extent, origin and source mapping; unknowns remain
  explicit. State why this medium/large representative is useful.
- Source checkpoint, target identity, compiler/profile and full input graph.
  Distinguish the actual maintained caller/support context from a private probe.
- Concrete TH10 observation and one coherent source/context model. Identify the
  uncertainty: value/width, capture/lifetime, rounding, branch/loop endpoint,
  packet/state/metadata, actual callee declaration, or supported graph context.
- Predicted effect and falsifier: affected operation/field/local target and the
  expected full-owner outcome. Explain how this differs from prior controls.
- Baseline and candidate paths/hashes, complete comparison command and all
  extent/field/local-target/callee/guard gates.
- Result category: structural failure, valid nonexact, byte-identical control,
  exact candidate awaiting cold replay, or accepted complete exact owner.
  Separate observation from inferred cause.
- Next decision: retain, reject/revert, refine using new evidence, or park and
  switch. State the reopening condition, not an unbounded list of guesses.

## Candidate-selection space

Use current ledger/source information to assess a small set of candidates:

| Dimension | Useful selection question |
| --- | --- |
| Comparability | Can the full extent and required bindings/callees be reviewed and reproduced now? |
| Semantic uncertainty | Is there a specific unresolved value, packet, loop, capture, rounding or callee question that target evidence can answer? |
| Family reuse | Will the result clarify a recurring engine/compiler pattern for other substantial owners? |
| Context cost | Is the necessary real caller/support graph available, and can its effects be separated from the source hypothesis? |
| Coverage value | Does this advance substantial authored ownership rather than optimize a tiny-function count? |

The selection space includes a fresh medium/large source-present owner, a
substantial source-absent owner with recoverable semantics and comparison
context, or a preserved core owner reopened by genuinely new evidence.
ANM/ECL's numerical proximity alone is insufficient.

Initial 2026-10-24 triage selected the following bounded first step:

| Candidate | Selection or queue state | Required next evidence |
| --- | --- | --- |
| MIDI ProcessEvent, target 1670 bytes | First pilot completed; current1690, unsupported helper removed, five guards retained | Park unsupported allocation/alignment trials; require new independent evidence to reopen |
| FrontEndOptions, target 3188 bytes | Fresh baseline and one WORD-consumer model complete; still3276/3188 | Preserve exact Refresh and known captures; park unsupported branch-factoring/adapter trials |
| EndingRun, target 1568 bytes | Queued only | Reassess complete comparability and one concrete semantic uncertainty before selection |

The MIDI audit independently inventories complete original/candidate bodies,
fields and selector tails, but rejects current structural mismatches. It grants
no exactness or ordinary-byte score. The first source model and tests are
recorded below; no further alignment/register panel is authorized by that result
alone. The subsequent Options width model is recorded below; no whole-owner exactness
follows from its two recovered consumers.

## Reporting and execution

Every checkpoint reports separately:

1. **New complete-owner exact bytes**, with the owner, canonical receipt and
   accepted scope. Also show the total against reviewed authored owned bytes.
2. **Maintained source coverage**, with newly covered owners/bytes, independent
   of exactness.
3. **Diagnostic progress**, including semantic corrections, retained near
   matches, structural rejections and useful negative controls. Never add their
   fragments or ordinary equal-byte counts to the exact total.

Current execution uses the Web/Factory registered `th10` repository. The
coordinator owns compiler scheduling, gates and local commits: native builds
run serially, on one CPU, at niceness 10. Read-only analysis and disjoint
documentation work may be delegated without starting competing builds.
Use commit prefix `gpt-dots: ...`; local commits only, no push. Earlier
`gpt-6.1-sol`, direct-local-only and no-subagent session directions are
historical context, not the current routing policy. Preserve the nine unrelated
untracked files under `docs/evidence/d3d-adapter-format/`; they are not part of
this method change.

## Method checkpoint validation

This checkpoint changes routing and documentation only. Source, headers,
compiler profiles, comparison contracts and exactness ledgers remain unchanged.
Fresh target/toolchain execution and Factory-native Ghidra attestation passed;
retained source-bound ANM/ECL/Player replay, tracking, generated progress, local
document links and whitespace checks pass. All nine unrelated D3D evidence
files retain their recorded hashes. No new cold reconstruction build or full
CI run is claimed for this documentation-only checkpoint; the existing source
CI coverage remains scoped to its recorded source checkpoint.

## First pilot checkpoint

[MIDI ProcessEvent](evidence/midi-exact-first-20261024/README.md) exposed a real
unsupported helper boundary in its first complete comparison. One lexical
expansion restores the original37-field inventory and preserves five exact
guards, but the complete owner remains nonexact at1690/1670. This is bounded
diagnostic/source progress, not new exact bytes. The next decision follows the
parking rule rather than an unbounded alignment/register experiment panel.

## Second pilot checkpoint

[Options](evidence/options-input-word-20261024/README.md) first received a fresh
current-source baseline, then one target-backed result-width model. Both AX
consumers are recovered, with all executable code outside the controller and
the full3999-byte exact Refresh peer unchanged. The controller is still
nonexact. Static FindVm sharing was distinguished from missing runtime work;
that finding does not justify a new spelling panel or partial exact credit.
