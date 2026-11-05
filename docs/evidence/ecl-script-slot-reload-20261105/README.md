# TH10 ECL AddScriptData: restore file-slot reload after header validation

Target: original Japanese TH10 v1.00a `0x00450220`, complete **589 bytes**,
SHA-256 `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

## Source and original instruction evidence

The original function stores the supplied SCPT header into a file-table slot,
checks its magic and version, then **reloads the header pointer through the
slot** before using the counted include length and subroutine count. Its
actual instruction at relative `+0x3B` is `mov eax,[eax]`, where EAX still
addresses the file pointer slot. The prior maintained `AddScriptData`
`EclVmScriptFileHeader **const fileSlot` declaration let the optimizer
reuse the already validated file pointer in a register, eliding this second
load. This was an observable departure from the target's alias/lifetime
schedule, rather than a problem with the serialized SCPT fields.

The installed source now declares a **pointer to volatile file pointer**:
`EclVmScriptFileHeader *volatile *const fileSlot`. This reads and writes
**the very same `files[fileCount]` table entry** and retains its existing
validation/failure paths. It forces the needed second slot load without
adding an allocation, a new field, target bytes, synthetic padding or a
fabricated private ABI. `volatile` is a compiler-visible alias/lifetime
model of the observed reload; it is not a claim of inter-thread mutation or
original compiler-era source spelling.

## Current full-owner native comparison

A fresh pinned VC7.1 SP1 two-source `/GL` `EclVm.cpp` + `Enemy.cpp`
build emits a **585-byte** complete PDB `AddScriptData` contribution, up
from the preceding source epoch's **570 bytes** and within four of the
**589-byte original**. This is a structural/semantic advance, not a byte
match: the original `REP MOVSD` is at **+0xFB** while the new candidate's
is at **+0xFE** (prior candidate +0xFC). Two candidate REL32 fields are now
at `+0x70/+0x108` rather than the original `+0x6C/+0x101`, so there is
**no original-bound exact acceptance**. An exploratory opcode/operand token
alignment counted 116 matching tokens versus 108 for the preceding source;
this metric does not measure exact bytes or prove the missing source.

The same fresh graph retains the complete **7,020-byte** generic
`EclVmContext::Run` owner, **189/189** independently original-bound
fields and **6,997/7,020** matching ordinary bytes. Run remains NONEXACT;
no other boss is promoted. Selectively replayed, independently
original-target-bound exact leaves are `ecl-vm-find-subroutine` **136/136**,
`enemy-ecl-resource-load-file-0040cd20` **95/95**, and
`enemy-ecl-resource-load-package` **239/239**, **470/470** in total.
No historical all-unit cold replay occurred.

Bounded alternative compiler trials on the installed prior source epoch:
forcing the entire `file` local volatile grew the owner to **614** and
regressed Run's complete decoding; removing the volatile offset cursor
reduced the owner to **563** and moved `REP MOVSD` to +0xF0; moving the
previous-table read behind the count was byte-neutral at **585**, or
regressed to **577** with a nonvolatile old table. All such trial sources
were restored without a commit. The chosen direct-qualified form produces
the same complete native codegen as a cast-qualified file-slot pointer.

Private reproduction and SHA-pinned current source:
`.analysis/gpt-web/20261105-turn-ecl-addscript/trial.py`,
`.analysis/gpt-web/20261105-turn-ecl-addscript/current-install/verify.py`,
`build/gpt-web/20261105-turn-ecl-addscript/current-installed/native-result.json`,
and the two selected-exact receipts in `.analysis/gpt-web/20261105-turn-ecl-addscript/`.
Current `src/EclVm.cpp` SHA-256:
`e964d47ad4336950065be90bf3be82981c818a666e12e2810f1e77c537b912c1`.
Neither product closure, runtime semantics, nor portability is established.
