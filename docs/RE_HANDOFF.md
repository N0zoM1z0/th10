# TH10 exact reconstruction handoff

## Recovery, authority, and checkpoint scope

Phase remains exact source reconstruction with early faithful Windows i386 build
feedback. Source presence, compiler feasibility, canonical exactness,
whole-build closure, runtime validation, and Factory Truth Kernel acceptance are
separate states. Native Ghidra observations have `exactness_credit=none`.

This session started clean on branch `main` at
`ae83c3ab72400534834746aeb7613a603a347310`, tracking `origin/main` at the same
commit: 0 staged, 0 unstaged, 0 untracked, and 0 conflicts. No interrupted dirty
work required recovery. Private `resources/th10.exe`, legacy `.analysis/`,
`.tools/`, `build/`, `ghidra-project/`, caches, and other ignored state were
preserved. The private target was not modified, replaced, relocated, or staged;
no `/mnt` search and no `TH10_TARGET_PATH` override were used.

All required repository and Factory guidance was reread through the registered
repository runner. `verify-target.py`, executable toolchain preflight,
`validate-tracking.py --require-target`, reconstruction status, and public CI
passed before editing. Native Ghidra operation discovery followed by `check {}`
passed for exact `target:th10-main` with
`attestation.provider_transport=factory-native-command`. The target SHA-256 is
`2f14760b...9040` (full identity remains in the target lock/preflight output).

The selected hard packet is the Player `+0x90` option-callback cohort connected
to the previously reviewed `RebuildPlayerOptions` owner. It covers 482 physical
candidate code bytes rather than harvesting unrelated small leaves:

- `0x00427950-0x0042795B`: 12-byte callback entry/wrapper
- `0x00427960-0x00427AC6`: 359-byte adjacent body
- `0x00427AD0-0x00427ADB`: 12-byte callback entry/wrapper
- `0x00427AE0-0x00427B42`: 99-byte adjacent body.

## Boundary, callback slots, and machine ABI

`0x00427950` preserves ESI, moves live-in ECX to ESI, calls `0x00427960`, forces
EAX to zero, restores ESI, and returns. `0x0042795C-0x0042795F` is `CC` padding.
The physical body `0x00427960-0x00427AC6` is 359 bytes. Ghidra reaches 353 bytes
because `0x00427A0A-0x00427A0F` is a retained unreachable six-byte LEA alignment
sled. Padding follows through `0x00427ACF`.

`0x00427AD0` preserves EDI, moves live-in ECX to EDI, calls `0x00427AE0`, forces
EAX to zero, restores EDI, and returns. `0x00427ADC-0x00427ADF` is `CC` padding.
The adjacent body is exactly `0x00427AE0-0x00427B42` = 99 bytes, followed by
`CC` padding through `0x00427B4F`; `0x00427B50` is a separate candidate.

The Player update path independently establishes the callback machine boundary.
At `0x004255CC` it forms ECX as the option record base; `0x00425600` loads the
function pointer at option `+0x90`; `0x00425607` calls it. A different loop at
`0x004263B3-0x004263D5` loads and calls option `+0x94`, again with ECX equal to
the 0x98-byte record base. Therefore `+0x90` and `+0x94` are distinct callback-
like slots. The rebuild writes `0x00427950`/`0x00427AD0` into `+0x90`; `+0x94`
remains unidentified. The maintained `__fastcall` declaration is a spelling of
the observed ECX-bound machine ABI, not proof of the original source declaration.

## Recovered behavior and maintained source

`PlayerOptionRuntime` now exposes observed `previousMode` at option `+0x84`,
keeps `optionIndex` at `+0x88`, `resetFlag` at `+0x8C`, types the `+0x90`
callback slot, leaves `+0x94` unknown, and retains the 0x98-byte size assertions.

Maintained `PlayerOptionTrailCallback` models the `0x00427950` entry behavior.
Its adjacent body uses option `+0x88` to address eight history samples per option.
In mode zero it derives replay pair `+0x44` from the endpoint and current player
position. In nonzero mode it reconstructs seven intermediate history samples;
the target constant at `0x00470B90` is directly verified as `0.125f`. It then
refreshes pair `+0x34` and stores the current mode at option `+0x84`.

Maintained `PlayerOptionSpecialCallback` models the `0x00427AD0` entry behavior.
On option-mode transitions its adjacent body applies managed-VM delete state 6
or 3 to primary VM id `+0x68`, copies replay pair `+0x3C` to `+0x4C` when
returning to mode zero or `+0x4C` to `+0x34` when entering nonzero mode, and
updates `+0x84`.

Source presence is recorded only at callback entries `0x00427950` and
`0x00427AD0`. The two adjacent physical bodies remain unmapped as standalone
source functions. All four physical candidates remain `origin=unknown`,
`disposition=review`, `confidence=unknown`. In particular, source-written
wrapper versus compiler/LTCG adapter and compiler-owned outlined body remain
unresolved; no authorship promotion was made.

## Compiler and Oracle feedback

Pinned VC7.1 SP1 build 6030 compiles final `src/Player.cpp` with fixed normal
`/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc` and the same profile plus `/GL`.
`src/ReplayManager.cpp`, which includes the changed Player layout, also compiles
under both profiles.

