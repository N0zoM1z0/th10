# Type-0 laser Bounce: real virtual owner and ordered predicates

The original Japanese TH10 v1.00a target is SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
The complete authored owner is `0x0041CFD0..0x0041D169`, **410 bytes**.
This checkpoint adds **zero exact credit**; the installed complete candidate
is **510/410, NONEXACT**.

## Original facts and installed source

Direct original PE reads bind type-0 vtable `0x0046DA60 + 0x3C` to
`0x0041CFD0`. The real Update calls that same slot at `0x0041D45F` under
mask `0x08000C00`. The old independent
`EnemyLaserBoundaryStateView::UpdateBoundary` model had no generated virtual
reference; the fresh four-input Update graph emitted498 bytes with a stack
receiver and RET4. Its historical402-byte isolated result was a different
source/entry epoch.

The body now implements `EnemyLaserType0StorageView::Bounce` on the existing
type-0 construction class. Its generated vtable points to the actual body,
and the compiler retains **ECX receiver / plain RET**. Existing construction,
storage layout and virtual slots are preserved. The known type-0 line view
supplies the typed member offsets; original class/TU spelling remains unknown.
The temporary declaration-only `EnemyLaserLineView::Bounce` model still
emitted498 with the wrong physical ABI and was superseded.

The original Y reflection condition is the direct ordered expression:
reflection is enabled, and Y is below minimum or is at/above maximum with
the bottom flag set. Unordered Y skips reflection. The old negated
`Y >= minimum` form incorrectly included unordered values. The installed
predicate recovers the target TEST-AH-5/JNP then TEST-AH-1/JNE instruction
shapes. This is instruction evidence, not an executed NaN test or a broader
semantic validation claim.

Whole-vector request assignment also recovers the original three memory-word
reads, including Z. The speed word at object `+0x13C` is the first value in
pattern4; it is copied to type-0 request speed at object `+0x444`. All17
original image fields are independently decoded and recorded in the
[candidate contract](candidate-contract.json), outside the canonical registry.

## Complete comparisons and remaining frontier

Follow-up factory controls use all ten real call sites in the same four-input
graph. Standard stdcall and a complete manager-member conversion both emit
factory157/RET8 with the request still on the stack; Bounce emits507 and533.
Each retains Update1001/all31 and both constructors202/219/all4. Neither
restores the target EDI request/RET4 interface. Both controls are restored;
calling-convention spelling is parked pending new caller-lifetime evidence.
Full receipts remain under `.analysis/gpt-web/20261106-laser-factory/`.

The first actual-vtable source emitted494 bytes. The final typed/ordered
source emits510 in both the private native link and the independent canonical
Update-entry cold. The strict comparator rejects the complete510-byte PDB
contribution against410; no shortened comparison or normalized score is used.
One FireLaser path is inlined, the second retains an extra stack argument,
and the receiver/fired registers still differ from the original. These are
the next factory/context issues; no fake ABI, noinline annotation, padding or
profile change was introduced.

Selecting Bounce itself as the link entry did not improve510 and changed
the existing Update from1001 to1009. That entry control is rejected. The
original four-input `/GL /GS` Update recipe remains installed:

```sh
scripts/repo-python scripts/replay-exact-units.py --unit laser-line-update --json
```

The final canonical cold passes existing **Update1001/all31**. Explicit
same-native comparisons also pass **polar helper30** and **sound queue123/all1**
in both the baseline and final graphs. Thus the final native guards cover
**1154 existing exact bytes /32 fields**; the separate canonical selector
covers1001/all31. No new match, boundary, origin or mapping count is added.
The provisional function name is reconciled in the existing source ledgers.

[Compact results and source pins](native-result.json) identify all measured
source epochs and the generated virtual-slot binding. Full receipts, source
controls and read-only target/native dumps remain privately under
`.analysis/gpt-web/20261106-laser-boundary/`. Canonical output is
`build/match-linked/LaserLineUpdate/`. No historical cohort, host failure
scenarios, runtime or product-closure tests were run; those phases and
original source ownership remain separate.
