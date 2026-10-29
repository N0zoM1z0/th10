# Complete Player shot producer exact — 2026-10-30

Player::SpawnShot **00427E90..0042815C** is now complete717-byte exact in two
independent canonical cold links. All17 address fields and25 fixed local branch
destinations match. This adds **717 authored exact bytes**; the existing reviewed
boundary and authored origin are retained. The [results](results.json) keep both
complete cold reports, the real graph/profiles and current source/object pins.

The target owner decodes to218 instructions, ends RET12 and has three following
CC bytes before PlayerEmitShotRows428160. Its ordinary Player member naturally
uses three stack values: Player, descriptor and frame. No register/ABI annotation,
padding, manufactured return or binary patch is added. Original class/name/TU
and original inline boundaries remain unknown.

The first complete source was765 bytes; the predecessor checkpoint2b95c23 had
708/717. Two source changes close the owner. Initialization now belongs inside
the first-inactive-row loop, returning after the selected row is initialized.
This restores the target's linear search and separate exhausted-search return,
including the pointer preparation before the occupancy gate. Direct shot
coordinate fields replace the captured position pointer for assignment, offsets
and sound. These recover ESI descriptor, EBP motion, the10/14 stack homes, XYZ
copy order and early EBP restore before the callback. Coordinate and ID reads
retain the target lifetimes; no operation is inserted to force allocation.

All17 external fields bind independently through maintained identities:

| Fields at owner offsets | Target identity/address |
| --- | --- |
| 6E | timer scale476F78 |
| BC, DB | float0.01 at470B00 |
| 121,15B | EnemyWrapAngle44BC70 |
| 13A | polar vector44C5D0 |
| 183,1BA,273 | ANM manager491C10 |
| 18F,248 | AllocateVm449950 |
| 1B4,26D | InitializeVm449870 |
| 1C5,27C | AddVmVariant04489D0 |
| 2B9 | shared sound owner492590 |
| 2BE | QueueSoundCue43DD10 |

The complete comparison preserves the128 stride5C slots, type3 occupancy gate,
timer initialization, body/option coordinate branches, both motion modes and
VMs, two-list lookup/stale-ID clear, spawn callback and sound read after callback.
AllocateVm and InitializeVm remain nonexact bodies; the exact caller binds their
actual interfaces without granting them credit. Callback return typing, original
data ownership and full runtime behavior are separate questions.

Acceptance uses the actual12-source Enemy/Player graph, with EnemyRuntimeUpdate
as the real maintained entry and Player.cpp as the qualified PDB owner. The
source order and pinned /O2 /GL profiles/overrides are unchanged. This graph
supplies the real polar, wrap, VM and sound definitions needed by private ABI
selection. A smaller graph's unbound callees do not establish this owner. First
cold explicitly selects producer717 and its direct caller105 in their two
existing recipes:822 bytes/all19 fields pass. Second cold selects only producer:
717/all17 passes with a different PDB GUID. No historical cohort is rebuilt.

Seventeen strict same-image comparisons pass2091 bytes/all53 fields. Ten previous
native contributions preserve every normalized ordinary byte and field identity,
including shots876, damage1172, runtime2412, input153 and the central Player2820
contribution. These are previous-native preservation proofs; the unclosed target
parents and the shot-update LEA difference remain open. No parent exact credit
is inferred from equal sizes or preservation.

The rejected assignment-result position pointer emits712 but loses target
registers. Direct VM-ID result assignment emits706 and grows the frame18 instead
of14. Both are restored. The selected direct-coordinate source emits717 and
passes the unchanged strict comparator, rather than merely having a closer
length. Seven source snapshots reconstruct from checked line deltas;6,613,668
bytes of superseded snapshots/builds were removed, retaining both canonical
colds and the selected warm image. Logs and compact controls remain private.

Current warm context is
`.analysis/gpt-6.1-sol/player-spawn-context-20261030/final-context.json`, with43
recursive pins/12 object hashes. The new canonical717 context is
`canonical-context.json` beside it; `caller-context.json` retains the current
three-input105 recipe with28 pins/three objects. Revalidate pins, actual profiles
and image/object hashes before reuse. Older player-motion/player-shots contexts
retain their source epochs. The13 ANM pins remain current. No Ghidra observation
or comparator mechanism changed in this batch.

```sh
scripts/repo-python scripts/replay-exact-units.py --unit player-spawn-shot --unit player-emit-shot-rows --json
scripts/repo-python scripts/replay-exact-units.py --unit player-spawn-shot --json
scripts/repo-python scripts/ci.py
git diff --check
```

Native jobs remain bounded, serial, CPU0/nice10 and logged. No host failure
campaign or product/runtime/portability claim accompanies this exact proof.
Authored exact progress is97,547/275,729 bytes,485 owners.
