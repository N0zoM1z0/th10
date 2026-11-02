# Recover missing ResultMenu callback-retirement owner (2026-11-02)

The Japanese TH10 v1.00a `.text` interval **0x00422200..0x0042221B** is a
previously untracked **28-byte** independent function, immediately followed by
`CC` alignment and the already inventoried complete controller cleanup
0x422220. Its 28 original bytes decode fully with Capstone and terminate in
`RET`; the previous 3-byte owner at 0x4221F0 ends at 0x4221F2. No tracked
function range overlaps the newly recovered interval. Original target
SHA-256: `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Whole original 28-byte owner SHA-256:
`ff7169a1ffda26fb648d1b9021d0a6affadd599732ae8acf918d02ca1872f6d1`.

Original behavior: the private EAX receiver holds the ResultMenu controller;
it reads pointer fields at +0x08/+0x0C, tests each, and sets bit 1 of the
corresponding `ChainElem::flags` at +0x04 if non-null. A shared EDX=2 constant
survives both tests. There is **no** resource free, chain removal, or global
mutation in this function. The larger `GameCleanupController422220` handles
actual list unregistration and other resources.

`src/ResultMenuLifecycle.cpp` now provides the natural typed model
`ResultMenuMarkTasks422200` with both side effects and original null guards.
In the selected real VC7.1 SP1 `/GL /GS /EHs` ResultMenuLifecycle+Chain
producer it yields **26 PDB-owned bytes versus the original 28**, with no
external link fields. It uses an ECX receiver rather than target EAX and lacks
the target's second-register handoff. **NONEXACT**: no match-unit admission,
no exact byte credit, and no mistaken 26-byte prefix claim. The physical
original TU and authorship are unknown: the origin is **indeterminate**.
The fully decoded new target boundary is recorded independently of original
source authorship, and both ledgers are updated conservatively.

## Scoped validation

Build only the changed two-source producer and inspect the full target plus
complete native PDB extent:

```sh
scripts/repo-python scripts/verify-target.py
nice -n 10 scripts/repo-python scripts/build-match-unit.py --unit result-menu-creator
scripts/repo-python docs/evidence/result-menu-mark-tasks-20261102/verify-native.py
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit result-menu-constructor --unit result-menu-initialize \
  --unit result-menu-creator --json
scripts/repo-python scripts/ci.py
```

The proof is source/codegen for a bounded native graph only; it does not
establish any original class declaration, origin, full game or runtime.
