# TH10 player homing-shot update and exact signed-angle helper

Japanese TH10 1.00a original, hash pinned in `config/target.toml`. Two
physically distinct executable bodies form an actual call edge:

- **0x00428B10..0x00428C19**, 266 bytes (player shot homing updater):
  full **source-present NONEXACT** reconstruction in `src/PlayerShotTracking.cpp`;
- **0x00428CE0..0x00428D2F**, 80 bytes (signed angular difference):
  **new canonical exact** `PlayerShotAngleDifference`, including 2 bytes
  of target alignment after the final RET8, a separately reviewed boundary.

## Full 266-byte shot behavior — target instruction audit

The first routine takes a shot pointer in private EDX, saves ESI and
returns zero through EAX with plain RET. The second owner/ECX argument of
the maintained descriptive callback is not established; its original C++
name, TU or registration are not claimed. It checks `PlayerShotRuntimeView`
state +0x40 for the already-hit value 2. Otherwise it follows the enemy
pointer +0x4C. The actual `EnemyFullObjectView` runtime flags word at
full enemy +0x2480 is tested for mask 0x11 and 0xC0000; on invalid flags
the shot clears +0x4C and enters the no-target path. Without a target,
`PlayerShotMotionView::polarMagnitude` (+0x2C shot-relative) increases by
0.1 and is clamped above at 16.0. The target's comparison leaves NaN
unclamped; the retained ordinary greater-than implements that path.

With a valid enemy, the target subtracts the shot's XY position
(+0x14,+0x18) from the enemy's world position (+0x1068,+0x106C), computes
the polar direction using x87 FPATAN, and calls the **separate exact**
`0x00428CE0` angle difference against shot angle +0x30. For timer current
at shot +0x4 >= 120, magnitude increments by 0.2 and angle is not updated.
Prior to frame 120: if absolute angular error is at least pi/4, magnitude
decreases by 0.3 and clamps below at 4.0; if error is smaller than pi/12,
magnitude increases by 0.1 and clamps above at 16.0; between thresholds,
magnitude is unchanged. Shot angle is advanced by 20% of signed angular
error, then normalized by real `EnemyWrapAngle` at 0x0044BC70. This is
native multi-branch x87 logic, not a dummy tracking approximation. The
physical helper/caller identities and some floating-point stack/lifetime
scheduling remain open: independent source-context compiler diagnostics
emit a **nonexact** 266-byte caller (linked sizes vary with entry/context).
No exact caller credit is claimed.

## Exact 80-byte helper — distinct owner, no false authorship

Direct raw PE reads prove every one of these 80 target bytes is **identical**
to the already independently exact 80-byte `BulletAngleDifference` function
at `0x00408660`. The target has a real E8 call at shot updater
`0x00428B7D` to `0x00428CE0`, 12 preceding CC bytes, and a complete RET8
at `0x00428D2D` followed by two zero alignment bytes before the next
function. This is a **new function boundary in the denominator**, not a
copy of code credited at the earlier address. Whether its presence arose
from hand-authored code, duplicated TU, or compiler/linker generation is
**origin indeterminate** and stays so in `config/function-origins.csv`.

The maintained C++ uses the same natural two-stack-float / `__stdcall`
expression and its observed target order, with `__declspec(noinline)` to
retain a callable boundary. Two independent zero-reuse VC7.1 `/GL`
`PlayerShotTrackEnemy`-entry linked-PE builds yielded different image
SHA-256 and PDB GUIDs. Each PDB isolates the **entire 80-byte** primary-TU
contribution. Strict comparison reproduces all 80 bytes with **64/64**
ordinary bytes equal and **4/4 independently target-bound** constant
addresses: +0x0A/+0x32 are pi at 0x00470B18; +0x1D/+0x45 are 2*pi at
0x00470B14. No unbound or cropped fields, fake register stubs or copied
machine code. Receipts:
`.analysis/gpt-web/20261103/player-shot-angle-cold{1,2}.json`.

The canonical angle unit is `player-shot-angle-difference-00428ce0`; this
80-byte owner is byte-exact but not proven authored. The adjacent target
acquisition owner described below adds an independent 53-byte exact match.
Original source/TU naming, game runtime and whole-product equivalence are
not established. Existing Player damage/shot and canonical angle-math units
are protected by local cold replays, not a full history or product replay.

## Adjacent 53-byte Player shot target acquisition — second exact owner

The separately reviewed original `0x00428AD0..0x00428B04` is **53 bytes**
with EAX zero and `RET 4`. It was formerly absent from maintained source,
but is now `int __fastcall PlayerShotAcquireTarget(Player *, PlayerShotRuntimeView *, int)`
in the *same* real PlayerShotTracking translation unit. The third argument
is a stack word whose native value is never read; its source spelling,
original callback registration and authored-versus-generated origin remain
**indeterminate**, as recorded before this checkpoint. There is no false
promotion of authorship.

The target writes NULL to the shot's tracked enemy at shot offset +0x4C,
then reads the player target pointer at +0x3504. When nonnull, it stores
that enemy and checks the enemy's actual world X at +0x1068. Native x87
`FLD`/`FABS`/`FCOMP` against **224.0f** clears the selected target only
when the absolute X exceeds the bound. The equality and NaN acceptance
paths remain intact; no extra safety or liveness guards were invented.
Compiler layout asserts verify both real offsets without overlaying a
second shadow Player/Enemy type.

The ordinary natural C++ source compiled under pinned VC7.1 SP1 `/GL`
produces **exactly 53 PDB-owned bytes** with **49/49** ordinary bytes equal
and one independently target-bound DIR32 at `+0x1E` for floating constant
`0x00470B4C`. Two completely separate `PlayerShotAcquireTarget`-entry
cold linked PEs with different image SHA-256 and PDB GUID both pass the
unchanged full canonical comparison, with no skipped, patched or cropped
bytes. Unit: `player-shot-acquire-target-00428ad0`; private receipts:
`.analysis/gpt-web/20261103/player-shot-acquire-cold{1,2}.json`.

This is **53 new exact codegen bytes**, but the function's original
hand-authored/compiler-produced status and full runtime correctness are not
established. It also does not turn the **266-byte** `PlayerShotTrackEnemy`
owner exact; the latter remains complete semantic source with nonmatching
native codegen. Both 53-byte and 80-byte exact units are tested through
focused cold replay, never full cold game reconstruction.

## Corrected adjacent 125-byte option-origin callback

The independent `0x00428C20..0x00428C9C` **125-byte** origin initializer
is now source-mapped under `PlayerShotSetOptionOrigin`, but remains
**NONEXACT**. Crucially, fresh original PE disassembly invalidated our
first `options[sourceIndex].replayPair0` hypothesis: it addressed the
wrong player storage. The corrected source indexes
`options[sourceIndex - 1].replayPair1` to access the *actual* fixed-point
values at original player +0x3244/+0x3248. This is a semantic correction,
not merely a compiler-register experiment. The complete target extent,
VC7.1 source candidate and rejected reload controls are documented in
[the dedicated option-origin evidence](../player-shot-option-origin-20261103/README.md).
The two neighboring 53-byte and 80-byte owners remain independently exact;
no credit is claimed for the 125-byte function.
