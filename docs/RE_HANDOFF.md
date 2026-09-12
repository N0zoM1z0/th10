# TH10 exact-reconstruction handoff

## Campaign state

The exact-reconstruction campaign remains active and incomplete. This conversation
reviewed one Main/window owner-and-ABI packet, expanded the target function
denominator by three Ghidra-missed source bodies, and closed the physical extent
of the window/timing storage. It did not establish source presence, canonical
function exactness, whole Windows i386 build closure, runtime validation,
semantic reconstruction, ports, or new Truth Kernel acceptance.

Repository selection was exactly `th10`, analysis provider exactly
`th10-ghidra`, and target exactly `target:th10-main`. The ignored operator target
remained at `resources/th10.exe`; it was verified in place and was never modified,
replaced, relocated, staged, or committed. No `/mnt` search or
`TH10_TARGET_PATH` override was used.

Session starting HEAD was
`5c4d770c6fff624150126d0ee716d712a2c5f705` on `main`. The tracked/untracked tree
was clean at entry: zero staged, unstaged, untracked, or conflicted paths. The
checkpoint containing this handoff should use subject
`gpt-web: review TH10 window owner seam`; the live Git commit containing this
file is the authoritative ending HEAD. No Factory repository command can push,
and no push was requested or executed in this conversation. Immediately after
the logical checkpoint the tracked/untracked tree was clean: zero staged,
unstaged, untracked, or conflicted paths, with the local checkpoint one commit
ahead of the externally updated `origin/main`.

At session entry the local checkpoint was one commit ahead of `origin/main`.
During read-only packet work, `origin/main` moved externally to the same
`5c4d770...` commit. Work was paused and the recovery gate was repeated around
that event: HEAD, staged/unstaged diffs, untracked files, and the worktree were
unchanged. The remote-tracking reflog records
`2026-09-12T18:31:16+08:00 update by push`. This conversation did not perform
that push and does not infer who did.

## Recovery gate and preserved state

The mandatory `factory_describe`, repository-list, and live TH10 status checks
passed before editing. Recovery inspection covered branch/HEAD/upstream, recent
commits, porcelain-v2, complete staged and unstaged diffs, relevant untracked
paths, ignored analysis/tool/build state, `origin/main` reflog, and the previous
handoff.

All repository instructions, Factory contracts, Factory guidance paths, and the
repository-local TH10 reconstruction skill named by the session prompt were
mounted and read before edits. No required guidance path was missing.

No tracked or untracked interrupted work required recovery. Ignored/private
state was classified and preserved as follows:

- `resources/th10.exe`: operator-supplied private target; preserved and excluded.
- `.tools/`: immutable-tool selectors plus game-bound Wine state; preserved and
  excluded.
- `ghidra-project/`: target-bound provider state; preserved and excluded.
- `.analysis/bootstrap/`: pre-existing/legacy analysis material; preserved
  without granting it evidence authority.
- earlier `.analysis/gpt-web/...` roots: prior checkpointed reproducible
  diagnostics; preserved.
- `.analysis/gpt-web/20260912-main-window-owner/`: current-session bounded
  diagnostic campaign; see the artifact section.
- `build/`: generated/open build state; no recovery edit required.

No reset, stash, bulk cleanup, target edit, or deletion of unknown/legacy state
was used to manufacture a clean tree.

## Identity and mandatory preflights

