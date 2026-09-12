# TH10 exact-reconstruction handoff

## Campaign state

The exact-reconstruction campaign remains active and incomplete. This conversation
reviewed one evidence-connected render/timing ABI and data-owner packet. It did
not establish source presence, canonical function exactness, whole Windows i386
build closure, runtime validation, semantic reconstruction, ports, or new Truth
Kernel acceptance.

Repository selection was exactly `th10`, analysis provider exactly
`th10-ghidra`, and target exactly `target:th10-main`. The ignored operator target
remained at `resources/th10.exe`; it was verified in place and was never modified,
replaced, relocated, staged, or committed. No `/mnt` search or
`TH10_TARGET_PATH` override was used.

Session starting HEAD was
`17166cc593f04acc3dd093bfbc3258e655daf596` on `main`. The tracked/untracked tree
was clean at entry: zero staged, unstaged, untracked, or conflicted paths. The
checkpoint containing this handoff should use subject
`gpt-web: review TH10 render timing ABI`; the live Git commit containing this
file is the authoritative ending HEAD. No Factory repository command can push,
and no push was requested or executed in this conversation.

At recovery entry, `origin/main` had already moved to the starting HEAD. Its
remote-tracking reflog records an external `update by push`; this conversation
did not perform that push and does not infer who did.

## Recovery gate and preserved state

The mandatory `factory_describe`, repository-list, and live TH10 status checks
passed. Recovery inspection covered branch/HEAD/upstream, recent commits,
porcelain-v2, complete staged and unstaged diffs, relevant untracked paths,
ignored analysis/tool/build state, remote-tracking reflog, and the previous
handoff before edits.

All required repository instructions, Factory contracts, and Factory guidance
paths named by the session prompt were mounted and read. No guidance path was
missing.

No tracked or untracked interrupted work required recovery. Ignored/private
state was classified and preserved as follows:

- `resources/th10.exe`: operator-supplied private target; preserved and excluded.
- `.tools/`: immutable-tool selectors plus game-bound Wine state; preserved and
  excluded.
- `ghidra-project/`: target-bound provider state; preserved and excluded.
- `.analysis/bootstrap/`: pre-existing/legacy analysis material; preserved
  without granting it evidence authority.
- `.analysis/gpt-web/20260912-main-frontier/`: prior checkpointed reproducible
  diagnostic scratch; preserved.
- `.analysis/gpt-web/20260912-d3d-owner-seam/`: prior checkpointed reproducible
  diagnostic scratch; preserved.
- `.analysis/gpt-web/20260912-render-timing-abi/`: current-session reproducible
  diagnostic scratch; see the artifact section.
- `build/`: no relevant recovered file required action.

No reset, stash, bulk cleanup, or deletion was used to manufacture a clean tree.

One read-only PE-boundary shell command failed because a repository helper was
called with a `Path` instead of bytes. Factory recorded a clean tree before and
after; live status was nevertheless re-read immediately, remained clean, and
the boundary read was rerun with a minimal read-only PE mapper. No recovery edit
was needed.

## Identity and mandatory preflights

