# TH10 exact reconstruction handoff

## Recovery and authority

Phase remains exact source reconstruction with early faithful Windows i386 build
feedback. Source presence, compilation, canonical exactness, whole-build
closure, runtime validation, and Factory Truth Kernel acceptance remain separate
states. Native Ghidra has `exactness_credit=none`.

This session started clean on branch `main` at
`a70e17cca7e36a34ca18eedfb9ed42297af09194`, tracking
`origin/main=ae83c3ab72400534834746aeb7613a603a347310`, ahead1/behind0, with 0 staged,
0 unstaged, 0 untracked, and 0 conflicts. No interrupted dirty work required
recovery. Private `resources/th10.exe`, legacy `.analysis/`, `.tools/`, `build/`,
`ghidra-project/`, caches, and other ignored state were preserved. The private
target was not modified, replaced, relocated, or staged; no `/mnt` search and no
`TH10_TARGET_PATH` override were used.

All required repository and Factory guidance was reread through the registered
repository runner before editing. Target, executable toolchain, tracking,
reconstruction-status, public-CI, canonical-match-graph, and open-build-graph
preflights passed. Honest `scripts/build.py` returned expected RC=2/open because
production compile flags, translation-unit partition, libraries, resources, and
link order remain unknown. Fresh native Ghidra operation discovery and mandatory
`check {}` passed exact `target:th10-main` with
`attestation.provider_transport=factory-native-command` and private-target SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

A later Factory Truth snapshot refresh failed with a network connection error.
One pre-handoff repository-runner request also hit a network connection error;
live repository status was immediately reread and showed the expected nine
unstaged packet files with no extra/untracked/conflicted paths, then the complete
diff audit was rerun successfully. No acceptance or filesystem result is inferred
from either unavailable operation.

## Hard packet: Player update/draw callback seam

The selected packet followed the previous option-callback cohort into the
central Player callback registration/draw seam rather than harvesting a small
leaf. Primary reviewed target extents are:

- `0x00426360-0x004264F8`: 409-byte EAX-bound Player draw body, existing
  `unknown/review` candidate
- `0x00426500-0x00426506`: newly tracked 7-byte Player update callback adapter
- `0x00426510-0x00426516`: newly tracked 7-byte Player draw callback adapter.

Both seven-byte entries were absent from the Ghidra candidate denominator and
are CC-delimited. `0x00426500` is preceded by `0x004264F9-0x004264FF` padding and
followed by `0x00426507-0x0042650F`; `0x00426510` is followed by
`0x00426517-0x0042651F` before the separate `0x00426520` candidate.

### Registration and callback-manager lanes

Player initialization at `0x004247F0` proves two independent callback
registrations. At `0x00424856`, it supplies `0x00426500` to callback-node
allocator `0x00449ED0`, stores the Player owner at node `+0x20`, inserts the node
through `0x00449AE0`, and retains it at Player `+0x08`. The entry pushes its
live-in ECX Player pointer, calls the large Player owner `0x00425730`, and
returns its result.

At `0x0042487E`, the same initializer supplies `0x00426510`, stores the Player
owner at node `+0x20`, inserts through `0x00449B70`, and retains the node at
Player `+0x0C`. The raw entry is exactly `mov eax, ecx; jmp 0x00426360`.

The callback manager independently establishes two list families. `0x00449C00`
runs the list rooted at manager `+0x18`, paired with insertion `0x00449AE0`;
`0x00449D40` runs the list rooted at manager `+0x3C`, paired with insertion
`0x00449B70`. Main frame owner `0x00439390` runs `0x00449C00` before the render
sequence and runs `0x00449D40` inside the render path after graphics preparation
and before later renderer/end/present virtual calls. Maintained names
`PlayerUpdateCallback` and `PlayerDrawCallback` therefore describe TH10-local
roles; they are not original-symbol claims.

Both seven-byte candidates remain `origin=unknown`, `disposition=review`, and
`confidence=unknown`. Source-written adapters versus compiler/LTCG-generated
callback wrappers remain unresolved.

