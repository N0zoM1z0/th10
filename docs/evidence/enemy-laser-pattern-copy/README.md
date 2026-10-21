# Enemy dispatcher laser pattern copies

Two loops that copy laser patterns now use the existing typed request pattern arrays and
`memcpy`. TH10 target 1AF and 1B0 each copy 108 dwords from runtime+2E4 into
request+2C and+48. Both copies still emit REP MOVSD; the unnecessary
source-relative stack-address arithmetic disappears. No request capacity,
argument read, compiler profile, ABI attribute or target byte changes.

The maintained actual 33-input eleven-TU graph emits the complete dispatcher
at 14352/14416 bytes, frame 2C4, 19 sentinel comparisons and all 181 selectors/
108 ordered physical groups. Case-aligned diagnostic agreement rises from
2980/11152 to3219/11148; aggregate absolute span gap falls 650 to 566, while
42 groups remain target-sized. These are diagnostics, with no case/parent
exact credit. The straight-laser path now reaches an independent EnemyFireLaser
call through a shared tail; its earlier type-1 allocation inline is gone.
Its physical interval is 389/439; the call and return now reside in a shared tail.
The current path jumps 40536B->405612 and calls the 153-byte factory at
405618->415630. Request/type remain on the stack with caller cleanup 8;
target ESI-manager/EDI-request/stack-type/RET4 remains unresolved.
The type0 laser-A interval changes 291/327 to 331/327. Neither is exact.

The private copy and actual-source links reproduce all three complete raw
native owner hashes. All 46 actual-context guards retain 5335 bytes/all 142
fields. ANM remains 9964 bytes,7478/7995 case-aligned positions,90/92 target-sized
groups and gap 2. Run compares its whole 7020-byte extent with all 189 fields,
62 local destinations and 88 selector bytes, retaining 6993 matches/27 ordinary
differences and the exact 171-byte float callee. Its generic decoder flag alone
is false; the strict table/selector annotations cover the final undecoded byte.
No ANM or ECL source changes.

One necessary full ten-TU canonical cold replays only its five accepted units.
The same fresh carrier additionally passes 35 contextual whole-owner guards,
4424 bytes/all 118 fields. It is measured separately in native-summary.json.
No historical whole-repository replay is performed. Exact ledgers, recipes,
authored denominator and numerator remain unchanged; the visible SVG remains
26.42% provisional. Only the dispatcher inventory note is refreshed to the
current measurements. Product/runtime/semantic/port gates remain open.

## Rejected executed controls

| Independent source control | Complete dispatcher | Absolute span gap | Result |
| --- | ---: | ---: | --- |
| Explicit block-local aim position pointers |14468/14416|652|Pointer displacement improves; duplicate tail adds 44 bytes; rejected |
| Keep allocated storage identity across initializer calls |14516/14416|628|Factory grows161/153; request remains on stack; rejected |
| Block-local aim through alternate real float-result tail |14476/14416|636|Tail relocation still differs; rejected |

All three retain 46 whole-owner guards and Run's 27 ordinary differences.
The factory control emits 30/137 structural positions versus baseline 16/137,
but still has the wrong private ABI. That score is no justification to adopt it.
No forced inline controls, padding, fake returns or register annotations are used.
Prepared `aim` and `copy-aim` alternatives were not compiled.

Local read-only/noanalysis Ghidra independently verifies the exact executable
and inclusive mapped-text hash before/after rollback with one CPU/768MiB.
The target aim branch jumps to 411956, a 49-byte tail physically in rank5;
cloning that tail in place of the five-byte jump adds 44 bytes.
Per-case span deficits must not be interpreted as omitted behavior.
The target constructors retain the allocated object in EBX and return it in
EAX, but the allocation-identity source model does not recover that convention.
Adjacent TH08/TH095 laser argument structs were inspected only as hypotheses;
no cross-game layout, byte or ownership evidence is adopted.

See [target facts](target-facts.json), [native identities](native-summary.json),
[reconstructible source overlays](source-overlays.json) and
[validation and cleanup](validation-summary.json). Private recovery:
`.analysis/gpt-6.1-sol/enemy-laser-lifetime-20261021/RECOVERY.md`.
