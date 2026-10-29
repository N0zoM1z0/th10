# FrontEnd launch and VM receivers — 2026-10-29

Four complete authored owners add **520 exact bytes/all37 fields**. The previously
exact shared65-byte interrupt helper moves to its own source without new credit.
Two independent canonical27-input colds pass all five: **585 bytes/all39 fields**.
The newly inventoried game-specific thread adds168 reviewed authored denominator
bytes. Three new source mappings cover455 bytes; the65-byte FrontEnd mapping is
renamed to its maintained member API, and the shared helper mapping is retargeted.

| Complete owner | Bytes | Fields | Selector |
| --- | ---: | ---: | --- |
| Load thread `42C9F0..42CA97` | 168 | 12 | `frontend-load-thread` |
| Task/resource setup `42CAA0..42CB5F` | 192 | 18 | `frontend-register-tasks` |
| Create VM `42C670..42C6CE` | 95 | 5 | `frontend-create-vm` |
| Deferred VM interrupt `42C770..42C7B0` | 65 | 2 | `frontend-mark-vm-interrupt` |
| Existing shared interrupt `409E50..409E90` | 65 | 2 | `enemy-mark-pending-interrupt` |

## Actual thread and registration

The thread was absent from the old function inventory and database functions.
Complete raw decoding includes both plain RET exits and three internal alignment
bytes. Four CC bytes separate the preceding constructor; eight CC bytes precede
the next192-byte entry. Every thread byte was queried in attested read-only
Ghidra. Only factory data references42CD82/42CD8B reach the entry; interior
references are its own jumps. Game-specific registration, Supervisor mode writes,
Startup timing and callback flag updates establish authored origin independently
of compiler equality. Original class/TU declarations remain unknown.

The prior handoff incorrectly called477820 a statistics owner. It is the existing
Startup object; the wait consumes frames+3EC, bounded by300 or Supervisor flag80,
with16ms sleeps. A shared header moves the existing3F0-byte view unchanged and
asserts+3EC. Failure chooses next mode2/3 from Supervisor flag1000 and returns0.
Success releases ANM slot1 when Startup exists, then sets update-task flag2.
The callback still ignores the thread argument and retains the observed plain RET.

Registration creates update/draw tasks with priorities6/3, clears their initial
flag2, stores the receiver and publishes+C/+10 before loading resources. The
target calls447280, the existing81-byte PreloadAnm; the first model accidentally
called the synchronous LoadAnm and grew to261/192. Correct binding gives192/192.
Reordering the second task's two independent stores to match their actual roles
removes the remaining ordinary differences. Both slot25/26 failures use the
existing data-corruption diagnostic; success enables cursor wrapping and clears
4918A4. No error branch or field is excluded from comparison.

## Natural VM interfaces and shared helper

CreateVm is an ordinary receiver method. LTCG naturally selects the target
stack receiver/EDI script/RET4 interface, including allocation, layer15,
initialization, variant0 insertion and the indexed ID store. The old static
cdecl helper introduced caller stack adjustments absent from the target.

The interrupt method naturally selects EAX receiver, ECX index and SI interrupt.
An explicit value ID copy before FindVm removes the implicit reference-view
temporary's register differences and reproduces all65 bytes. It marks the live
VM and root children without executing scripts or clearing the ID. No register
annotation or fake argument is used for either method.

The already exact409E50 helper is also used by FrontEnd's ASCII auxiliary-ID
retirement. Its definition moves unchanged from EnemyEclDispatcher.cpp to
VmIdInterrupt.cpp. The old opaque FrontEndResetAsciiAuxiliary seam is removed.
Both cold images bind its actual EAX pointer interface and reproduce the full65
bytes/all2 fields. Its name and Enemy origin are retained; the source move does
not identify its original translation unit or add credit.

## Complete larger controller remains nonexact

The1128-byte update owner includes1059 code bytes, one alignment byte and its
17-slot68-byte table. The original split source emits372 bytes with two outlined
helpers. Restoring complete in-place demo and initialization logic first gives
1100/1128; real VM receivers/shared helper give1076/1128. The parent remains
nonexact and receives no partial code or table credit.

Fresh target review corrects the mode2 return path: it selects replay screen12
and calls4315C0, rather than practice screen11/431EE0. Cursor selection uses its
clamp operation. The+10 flags belong to the actual draw task, superseding the
old transition-owner navigation label. Supervisor mode/flags use their existing
typed fields. Demo stage search uses a local counter and publishes its result
afterward; the decompiler's apparent global loop is contradicted by raw bytes.

Remaining differences include block placement, constant/register lifetimes and
clamp branch merging. An inline void setter model is byte-neutral for the parent
and is restored; park it. The unsigned mode spelling is also byte-neutral for
the parent and remains consistent with the actual thread expression. Practice
is1524/1968 and Draw112/116 in this context. These sizes grant no exactness.

## Focused native proof

Both cold builds compile all27 declared inputs from the final source/include
hashes with the pinned per-source profiles and `/GL`; no objects are reused.
The moved helper preserves its original profile without an explicit /EH switch. Complete qualified PDB
extents, every address field and fixed local destination pass. Thirty-four
explicit affected old contracts also pass in each image:10143 bytes/all351
fields, including VM dependencies, callbacks, UI displays, Startup, lifecycle
and worker interfaces. These are selected same-image comparisons, not a full
historical replay. Source/artifact identities are in [results.json](results.json).

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit frontend-load-thread --unit frontend-register-tasks \
  --unit frontend-create-vm --unit frontend-mark-vm-interrupt \
  --unit enemy-mark-pending-interrupt --json
```

Private recipes, snapshots, rejected models, complete receipts/listings and
bounded exit logs are in `.analysis/gpt-6.1-sol/frontend-launch-20261029/`.
Independent first witness: `build/gpt-6.1-sol/frontend-launch-20261029/cold-a/`;
second: `build/match-linked/FrontEndLaunch/`. Lightweight CI and staged whitespace
checks pass. No comparator mechanism, host scenario, product build, runtime or
portability claim is introduced.
