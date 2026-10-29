# Player effect-row allocator and damage geometry — 2026-10-30

Player::SpawnEffectRow at **0x00427B50..0x00427C04** is now complete181-byte
exact, including its sole resolved field, in two independent canonical cold
links. This adds **181 authored exact bytes**. The larger damage owner remains
**1172/1170 nonexact**; its improvement grants no partial exact credit.
The [results](results.json) retain both exact witnesses and bounded diagnostics.

The fresh target decode confirms181 contiguous instruction bytes, two RET20
exits at+22/+B2, three local branches and CC padding on both sides. Its two direct
callers are PlayerUpdateCallbackBody at425CCE and CalculateDamageToTarget at
428863. There is no external call inside this owner; the only address field is
DIR32 at+7E, binding g_AnmGameSpeed to476F78. Every ordinary byte, local destination
and complete PDB contribution is compared, rather than a selected prefix.

The old free function emitted177 bytes and plain RET at both exits. Its real
callers cleaned five stack arguments. An ordinary Player member declaration and
member calls naturally produce the target private EDX receiver, five stack
arguments and RET20, without a register annotation or changed profile. Capturing
rowPosition before the second clear recovers the ESI-relative Y/Z stores as well
as X. These two steps close the full181-byte owner. The logical member view is
supported by actual field ownership/calls; it does not prove the original class
name or production translation unit.

The source preserves scanning32 usable6C-byte rows and returning row32 unchanged
on exhaustion within the maintained33-row storage. It retains the full-row clear,
flags3, second2C-byte prefix clear before sequential XYZ reads/copies, radius and
delta, timer setup, damage, accumulator0, cap999999 and interval4. The source
pointer may overlap destination storage; no bulk-copy replacement was introduced.

Canonical acceptance reuses the existing three-input Player-constructor recipe:
Player.cpp with AnmManager.cpp and Gui.cpp, pinned VC7.1 /O2 /GL profiles and the
real Player constructor as entry. The first cold explicitly selects the new
allocator and existing interrupt helper:246 bytes/all3 fields pass. The second
independent cold selects only the allocator:181/all1 passes. Strict same-image
checks in the separate ten-input diagnostic also pass181/all1 and65/all2. Only
affected selectors were replayed; no historical cohort, comparator change or host
failure/sanitizer scenario is part of this claim. The anchor image is non-product.

The complete damage owner recovers the rectangle-first branch, lazy axis-aligned
Y half-extent, separate negative-half literals for rotated bounds, consumed row
iterator/radius pointer, collision-cap branch and direct consumption of the
returned VM-ID temporary. The latter restores the returned-pointer read and the
saved-angle/result-buffer homes. Descriptor and late target-size reloads remain.
The final candidate is1172 bytes with355 instructions;325 align diagnostically
against355 target instructions, versus276 at the preceding1175-byte checkpoint.
Bound vectors still occupy different stack homes, the shot damage increment is
a load/add/store rather than target memory-add, and rotated Y arithmetic and
loop alignment differ. Equal instruction counts do not close this owner.

The EnemyRuntimeUpdate source is restored after rejected operand/snapshot
controls. Its complete prior2412-byte normalized candidate and all89 local branch
destinations survive the Player changes; it remains nonexact against TH10.
Swapping whole vector operands changes only X and does not fix Y/Z. A compound
vector snapshot grows the parent2435 and is rejected. The constructed rotated
vector/separated comparison control does not close the damage differences; do
not repeat these controls without a new discriminating observation.

Current ten-input warm diagnostic,36 recursive source pins and ten object hashes:
`.analysis/gpt-6.1-sol/enemy-residual2-context-20261030/final-context.json`.
This is a validated warm successor of the previous cold context; the new exact
owner's cold evidence is the separate canonical three-input recipe. Native
cold-a/cold-b outputs and logs are retained below the matching build/scratch roots.

```sh
scripts/repo-python scripts/replay-exact-units.py --unit player-spawn-effect-row --unit player-set-shot-vm-interrupt2 --json
scripts/repo-python scripts/replay-exact-units.py --unit player-spawn-effect-row --json
scripts/repo-python scripts/ci.py
git diff --check
```