The private target passed `python3 scripts/verify-target.py` with SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`,
MD5 `7dc488d82c81dd4aee4ba098b8804d83`, PE32 i386 image base `0x00400000`,
entry `0x004537DC`, and Rich dominant build 6030.

`python3 scripts/verify-toolchain.py --execute` passed the hash-pinned VC7.1 SP1
normal-COFF, C++ LTCG, resource, and PE32-link smoke under headless Wine.
Tracking, reconstruction status, and public CI preflights also passed.

Native Ghidra operation schemas were discovered before target-dependent work.
A real `check {}` and every useful semantic query in this packet returned
`attestation.status=passed`, target `target:th10-main`, and
`attestation.provider_transport=factory-native-command`. The provider remained
read-only and every semantic result had `exactness_credit=none`.

Truth Kernel accepted-snapshot refreshes remained unavailable because another
Factory operation owned the operator path. No current accepted-state result was
obtained, and absence of a snapshot is not treated as acceptance or rejection.

## Packet selection and observable goal

This packet continued the reviewed Main/D3D seam rather than rotating to an
unconnected small function. The bounded scope was:

- `0x00439390-0x00439538`, the frame/render dispatcher immediately upstream of
  the already reviewed `0x004391F0` present/reset path;
- `0x00439540-0x00439659`, its shared timing source;
- the target storage rooted at `0x004924F0`, passed from WinMain to
  `0x00439390` and also populated by window/timing initialization.

The intended observable was not an easy exact-function count. It was to resolve
whether the adjacent-game `GameWindow::Render`/`GetTimestamp` source shape fits
TH10's actual ABI and layout, and to determine whether the evidence is mature
enough to introduce production source. The result is a reviewed authored
boundary/ABI/layout advance plus a strong non-accepted source-shape near-match;
source/TU ownership remains insufficient for production source.

## Reviewed `0x00439390-0x00439538` — proposed `GameWindow::Render`

This 425-byte extent is now reviewed `authored_game / Main / high`. Direct target
bytes show every return path using `ret 4`; the final instruction occupies
through `0x00439538`, followed by seven bytes of `0xCC` padding at
`0x00439539-0x0043953F` before `0x00439540`.

WinMain is the unique target-local caller. It pushes the address `0x004924F0`
and calls `0x00439390`. On entry the callee loads its only explicit pointer from
the stack and uses it to access fields at offsets `+0x14`, `+0x38`, `+0x40`, and
`+0x48`. The machine ABI is therefore one explicit pointer argument with callee
cleanup; the ledger records `__stdcall` at that machine-ABI level.

Behavior is independently target-local: the function samples `0x00439540`,
updates frame timing/deadline fields, runs gameplay/draw-chain work, advances the
frame counter, and calls the reviewed present/reset function at `0x004391F0`.
`GameWindow::Render` remains only a proposed adjacent-supported name, not an
original-symbol claim.

A pinned VC7.1 normal-COFF ABI diagnostic distinguishes source-shape families:

- an ordinary C++ member receives `this` in ECX and returns with plain `ret`;
- an explicit `__stdcall` member receives `this` on the stack and uses `ret 4`;
- a static/free `__stdcall(Window *)` helper has the same stack-pointer/`ret 4`
  machine shape as the explicit stdcall member.

The target therefore excludes the ordinary normal-COFF member-thiscall shape.
It does not distinguish explicit stdcall-member versus static/free helper, and
the surrounding Main seam already contains EAX/EBX private-ABI evidence, so an
LTCG/interprocedural transformation remains a material alternative. Source-level
owner, original declaration, TU, and exactness remain unknown.

## Reviewed `0x00439540-0x00439659` — proposed `GameWindow::GetTimestamp`

This 282-byte extent is now reviewed `authored_game / Main / high`. The target
returns at `0x00439659`; six `0xCC` bytes at `0x0043965A-0x0043965F` separate it
from independent raw code beginning at `0x00439660`. Ghidra does not currently
model that following raw code as a function, so no denominator claim is made for
it here.

The target routine enters the game's critical section at `0x004922EC`, increments
the corresponding lock-count byte, and selects between two timing paths:

- when the stored performance frequency is nonzero, it calls
  `QueryPerformanceCounter`, subtracts the stored start counter, divides by the
  QWORD frequency, maintains the minimum time origin, and returns elapsed time;
- otherwise it brackets `timeGetTime` with one-millisecond timer-period calls,
  handles unsigned-to-double conversion, maintains the same time origin, and
  scales milliseconds using target constants `1000.0` and `0.001`.

The no-argument source-level calling convention cannot be distinguished from a
plain `ret`. The proposed `GameWindow::GetTimestamp` name is corroborated by
committed TH095/TH08 source shape, but is not an original-symbol claim.

### Focused VC7.1 source-shape feedback

A natural no-argument timestamp source probe was compiled with the pinned VC7.1
SP1 compiler using `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG`. Probe mode of
`scripts/compare-coff-function.py` compared it against target `0x00439540`:

- object function size: **282 bytes**, exactly the reviewed target extent size;
- comparable non-relocation bytes: **162**;
- matched comparable bytes: **159**;
- the only three non-relocation differences are offsets `0x2`, `0x9D`, and
  `0x118`, all the same stack-frame immediate: probe `0x10`, target `0x14`;
- comparator result: **`mismatch`**;
- `acceptance_authority`: **`none`**.

The normal-COFF probe otherwise reproduces the target control flow, x87 integer
and double operations, API ordering, branches, and epilogues after relocation
fields are excluded. An identical `/GL` compile also produced a valid VC7.1
LTCG intermediate object, but the repository intentionally lacks an accepted
linked-image LTCG extent Oracle.

This is evidence for a highly constrained source shape and an optimizer/profile
or source-local-layout difference. It is not a zero-difference check and is not
promoted to source presence or exactness. The session deliberately did not fish
compiler flags merely to force the remaining `0x10`/`0x14` frame difference.

## TH10 window/timing storage at `0x004924F0`

TH10 target evidence now establishes a coherent object-like storage region rooted
at `0x004924F0`. Observed fields are:

- `+0x00`: HWND written by the create-window path;
- `+0x04`: closing state read by WinMain and cleared by D3D initialization;
- `+0x08`: WinMain HINSTANCE;
- `+0x0C/+0x10`: active/inactive state updated by the window-proc path;
- `+0x14`: signed frame counter incremented/reset by the render dispatcher;
- `+0x18/+0x20`: QWORD performance frequency and performance-start counter;
- `+0x28`: shortcut/relative-launch state set when resolved startup path differs
  from the module path;
- `+0x2C/+0x30/+0x34`: saved `SystemParametersInfoA` values restored during
  shutdown;
- `+0x38/+0x40/+0x48`: render timing/deadline doubles;
- `+0x50`: time-origin double used by `0x00439540`.

These direct accesses establish used storage through offset `+0x57`. The exact
complete object extent and original type name are still unknown; no `sizeof`
claim is made. In particular, this session does not infer an exact boundary from
the unrelated-looking references beginning at `0x00492548` without ownership
proof.

The layout is similar in purpose to committed TH095 `GameWindow`, but it is not
the same layout: TH10 has an observed HINSTANCE at `+0x08`, shifting later timing
fields relative to TH095. The TH10 offsets above were established from TH10
before using adjacent source for corroboration.

## Adjacent-game hypothesis discipline

Adjacent live states observed during this packet were:

- TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97`, clean;
- TH09 HEAD `943ec407a9429379fa9fdbcde97dd246e51f10a8`, with an unrelated live
  modification in `src/TitleScreen.cpp`; no uncommitted TH09 content was used;
