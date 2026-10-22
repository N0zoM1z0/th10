# ECL Run translation-unit visibility controls

At8bbfef7, three actual private-source controls split existing function bodies
out of EclVm.cpp while preserving their code, signatures and the GL/GS profile.
They test whether same-unit compiler visibility explains Run's remaining bytes.
No maintained source, profile, recipe, ledger or exact count changes.

| Moved definitions | Complete Run | Complete same-image guards |
| --- | --- | --- |
| Stack constructor, Push, Pop, EnterFrame, LeaveFrame |7020 bytes,27 differences|9 functions,1137 bytes,18 fields |
| HostRun, the actual direct caller |7020 bytes,27 differences|9 functions,1137 bytes,18 fields |
| ReadIntValue and its unchanged private flag helper |7020 bytes,27 differences|9 functions,1137 bytes,18 fields |

All three complete target-replayed Run bodies equal the independently checked
baseline, SHA-256
`830db1c50fc4f1228d0bfbf99f08c51e9d9e36043990d2900820c8b55600a472`.
All189 fields,62 local destinations,60 table words and88 selector bytes remain
covered, together with the complete171-byte ReadFloatValue callee. The generic
normalization flag remains false; the reviewed final selector byte is explicitly
covered. Equal scores alone were not used to infer equal complete bytes.

Each control compiles two actual ECL inputs and reuses nine hash-verified game
objects plus two pinned harness files. They are eleven-TU diagnostic links,
not full cold acceptance builds. No missing game function is replaced by a new
anchor. Guard PDB owners follow the actual moved source, rather than selecting
the old object's contribution. ANM and Enemy extents stay9964/14352; no whole
byte identity or new target equality is inferred from those sizes.

Fresh disk target, toolchain execution and tracking pass. Fresh local read-only,
noanalysis Ghidra confirms the two Run calls inside HostRun and preserves all
412289 mapped text bytes before/after rollback, with CPU1/heap768 MiB. Observed
separate machine functions justify the experiment, but do not prove original
source-file boundaries. Moving these bodies while retaining GL is neutral only
in these measured contexts; other optimizer contexts remain open.

[Evidence and pins](evidence.json), the removal patches and matching services
sources describe each precise split. The services sources contain existing
maintained C++ definitions, not decompiler output. Source/header, canonical
native/recipe and nine unrelated D3D pins remain unchanged. Focused toolchain,
tracking, progress and whitespace checks pass; no full CI per operator scope.

Hash-checked cleanup removes34 superseded files/2137686 logical bytes. Retain
the last split-int-service carrier, all private source pairs/results/producers
and the canonical baseline. Private recovery:
`.analysis/gpt-6-astra-medium/ecl-tu-visibility-20261022/RECOVERY.md`.
Next rotate to ANM's concrete operand/common-tail instruction differences;
do not repeat these three neutral ECL partitions. Large owners remain active,
with no small-function closure substituted. Product/runtime and semantic/port
stages remain open.
