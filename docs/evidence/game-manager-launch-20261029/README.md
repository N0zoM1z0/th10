# GameManager creation and Main worker — 2026-10-29

Three complete owners add **237 exact bytes:225 authored and12 indeterminate**.
Two independent canonical28-input colds pass every byte and all13 fields.
The small callback is newly inventoried and retains indeterminate origin; the
reviewed authored denominator is unchanged. Four new source mappings cover362
bytes, including a complete125-byte configuration candidate that remains nonexact.

| Complete owner | Bytes | Fields | Selector |
| --- | ---: | ---: | --- |
| GameManager factory `4180E0..41814A` | 107 | 7 | `game-manager-create` |
| Registered forwarder `417C70..417C7B` | 12 | 2 | `game-manager-load-thread` |
| Main worker launch `420EA0..420F15` | 118 | 4 | `main-start-mode-thread` |

## Real allocation and worker layout

The60-byte GameManager constructs its timer and34-byte configuration member
before clearing the whole object. Creation then calls device vtable slot14,
stores the stack mode at+5C, publishes477810, sets flag4 and launches the actual
initializer callback. The complete107-byte comparison retains the allocation
null branch and subsequent accesses. It grants no allocation-failure or runtime
claim. The device slot is a typed offset view; its original declaration and
source ownership are not inferred.

The Main worker starts at+62C and occupies1C bytes. Its running field is+63C,
superseding the old closeRequestBlocked navigation name. The existing113-byte
Supervisor callback now reads that actual field and remains exact. Offset
assertions preserve update gate648 and critical sections64C/counters6F4.

StartModeThread holds critical section6/counter6, calls the real Stop, stores
entry/running/stop state, expands beginthreadex with the actual context/thread-id
destination, publishes the handle and unlocks. Natural member code selects the
target EDI receiver and two stack arguments/RET8. The complete118-byte body and
all four import/callee fields pass. No forced register ABI or synthetic caller
is introduced.

The actual callback loads477810, passes it to the maintained1009-byte initializer
and forwards its result with plain RET. It was absent from the prior inventory
and database functions. Fifteen CC bytes follow the previous initializer; four
follow the callback before independent417C80. All twelve bytes were queried in
attested read-only Ghidra: only factory data reference41812C reaches the entry,
and no interior references appear. This small source/function/outline origin
remains unresolved. Its callee is source-backed but nonexact; address/interface
binding grants no initializer-byte credit.

## Complete configuration candidate and rejected controls

The shared34-byte configuration has version100003, nine16-bit bindings, two
600-valued fields, byte settings and options+30. Existing Main offsets agree
with color1A, windowed1D, frameskip1E, volumes20/21 and hint22. The default binding
data is declared externally; no original key table is copied into source.

A C++ constructor body produced127/125 and an extra returned-receiver move.
A void reset invoked by the constructor gives125/125, but nine ordinary bytes
still differ: prologue scheduling and EAX/ECX temporaries for600 and flag1.
Unsigned serialized fields recover the target's format/hint store ordering;
this does not establish original source types or grant partial credit.

A natural free-function interface is byte-neutral and restored. A bool-field
model is also rejected: actual configuration loading at420870 permits three
values at+1B, so bool would lose a valid serialized value. The byte view is
retained. Five actual reset call sites are reviewed; they do not establish a
consumed return contract. A zero return is not added to force register allocation.
The candidate remains nonexact, with all125 bytes and five binding-data fields
accounted for diagnostically.

## Larger owners and alias evidence

FrontEnd's491C00 mode and SupervisorModes' counter name refer to the same
physical slot. FrontEnd now uses the existing shared symbol. A complete native
control confirms this is byte-neutral for the1076/1128 update candidate; the
alias obligation closes without extra exact credit. Mode transition remains
678/666 and registration204/204 remains nonexact. Fresh larger GameManager
comparisons give initializer1039/1009 and cleanup1120/1105, also nonexact.
Do not repeat parked clamp/block/return controls without a new premise.

The remaining configuration callers provide concrete context leads: Main's
configuration load420870, Main initialization421F00 and replay-object creation
at428FEA. Their real code can distinguish source/context hypotheses; near bodies
or plausible register choices cannot waive ordinary byte differences.

## Focused native proof

Both cold links compile all28 declared inputs from the final source/include
hashes with pinned profiles and `/GL`; no input object is reused. Full qualified
PDB extents, every field and fixed local destination pass. Forty-four explicit
affected old contracts also pass in each image:10922 bytes/all402 fields,
including the actual Supervisor/GameManager callbacks and VM/worker dependencies.
This is selected same-image verification, not a historical-cohort replay.
Source/artifact identities are in [results.json](results.json).

The factory's device operand is bound to Main491C28+8. A nearest-public label
in the diagnostic anchor layout can name an unrelated stub at that interior
address; it is not the semantic binding. Configuration key references likewise
belong to the default binding base474E88 plus offsets, not neighboring anchors.
No data-storage or runnable-product claim is made from this harness.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit game-manager-create --unit game-manager-load-thread \
  --unit main-start-mode-thread --json
```

Private recipes, snapshots, rejected models, raw reviews, complete receipts and
bounded exit logs are under `.analysis/gpt-6.1-sol/frontend-controller-20261029/`.
First independent witness: `build/gpt-6.1-sol/frontend-controller-20261029/cold-a/`;
second: `build/match-linked/GameManagerLifecycle/`. Lightweight CI and staged
whitespace checks pass. No comparator, host-scenario, product, runtime or port
claim is added.
