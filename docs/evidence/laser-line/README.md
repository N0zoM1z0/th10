# Type-0 laser update, draw and Player contact

Follow-up2026-10-20: Update41D3D0 now passes two complete canonical four-TU
colds with all1001 bytes/31 fields after ordinary used sprite snapshots restore
the final read/store scheduling. The earlier nonexact diagnostics below remain
historical. See [the current proof and scope](../laser-type1/README.md).

The maintained source now covers **1645 authored bytes**: Update41D3D0/1001,
Draw41D7C0/173 and Player contact4267F0/471. Motion0010 at41D2C0 adds269
source-present bytes with its existing indeterminate origin preserved. Its
complete269-byte canonical linked comparison passes in two independent cold
builds, including all10 independently bound fields. This adds **zero authored
exact bytes**. The newly discovered command-parser owner41CA80 adds1356
authored denominator bytes and remains source-absent.

## Target and physical ownership

The input is the original Japanese TH10 v1.00a executable, SHA-256
2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.
Local read-only/noanalysis Ghidra uses768 MiB and one active processor. Both
disk identity and the complete412289-byte mapped-text hash are attested;
temporary function creation is rolled back, with all mapped bytes unchanged.
Raw target instructions independently check the provisional decompilation.

The existing dense1001/173/269/471 boundaries end before CC padding. The
19-slot table46DA60 establishes slot0 commands,8 Update,C Draw,18 collision,
and24..48 motion operations. This is a known-slot partial interface, not a
replacement external vtable or proof of C++ construction/lifetime ownership.
Update calls slot0 first, then tests the current flags before every motion
callback. In particular100 precedes80; bounce uses mask08000C00. Earlier
callbacks can change which later slots run.

The previously omitted41CA80 entry has an incoming table reference at46DA60.
Its complete code span through41CF74 is1269 bytes, including ten internal
alignment bytes at41CA96 omitted by Ghidra's1259-byte provisional body.
Three alignment bytes at41CF75 precede five pointer entries at41CF78 and64
selectors at41CF8C. DEC/CMP3F/JA bounds the indexed selector read; selectors
for original opcodes1/10/20/40 select the four game cases, with every other
selector using the fifth/default destination. All destinations are local
instruction starts. Four CC bytes at41CFCC separate the next owner41CFD0;
six CC bytes at41CA7A separate the preceding initialization owner. No external
interior Ghidra reference was observed. The target has no PE base relocations;
guards, indices, destinations and neighbors establish table ownership.

This yields **1356 owned bytes**, with no overlap or exact credit. Game-specific
opcode dispatch, motion-flag/timer writes, Player aiming, primary resource and
sound calls independently support authored classification. The source header's
virtual declaration does not count as a body or maintained source mapping.
Other untracked virtual entries remain a separate review queue.

`scripts/report-laser-line.py` replays identity, complete decodes, branches,
virtual order, direct call boundaries, parser tables/guards, selected field
operands, float constants, polar outputs, stack cleanup, padding and ledger
extents. It supplies target observations only.

## State and recovered behavior

The partial line layout preserves position24, velocity30, angle3C, length40,
width44, growSpeed48, traveled4C, eighteen34-byte records58, flags404,
holdTimer410, type-0 request424 and real ANM VMs600/9AC, totalD58. A shared
EnemyPrimaryResources.hpp keeps the existing resource view/global identity
4776F0+3E0B50 in both callers. EnemyLaser.cpp now consumes the existing complete
sound-queue header instead of an inconsistent same-name local type. Native
assertions check the layouts; complete storage and production ownership stay open.

Update clears expired8000 flash state or decrements its pattern5 timer, grows
to the request maximum, then advances the emitted laser in XYZ. A positive
terminal distance trims both length and maximum and exits on nonpositive
remaining length. Hold expiration tests both endpoints against the actual
[-192,192] by[0,448] playfield; the conjunction matters. Live hold timers use
the real speed-scaled timer Add helper.

Collision starts only above length16 and width3, uses the10% inset/80% length
and the target's two width formulas. Hit1 calls virtual box collision with a
fresh Player position and size32/32/0. Graze2 uses signed age modulo5, creates
script434 from the primary Enemy resource, then samples current line X for
sound28. VM creation is a real call boundary; the sound queue body has no
invented callback. Primary execution precedes the freshly tested traveled==0
tail execution, with unordered comparison excluded from equality.

Two field corrections made during review are retained: primary scaleX divides
the canonical sprite **width at34**, scaleY divides **height at30**; Draw writes
the VM's **position at334**, not positionOffset340. Draw adds224/16, normalizes
angle+pi/2, sets flag4 and resamples position, traveled and renderer after the
first actual renderer call. Update sets scale flag8 for both components.

