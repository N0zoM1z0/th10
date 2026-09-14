# TH10 exact reconstruction handoff

## Recovery and authority

The campaign remains in exact source reconstruction with early faithful Windows
i386 build feedback. Source presence, compilation, canonical exactness,
whole-build closure, runtime validation, and Factory acceptance are independent
states. Native Ghidra evidence remains provisional with `exactness_credit=none`.

This conversation started clean on branch `main` at
`8d392cbad616734505878870fabae8068abd9a40`, with
`origin/main=ae83c3ab72400534834746aeb7613a603a347310`, ahead5/behind0, 0 staged,
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
LTCG, resource, and PE32 paths through the pinned headless-Wine wrapper. Honest
`scripts/build.py` remains expected RC=2/open because production compile flags,
translation-unit partition, libraries, resources, and link order are not
established.

Native `th10-ghidra` operation discovery was repeated before target-dependent
work. Mandatory `check {}` passed exact `target:th10-main` with
`attestation.provider_transport=factory-native-command`; target SHA-256 remains
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Factory transport was intermittently unavailable during initial describe,
analysis discovery/check, preflight, and one source-edit status operation. Every
operation with uncertain execution state was followed by live repository or
artifact inspection before retrying; no transport failure was treated as
rollback or evidence.

A pre-checkpoint `factory_get_accepted_snapshot(th10)` request was unavailable
because another Factory operation owned the operator path. No accepted/rejected/
pending count is inferred from that failure and no acceptance submission or
replay was performed.

## Hard packet: Player lifecycle cohort

The selected packet followed the reviewed Player initializer into its complete
allocation/construct/reset/teardown seam rather than selecting an easy leaf:

- `0x004246C0-0x004247B3`: 244-byte Player constructor owner
- `0x00424D90-0x00424EA0`: 273-byte runtime-state reset owner
- `0x00424ED0-0x00425014`: 325-byte Player destructor owner
- `0x00425020-0x00425064`: 69-byte allocation/factory owner.

All four were `unknown/review` on entry. Their combined 911 target bytes are now
classified `authored_game` / Player with source present, but canonical exactness
remains unknown.

### Constructor and private ABI

`0x00425020` allocates exactly `0x4478` bytes, then at `0x00425033` moves the
allocation to ESI immediately before calling `0x004246C0`. The constructor never
establishes ESI from ECX or a stack parameter; it consistently uses live-in ESI,
returns the same object in EAX, and ends in RET. Therefore the target machine
boundary is recorded as `internal ESI Player* / RET`. Natural maintained source
uses `Player::Player()` and does not claim that private optimizer ABI.

The constructor-visible subobjects run before the containing Player body. The
containing body then executes `rep stosd` for exactly `0x111E` dwords = `0x4478`
bytes, publishes `g_Player`, and returns the same object. This independently
continues to support the maintained `sizeof(Player)==0x4478` layout.

### Constructor-driven layout closure

TH10 constructor order adds direct layout evidence rather than guessed names:

- Player `+0x49C..+0x329B` is exactly 128 rows of `0x5C`; each row exposes a
  constructor-visible timer flag at row `+0x10`.
- Four 0x98-byte option rows beginning `+0x32A0` expose constructor flag `+0x80`.
- The constructor iterates 33 0x6C-byte effect rows beginning `+0x350C`, clearing
  the nested timer flag at row `+0x54`. Central Player update still processes
  only the first 32 rows; the 33rd row is represented structurally without an
  invented gameplay role. The 33 rows end at `+0x42F8`, leaving 0x10 bytes before
  the known `+0x4308` transition field.
- Player `+0x36C` is draw-VM-relative `+0x358`; the destructor frees and nulls
  this pointer, so maintained source exposes it only as neutral `ownedData358`.
- Before the containing draw-VM constructor clears its 0x3AC-byte storage, nine
  neutral nested subobjects clear bit zero at VM-relative
  `+0x6C/+0xB0/+0xFC/+0x128/+0x174/+0x1B0/+0x1FC/+0x228/+0x378`.
  Maintained source models these as neutral constructor-flag subobjects rather
  than guessing timer/interpolator types. The containing VM constructor then
  clears its storage and writes the observed word at VM `+0x384`.

The nested timer at effect-motion `+0x20` naturally places its flag at effect-row
`+0x54`; existing central-update accesses were updated to use that nested
`PlayerTimerView` without changing behavior.

### Reset owner

All three observed `0x00424D90` callers establish ESI as the Player before the
call. The target body sets Player runtime state `+0x458` to 1, initializes/resets
all three timer-shaped records at `+0x460/+0x474/+0x488`, marks the managed VM id
at `+0x329C` pending and clears it, then tail-jumps at
`0x00424E9C-0x00424EA0` into the VM-group state helper using the two observed
globals. Its physical 273-byte extent is followed by CC padding through
`0x00424EAF`. Maintained source uses descriptive `PlayerResetRuntimeState` while
the target machine boundary remains `internal ESI Player* / tail JMP`.

