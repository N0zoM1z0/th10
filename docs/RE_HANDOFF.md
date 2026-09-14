# TH10 exact reconstruction handoff

## Recovery and authority

This campaign remains in exact source reconstruction with early faithful Windows
i386 build feedback. Source presence, compilation, canonical exactness,
whole-build closure, runtime validation, and Factory Truth Kernel acceptance are
independent states. Native Ghidra is provisional semantic evidence with
`exactness_credit=none`.

This conversation started clean on branch `main` at
`6a40c3a69c378132d48fe7e63997334ddc7412cc`, with
`origin/main=ae83c3ab72400534834746aeb7613a603a347310`, ahead4/behind0, 0 staged,
0 unstaged, 0 untracked, and 0 conflicts. No interrupted tracked work required
recovery. Ignored private `resources/th10.exe`, `.analysis/`, `.tools/`, `build/`,
`ghidra-project/`, caches, and prior campaign state were preserved. The private
executable was not modified, replaced, relocated, or staged; no `/mnt` search
and no `TH10_TARGET_PATH` override were used.

All prompt-named repository rules, Factory contracts, and Factory guidance paths
were reread through the registered repository runner before editing. No guidance
path was missing. Target, executable VC7.1 SP1 build-6030 toolchain, tracking,
reconstruction status, public CI, canonical-match graph, and open whole-build
graph preflights passed. The executable toolchain smoke covered normal COFF,
LTCG, resource, and PE32 paths under the pinned headless-Wine wrapper. Honest
`scripts/build.py` remains expected RC=2/open because production compile flags,
TU partition, libraries, resources, and link order are not established.

Native `th10-ghidra` operation discovery was repeated before target-dependent
work. Mandatory `check {}` passed exact `target:th10-main` with
`attestation.provider_transport=factory-native-command`; target SHA-256 remains
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
A fresh pre-checkpoint check passed the same binding. Intermittent Factory
transport failures occurred during the first Ghidra check, compile, and ledger
calls. Every uncertain operation was followed by live repository/artifact state
inspection before retrying; no transport failure was treated as rollback or as
evidence. One exploratory host-C++ layout probe was discarded because the host
compiler is neither VC7 nor i386; all layout acceptance below comes from TH10
target evidence plus pinned VC7 compilation.

The current Factory Truth Kernel snapshot is sequence 0 with `accepted_count=0`
and no accepted facts/evidence. No acceptance submission or replay was performed.
This does not accept the current checkpoint.

## Hard packet: Player initialization owner

The selected hard packet follows the previously reviewed Player update/movement
seam into its allocation/initialization owner rather than harvesting a small
helper:

- `0x004247F0-0x00424D8A`: reviewed 1,435-byte Player initialization body
- `0x00424939-0x0042493F`: seven-byte unreachable `lea esp,[esp]` alignment sled
  retained inside that physical body
- `0x00424D8B-0x00424D8F`: CC padding outside the body
- `0x00424D90`: independent next candidate, not folded into this packet.

Ghidra reports 1,428 reachable bytes for `0x004247F0`; the seven-byte sled exactly
reconciles the reviewed 1,435-byte physical extent. The body ends in RET at
`0x00424D8A` and the five CC bytes close the boundary before `0x00424D90`.

### Private ABI and Player object extent

The sole observed caller is the Player factory at `0x00425020`. It allocates
exactly `0x4478` bytes, calls the adjacent constructor/reset owner `0x004246C0`,
moves the returned Player into EBX, and immediately calls `0x004247F0`.
`0x004247F0` never establishes EBX and consistently addresses Player through it,
so the ledger records the target machine boundary as `internal EBX Player* / RET`.
The maintained C++ `Player *` parameter is a readable spelling of that behavior,
not an original declaration or ABI claim.

The adjacent `0x004246C0` constructor/reset performs `rep stosd` for exactly
`0x111E` dwords from the newly allocated object and returns/publishes that same
object. Together with the factory allocation, TH10-local evidence therefore
establishes a complete Player physical extent of `0x4478` bytes. The maintained
layout now has a pinned-VC7 `sizeof(Player)==0x4478` assertion.

