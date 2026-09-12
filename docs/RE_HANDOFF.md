# TH10 exact-reconstruction handoff

## Campaign state

The exact-reconstruction campaign remains active and incomplete. This conversation
reviewed one hard Main/startup owner-and-ABI packet centered on
`0x00439FF0-0x0043A1A9`, followed its immediately connected checksum/shortcut and
dialog-callback cohort, expanded the target denominator by two Ghidra-missed
authored bodies, and demonstrated with the pinned VC7.1 build-6030 toolchain that
LTCG can naturally perform unused-argument elimination and private-register
calling-convention transforms. It did not establish source presence, canonical
function exactness, whole Windows i386 build closure, runtime validation,
semantic reconstruction, ports, or new Truth Kernel acceptance.

Repository selection was exactly `th10`, analysis provider exactly
`th10-ghidra`, and target exactly `target:th10-main`. The ignored operator target
remained at `resources/th10.exe`; it was verified in place and was never modified,
replaced, relocated, staged, or committed. No `/mnt` search or
`TH10_TARGET_PATH` override was used.

Session starting HEAD was
`8e6189b175ba1be75824a5053b85837433844f33` on `main`. The tracked/untracked tree
was clean at entry: zero staged, unstaged, untracked, or conflicted paths. The
checkpoint containing this handoff should use subject
`gpt-web: review TH10 startup owner seam`; the live Git commit containing this
file is the authoritative ending HEAD. No push was requested or executed in this
conversation.

At session entry the local checkpoint was one commit ahead of `origin/main`.
During read-only target work, `origin/main` moved externally to the same
`8e6189b...` commit. Packet work was paused and recovery was repeated: HEAD,
complete staged/unstaged diffs, untracked paths, and `.analysis` were unchanged.
The remote-tracking reflog records
`2026-09-12T18:55:16+08:00 update by push`. This conversation did not perform
that push and does not infer who did.

## Recovery gate and preserved state

The mandatory `factory_describe`, repository-list, and live TH10 status checks
passed before editing. Recovery inspection covered branch/HEAD/upstream, recent
commits, porcelain-v2, complete staged and unstaged diffs, all relevant untracked
paths, ignored analysis/tool/build/provider state, the previous handoff, and the
`origin/main` reflog after the external move.

All repository instructions, Factory contracts, Factory guidance paths named by
the session prompt, the additional semantic-reconstruction guidance referenced
by `AGENTS.md`, and the repository-local TH10 reconstruction skill body were
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
- `.analysis/gpt-web/20260912-startup-owner/`: current-session bounded compiler
  diagnostic campaign; see the artifact section.
- `build/`: generated/open build state; no recovery edit was required.

No reset, stash, bulk cleanup, target edit, or deletion of unknown/legacy state
was used to manufacture a clean tree.

Multiple Factory repository commands encountered transport-layer connection
failures during read-only import/source queries and closeout validation/audit
shells. After every failure, live repository status was re-read before retry,
including staged/unstaged state, untracked paths, and the current `.analysis`
inventory as applicable. No unexpected repository side effect was present; work
continued only from the recovered live state. No repository recovery edit was
required.

## Identity and mandatory preflights