### Destructor and resource ownership

`0x00424ED0` has an MSVC SEH prologue but obtains the Player from the sole
original stack argument and ends at `0x00425014` with RET 4. The maintained source
represents these target-observed effects:

- removes Player `+0x08/+0x0C` update/draw callback nodes through the shared
  callback manager under the observed critical section and lock-depth counter;
- clears global `g_Player`;
- when the lifecycle flag bit is set, marks VMs using Player `+0x10` resource
  pending and transfers Player option-data ownership to the shared option-data
  global;
- otherwise destroys/frees the animation-manager Player cache at manager
  `+0x3AD08C`, frees Player option data, and clears shared ownership;
- frees and nulls draw-VM relative `+0x358` / Player `+0x36C`.

The exact original synchronization/SEH source abstraction remains unknown; the
maintained destructor deliberately does not invent a specific RAII guard type.
The body is bespoke Player lifetime code and is `authored_game` despite compiler
SEH scaffolding.

### Factory

`0x00425020-0x00425064` is a no-argument Player factory: operator-new 0x4478,
constructor, `PlayerInitialize`, destructor/free and NULL return on initialization
failure, otherwise return the Player. Natural `new Player` / `delete player`
source reproduces that ownership flow. Target private optimizer context passes
the object through ESI/EBX rather than the standalone ordinary C++ ABI.

## Denominator expansion around the lifecycle seam

Raw target review found seven CC-delimited bodies that the Ghidra candidate
inventory had missed; all were added to both candidate/origin ledgers:

- `0x004247C0-0x004247C6`: 7-byte `object+0x10` bit-zero-clearing ctor shape
- `0x004247D0-0x004247D9`: 10-byte `object+0x80` ctor shape
- `0x004247E0-0x004247E6`: 7-byte `object+0x54` ctor shape
- `0x00424EB0-0x00424EB2`: `xor eax,eax; ret`
- `0x00424EC0-0x00424EC2`: `xor eax,eax; ret`
- `0x00425070-0x00425088`: 25-byte deleting-destructor helper
- `0x00425090-0x004250A1`: 18-byte destructor/free cleanup-shaped body.

Only `0x00425070` is classified. Its instruction/control-flow template is
identical to the already reviewed 25-byte Replay deleting destructor at
`0x004296D0` apart from the destructor call target, and pinned VC7 maintained
Player source automatically emits `??_GPlayer@@QAEPAXI@Z`. It is therefore
`compiler_generated` / excluded. `0x00425090` could be an EH cleanup funclet or a
retained source helper; evidence does not distinguish them, so it stays
`unknown/review`.

The three constructor-shaped bodies also remain `unknown/review` despite very
strong codegen evidence: pinned normal VC7 emits byte-identical maintained
constructors at `0x004247C0` (7/7 comparable target bytes), `0x004247D0` (10/10),
and `0x004247E0` (7/7), each with zero relocations. No observed target xref/raw
absolute VA reference proves their original explicit/implicit source owner or
physical compiler profile, so they receive no source-presence mapping and no
canonical exactness credit. The two zero-return stubs likewise remain unknown.

## Compiler and exact feedback

Final `src/Player.cpp` and shared-header `src/ReplayManager.cpp` compile with the
pinned VC7.1 SP1 build-6030 candidate under fixed normal:

`/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc`

and the same profile plus `/GL`.

True fixed-normal `/Gy` Player lifecycle text extents are:

- `Player::Player`: 186 bytes versus target 244
- `PlayerResetRuntimeState`: 274 versus target 273
- `Player::~Player`: 266 versus target 325
- compiler-emitted `??_GPlayer@@QAEPAXI@Z`: 30 versus target helper 25
- `PlayerCreate`: 72 versus target 69.

Final target-bound normal diagnostics remain non-exact:

- constructor: 27/236 comparable bytes, 2 relocations
- reset: 28/253, 5 relocations
- destructor: 17/213, 28 relocations
- factory: 12/49, 5 relocations.

All report `acceptance_authority=none`. The reset/factory size proximity is not
an equality claim. Fixed normal keeps the draw-VM constructor as an out-of-line
call while the target optimizer context has it inlined into the Player
constructor; the target also uses private ESI/EBX/stack ABIs. Successful `/GL`
therefore preserves a live LTCG/interprocedural-optimizer hypothesis. No linked-
image LTCG extent Oracle exists here, so no standalone exactness promotion is
made. `config/matches.csv`, `config/match-units.toml`, and `config/build.toml`
remain unchanged.

## Ledger delta and adjacent-game discipline

Entry state at `8d392cbad616734505878870fabae8068abd9a40`:

- candidates 1,239
- origin/boundary pending 1,142
- authored 90 / 28,125 bytes
- exclusions 7
- source-present 63
- canonical exact 0.

