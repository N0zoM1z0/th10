# Player initialization: caller, type and publication controls

2026-10-23, starting276ada7. Complete PlayerInitialize4247F0/1435 improves from
1170 matching/265 different bytes to **1195 matching/240 different bytes**, after
replaying all49 independently identified address fields. It remains nonexact.
Headers, ABI declarations, canonical recipes and exact ledgers are unchanged.

## Maintained change and complete comparison

[The patch](maintained.patch) adds two compiler-only read/write barriers. One
completes draw-node flag writes before publishing its callback argument; the
other finishes highlight-timer writes before preparing the option-rebuild call.
The first recovers the target memory-form AND, and the second prevents the call
argument from being pushed before those timer writes. No CPU fence, extra call,
padding, dummy write or private-register declaration is introduced. Original
source barriers and threading remain unknown.

Over the entire1435-byte owner,178 bytes change:32 target repairs and7 losses,
net25 repairs. Highlight-only gives22 repairs/no losses; combining both tested
constraints gives the selected1195 result. The improvement is not monotonic.
[All240 residual differences and49 fields](parent-differences.json) remain
available; no ordinary instruction or local control destination is masked.

[Evidence](evidence.json) records16 completed controls plus the maintained
reproduction. Every complete image retains1435 bytes, all49 independent field
bindings and eight full shared guards1753/all99. Target destinations are bound
from the reviewed TH10 calls/data and existing canonical contracts, never from
candidate field positions. GameScoreState+28/+2C remain explicit base/addends.

The selected private and maintained builds reproduce all519 uniquely enumerated
complete public PDB owners raw, totaling140370 bytes. Static/unowned fragments
and target equality are outside this check. Maintained reproduction compiles
Player once and reuses16 pinned inputs from the preceding17-input graph. No
independent whole-graph pair, new exact credit, product or runtime is claimed.

## What was actually tested

| Control | Complete match /1435 | Observed result |
| --- | ---: | --- |
| Ordinary Chain insertion members | 1170 | All17 inputs freshly compiled; whole-parent neutral |
| Ordinary Player Initialize member and its real caller | 1170 | All17 inputs freshly compiled; whole-parent neutral |
| All remaining maintained VM caller sources | 1170 | 24 inputs, seven newly compiled/17 pinned; whole-parent neutral |
| Actual AnmVmView embedding, corrected field mapping | 1170 | All17 inputs freshly compiled; whole-parent neutral |
| Draw flags followed by compiler barrier | 1173 | Ten repairs/seven losses |
| Volatile draw flags / existing flag bitfield | 1161 /1170 | Volatile regresses; bitfield whole-parent neutral |
| Calc manager local before/after flags snapshot | 1170 /1170 | Both forms have identical complete replay, different from baseline |
| Draw manager local with publication barrier | 1172 | Fourteen repairs/12 losses; not selected |
| Draw publication with original calc / unbarriered calc snapshot | 1155 /1155 | Equal complete replays; regressive |
| Additional barrier after draw-node owner publication | 1173 | Changes12 still-wrong bytes versus draw-only barrier; not byte-neutral |
| Callback fields published immediately after allocation | 1158 | Regressive; target store order does not prove original source order |
| Highlight publication alone | 1192 | 22 repairs, no losses |
| Draw plus highlight publication | 1195 | Selected;32 repairs/seven losses |

The Chain member model consistently changes declarations, definitions and all
nine calls in its17-input graph. Its newly mangled symbols bind the same target
insertion owners; both complete131-byte guards still pass. No old static-call
object is mixed into that private interface. TH09/TH095 members corroborate a
possible source family only; TH10 equality is established independently.

The24-input graph contains all27 currently maintained explicit VM-member calls
across nine caller TUs. This does not close the target's28-call inventory in11
physical owners. The wrapper remains200 bytes/nativeEDX versus202/targetESI;
the core273-byte guard stays exact. Declarations, more caller visibility and the
tested real embedding therefore do not explain that receiver bridge here.

The first embedding attempt fails before emitting an image: a native offset
assertion rejects mapping Player's color+300 to ANM primaryColor+2FC. The corrected
mapping uses secondaryColor+300, and all six extent/field assertions pass. This
is a relation between current storage views, not original class ownership.
The corrected embedding is measured and neutral; the bad mapping is never
silently compiled or adopted. Equal scores elsewhere are not called neutral
unless the complete replayed bytes agree.

## Recovery, remaining work and artifact lifecycle

Current producers are under
`.analysis/gpt-6.1-sol/player-chain-members-20261023`. Use manifest-final.json,
common-final.py, probe-maintained.py and bound-parent-final.py for current source.
Historical manifest/producer pins remain immutable and belong to276ada7 inputs.
Do not restore old maintained source or relax those pins to rerun a probe.

```sh
scripts/repo-python .analysis/gpt-6.1-sol/player-chain-members-20261023/bound-parent-final.py maintained
```

Expected result: **mismatch**,1195/1435 with49 fields. Keep both complete carriers
under `build/gpt-6.1-sol/player-chain-members-20261023`: maintained-17 and
highlight-publication-17. Previous player-schedule maintained17 and player-services
support caches remain required by retained historical/current producers.

The current residual contains197 differences before offset150,21 in150..1FF,
11 in200..3FF and11 in400..59A. Prioritize callback/VM receiver and address
lifetime constraints in the prefix. The first flags/global pair still chooses
different registers and the global load's longer encoding shifts later bytes;
the wrapper receiver bridge remains open. Consistent natural Chain parameter
reordering is unexecuted; it requires every graph consumer to recompile, new
signature bindings and both complete131-byte insertion guards. Later timer-constant placement, one
commuted vector operand, scratch-store order and selection-index registers also
remain. A different natural vector-value operator return shape is **unexecuted**;
the earlier direct-constructor control does not test it. Preserve every byte and
field; do not substitute a small helper match for the large owner.

[Cleanup](cleanup-summary.json) losslessly archives15 completed control builds
and four closed source/header snapshots:665 files,33400408 bytes become729592
bytes, reclaiming32670816 payload bytes. XZ retains duplicate inputs compactly.
Every member is read back and checked for length/SHA-256 before removal, and all
37241 protected file hashes remain unchanged. Sources for the selected candidate,
both current carriers, compact reports, manifests and producers remain expanded.
Canonical/current large-owner witnesses, all nine unrelated D3D files, original
target, toolchains, Wine, private databases and previous archives are preserved.

Restore archived controls/snapshots from the repository root before using their
historical consumers; append one exact member path to restore a single file:

```sh
tar -xJf .analysis/player-caller-cleanup-20261023/completed-controls.tar.xz -C .
```

The member/protection inventories and cleanup producer are beside that archive.
[Validation](validation-summary.json) states the focused Oracle/check scope.
Full CI is omitted under the explicit operator preference; no semantic or port
gate is opened.
