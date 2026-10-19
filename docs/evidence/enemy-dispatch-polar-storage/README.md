# Enemy polar interpolation storage checkpoint (2026-10-19)

Scope: complete `EnemyRuntimeView::DispatchEclInstruction`, target
`0x0040E770..0x00411FBF`, **14,416 bytes**, including its switch data.
This follows the [polar-input checkpoint](../enemy-dispatch-polar-lifetime/README.md).
The parent remains nonexact; **zero new exact bytes** are claimed.

## Target and source model

The target's polar0x11D/0x11F path stores the final angle to float32 at
`0x40F507`, then loads initial angle and subtracts that stored final angle at
`0x40F573/0x40F577`. Initial speed is copied at `0x40F57B`, after subtraction
and before FABS. The reviewed prior candidate instead retained the reflected
angle on x87 and subtracted ST(1). This is a machine-operation difference,
independent of total owner length or source-level casts.

Maintained source now uses the existing plain `EnemyFloat2` for the actual
initial and final interpolation values. Both pairs feed the real scalar
interpolator. The random-move path also populates these pairs and shares the
observed initialization tail, reached in the target from `0x40F5F0` at
`0x40FC07`. No unused workspace, constructor, volatile access, forced reload,
layout change, fake return, padding, helper outlining or profile change is
introduced. Source shape and original variable spelling remain hypotheses.

The emitted candidate restores the final-angle FSTP to float32 before the
common subtraction and uses an FSUB memory operand. Initial speed is again
copied after that subtraction. Its stack allocation is now the target's
`0x2C4`, versus the prior `0x2BC`. Stack-slot identities and the rest of the
parent's native schedule are still open.

`target-facts.json` pins the complete owner, reviewed instructions, constants,
and local read-only Ghidra attestation. Its mapped `.text` SHA-256 is unchanged
before/after the rolled-back review. No new boundary or origin classification
is added.

## Numerical qualification

`EnemyWrapAngle @ 0x44BC70` is94 bytes in the target and the actual `/GL /GS`
carrier. A complete strict same-image check binds all six constant fields.
Its loop returns a live x87 result, with no final float32 store. The counter
test uses the old count after each update; one-direction examples cap at34
updates. The helper's canonical normal-COFF recipe remains unchanged; this
additional linked-image guard gives no new exact credit or TU ownership claim.

The stdlib-only `scripts/check-enemy-polar-rounding-model.py` reproduces one
conditional finite witness under RN-even float32 stores and x87 PC53/PC64.
Input angle447 (`43DF8000`) and initial angle`C1575CFE` deliberately exceed
the wrapper's normalization cap. The live final result is
`-17408245/1048576`; its float32 store is`-4352061/262144`. Target storage
leaves the absolute angle difference below pi, while the old live-result
model leaves it above pi, selecting different adjustment branches.

The script checks the exact target's four constant payloads when requested.
Normal and Python-O executions agree. This establishes a conditional arithmetic
counterexample to discarding the store. Runtime x87 control, game reachability
of this large input, whole-opcode equivalence and a normal-input gameplay bug
remain unknown. Diagnostic executables contain unresolved anchors and are not
runtime probes.

```sh
scripts/repo-python scripts/check-enemy-polar-rounding-model.py --check-target
scripts/repo-python -O scripts/check-enemy-polar-rounding-model.py --check-target
```

## Compiler controls and reproduction

`native-summary.json` records source/header pins, full-owner extents and hashes,
case diagnostics, and the retained cold-image guards. Its comparison counts
use diagnostic normalization and alignment; they are neither exact percentages
nor partial-case credit.

All native controls use the same real eight-TU graph: primary
`src/EnemyEclDispatcher.cpp`, entry
`?DispatchEclInstruction@EnemyRuntimeView@@QAEHXZ`, then support sources in order
`EclVm.cpp`, `AnmManager.cpp`, `AnmVmCreate.cpp`, `EnemyLaser.cpp`, `Enemy.cpp`,
`AnmVmId.cpp`, `RandomMath.cpp`. Every source uses
`/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL /GS`.
Use `ltcg_link.cold_link` with this graph and the pinned toolchain, freezing
the26 project CPP/quoted-header inputs before/after each build. This is a
declared diagnostic graph, not the proved original production TU partition.

Two ordinary nonmember adapter controls, with runtime/index and index/runtime
formal order, both retain the baseline's entire16-byte adapter and three
ordinary register differences. They do not recover target EAX-runtime,
ECX-index ownership; all adapter/header changes are restored. Do not repeat
these parameter-order or member/nonmember controls without new TH10 evidence.

The initial scoped-pair control restored storage/frame but duplicated the
initialization tail, producing14,540 bytes and a424/405 polar interval. Sharing
the actual pair values with random movement reduces that owner to14,524 and
the interval to396/405 while retaining the required store. Length alone did
not motivate the source change; both paths genuinely consume those pairs.
Writing the subsequent comparison as `initialPolar.x >= finalPolar.x`
restores the target's initial-versus-final load orientation. Four ordinary
instruction bytes change, but complete size and aligned diagnostic counts
do not. The status-test/branch encoding still differs from the target.

Two frozen-source cold links of this retained version emit identical complete
owner code:14,524 bytes, frame0x2C4,19 sentinel comparisons, all181 selector
bytes equal and all108 physical groups in order. There are35 target-sized
spans. Whole-owner diagnostics are613/11580 comparable positions; case-aligned
diagnostics2447/11143, with aggregate absolute span gap792 versus the prior786.
The store/frame corrections do not establish whole-owner byte equality.

Only the changed owner and necessary existing guards are replayed. Complete
ResolveIntArgument75, ReadInt144, ReadFloat183 and WrapAngle94 checks cover
**496 bytes and11 target-bound fields** in each retained image. Canonical
recipes, ledger numerator, compiler flags and comparison policy are unchanged.

Standard selector checks, the conditional arithmetic model, toolchain/tracking
checks, generated progress, public CI and whitespace checks pass. Disposable
controls are removed; one ignored image remains for the unresolved parent.
Native product, runtime, dual-Oracle semantic and port gates remain open.