### Draw body and option +0x94

The registered draw adapter transfers to `0x00426360` with the Player owner in
EAX. The 409-byte body checks Player `+0x458`; when it is not 2, the body writes
an embedded draw view beginning at Player `+0x14`: view-relative positions
`+0x340/+0x344/+0x348` receive Player floats `+0x3C0/+0x3C4/+0x3C8`, with
`224.0f` added to X and `16.0f` added to Y, then the target draw-VM owner is
called.

The body next iterates four 0x98-byte option records starting at Player
`+0x32A0`. It begins from Player `+0x3334` (option 0 `+0x94`), loads each
non-null function pointer, forms ECX as `slot - 0x94` (the option record base),
calls the hook, and advances by 0x98. This proves the machine boundary of option
`+0x94` independently from the prior `+0x90` update callback path.

A complete target `.text` Capstone displacement scan found no Player-local write
to option `+0x94`. Other `[reg+0x94]` writes belong to unrelated owners and have
no observed static connection to this Player registration/update seam. Therefore
the maintained field is now named `drawCallback` from the TH10 draw-chain
consumer role, while the actual producer/assignment remains unknown. Do not add
a producer to `RebuildPlayerOptions` without new target evidence.

The remaining auxiliary draw branch is also represented naturally. Target
constants were directly decoded as `225.0f`, `17.0f`, `24.0f`, `2.0f`, `1.0f`,
and `0.29230770468711853f`. `0x00427C50` takes a float, converts it to double,
calls the target floor implementation at `0x00452FF0`, and returns through x87;
it is called twice by this body. The target then draws the derived four-float
rectangle twice with colors `0x80000000` and `0xFFFFFFFF`, subtracting 1.0f from
all four coordinates before the second pass. The gate objects/global identifiers
remain neutral maintained names because their original ownership and semantics
are not established.

`src/Player.hpp` now exposes only a maintained partial `PlayerDrawVmView` through
the observed relative `+0x35C` flags field. Its original VM type and physical
object extent explicitly remain unknown. Player float coordinates `+0x3C0`,
runtime state `+0x458`, and option `drawCallback +0x94` receive offset assertions.

Maintained `PlayerDrawCallback` in `src/Player.cpp` models the registered draw
entry and connected behavior. Source presence is recorded at `0x00426510` only.
The separate physical body at `0x00426360` remains without standalone source
ownership, and `0x00426500` has no maintained source mapping in this packet.

## Compiler and Oracle feedback

Pinned VC7.1 SP1 build 6030 compiles final `src/Player.cpp` with fixed normal

`/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc`

and the same profile plus `/GL`. `src/ReplayManager.cpp`, which consumes the
changed Player layout, also compiles under both profiles.

The true fixed-normal `/Gy` `PlayerDrawCallback` COMDAT is exactly `0x1A0` = 416
bytes. That is numerically equal to the target's 409-byte EAX-bound body plus its
7-byte registered ECX adapter. The normal object also naturally retains the same
runtime-state gate, four-option `+0x94` callback loop, target floating constants,
gate ordering, and floor-driven auxiliary rectangle structure.

This is not exactness evidence. The target code chunks are non-contiguous and
have different optimizer/private-ABI ownership. A relocation-aware 409-byte
body diagnostic against target `0x00426360` is `mismatch`: 69/317 comparable
bytes match, 23 object relocations are reported, and
`acceptance_authority=none`. Successful `/GL` compilation preserves an
optimizer/LTCG split hypothesis, but the standalone COFF Oracle cannot assign a
linked-image LTCG extent. No canonical match unit or exact ledger row was added.

## Denominator and adjacent-game discipline

Entry ledger state at `a70e17cca7e36a34ca18eedfb9ed42297af09194`:

- candidates 1,237
- origin/boundary pending 1,143
- authored 87 / 22,010 bytes
- exclusions 7
- source-present 59
- canonical exact 0.

Current packet state before checkpoint:

- candidates 1,239
- origin/boundary pending 1,145
- authored 87 / 22,010 bytes
- exclusions 7
- source-present 60
- canonical exact 0.

Delta: +2 denominator candidates, +2 pending, +0 authored, +0 exclusions, +1
source-present, +0 exact. Both denominator additions are the missed seven-byte
callback adapters. The hard packet does not improve authored/exact percentage by
reclassifying uncertain physical chunks.

Adjacent committed content was consulted only after TH10-local evidence existed:

- TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97`, clean. Its committed exact
  Player corpus independently has separate option update/render callback slots;
  this is corroboration only.
- TH09 HEAD `3e1a9c86318652f13e6fed86d45b6cbeb8479c3a`, clean, branch ahead6.
- TH095 HEAD `fb9d07b0dd9ad069a67ac18f367497bbb536b665`, branch ahead1, with unrelated
  modified `src/Main.cpp` plus four unrelated untracked files. Only `HEAD:`
  committed content was queried; those dirty files were not read or used.

No adjacent address, extent, layout, owner, exactness, or completion claim was
transferred into TH10.

## Verification planes and unavailable acceptance

Final pre-checkpoint verification currently separates the planes as follows:

- source presence: maintained source at registered draw entry `0x00426510`;
  standalone ownership of body `0x00426360` and update adapter `0x00426500`
  remains unknown
- canonical exactness: 0 functions / 0 bytes repository-wide
- whole faithful Windows i386 build: open; `build.py --check` passes the honest
  open graph and actual `build.py` returns expected RC=2
- runtime validation: not performed because there is no closed reconstructed
  product
- Factory Truth Kernel acceptance: current refresh unavailable because the
  snapshot request hit a Factory network connection error; no new accepted,
  rejected, pending, or replay fact is claimed.

Final focused normal and `/GL` Player/Replay compilation passed. Target identity,
`verify-toolchain.py --check`, tracking, reconstruction status, regenerated
progress artifacts, public CI, and `git diff --check` all passed. The canonical
normal-COFF match graph still contains 0 configured units.

## Scratch discipline

Session entry `.analysis/` was 69 regular files / 245,708 regular bytes and 476K
allocated, with no file over 64 MiB. This campaign reuses
`.analysis/gpt-web/20260914-player-postupdate/` and retains the manifest, final
normal/LTCG Player and Replay probes, relocation-aware draw mismatch JSON,
comparator stderr, and honest whole-build stdout/stderr.

Immediately before handoff, `.analysis/` is 78 regular files / 363,757 regular
bytes and 612K allocated; this campaign is 9 regular files / 118,049 bytes. No
current campaign artifact exceeds 64 MiB. Nothing from legacy/shared/provider/
toolchain/target analysis state was deleted.

## Checkpoint plan and next hard frontier

Audit the complete staged diff after adding this handoff, run a fresh native
Ghidra `check {}`, stage only the intended packet files, create one local English
`gpt-web:` checkpoint, never push, then run cold post-commit CI/status/target
checks. The checkpoint hash must be read from live HEAD after commit; this file
does not self-claim a future hash.

The next evidence-connected hard packet should be the directly registered Player
update owner `0x00425730-0x00426340` (3,089 bytes) together with its
`0x00426500` adapter context. It is the first-list counterpart to this session's
draw seam, contains the central movement/state/option update logic, and directly
connects to previously reconstructed `RebuildPlayerOptions` and option `+0x90`
callbacks. This is a materially larger central owner, not the easiest remaining
function. Reconcile its private machine ABI, physical tails/alignment, state and
input owners, option update dispatch, and compiler profile before any authorship
or source mapping promotion.

Packet-selection balance remains hard-frontier oriented: the previous packet
reviewed a 482-byte option callback cohort after a 2,494-byte rebuild owner; this
session followed it into a 409-byte central draw body, expanded the denominator
with two hidden registration adapters, and resolved the update/draw chain seam.
The next proposed packet escalates to the connected 3,089-byte Player update
owner rather than harvesting small exact-looking helpers such as the 21-byte
floor wrapper.