Current pre-checkpoint state:

- candidates 1,246
- origin/boundary pending 1,144
- authored 94 / 29,036 bytes
- exclusions 8
- source-present 67
- canonical exact 0.

The four existing lifecycle candidates move pending -> authored/source-present,
while seven newly discovered denominator candidates add six unresolved rows and
one compiler-generated exclusion. Net delta: candidates +7, pending +2, authored
+4 / +911 bytes, exclusions +1, source-present +4, exact +0.

Committed TH08 source was consulted only after TH10-local lifecycle/layout
recovery. TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97` was clean.
Its committed Player/chain and timer source corroborates only broad source-family
patterns such as constructors, top-level Player zeroing, callback-chain teardown,
and timer objects. No TH08 address, layout, state value, physical owner,
exactness, or completion claim was transferred. TH09 and TH095 content was not
used in this packet.

## Verification planes

Final pre-checkpoint state remains deliberately separated:

- source presence: yes for the four reviewed lifecycle owners at
  `0x004246C0`, `0x00424D90`, `0x00424ED0`, and `0x00425020`
- origin/boundary: those four physical bodies are reviewed `authored_game`; seven
  new raw candidates expand the denominator, of which only `0x00425070` is a
  reviewed compiler-generated exclusion
- canonical exactness: 0 functions / 0 bytes repository-wide
- whole faithful Windows i386 build: open; `build.py --check` passes the honest
  open graph and actual `build.py` returns expected RC=2
- runtime validation: not performed because no closed reconstructed product exists
- Factory acceptance: pre-checkpoint accepted-snapshot request unavailable because
  another Factory operation owned the operator path; no acceptance count claimed.

Final Player/Replay normal and `/GL` compilation passed. Final target diagnostics
remain as recorded above. `verify-target.py`, entry executable
`verify-toolchain.py --execute`, final `verify-toolchain.py --check`, target-bound
tracking, reconstruction status, `build-match-unit.py --check`, `build.py --check`,
honest RC=2 whole-build diagnostic, regenerated progress, public CI, and
`git diff --check` passed. A fresh native Ghidra `check {}` immediately before
staging again passed exact `target:th10-main` with `factory-native-command` transport.

## Scratch lifecycle

Session entry `.analysis/` was 101 regular files / 703,941 regular bytes and 992K
allocated, with no file over 64 MiB. This session created and reused
`.analysis/gpt-web/20260914-player-lifecycle/` with a manifest.

After final validation and current-session cleanup, `.analysis/` is 114 regular
files / 856,713 regular bytes and 1176K allocated. The lifecycle campaign is 13
regular files / 152,772 bytes, with no file over 64 MiB. Removed artifacts are
only the reproducible current-session minimal lifecycle shape-probe source,
normal/LTCG probe objects, and ReplayManager normal/LTCG header-sanity objects.
Retained campaign artifacts are final Player normal/LTCG objects, seven target-
bound diagnostic JSON files, bounded target lifecycle objdump, whole-build
stdout/stderr, and the manifest. No legacy/unknown/provider/toolchain/target state
was deleted.

## Checkpoint and continuation frontier

Pre-commit gates completed successfully: the complete working diff was audited
to durable-output completion, exactly the ten intended packet files are staged,
there is no unstaged or untracked remainder, `git diff --cached --check` passes,
and public CI passes. The staged index is identical to the just-audited worktree
because staging left zero unstaged changes; the only subsequent change is this
checkpoint-status wording, reviewed below and re-staged separately. Create one
local English `gpt-web:` checkpoint and never push. After commit, read the actual
hash from live HEAD and run cold post-commit target/tracking/CI/Ghidra and Factory
accepted-snapshot checks. The checkpoint is continuation state, not phase
completion.

The next evidence-connected hard packet should be `0x00428280-0x004285EB`
(876 bytes, currently `unknown/review`). It is the central Player shot/update
owner called directly from the already reviewed Player update body, and prior TH10
analysis observes its 128-row work beginning at Player `+0x49C`. This lifecycle
packet has now independently closed that storage as exactly 128 rows of 0x5C with
a constructor-visible timer at row start, providing stronger layout/constructor
context for shot ABI, row ownership, VM/data dependencies and source shape.
Reconcile its full physical boundary, 128-row semantics, callbacks/data owners,
and compiler profile before any source/origin promotion.

Packet balance remains hard-frontier oriented: recent checkpoints covered a
2,494-byte option rebuild, 3,089-byte central update body, 1,591-byte movement/
options owner, 1,435-byte initializer, and now a 911-byte four-owner lifecycle
cohort plus seven denominator discoveries. The proposed 876-byte shot owner is
selected for central call-graph/layout importance, not because it is the easiest
remaining function; smaller 174/181-byte connected helpers remain available but
are not preferred merely for function-count progress.
