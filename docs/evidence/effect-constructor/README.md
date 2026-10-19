# Complete transition factory through constructor ownership (2026-10-20)

StageCreateTransitionEffect at0x43C8B0 closes **82 authored exact bytes**.
The natural constructor now performs the existing whole-object reset and flags2
initialization after its existing timer member constructor. The factory simply
allocates, checks the result and calls Initialize. No data layout, real operand,
calling convention, profile or updater entry changes. The prior88-byte factory
source binding is superseded; initializer396 and cleanup127 remain nonexact.

## Target and source evidence

Fresh read-only local Ghidra attests the original JP1.00a target, its complete
mapped `.text`, all82 factory bytes and nine incoming direct references. The
existing reviewed boundary/origin are retained. The target's XOR EAX,EAX precedes
the allocation null test, followed by timer flags+0x40 read/mask, seventeen-word
zero-fill, flags2 and Initialize; both paths share RET20. Source uses the existing
AnmVmTimerView constructor, with no additional timer-flag statement. Full bytes
are reproduced naturally through historical C++ new-expression construction.
No explicit result-value workaround or forced inlining/outlining is added.

| Complete factory field | Independently decoded target |
| --- | --- |
| +4 REL32, CALL at0x43C8B3 | ordinary one-size allocator0x452493 |
| +72 REL32, CALL at0x43C8F7 | effect initializer0x43C970 |

`target-facts.json` pins the whole-owner hash and both call operands. The ordinary
allocator's complete14-byte pinned CRT proof is retained in the preceding
[allocator review](../effect-allocation-abi/README.md); it is not counted again.
The child initializer has a reviewed entry/complete396-byte owner but remains
nonexact. Public child-entry binding grants no child codegen credit.

This tests the constructor-ownership hypothesis suggested by the pinned MIT
reference's `matching2/msvc/include/th10/core/screen.hpp`, commit
`c6791d82dc9f02e55eca45a5314f900a85481efd`. The reference's extra timer statement,
noinline annotations and section pragmas are not adopted. Our constructor is
an ordinary out-of-class C++ definition. The target and historical compiler
independently establish the resulting codegen; a matching source model does
not identify a unique original constructor partition, class or production TU.
See [the recorded license](../../licenses/th10-decomp-MIT.txt).

## Complete cold replay and validation scope

Two independent cold links use the unchanged real14-TU Gui updater graph,
42 identical CPP/quoted-header inputs and the existing /GL profile. The first
control isolates constructor ownership; the second executes the formal new
canonical recipe. Different PE/PDB identities and complete strict results are
recorded in `native-summary.json`. All82 bytes/two fields pass. Eight existing
same-graph units retain **591 bytes/all24 fields** in both images:673 complete
bytes/all26 fields per image, with no duplicate or constructor-only credit.

```sh
scripts/repo-python scripts/build-match-unit.py --unit transition-effect-factory
scripts/repo-python scripts/compare-linked-function.py --unit transition-effect-factory --json
```

The header is included only by TransitionEffect.cpp. No caller prototype, shared
Chain/timer body or pinned dependency changes in this unit. The eight same-graph
guards are the actual native validation scope; there is no historical full
native-cohort or repeated StageDraw build. The diagnostic data-anchor image is
not executed and is not a native product.

The actual-source host baseline passes14 scenarios/843 assertions, including
prefilled construction, all seventeen words at registration, null allocation
and all callback/lifecycle observables. The existing init_flags negative control
now selects the real constructor body; it compiles and fails only with the
expected assertion/exit86. No host implementation, fixture or allocation seam
is substituted. All dependency pins/normalization policy remain unchanged.

Allocation handlers/throw behavior, prezero timer-flag object lifetime, physical
global storage and native product/runtime remain open. Exact integrated factory
bytes do not establish an independent target constructor extent or grant one.

Canonical default replay, pinned toolchain execution/declarations, tracking,
generated progress, full public CI and whitespace checks pass. CI also passes
all19 unchanged EOL pin controls. `validation-summary.json` records the actual
scope and cleanup of4264812 bytes from the completed control and unused old
callback-group output. The final shared PE/MAP/PDB is retained for the nonexact
initializer/cleanup frontier. Pre-existing untracked D3D work is untouched.
