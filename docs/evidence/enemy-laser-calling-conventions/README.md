# Laser factory parameter lowering after genuine construction

Date: 2026-10-22. Starting pushed commit `3e6cb46`.
Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.

Three actual private source controls retain the newly accepted genuine C++
construction context. None reproduces the factory153/RET4 contract, and none
is adopted. Maintained source, headers, profiles, recipes and matches remain
unchanged. Overall1252/141647 and authored409/74716 remain independent totals.
[Facts](facts.json), [our source patches](source-controls.json) and
[focused validation](validation-summary.json) retain the evidence.

## Actual controls

Each control freshly compiles all ten game-source TUs, serial nice10, using a
consistent32-file source/header overlay and the existing GL/GS profiles/scale
Ob1 override. All ten real factory call sites are represented. No factory name
may be supplied by an unresolved diagnostic anchor. Both pinned harness files
are reused; each first link succeeds with zero new anchors. These are distinct
diagnostic source hypotheses, not an acceptance cold pair.

| Source shape | Changed source/header roles | Factory | Enemy bytes | Case alignment / aggregate gap |
| --- | ---: | --- | ---: | --- |
| Free stdcall(manager, request, type) | 2 | 157 / RET8 | 14344 | 3225/11145 /562 |
| Free fastcall(manager, type, request) | 3 | 157 / RET8 | 14344 | 3221/11145 /566 |
| Ordinary manager member(type, request) | 3 | 157 / RET8 | 14344 | 3221/11145 /566 |

The latter two convert all ten calls and reorder both declarations/definition
consistently. The member form has no free-function wrapper or method stub.
TH095's provisional Spawn(type,args) suggests only that source shape; its
addresses, semantics and forced-inline/compiler-storage helpers are not copied.
TH09's reviewed pool-based laser spawn is not a matching factory analogue.
Every TH10 observation below comes from TH10-local code and compiler results.

Reordering puts type at the target's entry stack+4 slot. Request remains a
late four-byte MOV ECX,[ESP+C], followed by PUSH ECX; target instead uses
PUSH EDI. Both exits still RET8 rather than targetRET4. The strict factory
comparator rejects the complete157-byte PDB extent against153. No cropping,
field omission or partial equality claim is used.

Fastcall/type-first and ordinary member/type-first produce four complete
raw-identical native bodies: Enemy14344, factory157, ANM9964 and ECL7020.
This falsifies an effect from membership alone in these two measured contexts;
it proves neither original membership nor a universal source-ABI exclusion.

## Complete focused checks

Each image passes36 complete guards4618/all121 and all three complete natural
constructors98/202/219, two fields each:39 functions5137/all127 total. Constructor
checks grant no duplicate credit. Five separate canonical normal-COFF contracts
among the36 remain extra linked checks, without changing their artifact kind.

ANM retains7541/7995,90/92 target-sized groups, gap2. ECL retains7020/6993/27,
all189 fields/62 local destinations and its complete171-byte float callee.
Enemy retains all181 selectors and108 ordered physical groups; the table scores
above are navigation diagnostics without case or parent exact credit.

The raw producer summaries inherited a hardcoded harness_regenerated=true flag.
Actual attempt logs, zero added symbols and unchanged harness hashes establish
two harness reuses and no regeneration. The facts packet reports that measured
scope rather than repeating the stale flag.

All373 source/test/config inputs,108 maintained sources,32 current large-context
sources/headers,15 canonical native files and9 unrelated D3D files stay unchanged.
The source-identical3e6cb46 already passed111 CI groups. Per the operator's newer
speed instruction, this documentation checkpoint runs focused comparisons and
necessary tracking/toolchain/progress/documentation checks; full CI is not
repeated. Whole Windows i386 product/runtime remains open; semantic and port
stages remain unstarted.

Hash-checked cleanup removes50 owned superseded files/3213746 logical bytes.
Retain the latest member157 carrier/consistent overlay, compact producers,
source patches and results, accepted constructor cold1/current cold2 and the old
baseline15/32. This supersedes the previous packet's retention of the older
prefix-fast carrier. Private recovery remains
`.analysis/gpt-6.1-sol/enemy-construction-context-20261022/RECOVERY.md`.

Next investigate request liveness and actual callback/caller visibility. The
ten direct calls are present; unknown production owners or missing callback
definitions are hypotheses to examine, not established causes of the mismatch.
