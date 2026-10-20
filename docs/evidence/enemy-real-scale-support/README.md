# Real ANM scale support in the dispatcher graph

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Parent checkpoint: `f498991`. Reviewed on 2026-10-21.

The six canonical recipes sharing the dispatcher carrier now compile the
maintained AnmInt3Scale.cpp body. The previous eight-TU graph resolved its
operator symbol to a data anchor. The new nine-TU graph removes that anchor
and reproduces both the complete 56-byte scale operator and 820-byte RGB
Evaluate owner. RGB Evaluate was only 780 bytes in the old graph.

Only this added support uses the existing reproduced `/GL /GS /Ob1` profile
from the separate RGB recipe. The primary and other supports retain their
profiles. No C++ source, source API, target boundary, origin or semantic
symbol-to-target binding changes. This is a declared compiler-context repair;
it does not identify original production flags or physical TUs.

## Complete replay

Two independent full cold links compile EnemyEclDispatcher.cpp followed by
EclVm.cpp, AnmManager.cpp, AnmVmCreate.cpp, EnemyLaser.cpp, Enemy.cpp, AnmVmId.cpp,
RandomMath.cpp and AnmInt3Scale.cpp. Both select the real dispatcher entry.
All 30 recursive project inputs agree. Both strictly reproduce 21 existing
complete target extents, 2415 bytes and all 52 fields: the former nineteen
guards plus scale56/all3 and RGB820/all17. No new exact credit is counted.
The second cold replays the six migrated canonical units, 142 bytes total,
in `build/match-linked/AnmReleaseSlot`. The other canonical contexts, including
the separate RGB/scale recipes, are unchanged.

Fresh local Ghidra review attests the disk target and all mapped text at
401000..465A80 inclusive: 412289 bytes, SHA-256
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
Read-only/noanalysis uses a rolled-back transaction, one CPU and 768 MiB.
The target scale helper has four direct calls, all from RGB Evaluate at441600.
The new operator is selected through its real source-owned PE/PDB contribution.
The operator's former unresolved-symbol alias is absent from both new links.

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit anm-release-slot --unit enemy-read-int-argument \
  --unit enemy-read-float-argument --unit enemy-resolve-float-argument \
  --unit enemy-read-raw-int-argument --unit enemy-read-raw-float-argument --json
```

## Three large owners

| Diagnostic | Eight-TU anchor baseline | Nine-TU real scale support | Target |
| --- | ---: | ---: | ---: |
| ANM ExecuteScript bytes | 9964 | 9964 | 9964 |
| ANM aligned ordinary positions | 7250/7995 | 7478/7995 | diagnostic only |
| RGB Evaluate bytes | 780 | 820, complete exact | 820 |
| ECL Run bytes and ordinary differences | 7020,27 | 7020,27 | 7020,0 |
| Enemy dispatcher bytes | 14468 | 14468 | 14416 |

The added real callee improves 228 aligned ANM diagnostic positions while
total interpreter size remains unchanged. Ninety of 92 spans have target sizes;
absolute span-size gap remains two. This illustrates why size alone cannot
establish unchanged code generation. Full ExecuteScript remains nonexact.

ECL Run retains all 189 bound fields, including 62 local destinations, and
6993/7020 full strict bytes. The separate two-TU 26-difference result remains
a different context. Dispatcher retains 40/108 target-sized spans, absolute
case-size gap652, frame2C4, nineteen sentinel comparisons and polar399/405.
Its 107 equal native-to-native case spans leave one unbound static constructor
call field; that map entry names EnemyInitializeLaserType1 in EnemyLaser.cpp.
This diagnostic does not grant target equality or resolve its lifecycle body.

Both nine-TU images have equal complete native owned bytes after verified
field bindings for ExecuteScript, Run and the dispatcher. All 108 dispatcher
spans agree between them. Their PE/map/PDB identities, source pins and scoped
receipts are recorded in [native identities](native-summary.json).

## Explicit ECL diagnostic contexts

`diagnose-ecl-run-native.py --context-manifest PATH --unit NAME` can now select
a separately declared build graph. The context supplies source/support lists,
profiles, harness, entry and artifact directory. Callee templates and semantic
bindings always come from canonical match-units.toml. Both input files are
hashed and must remain unchanged during the comparison. The command still
requires the complete exact 171-byte float-value callee, the 7020-byte Run
owner, all 189 fields/62 local destinations, and complete table/selector
coverage. It remains noncanonical and does not attest how an artifact was built;
producer input pins must be recorded separately.

The old command's complete Run/guard reports are unchanged. An explicit old
context gives the same reports; the new context retains the same 27 differences.
A wrong target hash is rejected. Forged float-value and host-run templates in
the context file are ignored, with canonical guard/Run reports unchanged.
The optional context file cannot redefine those target bindings.

No full historical replay is run. Existing exact counts remain1216 overall,
375 authored/68104 bytes. Other data anchors remain, so neither diagnostic
image is a runnable product. Original declarations, data ownership, all three
large exact claims, runtime, semantic and portability gates remain open.
Private reproducible controls and reports are under
`.analysis/gpt-6.1-sol/enemy-real-scale-support-20261021/`.

All 100 standard CI steps, final toolchain/tracking/progress checks and
whitespace checks pass. Only the six affected canonical recipes are replayed.
Two verified duplicate CLI reports are removed after recording their identities
and core equality, reclaiming 143018 bytes; their inputs and compact controls
remain. Both cold carriers and the old eight-TU baseline are retained.
See the [validation and cleanup receipt](validation-summary.json).
