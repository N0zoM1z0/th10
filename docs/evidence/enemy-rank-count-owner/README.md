# Dispatcher rank-count owner reloads

Target: original Japanese TH10 v1.00a, SHA-256
`2f14760b6fbbf57549541583283badb9a19a4222b90f0a146d5aa17f01dc9040`.
Parent checkpoint: `8ecf0ee`. Reviewed on 2026-10-21.

Two final integer reads in the five-band rank-count case now use a local
EclVmHost view of the loaded full owner before reading activeContext. These
are operand indices 10 and 8, after the corresponding first count has been
stored. The previous local captured only the context after accessing the
partial Enemy context view. Both target-proven pointer loads remain. No API,
header, helper body, compiler switch or requested operation changes.

This ordinary source recovers the target MOV EBX,[EBX+14D8] followed by the
context load into EDX in these two paths. It also restores the lowest rank
path inside this physical case. Target 1AA has eleven generic integer calls
and five returns; the previous candidate had nine calls and four returns,
sharing its lowest path elsewhere. The retained candidate has eleven/five.
The other three target last-read EBX reloads remain unresolved. No source
register binding, padding, artificial return or target bytes are introduced.

Whole-dispatcher direct ReadInt calls increase 103 to 105, against target 106.
The remaining physical call-count deficit is specifically opcode 1B4: target
three calls, candidate two. This identifies unwanted tail sharing in the
difficulty-count case; it does not prove that all other call destinations,
instructions or behavior match.

## Measurements and limits

| Declared eight-TU context | Dispatcher bytes | Rank-five bytes | Absolute case-size gap |
| --- | ---: | ---: | ---: |
| Target | 14416 | 420 | 0 |
| Previous source | 14404 | 369 | 690 |
| Retained owner views | 14468 | 434 | 652 |

All 181 selector bytes and 108 physical groups retain their target mapping.
Forty groups have target sizes. Both frames allocate 2C4 bytes and both contain
19 comparisons with the double -999999 sentinel. The polar interval remains
399/405. Total size proximity is insufficient to select a source model: the
retained change recovers specific target registers and branch placement while
the complete owner remains nonexact.

A native-to-native comparison validates actual image hashes and encoded field
destinations, then additionally binds internal relative branches by physical
case plus destination offset. It finds 101/108 whole physical spans equal
between the old and retained sources. Seven spans differ or retain unbound
destinations: 196, 1AA, 1AB, 1B4, the 156/15C/165/166/167 group, the 19D/1B0
group and the default span. This prevents a claim that only the rank case
changed. The comparison is an owned-byte diagnostic, not target exactness or
runtime equivalence. Numeric data-anchor names are excluded from semantic
symbol binding. Its self-comparison passes all 108 spans; copied-image ordinary
operand and shifted-local-destination controls each fail the affected span,
and stale external-field metadata is rejected. The original target and real
native carriers are unchanged by these controls.

## Cold proof and target review

Two independent full cold links compile EnemyEclDispatcher.cpp followed by
EclVm.cpp, AnmManager.cpp, AnmVmCreate.cpp, EnemyLaser.cpp, Enemy.cpp, AnmVmId.cpp
and RandomMath.cpp. All use `/TP /MT /O2 /Gy /GF /Oi /DNDEBUG /Isrc /GL /GS`,
the real dispatcher entry and the existing anchor harness. All 29 recursive
project inputs agree. The complete native dispatcher is reproducible between
the images after verified field bindings, including all 108 physical spans.

Each image strictly replays nineteen existing complete target extents:
1539 bytes and all 32 fields, including the five operand adapters from the
preceding checkpoint. The second cold replays six canonical units sharing
`build/match-linked/AnmReleaseSlot`; remaining guards explicitly override only
their diagnostic context. Canonical recipes are unchanged. The carrier's old
identity is historical after this rebuild. Run remains 7020 bytes with all
189 fields bound, including 62 local destinations, and 27 ordinary differences
in this graph. The separate two-TU 26-difference measurement remains distinct.
ANM source is unchanged; ExecuteScript is not replayed here.

Fresh direct local Ghidra review verifies the executable and inclusive mapped
text 401000..465A80, 412289 bytes, SHA-256
`cbe16ea35965fd0e6cda23b3680c72e806924c488202f1c9a6602a606f925d2f`.
Read-only/noanalysis review uses rollback, one CPU and 768 MiB. The rank-three
and rank-five range 410858..410A28 contains twelve generic integer calls and
five last-read owner reloads into EBX. Target bytes remain unchanged.
See [native identities and scope](native-summary.json).

```sh
scripts/repo-python scripts/replay-exact-units.py \
  --unit anm-release-slot --unit enemy-read-int-argument \
  --unit enemy-read-float-argument --unit enemy-resolve-float-argument \
  --unit enemy-read-raw-int-argument --unit enemy-read-raw-float-argument --json
```

The generic view does not identify the original class hierarchy, declaration,
production TU or allocated-data aliases. No new exact, source-presence or
authored denominator credit is added. Product, runtime, semantic and portability
gates remain open.

## Rejected controls

The direct integer-read macro's host view and generic instruction-pointer
prologue each preserve the complete native dispatcher after verified field
rewrites; both are restored. A typed pattern pointer introduces a 2C4 bias
absent from the target. Direct low-rank branches and a boolean range predicate
change placement and scheduling without closing the target interval; restored.

A local host for the final difficulty-count read recovers that case's third
generic call, but loses two rank-case calls again: the root has only 104/106.
Putting constant operand calls directly in its difficulty branches also leaves
104 calls and a 369-byte rank case. These controls are rejected. They show
that uniform view substitutions do not monotonically improve the dispatcher.
Private overlays, commands and unresolved reproductions are under
`.analysis/gpt-6.1-sol/enemy-direct-integer-host-20261021/`.

The retained graph still uses a data anchor for AnmInt3View::operator*, although
its maintained body exists in AnmInt3Scale.cpp and the separate RGB recipe
supplies it with an explicit `/Ob1` support profile. This confirms that the
declared eight-TU image is incomplete. An enlarged graph with that real known
support is an untested next hypothesis; neither original production flags nor
unchanged codegen in that context are inferred here.

All 100 standard CI checks, toolchain declarations, tracking, progress and
whitespace checks pass. Final source pins and retained native artifact hashes
still match. Cleanup removes nine superseded native directories and eight
cloned analysis files, reclaiming 9148041 bytes. See
[validation scope](validation-summary.json).
