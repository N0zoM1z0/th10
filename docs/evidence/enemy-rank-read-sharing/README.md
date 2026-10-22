# Enemy middle/zero read-sharing controls

2026-10-22, starting83d828a. Four controls test the preceding14396-byte
scoped-rank/shared-aim candidate's duplicated zero-result tail. None is adopted;
that candidate remains retained, and maintained Enemy stays14336/14416.
No new exact credit or compiler/ABI/recipe change.

## Observations

| Control | Enemy extent | Case matched / comparable | Absolute gap | Rank-five / aim |
| --- | ---: | --- | ---: | --- |
| Common middle/zero read, fresh index local |14380|3266/11131|296|180 /120|
| Same read, existing iVar27 index |14380|3266/11131|296|180 /120|
| Separate middle/zero reads, common fVar21 store |14428|3253/11143|372|241 /120|
| Scoped difficulty only; original rank source retained |14440|3255/11143|384|241 /120|
| Previous combined candidate |14396|3331/11138|298|177 /120|
| Target |14416|—|—|169 /76|

The first two controls share both the read and result sink at source level,
selecting index2 or0 before the common label. Nevertheless, VC7.1 sends the
middle range back into difficulty and puts index3's read/store inside rank-five.
The preceding candidate had recovered target's index3 back-edge and middle-range
fallthrough. The gap drops by two while those branch relationships regress and
the matched-position total falls3331 to3266; this is not an improvement to adopt.
The fresh/existing-index controls have identical raw hashes for all four complete
Enemy/factory/ANM/ECL bodies, not merely equal sizes or aggregate scores.

Sharing only the store after two explicit fVar21 reads enlarges rank-five to241
bytes. Its high index4 path now executes inside rank-five instead of jumping to
difficulty. The difficulty-only isolation control preserves the maintained rank
source and its original high-case labels, but replaces difficulty result scopes
and adds the typed aim pointer. It likewise produces rank-five241. Scoped
difficulty alone therefore does not explain the preceding combined candidate's
better branch shape; rank and difficulty forms interact in this compiler context.

All four preserve target physical order for108 groups and all181 selector bytes,
but all retain the49-byte shared rank-one/aim tail inside the120-byte aim region.
Target has a76-byte aim region jumping into rank-five. Common source labels are
not constraints on the optimizer's final merging or placement. These bounded
observations do not prove which original source form or variable names were used.
Do not replace the preceding candidate merely because the new gap is296 rather
than298. Preserve its high/middle branch topology while investigating other
result-lifetime contexts; the tested index/store-sharing forms are now ruled out
as improvements in the pinned context.

Adjacent-game search was hypothesis-only: named rank-selection and threshold
searches in TH08/TH09/TH095 found no matching five-level selector. The TH08
EnemyManager threshold600 hit belongs to a different state-selection ladder
(600/1600/2400); no TH10 conclusion is promoted from it.

## Reproduction and validation

Private producer, frozen manifest, source overlays, routing disassembly and full
reports live in
`.analysis/gpt-6-astra-medium/enemy-rank-read-sharing-20261022/`.
The previous hybrid input is separately hash-pinned. Patches here are relative
to maintained source at the starting commit. [evidence.json](evidence.json)
records full-body hashes, complete guards and artifact inventories.

Fresh disk/toolchain execution/tracking and direct read-only Ghidra attestations
pass. Target SHA-256 is
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`;
all412289 mapped bytes retain
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
Ghidra uses rollback, readOnly/noanalysis, one CPU and768 MiB heap. Each control
compiles one Enemy object at nice10 and reuses nine verified support objects and
two harness inputs with unchanged GL/GS profiles. All four finish exit0 and pass
36 complete guards4618/all121, including the194-byte ANM direct caller. ECL stays
7020/6993 with its171-byte float callee exact; ANM stays9964 and7541/7995 case
diagnostics. No full parent, whole-product or runtime equality follows.

Maintained inputs, canonical artifacts/recipes, the previous hybrid overlay and
nine unrelated D3D files are hash-unchanged. All four new carriers were verified
and removed:64 files,4270750 bytes; compact reports and reproduction inputs remain.
Full cold replay/full CI was omitted under the operator's focused-replay policy.
Authored409/74716 and overall1252/141647 exact counts remain unchanged. Semantic
and portability work remains unopened.