The private target passed `python3 scripts/verify-target.py` with SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`,
MD5 `7dc488d82c81dd4aee4ba098b8804d83`, PE32 i386 image base `0x00400000`,
entry `0x004537DC`, four sections, and Rich dominant build 6030.

`python3 scripts/verify-toolchain.py --execute` passed the hash-pinned VC7.1 SP1
normal-COFF, C++ LTCG, resource, and PE32-link smoke under headless Wine.
Tracking, reconstruction status, and public CI preflights also passed.

Native Ghidra operation schemas were discovered before target-dependent work.
The mandatory `check {}` and every useful semantic query in this packet returned
`attestation.status=passed`, target `target:th10-main`, and
`attestation.provider_transport=factory-native-command`. The provider remained
read-only and semantic results carried `exactness_credit=none`.

Truth Kernel accepted-snapshot reads were attempted twice during the packet and
closeout. Both were unavailable because another operation owned the shared
operator path. This absence is neither acceptance nor rejection; no new accepted
fact is claimed.

## Packet selection and observable goal

The selected hard packet was the previous handoff's connected startup/shortcut
owner seam rather than a smallest-function choice:

- `0x00439FF0-0x0043A1A9`, 442 bytes, initially `unknown/review`;
- immediately adjacent raw/known bodies needed to reconcile physical ownership
  and call flow: `0x0043A1B0`, `0x0043A1C0`, and `0x0043A290`;
- a raw callback at `0x0043A3A0` discovered because WinMain passes that target
  address directly to `DialogBoxParamA`.

The intended observable was to resolve the startup singleton/shortcut/checksum
source roles and machine ABIs, test whether the private-register Main seam can be
explained by natural VC7.1 link-time optimization rather than invented source
calling conventions, and challenge the Ghidra denominator at the same owner
boundary. The result is a reviewed authored boundary/ABI advance, two new
function candidates, and a compiler-mechanism result. Per-function normal-COFF
versus LTCG ownership remains unresolved, so no production source or exact unit
is justified.

## Reviewed `0x00439FF0-0x0043A1A9` — proposed `GameWindow::CheckForRunningGameInstance`

This 442-byte extent is now reviewed `authored_game / Main / high`. WinMain is
its unique direct caller and calls it without an explicit stack argument. Direct
target behavior is:

- create the named mutex `Touhou 10 App` and return failure if another instance
  already owns it;
- read startup information, module filename, and console title;
- follow shortcut paths through reviewed `0x0043A290` when the startup path ends
  in `.lnk`;
- compare the resolved startup path to the module path;
- write byte `1` to `0x00492518`, the reviewed 0x58-byte window/timing storage
  at `0x004924F0 + 0x28`, when the launch path differs;
- maintain the target startup-state bit at `0x00491FF4` and return target-local
  success/failure state.

The extent ends at `0x0043A1A9`; `0x0043A1AA-0x0043A1AF` is `0xCC` padding before
an independent retained raw body.

Committed TH08/TH095 source provides a close
`GameWindow::CheckForRunningGameInstance(HINSTANCE)` source-shape/name
hypothesis, but the adjacent HINSTANCE parameter is unused and is not transferred
to TH10: the target call has no explicit argument. The source-level declaration,
TU, physical normal-COFF versus LTCG owner, and exactness remain unknown.

## Newly tracked `0x0043A1B0-0x0043A1B7` — proposed `GameWindow::ActivateWindow`

This eight-byte target body was missing from the original Ghidra candidate
inventory. It consumes an HWND through live-in EAX, pushes that value, calls the
`USER32.dll!SetForegroundWindow` import, and returns. No direct or literal entry
xref was found. `0x0043A1B8-0x0043A1BF` is `0xCC` padding before
`0x0043A1C0`.

The reviewed WindowProc already contains the same foreground-window operation
inline in its target message path. The physical ordering also matches committed
adjacent Main source that places an `ActivateWindow` source function between the
single-instance and executable-checksum routines. These are corroborating
signals only; TH10 target behavior controls the boundary and ABI.

A natural pinned normal-COFF `ActivateWindowProbe(HWND)` is 12 bytes and first
loads the argument from `[esp+4]`; the TH10 retained body is eight bytes and uses
live-in EAX. This excludes the ordinary standalone normal-COFF source ABI for the
target machine body. The generic pinned LTCG diagnostic below demonstrates that
this transformation class is naturally possible, but does not prove the target
body's physical owner or exactness. `GameWindow::ActivateWindow` is therefore a
proposed name only.

## Reviewed `0x0043A1C0-0x0043A28E` — proposed `GameWindow::CalcExecutableChecksum`

This 207-byte extent is now reviewed `authored_game / Main / high`. WinMain is
its unique caller. The target:

- obtains the current module filename;
- loads the executable through `0x0044B360`;
- sums the loaded image by dwords while excluding the final dword;
- frees the temporary data;
- stores the resulting checksum at `0x00492380` and file size at `0x00492384`.

The call to `0x0044B360` is compiler-sensitive: the filename is supplied through
live-in EAX, while `size_out` and mode `1` are stack arguments. The target loader
has 15 direct callers using this mixed-ABI family. The reviewed checksum extent
ends at `0x0043A28E`; `0x0043A28F` is one `0xCC` byte before `0x0043A290`.

Committed TH08 source supplies a close `GameWindow::CalcExecutableChecksum`
source-shape/name hypothesis. No adjacent address, ABI, ownership, or exactness
is transferred.

## Reviewed `0x0043A290-0x0043A39F` — proposed `GameWindow::ResolveShortcut`

This 272-byte extent is now reviewed `authored_game / Main / high`. Its unique
caller is `0x00439FF0`. Direct target disassembly establishes a mixed internal
machine ABI:

- shortcut path: one stack argument, consumed by the callee, which returns with
  `ret 4`;
- destination buffer: live-in EBX;
- destination capacity: live-in EDI, supplied as `0x104` by the caller.

The body initializes COM, creates and queries ShellLink-related interfaces,
converts the input path to wide characters, loads the shortcut through the COM
persistence interface, obtains the resolved path into the live-in destination,
releases interfaces, and uninitializes COM. The function ends immediately before
`0x0043A3A0`; there is no padding gap.

Committed TH08/TH095 source supports `ResolveIt`/`GameWindow::ResolveShortcut`
as source-shape/name hypotheses. The ledger uses
`GameWindow::ResolveShortcut` only as a proposed name; the original declaration,
TU, physical normal-COFF/LTCG owner, and exactness remain unknown.

## Newly tracked `0x0043A3A0-0x0043A488` — configuration dialog callback

Ghidra did not model a function at `0x0043A3A0`, but target-local ownership is
direct: WinMain passes the address as the DLGPROC argument to
`USER32.dll!DialogBoxParamA`. Raw target disassembly closes a 233-byte callback
extent ending with `ret 0x10` at `0x0043A488`; `0x0043A489-0x0043A48F` is seven
bytes of `0xCC` padding before unrelated following code.

The body handles `WM_INITDIALOG` (`0x110`) and `WM_COMMAND` (`0x111`) and uses the
target imports `SendMessageA`, `GetDlgItem`, `EndDialog`, and
`IsDlgButtonChecked`. The machine ABI is therefore the four-argument Win32 DLGPROC
family. The original identifier and source owner remain unknown, so no proposed
source name is invented.

## Pinned VC7.1 LTCG calling-convention diagnostic

The current ignored campaign contains a small natural C++ diagnostic compiled
with the pinned VC7.1 SP1 build-6030 toolchain. The source declares a noinline
three-argument callee whose first argument is unused, plus one caller. It was
compiled both as normal COFF and with `/GL`; the `/GL` object was linked using the
pinned linker with `/LTCG`.

Normal COFF uses the ordinary stack ABI: the caller pushes all three arguments and
the callee reads the surviving inputs from `[esp+8]`/`[esp+0xC]`. The linked LTCG
image instead eliminates the unused argument from the internal machine boundary,
passes surviving inputs in EAX/ECX, and tail-jumps from the caller to the callee.

This directly demonstrates that VC7.1 build 6030 can naturally produce the same
*class* of signature shrink and private-register ABI observed repeatedly in the
TH10 Main seam. It therefore prevents the reconstruction from inventing exotic
source declarations merely to mirror EAX/EBX/EDI machine inputs. It does **not**
prove that any particular TH10 function came from one of the target's 52 LTCG C++
Rich inputs; per-function physical ownership and exactness remain unknown.

The separate natural normal-COFF `ActivateWindowProbe(HWND)` diagnostic confirms
the ordinary source boundary uses a stack argument and is 12 bytes, unlike the
8-byte target EAX body. Both diagnostics have `exactness_credit=none` and are not
canonical match units.

## Adjacent-game hypothesis discipline

Adjacent live states observed during this packet were:

- TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97`, branch
  `port/portable-64bit`, clean;
