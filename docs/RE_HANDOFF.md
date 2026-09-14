# TH10 Exact Reconstruction Handoff

## Recovery and checkpoint context

- Repository: `th10`; branch: `main`; target: `target:th10-main`; analysis provider: `th10-ghidra`.
- Starting HEAD for this packet: `63f0c4831bed25f569cdf30eee5e850a493a6000` (`gpt-web: reconstruct TH10 player lifecycle`), initially ahead 6 / behind 0 with staged 0 / unstaged 0 / untracked 0 / conflicts 0.
- The mandatory recovery audit found no interrupted tracked work and no unrelated/unknown dirty tracked paths. Existing ignored `.analysis/`, `.tools/`, `build/`, `ghidra-project/`, caches, and the operator-supplied `resources/th10.exe` were preserved.
- Entry `.analysis/`: 114 regular files, 857,570 regular bytes, 1176K allocated, no file over 64 MiB.
- All required repository and Factory guidance was reread through the registered repository runner before editing. No required guidance path was missing.
- Entry target, executable toolchain (`VC7.1 SP1` build 6030 normal COFF/LTCG/resource/PE32 smoke), tracking, reconstruction status, public CI, match graph, and open build graph preflights passed.
- Native Ghidra discovery and mandatory `check {}` ultimately passed for `target:th10-main` with `attestation.provider_transport=factory-native-command`. Initial Factory transport failures for repository listing, analysis discovery, and `check {}` were recovered by live status inspection and retry; there was no target/provider identity mismatch.
- Intended local checkpoint subject: `gpt-web: reconstruct TH10 player shot update`. Nothing is to be pushed. Recover the live commit hash from Git on the next session rather than inferring it from this handoff.

## Completed hard-frontier packet: Player shot update owner

### Physical owner and ABI

Reviewed and reconstructed the central Player shot-update owner:

- `0x00428280-0x004285EB`, physical span 876 bytes.
- The sole direct caller is the already reconstructed central Player update owner at `0x00425730`; its call is at `0x0042632D`.
- The target receives one stack `Player *` and returns zero with `RET 4`.
- Ghidra reports 870 reachable body addresses; the six-byte difference is the unreachable compiler-alignment region `0x004282AA-0x004282AF`. It remains physically inside the reviewed owner rather than becoming a separate source/data owner.
- Four `CC` bytes at `0x004285EC-0x004285EF` separate this owner from the next raw body.
- `PlayerUpdateShots(Player *)` was already a deliberate maintained source seam before this packet; this packet supplies its body and promotes this owner from `unknown/review` to Player `authored_game` / source-present. The original identifier, translation unit, production optimizer ownership, and helper source ABIs remain unknown.

### Shot runtime row and descriptor evidence

The owner iterates exactly 128 Player rows at `+0x49C`, stride `0x5C`, independently agreeing with the constructor-recovered storage extent. The maintained `PlayerShotRuntimeView` now names only reviewed fields:

- `+0x00..+0x13`: `PlayerTimerView`.
- `+0x14..+0x3F`: a `0x2C` motion subobject containing position, velocity, speed/angular-step, angle, polar magnitude/delta, and flags.
- `+0x40`: shot state.
- `+0x44/+0x48`: primary/secondary managed-VM ids.
- `+0x4C`: still unknown.
- `+0x50/+0x54`: collision/update handoff fields established by the update/collision paths.
- `+0x58`: pointer to a `0x34` shot descriptor.

The maintained `PlayerShotDescriptorView` closes only target-observed fields: signed schedule bytes `+0x00/+0x01`, short value `+0x02`, spawn offsets `+0x04/+0x08`, hitbox extents `+0x0C/+0x10`, angle/speed `+0x14/+0x18`, signed source index `+0x1C`, type `+0x1D`, animation/sound shorts through `+0x22`, callbacks at `+0x24/+0x28`, an intentionally neutral `unknown2C`, and collision callback `+0x30`. The update callback at descriptor `+0x28` is called with Player in ECX and the shot row in EDX; its return value is not consumed, so the maintained callback return type is descriptive rather than an original-source claim.

