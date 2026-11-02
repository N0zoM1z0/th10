# TH10: 7,020-byte ECL Run LENGTH_SQUARED stack-home recovery

Checkpoint 2026-11-02. The complete generic ECL interpreter
EclVmContext::Run at 0x0044E1A0 remains NONEXACT.
The maintained source now reproduces 6997/7020 ordinary bytes,
versus the independently documented prior 6995/7020,
without changing the owner extent, 189 original independently
bound location/target fields, 62 local code/data destinations,
control-flow table, or terminal selector byte.

## Actual retained source change

The ECL_VM_LENGTH_SQUARED case reads two scalar operands,
squares them and stores their sum through ResolveFloat(0).
The first scalar is now a source-local const volatile float x;
the second stays const float y. This preserves the two operand
service calls and the same mathematical and output semantics while
influencing VC7.1's reuse of the first scalar in the x*x calculation.
It is a codegen observation, not evidence that the original source
used volatile or was synchronized across threads.

Full native owner comparison under the existing actual
EclVm.cpp + Enemy.cpp /GL ECL host source graph repairs two
bytes at offsets +0x181B and +0x182B, with no newly different
ordinary byte offsets. Both are local x/y stack-home displacements
in the squared-length calculation; the other 23 remaining ordinary
differences are not silently ignored or redefined as relocations.
The source and original EXE are hash-pinned in native-summary.json.

The comparison checks all 189 field locations and every existing
original target destination, not just the masked ordinary bytes.
The final target-owned selector byte at +7019 is data following 7019
decoded instruction bytes; it is still compared. This remains a
complete 7020-byte proof of incomplete reconstruction, not a
cropped prefix. The original source, production TU, whole-game
link and runtime are not established.

## Bounded regression

The selected real source-graph cold replay passes six separate existing
canonical exact owners, 859/859 bytes: ReadFloat183, ReadInt144,
ReadFloatValue171, ResolveFloat93, StackPop122, HostRun146.
Other large ECL functions, including the full ECL Run owner, get
zero new exact credit. Rejected controls retained the same
full 25 differences, or emitted 7028/7036, 6928 or other
wrong-sized functions; they have been restored. Only this
measurably improved one-line source change is retained.

Selected reproduction (no historical full cold replay):

    scripts/repo-python scripts/verify-target.py
    scripts/repo-python scripts/replay-exact-units.py \
      --unit ecl-vm-host-run --unit ecl-vm-read-float \
      --unit ecl-vm-read-int --unit ecl-vm-read-float-value \
      --unit ecl-vm-stack-pop --unit ecl-vm-resolve-float --json
    scripts/repo-python scripts/ci.py
    git diff --check
