# Four complete audio command owners — 2026-10-30

Two independent canonical18-input colds close four complete authored owners:
**381 bytes/all22 fields/all9 fixed local destinations**, zero differences.
The real queue and all three selected callers agree on their actual interfaces.
There is no partial-owner or summed-helper admission.

| Owner | Target span | Bytes | Instructions | Fields | Locals |
| --- | --- | ---: | ---: | ---: | ---: |
| SoundPlayerView::QueueCommand | 43E460..43E4C7 | 104 | 32 | 0 | 4 |
| FrontEndLoadMusic | 420A90..420B07 | 120 | 32 | 5 | 1 |
| FrontEndPlayMusic | 420B10..420B58 | 73 | 18 | 8 | 1 |
| GameQueueScaledSoundCommand | 420C30..420C83 | 84 | 23 | 9 | 3 |

## Fresh complete source and target evidence

At entry the actual18-input GUI image already has load120/play73 structural
matches. Neither was previously admitted. Their complete strings, supervisor,
profile, sound-object, CRT and security-cookie fields are now independently
bound to target addresses. Load retains the256-byte local, dot search and three
extension writes before opcode1 publication. Play retains optional dummy opcode4,
requested opcode2 and immediate profile-byte publication. Both return RET8.

Scaled command was78/84. The old `speed <= 1` predicate emitted `TEST AH,41`/JP,
where target uses JE: an unordered comparison must reach division. Explicit zero
and ordered-greater-than-one bypasses restore that predicate. Computing each
selected double value in its actual branch restores the two input loads, the
complete84-byte x87 schedule and conversion binding. An initial live-value
rewrite fixes the predicate but still emits78 and hoists the input load; it is
replaced by the branch-local quotient. Source uses ordinary C++ and the existing
speed object; no asm, volatile, new data alias, forced register or padding.
The native quotient remains on x87 until the target conversion call. This
exact pinned codegen does not establish portable numeric or runtime behavior.

Queue was90/104. A natural bounded0..30 index scan and direct indexed record
stores remove its duplicated first test/extra saved register and recover the
whole-owner base offsets and compiler loop alignment. The0x10C-byte records
start at owner+1F88. Opcode/argument precede the actual byte copy, then step is
cleared. Full-queue drop and both RET8 exits are included in the raw-equal
104 bytes. Actual ABI is EAX sound owner, EDX path and two stack words. Load,
play and scaled callers reproduce that interface; all three bind the real
complete queue owner. No annotations or compiler switches are added.

Fresh dense target decoding covers all105 instructions and all9 local
branches/returns. CC counts before/after are queue3/8, load10/8, play8/7,
scaled7/12. Source-boundary instruction decoding verifies20 queue entry refs,
10 load,9 play and5 scaled refs. Four additional encoded queue references have
unclassified caller boundaries; all address entry. No encoded E8/E9 reference
enters any of the four interiors. Those four caller attributions remain unknown;
no new boundary or origin classification follows from this audit.

## Canonical reproduction

All recipes retain the genuine existing Enemy/GUI18-input responsibility order
and InitializeVm root. FrontEnd keeps its declared /GL /GS /EHs profile, Sound
its existing /GL profile, and all other support overrides stay fixed. Each cold
recompiles all18 inputs without reuse, serial CPU0/nice10 with bounded jobs and
retained exits. Both runs have identical frozen source/header/manifest hashes.
PDB contribution ownership, all resolved fields, fixed destinations, literal
payloads, complete extents and actual callee bindings are checked independently.

```sh
timeout 600 taskset -c 0 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit sound-player-queue-command --unit front-end-load-music --unit front-end-play-music --unit front-end-queue-scaled-sound --json
scripts/repo-python docs/evidence/front-end-music-exact-20261030/verify-proof.py --artifact-dir build/gpt-6.1-sol/front-end-music-context-20261030/cold-b
```

Only the four new units are cold-replayed, twice. Same-image explicit selectors
for existing damage1170/auxiliary202 pass1372/all34. Complete GUI interpreter
receipt4496/all333 and updater3836/all72 also reproduce, retaining their existing
NONEXACT states and no new parent credit. No full historical replay, allocation
failure/lifecycle simulation, host mutation or product test is required.

Current18-input context is
`.analysis/gpt-6.1-sol/front-end-music-context-20261030/cold-b-context.json`.
Earlier GUI18-input and separate Player17-input contexts now have historical
FrontEnd.cpp/Sound.cpp pins. Before further17-input work, relink that declared
graph with the two changed sources; do not substitute the18-input parent epoch.
Unchanged smaller ANM and Player drawing contexts remain independent.

The four existing source mappings/origins/boundaries stay unchanged. Authored
exact increases381 to101913/275817 across499 owners; total codegen170017/1376.
Source mappings1207/authored689/backlog190. Original identifiers/helper/class/TU,
physical global ownership, product/runtime closure and portability remain open.
