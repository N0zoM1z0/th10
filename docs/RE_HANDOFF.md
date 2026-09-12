# TH10 exact-reconstruction handoff

## Campaign state

The exact-reconstruction campaign remains active and incomplete. This conversation
reviewed one hard D3D/application-entry owner seam. It did not establish any
source-present mapping, canonical exact function, whole Windows i386 build,
runtime validation, or new Truth Kernel acceptance.

Repository selection was exactly `th10`, analysis provider exactly
`th10-ghidra`, and target exactly `target:th10-main`. The ignored operator target
remained at `resources/th10.exe`; it was verified in place and was not modified,
relocated, staged, or committed.

Session starting HEAD was
`9f7886f17954621890754ee86a90a3cd0d53a7c3` on `main`. The tree was clean at
entry: zero staged, unstaged, untracked, or conflicted paths. The checkpoint
containing this handoff should use subject `gpt-web: review TH10 D3D owner seam`;
the live Git commit containing this document is the authoritative ending HEAD.
No Factory repository command in this session performs network Git operations,
and no push was requested or executed.

## Recovery gate and preserved state

The mandatory Factory authority/repository/status checks passed. All required
repository and Factory guidance paths were mounted and read before editing.
Recovery inspection covered recent commits, porcelain-v2 status, complete staged
and unstaged diffs, untracked paths, relevant ignored state, build state, and
this handoff.

No tracked or untracked interrupted work required recovery. Ignored paths were
classified as follows:

- `resources/th10.exe`: operator-supplied private target; preserved and excluded.
- `.tools/`: shared/tool-selection and game-bound Wine state; preserved and
  excluded.
- `.analysis/bootstrap/`: pre-existing analysis material; preserved as
  legacy/pre-existing state without granting it authority.
- `.analysis/gpt-web/20260912-main-frontier/`: prior checkpointed diagnostic
  material; preserved.
- `.analysis/gpt-web/20260912-d3d-owner-seam/`: current-session reproducible
  diagnostic scratch; see the artifact section below.
- `build/`: no relevant file was present at recovery inspection.

No reset, stash, cleanup, or deletion was used to obtain a clean tree.

## Identity and preflight