The private target passed `python3 scripts/verify-target.py` with SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`,
MD5 `7dc488d82c81dd4aee4ba098b8804d83`, PE32 i386 image base `0x00400000`,
entry `0x004537DC`, four sections, and Rich dominant build 6030.

`python3 scripts/verify-toolchain.py --execute` passed the hash-pinned VC7.1 SP1
normal-COFF, C++ LTCG, resource, and PE32-link smoke under headless Wine.
Tracking, reconstruction status, and public CI preflights also passed.

Native Ghidra operation schemas were discovered before target-dependent work.
The mandatory `check {}` and the useful semantic queries in this packet returned
`attestation.status=passed`, target `target:th10-main`, and
`attestation.provider_transport=factory-native-command`. The provider remained
read-only and semantic results carried `exactness_credit=none`.

Truth Kernel accepted-snapshot reads were unavailable because another Factory
operation owned the shared operator path. This absence is not acceptance or
rejection; no new accepted fact is claimed.

## Packet selection and observable goal

The bounded packet continued the Main/render owner seam from the previous
checkpoint rather than rotating to an unrelated small function:

- `0x004390E0-0x004391E8`, the registered Win32 callback immediately preceding
  the reviewed present path;
- `0x004392E0-0x00439343`, the system-setting and performance-counter initializer;
- `0x00439730-0x00439884`, the window-creation path immediately preceding the
  reviewed D3D device initializer;
- the object-like storage rooted at `0x004924F0` used by these paths;
- raw target code between those candidates, specifically to challenge the
  denominator and physical owner assumptions.

The observable goal was to decide whether the Main seam can be represented as
ordinary standalone normal-COFF C++ source, recover the window/timing storage
extent from TH10 rather than adjacent layouts, and expose any target functions
missed by the Ghidra navigation inventory. The result is a private-ABI/owner
advance and denominator expansion. Physical normal-COFF versus LTCG ownership
is still unresolved, so production source remains premature.

## Reviewed `0x004390E0-0x004391E8` — proposed `GameWindow::WindowProc`

This 265-byte candidate is now reviewed `authored_game / Main / high`.
`0x00439730` stores its address directly in `WNDCLASSA.lpfnWndProc`. Direct target
disassembly shows the function consuming exactly the four standard Win32 callback
stack arguments (`HWND`, `UINT`, `WPARAM`, and `LPARAM`) and every exit using
`ret 0x10`.

Ghidra's provisional `__thiscall` decompilation with an extra `this` parameter is
therefore contradicted by the target machine ABI. The reviewed behavior includes
activation state, cursor visibility, close/erase handling, foreground-window
handling, and the target custom message path. The function ends at
`0x004391E8`; `0x004391E9-0x004391EF` is `0xCC` padding before the independent
reviewed present function at `0x004391F0`.

The ledger records the machine-level `__stdcall` callback ABI.
`GameWindow::WindowProc` is only a proposed adjacent-supported name, not an
original-symbol or source-file claim.

## Reviewed `0x004392E0-0x00439343` — system/timing initializer

This 100-byte candidate is now reviewed `authored_game / Main / high`. WinMain is
its only direct target caller. It saves three `SystemParametersInfoA` values into
the `0x004924F0` storage, disables those system behaviors, then initializes the
stored performance frequency and performance-start counter with
`QueryPerformanceFrequency` and `QueryPerformanceCounter`.

The function returns at `0x00439343`; `0x00439344-0x0043934F` is `0xCC` padding.
A natural pinned VC7.1 SP1 `/O2` normal-COFF diagnostic is also exactly 100 bytes
and matches all 68 comparable non-relocation bytes. Probe mode reports
`structural-exact` with `acceptance_authority=none`.

That diagnostic is strong source-shape evidence but not a canonical exact match.
The surrounding Main seam contains private-register ABIs and the target's
per-function normal-COFF versus LTCG owner is not established, so no source
mapping, match unit, or exact row was added.

## Newly tracked `0x00439350-0x0043938D` — system restore body

The denominator review found a 62-byte CC-delimited source body not present in
the original 1,195-candidate Ghidra ledger. Ghidra has no function or direct xref
at `0x00439350`, and a bounded raw PE scan found no direct `call rel32` or literal
pointer reference to the entry.

The body restores the three saved `SystemParametersInfoA` states and re-enables
the relevant IME state. It returns at `0x0043938D`; two `0xCC` bytes separate it
from `0x00439390`. WinMain contains the same restore logic inline.

A natural VC7.1 `/O2` normal-COFF restore helper is also exactly 62 bytes and
matches all 42 comparable non-relocation bytes. The probe is
`structural-exact / acceptance_authority=none`. The target body is therefore
tracked as high-confidence authored Main code while its original identifier,
source mapping, and physical COFF/LTCG owner remain unknown.

## Newly tracked `0x00439660-0x004396F7` — unlocked timestamp body

A second Ghidra-missed CC-delimited body was found immediately after the reviewed
locked timestamp function. Ghidra has no function/xref for `0x00439660`, and the
bounded raw PE scan found no direct call or literal entry reference.

The 152-byte body computes elapsed time from the same performance-frequency,
performance-start, and time-origin storage, falling back to `timeGetTime` when
needed. It returns at `0x004396F7`; `0x004396F8-0x004396FF` is `0xCC` padding.

A target-derived natural VC7.1 `/O2` normal-COFF helper is also exactly 152 bytes
and matches all 96 comparable non-relocation bytes. Probe authority remains
`none`; the body is added to the authored denominator but not to source presence
or exactness.

## Newly tracked `0x00439700-0x0043972C` — proposed `GameWindow::InitD3DInterface`

A third Ghidra-missed body begins after the timestamp padding. It is 45 bytes,
ends with `ret` at `0x0043972C`, and is separated from `0x00439730` by three
`0xCC` bytes. No Ghidra or bounded raw direct/literal xref was found for the
entry.

The target body calls `Direct3DCreate9(0x20)`, stores the interface pointer, and
uses the target Direct3D creation error path when creation fails. WinMain contains
the same interface-creation/error logic inline. The error logger at `0x0044B8E0`
itself consumes its object through live-in EDI, and this raw body loads EDI with
that logger object immediately before the call. That is another target-local
private-register ABI observation.

The role is independently established by TH10. `GameWindow::InitD3DInterface`
is retained only as an adjacent-supported proposed name. Original identifier,
source file, TU, and exactness remain unknown.

## Reviewed `0x00439730-0x00439884` — proposed `GameWindow::CreateGameWindow`

This 341-byte candidate is now reviewed `authored_game / Main / high`. WinMain is
its unique direct target caller. Immediately before the call, WinMain copies its
HINSTANCE into EBX. The callee then consumes live-in EBX for both
`WNDCLASSA.hInstance` and the HINSTANCE argument to `CreateWindowExA`, without
receiving a stack parameter.

The body registers the reviewed `0x004390E0` callback, creates the window, and
publishes the HWND into the reviewed window/timing storage. It returns at
`0x00439884`; `0x00439885-0x0043988F` is `0xCC` padding before the already
reviewed D3D initializer.

A pinned VC7.1 normal-COFF noinline helper with an explicit HINSTANCE argument
reads that argument from `[esp+4]`, not EBX. Therefore this target machine ABI is
not an ordinary standalone normal-COFF explicit-parameter call. The private EBX
convention is strong evidence for interprocedural/LTCG context, but it does not
identify a particular Rich-record input or TU. `GameWindow::CreateGameWindow` is
only a proposed adjacent-supported name.

## Physical window/timing storage extent

TH10 target evidence now closes the object-like storage rooted at `0x004924F0`
to exactly `0x58` bytes, covering `0x004924F0-0x00492547`.

Observed TH10 fields and widths are:

- `+0x00`: HWND, 4 bytes;
- `+0x04`: closing state, 4 bytes;
- `+0x08`: WinMain HINSTANCE, 4 bytes;
- `+0x0C/+0x10`: active/inactive state, 4 bytes each;
- `+0x14`: signed frame counter byte, followed by three bytes of alignment;
- `+0x18`: performance frequency `LARGE_INTEGER`;
- `+0x20`: performance-start `LARGE_INTEGER`;
- `+0x28`: shortcut/relative-launch flag byte, followed by three alignment bytes;
- `+0x2C/+0x30/+0x34`: saved system-setting DWORDs;
- `+0x38/+0x40/+0x48`: render timing doubles;
- `+0x50`: time-origin double.

The previous checkpoint only established used storage through `+0x57`. This
conversation established the upper physical boundary independently: static
startup code at `0x00465960` begins a different object exactly at `0x00492548`,
clears 17 dwords from that address, initializes its first-word flags, and
registers a destructor wrapper at `0x00465A50`. That wrapper passes
`0x00492548` to a separate cleanup routine. Ghidra xrefs to `0x00492548` are
confined to that independent static-object lifecycle.

A default-layout pinned VC7.1 structure using only those TH10-observed field
widths naturally has size `0x58` and the observed offsets; no packing trick is
required. The original TH10 type identifier remains unproven, so no type-name or
source-file claim is promoted from this size result alone.

## Target-observed original source path

The attested TH10 string table contains:

- `.\src\core\main.cpp:92 SprtCtrlInf`
- `.\src\core\main.cpp:84 FuncCtrlInf`

This proves that `src/core/main.cpp` participated in the original build. It is
not a per-function ownership proof: Ghidra reports no xrefs to either string, and
a bounded raw literal-address scan over the string ranges also finds no code
reference. Consequently no function ledger row received `source_file`, no
`config/implemented.csv` source mapping was added, and no TU ownership claim is
made from string proximity.

## Adjacent-game hypothesis discipline

Adjacent live states observed during the packet were:

- TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97`, clean;
- TH09 HEAD `1a4b36f2b7da18b20e808099817ab70105bacd74`, clean; bounded committed
  searches did not provide a useful corresponding Main/window seam;
