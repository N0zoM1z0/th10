# Complete Player shot-update recovery — 2026-10-30

PlayerUpdateShots **00428280..004285EB** now emits the complete876-byte owner
in the actual12-source /GL graph, compared with738 bytes at entry. It remains
**nonexact**. A fresh compilation of all12 inputs, with no object reuse, reproduces
all876 native bytes at the same linked address. No new exact owner or byte is
accepted. The [results](results.json) keep the rejected complete comparison,
current graph/profiles/pins, cold proof and scoped exact guards.

Fresh PE/Capstone decode confirms the stack Player argument, RET4,128 rows of
stride5C and the unreachable six-byte alignment gap at+2A. No Ghidra observation,
profile sweep, register annotation or target patch is used. Original owner name,
TU and inline boundaries remain unknown.

The maintained caller now binds real source implementations: pending interrupt1
409E50, PendingThree40C4D0, polar XY vector44C5D0, angle wrap44BC70, motion44C2A0,
FindVm4491C0 and deletion4492A0. The old interrupt1 declaration incorrectly named
a delete-state operation. Ordinary declarations reproduce the observed callee
interfaces; the shared vector view moves to Enemy.hpp without changing its body.
Compile-time checks bind all fields of the two2C motion views. The shared motion
helper itself still emits160/171 nonexact; caller binding does not close it.

The primary-missing route now contains the complete secondary VM release:
check the ID, search primary then secondary lists, mark04000000 on a found VM
and its root child chain, and clear the ID unconditionally. A zero ID still
reaches this clear before the caller clears both IDs. An outer nonzero guard
skipped that target-observed store and changed the lookup registers; that earlier
candidate is superseded. Typed four-byte ID copies, a captured callback value,
consumed row iterator and motion/coordinate lifetimes recover the loop entry,
eight-byte frame, callback ABI, culling, position/angle stores and timer paths.

The remaining scheduling difference is precise: `lea esi,[edi-0C]` occurs at
candidate+111 instead of target+142. The ordinary-motion skip therefore enters
+145 instead of+142. Two REL32 fields in the moved span are three bytes late.
The unchanged strict comparator rejects the full22-field contract; equal size
and a small instruction difference grant no credit. A separate diagnostic
maps the moved instruction's span, checks all22 external bindings, and retains
all53 local destinations including this differing edge. The other272 ordered
instructions have one residual JMP destination under that explicit mapping.
It is a diagnostic, not an alternate acceptance rule, and no binary is modified.

Ten actual helper bodies pass strict same-image target binding:785 bytes/all22
fields, including all referenced pending, vector, wrap, lookup, deletion and
bounds interfaces. Five explicitly selected existing units pass canonical cold
replay: vector30, playfield bounds91, allocator181, planar bounds55 and interrupt65;
combined422 bytes/all11 fields in their three existing recipes. Seven previous
native candidates, including damage1172/1170, runtime2412/2412, scalar652 and
motion160/171, preserve every normalized ordinary byte and field identity.
Preservation of a previous candidate is distinct from TH10 exactness.

Bounded rejected controls retain source hashes and logs: index iteration loses
the frame; wrapper/member forms, direct lookup/manager captures and noinline
bounds are neutral; delaying the motion pointer or keeping only a velocity
pointer emits829 with the wrong iterator anchor. Two-scale-load timer and ID
forms are recorded at their own epochs. The one source macro typo failed before
link and is reported separately. Do not repeat neutral controls without new
TH10 evidence and a falsifiable prediction.

The current warm context is
`.analysis/gpt-6.1-sol/player-shots-context-20261030/final-context.json`:
43 recursive project pins and12 object hashes. Revalidate every pin/hash before
reuse. The five-input ANM context remains current; its13 pins were revalidated.
Older ten-input Player/Enemy contexts retain their source epochs. Original
bytes, private target, native images and experiments stay uncommitted.
All25 retained source controls reconstruct from checked line deltas; cleanup
removed41,825,141 bytes of superseded snapshots/builds while preserving logs
and the before/final/cold native witnesses.

```sh
scripts/repo-python scripts/replay-exact-units.py --unit enemy-runtime-vector --unit player-shot-outside-playfield --unit player-spawn-effect-row --unit player-planar-bounds --unit player-set-shot-vm-interrupt2 --json
scripts/repo-python .analysis/gpt-6.1-sol/player-shots-context-20261030/run-probe.py replay .analysis/gpt-6.1-sol/player-shots-context-20261030/final-context.json --cold
scripts/repo-python scripts/ci.py
git diff --check
```

Native runs use bounded serial CPU0/nice10 jobs with retained exit logs. No full
historical replay, comparator modification/self-test or host failure campaign
is part of this checkpoint. Runtime/product/portability closure is not claimed.