- TH09 HEAD `8aa2716a099af6fd4354294ebb73d170080fbc54`, branch `main`, clean,
  one commit ahead of its own upstream; bounded committed searches did not yield
  a useful corresponding Main helper source;
- TH095 HEAD `f8043aa131929d142df058f34c95f799e0e2ec5a`, branch `main`, with
  unrelated modified `src/PhotoGameTask.hpp`, `src/PhotoStage.cpp`, and
  `src/ResultScreen.cpp`, plus unrelated untracked runtime/reconstruction files.
  Only committed `HEAD:` content was consulted.

Committed TH08/TH095 source supplied naming/source-shape hypotheses for the
single-instance, activate-window, executable-checksum, and shortcut-resolution
routines. No adjacent address, target extent, ABI, object layout, exactness, or
completion state was transferred to TH10. The dialog callback remains unnamed
because adjacent committed searches did not establish a useful original-name
hypothesis.

## Ledger and verification-plane state

After this packet:

- tracked candidates: **1,200**;
- origin/boundary pending: **1,181**;
- reviewed authored: **18 functions / 6,371 bytes**;
- reviewed exclusions: **1**;
- source-present authored mappings: **0**;
- canonical exact functions/bytes: **0 / 0**.

The packet reviews three existing candidates and adds two previously missing
authored target extents. It adds 1,162 reviewed authored bytes relative to the
session entry state and expands the denominator rather than merely improving a
fixed ratio.

Verification planes remain independent:

- **Source presence: 0.** No production source file or source mapping was added.
  The target-observed `src/core/main.cpp` path remains build evidence without a
  per-function ownership xref.
- **Function/owned-extent exactness: 0.** The compiler work in this packet is
  diagnostic mechanism/ABI feedback, not a target-bound zero-difference match
  unit.
