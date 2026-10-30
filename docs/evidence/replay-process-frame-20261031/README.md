# Complete Replay frame processing — 2026-10-31

The full **ReplayManager::ProcessFrame596/all33** and **ReplayAppendRecord85/raw**
are exact in two independent canonical cold links: **681 new authored bytes**.
The complete10-byte ECX-to-EDI callback/all1 also becomes exact; its indeterminate
origin is retained and grants no authored credit. No target boundary, origin,
denominator, compiler profile or comparator changes.

## Target-backed source repair

The596-byte owner4297D0..429A23 has142 instructions,33 address fields,14 direct
local branches, four direct calls and three plain RET exits. Independent complete
decoding binds every field and verifies every local destination as an instruction
start. The natural member receives its manager privately in EDI. Its10-byte
adapter42A3D0 saves EDI, moves ECX into EDI, calls the complete owner and restores
EDI before RET. Neither declaration forces a private register or return slot.

The old source used an unresolved input-update facade. Target call42984E binds
the existing FrontEndInputView::UpdateDebugRepeat40AC20, whose complete109-byte
body is raw-equal in the new image and cold-passes its unchanged canonical
recipe. Actual input storage is the existing474E30 view, not independent replay
scalar variables: hold counter+2A, current/previous replay words+2C/+2E and the
two replay masks+32/+34. The header exposes only the observed hold field and
asserts its offset; the complete view size and all existing offsets stay equal.
Original data ownership and field names remain unproven.

Recording publishes masked input before hold processing, saturates the counter
at8, updates repeat state through the real helper and records rounded/clamped
FPS every signed30 frames. The FPS sample is an ordinary unsigned-int temporary
before the final byte store, matching target MOV ECX,FF/MOVZX ECX,AL. The frame
payload is captured after sampling and again before the append call; no unrelated
value or code-generation-only padding is introduced.

Playback now saves the preceding input word before consuming an active stage;
this target store was absent in the old source. Indexed stage/cursor reads are
performed again after the actual global input writes, matching the target's
alias-sensitive accesses. FPS is published before the record cursor advances.
Both negative-stage and exhausted-record paths preserve their three word clears;
only the active-stage path advances recordIndex. All paths preserve frameCounter
behavior and the runtime-null early return.

The85-byte append helper42A8A0..42A8F4 writes all three ushort fields through the
current record cursor, advances by six bytes and returns the signed record-count
comparison against3600. Its ordinary int return and four meaningful stdcall
parameters derive frame EAX/input EDX/two stack words/RET8. The target's entire
85-byte PDB extent is raw-equal, including pointer subtraction, signed division,
SETGE and return. No target bytes, artificial register declarations or fabricated
return are used. This is one maintained source mapping; no constructor credit.

## Complete comparisons and controls

| Model | Complete ProcessFrame /596 | Append /85 |
| --- | ---: | ---: |
| Entry29-input source, unresolved dependencies |485, nonexact|source absent|
| Actual input helper, append and playback previous-word repair |472, nonexact|85, exact|
| Actual input storage, indexed rereads and FPS publication order |591, nonexact|85, exact|
| Wide FPS sample temporary |597, nonexact|85, exact|
| Frame payload snapshot before append arguments |596, exact|85, exact|

The final source matches all33 semantically bound fields and all14 fixed local
destinations. Full target-bound hashes, all four call identities, all142
instructions and all three returns agree. The strict comparator independently
accepts the entire596-byte contribution; no fragment or length-only credit is
used. The callback receives only its complete10-byte codegen credit.

A preceding bounded scan found71 direct477834 Player-pointer accesses in reviewed
owners, no immediate address-taking operand and only constructor/destructor
stores. This does not rule out indirect/address-arithmetic aliases and does not
justify volatile or fake clobbers. The582-byte BeginStage remains566/582; allocator
139/139 remains unlike in adjacent LEA/store order. Their parked controls are not
repeated. Initialize1297/1344, Replay destructor237/367, Replay Create181/185 and
playback-control adapter62/68 are separate nonexact bodies in this context.

## Scoped cold acceptance

The new recipes add the actual InputState.cpp to the prior29-input graph, with
its existing pinned GL/GS/EHs profile and the same real InitializeVm entry.
Only the three new recipes use this30-input producer:
`build/match-linked/ReplayFrame`. No old recipe migrates.

Two independent zero-reuse30-input cold links reproduce all691/all34 codegen
bytes. Their frozen images each pass27 existing complete contracts3405/all116,
including the two prior stage helpers and the actual109-byte input helper. The
complete Stage destructor remains an explicit460/499 rejection. The first scoped
canonical run separately cold-passes existing stage helpers176 and input109 using
their unchanged29/14-input recipes. No historical cohort is rebuilt.

[results.json](results.json) retains complete target/native inventories, current
source/header pins, independently bound contracts, distinct PE/PDB identities,
controls and explicit rejections. Serial CPU0/nice10 bounded native logs and
frozen artifacts remain under the private replay-frame-20261031 analysis/build
campaign. The Player reference scan remains under replay-alias-20261031.

```sh
scripts/repo-python scripts/replay-exact-units.py --unit replay-process-frame --unit replay-append-record --unit replay-process-frame-callback --unit replay-clear-stage-frame-data --unit replay-restore-stage-position --unit input-debug-repeat --json
scripts/repo-python scripts/ci.py
git diff --check
```

Authored exact becomes105509/275863 across513 owners; canonical173623/1391.
Mappings1218/authored700 and backlog187 retain source presence independently.
Original source/class/TU/data ownership, product closure, runtime semantics and
portability remain open. Exact bytes do not claim a runnable replay subsystem.
