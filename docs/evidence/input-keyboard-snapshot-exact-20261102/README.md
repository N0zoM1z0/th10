# TH10 keyboard state acquisition: complete 103-byte source exact

**Target:** original Japanese TH10 v1.00a `0x0044B010..0x0044B076`
(103 bytes). The maintained `InputKeyboardSnapshotView::ReadKeyboardState`
in `src/InputKeyboardSnapshot.cpp` is **103/103 PDB-owned byte-codegen exact**
under actual VC7.1 SP1 `/GL /GS /EHs`, with all six original fields bound
at their real locations and every instruction decoded. No raw target bytes
are copied, no inline assembly or fabricated register-movement statements
are used, and the candidate and original have identical full extents.

## Exact control flow and real device interfaces

The function clears the caller-supplied **256-byte** keyboard buffer using
`REP STOSD`, checks the actual input-enabled global at `0x004924FC`, and
branches on the DirectInput availability flag 0x200 at `0x00491FF4`.
If DirectInput is disabled, it calls the real `GetKeyboardState` Win32 IAT
function at `0x00466208` and returns zero. If enabled, it obtains the
real keyboard device at `0x00491C38` and calls virtual `GetDeviceState`
at vtable +0x24. For `DIERR_NOTACQUIRED` (`0x8007001E`), it separately
calls the virtual `Acquire` at +0x1C; for other nonzero statuses it follows
the other Acquire branch. Both cases return 1, as does a successful state
retrieval. These deliberately distinct branches reproduce the target rather
than simplifying two neighboring calls into one.

A **nonstatic `__fastcall` C++ member** provides a natural source model
for the original private ABI: the unused object receiver is passed via
ECX, and the buffer is passed via EDX. Its target instruction sequence
preserves EDX until pushing it as the buffer parameter. This does not prove
the original physical class name, compiler ancestry or translation unit.

## Strict binding and two independent selected colds

The single actual-source linked-PE graph binds exactly six external fields:

| Offset | Original identity | Target address |
| --- | --- | --- |
| +0x0D | input-enabled global | 0x004924FC |
| +0x17 | supervisor input flags | 0x00491FF4 |
| +0x23 | `__imp__GetKeyboardState@4` | 0x00466208 |
| +0x2B, +0x42, +0x57 | keyboard device pointer | 0x00491C38 |

The strict comparator certifies **103 decoded bytes**, a complete PDB section
contribution, full coverage and normalization of all six fields, and zero
remaining ordinary differences. Two independent selected cold links again
produce **103/103**, with different PE hashes and PDB GUIDs. Their identities
are pinned in [native-summary.json](native-summary.json). The first cold
also rebuilds the neighboring `FrontEndGetControllerState` using its own
pinned source graph: **264/264 exact**, all twelve original fields. Total
**367/367** selected bytes passed without any all-unit cold replay.

The upstream **1,052-byte** stage-controller owner at `0x00430FF0` remains
nonexact; no credit is transferred to it. This result does not establish
original source spelling, whole-game linking, installed hardware runtime,
assets or portability.

Reproduce selected actual source owners only:

```sh
scripts/repo-python scripts/verify-target.py
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit input-keyboard-state-snapshot --unit input-controller-buttons --json
nice -n 10 scripts/repo-python scripts/replay-exact-units.py \
  --unit input-keyboard-state-snapshot --json
scripts/repo-python scripts/ci.py
git diff --check
```
