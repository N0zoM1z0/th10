# TH10 ECL dispatcher: close typed pattern and extra-record strides

Original input: Japanese TH10 1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The **complete** `EnemyRuntimeView::DispatchEclInstruction` owner starts at
`0x0040E770` and occupies **14,416 original bytes**. It is still **NONEXACT**.

## Actual source recovery

The dispatcher previously recovered the native `EnemyBulletExtraView` overlay
but still indexed its variable-length bullet data through `0x16` and `0x18`
magic multipliers. Opcode `0x199` now derives the original 22 records per
pattern from `sizeof(EnemyBulletPatternView) / sizeof(EnemyBulletExtraView)`
and the 24-byte record stride from `sizeof(EnemyBulletExtraView)`. The
existing independent layout assertions bind `0x210 == 22 * 0x18`, the
record's six field offsets, and its actual first origin `runtime +0x2E4`.
The extra-record view *overlaps* the pattern tail; it is not a bounded
standalone 22-element array with independent storage.

The bullet-pattern firing path now derives its existing dword cursor from
`offsetof(EnemyRuntimeView, bulletPatternOffsets) / sizeof(unsigned int)`
and its 3-dword stride from `sizeof(PlayerFloat3) / sizeof(unsigned int)`.
The native `bulletPatternOffsets` array begins at `runtime +0x1344` and
contains 12-byte vectors. The source retains the original *signed* integer
address-calculation lifetime, script operand order, and unguarded indexing.
It does not introduce a fictitious range check or change any target flags.

## Native entire-owner evidence

Because `EclVm.cpp` and `EclVm.hpp` were changed by the adjacent FORMAT
restoration, we did **not** reuse the stale earlier ECL support object.
We freshly compiled **all 11 actual ECL-graph translation units** with
pinned VC7.1 SP1 `/GL` and per-source profiles (including the `/Ob1`
ANM helper). Only this selected ECL boss graph was built, **not** the
full historical exact-unit cohort. The complete linked PDB contributions are:

- ECL dispatcher: **14,352 candidate bytes**, **716** decoded linked fields,
  with identical field positions/types and **0** non-field byte differences
  from the previous complete ECL candidate.
- `EnemyBeginSpell`: **2,432 candidate bytes**, **108** decoded linked fields,
  identical field positions/types and **0** ordinary differences against the
  previous complete candidate.

These are **candidate-to-candidate** invariants; the original ECL dispatcher
is 64 bytes longer and does not have a complete original-bound field proof.
No 14-KB exact claim or match-ledger entry follows from preserving 716
candidate fields. This refutes the assumption that a typed source
restructuring alone closes the original body.

Four selected original-target exact replays all passed: resource package
**239/239**, ECL forwarder **11/11**, integer reader **16/16** and raw float
reader **26/26**; **292/292 previously accepted bytes** remain valid.
Seven-stage CI and whitespace check passed. New exact credit: **zero**.

The machine-readable report includes all 11 source SHA pins, the changed
header pins, complete linked-owner sizes/fields and selected replay results:
[native-result.json](native-result.json). Native PE/MAP/PDB and compiler
logs remain ignored under `build/gpt-web/20261105-typed-strides/`.
