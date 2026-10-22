# ECL Run aggregate lifetime controls

Starting at2dd9776, four actual private-source compiler experiments target the
remaining Run stack homes. None is adopted. The baseline complete7020-byte
Run still has27 ordinary differences after all189 fields are bound; its complete
171-byte ReadFloatValue callee passes. No exact count or acceptance rule changes.

| Source model | Run extent | Entry stack allocation | Result |
| --- | ---: | --- | --- |
| Existing three-member vector in LENGTH_SQUARED |7036|114h|Complete extent rejected |
| Existing three-member vector in POINT_ANGLE |7036|114h|Complete extent rejected |
| New ordinary two-float pair in both operations |7056|118h|Complete extent rejected |
| Unsigned FORMAT argument counter |7020|108h|Same complete27-difference inventory |

The first three preserve service-call ordering and arithmetic expressions while
changing local storage representation. They introduce no padding, assembly,
forced register or profile changes. The first model moves the squared-length
operands from short stack offsets to10Ch/110h; longer encodings and other
allocation changes make it unsuitable. Removing the unused third member does
not restore the target extent in the tested combined model. These observations
are context-specific; they do not prove original scalar types or exclude all
possible aggregates. The unsigned control is only a private source hypothesis;
its malformed/overflow input semantics are not promoted.

Each experiment compiles ECL once, reuses nine hash-verified game objects and
two pinned anchor files, then relinks serially at nice10 with GL/GS retained.
These are not full cold acceptance builds. Each passes nine whole same-image
ECL guards1137 bytes/all18 fields. Other large owners retain extents ANM9964
and Enemy14352; equal sizes do not imply equal bytes. Run's three failed sizes
are not cropped or assigned a comparable-byte score. The unsigned result checks
all7020 bytes,189 fields,62 local destinations, all table/selector data and the
complete171-byte callee; all27 differing offsets and byte values equal the fresh
baseline. The generic normalization flag remains false with the reviewed final
selector byte explicitly covered.

Fresh disk target/toolchain execution/tracking preflight passes. Direct local
Ghidra uses readOnly/noanalysis, transaction rollback, maxcpu1 and768 MiB heap;
all412289 mapped text bytes agree with the canonical disk target before/after.
Only focused checks run; full CI is omitted per the operator's speed preference.
Maintained108 source files, canonical recipes/native baseline and nine unrelated
D3D files are hash-checked unchanged. Toolchain check, tracking, progress and
whitespace pass. [Evidence and input hashes](evidence.json) and four patches
preserve the exact models. No small function is added or promoted.

Hash-verified cleanup removes48 files/3198000 logical bytes in three superseded
native carriers. Retain the7056-byte pair failure, all four private sources,
producer/results and the canonical baseline. Private recovery is under
`.analysis/gpt-6-astra-medium/ecl-aggregate-lifetimes-20261022/RECOVERY.md`.
Next investigate real caller/callee optimizer visibility rather than repeat
these local aggregate or counter forms. No claim that the large owner is
exhausted; whole-product/runtime and semantic/port gates remain open.