Player contact4267F0 is a laser hit/graze detector, correcting the old
option-position description. Its two laser callers and Player3C0/41C/4310
fields establish that role. It rotates relative coordinates, checks the
expanded hit interval, gates hits on GUI message, states2/4/3 and positive
highlight timer, then invokes426CF0. Graze compares the **lower Y** against
both1.5-width limits. Negated rejection comparisons preserve the target's
unordered behavior; Ghidra's positive interval rewrite would change NaN cases.
The hit boundary takes one stack Player pointer and returns with RET4; its
627-byte body stays source-absent. The existing PlayerCollision.cpp declaration
PlayerHandleCollisionHit is reused with the same stack/RET4 signature. No
declaration-only mapping is added.

The native polar helper41F800 writes only X/Y. Both caller scratch Z values
remain unknown before the target's XYZ additions; contact also computes Z
differences although its interval tests use only X/Y. Native source preserves
that unresolved scratch path. The host suite explicitly prefills only those
two local Z values, using17, and its polar math role writes only X/Y. Host
success therefore does not prove native Z initialization, FP exceptions or
general numerical equivalence. Pattern1 is an acceleration view of opaque
storage; the host explicitly starts that view's lifetime. Production object
lifetime and original source types remain unresolved.

## Compiler and host validation

One complete four-TU cold link compiles LaserLine.cpp, EnemyLaser.cpp,
AnmManager.cpp and EnemyEclDispatcher.cpp with pinned VC7.1 SP1 /GL /GS,
freezing21 project inputs. Corrected Update is1001/1001 but remains nonexact:
847/877 comparable bytes, with differences starting at380h in the scale tail.
Draw168/173 and Player contact453/471 also remain nonexact. Existing bounds91,
timer Add88 and sound cue149 structurally agree in that carrier; these are
diagnostics without new canonical helper credit.

The real virtual-entry controls keep ordinary source declarations. A two-TU
Draw entry before the shared hit-name unification emits172/173 and stays
nonexact. Motion0010 as the single-TU entry
restores the observed ECX receiver and emits269/269. Two subsequent complete
canonical cold builds freeze the same12 project inputs and each replay all269
bytes plus the10 declared fields: four game-speed references, five float
references and the genuine __ftol2 call. No dummy arguments, forced attributes,
volatile, packing, code placement or ABI coercion are introduced. Matching
this bounded diagnostic image does not make its unresolved anchors runnable
or settle Motion's authorship. Player's extended x87 rotation intermediates
and the parent scale-tail register/lifetime context remain open.

The affected Player header client separately compiles with native assertions.
No full historical cohort or whole product replay is claimed. Header/source
changes invalidate older FrontEnd/SpellPanel cache entries that depend on
Player.hpp or EnemyEclDispatcher.cpp; check individual dependency closures
before reusing any object. The independent MIDI cache is separate.

`scripts/test-laser-line.py` extracts the maintained four bodies plus the
actual timer Add, bounds, sound-cue and angle-normalization helpers. Canonical
recursive headers use explicit host layout/assertion/constructor adaptations.
Typed live outer objects, unknown virtual slots, polar math, VM creation/
execution, renderer calls and PlayerHandleCollisionHit are declared roles.
Checks cover live callback flags/order, speed/cap/terminal/bounds, hit/graze gates and
sampling, actual queued pan values, sprite axes/flags, fresh VM/draw state,
motion duration/tick/acceleration and interval/NaN boundaries. ASan/UBSan/leak
checks pass with21 strict mutants, each failing its intended functional CHECK;
sanitizer crashes do not count as rejection. No invalid-pointer, x87 bitwise,
runtime reachability, construction, product or portability claim is made.
All88 public CI checks pass. Final target/tracking/toolchain/progress/whitespace
checks also pass. Superseded native controls and host binaries are pruned;
current exact and nonexact carriers retain dependency-hash manifests.

The MIT [reference repo](https://github.com/levzzz5154/th10-decomp), linked from
[issue1](https://github.com/N0zoM1z0/th10/issues/1), is pinned at
c6791d82dc9f02e55eca45a5314f900a85481efd in ignored _reference/th10-decomp.
Its laser_line.cpp, laser_line_draw.cpp, laser_line_decelerate.cpp and Player
trigger_region.cpp supplied hypotheses. The [MIT attribution](../../licenses/th10-decomp-MIT.txt)
is retained. TH09 BulletLaserSpawn.cpp was inspected as adjacent-game
hypothesis material; its architecture does not establish TH10 facts. Upstream
placement/compiler controls and completion scores are not adopted. Pre-existing
D3D evidence remains untouched.

Reproduce with:

```sh
scripts/repo-python scripts/report-laser-line.py
scripts/repo-python scripts/test-laser-line.py --sanitize
nice -n 10 scripts/repo-python scripts/build-match-unit.py --unit laser-line-motion-0010
scripts/repo-python scripts/compare-linked-function.py --unit laser-line-motion-0010 --json
```