The private target passed `scripts/verify-target.py` with SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`,
MD5 `7dc488d82c81dd4aee4ba098b8804d83`, PE32 i386 image base `0x00400000`,
entry `0x004537DC`, and Rich dominant build 6030.

`scripts/verify-toolchain.py --execute` passed the hash-pinned VC7.1 SP1
normal-COFF, C++ LTCG, resource, and PE32-link smoke under headless Wine.
Tracking, reconstruction status, and public CI preflights also passed.

Native Ghidra discovery was performed before target-dependent queries. A real
`check {}` and every useful query returned a passed attestation for
`target:th10-main` with
`attestation.provider_transport=factory-native-command`. Ghidra remained
read-only and every semantic result carried `exactness_credit=none`.

A Truth Kernel accepted-snapshot refresh was attempted twice during this
conversation, but both attempts were unavailable because another Factory
operation owned the operator path. Do not infer acceptance from this absence;
no current accepted-state result was obtained in this conversation.

## Reviewed hard-frontier packet

The packet was selected because it is directly connected to the previously
reviewed WinMain/device-reset seam and includes a 1,159-byte central D3D
initializer plus a 710-byte render-state initializer. It was not selected by
smallest function size.

### `0x004391F0-0x004392DB` — proposed `GameWindow::Present`

This 236-byte function is now reviewed as `authored_game` with high confidence.
Direct target bytes show a `ret` at `0x004392DB`, followed by four `0xCC` bytes
through `0x004392DF`; Ghidra independently identifies `0x004392E0` as the next
function.

Target-local behavior is sufficient to establish the role without relying on an
adjacent name: the function calls the D3D9 device `Present` vtable slot; on
failure it calls the reviewed EAX-private resource-release helper at
`0x00438A30`, resets the device with the presentation-parameter global, invokes
`0x00439D20`, and sets a reset countdown/state. It then processes the
`snapshot/th%.3d.bmp` capture path.

`GameWindow::Present` is only a proposed source name. Committed TH08/TH095
sources contain a static function with the same engine role and unusually close
control-flow shape, but those sources do not prove the TH10 identifier, TU, or
owner. No source mapping or exactness claim was added.

### `0x00439890-0x00439D16` — proposed `GameWindow::InitD3DRendering`

This 1,159-byte function is now reviewed as `authored_game` with high confidence.
The target ends in `ret` at `0x00439D16`; `0x00439D17-0x00439D1F` is nine bytes
of `0xCC` padding before independent `0x00439D20`.

WinMain calls it directly and treats nonzero return as startup failure. The body
constructs D3D9 presentation parameters, attempts HAL hardware-vertex-processing,
HAL software-vertex-processing, and REF fallbacks, retries refresh/presentation
settings, copies the selected presentation parameters into the persistent global,
constructs the view/projection matrices, queries viewport/device capabilities,
checks texture-format support, invokes `0x00439D20`, then clears startup/window
state and returns success.

The proposed `GameWindow::InitD3DRendering` name is corroborated by committed
TH08/TH095 source, while TH10 target control flow independently establishes the
D3D-initialization role. Original identifier, source file, TU partition, and
normal-COFF/LTCG owner remain unknown.

### `0x00439D20-0x00439FE5` — proposed `GameWindow::ResetRenderState`

This 710-byte function is now reviewed as `authored_game` with high confidence.
It ends in `ret` at `0x00439FE5`; ten `0xCC` bytes through `0x00439FEF` precede
independent `0x00439FF0`. Its three reviewed callers are the WinMain device-reset
path, `0x004391F0`, and `0x00439890`.

A material ABI correction was made. Ghidra provisionally described the function
as `__fastcall` with one parameter, but the target instruction stream contradicts
that interpretation. Entry `push ecx` reserves one local dword. That same stack
slot is overwritten with the bit patterns for `1.0f`, `1000.0f`, and `5000.0f`
and passed to `SetRenderState`; exit `pop ecx; ret` releases the scratch. The
first render-state call independently pushes the constant value `1`, so the
saved ECX is not its source parameter. All three target callers issue a direct
zero-stack-argument call. The source-level no-argument calling-convention family
is still not distinguishable merely from `ret`, so no calling convention was
invented in the ledger.

The target applies a fixed D3D9 render-state sequence, then texture-stage states,
then sampler states. The tail conditionally writes five bytes/dwords inside the
large manager object referenced by `0x00491C10`. Committed TH095 source suggests
these may correspond to inlined render-cache clearing methods, but TH10 does not
yet establish the manager type/layout or exact source-level method ownership.
Those writes therefore remain an owner/layout blocker rather than being encoded
as a guessed structure.

A pinned VC7.1 `/O2` diagnostic probe compiled a no-argument C++ function with a
float local and virtual render-state calls. The normal-COFF object naturally
emitted the same `push ecx`, stack-dword stores of `1.0f`/`1000.0f`/`5000.0f`,
and `pop ecx; ret` source shape. A `/GL` probe also produced a valid LTCG
intermediate object. This confirms that the observed local-scratch shape is
natural VC7.1 output; it does not identify which artifact family owns the TH10
function and grants no exactness credit.

## Adjacent-game hypothesis sources

Only committed adjacent content was used. The observed repository states were:

- TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97`, clean.
- TH09 HEAD `943ec407a9429379fa9fdbcde97dd246e51f10a8`, clean; it did not supply a
  materially useful D3D-main analogue for this packet.
- TH095 HEAD `d8949073a3cb10586e3a5e0f9436594cede2e58b`, with unrelated live
  modifications/untracked files. The dirty TH095 worktree was not consulted for
  hypotheses; only `git show HEAD:...` committed content was read.

TH08/TH095 both supplied `GameWindow::Present`, `InitD3DRendering`, and
`ResetRenderState` source-shape hypotheses. TH10 differs materially in D3D9
state layout and constants, including an alpha-reference value of 1, unconditional
fog enable in the reviewed target function, D3D9 sampler-state calls, and no
corresponding edge-antialias state call in the reviewed extent. TH10 evidence
wins over adjacent source wherever they differ.

## Ledger and verification-plane state

After this packet the ledgers report:

- 1,195 tracked candidates;
- 1,189 origin/boundary reviews pending;
- 5 reviewed authored functions totaling 3,537 bytes;
- 1 reviewed exclusion;
- 0 source-present mappings;
- 0 canonical exact functions and 0 canonical exact authored bytes.