- TH095 HEAD `d8949073a3cb10586e3a5e0f9436594cede2e58b`, with unrelated tracked and
  untracked live work; only committed `HEAD:src/Main.cpp` and `HEAD:src/Main.hpp`
  were consulted.

Committed TH08/TH095 source supplied `GameWindow::Render` and
`GameWindow::GetTimestamp` naming/source-shape hypotheses. No adjacent address,
layout, ABI, ownership, exactness, or completion claim was transferred to TH10.
Where the layouts differ, TH10 target evidence is authoritative.

## Ledger and verification-plane state

After this packet:

- tracked candidates: **1,195**;
- origin/boundary pending: **1,187**;
- reviewed authored: **7 functions / 4,244 bytes**;
- reviewed exclusions: **1**;
- source-present authored mappings: **0**;
- canonical exact functions/bytes: **0 / 0**.

This packet adds two reviewed authored functions totaling 707 bytes. The authored
exact denominator remains moving and incomplete.

Verification planes remain independent:

- **Source presence: 0.** No production source file or source mapping was added.
  The original TU, complete `GameWindow` type extent, and normal-COFF versus LTCG
  ownership are still unresolved.
- **Function/owned-extent exactness: 0.** The timestamp probe is a diagnostic
  mismatch, not an accepted match unit.
- **Whole Windows i386 build closure: open.** `scripts/build.py --check` passes
  the explicitly open skeleton; the honest `scripts/build.py` run returns the
  expected RC=2 because compile flags, TU partition, libraries, resources, and
  link order remain unknown.