- **Whole Windows i386 build closure: open.** `scripts/build.py --check` passes
  the explicitly open skeleton. The honest `python3 scripts/build.py` invocation
  returned the expected RC=2 because compile flags/profile, TU partition,
  libraries, resources, and link order remain unknown.
- **Runtime validation: not started.** No runtime scenario or behavioral-closure
  claim is made.
- **Truth Kernel acceptance: unavailable for refresh.** Two snapshot attempts
  were blocked by the shared operator-path lock; no new accepted fact is claimed.

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

Factory-native Ghidra used discovered schemas for `check`, `function`,
`decompile`, `callers`, `callees`, `disassemble`, and `xrefs_to`. Every useful
result used the exact TH10 target/native transport attestation. Direct target PE
work was bounded to the selected startup/callback entries, their adjacent
padding, import slots, and entry-reference checks.

Focused compiler work used `scripts/compile-probe.sh` for normal COFF and `/GL`,
then the hash-pinned VC7.1 linker with `/LTCG` only inside the ignored diagnostic
campaign. `objdump`/link map output was reduced to a compact retained compiler
report. No canonical exact unit was created and `/GL` was not removed from a TH10
hypothesis to fit the normal-COFF Oracle.

After ledger edits, the following passed:

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

The honest `python3 scripts/build.py` diagnostic returned the expected RC=2/open
state rather than product closure.

## Analysis artifacts

`.analysis/` measured **62,774 bytes** at session entry. During the current
compiler experiment it reached 73,990 bytes before cleanup. After deleting only
current-session reproducible intermediates whose compact results were already
retained, the pre-checkpoint inventory measured **68,313 bytes**. After the
ignored manifest was marked `checkpointed` and bound to the local checkpoint,
the final closeout inventory measured **68,494 bytes**. No current-session
artifact exceeds 64 MiB, and the campaign remains far below the 256 MiB review
budget.

Current retained scratch under
`.analysis/gpt-web/20260912-startup-owner/` is:

- `ltcg-callconv-probe.cpp`: generic normal-COFF versus `/GL`+`/LTCG` ABI source;
- `activate-window-probe.cpp`: normal-COFF HWND ABI source;
- `compiler-shape-report.txt`: compact disassembly/link-map result;
- `manifest.json`: target/toolchain bindings, results, and artifact dispositions.

Removed current-session artifacts were all reproducible outputs with completed
producers and no remaining byte-level reference:

- `ltcg-callconv-normal.obj`;
- `ltcg-callconv-ltcg.obj`;
- `ltcg-callconv.exe`;
- `ltcg-callconv.map`;
- `activate-window-probe-coff.obj`.

No legacy bootstrap state, previous campaign root, target, Wine prefix, Ghidra
project, toolchain, or unknown artifact was deleted. The ignored manifest is
marked `checkpointed`, records `pushed=false`, and is rebound to the final live
commit during closeout.

## Remaining unknowns and next evidence-connected packet

The main blockers remain:

1. physical normal-COFF versus LTCG ownership and original source declarations
   for the reviewed Main/startup bodies;
2. per-function ownership within the target-observed `src/core/main.cpp` build
   path, whose source-path strings still have no code xrefs;
3. whether each EAX/EBX/EDI private machine boundary is an LTCG transform,
   explicit source declaration, inlining artifact, or a mixture; the new compiler
   diagnostic establishes mechanism plausibility but not per-function ownership;
4. original identifiers for the dialog callback and earlier retained raw helper
   bodies where target evidence does not establish a name;
5. whole-build TU partition, libraries, resources, link order, and production
   compiler flags/profile;
6. earlier unresolved `0x00439040/60/80/A0` blocks remain outside the denominator
   pending stronger origin evidence.

The preferred next hard packet is the directly connected target file-loader seam
centered on **`0x0044B360-0x0044B4C7` (360 bytes)**. It remains
`unknown/review` in the ledger and has **15 direct target callers** across several
subsystems, including the newly reviewed executable-checksum routine. Target
callers pass filename through live-in EAX while `size_out/mode` are stack inputs;
the callee selects archive/global-table versus real-file loading paths. Reviewing
this function with a representative caller/data-table cohort can therefore test
whether the widespread mixed ABI has one physical owner and recover file/archive
data ownership. It is materially more central and evidence-connected than simply
continuing into the small wrappers after `0x0043A490`, so it is not an easy-count
choice.

Immediately after the local checkpoint, the tracked/untracked worktree was clean
with zero staged, unstaged, untracked, or conflicted paths and the local branch
was one commit ahead of the externally updated `origin/main`. No push was
performed.

The campaign remains active. A checkpoint, a compiler mechanism diagnostic, a
future high review percentage, or even a later 99.5% reviewed ratio does not
authorize GPT-web to declare the exact phase complete.