The initializer passes Player `+0x14` as the embedded draw-VM base. Its observed
reset owner `0x00401DE0` clears exactly `0xEB` dwords while preserving selected
VM fields, establishing `0x3AC` bytes of embedded VM storage through Player
`+0x3BF`; Player draw position begins immediately at `+0x3C0`. The maintained
`PlayerDrawVmView` is correspondingly closed to `0x3AC` bytes without claiming
original VM field names.

### Initializer behavior and lifetime seam

The reviewed initializer source represents these TH10-local effects:

- selects `pl00.anm` or `pl01.anm` from character state and loads it through the
  observed animation owner; null resource takes the target error path and returns
  -1;
- consumes an already shared option-data object if present, otherwise selects one
  of `pl00a.sht`, `pl00b.sht`, `pl00c.sht`, `pl01a.sht`, `pl01b.sht`, or
  `pl01c.sht` from character/shot state and invokes the observed data loader;
- allocates two 0x24-byte callback nodes, stores their callback at `+0x08` and
  Player owner at `+0x20`, clears node flag bit 1, registers the reviewed
  `0x00426500` update callback at priority `0x10` and `0x00426510` draw callback
  at priority `0x16`, then stores the node handles at Player `+0x08/+0x0C`;
- initializes the embedded draw VM from the loaded resource;
- initializes float/fixed starting position, the four mode-specific axis/diagonal
  speeds, all 33 replay-position history rows, three timer-shaped records, option
  transition state, and the reviewed Player extent/derived-vector fields;
- calls `RebuildPlayerOptions` and returns 0 on success.

The paired teardown owner `0x00424ED0` later removes the same callback nodes,
corroborating the `+0x08/+0x0C` lifetime role. Helper declarations in maintained
source are descriptive interfaces for observed effects. Private-register helper
ABIs, original symbols, translation-unit partition, and source declaration remain
unknown unless independently demonstrated.

The maintained option-data header exposes only the observed fixed fields: entry
count at `+0x02`, neutral values at `+0x04/+0x08/+0x0C`, mode-0/mode-1 axis speeds
at `+0x10/+0x14`, and corresponding diagonal speeds at `+0x18/+0x1C`. The first
three float fields and the three Player extent families remain deliberately
semantically neutral; this packet does not guess hitbox/graze/item names.

## Compiler and exact feedback

Final `src/Player.cpp` compiles with pinned VC7.1 SP1 build 6030 under fixed
normal:

`/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc`

and the same profile plus `/GL`. `src/ReplayManager.cpp`, which consumes the
closed Player layout, also compiles under both profiles.

The true fixed-normal `/Gy` `PlayerInitialize` COMDAT is `.text` section 50 with
size `0x474` = 1,140 bytes. Adjacent object sections 51 and 52 are only 4-byte
`.rdata` sections and are not counted as function extent. A 1,435-byte
relocation-aware diagnostic against target `0x004247F0` remains `mismatch`:
133/1,251 comparable bytes match, 46 object relocations are present, and
`acceptance_authority=none`. The comparator's requested target window is not the
true object-owned COMDAT size.

Fixed normal is therefore rejected as exact physical ownership. The target's
private EBX live-in boundary and compiler/optimizer context differ immediately
from an ordinary standalone C++ call boundary. `/GL` compilation succeeds and
preserves an LTCG/interprocedural-optimizer hypothesis, but there is no linked-image
LTCG extent Oracle here. Exactness remains unknown. No canonical match unit,
`matches.csv` row, or exact ledger entry was added, and `config/build.toml` remains
open.

## Ledger delta and adjacent-game discipline

Entry ledger state at `6a40c3a69c378132d48fe7e63997334ddc7412cc`:

- candidates 1,239
- origin/boundary pending 1,143
- authored 89 / 26,690 bytes
- exclusions 7
- source-present 62
- canonical exact 0.

Current pre-checkpoint state:

- candidates 1,239
- origin/boundary pending 1,142
- authored 90 / 28,125 bytes
- exclusions 7
- source-present 63
- canonical exact 0.

Delta: pending -1, authored +1 / +1,435 bytes, source-present +1, candidates +0,
exclusions +0, exact +0. The packet expands the reviewed authored denominator
while retaining optimizer/exactness uncertainty.