The reviewed owner now naturally represents the target's major behaviors: type-3 source retirement, VM delete-state transitions, per-shot update callbacks, polar/ordinary motion, managed-VM lookup and placement, ordinary-shot playfield culling after timer frame 10, VM-angle propagation, and the established timer-scale update.

### Effect rows versus shot-source occupancy

A temporary maintained-layout hypothesis that split the 33rd effect row was rejected during compiler/source-shape review. The target Player constructor has one 33-iteration loop with stride `0x6C`, so the final maintained layout preserves all 33 constructor-visible effect rows. Four shot-source occupancy integers instead start naturally at Player `+0x42F8`, and type-3 shots use `shotSourceActive[sourceIndex - 1]`. This produces the target address algebra `+0x42F4 + sourceIndex*4` for source indices 1-4 without an overlap, union, raw-offset accessor, or artificial source statement. The existing transition field remains at `+0x4308` and the Player aggregate remains `0x4478` bytes.

### Helper/callee discipline

The target owner calls or inlines behavior from `0x00409E50`, `0x0040C4D0`, `0x00428D70`, `0x004491C0`, `0x004492A0`, `0x0044BC70`, `0x0044C2A0`, `0x0044C5D0`, and `0x00463B2C`. Maintained source uses descriptive helper declarations for target-specific VM state transitions, angle wrapping, polar velocity, motion advancement, and bounds checking. These declarations intentionally hide private register contracts rather than claiming original helper declarations or translation-unit ownership.

### Adjacent-game hypothesis use

Only after the TH10 row/descriptor/update behavior was recovered, committed TH08 source was consulted as a source-family hypothesis. The observed TH08 HEAD was `a45e99fb1942714e6edded20847e32a654d56f97`, and that repository was clean. TH08 independently corroborates the broad fixed-slot / per-shot update-callback / motion / bounds / VM/timer family, but its shot layout and implementation differ substantially. No TH08 address, extent, layout, origin, exactness, or completion claim was transferred to TH10. TH09 and TH095 were not used for this packet.

## Denominator expansion

Raw target review immediately after the main owner found one complete Ghidra-missed body:

- `0x004285F0-0x00428626`, 55 bytes.
- Four preceding `CC` bytes and nine following `CC` bytes bound it physically before independent `0x00428630`.
- It performs floating-point min/max-style pair construction through register-bound pointers using target constant `0x00470B0C` and ends in plain `RET`.
- Native Ghidra reports neither a containing function at `0x004285F0` nor xrefs to that address.

It was therefore added to the reconstruction denominator as `unknown/review` (`raw_004285F0`) with no source, origin, ABI, role, or exactness promotion. It must not be converted into an easy count win without new evidence.

## Compiler and exactness feedback

Pinned VC7.1 SP1 build 6030 feedback for the final source shape:

- `Player.cpp` fixed normal `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc`: passed.
- `Player.cpp` with the same profile plus `/GL`: passed.
- `ReplayManager.cpp` shared-header sanity under both fixed normal and `/GL`: passed; the reproducible Replay probe objects were removed after the final compile.
- Normal `/Gy` `?PlayerUpdateShots@@YAHPAUPlayer@@@Z` COMDAT size: `0x2B3` = 691 bytes versus the target physical 876-byte owner.
- Relocation-aware target diagnostic over the complete 876-byte target window: `mismatch`, 47 / 784 comparable bytes matched, 23 object relocations, `acceptance_authority=none`.
- The normal object uses ordinary maintained helper calls while the target mixes private-register helpers, an inline primary-VM lookup, and a secondary managed-VM lookup call. Fixed normal therefore does not establish physical ownership or exactness.
- `/GL` compilation preserves an LTCG/interprocedural optimizer hypothesis, but no target-bound linked-image Oracle is available for this owner.
- No canonical match row, exact ledger row, or match unit was added. Reviewed exact function count remains zero.

## Ledger and source state

After this packet:

- 1247 candidates.
- 1144 pending.
- 95 authored functions / 29,912 authored bytes.
- 8 exclusions.
- 68 source-present mappings.
- 0 canonical exact functions.

Delta from packet entry: +1 candidate in the denominator, +1 authored/source-present owner, +876 authored bytes, exact +0. Pending is unchanged because the main owner left pending while the new raw body entered pending.

