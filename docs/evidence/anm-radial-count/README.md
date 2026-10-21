# ANM radial counter scheduling

The maintained render-mode9 block now declares its seven real locals together,
snapshots the original vertex count, and uses count-1 for the coefficients and
loop bound. Its ordinary counter setup now matches TH10: LEA ESI,[EAX-1], TEST
ESI, integer spill and FILD before coefficient division. The late MOV ESI,EAX
is gone. TH10's existing geometry, calls, two closing V writes and all surrounding
code remain. No layout, signature, ABI attribute, compiler profile, inline
control, dummy operation or source padding is added.

The actual33-input eleven-TU source link reproduces every raw byte of the
private positive's three complete large owners. Both that graph and one fresh
full32-input canonical ten-TU cold retain ExecuteScript9964 bytes,90/92 target-
sized groups/gap2; positional diagnostic agreement improves7478->7541/7995.
The recovered counter is a bounded instruction observation; neither the case
nor whole function is exact. Remaining ordinary common-interval differences
are compiler loop alignment and slow Tick operand order. Child91, FlipX61,
float-variable index temporaries49/53/54 and common-tail2 remain nonexact.

Current actual-context46 whole-owner guards retain5335 bytes/all142 fields.
The canonical cold replays its five accepted units, and the same fresh image
passes35 whole-owner guards4424/all118. Complete Run still compares6993/7020
with all189 fields/62 locals and the exact171-byte float callee; Enemy stays
14352/14416, frame2C4/19sentinels/42 equal-sized groups/gap566. Run and Enemy
actual native bodies are raw-unchanged by the positive. No new exact bytes,
canonical recipes or inventory-state counts are added; SVG stays26.42% provisional.

## Reviewed host dependency pin

The first CI invocation stops at TransitionEffect's strict whole-file LF pin
for AnmManager.cpp. Its actual extracted AnmRandomU32InRange body remains
LF-byte-identical (SHA2564c05abf3c1f5e0d169264b10f8de0a916d49144c1dc077d7827f26797149fbd1);
the other six host dependencies retain their existing strict pins. One additional
canonical two-TU cold replays that complete88-byte range helper raw-equal with
zero fields. Only the AnmManager.cpp LF pin and reviewed migration metadata
change; normalization/rejection policy and every fixture assertion/adaptation
remain. Targeted host baseline843 assertions and all19 EOL/pin controls pass.
This is host-only regression evidence; no semantic or product gate opens.

## Executed source models

| Independent model | ANM aligned positions | Outcome |
| --- | ---: | --- |
| Preserve vertex count and start index at1 |7475/7995|Counter allocation not recovered; rejected |
| Group declarations, use original count-1 bounds and short angular-step name |7541/7995|Counter setup recovered without other ordinary geometry regressions; adopted |
| Above plus split raw flag writes |7540/7995|Counter remains correct and Tick order matches, but radial Z operands reverse; rejected |

All three private controls compile only AnmManager and reuse ten verified game
objects plus unchanged diagnostic anchors; none is an independent full cold.
They retain all46 helper guards and complete native Run/Enemy identities.
A prepared repeated-expression/one-based variant was not compiled.
Declaration order, local names and loop expressions changed together: this
experiment does not uniquely identify which source detail controls allocation.
The flag combination is retained for the observed nonlocal scheduling relationship.

TH095 AnmManager supplied a source-shape hypothesis after TH10's actual counter
instructions were reviewed. Its names, fields and bytes
are not TH10 evidence. The declaration/loop shape suggested the control;
TH095's closing-V quirk and position callback are not imported. The TH10 compiler/target
comparison alone supports the adopted setup. No original source spelling,
class/TU/profile or data-owner claim follows.

The actual-source absolute-memory audit still validates45 ordered references
inside92 physical spans:21 literal references in read-only rdata match payloads,
9 globals retain canonical bindings, and15 vector fields retain reviewed
491C14/491E6C bases plus addends despite ten diagnostic-anchor name collisions.
This excludes prologue/immediate addresses/calls/tables/indexed memory and is
not exhaustive function normalization or runtime validation. Earlier source pins
are historical after this AnmManager.cpp change. Product/semantic/port gates
remain open; all original three large functions remain nonexact.

Reproduce the necessary canonical artifact with:

```sh
scripts/repo-python scripts/replay-exact-units.py --unit enemy-drop-item-counts --unit enemy-drop-item-counts-core --unit enemy-finalize-death --unit enemy-cancel-bullet-record --unit enemy-cancel-bullet-pattern --json
scripts/repo-python scripts/diagnose-ecl-run-native.py --unit enemy-cancel-bullet-pattern --artifact-dir build/match-linked/EnemyItemDrops
```

See [target facts](target-facts.json), [native identities](native-summary.json),
[ordered source overlays](source-overlays.json) and [validation/cleanup](validation-summary.json).
Private recovery: `.analysis/gpt-6.1-sol/anm-vertex-count-20261021/RECOVERY.md`.

The operator then requested handoff. [Cleanup and retained state](handoff-cleanup.json)
record the documentation consolidation and removal of disposable outputs;
source and retained native identities remain unchanged after the passing CI.