- TH095 HEAD `d8949073a3cb10586e3a5e0f9436594cede2e58b`, with unrelated tracked and
  untracked work. Only committed `HEAD:` Main source was consulted.

Committed TH08/TH095 source supplied naming and source-shape hypotheses for
window creation, WindowProc, system-state handling, and D3D-interface setup.
No adjacent address, extent, layout, ABI, ownership, exactness, or completion
claim was transferred. TH10 target evidence determined the layout, private ABIs,
and denominator additions above.

## Compiler diagnostics and exactness discipline

Current ignored diagnostics live under
`.analysis/gpt-web/20260912-main-window-owner/`. The pinned compiler profile used
for source-shape feedback was `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG`.

The first compile attempt failed because the pinned SDK headers did not expose a
prototype for `WINNLSEnableIME`. Factory recorded the tracked/untracked tree as
clean before and after the failed command; live status was re-read immediately.
Only the ignored diagnostic source was then given an explicit Win32 ABI
prototype, after which compilation succeeded.

Diagnostic results are:

- default VC7.1 layout assertions: the TH10-derived storage fields naturally
  produce size `0x58` and the observed offsets;
- explicit normal-COFF HINSTANCE helper: consumes the argument from `[esp+4]`,
  unlike target `0x00439730` using live-in EBX;