Adjacent repositories were observed read-only. TH08 HEAD
`a45e99fb1942714e6edded20847e32a654d56f97` was clean; focused committed Player
source was queried only after TH10-local evidence and supplied family-level naming
or source-shape corroboration, not TH10 facts. TH09 HEAD
`b0d8fc11579b821fe07845669333d43478db72eb` was ahead8 with no dirty entries in
the observed status. TH095 HEAD `c1610c4a10cf44b93e263b30fe0ecbf91f46b42b`
was ahead4 with unrelated modified `src/PhotoCamera.cpp`, `src/PhotoRuntime.cpp`,
`src/ecl/EnemyEclRuntimeView.hpp` and unrelated untracked analysis/runtime files;
those dirty files were not read or used. No adjacent address, extent, layout,
owner, exactness, or completion claim was transferred into TH10.

## Verification planes

Final pre-checkpoint state remains deliberately separated:

- source presence: yes for maintained `PlayerInitialize` at `0x004247F0`;
- origin/boundary: reviewed 1,435-byte body classified `authored_game`; internal
  seven-byte alignment retained in the physical extent, post-body CC padding kept
  outside;
- canonical exactness: 0 functions / 0 bytes repository-wide;
- whole faithful Windows i386 build: open; `build.py --check` validates the open
  graph and actual `build.py` returns expected RC=2;
- runtime validation: not performed because no closed reconstructed product exists;
- Factory Truth Kernel: snapshot sequence 0, accepted count 0; no submission or
  replay was made for this packet.

Final focused Player/Replay normal and `/GL` compilation passed. The final
initializer diagnostic remained the mismatch above. `verify-target.py`, pinned
executable toolchain preflight, final toolchain declaration check,
target-bound tracking, reconstruction status, `build-match-unit.py --check`,
`build.py --check`, regenerated progress, public CI, and `git diff --check` all
passed. `config/matches.csv`, `config/match-units.toml`, and `config/build.toml`
are unchanged. A fresh native Ghidra `check {}` again passed exact
`target:th10-main` with `factory-native-command` transport.

## Scratch lifecycle

Session entry `.analysis/` was 94 regular files / 606,862 regular bytes and 880K
allocated, with no file over 64 MiB. This session created and reused
`.analysis/gpt-web/20260914-player-init/` with a manifest.

Immediately before tracked handoff/checkpoint audit, `.analysis/` is 101 regular
files / 703,374 regular bytes and 992K allocated. This campaign is 7 regular
files / 96,512 bytes, with no file over 64 MiB. Current-session ReplayManager
normal/LTCG header-sanity objects were removed after final compilation passed.
Temporary preflight whole-build logs outside the campaign were removed after
campaign-local whole-build logs were written. Retained campaign artifacts are
Player normal and `/GL` objects, the target-bound mismatch JSON/stderr,
whole-build stdout/stderr, and the manifest. No legacy/unknown/provider/toolchain/
target state was deleted.

## Checkpoint and continuation frontier

Pre-commit gates completed successfully: the complete working diff and staged
diff were audited, exactly the ten intended packet files are staged, no unstaged
or untracked remainder exists, `git diff --cached --check` passes, and public CI
passes. A fresh native Ghidra `check {}` also passed exact `target:th10-main` with
`factory-native-command` transport. Create one local English `gpt-web:` checkpoint
and never push. After commit, read the actual hash from live HEAD and run cold
post-commit target/tracking/CI/Ghidra and Truth snapshot checks. The checkpoint is
continuation state, not phase completion.

The next evidence-connected packet should be the Player lifecycle cohort around
this initializer rather than the easiest individual helper:

- `0x004246C0-0x004247B3`: 244-byte constructor/reset candidate;
- `0x00424D90-0x00424EA0`: 273-byte state/reset candidate;
- `0x00424ED0-0x00425014`: 325-byte teardown candidate;
- `0x00425020-0x00425064`: 69-byte allocation/factory candidate.

Together these 911 target bytes form one allocation/zeroing/init/rollback/
teardown seam and directly constrain the newly closed 0x4478 Player extent,
callback-node ownership, resource/option-data lifetime, and original source/data
partition. They remain `unknown/review`; their individual Ghidra boundaries and
origins must be reconciled before any promotion.

Packet balance remains hard-frontier oriented. Recent work covered the 2,494-byte
option rebuild, 3,089-byte central update body, 1,591-byte movement/options owner,
and now this 1,435-byte initializer. The proposed lifecycle cohort is selected for
ownership and destruction connectivity, not for easy function-count progress.
