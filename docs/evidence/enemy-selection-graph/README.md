# Enemy selection graph without moving case labels

2026-10-22, starting6ecb5a6. Four complete-dispatcher controls retain all108
physical case groups and all181 selector bytes in target order. None is adopted;
maintained Enemy remains14336/14416 and3245/11128 case-aligned positions, gap328.
The combined scoped-rank/shared-aim candidate is retained for further work.

## Controls

| Source control | Enemy bytes | Case score | Gap / target-sized groups | Rank-five / aim bytes |
| --- | ---: | --- | --- | --- |
| Ascending rank thresholds, typed aim pointer |14340|3267/11135|334 /46|128 /120|
| Branch-local read/resolve/store in difficulty/rank/aim |14412|3292/11135|326 /45|163 /132|
| Same scopes, preserving two owner/context access forms |14412|3286/11135|326 /45|163 /132|
| Scoped difficulty/rank, shared rank-one/aim result |14396|3331/11138|298 /46|177 /120|
| Target |14416|—|—|169 /76|

All threshold choices preserve the maintained selection behavior. Direct target
byte decoding at4118BB..4118E1 additionally confirms that rank-three uses index2
above/equal512 and index0 for both other paths, with separate result-tail
branches. The repeated zero is target evidence, not a typo to fix by analogy.
The scoped controls retain those source conditions, but compilation collapses
the equivalent low paths: rank-three36 versus target39 and baseline45.

Ascending thresholds alone fail to recover target sharing; their apparent extra
target-sized group is the default region, while the difficulty and spell-related
groups also change. In both fully scoped controls, rank-five high selections
jump back to difficulty, and results spill to ESP+10, matching those aspects of
target. However aim is132 bytes and evaluates FPATAN after ResolveFloat. MAP
binds that call to ?ResolveFloat@EclVmContext@@QAEPAMI@Z at406640. Target computes
FPATAN before its jump into the result-resolution tail. Thus14412, only four
bytes short, is not a near-exactness claim. Preserving the manual owner-access
form does not repair this instruction ordering.

The combined control keeps scoped difficulty/rank reads, but restores the
existing hoisted aim result and explicit shared rank-one/aim tail. It recovers
FPATAN before resolution and the typed position-pointer access. Rank-five gains
both high-value JGE branches to difficulty and its middle-case fallthrough with
JL to the low group, as in target. Its case score rises7 to79; parent case gap
falls328 to298 and score3245/11128 to3331/11138. These are diagnostics only.

Remaining concrete mismatches prevent adoption: rank zero has a separate
49-byte result tail instead of reusing the middle read/store path, rank one jumps
to the tail in aim, and aim still contains that tail. Target instead keeps both
tails in rank-five and aim jumps back to one of them. Rank-three's redundant
branch has also collapsed, and several unrelated cases lose matched positions
from changed stack allocation (all changes are inventoried in evidence.json).
Next test: share the middle/zero read in this combined context while preserving
the now-correct high/middle branch orientation; do not simply move case labels.

## Reproduction and checks

Private producers, frozen manifest, reports, routing disassembly and overlays:
`.analysis/gpt-6-astra-medium/enemy-selection-graph-20261022/`.
The four patches apply to the starting source; [evidence.json](evidence.json)
records complete-body hashes, guard results, changed cases and artifact hashes.
No generated decompiler output or executable is committed.

Fresh target/toolchain execution/tracking and read-only Ghidra attestation pass.
Original Japanese v1.00a target SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
All412289 mapped bytes retain
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
Ghidra is readOnly/noanalysis, transaction rollback, one CPU and768 MiB heap.
Each control compiles one Enemy object serially at nice10, reuses nine verified
support objects and two harness inputs, and preserves GL/GS profiles. All four
finish exit0 and pass36 complete guards4618/all121, including the194-byte ANM
caller. ECL remains7020/6993 with the171-byte float callee exact; ANM remains9964
and7541/7995 case diagnostics. This does not establish runtime/product closure.

Maintained sources, canonical artifacts/recipes and nine unrelated D3D files
were hash-verified unchanged. Retain the combined candidate; three other carriers
were hash-verified and removed:48 files,3203320 bytes. Compact inputs remain.
Focused checks replace full cold replay/full CI under operator instructions.
Authored409/74716 and overall1252/141647 exact counts remain unchanged; semantic
and portability gates remain closed.
