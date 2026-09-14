# TH10 Reconstruction Handoff

## Checkpoint scope

This checkpoint continues the exact-reconstruction campaign from starting HEAD
`4324f63ab902a124ffae87a28780f2dc2785c4ec` on `main`. The tracked tree was
clean at entry. Recovery found no interrupted tracked work, no unrelated dirty
work, and no unknown untracked paths. Existing ignored `.analysis/`, build,
toolchain, Ghidra/cache state, and the operator-supplied private target were
preserved. The target was never modified, relocated, staged, or committed.

The bounded hard packet is the Player damage/collision owner at
`0x00428630-0x00428AC1` (1,170 physical bytes). It was selected because it is a
central consumer of the previously reconstructed 128-shot runtime, also consumes
the first 32 Player collision/effect rows, crosses shot callback/VM/data-owner
boundaries, and is called by an unresolved 2,412-byte hostile runtime owner. It
was not selected for size or expected ease of exactness.

## Recovery and authority preflights

The Factory repository/target/provider binding was re-established before edits.
All required repository and Factory guidance files were read through the
repository runner. Entry target verification passed for
`target:th10-main`: SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`,
MD5 `7dc488d82c81dd4aee4ba098b8804d83`, PE i386 image base `0x00400000`,
entry `0x004537DC`, Rich dominant build 6030. The pinned VC7.1 SP1 candidate
passed the executable normal-COFF, LTCG, resource, and PE32 smoke preflight.
Tracking/status/public CI, the zero-unit match graph, and the explicitly open
whole-build graph passed. Native Ghidra discovery was followed by `check {}`;
the mandatory and final checks both returned `attestation.status=passed`,
`target_identity_id=target:th10-main`, and
`provider_transport=factory-native-command`.

Several Factory calls encountered transport-level network errors. Every
uncertain repository/provider call was followed by live-status recovery before
continuing. No target or provider identity mismatch was observed.

## `0x00428630-0x00428AC1` physical boundary and ABI

Fresh native Ghidra reports one function with `body_min=0x00428630`,
`body_max=0x00428AC1`, and 1,170 body addresses, so this owner has no internal
unreachable-gap discrepancy. Its sole direct caller is the 2,412-byte
`0x0040DC80-0x0040E5EB` owner. At `0x0040E128-0x0040E13C` the caller pushes
three values in order: target size, target position, and the global Player
pointer; it clears ECX immediately before the call. The callee conditionally
clears the live-in ECX pointee and ends in `RET 12`.

The target machine boundary is therefore recorded as
`private ECX optional int* + 3 stack (Player*, position*, size*) / RET 12`.
Maintained source uses the natural logical spelling
`Player::CalculateDamageToTarget(const PlayerFloat3 *, const PlayerFloat3 *, int *)`.
This source spelling is not a claim that the target used ordinary source-level
`thiscall`; the target optimizer/private ABI has rotated the four logical
values so the Player owner is stack-bound and the optional output pointer is in
ECX. The original identifier and translation unit remain unknown.

## Target-observed behavior and layout

The owner returns zero when the Player update timer did not advance. Otherwise
it builds a target AABB from half-extents, optionally clears the ECX-bound
output integer, and iterates all 128 `0x5C` Player shot rows. Shot collision
uses descriptor hitbox extents, distinct type-3/non-type-3 vertical gates, and
an ECX/EDX Player/shot collision callback at descriptor `+0x30` with target
position on the stack. A nonzero callback return suppresses the hit.

TH10-local collision/caller evidence closes descriptor `+0x02` as a signed
damage value. Hit processing marks the target-observed collision handoff fields,
uses the dedicated managed-VM state-2/resolve/pending helpers, accumulates type-3
damage only through the target timer-interval helper, transitions non-type-3
shots to their hit VM/state, and scales the observed motion field by `0.125f`.
Type 2 spawns one Player collision/effect row with the target-observed
parameters.

After an auxiliary target-position damage helper, the owner iterates the first
32 of the constructor-proven 33 `0x6C` Player rows. Independent TH10 spawn,
update, motion-helper, and collision evidence now establishes a maintained row
view with radius/radius-delta at `+0x00/+0x04`, angle/angle-delta at
`+0x08/+0x0C`, extents at `+0x10/+0x14`, Float3 position at `+0x18`, Float3
velocity at `+0x24`, timer at `+0x44`, damage/hit-accumulator/hit-cap/
collision-interval at `+0x58/+0x5C/+0x60/+0x64`, and active/mode flags at
`+0x68`. Circle, axis-aligned rectangle, and rotated rectangle collision paths
are represented directly. The 33rd row remains structurally present because
the Player constructor proves 33 rows, but no first-32 runtime semantics are
invented for it.

When aggregate damage is nonzero, the body updates the global at `0x00474C44`
and caps it at 999,999,999. A separate TH10 writer at `0x00409C00` independently
shows the same capped gameplay accumulator, so maintained source uses the
descriptive name `g_GameScore`; that is not an original-symbol claim.

Committed TH08 source was consulted only after the TH10-local control flow and
layout were recovered. TH08 HEAD
`a45e99fb1942714e6edded20847e32a654d56f97` was clean and contains the broad
128-shot/collision-region damage source family. No TH08 address, extent,
layout, state value, origin, exactness, or completion fact was transferred.

## Denominator expansion after the owner

Direct inspection of the verified target corridor closes the post-body physical
ownership and finds two Ghidra-missed code bodies:

- `0x00428AC2-0x00428ACF`: 14 bytes of `CC` padding.
- `0x00428AD0-0x00428B04`: 53-byte body ending in `RET 4`.
- `0x00428B05-0x00428B0F`: 11 bytes of `CC` padding.
- `0x00428B10-0x00428C19`: 266-byte body ending in plain `RET`.
- `0x00428C1A-0x00428C1F`: 6 bytes of `CC` padding before the existing
  `0x00428C20` candidate.

Native Ghidra reports no containing function and no xrefs for either raw entry.
Both are added only as `unknown/review` Player denominator candidates. Their
indirect-callback/source ownership, exact logical ABI, origin, and exactness are
not guessed and they receive no source-present/authored credit.

## Source, ledger, and compiler feedback

Tracked source changes are confined to `src/Player.cpp` and `src/Player.hpp`.
The maintained Player collision/effect row and shot collision callback views are
refined from TH10 target evidence, the descriptor damage field is named, and
`Player::CalculateDamageToTarget` is implemented in natural C++.

Ledger changes are in `config/functions.csv`, `config/function-origins.csv`,
`config/implemented.csv`, and `config/reccmp-functions.csv`. The 1,170-byte
owner is promoted to authored/source-present with `authored_game` origin. The
two newly discovered raw bodies remain `unknown/review`. No row was added to
`config/matches.csv`, no canonical unit was added to `config/match-units.toml`,
and `config/build.toml` remains explicitly open. Durable knowledge is recorded
as `PLAYER-018`, `PLAYER-019`, and `TOOLCHAIN-023`; progress artifacts were
regenerated.

Final pinned VC7.1 SP1 build-6030 probes use
`/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc` and the same source with `/GL`.
`Player.cpp` compiles under both profiles, and `ReplayManager.cpp` also compiles
under both profiles as a shared-header/layout sanity check. The true fixed-normal
section for
`?CalculateDamageToTarget@Player@@QAEHPBUPlayerFloat3@@0PAH@Z` is
`0x46C = 1,132` bytes versus the 1,170-byte target owner. The final target-bound
relocation-aware diagnostic is `mismatch` at 72/1,078 comparable bytes with 23
object relocations and `acceptance_authority=none`. Successful `/GL`
compilation preserves the LTCG/interprocedural/private-ABI hypothesis; there is
no target-bound linked-image Oracle for it. Exactness remains unknown and no
canonical exact credit is claimed.

Final reconstruction status before checkpoint is 1,249 candidates, 1,145
origin/boundary pending, 96 authored, 8 excluded, 69 source-present, 0 exact,
and 31,082 authored bytes. Relative to entry this packet adds two denominator
candidates, promotes one 1,170-byte owner to authored/source-present, and adds no
exact function or exact byte.

## Verification planes

The following final checks pass independently: target verification; toolchain
declaration check (with a real executable `--execute` smoke already passed at
entry); tracking validation; reconstruction-status report; public CI;
zero-unit normal-COFF match graph; explicitly open build graph; generated
progress check; whitespace/diff check; final Player and Replay normal/LTCG
focused compiles; and final Ghidra target/provider attestation.

The honest whole-build diagnostic returns RC=2 and states that compile flags,
translation-unit partition, libraries, production resources, and link order
remain unknown. Whole-product Windows i386 build closure is therefore open.
Runtime validation was not performed.

Factory Truth acceptance is a separate plane. Two read-only accepted-snapshot
attempts were rejected by Factory with
`ReplayError: another factory operation owns <operator-path>`. Therefore the
current accepted snapshot is unavailable for this checkpoint. No Truth
submission or replay was performed, and no acceptance state is inferred from
the unavailable read.

## Scratch and artifact discipline

Entry `.analysis/` inventory was 120 regular files / 971,344 regular bytes /
1,304K allocated, with zero files over 64 MiB. This campaign reuses
`.analysis/gpt-web/20260914-player-shot-collision/`. Final retained artifacts are
the Player normal/LTCG probes, relocation-aware diagnostic JSON/stderr, honest
whole-build stdout/stderr, and the campaign manifest. Current-session
ReplayManager normal/LTCG header-sanity objects were deleted after their final
successful compile. No legacy/unknown provider, toolchain, target, Ghidra, Wine,
or earlier campaign state was removed.

## Next hard frontier

Continue with `0x0040DC80-0x0040E5EB`, 2,412 bytes, currently
`unknown/review`. Fresh Ghidra confirms exactly 2,412 body addresses over that
ledger extent. It is the sole caller of the newly recovered Player damage owner
and consumes its return value as a hostile health-like decrement, so it is an
evidence-connected central runtime owner rather than an easy adjacent leaf.
Reconcile its physical entry/exit, owner ABI, hostile-object layout, collision/
health state transitions, and compiler profile before any origin/source
promotion. The nearby 53/125/266-byte Player helpers remain useful context but
should not replace this regular hard-frontier attempt merely to increase the
function count.

This handoff is a continuation checkpoint only. TH10 exact reconstruction is
not complete; whole-build closure and runtime validation are not complete; and
Factory acceptance is not claimed.
