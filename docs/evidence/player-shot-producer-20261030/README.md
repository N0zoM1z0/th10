# Player shot producer and complete row firing — 2026-10-30

The717-byte producer status below describes checkpoint2b95c23 and is superseded
by the [complete717-byte exact proof](../player-spawn-exact-20261030/README.md).
The174-byte input owner remains nonexact.

PlayerEmitShotRows **00428160..004281C8** is complete105-byte exact in two
independent canonical cold compilations, including both fields. This adds
**105 authored exact bytes** with the existing authored origin retained.
Player::SpawnShot717 and PlayerUpdateInputAction174 now have complete maintained
source, replacing absent owners/placeholders, but remain **nonexact**. The
[results](results.json) separate these claims and retain complete cold reports.

## Target and accepted owner

Fresh disk PE/Capstone decode uses the canonical original Japanese1.00a target.
The105-byte owner has3 preceding/7 following CC bytes and ends RET at+68. The
reviewed caller428223 in the input timer supplies EDI Player and EBX frame; the
ordinary source naturally reproduces that private interface and zero return.
It reads power as unsigned16 promoted to int, divides20, clamps the level to4,
adds5 for nonzero optionMode, and indexes the fixed-up8-byte entry at .sht+110.
A negative signed fireInterval terminates the34-byte descriptor list. Signed
remainder equals fireFrame to select the call. No zero-interval or null-list
branch is added to the target's actual behavior.

DIR32 at+03 binds g_PlayerPower474C48; REL32 at+57 binds the real maintained
Player::SpawnShot427E90. That member naturally uses three stack values and RET12
in the coupled graph, without an ABI annotation. Binding the call establishes
its identity/interface, not the producer's complete codegen equality. The
producer and input owner retain their existing authored boundaries/origins;
original source names, TUs and original inline ancestry remain unknown.

The accepted recipe is the existing three-input Player-constructor graph:
Player.cpp, AnmManager.cpp and Gui.cpp, entry Player constructor, pinned /O2 /GL
profiles. First cold selects only the new105 and existing planar55, allocator181
and interrupt65:406 bytes/all9 fields pass. Second independent cold selects only
105/all2; its PDB GUID differs. No production flag or whole-product claim follows.

## Complete producer and input source

The717-byte producer searches128 stride5C slots, honors type3 source occupancy,
sets state/descriptor and timer, and chooses drawPosition or an option's delayed
replayPair1 as coordinates. A compile-time offset check binds the latter's
sourceIndex-based address3244. Both XYZ temporary stores are retained in source.
It initializes angle/velocity, subtracts velocity from descriptor offsets,
creates primary and type3 secondary VMs from the actual loaded resource,
resolves both VM lists and clears a stale primary ID, publishes the angle flag,
then invokes the spawn callback and positional sound cue. Existing real vector,
wrap, AllocateVm/InitializeVm/AddVmVariant0 and QueueSoundCue APIs are used.
Allocation/InitializeVm themselves remain nonexact. No synthetic failure guard,
ABI helper, padding or copied target bytes is introduced.

The first complete candidate emitted765/717 and unrolled its tiny search loop
four ways. Early exhaustion prevents that duplication; inlining the complete
lookup restores the target's two-list path. The selected ordinary member emits
708/717 in the12-input Enemy/Player context, with162/218 broadly address-normalized
instructions; the three-input canonical context emits709/717. Neither is exact.
Search block/exit placement, descriptor/position registers, temporary homes and
alignment differ. A closer total length alone does not select a control.

The174-byte input owner restarts a negative timer only while input mask0x1 is held,
emits rows on current/previous change, wraps at15 through actual Add(-15), stops
at-1 when released, and clears trackedEnemy/valid outside the active state.
Its source emits153/174, still nonexact: constant-one lifetime, timer reads,
callee-save placement and epilogues differ. A typed timer view/reversed guard
emits138 and is restored. Fresh central target call4262C7 passes Player in EAX;
the newly bound native call now moves Player to EAX rather than pushing the old
opaque cdecl argument. The complete central native contribution stays2820
against the reviewed3089-byte owner. This is call/ABI recovery, not parent credit.

## Other controls and validation

The original876-byte shot-update frontier remains byte/field-identical to its
prior candidate, with the moved LEA and changed skip edge still open. Its actual
motion helper stays160/171. Complete vector operations recover its initial X
addition order in a diagnostic but not the polar-Z store or stack cleanup;
a constructor and a separated ordinary-COFF definition do not resolve them.
Those experiments are restored, including the source partition and flags.
Global stdcall versus ordinary member producer forms are byte-neutral; the
ordinary member is retained. Branch-local position lifetimes are rejected.

Eight prior native candidates preserve every ordinary normalized byte and field
identity, including shots876, damage1172, runtime2412, scalar652 and motion160.
This is previous-native preservation, not TH10 exactness. Sixteen strict
same-image target guards pass1374 bytes/all36 fields, including the new105 and
actual AddVmVariant0, sound cue, Tick/SetCurrent/Add interfaces. Tick's inline
body materializes in Enemy.cpp in this graph; this same-image qualification does
not change its accepted canonical StageScript materialization or source origin.

Three ordinary source attempts failed before linking: a missing field name,
missing shared sound declaration, and an SDK/manual critical-section declaration
collision from including Main.hpp. The final source uses the existing bounded
VM view and the same shared sound array declaration without that header. Logs
are retained; none is reported as a successful comparison or a toolchain fault.

Current warm context is
`.analysis/gpt-6.1-sol/player-motion-context-20261030/final-context.json`:
43 recursive pins/12 object hashes. The accepted three-input context is
`canonical-context.json` beside it with28 pins/three object hashes. Revalidate
all pins, actual profiles and image/object hashes before reuse. The old
player-shots context predates these source/header changes. The13 ANM pins remain
current. Seventeen source controls reconstruct from line deltas; superseded
builds/snapshots were removed after verification, keeping both canonical colds
and the final native image. Private binaries, target and experimental code stay
uncommitted. No Ghidra observation was used in this batch.

```sh
scripts/repo-python scripts/replay-exact-units.py --unit player-emit-shot-rows --unit player-planar-bounds --unit player-spawn-effect-row --unit player-set-shot-vm-interrupt2 --json
scripts/repo-python scripts/replay-exact-units.py --unit player-emit-shot-rows --json
scripts/repo-python scripts/ci.py
git diff --check
```

All compiler/link jobs are bounded, serial, CPU0/nice10 and logged. No historical
cohort, comparator change/self-test or host failure campaign is required.
Authored exact progress is96,830/275,729 bytes,484 owners. Boundary/origin,
product/runtime and portability closure remain independent.
