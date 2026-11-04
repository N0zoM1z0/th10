# TH10 virtual type-0 laser point contact @0x0041E4D0 — 226-byte source, NONEXACT

Target: original Japanese TH10 v1.00a PE, pinned in `config/target.toml`.
The dense, independently reviewed executable extent `0x0041E4D0..0x0041E5B1`
is **226 bytes** with native ECX object receiver, two stack arguments
(point address and float radius), `RET 8` and no overlapping entry.

## Type ownership recovered from the original *vtable*, not guessed

The original `EnemyLaserLineView` vtable is at **0x0046DA60**. Its actual
32-bit table entries resolve as follows:

| Slot | Original executable target | Maintained type-level interpretation |
| --- | --- | --- |
| +0x10 | 0x0041D870 | `Unlink()`, 3-byte EAX=0 leaf |
| +0x14 | 0x0041E260 | `Clear(int)`, 609-byte nonexact owner |
| +0x18 | 0x0041D880 | `CheckCollisionBox(...)`, 1269-byte nonexact owner |
| +0x1C | 0x0041DD80 | `CheckCollisionCircle(...)`, 1238-byte nonexact owner |
| **+0x20** | **0x0041E4D0** | **`CheckPoint(EnemyLaserVectorView *, float)`** |

These exact pointer values are read from the original `.rdata`; they are
not inferred from adjacency. The previous header used **incorrect
`UnknownSlot10`, `UnknownSlot1C` and `UnknownSlot20` declarations**.
`src/EnemyLaserLine.hpp` now gives those slots the real typed return and
argument signatures, without changing object size, data fields or physical
slot order. Existing `EnemyLaserConstructionBaseView::CheckPoint` also
uses virtual slot +0x20. Consequently the newly source-present, separately
reviewed 54-byte `EnemyLaserManagerStorageView::AccumulatePointChecks`
operates on **this exact virtual method** when visiting a type-0 laser.

The corrected interface and its actual 226-byte implementation are in the
existing `EnemyLaserLineView` (`src/LaserLinePoint.cpp`), not in a competing
shadow or duplicate laser model. Original source authorship/TU remains
*indeterminate*: a reviewed vtable pointer proves the callable object method,
not which historic source wrote or generated it.

## Native x87 rotation and collision result

The original computes all three position differences between the caller's
point and the line's `position` at +0x24/+0x28/+0x2C. It explicitly stores
all three in a local float3 even though Z is not used by the subsequent
XY collision result. It loads negative angle from +0x3C and evaluates
`FSIN` and `FCOS` in the x87 extended register stack. It then computes
local X and Y with the normal 2D rotation, rounds X through a float slot,
and checks the radius-expanded rectangle:

- local X minus radius must not exceed the length at laser +0x40;
- local Y minus radius must not exceed half the width at laser +0x44;
- local X plus radius must not fall below zero;
- local Y plus radius must not fall below negative half-width.

The native uses `FCOMP/FNSTSW AX/TEST AH` rather than ordinary C++ ordered
relational operators. Its four gates accept **unordered NaN comparisons**;
the maintained code uses **negated strict rejection predicates** to preserve
that edge behavior. Result is exactly **2** for an accepted collision and
**0** otherwise; the third input component Z is calculated but not used in
the decision. No artificial bounding guard, EPSILON, geometry simplification
or alternate angle normalization is introduced.

## Reproducible pinned compiler exploration

A natural pinned VC7.1 SP1 `/GL` source expression first emitted a **170-byte**
PDB-owned function vs target **226** (7/214 preliminary ordinary agreement).
That initial form allowed VC7.1 to eliminate the native Z-difference
computation. Explicitly retaining the original local float3 computation
using a volatile local aggregate (not byte patching or register-forcing)
restores the whole native 3-component side-effect-free arithmetic shape and
produces **200/226 bytes**, **36/214** ordinary bytes and three candidate
constant fields. This is the selected maintainable source baseline.

Four additional real compiler controls tested a shared float/double negated
angle, 32-bit versus extended Y result lifetimes, and alternative float
sine/cosine storage. None improved the 200-byte selected candidate (sizes
192–200). Recompiling in two independently real source graphs with actual
`LaserClear.cpp` and then `LaserClear.cpp + EnemyLaser.cpp` left it at
**200/226**, 36/214; hence the remaining discrepancy is *not* resolved by
these natural WPO contexts. All original source files were preserved around
experiments. Private reproducible reports and controller:
`.analysis/gpt-web/20261103/laser-line-point-226-{probe,variants}.py`,
`laser-line226-angle-results.json`, `laser-line226-real-tu-results.json`.

There is **no canonical byte match**, full address-field contract, virtual
call ancestry claim or runtime/product exactness for this 226-byte owner.
The established type-1 laser update and type-0 constructor exact units,
as well as the real draw Chain functions, are guarded by scoped cold replay
only; no full cold replay is needed for this milestone.
