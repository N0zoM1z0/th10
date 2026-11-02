# TH10 SptTestCreate: complete 67-byte exact factory — 2026-11-02

The original Japanese TH10 1.00a factory at `0x0042BF30..0x0042BF72`
allocates an SptTest owner, calls its real constructor and task registration,
and on failure calls the separate cleanup owner before operator delete. Its
complete **67-byte** PDB-owned native function is **67/67 codegen exact**, with
all five original `REL32` fields strictly bound: operator new (`0x452493`),
`SptTestView::SptTestView` (`0x42BB30`), `RegisterTasks` (`0x42BBD0`),
`SptTestCleanupOwner42BC30` (`0x42BC30`) and operator delete (`0x4524A1`).
The last cleanup is an unresolved real dependency with stack argument and
`RET 4`; no substitute cleanup code or invented register ABI was added.

The source in `src/SptTest.cpp` now retains explicit cleanup/delete and null
branch, and outlines constructor/registration naturally through noinline
member definitions. The separately maintained constructor is 145/145 exact;
registration is 84/84 exact. A selected canonical five-source LTCG `/GL
/GS /EHs` replay of **only these three units** passes 296/296 bytes.
A second independent creator-only scoped cold build also passes 67/67;
the PEs and PDB GUIDs differ, proving this is not reuse of a single cached
link. Original physical translation unit and runtime equivalence remain unknown.

| Selected cold | Creator | Native PE SHA-256 | PDB GUID |
| --- | ---: | --- | --- |
| Three-unit owner graph | 67/67 | `a30f45c71116f28da40a4e8820159a93f8db79042604c9b102ec29a97e25187c` | `5fc88adf56438d4b9c2f3dd99ed005a0` |
| Independent creator cold | 67/67 | `2ac4f66a89e68388e0ab5e01f0d2037ae02dbbe1810fc7249b3f127c80046f5b` | `b201e75dffc8164fbb8781924cda3578` |

The target cleanup at `0x42BC30` is **not** declared exact by this source or
by the factory's bound call. Product closure/runtime and portability remain
separate. No historical all-unit cold replay or host lifecycle simulation was run.

Reproduce:

```sh
scripts/repo-python scripts/verify-target.py
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit spt-test-create --unit spt-test-constructor \
  --unit spt-test-register-tasks --json
scripts/repo-python scripts/ci.py
git diff --check
```
