# Primary alpha setup: source order and private ABI

`AnmVmView::StartPrimaryAlphaInterpolation` at **0x00442300** now reproduces
its complete **130-byte** owner in two independent canonical cold links.
The single DIR32 field at +0x5F is independently bound to game speed at
0x00476F78. This adds 130 authored exact bytes; ExecuteScript remains nonexact.

See [target observations](target-facts.json) and [source pins, controls and
strict native comparisons](native-summary.json). Target, source, compiler,
reference hypotheses and unvalidated runtime claims remain separate.

## Source change and evidence

Only the two zero-tangent assignments move to the beginning of the existing
helper. Duration, byte formals, endpoint conversions, caller expressions,
timer behavior, class layout and compiler profiles are unchanged. The genuine
ExecuteScript entry, RandomMath, AnmVmCreate and AnmVmId stay in the four-TU
`/GL /GS` graph. No calling-convention annotation or register hint is added.

Local read-only Ghidra and independent target decoding agree on the complete
130-byte extent, two direct call sites, EAX VM, ECX duration, three stack byte
values and RET 12. The earlier source let LTCG pass mode in EDX and return
with RET 8. Moving the zero assignments naturally recovers the observed
interface and every body byte after the sole address field is replayed.

The pinned MIT reference's `StartAlpha` at
`matching2/msvc/src/core/sprtanim/anm_vm_tick_context.cpp:279` suggested the
source order. Its source writes tangents first, while target machine code
stores duration/mode/initial before the tangent zeros. The successful control
shows why machine store order cannot establish unique C++ statement order.
Neither the reference nor this match proves the original source spelling,
formal declarations or production translation-unit partition.

## Rejected controls and corrected history

| Source model | Complete helper / target | Comparable helper positions equal | ExecuteScript / target | Target-sized groups |
| --- | --- | --- | --- | --- |
| Previous byte formals, endpoint-first | 128 / 130 | unequal extents; nonexact | 9948 / 9964 | 83 / 92 |
| All three byte formals widened to int, explicit low-byte stores | 130 / 130 | 93 / 126 | 9948 / 9964 | 83 / 92 |
| Only mode widened to int, explicit low-byte store | 130 / 130 | 93 / 126 | 9948 / 9964 | 83 / 92 |
| Original byte formals, tangent-first | 130 / 130 | 126 / 126; sole field strictly replayed | 9948 / 9964 | 84 / 92 |

The two integer controls emit the same complete helper but different callers.
All-integer operands recover opcode 58's length while shortening opcode 55;
mode-only still leaves opcode 58 one byte short. Both are reverted.

ANM-059's early Wine failures were not the final integer-control outcome:
the later [2026-10-18 archive](../anm-primary-alpha-2026-10-18/ACTUAL_CANDIDATE_RESULT.json)
and [whole-helper comparison](../../../probes/anm-primary-alpha/comparison/ACTUAL_COMPARISON.md)
already established a successful, nonexact 130-byte candidate. The fresh
all-integer helper has the same complete candidate hash. The knowledge entry
now links that superseding result. Two accidentally repeated all-integer links
are recorded as repetitions of one input, not additional hypotheses.

## Complete parent and validation scope

Both retained cold images reproduce the same ExecuteScript diagnostics:
9948 / 9964 owned bytes, frame **0xF8 / target 0xFC**, all 92 physical groups
in order, 84 equal-sized spans, 1312 / 8564 whole-owner comparable positions
and 6492 / 8000 case-aligned positions. The 94-entry table still starts 16
bytes early. These are diagnostic layouts and candidate field normalization,
not complete target-bound parent acceptance.

Opcodes 55 and 58 now have equal spans and equal comparable positions, but
eight other spans differ; the later update region also shrinks. No case or
matching fragment earns separate credit. Getter, pointer-getter, secondary
alpha and evaluator diagnostics are recorded without changing their recipes
or claiming broader canonical regression coverage.

Each canonical cold image strictly passes Initialize 273 and SetupVertexBuffer
472 as existing complete guards, including all 22 guard address fields, plus
the new helper's one field. An additional strict warm check in the second image
also preserves the already accepted radial initializer803 and all43 fields;
this adds no credit and supersedes no prior source binding. Only the necessary
graph is rebuilt; the historical
ANM cohort is not replayed. Toolchain declarations, tracking, generated progress,
standard repository CI and whitespace checks pass at the checkpoint.

The first CI run stops at the transition-effect fixture's whole-AnmManager.cpp
pin. Only its reviewed normalized pin is refreshed: the extracted range helper
is byte-identical, other dependencies and strict rejection rules are unchanged,
and the original raw provenance is preserved. The focused host suite and all
19 normalization/rejection controls pass before the final CI rerun.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit anm-start-primary-alpha-interpolation \
  --unit anm-vm-initialize-private --unit anm-setup-vertex-buffer --json
```

Repeat the command for a second independent cold build. The canonical manifest
and strict comparator are the reproducible acceptance path. Failed experiment
outputs and duplicate binaries are removed; one retained native image remains
ignored for the unresolved complete parent. No game runtime was exercised.
Windows i386 product closure, semantic reconstruction and portability remain open.
