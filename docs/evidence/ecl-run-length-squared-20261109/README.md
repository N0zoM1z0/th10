# ECL Run: retain explicit squared-length output evaluation order

In `EclVmContext::Run`, opcode `ECL_VM_LENGTH_SQUARED` reads two scalar
operands and stores their squared-length in `ResolveFloat(0)`. The target
instructions read both operands, call the destination resolver, then multiply
the second value before the first. The maintained C++ now obtains the output
pointer explicitly before the squares and expresses the multiplication in
that order.

This settles an outstanding local experiment as a source-level expression
dependency; it is **not** a new exact owner. The pinned VC7.1 selected
`ecl-vm-host-run` two-TU linked PE still has the complete 7,020-byte
`Run` contribution with **6997/7020** target-bound matching bytes across
**189 original call/data fields**, and the affected independent callee guard
is **171/171 exact**. All 23 original mismatch offsets remain. Reordering
or removing the volatile source home generated no accepted improvement in
prior trials.

Reproduce only the selected graph:

    scripts/repo-python scripts/build-match-unit.py --unit ecl-vm-host-run
    scripts/repo-python scripts/diagnose-ecl-run-native.py --unit ecl-vm-host-run

The diagnostic exits nonzero for the known nonexact `Run` owner; its JSON
output and the 2026-11-09 source-specific receipt remain in ignored
`.analysis/gpt-web/20261109/ecl-run-handoff-dirty-source.json`. This is a
record of the *current source epoch*, not evidence of original source spelling.
