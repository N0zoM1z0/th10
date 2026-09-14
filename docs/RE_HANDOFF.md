# TH10 exact reconstruction handoff

## Recovery and authority

The campaign remains active in exact source reconstruction with early faithful
Windows i386 build feedback. Source presence, compilation, canonical exactness,
whole-build closure, runtime validation, and Factory Truth Kernel acceptance are
independent states. Native Ghidra has `exactness_credit=none`.

This conversation started clean on branch `main` at
`81b10925249c4f0620ffa467895f3659682720de`, with
`origin/main=ae83c3ab72400534834746aeb7613a603a347310`, ahead2/behind0, 0 staged,
0 unstaged, 0 untracked, and 0 conflicts. No interrupted tracked work required
recovery. Ignored private `resources/th10.exe`, `.analysis/`, `.tools/`, `build/`,
`ghidra-project/`, caches, and other prior state were preserved. The private
executable was not modified, replaced, relocated, or staged; no `/mnt` search
and no `TH10_TARGET_PATH` override were used.

All prompt-named repository rules, Factory contracts, and Factory guidance paths
were reread through the registered repository runner before editing. No guidance
path was missing. Target, executable toolchain, tracking, reconstruction status,
public CI, canonical-match graph, and open whole-build graph preflights passed.
Honest `scripts/build.py` returned expected RC=2/open because production compile
flags, TU partition, libraries, resources, and link order remain unknown.

