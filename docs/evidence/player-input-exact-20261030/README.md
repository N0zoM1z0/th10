# Complete Player input timer exact — 2026-10-30

The current source replays input174 in the
[Player main owner batch](../player-update-owner-20261030/README.md). That batch
corrects the main target boundary from3089 code bytes to3112 owned bytes.

PlayerUpdateInputAction **004281D0..0042827D** is complete174-byte exact in two
independent canonical12-source /GL cold links. All7 address fields,55 decoded
instructions and6 fixed local branch destinations match. This adds **174 authored
exact bytes** with the existing reviewed boundary and origin. The
[results](results.json) retain the complete cold comparisons, pinned graph,
source/object hashes and compact rejected controls.

The complete owner takes Player in EAX and returns zero through RET on every
exit. The target has two following CC alignment bytes before428280. Active
state1 gates the timer; held input restarts a negative timer, a changed frame
emits shot rows, and frame15 wraps by Add(-15), stops at-1, or otherwise ticks.
The inactive state clears trackedEnemy and its valid byte. Source retains every
branch and real SetCurrent/row/Add/Tick interface.

The starting source emits153/174. An ordinary inline HasChanged member retains
the direct current/previous predicate and restores the target's constant-one
and frame reload schedule. Separately returning from wrap, stop and tick
restores delayed ESI preservation at+44 and the early short JE+27 to+79. That
edge shares the Add exit after its POP ESI, without popping an unsaved register.
The source adds no ABI hint, padding, manufactured return or binary patch.
Original method/class names, TU and inline ancestry remain unknown.

All7 external fields bind through maintained identities:

| Owner field offsets | Target identity/address |
| --- | --- |
| 23,68 | g_PlayerInputBits474E5C |
| 32,83 | AnmVmTimerView::SetCurrent405410 |
| 54 | PlayerEmitShotRows428160 |
| 74 | AnmVmTimerView::Add44BF40 |
| 8E | AnmVmTimerView::Tick404ED0 |

The target-bound comparator checks every ordinary byte after these explicit
field bindings; local branch bytes and destination offsets remain fixed. The
six branches are +11→98, +1F→36, +27→79, +4B→5D, +64→8D and +6C→7E.
An instruction diff that masks arbitrary local addresses cannot establish this
claim: the178-byte control has matching normalized instructions but an incorrect
near early JE to the inactive exit.

Acceptance retains the established12-source Enemy/Player graph, real
EnemyRuntimeUpdate entry, Player.cpp PDB owner and pinned profiles. First scoped
cold selects input174 plus existing producer717 and row105:996/all26 passes
across their two recipes. Second selects input174 alone:174/all7 passes with an
independent PDB GUID. No historical cohort is rebuilt. Eighteen strict same-image
bodies pass2265/all60. Nine previous native candidates preserve every ordinary
byte and field identity, including shots876, damage1172, EnemyRuntime2412 and
the2820-byte Player central contribution. Their unclosed target owners receive
no new exact credit; central target3089 and the shot-update moved LEA remain open.

Rejected controls distinguish the actual mismatch. Ordinary member conversion
retains153; HasChanged with the old shared exit emits175, with ESI saved early.
Force-inlining and changing its bool return to int leave175 unchanged. An early
inactive guard emits170 but places cleanup before the active body. One common
active return emits178 with the wrong early near branch; an explicit shared
exit label is neutral. Gating all active work emits182 with a repeated sign
check. Existing HasIntervalTick(1) also emits175; removing its noinline and a
callsite depth control are restored. The final source keeps the old interval
predicate and damage call boundary. Twenty-one private snapshots reconstruct
from checked line deltas;16,826,967 bytes of superseded source/build experiments
were removed. Selected warm and both cold outputs remain pinned.

Current warm context is
`.analysis/gpt-6.1-sol/player-input-context-20261030/final-context.json`,43 recursive
pins/12 object hashes. `canonical-context.json` beside it pins the second cold
for input174 and the shared producer717 recipe; `caller-context.json` pins the
current three-input105 recipe,28 pins/three objects. Revalidate source pins,
profiles, image and objects before reuse. The preceding player-spawn context
retains its earlier Player.cpp/Player.hpp source epoch. The separate13 ANM pins
remain current. No Ghidra observation or comparison mechanism changed.

```sh
scripts/repo-python scripts/replay-exact-units.py --unit player-update-input-action --unit player-spawn-shot --unit player-emit-shot-rows --json
scripts/repo-python scripts/replay-exact-units.py --unit player-update-input-action --json
scripts/repo-python scripts/ci.py
git diff --check
```

Native jobs are serial, CPU0/nice10, bounded and logged. Product, runtime and
portability closure remain separate. Authored exact progress is97,721/275,729
bytes,486 owners.
