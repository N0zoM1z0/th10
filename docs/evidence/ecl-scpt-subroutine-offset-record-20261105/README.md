# TH10 ECL SCPT serialized subroutine offsets: real 4-byte record view

Verified Japanese TH10 1.00a original SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
This source change affects the 589-byte original `EclVmScriptDatabase::AddScriptData`
owner at `0x00450220` and its shared 7,020-byte ECL interpreter graph.
Both remain **NONEXACT**. No copied binary code, simulated source spelling or
partial-match admission was introduced.

The existing SCPT header is 0x24 bytes. After its counted include payload,
the native `AddScriptData` accesses one 4-byte subroutine instruction offset
per entry before walking a contiguous string/name stream. The maintained
`EclVmSerializedSubroutineOffsetView` now represents each encoded 4-byte
record, checked with `sizeof` and `offsetof`. The former `unsigned int *`
array is replaced by a **volatile pointer variable to the actual record
layout**. Both the initial insertion loop and sorted merge use its typed
`instructionOffset` field. Counting, name boundaries, `REP MOVSD`, allocation,
conditional release, and sorted insertion order remain unchanged. This is an
on-disk representation, not a new allocated table or source-name claim.

A real pinned VC7.1 SP1 `/GL` compilation and link of `EclVm.cpp` and
`Enemy.cpp` freshly rebuilt the *entire* selected graph. PDB contributions
and all candidate linked field positions/types versus the prior checked graph:

- ECL VM `Run`: 7,020 bytes, 189 fields, zero ordinary source-candidate differences.
- `AddScriptData`: 570 bytes, 2 fields, zero ordinary differences against
  its previous 570-byte candidate. Original target is 589, so still NONEXACT.
- `FindSubroutine`: 136 bytes, no fields, unchanged candidate geometry.

The large interpreter still has the independently documented 23 original
ordinary differences from its prior target-bound baseline; candidate equality
is **not** a fresh target exact certificate. The full PDB contributions and
source hashes are captured in `native-result.json`; ignored native PE/MAP/PDB
under `build/gpt-web/20261105-ecl-serialized-offsets/full-run/`.

Selected original-target cold replays on the **installed modified source**:
`ecl-vm-host-run` 146/146, `ecl-vm-find-subroutine` 136/136,
`ecl-vm-read-int` 144/144, `ecl-vm-read-float` 183/183. Total **609/609
previously accepted bytes** passed, with **zero new exact credit**. No full
historical replay, complete Windows product build or runtime claim.
