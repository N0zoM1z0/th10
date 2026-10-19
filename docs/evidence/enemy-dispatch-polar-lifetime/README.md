# Enemy dispatcher: polar operand lifetimes and switch ownership

The retained complete dispatcher is **14,500 / 14,416 bytes**, reproduced in
two independent cold eight-TU links. It remains nonexact and adds **zero exact
bytes**. This checkpoint restores one target-observed shared input comparison
and repairs a false source-coverage failure in the selector reporter.

See [target observations](target-facts.json) and [source pins, full-owner
diagnostics and strict existing guards](native-summary.json).

## Retained source model

Polar interpolation opcodes 0x11D/0x11F share the 405-byte physical interval
0x40F460–0x40F5F4. Target FCOMP at 0x40F4B5 examines the decoded angle before
the mode store, integer mode comparison and FNSTSW. Both mode paths consume
that one floating predicate. There are three sentinel comparisons in this
interval and 19 in the complete target dispatcher. The sentinel's actual
eight-byte payload is checked separately from address normalization.

The previous additive path first assigned the angle/speed result temporaries,
then compared those outputs. It produced four comparisons in the interval
and 20 across the owner. The source now compares the saved inputs before
assigning the additive results. Ordinary mode is the first source arm, which
also restores the target JZ-to-additive orientation after the shared predicate.
Existing zero/motion fallbacks, mirroring calls, shortest-path arithmetic,
mode writes and subsequent initial-value capture remain unchanged.

This is source/dataflow and emitted-order evidence, not a recovered original
spelling or a runtime bug fix. Target code still stores the final angle to
float32 before the common difference; both baseline and retained candidate
keep that reflected angle on x87. That pre-existing precision gap is open.
No easing/evaluator arithmetic, flags, object layout, ABI annotation, padding
or compiler profile changes are used.

## Full-owner controls

All rows use the same declared direct-entry graph: EnemyEclDispatcher plus
EclVm, AnmManager, AnmVmCreate, EnemyLaser, Enemy, AnmVmId and RandomMath, all
`/GL /GS`. These are real maintained definitions, with remaining unresolved
diagnostic anchors; this graph is not a production image or a proved original
TU partition. Its fresh baseline supersedes older sizes only for this context.

| Source | Complete owner / target | Sentinel comparisons | Polar interval / target |
| --- | --- | --- | --- |
| Current-source baseline | 14512 / 14416 | 20 / 19 | 396 / 405 |
| Unsigned adapter index | 14512 / 14416 | unchanged | unchanged |
| Compare additive inputs before outputs | 14500 / 14416 | 19 / 19 | 386 / 405 |
| Above, ordinary-mode arm first: retained | 14500 / 14416 | 19 / 19 | 386 / 405 |
| Also defer ordinary outputs: rejected | 14484 / 14416 | 19 / 19 | 370 / 405 |

The unsigned-index control produces identical complete dispatcher bytes and
the same three ordinary adapter-register differences; its header/body edits
are restored. The last row has a smaller total gap but introduces a second
live final value on x87: common subtraction becomes FSUB ST(2), whereas the
baseline/retained path uses ST(1) and the target uses stored floats. The added
speed lifetime expands the unresolved mismatch and is not adopted.

Both retained images have frame0x2BC versus target0x2C4, all181 selector bytes
equal, all108 groups in physical order and35 equal-sized physical spans.
Whole-owner comparison is533/11576 candidate-normalized comparable positions;
case-aligned comparison is2424/11133. Aggregate absolute span gap is786 versus
baseline776, despite the closer total length. These differently aligned/masked
diagnostics are not exact percentages or partial-case credit. The polar span,
stack homes, argument registers and complete parent remain unresolved.

## Selector source scope

The previous regex counted all127 named labels, including three spell labels
inside a nested switch, and reported a false duplicate failure. The reporter
now requires one named method and one compound outer switch, ignores comments
and ordinary literals, and assigns labels to their nearest switch. It counts
the actual124 outer labels. Genuine outer duplicates, missing outer labels
shadowed by inner labels, unknown labels, ambiguous/unbalanced definitions and
unsupported switch forms still fail. Seven target-independent tests exercise
these controls and run in CI.

The target-bound table check now passes, including the retained candidate's
literal selector equality and physical group order. This proves source/selector
coverage only; it does not establish opcode semantics or parent exactness.
The comparison Oracle and its data/normalization policy are unchanged.

## Verification and remaining work

Each retained cold image passes complete strict same-image guards for
ResolveIntArgument75, ECL ReadInt144 and ReadFloat183: **402 bytes and five
target-bound fields**. Their existing field recipes are adapted explicitly
to the actual eight-TU carrier and PDB support owner for these guard checks;
canonical recipes and exact totals are unchanged. The three-register-encoding
gap in the16-byte read adapter and the nonexact153-byte laser factory remain.
Only the modified owner and its necessary guards were checked; no historical
full native cohort was replayed.

```sh
scripts/repo-python scripts/test-ecl-dispatch-table.py
scripts/repo-python scripts/report-ecl-dispatch-table.py --check
```

The recorded graph/profile and complete-owner pins reproduce the cold
diagnostics through `ltcg_link.cold_link`. Toolchain declarations, tracking,
generated progress, standard CI and whitespace checks pass at this checkpoint.
Superseded controls and duplicate images are cleaned; one retained native image
remains ignored for further complete-owner investigation. Windows product,
runtime, dual-Oracle semantic and port closure remain open.
