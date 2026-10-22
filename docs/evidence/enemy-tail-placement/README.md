# Enemy float-tail placement and source case order

2026-10-22, startingabdbffe. Four controls extend the preceding typed aim-pointer
experiment. No maintained source or recipe is changed and no new exact credit
is claimed. This batch establishes that source case order can change shared-tail
ownership even though swapping the two tail labels cannot.

## Results

Every control includes the second drawPosition pointer expression that restored
ADD EDI,3C0 and [EDI+4] in the preceding experiment. Its reference Enemy extent
is14340, with aim120/rank-five125, versus target76/169.

| Additional control | Enemy extent | Aim / rank-five | Result |
| --- | ---: | --- | --- |
| Swap the two read/store tail blocks |14340|120 /125|All four full bodies raw-equal to pointer-only reference|
| Dedicated result variables across difficulty/rank/aim |14340|120 /125|Changes stack homes; case score3261 to3255, gap332 unchanged|
| Move aim case before rank-three case |14308|76 /172|Aim jumps into rank-five tail; physical case order differs|
| Dedicated results, separate block-local aim Y |14340|120 /125|All four full bodies raw-equal to dedicated-result control|

The reordered control emits aim at4058FB, ending at405942 with JMP4059E0.
Destination4059E0 lies inside rank-five405965..405A10, and is the49-byte
owner-reload/store/return tail. Target aim411C5A ends with JMP411956 into its
rank-five region. Thus the requested sharing shape can be produced without
padding, fake branches, target bytes, ABI changes or compiler flag changes.
It is still not an exact region: register selection, stack homes, scheduling
and rank selection branches differ. Equal aim length alone is not proof.

Moving the source case also moves its machine-code body before rank-three.
The first physical-order mismatch is ordinal95; selectors still agree for all181
bytes. The normal target-order case diagnostic deliberately stops there with
an assertion (terminal exit1), after complete guards and owner/ANM/ECL reports.
Do not use a negative or overlapping target-order extent, or an aggregate score
built from it. Independent inspection sorts the candidate's actual case-entry
addresses to establish the reported76/172 extents. The failed order check is
recorded explicitly in [evidence.json](evidence.json).

Target rank-five instead branches high values back to difficulty entries and
retains both result tails in its169-byte region. The reordered control still
keeps high selections locally and sends the middle selection to difficulty.
Next work should address the joint case layout and branch-sharing graph; merely
moving a source case or its two result labels is insufficient. The dedicated
result control changes aim's spill home from ESP+C to ESP+18; target uses ESP+10.
Splitting aim Y from the other result variable leaves all four complete bodies
unchanged in this context. These observations do not identify original source.

## Validation and retention

Fresh target/toolchain execution/tracking preflight and direct read-only Ghidra
review passed. Original Japanese v1.00a SHA-256:
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
All412289 mapped target bytes retain
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`;
Ghidra uses rollback, readOnly/noanalysis, one CPU and768 MiB heap.

Each control compiles one Enemy object at nice10, reusing nine hash-verified
support objects and two harness inputs. GL/GS and other source profiles remain
unchanged. All four pass36 complete guards4618/all121, including the194-byte ANM
caller. ECL remains7020/6993 with its complete171-byte float callee exact; ANM
remains9964 and7541/7995 case diagnostics. Size/diagnostic equality is not raw
identity; full-body hash observations are recorded separately.

Private producers, frozen manifest, source overlays, logs and routing review:
`.analysis/gpt-6-astra-medium/enemy-tail-placement-20261022/`.
Patches here apply to the starting source. Maintained sources, canonical build
artifacts/recipes and nine unrelated D3D files were hash-verified unchanged.
Retain the reordered carrier and the previous pointer-only carrier for further
layout work. Three redundant carriers were individually verified and removed:
48 files,3199912 bytes. Full cold replay/full CI was not run; focused guards and
tracking/toolchain/document checks cover this unadopted experiment checkpoint.
Maintained Enemy14336/14416, ANM and ECL exact status, authored409/74716 and overall
1252/141647 remain unchanged. Whole-product/runtime and semantic/port gates stay
open.
