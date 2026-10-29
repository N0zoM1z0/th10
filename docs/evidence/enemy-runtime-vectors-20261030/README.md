# Enemy runtime vector copies — 2026-10-30

The complete2412-byte EnemyRuntimeUpdate remains **nonexact**, with a2346-byte
current candidate. This checkpoint restores target-observed vector temporary
operations and branch order. It adds **zero exact bytes**. The
[results](results.json) pin current source inputs and the bounded comparisons.

The target tests each position interpolation's nonzero duration before its
ordinary-motion alternative. Each evaluated pointer feeds a full Float3
subtraction and then an aggregate velocity copy. The old source put the ordinary
path first and wrote three scalar differences directly to the destination.
Using the existing Float3 value operations restores the intermediate component
stores and target0x18 frame. A captured baseMotion pointer serves the second
interpolation and subsequent motion, composition and bounds operations; it
recovers the delayed EBX pointer lifetime seen in the target.

The first vector change emits2410 bytes; the consumed pointer produces2346.
The earlier2362-byte source aligns337/671 normalized instructions, the vector
change383/671, and the current source440/671. A complete cold image reproduces
the latter result. These address-masked alignment counts are diagnostic only:
they neither bind every parent field nor establish a partial exact owner.
Remaining differences include a base-relative LEA, world-velocity aggregate
arithmetic, bounds, damage/callback control flow and timer scheduling.

The world-velocity target first adds base/offset positions as a vector, then
subtracts the old world position and copies the result. That is the next
concrete source lead. Current direct scalar stores retain different rounding
and storage schedules. EnemyAdvanceMotion stays160/171; its consumed vector-sum
control did not recover the target polar-Z store or final stack cleanup and is
restored. Do not repeat that control without new evidence.

One explicit canonical cold replay preserves the already exact scalar evaluator:
652/652 bytes, all10 semantic fields and its actual ESI/EDI interface pass.
The source changes only the nonexact runtime caller; no existing exact body or
declaration is edited. The six-input context retains established compiler profiles
and freezes24 recursive inputs. Comparator, matches, boundaries, origins and
generated progress remain unchanged. Product/runtime closure remains open.

```sh
scripts/repo-python scripts/replay-exact-units.py --unit enemy-evaluate-scalar-interpolation --json
scripts/repo-python scripts/ci.py
git diff --check
```

Current complete native diagnostics and cache pins are retained under
`.analysis/gpt-6.1-sol/enemy-motion-context-20261030/final-context.json`.