Tracked files changed by the packet are expected to be exactly:

- `src/Player.cpp`
- `src/Player.hpp`
- `config/functions.csv`
- `config/function-origins.csv`
- `config/implemented.csv`
- `config/reccmp-functions.csv`
- `docs/KNOWLEDGE_BASE.md`
- `docs/PROGRESS.md`
- `resources/progress.svg`
- `docs/RE_HANDOFF.md`

`config/matches.csv`, `config/match-units.toml`, and `config/build.toml` were intentionally not changed.

## Verification planes

Pre-checkpoint final checks completed before staging:

- target verification: passed.
- toolchain lock check: passed; the entry `--execute` preflight had also exercised real normal/LTCG/resource/PE32 output successfully.
- tracking validation: passed, 1247 candidates / 68 mappings / 0 exact.
- reconstruction status: passed, 1247 / 1144 pending / 95 authored / 8 excluded / 68 source-present / 0 exact; build remains open.
- focused Player and Replay normal + `/GL` compiles: passed.
- target-bound shot diagnostic: stable mismatch 47/784 with 23 relocations and no acceptance authority.
- canonical match graph: passed with 0 configured units.
- target-bound build graph check: passed and explicitly open.
- honest whole-build diagnostic: RC=2/open, as expected while production flags/TU partition/libraries/resources/link order remain unresolved.
- public CI: passed.
- `git diff --check`: passed.
- runtime validation: not performed.
- Factory Truth read-only snapshot: sequence 0, accepted count 0, accepted list empty, no pending submission/replay, and no submission/replay was performed by this packet. This is not acceptance of the checkpoint.

Source presence, canonical exactness, whole-product build closure, runtime validation, and Factory acceptance therefore remain separate states: this packet is source-present for its main owner, non-exact, whole build open, runtime unvalidated, and unaccepted by Truth.

## Scratch disposition

Entry `.analysis/` was 114 regular files / 857,570 regular bytes / 1176K allocated, with no file over 64 MiB. After packet work and current-session cleanup it is 120 regular files / 970,396 regular bytes / 1304K allocated, again with no file over 64 MiB. The current campaign `.analysis/gpt-web/20260914-player-shot-update/` contains six retained files / 112,826 bytes: final Player normal/LTCG probes, the target-bound diagnostic JSON, whole-build stdout/stderr, and the manifest.

Only the current-session reproducible ReplayManager normal/LTCG header-sanity objects were removed after their final successful compile. No legacy campaign, provider state, toolchain state, target, Wine prefix, or unknown analysis artifact was removed.

## Remaining unknowns and next hard frontier

The shot owner's original source identifier/TU, per-helper source declarations/private ABIs, production normal-vs-LTCG ownership, descriptor `+0x2C`, shot `+0x4C`, and the complete semantics of several target helpers remain unknown. The main owner is not exact under the available standalone COFF Oracle, and no linked-image LTCG extent workflow currently supplies exactness authority.

The next evidence-connected hard packet should be **ledger candidate `0x00428630-0x00428AC1` (1170 bytes), currently `unknown/review` and boundary-unreviewed**, the immediately adjacent Player shot collision/scoring owner. It is preferable to unrelated small leaves because it consumes the same 128 shot rows and `0x34` descriptors, independently exercises hitbox/collision callback and state transitions, and loops the 32 gameplay effect rows whose relationship to the constructor-visible 33-row storage now has stronger evidence. It can therefore challenge or refine this packet's names/layout while attacking another materially large central owner. It must still independently reconcile its physical boundary, ABI, origin, callbacks/data ownership, compiler profile, and exactness.

Recent scheduling remains hard-frontier weighted: prior checkpoints attacked the 3089-byte central update body, 1591-byte movement/options owner, 1435-byte initialization owner, 911-byte lifecycle cohort plus denominator expansion, and now this 876-byte shot-update owner plus a new raw denominator candidate. The proposed 1170-byte collision/scoring owner continues that balance rather than optimizing for the easiest remaining function count.

This handoff is a continuation point only. The TH10 exact phase, faithful whole Windows i386 product closure, runtime validation, semantic phase, and ports are not complete.
