# Line initialization and timed turning

The previously omitted Initialize41C8C0/442 and Motion004041D170/329 now have
maintained source in LaserInitialize.cpp. Both have independently reviewed
game-authored behavior, adding771 authored denominator/source bytes. The
neighboring unreferenced54-byte candidate41C880 is tracked separately with
unknown origin and no source mapping. Initialization remains nonexact;
Motion0040 closes329 authored exact bytes with two independent canonical
cold builds and all eight linked fields, recorded in
[native-proof.json](native-proof.json). Original production class/TU ownership,
construction, data storage and runtime/product closure remain unresolved.

## Target evidence

The input is original Japanese TH10 v1.00a, SHA-256
2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040.
Fresh local Ghidra is read-only/noanalysis with768 MiB and one active CPU.
The disk and complete412289 mapped text bytes are attested, metadata edits
roll back and all mapped bytes remain unchanged. Ghidra names, argument lists
and decompilation remain hypotheses; raw instructions establish the protocols.

The initializers' slot is46DA64 and motion's is46DA90. All442/329 bytes decode
densely through their returns with no table or external interior reference.
CC padding separates all neighbors. The initializer is ECX=this, one stack
request and RET4/EAX0; motion is ECX=this/RET. The existing C++ construction
contribution41C5B0/202 is a separate indeterminate owner, not reclassified by
the Initialize name or its matching receiver.

The54-byte41C880 candidate ends with RET8 and ten CC bytes before41C8C0.
It prefetches next pointers while summing virtual-slot20 results for nodes
outside state1. Ghidra reports no incoming references, the complete text has
no direct-CALL byte pattern to it, and the PE has no literal VA41C880.
That does not prove runtime unreachability or authorship. Its origin stays
indeterminate and it adds no authored credit. `report-laser-init.py` replays
these boundaries, slots, call identities, fields, float constants and padding
against the hash-attested target and current ledger.

## Recovered initialization

Initialize copies all77 dwords/1DC bytes into the existing request at424 and
sets state wordC to2. It starts the primary VM600 through the genuine
InitializeForLoadedScript404F30 with signed type/color and the existing
Bullet script-base array474170, sets pendingInterrupt304 to2 and explicitly
executes43EE30. The binder itself has a script-execution boundary; that does
not replace the parent's explicit second step. Only afterwards does the parent
read the live request flags, apply its conditional20-to10 flag change and the
primary maskFC63FFFF/00600000.

The secondary VM9AC uses a **fresh** manager47781C/resource458 lookup and the
current copied color+103, followed by the same interrupt/explicit execution.
Its20-to10 change is unconditional and its mask isFC7FFFFF/00400000. This is
the laser-manager resource, distinct from parser/graze primary resource4776F0.
No replacement data owner or vtable is installed by this source body.

The hold timer then becomes current30, previous29, subframe30, preserving an
already-selected scale or lazily selecting g_AnmGameSpeed. Geometry is read
from the live **owned copy** after the VM calls: position, initial length14,
width1C, speed20 and angleC. Sound ID40C becomes24. Positive length sets the
traveled4C marker to.01; other/unordered lengths set positive zero. Polar
conversion writes velocity X/Y only and preserves its previous Z.

Reference labels calling44 length_step or40C track_count conflict with these
TH10-local consumers. The maintained width, soundId and traveled view names
are retained; they do not claim original source identifiers.

## Recovered motion and compiler scope

Record3 atF4 supplies duration11C, next speed108, angular delta10C, repeat
limit120 and count124. During the duration, velocity magnitude is the current
base speed minus its fractional elapsed proportion; the base speed field is
unchanged. At expiry, the routine optionally queues the real sample sound,
increments the repeat count, clears flag40 at the limit, adds the angular
delta **without normalization**, installs the new speed and resets the timer.
It still performs final polar conversion and timer Tick after clearing the
flag. This identifies the parser's formerly named mode word as a float angular
delta for0040; meanings in the other motion slots remain provisional.

The first complete four-TU /GL /GS initializer diagnostic emits451/442; the
shared VM initializer emits200/202 with a private receiver mismatch. It remains
a diagnostic before the later motion-only refinement. The actual motion-entry
three-TU control emits326/329. An initial magnitude loaded before the branch
emits331 and is rejected. A normal by-value deceleration helper captures the
used speed value at the arithmetic boundary and preserves target x87 reuse;
the resulting complete329-byte PDB contribution has297 equal ordinary bytes
and eight independently bound fields. No dummy arguments, forced attributes,
volatile, placement or byte padding are added. The source helper's own
production boundary is not inferred from its inlining.

The canonical motion unit compiles LaserInitialize.cpp, EnemyLaser.cpp and
EnemyEclDispatcher.cpp completely. Its fields bind fixed sound storage,
QueueSoundSample, g_AnmGameSpeed, polar conversion, three timer constants and
__ftol2. The artifact is an unresolved-anchor diagnostic image, not a product.
Both independent colds freeze17 source/header inputs and replay all329 bytes.
The existing269-byte Motion0010 unit passes a focused canonical cold guard,
including all10 fields, because the shared virtual-slot declaration changed. No full historical cohort
replay is claimed. Existing30/123 helper structural diagnostics grant no credit.

## Host checks and limitations

The new suite executes the actual initializer, motion, command parser, timers,
aim/sound bodies and embedded-VM wrapper. Canonical headers use explicit host
layout/assertion/constructor-prefill adaptations; line, request, resources and
queue have live typed host storage. VM reset is **pure entry observation**.
Callbacks are allowed only at actual script binding/execution, and polar
observation does not alter Z or invoke callbacks.

Twenty-seven strict mutants check request copying, signed script selection,
interrupt/step order, live resources/flags/owned geometry, both masks, startup
timer, length/NaN marker, preserved Z and return value, fractional decay,
repeat boundary, post-clear tail work, unwrapped angle, speed/reset/tick and
sound ID0. ASan/UBSan/leak checks pass. The older parser fixture's reset-time
callback was overbroad; it is corrected to a pure observation and all23 strict
mutants pass again with mutation only at the real script boundary.
All90 public CI checks and final target/tracking/toolchain/progress/whitespace
checks pass. Superseded native controls and host binaries free about5.1 MB;
the original nonexact initializer carrier and current exact carriers are retained.

Tests require valid live objects, bounded counters/indices and defined finite
arithmetic. Original C++ object/union lifetimes, allocation failures, integer
overflow, native x87 control/exception behavior, assets and runtime reachability
remain outside this host proof. Exact bytes do not close those planes.

The MIT [reference repo](https://github.com/levzzz5154/th10-decomp) is pinned at
c6791d82dc9f02e55eca45a5314f900a85481efd in ignored _reference/th10-decomp.
line_laser_initialize.cpp supplied hypotheses; its code-segment pragmas,
noinline/assembly interfaces and special startup-timer type are not adopted.
The [MIT attribution](../../licenses/th10-decomp-MIT.txt) is retained. The
pre-existing D3D evidence is untouched.

```sh
scripts/repo-python scripts/report-laser-init.py
scripts/repo-python scripts/test-laser-init.py --sanitize
scripts/repo-python scripts/test-laser-script.py --sanitize
scripts/repo-python scripts/build-match-unit.py --unit laser-line-motion-0040
scripts/repo-python scripts/compare-linked-function.py --unit laser-line-motion-0040 --json
```