Target-shaped direct option-index expressions produce a true 358-byte normal
`/Gy` `PlayerOptionTrailCallback` COMDAT versus the 359-byte adjacent target
body. A relocation-aware 358-byte diagnostic against target `0x00427960`
returns `mismatch`: 89/338 comparable bytes match, with 20 relocation-owned
bytes ignored and `acceptance_authority=none`.

The normal `PlayerOptionSpecialCallback` COMDAT is 111 bytes. That is only a
shape clue: the target wrapper and adjacent body contain 12+99 code bytes but
are separated by four `CC` bytes. A 99-byte diagnostic against target
`0x00427AE0` returns `mismatch`: 7/87 comparable bytes match, with 12
relocation-owned bytes ignored and `acceptance_authority=none`.

These diagnostics reject the fixed standalone normal objects as exact physical
matches. Successful `/GL` compilation preserves an optimizer/LTCG ownership
hypothesis, but the standalone COFF Oracle cannot assign linked-image LTCG
extent or exactness. No canonical match unit or exact ledger row was added.

## Adjacent-game hypothesis provenance

Only committed adjacent content was used as hypothesis material after TH10-local
evidence existed:

- TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97`, clean.
- TH09 HEAD `3e1a9c86318652f13e6fed86d45b6cbeb8479c3a`, clean.
- TH095 HEAD `bd97b9e08be1e4d4d485857284757b483512915a`. Four unrelated untracked
  files were present and were not read or used.

One read-only TH095 query requested a nonexistent committed `src/Player.cpp` and
returned nonzero. TH10 status was reread immediately and remained unchanged;
no files were left by that failed query. No adjacent address, extent, layout,
owner, exactness, or completion claim was transferred into TH10.

## Ledger and verification planes

Starting ledger state at `ae83c3ab72400534834746aeb7613a603a347310`:
1,237 candidates, 1,143 pending, 87 authored, 7 exclusions, 57 source-present,
and 0 canonical exact.

Current packet state before checkpoint:
1,237 candidates, 1,143 pending, 87 authored, 7 exclusions, 59 source-present,
and 0 canonical exact. The only metric delta is +2 source-present callback
entries. The authored denominator does not change because all four packet
candidates remain `unknown/review`.

Verification-plane state is intentionally separate:

- source presence: yes for maintained callback entries `0x00427950` and
  `0x00427AD0`; adjacent body ownership remains unknown
- canonical exactness: 0 functions / 0 bytes for this packet and repository
- whole faithful Windows i386 build: open; build graph still has 0 canonical
  translation units/libraries/resources and incomplete production flags/order
- runtime validation: not performed; no whole product exists
- Factory Truth Kernel acceptance: accepted snapshot sequence 0,
  `accepted_count=0`, no pending submission, and no pending replay.

`build-match-unit.py --check` validates the empty canonical match graph.
`build.py --check` validates the intentionally open build configuration. Honest
`build.py` returns expected RC=2/open because compile flags, source TUs,
libraries, resources, and link order remain unknown; it does not create a
product.

## Scratch and checkpoint discipline

Session entry `.analysis/` allocated size was 364K; the preceding checkpoint's
regular-file total was 147,806 bytes. Current campaign scratch is
`.analysis/gpt-web/20260914-player-option-callbacks/`. It contains only this
session's small manifest, normal/LTCG compile probes, relocation-aware mismatch
diagnostics, and whole-build stdout/stderr. No legacy/shared/provider/toolchain/
target state was removed. Final inventory and disposition are recorded after the
checkpoint in the ignored campaign manifest and operator-facing report.

Final pre-commit target, toolchain, tracking, status, and `git diff --check`
passed. The first public-CI rerun correctly failed because generated
`docs/PROGRESS.md` still reported 57 source-present mappings; repository
`progress.py` regenerated it to 59 and the complete public CI then passed.
A fresh native Ghidra `check {}` also passed the exact target and
`factory-native-command` transport attestation. The complete staged diff was
audited with no unstaged tracked remainder. The checkpoint is local only and
must never be pushed by this workflow.

## Remaining unknowns and next hard packet

Still unknown are the original callback declarations/calling convention, whether
the two 12-byte entries are source-written or compiler/LTCG adapters, physical
ownership/origin of the 359-byte and 99-byte bodies, production per-function
optimizer/LTCG profile and TU, the producers/targets of option `+0x94`, and the
whole product's TU/library/resource/link-order closure.

The next evidence-connected hard packet should start at the central Player
candidate `0x00426360-0x004264F8` (409 bytes, currently `unknown/review`) and
trace its four-record `+0x94` callback dispatch back to the slot producers and
actual callback targets. This directly continues the option ABI/layout seam and
attacks a central dispatcher/owner rather than selecting the easiest remaining
function. Do not absorb callback targets or neighboring candidates until target
xrefs/data ownership establish the boundary.

Packet balance remains hard-frontier oriented: this session reviewed a
482-byte multi-entry ABI/layout/control-flow cohort connected to the prior
2,494-byte rebuild owner, corrected a plausible but false `+0x90`-versus-`+0x94`
offset interpretation, recovered behavior and source shape, and retained
origin/exactness unknown where the target-bound Oracle does not decide them.
