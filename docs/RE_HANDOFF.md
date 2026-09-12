# TH10 exact reconstruction handoff

## Recovery and authority

Phase remains exact source reconstruction. Source presence, compilation,
canonical exactness, whole-build closure, runtime validation, and Factory
acceptance remain separate. Ghidra has `exactness_credit=none`.

Live entry was clean `main` at
`55b876dc15a74198ab43135484cdac19989bc3f0`, upstream
`origin/main=c10c371ed3b09451ef1076f6ec1e99d87f174d22`, ahead5/behind0,
0 staged/unstaged/untracked/conflicts. `.analysis` was 142,662 bytes. The
recovered HEAD already contained committed replay-loader and replay-stage
transition packets; no dirty recovery work existed. Private target, `.tools/`,
`ghidra-project/`, prior `.analysis`, and `build/` were preserved.

All required repository/Factory guidance, contracts, semantic guidance, and
repo-local skill were reread in full. Target/toolchain execute/tracking/status/
public-CI preflights passed. Fresh Ghidra discovery passed; the first mandatory
`check {}` transport-failed, live status was reread clean, and retry passed exact
`target:th10-main` + `factory-native-command`. Truth refresh was unavailable
because another Factory operation owned `<operator-path>`; no packet acceptance
is inferred.

The ignored private `resources/th10.exe` was not modified, relocated, replaced,
or staged. No `/mnt` search or `TH10_TARGET_PATH` override was used.

## Hard packet — player option rebuild

Primary target extent:

`0x00426F70-0x0042792D` — 2,494 bytes, previously `unknown/review`.

Maintained descriptive mapping: `RebuildPlayerOptions` in `src/Player.cpp`.
TH10 target strings retain `.\\src\\game\\player.cpp:193 PlayerInf` plus
PlayerInf initialize/shutdown labels, supporting the Player source family but
not proving this function's original identifier or TU owner.

### Boundary and machine ABI

The physical extent is correct. Ghidra reports 2,471 reachable bytes because
three retained unreachable alignment sleds account exactly for 23 bytes:

- `0x0042701A-0x0042701F`: 6 bytes
- `0x00427197-0x0042719F`: 9 bytes
- `0x004278D8-0x004278DF`: 8 bytes.

The final instruction is `ret 4` at `0x0042792B-0x0042792D`.
Nine direct calls at `0x004182E0`, `0x004184E0`, `0x0041B420`, `0x0041B67A`,
`0x00424D3D`, `0x004259DD`, `0x00425B98`, `0x00425C4C`, and `0x0042A5EF`
all push the player/runtime owner. Target machine boundary is therefore one
stack `Player *` input with callee pop 4. This is not a source calling-convention
claim.

### Reviewed layout and behavior

Four 0x98-byte option records begin at player `+0x32A0`. Replay's old
`+0x32D4` view was only the first replay-copied field inside option 0, not the
option base. Observed option fields are state `+0x00`, four 8-byte position
pairs `+0x34/+0x3C/+0x44/+0x4C`, primary/secondary VM ids `+0x68/+0x6C`,
option index `+0x88`, reset flag `+0x8C`, and callback `+0x90`.

Observed player fields used by this seam are resource pointer `+0x10`, position
`+0x3CC/+0x3D0`, option-data pointer `+0x45C`, options `+0x32A0`, option count
`+0x3500`, 33 8-byte history pairs at `+0x436C-0x4473`, and option mode
`+0x4474`.

The target rebuilds all secondary option VMs, computes signed power/20 capped at
four, rebuilds active option position/VM/callback state by character/shot data,
retires inactive primary VMs, writes option count, and sets all four reset flags.
Target `0x00463B2C` is an x87 float-to-int compiler/runtime helper, so maintained
source uses natural casts rather than exposing it as a game API.

New `src/Player.hpp` holds only the reviewed partial layout with static offset/
size assertions. New `src/Player.cpp` holds natural maintained behavior and
neutral descriptive VM-manager interfaces. `src/ReplayManager.cpp` now reuses
this Player layout during playback restore instead of maintaining a conflicting
Replay-only slot type. Original Player type completeness, function identifier,
TU, helper names/conventions, and physical owners remain unknown.

## Denominator expansion

New raw candidate:

`0x00427AD0-0x00427ADB` — 12 bytes.

The reviewed rebuild stores this address as an option callback. The wrapper
preserves EDI, moves live-in ECX to EDI, calls separate candidate `0x00427AE0`,
returns zero, and uses plain RET. It is CC-delimited and has only the observed
data reference at `0x00427372`. Source-written adapter versus compiler/optimizer
wrapper is unresolved, so it remains `unknown/review` with no source or exact
credit.

## Compiler feedback

Pinned VC7.1 SP1 build 6030 compiled final `src/Player.cpp` and the modified
`src/ReplayManager.cpp` with fixed normal:

`/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc`

and the same source/profile plus `/GL`.