Native `th10-ghidra` operation discovery exposed the documented bounded read
schemas. Mandatory `check {}` passed exact `target:th10-main` with
`attestation.provider_transport=factory-native-command`; the private target SHA-256
remains `2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Several Factory calls suffered transient network failures. In each case the live
repository status or durable output was reread before continuing; failed patch
calls were confirmed not to have partially edited tracked files before retry.
A helper-decompile query failed once and then succeeded with a fresh passed
attestation. No timeout or failure is treated as a rolled-back transaction.

A current Factory accepted-snapshot request is unavailable because another
Factory operation owns the operator path. No accepted/rejected/pending count is
inferred from that unavailable check.

## Hard packet: central Player update callback

This packet attacked the directly registered Player update owner rather than an
easy leaf. Reviewed target scope:

- `0x00425730-0x00426340`: 3,089-byte central Player update body
- `0x00426500-0x00426506`: seven-byte registered first-lane adapter.

The packet is directly connected to the previous Player draw seam, reviewed
`RebuildPlayerOptions`, option `+0x90` callbacks, and the next movement/options
owner.

### Physical boundary and machine ABI

Ghidra recognizes `0x00425730-0x00426340` but reports only 3,060 reachable body
bytes. Raw target review reconciles the 29-byte difference exactly with five
retained unreachable alignment sleds skipped by control flow:

- `0x004257C8-0x004257CF`: 8 bytes
- `0x0042580D-0x0042580F`: 3 bytes
- `0x00425858-0x0042585F`: 8 bytes
- `0x00425A3D-0x00425A3F`: 3 bytes
- `0x00425B39-0x00425B3F`: 7 bytes.

The physical body ends with `ret 4` at `0x0042633E-0x00426340`. It is followed
by a separate three-byte alignment LEA at `0x00426341-0x00426343`, then a
20-byte five-entry state jump table at `0x00426344-0x00426357`, then CC padding
through `0x0042635F`. None of that post-body table/padding is absorbed into the
3,089-byte function extent.

Ghidra's provisional `__thiscall` signature is rejected by raw evidence. The
body prologue loads the sole Player pointer from `[ESP+0x28]` after saving its
frame state, and the epilogue consumes exactly one four-byte stack argument.
The only observed direct transfer is the raw call at `0x00426501`. The
seven-byte adapter is exactly `push ecx; call 0x00425730; ret`, so its callback
boundary is live-in ECX Player while the body boundary is one stack Player* /
`ret 4`.

The body is classified `authored_game` / Player with high confidence because it
contains a large bespoke Player state machine, power rules, option rebuilding,
effect/timer updates, movement seams, and shot/update integration. This origin
classification does not resolve whether the seven-byte adapter is source-written
or compiler/LTCG-generated, nor whether the maintained source function maps
physically to the body, adapter, or an optimizer split. The adapter therefore
remains `unknown/review`.

## State-machine and layout evidence

The target state jump table selects numeric states 0 through 4, but raw target
layout establishes physical lexical block order `0, 1, 4, 2, 3`. State 4 can
call the connected transition helper and fall through into the physical state-2
block. Maintained source preserves that lexical order rather than sorting case
labels for readability.

Target-local behavior recovered for the five states includes:

- state 0 drives the initial vertical position from the Player timer, marks all
  four option reset flags, reconstructs the 33-row replay/history seam, runs
  bounded transition effects/cleanup, then changes to state 1 after 60 frames;
- state 1 handles the observed power/input gate, updates the displayed power,
  invokes `RebuildPlayerOptions`, performs early cleanup, and calls the connected
  movement/options owner at `0x004250B0`;
- state 4 contains a short transition window and can enter state 2 through the
  connected `0x004269D0` helper;
- state 2 performs the observed 64-power transition, seven-direction effect
  emission using target constants, invokes `RebuildPlayerOptions`, and later
  either requests another runtime transition or restores the observed Player
  position/draw state;
- state 3 owns the observed frame-15 cleanup path.

The common tail independently establishes several maintained partial layouts:

- four 0x14 timer-shaped Player records at `+0x460`, `+0x474`, `+0x488`, and
  `+0x430C`;
- 32 repeated 0x6C Player effect rows beginning at `+0x350C`;
- the draw-VM color field at Player `+0x314`, in addition to prior draw-VM
  position/flags evidence;
- float Player draw position as one contiguous Float3 at `+0x3C0`;
- fixed movement speed/delta and direction fields through `+0x454`;
- six contiguous derived Float3 rows at `+0x4324`;
- the existing replay/history array at `+0x436C` and option mode at `+0x4474`.

These are deliberately partial maintained views. Original class/type names,
complete object extents, timer type identity, and field identifiers remain
unknown. Neutral helper interfaces in `src/Player.cpp` spell observed calls
without claiming their original names or private register ABIs.

Maintained `PlayerUpdateCallback` is source-present at registered entry
`0x00426500`. The adjacent 3,089-byte body remains without a standalone
`source_file` mapping so source presence is not confused with physical source or
optimizer ownership.

## Compiler and exact feedback

Final `src/Player.cpp` compiles under pinned VC7.1 SP1 build 6030 using fixed
normal:

`/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc`

and the same profile plus `/GL`. `src/ReplayManager.cpp`, which consumes the
expanded Player layout, also compiles under both profiles.

The true fixed-normal `/Gy` `PlayerUpdateCallback` COMDAT is `0xAF0` = 2,800
bytes. It naturally emits the same five-way switch family and target-observed
lexical block ordering, but that object is not the target physical owner. The
target has a separate seven-byte registered adapter, a 3,089-byte stack-bound
body, and its five-entry jump table outside the body.

A caller-supplied 3,089-byte relocation-aware diagnostic against target
`0x00425730` returns `mismatch`: 191/2,693 comparable bytes match, 99 object
relocations are reported, and `acceptance_authority=none`. The comparator's
reported 3,089-byte requested window must not be confused with the true 2,800-byte
COMDAT extent. Fixed normal is therefore excluded as an exact physical match.
Successful `/GL` compilation preserves a live optimizer/LTCG ownership
hypothesis, but the standalone COFF Oracle cannot assign linked-image LTCG
extent or exactness. No canonical match unit and no exact ledger row were added.

## Ledger delta and adjacent-game discipline

Entry ledger state at `81b10925249c4f0620ffa467895f3659682720de`:

- candidates 1,239
- origin/boundary pending 1,145
- authored 87 / 22,010 bytes
- exclusions 7
- source-present 60
- canonical exact 0.

Current pre-checkpoint state:

- candidates 1,239
- origin/boundary pending 1,144
- authored 88 / 25,099 bytes
- exclusions 7
- source-present 61
- canonical exact 0.

Delta: pending -1, authored +1 / +3,089 bytes, source-present +1, candidate count
unchanged, exclusions +0, exact +0. The authored denominator expands rather than
being hidden by the adapter/profile uncertainty.

Adjacent committed material was consulted only after TH10-local state-machine
and ABI evidence existed. TH08 HEAD
`a45e99fb1942714e6edded20847e32a654d56f97` was clean; its exact Player source
was used only to corroborate that VC7 Player switch lexical order and timer
source shape can be target-visible. TH09 HEAD
`1cf5cf3a356df05aef20412a167ec146b4a54de0` was clean and ahead7; no TH09
content materially shaped this packet. TH095 HEAD
`f93406f4488acb29240c048a17d36f67d28abf5f` was ahead3 with unrelated modified
`src/PhotoGameTask.cpp` plus four unrelated untracked files. No TH095 dirty
content was read or used. No adjacent address, layout, state value, ownership,
exactness, or completion claim was transferred into TH10.

## Verification planes

Final pre-checkpoint state remains deliberately separated:

- source presence: yes at registered update entry `0x00426500`; the body/source
  physical ownership relationship remains unresolved
- origin/boundary: body reviewed and classified authored_game; adapter remains
  unknown/review
- canonical exactness: 0 functions / 0 bytes repository-wide
- whole faithful Windows i386 build: open; `build.py --check` passes the honest
  open graph and actual `build.py` returns expected RC=2
- runtime validation: not performed because there is no closed reconstructed
  product
- Factory Truth Kernel acceptance: unavailable because another Factory operation
  owns the operator path; no accepted, rejected, pending, or replay count is
  claimed.

Final focused normal and `/GL` Player/Replay compilation passed. `verify-target.py`,
`verify-toolchain.py --check`, target-bound tracking, reconstruction status,
regenerated progress, `build-match-unit.py --check`, `build.py --check`, public
CI, and `git diff --check` passed. The canonical normal-COFF match graph remains
empty.

## Scratch lifecycle

Session entry `.analysis/` was 78 regular files / 364,833 regular bytes and 612K
allocated, with no file over 64 MiB. This session created and reused
`.analysis/gpt-web/20260914-player-update/` with a manifest.

After final probes and cleanup, `.analysis/` is 87 regular files / 518,056
regular bytes and 780K allocated. The current campaign contains 9 regular files /
153,223 bytes, with no file over 64 MiB. Four header-sanity compiler objects and
the temporary `PlayerUpdateCallback.inc` were removed only after final probes
superseded them; they were current-session, reproducible, unreferenced scratch.
Retained artifacts are the final Player/Replay normal and `/GL` objects, the
relocation-aware mismatch JSON/stderr, whole-build stdout/stderr, and manifest.
No legacy/unknown/provider/toolchain/target state was deleted.

## Checkpoint and next hard frontier

Final pre-commit gates completed successfully: the complete working diff and
staged diff were audited, only the ten intended tracked packet files are staged,
there is no unstaged or untracked remainder, `git diff --cached --check` passes,
and a fresh native Ghidra `check {}` again passed exact `target:th10-main` with
`factory-native-command` transport. Final Player/Replay normal and `/GL` probes,
target/toolchain/tracking/status, open match/build graphs, honest RC=2 whole-build
diagnostic, and public CI also passed. Create one local English `gpt-web:`
checkpoint and never push. Read the checkpoint hash from live HEAD after commit,
then run cold post-commit CI/status/target checks.

The next evidence-connected hard packet should be
`0x004250B0-0x004256E6` (1,591 bytes, currently `unknown/review`). This is the
central movement/options owner called directly from update state 1. TH10-local
decompilation already shows movement-direction selection, fixed and float Player
position updates, replay/history maintenance, four-option interpolation, and the
option `+0x90` update-callback dispatch. It is directly connected to this packet,
`RebuildPlayerOptions`, and the previously reviewed option callbacks. Resolve its
private live-in register boundary, physical gaps/tables, authored origin, option
layout dependencies, and compiler profile before promoting source or exactness.

Packet balance remains hard-frontier oriented: prior conversations reviewed a
2,494-byte option rebuild, a 482-byte option callback cohort, and the 409-byte
draw body plus hidden adapters. This conversation attacked the materially larger
3,089-byte central Player update body, expanded the reviewed authored-byte
denominator, and rejected an attractive but non-exact fixed-normal profile. The
next packet follows the central movement/options seam rather than selecting a
small floor/timer/helper function for easy counts.
