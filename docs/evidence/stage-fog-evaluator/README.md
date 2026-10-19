# Fog evaluator and shared easing review (2026-10-19)

This checkpoint corrects the shared easing call protocol, makes Fog's initial
snapshot explicit, and restores target x87 arithmetic paths. **No new exact
bytes, source owners, boundaries or origin classifications are promoted.**
The five existing Fog/object-VM units retain all533 bytes and17 fields in a
strict complete replay of the selected cold image.

## Target observations

Hash-attested local Ghidra and pinned Capstone agree on Fog Evaluate at0x4049A0:
688 bytes, frame0xD4, EBX receiver, one stack output and RET4. Modes7 and17 copy
seven initial dwords before addition. The completion path returns final first,
then handles the additive exception. Fourteen outlined calls appear in the
reviewed order: conversion, three additive updates, four Hermite scales, two
Hermite additions, easing, subtraction, scale and final addition. The final
addition is shared by the Hermite and ordinary paths.

CalculateAnmInterpolation at0x44C350 receives mode in EAX and two float32 stack
arguments; every one of its17 reviewed RET instructions pops8 bytes. All six
direct consumers were reviewed: Fog, ANM float3/color/alpha/float2, and Enemy
scalar interpolation. This is a measured private LTCG interface. It does not
identify the original C++ keyword or imply standard ECX fastcall.

The code ends at0x44C54C. Three compiler-alignment bytes precede the16-dword
absolute dispatch table at0x44C550..0x44C58F. Its destinations all lie inside
the reviewed code. Local Ghidra independently decodes a separate57-byte body
at0x44C590, rolling its temporary metadata back. The candidate's complete PDB
contribution includes code, alignment and table, so the existing509-byte
code-only ledger window cannot be accepted by cropping a larger candidate.
No global boundary inventory or denominator is changed here.

Both local Ghidra reviews verify the disk target and full mapped `.text`
SHA-256 `c1611c9becb832a4e2ba456afc58218fb5b8fafd6c85d19e3addbebb0c946e0a`
before/after under `-readOnly`. The database is not committed.

## Maintained source and compiler observations

`__stdcall` on the genuine three-argument easing declaration/definition causes
VC7.1 LTCG to produce the measured EAX/RET8 form. The previous cdecl form left
an ADD ESP8 in the Fog caller that the target does not have. The spelling
represents the observed cleanup behavior; original source ownership and spelling
remain unknown. The four ANM consumers and separate genuine Enemy graph are
checked alongside Fog; none receives exact credit.

Fog now uses real scoped initial-value snapshots and named result values.
The target's final-first completion order is retained. The measured addition
boundary is kept outlined with a compiler directive: otherwise the snapshot
model inlines the first addition and introduces four extra conversions. The
addition's own complete92 bytes remain exact. The directive does not establish
that the original source contained that annotation.

Repeated cubic/quartic inverse expressions preserve the target's live x87
intermediates. Pairing the switch cases follows their physical target order.
A diagnostic follows each of16 table modes plus0,17,-1,99 through both possible
comparison arms:40 instruction traces. It elides unconditional control transfers
and replaces absolute float addresses with their actual four bytes, retaining
all remaining instructions. The prior stdcall-only body agrees on30 traces;
the selected body agrees on40. This checks arithmetic shape, not native execution,
FP exception behavior, reachability or complete-byte exactness.

The selected complete easing contribution remains612 bytes (547 code, one
alignment byte,64 table bytes), versus the576-byte target code/alignment/table
window. Cubic/quartic common-tail placement still differs. No helper credit,
509-byte crop, partial case credit, flag sweep or ABI fabrication is used.

The Fog candidate remains691/688, with target frame0xD4 and all14 calls. After
normalizing only known link destinations and local branch addresses for
inspection, six LEA operands differ; every other instruction is in order.
They concern the second accelerated addition, four Hermite destinations and
the ordinary subtraction destination. This diagnostic is not an acceptance
Oracle. All22 target address fields are independently inventoried in
`target-facts.json` for a future complete replay.

Named versus temporary initial values, one shared initial-value object, named
result values, scoped accelerated results and a natural aggregate-return API do
not close the owner. The aggregate-return API is byte-neutral and is restored.
A nonmember value-add control expands the helper to99/92 and is also restored.
Do not repeat these controls without new alias, lifetime or compiler-context
evidence. The issue reference suggests hypotheses only; no reference byte or
exactness claim is adopted.

## Validation and reproducibility

The selected cold graph uses primary StageScript.cpp, real Stage Update entry,
and support sources AnmManager.cpp, AnmVmCreate.cpp, AnmVmId.cpp, RandomMath.cpp,
in that order. Every source uses `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL /GS`.
Thirteen project CPP/quoted-header inputs are frozen before/after. One native
job runs at nice10; Ghidra uses768MiB and one active processor.

`native-summary.json` records the selected source inputs, native diagnostic
sizes, complete533-byte/17-field guards, rejected controls, arithmetic-trace
limits and the separately frozen Enemy consumer. Diagnostic images use data
anchors for unresolved symbols and are never run. The two cold links that
originally established the five exact units remain documented in
[their original evidence](../stage-fog-native/README.md).

The source-extracted Fog/stage and Enemy fixtures erase calling-convention
keywords for the host. The transition fixture's timer, viewport and random
helper are unchanged; its strict LF-normalized pins are recertified only for
the reviewed easing declaration/definition. Public CI checks both the logical
fixture and line-ending rejection controls. Host tests are synthetic contracts,
not an x87/ABI or game-runtime Oracle.

Rebuild one actual graph for all five guards:

```sh
scripts/repo-python scripts/build-match-unit.py --unit stage-fog-constructor
for unit in stage-fog-constructor stage-fog-subtract stage-fog-add stage-fog-scale stage-update-object-vms; do
    scripts/repo-python scripts/compare-linked-function.py --unit "$unit" --json || exit 1
done
```

Native product, global storage, runtime, semantic and portability gates remain
open. The current frontier is compiler temporary allocation and easing-tail
sharing; none of this checkpoint changes the exact numerator or denominator.

Full public CI, toolchain declaration, tracking, generated-progress and whitespace
checks pass. Cleanup removes6,016,748 bytes of duplicate builds/source snapshots;
one selected Stage graph and the necessary Enemy consumer graph remain ignored.