The true normal `/Gy` `?RebuildPlayerOptions@@YAXPAUPlayer@@@Z` COMDAT is
0x797 = 1,943 bytes; target physical extent is 2,494 bytes. Fixed standalone
normal is therefore not the physical target match.

A separate caller-supplied 2,494-byte comparator window returns mismatch with
186/2,246 comparable bytes and `acceptance_authority=none`. Its reported
2,494-byte object size is the requested comparison window, not the true
1,943-byte COMDAT. No canonical match unit or exact row was added. `/GL`
compilation only preserves an LTCG/interprocedural hypothesis.

## Adjacent provenance

Adjacent games were consulted only after TH10-local recovery:

- TH08 HEAD `a45e99fb1942714e6edded20847e32a654d56f97`, clean. Committed Player
  source corroborates a four-option subsystem but has a different layout.
- TH09 HEAD `ae13c343ff58cbce411cd507f9837b468635d58e`, clean. No relevant committed
  Player implementation/history was found.
- TH095 HEAD `a7e340d3ff00e81b56584bc03288953f754b1faf`, ahead6, with unrelated modified
  `src/ResultScreen.cpp` and four unrelated untracked files. Only committed
  content was queried; dirty content was not used.

No adjacent address, extent, layout, ownership, exactness, or completion claim
was transferred.

## Ledger state

Entry metrics from the committed start HEAD:

- candidates 1,236
- pending 1,143
- authored 86 / 19,516 bytes
- exclusions 7
- source-present 56
- canonical exact 0.

Current packet metrics:

- candidates 1,237
- pending 1,143
- authored 87 / 22,010 bytes
- exclusions 7
- source-present 57
- canonical exact 0.

Delta: +1 candidate, pending net 0, +1 authored / +2,494 bytes, +1 source
mapping, +0 exclusions, +0 exact. `config/matches.csv`,
`config/match-units.toml`, and `config/build.toml` are unchanged.

## Scratch / acceptance

`.analysis` entry was 142,662 bytes. Current campaign is
`.analysis/gpt-web/20260913-player-rebuild/`. Peak was 244,336 bytes. After
reducing evidence and deleting only this session's reproducible normal/LTCG
objects and detailed comparator JSON/stderr, `.analysis` measured 147,806 bytes.
Retained campaign files are `compiler-shape-report.txt` (4,199 bytes) and
`manifest.json` (945 bytes). No current campaign artifact exceeded 64 MiB; no
legacy/shared/provider/toolchain/target state was removed.

Truth snapshot refresh remained unavailable because another Factory operation
owned `<operator-path>`. No Player boundary/source/compile/ownership/exactness
claim is accepted or rejected by this packet on that basis.

## Verification and checkpoint plan

Before checkpointing, rerun from final tracked source:

- normal and `/GL` compile for `src/Player.cpp` and `src/ReplayManager.cpp`
- true normal `RebuildPlayerOptions` COMDAT extent and target comparator
- `verify-target.py`
- `verify-toolchain.py --check`
- tracking and progress checks
- `build-match-unit.py --check`
- `build.py --check`
- honest `build.py` (expected RC=2/open while inputs remain unknown)
- reconstruction status and public CI
- `git diff --check`
- fresh target-bound Ghidra `check {}`.

Then audit the complete working diff, stage only the intended packet paths,
audit the complete staged diff, create one local English `gpt-web:` checkpoint,
never push, and repeat applicable cold validation post-commit.

Current verification-plane state before checkpoint:

- source presence: reviewed maintained source exists for `0x00426F70`
- canonical exactness: 0 functions / 0 bytes
- whole Windows i386 build: open
- runtime validation: not started for this packet
- Truth acceptance: unavailable.

## Next evidence-connected hard packet

Preferred next packet is the directly connected Player option callback cohort:

- `0x00427950-0x0042795B` — 12-byte wrapper, `unknown/review`
- `0x00427960-0x00427AC6` — 359-byte body, `unknown/review`; Ghidra reports 353
  reachable bytes and raw target code has a skipped 6-byte alignment LEA at
  `0x00427A0A-0x00427A0F`
- `0x00427AD0-0x00427ADB` — new 12-byte wrapper, `unknown/review`
- `0x00427AE0-0x00427B42` — 99-byte body, `unknown/review`.

The reviewed rebuild writes the two wrapper addresses into option records, and
each wrapper dispatches to its adjacent larger body. This 482-byte cohort is
connected ownership/control-flow work rather than an unrelated easy leaf.
Resolve source-written callback versus compiler/optimizer adapter independently
before origin promotion.

Fresh Ghidra function metadata confirms 12/353/99 reachable bytes for the three
Ghidra-recognized entries. A callback-body decompile attempt suffered Factory
transport failure; live status was reread unchanged. Do not invent semantics
from that unavailable result. Do not absorb nearby `0x00427B50-0x00427C04`
without separate call/data ownership evidence.

Packet balance remains hard-frontier oriented: this session attacked a central
2,494-byte owner, corrected a cross-subsystem layout view, and expanded the
candidate denominator instead of harvesting small exact-looking functions.
