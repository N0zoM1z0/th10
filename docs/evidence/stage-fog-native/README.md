# Complete stage Fog and object-VM native closure (2026-10-19)

Five maintained owners now close **533 authored exact bytes**, with all17
address fields independently target bound. Two independent cold VC7.1 SP1
CL13.10.6030 / LINK7.10.6030 links pass every complete byte and field. The new
canonical units also pass at their declared default output directory.
No source, boundary, origin or source-presence change is needed.

| Target | Owner | Bytes | Fields | Canonical unit |
| --- | --- | ---: | ---: | --- |
| 0x405040 | Six-float Fog constructor | 94 | 4 | stage-fog-constructor |
| 0x404D40 | Fog subtraction | 92 | 4 | stage-fog-subtract |
| 0x404E10 | Fog addition | 92 | 4 | stage-fog-add |
| 0x404DA0 | Fog scalar multiplication | 101 | 4 | stage-fog-scale |
| 0x403990 | UpdateObjectVms | 154 | 1 | stage-update-object-vms |

The older structural-match observations did not grant these functions exact
credit. This checkpoint adds complete target-field recipes, repeatable strict
replays and canonical ledger entries. It does not grant credit to the larger
Fog evaluator or stage callback/dispatcher.

## Target and physical ownership

Read-only hash-attested local Ghidra, pinned Capstone decoding and target bytes
agree on the complete Fog extents. UpdateObjectVms has a148-byte reachable
Ghidra body, with a six-byte internal compiler-alignment gap at0x4039AA; its
already reviewed physical extent is154 bytes through RET at0x403A29. Every
physical byte, including alignment, is compared. No padding is added to source,
and no boundary or authored denominator is changed.

`target-facts.json` records complete target code hashes, the17 independently
decoded call operands, private interfaces and mapped `.text` attestation.
All16 conversion calls bind `__ftol2 @ 0x463B2C`; the updater's one call binds
ANM ExecuteScript at0x43EE30. These fields are exhaustively checked by the
strict linked comparator, rather than accepted from a similarity mask.

The constructor takes six float32 stack values and ESI storage, returning
that storage in EAX with RET24. Addition/subtraction use ECX/EAX operands and
ESI hidden result storage. Multiplication uses EAX input, ESI result storage
and one stack float with RET4. The object updater receives the stage in EBX
and returns zero with plain RET. Natural maintained C++ produces these private
interfaces; no ABI annotations, byte copying, dummy operands or forced returns
are introduced. Original class identifiers and source/TU ownership remain unknown.

Fog consists of six float channels plus derived packed BGRA at+0x18. The
arithmetic stores channels with FST while retaining live x87 color results
for truncation and low-byte packing without saturation. Complete native-byte
equality preserves that sequence; it does not make the ordinary host fixtures
an x87 or invalid-conversion Oracle. The six-float constructor is selected by
its full symbol, separately from the one-byte empty default constructor.

The object updater retains signed counts and primitive sizes, skips inactive
objects and checks each VM's instruction pointer after execution. Its zero
return and internal loop alignment are reproduced across both cold links.
Host contract tests remain bounded synthetic fixtures, not game runtime proof.

## Reproducible compiler context

Both cold links use primary `src/StageScript.cpp`, real entry
`?Update@StageScriptView@@QAEHXZ`, and support sources in this order:
AnmManager.cpp, AnmVmCreate.cpp, AnmVmId.cpp, RandomMath.cpp. All five inputs use
`/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL /GS`. Thirteen project CPP/quoted
header hashes agree across the links. The first observer's constructor-overload
ambiguity was repaired by reading the completed PE/MAP/PDB; no build was repeated
to repair observation. Both images have distinct PE hashes/PDB identities.

`native-summary.json` pins both complete strict results, inputs, tools, graph,
whole-owner hashes and the canonical default replay. This declares a bounded
compiler context, not the original production partition. Remaining unresolved
symbols use diagnostic data anchors, so neither executable is run.

One build covers all five canonical units:

```sh
scripts/repo-python scripts/build-match-unit.py --unit stage-fog-constructor
for unit in stage-fog-constructor stage-fog-subtract stage-fog-add stage-fog-scale stage-update-object-vms; do
    scripts/repo-python scripts/compare-linked-function.py --unit "$unit" --json || exit 1
done
```

Only these five new units are replayed; no historical full native cohort is
rerun. Existing bounded Fog and stage host fixtures run through public CI.

## Remaining work

The current same-graph Fog evaluator remains664/688 bytes, despite the matching
0xD4 frame and14 direct-call identities/order. Its scheduling and temporary
lifetimes remain open. Target modes7/17 copy initial into seven-word temporary
storage before addition; this candidate passes initial directly. Target easing
uses EAX mode and RET8, while the current ordinary-cdecl source produces an
ADD ESP8 at the evaluator call. These are the next falsifiable lifetime/ABI
controls, not a proved source spelling or standard fastcall interface.
Stage Update remains294/304. The stage dispatcher is
a1972-byte candidate contribution against the reviewed1806 code bytes; its
separately provisional table ownership is unchanged. These are scoped
diagnostics, not exact ratios or partial-case credits.

StageScript now has5 exact owners and6 source-present nonexact owners. Native
product, runtime, overlapping published-camera storage, dual-Oracle semantic
and portability closure remain open. Toolchain/tracking, generated progress,
public CI and whitespace checks pass. The duplicate cold image is removed;
one canonical image remains for the unresolved evaluator.
