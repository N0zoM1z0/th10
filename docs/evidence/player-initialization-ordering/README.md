# Player initialization ordering and handoff cleanup

2026-10-23, recovered from source checkpoint07b1be2. The maintained complete
PlayerInitialize4247F0/1435 now matches1170 bytes after replay of49 independently
bound address fields, leaving265 differences. The previous result was1123/312.
No exact ledger, canonical recipe, header or ABI declaration changes.

## Maintained source and evidence

[The patch](maintained.patch) snapshots the update-node flags before its owner
association, uses the existing contiguous fixed-position pair as the history
copy source, and applies three compiler-only read/write barriers to constrain
observed memory ordering. The barriers emit no CPU fence or new external call.
The existing exact reset path already uses this intrinsic. Its presence in the
original source and the original threading model remain unknown.

The complete owner changes284 bytes:68 target repairs and21 target-equal losses,
for47 net repairs. This is a measured compromise, not monotonic improvement or
exactness. [All265 residual differences and49 bindings](parent-differences.json)
are retained. No ordinary instruction or local destination is masked. Semantic
base-plus-addend binding for GameScoreState+28/+2C remains essential.

[Evidence and input pins](evidence.json) identify all measured trials, compiler
profiles, private producers, source hashes and PE/MAP/PDB artifacts. Maintained
and selected private builds have identical inventories and raw bodies for all519
complete public PDB owners, totaling140370 bytes. Static/unowned fragments and
target equality are outside that check. Only private Player is recompiled;
the16 other inputs are reused from the pinned previous maintained17-input graph.
These builds are not independent whole-graph cold pairs.

Eight complete shared guards pass1753 bytes/all99 fields: four Chain owners,
error Log, PreloadAnm, FileSystem::OpenFile and the273-byte VM core initializer.
No new exact credit, product closure or runtime validation is claimed.

## Measured controls and remaining hypotheses

| Trial | Full parent match /1435 | Disposition |
| --- | --- | --- |
| Tail timer barrier | 1147 | 24 repairs, no target-equal losses |
| Both timer barriers | 1160 | Improved net agreement |
| Node publication snapshot with tail barrier | 1148 | Combined later with both barriers |
| Contiguous history pair with both barriers | 1169 | Nine repairs over both barriers, no losses |
| Combined node/history/timer form | 1170 | Maintained candidate |
| Swapped scalar history stores / memcpy history | 1146 /1157 | Rejected alternatives |
| Chain phase snapshots | 1152 | Rejected alternative |
| Typed Chain global / chained glyph dimensions | 1123 /1123 | Whole-parent neutral against07b1be2 |
| Direct Float3 constructor | 1147 | Whole-parent neutral against tail-barrier form |
| VM core as diagnostic entry | 1160 | Rejected: core275 versus target273; only seven guards pass |
| Local history-pair temporary | Not comparable | Full owner1471; strict comparator rejects size |

Equal scores alone do not establish identical bytes. The local-pair trial is
never cropped to1435. Rooting the VM core changes the parent bytes and breaks a
guard; its202-byte wrapper does not close the target. Target review locates the
wrapper's two extra bytes in the receiver bridge, not a fabricated return value.

Next investigate callback setup/VM receiver, history addressing, first data
pointer schedule, one commuted float operand and the late timer/index schedule.
A coherent member-method Chain interface is an **unexecuted** hypothesis. It
would require private declarations, definitions and all17 graph inputs to change
together, new mangled-symbol bindings, and every complete Chain guard to pass.
Do not combine old static-call objects with a new interface or infer the original
ABI from the current byte-exact static wrappers. No such change is maintained.

## Private recovery and space cleanup

Current producers are under
`.analysis/gpt-6.1-sol/player-schedule-20261023`: use manifest-final.json,
common-final.py, probe-maintained.py and bound-parent-final.py for current source.
Historical manifests remain immutable; do not restore old maintained source or
relax their pins. The existing maintained result can be checked without compiling:

```sh
scripts/repo-python .analysis/gpt-6.1-sol/player-schedule-20261023/bound-parent-final.py maintained
```

Its expected result is **mismatch**,1170/1435 with49 fields. The command's
successful execution does not imply exactness. The producer refuses an existing
build directory; use a fresh directory/report label for another compilation.

[The cleanup receipt](cleanup-summary.json) records338 files in13 completed
control builds:24989945 bytes become a6820472-byte lossless private archive,
reclaiming18169473 payload bytes. Every member was read back and checked for exact
length/SHA-256 before removing the expanded copy. All36952 protected files were
hash-unchanged at cleanup, including both retained carriers, other analyses,
canonical outputs, tracked source, original target and the nine unrelated D3D
files. Sources, reports, recipes, Ghidra review and both current build carriers
remain expanded. Toolchains, Wine, private databases and game data are untouched.

Restore all archived controls from the repository root, or append an exact member
path shown by tar -tzf to restore one file:

```sh
tar -xzf .analysis/handoff-player-ordering-20261023/control-builds.tar.gz -C .
```

The compressed member/protection inventories and cleanup producer are beside
that archive. Keep both full carriers under
`build/gpt-6.1-sol/player-schedule-20261023`:
maintained-17 and combined-history-pair-17. Retain the previous player-services
maintained/full-services support caches and all canonical/current large-owner
witnesses. Earlier cleanup archives and restoration instructions are unchanged.

[Validation](validation-summary.json) records completed focused checks and Git
packing. Full CI is omitted under the operator's explicit focused-validation
preference. No new cold game build or target analysis is started for cleanup.
