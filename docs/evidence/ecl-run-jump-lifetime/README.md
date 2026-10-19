# ECL Run jump operand lifetime

This checkpoint refines one source model in the complete **7,020-byte** ECL
Run owner. It adds **zero exact bytes**. Two cold real-caller links reproduce
7,020 bytes, all 60 relative table destinations and all 88 selector bytes,
with 6,125 / 6,264 diagnostic comparable matches. The selector suffix still
leaves normalization incomplete, and 139 ordinary positions differ.

See [the current native comparisons and source controls](native-summary.json)
and [the TH10-local target observations](target-facts.json).

## Retained source model

At 0x44E40F the target loads the live instruction through its field pointer
into EAX. It reads time at EAX+0x14, stores currentTime, then reads the delta
at the same EAX+0x10 at 0x44E416. Maintained source previously read the second
operand through the loop-entry `current` cache. Both operand reads now use one
explicit scoped instruction snapshot, retaining the existing common advance
path. The pinned MIT reference also uses a fresh pointer for these two operands;
TH10-local target instructions establish the observation.

The generated delta address changes from `[EDI+0x10]` to `[EAX+0x10]`, matching
the observed input base. Its result still occupies EAX rather than target ECX.
Only one byte outside recognized linkage fields changes; both versions differ
from the target at that byte, so the comparable match count does not increase.
This is an operand-view refinement, not a claimed normal-input runtime bug fix,
full jump/base reconstruction, original declaration or parent exact match.

## Bounded negative controls

| Model | Candidate / target | Comparable matches | Table positions |
| --- | --- | --- | --- |
| Unchanged baseline | 7020 / 7020 | 6125 / 6264 | all 60 aligned |
| Reload loop `current`, use it for operands/base | 6948 / 7020 | 726 / 6268 | shifted |
| Scoped fresh target, direct instruction store/continue | 6948 / 7020 | 726 / 6268 | shifted |
| Separate member re-read for delta | 7020 / 7020 | 1025 / 6264 | later entries shift by 2 |
| Scoped operand snapshot, common advance retained | 7020 / 7020 | 6125 / 6264 | all 60 aligned |

The last row is reproduced in a second independent cold link. The complete
fresh-base models change optimization across several other case groups; they
are not adopted. The two-byte shift also rejects the separate member read.
All numeric comparisons remain whole-owner diagnostics. Differing field
inventories and layouts make the rejected rows unsuitable as percentage scores.
No partial case is added to the exact ledger.

The retained baseline discrepancy inventory is 13 positions before the FORMAT
physical interval, 118 within it and eight after it, excluding candidate-
recognized fields. This is not semantic opcode ownership or proof of complete
link normalization. The fresh view confirms that the earlier FORMAT pointer,
volatility, counter and switch controls remain parked; none is retried here.

## Validation and remaining work

Both retained-source cold images pass seven existing complete canonical guard
comparisons: host Run 146, ReadInt 144, ReadFloat 183, ReadIntValue 124,
ReadFloatValue 171, ResolveInt 58 and ResolveFloat 93. All 919 bytes and 17
linkage fields pass strict PE/MAP/PDB and target-bound checks. These are
same-image guard recipes, with no duplicate credit or canonical recipe change.
The actual EnemyRuntimeUpdate caller remains in the two-TU `/GL` graph.
Compiler profiles and helper interfaces are unchanged.

The target/source table reporter, toolchain declarations, tracking, progress,
documentation and standard repository CI pass. Only this Run body changes;
no full historical native cohort is replayed. A conservative source/header
freeze and both independent PE/PDB identities are recorded in the native summary.

```sh
scripts/repo-python scripts/build-match-unit.py --unit ecl-vm-host-run
scripts/repo-python scripts/compare-linked-function.py --unit ecl-vm-host-run --json
scripts/repo-python scripts/report-ecl-vm-table.py --check
```

The existing generic probe and recorded graph can inspect the complete Run
contribution; Run has no canonical exact unit. Remaining register/stack homes,
common advance/base aliasing and FORMAT remain unresolved. The adjacent TH08,
TH09 and TH095 control sources were hypothesis material only; their different
engines do not establish a TH10 pointer or ABI fact. Complete Windows i386
product/runtime closure, semantic reconstruction and portability remain open.
