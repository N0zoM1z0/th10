# ECL value lifetimes and processor-profile controls

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Parent checkpoint: `6a2bcf5`. Reviewed on 2026-10-21.

Eight private controls produce no source, profile, recipe or ledger adoption.
Current complete ECL Run remains **7020 bytes/all189 fields/62 locals, with27
ordinary differences**. All three original large owners remain active/nonexact.
The accepted death-finalizer proof and its current carrier are unchanged.

## Controlled compiler observations

Each probe uses the current ten-TU dispatcher graph, changes only the ECL
support object, and reuses nine hash-verified objects. Builds are serialized
at nice10. /GL and /GS remain enabled; the separate scale support retains /Ob1.
The private diagnostic adapters substitute only the ECL source/PDB pathname;
canonical target bindings, tables and the171-byte float-value guard stay fixed.
Every control independently retains **32 complete helper owners3874/all105**,
including the complete96-byte Push and122-byte Pop. These are comparisons in
the declared private context, not additional canonical exact credit.

| Control | Complete Run result |
| --- | --- |
| PushFloat takes `const float &` instead of `float` | All bytes equal baseline |
| Pop retains aliases with const byte pointees | All bytes equal baseline |
| Comparison result uses existing EclVmScalar integer member | All bytes equal baseline |
| Comparison declarations move before the loop | All bytes equal baseline |
| ECL support alone adds /G7 | All bytes equal baseline |
| ECL support alone adds /G6 | All bytes equal baseline |
| Pop directly addresses data instead of local byte aliases | Unsupported field binding at7020 bytes |
| Boolean comparison word changes from signed to unsigned int | Unsupported complete extent6876/7020 |

For the six neutral controls, all bytes of complete native ANM9964,
Run7020 and dispatcher14468 equal their baseline bodies. All7020 Run bytes
also equal baseline after verified target field rewrites, SHA-256
`830db1c50fc4f1228d0bfbf99f08c51e9d9e36043990d2900820c8b55600a472`.
This establishes more than an unchanged mismatch count. The /G6 and /G7
controls compile identical source and capture no ignored-option diagnostic;
their neutrality is observed only for this graph/profile.

Direct Pop addressing preserves member-index rereads and operation order, but
moves a late FORMAT ReadIntValue call field to6524/0x197C. The target template
cannot bind that position, so the strict diagnostic rejects it before any
ordinary-byte score. The unsigned word still receives Boolean0/1, but passing
it to PushInt(const int&) introduces an implicit signed temporary; the full
Run contribution becomes6876. It cannot be cropped or scored as7020. Both
controls retain all32 helper bodies. Exact standalone helpers therefore do
not determine byte identity of their inlined caller.

These are bounded compiler observations, not recovered original declarations,
types or optimization policy. The [native summary](native-summary.json)
records variant definitions, all31 current raw input hashes, actual profiles,
producer/adapter hashes, guard inventory, complete-body identities and all27
current residual positions with target opcode groups. Older two-TU allocator
color weights have not been remeasured in this ten-TU graph; these controls
do not establish the allocator cause of the remaining differences.

## Fresh target review

Direct local Ghidra runs readOnly/noanalysis, rollback, CPU1 and heap768 MiB.
Disk target identity and full mapped text agree before/after: inclusive
401000..465A80,412289 bytes, SHA-256
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
SINE and COS merge at44F934 into one float store and a retained Push4505B0
call at44F941. The residual1710 belongs to SINE, not COS.

The current Ghidra database has no function/instruction record for450610 Pop.
That metadata gap does not override its existing122-byte ledger extent or
the independent qualified PDB/target-byte comparison. No boundary is changed.

## Reproduction and scope

Rebuild the declared graph when its pinned inputs change. With the current
hash-pinned carrier, the focused complete diagnostic is:

```sh
scripts/repo-python scripts/diagnose-ecl-run-native.py \
  --unit enemy-finalize-death --artifact-dir build/match-linked/EnemyItemDrops
```

Expected exit1 means27 ordinary differences; it grants no exact credit.
The private controls, source copies, strict reports and recovery note are in
`.analysis/gpt-6.1-sol/ecl-push-value-lifetimes-20261021/`.
Six inspected terminal duplicate build directories are pruned/6241608 bytes;
both unsupported controls, accepted independent colds and all source/producer/
comparison receipts remain. Independent untracked D3D work is untouched.
The [validation receipt](validation-summary.json) records final gates and scope.
No original/native-product runtime, semantic or portability claim follows.