The authored denominator is still moving and incomplete. Proposed names do not
count as source presence. No canonical match unit or `config/matches.csv` exact
row was added.

Verification planes remain separate:

- Source presence: **0**; no TH10 production source file was added in this packet.
- Function/owned-extent exactness: **0**; no target-bound zero-difference Oracle
  passed.
- Whole Windows i386 build closure: **open**. `scripts/build.py --check` passes
  the explicitly open skeleton; the honest `scripts/build.py` diagnostic exits
  RC=2 because compile flags, TU partition, libraries, resources, and link order
  remain unknown.
- Runtime validation: **not started**; no runtime claim is available.
- Truth Kernel acceptance: **unavailable for refresh in this conversation** due
  to the Factory operator-path lock; no new acceptance is claimed.

## Commands and checks actually run

The conversation ran the mandatory target/toolchain/tracking/status/public-CI
preflights, native Ghidra discovery plus `check {}`, bounded Ghidra
`function`/`decompile`/`callers`/`callees`/`xrefs_to`/`disassemble` queries, and
direct read-only PE byte/disassembly boundary inspection.

Focused compiler feedback used `scripts/compile-probe.sh` twice for the
render-state source-shape diagnostic: one normal COFF `/MT /O2 /Gy /GF /Oi
/DNDEBUG` build and one identical `/GL` build. These are diagnostics only.

After ledger edits the following passed:

- `python3 scripts/verify-toolchain.py --check`
- `python3 scripts/validate-tracking.py --require-target`
- `python3 scripts/progress.py --check`
- `python3 scripts/build-match-unit.py --check`
- `python3 scripts/build.py --check`
- `python3 scripts/report-reconstruction-status.py`
- `python3 scripts/ci.py`
- `git diff --check`

The honest `python3 scripts/build.py` run returned the expected RC=2/open state;
that is not a failed exact unit and not product closure.

One earlier read-only focused-search shell command returned RC=1 because it
assumed a TH10 `src/` directory that does not exist. Git status was immediately
rechecked and remained clean before edits; the search was rerun using tracked
file lists. No filesystem recovery was needed.

## Analysis artifacts

`.analysis/` was 25,077 bytes at conversation entry and 31,831 bytes at the
final session inventory. No artifact was removed in this session. The current
campaign uses
`.analysis/gpt-web/20260912-d3d-owner-seam/` with a manifest and the following
small reproducible diagnostics:

- `reset-render-state-probe.cpp`
- `reset-render-state-probe-coff.obj`
- `reset-render-state-probe-ltcg.obj`

No current-session artifact approaches the 64 MiB review threshold. These files
are ignored diagnostic scratch, not a knowledge base and not exactness evidence.
The previous bootstrap and main-frontier analysis roots remain untouched.

## Remaining unknowns and next evidence-connected packet

The principal blockers are still physical/TU ownership, per-function
normal-COFF versus LTCG context, the large manager object's real type/layout,
and the source-level identity of the render-cache writes following
`0x00439D20`. These blockers are why no production `Main.cpp` was manufactured
from adjacent source.

The next preferred packet is the directly connected render/timing cohort:

- `0x00439390-0x00439538` (425-byte span), the frame/render dispatcher that calls
  `0x004391F0`;
- `0x00439540-0x00439659` (282 bytes), its timing source;
- the global object at `0x004924F0` passed on the stack to `0x00439390`, plus the
  relevant call sites and adjacent committed `GameWindow::Render` hypotheses.

This packet is selected to resolve an ABI/owner contradiction, not because it is
small. WinMain explicitly pushes `0x004924F0` before calling `0x00439390`, while
adjacent games expose an instance-style `GameWindow::Render`; TH10 must determine
whether this is a static/helper ABI, optimizer transformation, or a genuinely
different source shape. That result can establish useful `GameWindow` layout and
TU evidence needed before production source is introduced.

A secondary route, if that ABI packet becomes blocked, is `0x00439730-0x00439884`
(341 bytes), the direct setup function called immediately before the reviewed
D3D initializer. `0x00439FF0-0x0043A1A9` is another larger same-module candidate,
but it is less directly connected to the current unresolved render-owner ABI and
should not be selected merely to accumulate reviewed counts.

The campaign remains active. Crossing any future reviewed ratio target does not
authorize GPT-web to declare the exact phase complete.