- **Runtime validation: not started.** No runtime scenario is claimed.
- **Truth Kernel acceptance: unavailable for refresh.** Operator-path locking
  prevented a current snapshot; no new accepted fact is claimed.

`config/implemented.csv`, `config/matches.csv`, `config/match-units.toml`, and
`config/build.toml` were not promoted or populated by this packet.

## Commands and validation actually run

Mandatory preflight and identity work included:

```text
python3 scripts/verify-target.py
python3 scripts/verify-toolchain.py --execute
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 scripts/ci.py
```

Factory-native Ghidra work used discovered schemas for `check`, `function`,
`callers`, `callees`, `decompile`, `disassemble`, and `xrefs_to`. Direct PE reads
were bounded to the reviewed function boundaries and relevant storage windows.

Focused compiler feedback used `scripts/compile-probe.sh` for normal COFF and
`/GL` ABI/timestamp diagnostics, plus `scripts/compare-coff-function.py` probe
mode for the 282-byte timestamp near-match. No canonical exact unit was created.

After ledger edits, these passed:

```text
python3 scripts/verify-toolchain.py --check
python3 scripts/validate-tracking.py --require-target
python3 scripts/progress.py --check
python3 scripts/build-match-unit.py --check
python3 scripts/build.py --check
python3 scripts/report-reconstruction-status.py
python3 scripts/ci.py
git diff --check
```

The honest whole-build diagnostic `python3 scripts/build.py` returned the
expected RC=2/open result, not product closure.

## Analysis artifacts

`.analysis/` measured **31,831 bytes** at session entry and **47,612 bytes** at
the final session inventory. No current-session artifact exceeds 64 MiB and the
campaign remains far below the 256 MiB soft budget.

Current scratch root:
`.analysis/gpt-web/20260912-render-timing-abi/`.
It contains a manifest plus:

- `render-abi-probe.cpp`;
- `render-abi-probe-coff.obj`;
- `timestamp-probe.cpp`;
- `timestamp-probe-coff.obj`;
- `timestamp-probe-report.json`.

These are small reproducible session diagnostics. The compact mismatch report is
retained because it precisely identifies the unresolved three-byte frame-profile
difference. Two current-session `/GL` intermediate objects
(`render-abi-probe-ltcg.obj` and `timestamp-probe-ltcg.obj`) were removed after
reference review because their producers had exited, they were reproducible, and
no tracked or active record required their bytes. Older bootstrap and prior
gpt-web scratch roots were left untouched.

The manifest is marked `checkpointed`; its commit binding is refreshed to the
final amended checkpoint after the tracked handoff is committed.

## Remaining unknowns and next evidence-connected packet

The main unresolved questions are:

1. whether the `GameWindow` Main seam is an LTCG contribution or another
   interprocedurally optimized owner, versus explicit source-level private/stdcall
   declarations;
2. why the natural 282-byte normal-COFF timestamp source shape uses a `0x10`
   frame while the target uses `0x14`;
3. the exact complete `GameWindow` storage extent and original type/source owner;
4. original TU partition and link-time owner for the reviewed Main functions;
5. the large manager object's layout/owner from the earlier D3D packet.

The preferred next packet remains on this same owner seam rather than selecting
an easy leaf:

- `0x00439730-0x00439884` (341 bytes), the create-window path that writes the
  HWND into `0x004924F0` and receives HINSTANCE through an observed EBX private
  convention from WinMain;
- `0x004392E0-0x00439343` (100 bytes), which saves system settings and initializes
  performance frequency/start in the same storage;
- `0x004390E0-0x004391E8` (265 bytes) as needed to close the callback ABI and
  active/inactive fields.

This route is selected because it directly tests the Main TU/private-ABI
hypothesis and closes object ownership around the current near-match. It is not
chosen for function-count convenience. If that packet establishes a coherent
source owner/profile, the next conversation can decide whether natural
`GameWindow` production source is finally justified; otherwise it should retain
source presence as zero and continue owner/profile investigation.

The campaign remains active. A conversation checkpoint or future 99.5% reviewed
ratio does not authorize GPT-web to declare the exact phase complete.
