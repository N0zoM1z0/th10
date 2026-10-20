# Complete ECL Run: disabled trace and shared advance base

> Handoff update, 2026-10-20: Run remains nonexact. This pack predates later shared-header changes; its cache is historical until all input pins are verified or a fresh graph is built. The operator ended the 95% campaign; see [current handoff](../../RE_HANDOFF.md) and [RGB checkpoint](../anm-rgb-interpolation/README.md).

The complete **7,020-byte** `EclVmContext::Run` owner remains nonexact.
Two independent cold real-EnemyRuntimeUpdate links reproduce its full extent,
all60 relative jump destinations and all88 selector bytes. Diagnostic ordinary
differences fall from139 to **28**:6236/6264 comparable positions agree.
There is **zero new exact credit** and no denominator change.

See [target facts](target-facts.json) and
[source pins, controls and native guards](native-summary.json).

## Retained source model

FORMAT copies a string into an otherwise unused allocation and retains a
three-way conversion dispatch, including `%%`, while discarding operand-service
results. A natural empty variadic trace consumer explains these observations.
The pinned MIT reference supplies this hypothesis; it is not proof of an
original logging function, declaration, name or translation-unit partition.
The maintained trace has no retained function contribution in either image.

The source now uses one argument counter, a normal percent pointer and a
conversion switch. The old volatile pointer and volatile metadata counter are
removed. Each typed branch evaluates its operand service, passes the result to
the disabled trace and advances the counter. VC7.1 naturally strength-reduces
the single counter to the target's flag/value/metadata induction variables.
Keeping the increment within each typed branch is material: a common increment
instead emits7028 bytes, versus7020 with the branch-local increments.

This differs from the historical isolated switch/qualifier spelling probes.
The new lifetime evidence is the disabled consumer and its typed branch-local
counter versions. Without the consumer, the compiler removes the target's
otherwise redundant percent arm. No forced register, volatile, noinline,
padding, synthetic ABI or target bytes are added.

Jump and ordinary advancement also share one explicit `advanceInstruction`
snapshot. Target0x44E40F loads this base once, reads time/delta through it and
joins0x44FB81; the ordinary path at0x44FB77 loads the current member before
joining. The new source expresses that common base directly instead of using
the dispatch-entry cache for jumps. Its native EAX base, ECX delta and ordinary
advance sequence now agree. The final loop-time reload still uses EAX where
the target uses EDX. No normal-input gameplay bug is claimed for the old model.

## Controls and limits

| Model | Candidate / target | Diagnostic comparable matches |
| --- | --- | --- |
| Unchanged source | 7020 /7020 | 6125 /6264 |
| Reuse point-angle result local | 7020 /7020 | 6125 /6264; code-neutral |
| Single argument counter, no trace | 7020 /7020 | 5893 /6264 |
| Disabled trace, common increment | 7028 /7020 | 5813 /6264 |
| Disabled trace, increment in each typed arm | 7020 /7020 | 6231 /6264 |
| Above plus shared advance base, retained | 7020 /7020 | 6236 /6264 |

Moving the counter declaration before allocation, reversing the squared-sum
expression, and naming the trace-result values are separately code-neutral
against the retained shared-base model. All are restored. Earlier warm images
and historical source hashes are not reused as current evidence.

Remaining ordinary differences are16 before the FORMAT physical interval,
ten within it and two after it. They include stack homes, equivalent base/index
encodings, register selection and the float-service argument-move order.
The generic diagnostic still reports incomplete normalization for the selector
suffix. Case/table equality and these counts do not authorize partial credit,
masking ordinary bytes or relaxing the canonical comparator.

## Reproduction and validation

The primary source is `src/EclVm.cpp`, support `src/Enemy.cpp`, entry
`?EnemyRuntimeUpdate@@YGHPAUEnemyRuntimeView@@@Z`. The existing host/spawn
canonical recipes use `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL`.
Each full cold build freezes16 recursive project inputs before/after linking.
The two independent PDB identities and retained-image hashes are in the proof.
Eight existing complete host/spawn/operand guards strictly replay1061 bytes
and20 target-bound fields in each image. The operand guards are additional
same-image contexts; their canonical recipes and credit are unchanged.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit ecl-vm-host-run --unit ecl-vm-spawn-thread --json
scripts/repo-python scripts/report-ecl-vm-table.py --check
scripts/repo-python scripts/test-ecl-format.py --sanitize
```

The generic `probe-ltcg-backlog.py` and the graph above inspect the full Run
owner; it has no canonical exact unit. The source-extracted FORMAT host suite
checks10 bounded packet scenarios and8 assertion-only functional mutants under
ASan/UBSan/leak checking. It covers percent/unknown conversions, typed metadata,
flag/value strides and live instruction replacement during either operand
service. Services are explicit roles, not their implementations. A separately
reviewed private adapter of the existing control projection preserves all11
assertion-only mutants with the renamed advance variable; no archive is edited.

These are bounded host checks, not original/native execution or malformed-input
safety guarantees. The diagnostic images contain unresolved data anchors and
are not run. Local Ghidra is read-only, noanalysis, one CPU and768MiB, with the
entire mapped `.text` checked before/after a rolled-back transaction.
Only the necessary two-TU graph is rebuilt, serially at nice10. Superseded
controls and generated host binaries are pruned; one current carrier remains.
The pre-existing untracked D3D evidence is untouched.

Whole Windows i386 product/runtime closure, semantic reconstruction and
portability remain open. Large owners remain the active priority; the former95% campaign is closed.

All95 standard CI checks pass. The final test-runner change only places its
temporary builds under `build/`; its complete focused suite is rerun afterwards.
Toolchain declarations, tracking, generated progress and whitespace checks pass.
