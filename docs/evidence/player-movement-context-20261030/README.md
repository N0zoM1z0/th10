# Complete Player movement scope and source recovery — 2026-10-30

**Movement remains nonexact:1616/1656 complete native/target bytes.** This batch
corrects its owned scope, restores byte input/history/VM-ID schedules and fixes
an omitted post-script decision. A zero-reuse seventeen-input cold reproduces
all1616 native bytes,54 resolved fields,78 local branches and both full tables.
New exact credit is **zero**. The [results](results.json) contain target ownership,
full native inventories, source/object pins, controls and scoped cold guards.

## Complete ownership

The old1591-byte row ended at4256E6 RET and omitted one NOP at4256E7 and two
32-byte switch tables at4256E8..425707 /425708..425727. Each table's sole encoded
absolute base reference in the entire file belongs to this function's scaled
indexed JMP. Every one of their16 entries targets a decoded instruction boundary
in this same owner; no direct E8/E9 enters their data. The following Main prefix
also has no direct branch into them. Eight CC bytes separate425730.

The complete reviewed owner is therefore **4250B0..425727 /1656 bytes**:1591 code
bytes (including the retained three-byte LEA), one NOP and64 bytes of generated
dispatch data. The candidate's PDB contribution also owns its two full tables.
These bytes must participate in comparison. This adds65 bytes to the authored
owned denominator without changing origin or adding a source/exact owner.

Fresh target decoding covers1592 prefix bytes/417 instructions,54 address
fields including all16 table entries,80 local branches and one plain RET.
The sole direct caller425A8C places Player in EDI; the body retains that receiver
and returns zero in EAX. Original names, declaration/TU and compiler partition
remain unknown. The target scope is independent of native matching size.

| Table | Target base offset | Native base offset | Entry destinations |
| --- | ---: | ---: | --- |
| Mode1 | 638 | 610 | All eight owner-relative offsets match |
| Mode0 | 658 | 630 | All eight native offsets are one byte early |

The two base offsets remain40 bytes early. A strict complete comparison cannot
normalize these differences or the remaining local code destinations away.

## Retained source and behavior correction

Direction decoding now reads the actual low byte of the existing input storage.
A value cast alone leaves a DWORD load shared with later input decisions; the
byte storage view restores AL/CL/DL operations and fresh later full-input reads.
No volatile qualifier or barrier is added. The named VM ID result restores its
observed stack-home read after AddVmVariant0.

History retains a live optionCount bound instead of caching it, restoring the
per-iteration reads and the target count/cursor arithmetic. History[0] uses the
existing fixed-position pair assignment, restoring X-store/Y-load order. This
uses the same already maintained contiguous pair view as the main owner.

The earlier source incorrectly used one else-if chain for four script selections.
Target +317 calls script2, reloads EBX, then jumps to +326 and rereads Player+44C
before independently deciding script4. Source now preserves this decision after
script execution. The two right-script roles share one ordinary selected-index
call tail. Four logical selections have **three physical target calls**, correcting
the earlier four-call assumption. Grouped independent stop guards preserve the
same native body; the original else-if chain does not preserve this target flow.
This is a target branch obligation, not a claim about a reachable game bug.

The current candidate restores three physical calls and all54 field roles. Its
actual script callee still uses the unlike ESI resource/EDX VM/EAX index interface
and emits197/203; target uses ECX/EAX/EBX. Ordinary constant/register scheduling,
ID reloads, x87 store/reload, option interpolation and fixed destinations also
remain different. Neither field count, matching Mode1 offsets nor size supplies
partial exactness. No fake ABI, padding, target-byte array or new flags are used.

## Bounded controls

| Control | Full native bytes | Result |
| --- | ---: | --- |
| Entry | 1576 | Two complete native tables already owned |
| Direction value cast | 1576 | Byte operations improve, initial DWORD remains |
| Direction byte storage view | 1576 | Actual byte load and later input reads restored |
| Live history / first pair assignment | 1600 | Target history arithmetic/storage recovered |
| Target pair value | 1608 | Changes option cursor anchor; restored |
| Paired products / destination snapshot | 1608 | Reorders products but changes anchor or spills; restored |
| Staged scalar products | 1600 | Whole native body neutral; restored |
| VM-first real-operand helper | 1600 | Helper197 / adapter5; ABI mismatch remains; restored |
| Named mode ID | 1600 | Stack-home read recovered; retained |
| Position copy / constructor | 1620 /1604 | Extra copies or continued extended live value; restored |
| Independent script guards | 1632 | Restores post-script reread; four native calls |
| Grouped zero-speed guards | 1632 | Whole native body neutral |
| Shared right-script tail | 1616 | Three calls/all54 fields; retained, still nonexact |

The first1600-byte cold was completed before the later CFG correction. New source
changes justify the final separate cold; neither replays a historical cohort.
Park these controls until a new observation and prediction distinguish them.

## Reproduction and checkpoint

The final graph retains the existing seventeen-input /GL recipe, real InitializeVm
entry and all pinned per-source profiles. Actual definitions remain visible;
the anchor image is diagnostic and does not establish a runnable product.

    timeout 600 taskset -c 0 nice -n 10 scripts/repo-python \
      scripts/replay-exact-units.py \
      --unit player-calculate-damage-to-target \
      --unit player-auxiliary-damage-at-position --json
    scripts/repo-python docs/evidence/player-movement-context-20261030/verify-native.py \
      --artifact-dir build/match-linked/PlayerCalculateDamage

Only those two existing exact owners are cold-replayed:1372 bytes/all34 fields
pass in one fresh seventeen-input build. The separate verifier checks complete
native receipt identity, including every table destination and all54 fields;
it explicitly grants zero target exact credit. The warm/cold native inventories
are identical across all owned bytes and field identities. Native jobs are
serial, CPU0/nice10, bounded and logged; each cold freezes source/header inputs.

Thirty-four selected exact bodies pass5282 bytes/all168 fields in that cold image. Eleven prior
native candidates, including main3096, state2 786, shots876, initializer callee197
and EnemyRuntimeUpdate2412, preserve bytes/field identities. The unrelated D3D
adapter evidence is untouched. No Ghidra observation, compiler change, Oracle
change, full-cohort replay or runtime/failure simulation is required.

Default seven-stage CI and whitespace checks pass. Exact numerator remains99,996
bytes/491 authored owners; corrected authored owned denominator is275,817.
Product/runtime/portability and original source/type ownership remain independent.
