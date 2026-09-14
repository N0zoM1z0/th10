# TH10 exact reconstruction handoff

## Recovery and authority

The campaign remains active in exact source reconstruction with early faithful
Windows i386 build feedback. Source presence, compilation, canonical exactness,
whole-build closure, runtime validation, and Factory Truth Kernel acceptance are
independent states. Native Ghidra remains provisional semantic evidence with
`exactness_credit=none`.

This conversation started clean on branch `main` at
`275ea521cef4723b4d46b5c7a31355edddbeff17`, with
`origin/main=ae83c3ab72400534834746aeb7613a603a347310`, ahead3/behind0, 0 staged,
0 unstaged, 0 untracked, and 0 conflicts. No interrupted tracked work required
recovery. Ignored private `resources/th10.exe`, `.analysis/`, `.tools/`, `build/`,
`ghidra-project/`, caches, and prior campaign state were preserved. The private
executable was not modified, replaced, relocated, or staged; no `/mnt` search
and no `TH10_TARGET_PATH` override were used.

All prompt-named repository rules, Factory contracts, and Factory guidance paths
were reread through the registered repository runner before editing. No guidance
path was missing. Target, executable VC7.1 SP1 toolchain, tracking, reconstruction
status, public CI, canonical-match graph, and open whole-build graph preflights
passed. Honest `scripts/build.py` returned expected RC=2/open because production
compile flags, TU partition, libraries, resources, and link order remain unknown.

Native `th10-ghidra` operation discovery exposed the bounded read schemas.
Mandatory `check {}` passed exact `target:th10-main` with
`attestation.provider_transport=factory-native-command`; target SHA-256 remains
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Factory transport was intermittently unavailable during recovery, preflight,
diagnostic, and diff-read attempts. After every operation whose execution state
was uncertain, live repository status was reread before continuing. No failed
transport call is treated as a rolled-back transaction or as evidence.

A current Factory accepted-snapshot request was unavailable because another
Factory operation owns the operator path. No accepted, rejected, pending, or
replay count is inferred from that unavailable check.

## Hard packet: Player movement/options owner

The selected packet follows the previously reviewed 3,089-byte Player update
body into its central movement/options callee rather than harvesting a small
helper. Reviewed target scope:

- `0x004250B0-0x004256E6`: 1,591-byte Player movement/options body
- `0x004256E7`: one-byte post-body NOP
- `0x004256E8-0x00425707`: first eight-entry switch table
- `0x00425708-0x00425727`: second eight-entry switch table
- `0x00425728-0x0042572F`: CC padding before separate `0x00425730`.

### Boundary and private ABI

Ghidra recognizes the function at `0x004250B0` with 1,588 reachable bytes.
Target control flow reconciles the three-byte difference exactly: the branch at
`0x0042558B` skips retained LEA alignment bytes
`0x0042558D-0x0042558F`. The authored physical body ends at plain RET
`0x004256E6`; the following NOP, two switch tables, and CC padding are not folded
into the 1,591-byte authored body.

The sole observed caller is `0x00425A8C` inside the previously reviewed central
Player update body. Immediately before that call, target code executes
`mov edi, ebp`, where EBP is the Player owner. The callee consistently addresses
Player storage through live-in EDI and never initializes EDI from ECX or a stack
argument. Therefore Ghidra's provisional fastcall parameter is rejected as an
original machine boundary. The maintained C++ function uses an ordinary
`Player *` parameter for readability, while the ledger records the target ABI as
`internal EDI Player* / RET`. Original source declaration, TU, and optimizer
ownership remain unknown.

The body is classified `authored_game` / Player with high confidence: its target
behavior is bespoke Player input decoding, movement, fixed/draw position update,
mode VM lifecycle, replay history, option callback dispatch/interpolation, and
option VM positioning.

## Direction tables, layout, and behavior

Target input bits map to maintained directions 0-8 in this exact priority:
up-left (`0x50`), down-left (`0x60`), up-right (`0x90`), down-right (`0xA0`),
down (`0x20`), up (`0x10`), left (`0x40`), right (`0x80`), otherwise none.