- normal-COFF standard callback helper: produces the expected four-argument
  callee-pop Win32 callback family;
- `0x004392E0`: 100-byte object, 68/68 comparable non-relocation bytes;
- `0x00439350`: 62-byte object, 42/42 comparable non-relocation bytes;
- `0x00439660`: 152-byte object, 96/96 comparable non-relocation bytes.

The three comparator results are `structural-exact`, but every one has
`acceptance_authority=none`. They are probe-mode diagnostics, not canonical
match units. A `/GL` intermediate was also produced only to verify that the
source compiles through the pinned LTCG path; the repository has no accepted
linked-image LTCG extent Oracle. The unreferenced reproducible `/GL` intermediate
was removed during artifact cleanup and grants no exactness credit.

No compiler flags were changed merely to force a match, and LTCG was not stripped
from the hypothesis to fit the normal-COFF Oracle.

## Ledger and verification-plane state

After this packet:

- tracked candidates: **1,198**;
- origin/boundary pending: **1,184**;
- reviewed authored: **13 functions / 5,209 bytes**;
- reviewed exclusions: **1**;
- source-present authored mappings: **0**;
- canonical exact functions/bytes: **0 / 0**.

This packet reviewed three existing candidates and added three previously missing
authored target extents. It therefore expands the denominator rather than merely
improving a fixed review ratio. The six reviewed/new functions add 965 authored
bytes relative to the session entry state.

Verification planes remain independent:

- **Source presence: 0.** No production source or source mapping was added. The
  `src/core/main.cpp` target string is real build evidence but has no per-function
  ownership xref.
- **Function/owned-extent exactness: 0.** Structural probe equality is not a
  canonical target-bound normal-COFF unit and provides no accepted exactness.
- **Whole Windows i386 build closure: open.** `scripts/build.py --check` passes
  the explicitly open skeleton; the honest `scripts/build.py` run returns RC=2
  because compile profile, TU partition, libraries, resources, and link order
  remain unresolved.
- **Runtime validation: not started.** No runtime claim is available.
- **Truth Kernel acceptance: unavailable for refresh.** The shared operator-path
  lock prevented a current snapshot; no new accepted fact is claimed.

`config/implemented.csv`, `config/reccmp-functions.csv`, `config/matches.csv`,
`config/match-units.toml`, and `config/build.toml` were not populated or promoted
by this packet.

## Commands and validation actually run

Mandatory recovery/preflight work included the Factory authority/repository/live
status calls and:

```text
python3 scripts/verify-target.py
python3 scripts/verify-toolchain.py --execute
python3 scripts/validate-tracking.py --require-target
python3 scripts/report-reconstruction-status.py
python3 scripts/ci.py
```

Factory-native Ghidra used discovered schemas for `check`, `function`, `callers`,
`callees`, `decompile`, `disassemble`, `xrefs_to`, and `search_strings`. Target
raw scans were bounded to selected entries, adjacent padding, storage ownership,
and direct/literal-reference checks.

Focused compiler feedback used `scripts/compile-probe.sh` for normal COFF and
`/GL` diagnostics plus `scripts/compare-coff-function.py` probe mode. No canonical
exact unit was created.

After ledger edits, the following passed:

```text
python3 scripts/validate-tracking.py --require-target
python3 scripts/progress.py --check
python3 scripts/build-match-unit.py --check
python3 scripts/build.py --check
python3 scripts/report-reconstruction-status.py
python3 scripts/ci.py
git diff --check
```

The honest `python3 scripts/build.py` diagnostic returned the expected RC=2/open
state. A post-edit native Ghidra `check {}` also passed the exact target and
factory-native transport attestation.

## Analysis artifacts

`.analysis/` measured **47,612 bytes** at session entry. After the current probes
and bounded reports it grew to 82,188 bytes; after deleting only the
current-session, reproducible, unreferenced `window-owner-probe-ltcg.obj` and
marking the campaign manifest checkpointed, the final inventory measured
**62,774 bytes**. No current-session artifact exceeds 64 MiB and the campaign is
far below the 256 MiB review budget.

Current retained scratch under
`.analysis/gpt-web/20260912-main-window-owner/` is:

- `window-owner-probe.cpp`: natural diagnostic source;
- `window-owner-probe-coff.obj`: compact normal-COFF reproducer;
- `system-init-report.json`: compact `0x004392E0` structural report;
- `system-restore-report.json`: compact `0x00439350` structural report;
- `timestamp-unlocked-report.json`: compact `0x00439660` structural report;
- `manifest.json`: campaign bindings and dispositions.

Removed current-session artifact:

- `window-owner-probe-ltcg.obj`: reproducible, producer complete, no tracked or
  active reference; removed only after its `/GL` compilation role was recorded.

No legacy bootstrap state, previous campaign root, target, Wine prefix, Ghidra
project, toolchain, or unknown artifact was deleted. The manifest is marked
`checkpointed`; its ignored commit binding is refreshed to the final amended
checkpoint after the tracked handoff is committed.

## Remaining unknowns and next evidence-connected packet

The main blockers are now narrower but still material:

1. physical normal-COFF versus LTCG ownership for the Main/window source bodies;
2. original per-function ownership inside the observed `src/core/main.cpp` build
   path, because the source-path strings have no code xrefs;
3. whether private EBX/EDI/EAX machine ABIs are direct LTCG consequences, source
   declarations, or a mixture of inlining and retained out-of-line bodies;
4. original identifiers for the raw `0x00439350` and `0x00439660` functions;
5. TU partition, link order, libraries/resources, and production whole-build
   inputs;
6. earlier unmodeled `0x00439040/60/80/A0` blocks remain unresolved and are not
   yet added to the denominator.

The preferred next hard packet is the connected startup/shortcut owner seam
centered on `0x00439FF0-0x0043A1A9` (442 bytes), with only the helper context
needed to explain its mutex/shortcut/module-path behavior. This function writes
the reviewed window-storage `+0x28` launch-path flag and is adjacent in behavior
to the target-observed `src/core/main.cpp` source path. It is therefore a direct
owner/TU test rather than an easy remaining leaf-function choice. The packet
should also revisit the three structural-exact diagnostic bodies only if new
TH10 evidence can establish their physical normal-COFF/LTCG owner; otherwise
exactness must remain unknown.

The campaign remains active. A checkpoint, structural probe equality, future
review percentage, or even a later 99.5% reviewed ratio does not authorize
GPT-web to declare the exact phase complete.