The two target switch tables independently establish the two movement profiles.
For direction values 1-8, both tables point into physical case-block order
`4, 3, 1, 2, 5, 7, 6, 8`. The first target table at `0x004256E8` belongs to the
nonzero option-mode speed selection and uses Player `+0x3D8` axis /
`+0x3E0` diagonal speed. The second at `0x00425708` belongs to mode zero and
uses `+0x3D4` axis / `+0x3DC` diagonal speed. Those fields are therefore
maintained as mode-specific axis/diagonal speeds, replacing the earlier
unproven X/Y naming.

Additional TH10-local facts represented in maintained source/layout:

- Player `+0x44C/+0x450` store the previous/current horizontal and vertical
  movement speeds used by main-VM movement-script transitions.
- `+0x3F0/+0x3F4` are fixed movement deltas obtained from the selected speed and
  target timer scale; fixed position `+0x3CC/+0x3D0` is clamped to
  `[-18400, 18400] x [3200, 43200]`.
- float draw position `+0x3C0/+0x3C4` is fixed position multiplied by target
  `0.01f`; Z remains the existing `+0x3C8` float.
- Player `+0x329C` is a managed-VM handle used only by this option-mode seam.
  Nonzero mode creates it with observed layer 9, script `0x160`, and runtime
  flag `0x40000000`; mode zero applies delete state 1 and clears the handle.
  While valid, its target position follows Player draw position with the
  observed 224.0f/16.0f offsets.
- mode zero plus nonzero movement maintains the existing 33-pair replay history;
  history row zero is refreshed every call.
- each active 0x98-byte option selects mode-zero offset pair `+0x44` or nonzero
  offset pair `+0x4C`, adds it to Player fixed position to form pair `+0x34`,
  invokes the `+0x90` callback with ECX equal to the option base, then either
  interpolates pair `+0x3C` toward the target when Player `+0x4308 >= 30` or
  snaps on reset flag `+0x8C`.
- primary/secondary option VM ids `+0x68/+0x6C` are positioned from pair `+0x3C`
  after target `0.01f` conversion; the called VM helper supplies the established
  screen offsets.

Maintained `PlayerUpdateMovementAndOptions` is source-present at
`0x004250B0`. Descriptive helper declarations express observed effects without
claiming original symbols or private helper ABIs.

## Compiler and exact feedback

Final `src/Player.cpp` compiles with pinned VC7.1 SP1 build 6030 under fixed
normal:

`/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc`

and the same profile plus `/GL`. `src/ReplayManager.cpp`, which consumes the
refined Player layout, also compiles under both profiles.

The true fixed-normal `/Gy` movement COMDAT is `0x604` = 1,540 bytes. Local
labels at object offsets `0x5C4` and `0x5E4` begin two 32-byte relocated switch
tables, so the object contains 1,476 code bytes plus 64 table bytes. Both object
tables reproduce the target direction mapping and physical lexical block order
`4, 3, 1, 2, 5, 7, 6, 8`.

That strong source-shape signal is not exactness. Target physical ownership is a
1,591-byte EDI-bound body followed by a one-byte NOP and two separate 32-byte
tables, for 1,656 bytes across the reviewed code/table corridor. A caller-window
1,591-byte relocation-aware diagnostic against target `0x004250B0` returns
`mismatch`: 101/1,415 comparable bytes match, 44 object relocations are reported,
and `acceptance_authority=none`. The comparator's requested 1,591-byte window is
not the true 1,540-byte object-owned COMDAT extent. Fixed normal is therefore
rejected as exact physical ownership. Successful `/GL` compilation preserves an
optimizer/LTCG hypothesis; no linked-image LTCG Oracle exists, so exactness stays
unknown. No canonical match unit or exact ledger row was added.

## Ledger delta and adjacent-game discipline

Entry ledger state at `275ea521cef4723b4d46b5c7a31355edddbeff17`:

- candidates 1,239
- origin/boundary pending 1,144
- authored 88 / 25,099 bytes
- exclusions 7
- source-present 61
- canonical exact 0.

Current pre-checkpoint state:

- candidates 1,239
- origin/boundary pending 1,143
- authored 89 / 26,690 bytes
- exclusions 7
- source-present 62
- canonical exact 0.

Delta: pending -1, authored +1 / +1,591 bytes, source-present +1, candidates +0,
exclusions +0, exact +0. The hard packet expands the reviewed authored-byte
denominator rather than hiding optimizer/profile uncertainty.

Committed TH08 Player source was consulted only after TH10-local tables, ABI,
offsets, and behavior were recovered. TH08 HEAD
`a45e99fb1942714e6edded20847e32a654d56f97` was clean and independently has a
related movement/options function plus the same useful VC7 lexical switch-order
idiom. This is source-family corroboration only. TH09 HEAD
`1cf5cf3a356df05aef20412a167ec146b4a54de0` was ahead7 with unrelated tracked
dirty work; no TH09 dirty content was read or used. TH095 HEAD
`c1610c4a10cf44b93e263b30fe0ecbf91f46b42b` was ahead4 with four unrelated
untracked files; no TH095 dirty content was read or used. No adjacent address,
extent, layout, state value, owner, exactness, or completion claim was transferred
into TH10.

## Verification planes

Final pre-checkpoint state remains deliberately separated:

- source presence: yes for maintained `PlayerUpdateMovementAndOptions` at
  `0x004250B0`
- origin/boundary: reviewed 1,591-byte body classified `authored_game`; post-body
  NOP/tables/padding kept outside authored body
- canonical exactness: 0 functions / 0 bytes repository-wide
- whole faithful Windows i386 build: open; `build.py --check` validates the open
  graph and actual `build.py` returns expected RC=2
- runtime validation: not performed because no closed reconstructed product exists
- Factory Truth Kernel acceptance: unavailable because another Factory operation
  owns the operator path; no acceptance count is claimed.

Final focused Player/Replay normal and `/GL` compilation passed. The final
movement diagnostic remained the mismatch described above. `verify-target.py`,
`verify-toolchain.py --check`, target-bound tracking, reconstruction status,
`build-match-unit.py --check`, `build.py --check`, regenerated progress, public
CI, and `git diff --check` all passed. `config/matches.csv`,
`config/match-units.toml`, and `config/build.toml` are unchanged.

## Scratch lifecycle

Session entry `.analysis/` was 87 regular files / 518,355 regular bytes and 780K
allocated, with no file over 64 MiB. This session created and reused
`.analysis/gpt-web/20260914-player-movement/` with a manifest.

Immediately before tracked handoff/checkpoint audit, `.analysis/` is 94 regular
files / 606,414 regular bytes and 880K allocated. This campaign is 7 regular
files / 88,059 bytes, with no file over 64 MiB. Current-session ReplayManager
normal/LTCG header-sanity objects were deleted after final compilation passed;
they were reproducible and unreferenced. Retained artifacts are Player normal and
`/GL` objects, the target-bound mismatch JSON/stderr, whole-build stdout/stderr,
and the manifest. No legacy/unknown/provider/toolchain/target state was deleted.

## Checkpoint and next hard frontier

Final pre-commit gates completed successfully: the complete working diff and
staged diff were audited, only the ten intended tracked packet files are staged,
there is no unstaged or untracked remainder, and `git diff --cached --check`
passes. A fresh native Ghidra `check {}` again passed exact `target:th10-main`
with `factory-native-command` transport. Final Player/Replay normal and `/GL`
probes, target/toolchain/tracking/status, open match/build graphs, honest RC=2
whole-build diagnostic, and public CI also passed. Create one local English
`gpt-web:` checkpoint and never push. Read the checkpoint hash from live HEAD
after commit, then run cold post-commit CI/status/target checks.

The next evidence-connected hard packet should be the Player initialization and
callback-registration owner `0x004247F0-0x00424D8A` (1,435 bytes, currently
`unknown/review`). It is not an easy helper: prior target evidence already ties
it to both `0x00426500` update and `0x00426510` draw registration, while the new
movement/update work exposes additional Player fields, managed-VM state, speed
profiles, timers, and callback-node ownership that initialization may establish.
Reconcile its full boundary, callback-node lifetimes, Player field initialization,
VM/resource owners, and compiler profile before any origin/source promotion.

Packet balance remains hard-frontier oriented: recent checkpoints reviewed a
2,494-byte option rebuild, a 3,089-byte central update body, and now this
1,591-byte movement/options owner. The next proposed 1,435-byte initializer
continues the central Player ownership seam rather than selecting the 67-, 174-,
or 229-byte adjacent helpers for easy function-count progress.
