# Complete Enemy operand readers — 2026-10-30

`EnemyFullObjectView::ReadIntOperand` at 411FC0..4122F3 now reproduces its
complete **820-byte** owner with all **95** linkage fields. The 616-byte code
and complete 204-byte, 51-entry table are compared together. Two independent
canonical 21-input cold builds reproduce zero differences. The adjacent
884-byte float reader remains **NONEXACT**, with a 876-byte candidate.

## Complete target and source review

The integer owner contains 142 instructions, 41 RET4 exits, 28 direct calls,
one direct local branch and one indexed dispatch. The selector is advanced by
10000, compared unsigned with 50 and dispatched through 412228. Every one of
the 51 table destinations is an instruction start in this owner, including the
shared interior world-Y load. The table has one encoded base reference in the
executable. Twelve CC bytes precede the independent 412300 resolver. All 95
encoded operands are enumerated independently from retail instructions and
table words; 52 are fixed local destinations (base plus 51 entries), and 43
bind external identities. No boundary or denominator change is needed.

The existing source grouped both integer player-coordinate selector families
into common case labels. Target has two separate X/Y bodies. The maintained
switch now retains those separate source cases and follows the recovered
random/player, timer/variables, motion and environment case grouping. Shared
motion aliases remain shared. World velocity angle uses the existing natural
atan2 expression. Every selector/default and every table word remains present.
The numeric selector names are provisional; byte reproduction does not recover
their original names, protocol meanings or original source order.

The integer VM case formerly called an unresolved cdecl
EnemyResolveManagedVmId shim. Target calls the existing 449450 GetVm body with
ESI pointing directly to the stored ID. Source now invokes AnmVmIdView::GetVm
on that same storage and reads the existing signed value38A layout view. It
constructs no temporary ID and changes no shared header or ABI declaration.
The real AnmVmId/AnmManager definitions in the declared graph derive the target
interface naturally. Their complete 21-byte GetVm body also passes in both
images. The unused shim declaration is removed.

RNG calls keep ordinary ECX receivers and bind the observed 44B9E0/44BB20/
44BB90 entries and global4918B0. Their provisional EnemyRandomView names are
descriptive source aliases. The anchor image is not runnable, and this owner
does not claim RNG source ownership or production linkage. All 24 __ftol2
calls bind 463B2C, and every player/manager/rank/difficulty field is explicit.

## Float reader and restored controls

The complete 412350..4126C3 float target includes 679 executable bytes, one
alignment byte and 51 table words at4125F8. Dense review consumes 680 bytes /
152 instructions including the NOP, 43 RET4 exits and seven calls. Its full
unsigned selector guard, table, fixed direct branches and twelve following CC
bytes are independently checked. Existing ownership stays unchanged.

Its four difficulty tests compare float-converted difficulty, produce integer
boolean results and convert those results to float. Source now preserves that
conversion instead of selecting float literals. Case grouping follows the
target's random, timer/variables and motion families. The whole candidate grows
from788 to876 bytes in the same graph. It still uses EAX where the flag result
uses EDX in target, merges world-X with another tail, and rotates later pointer
registers differently. These differences and all shifted table/local positions
remain failures; no float bytes receive partial credit.

Signed-integer and boolean spellings of the flag conversion reproduce exactly
the same complete candidate inventories. Both are restored and parked. Do not
repeat them without new evidence and a distinguishing prediction. No volatile,
barrier, ABI forcing, target bytes, authored padding or compiler flag change is
introduced. Game source changes are limited to the two operand readers and the
now-unused shim declaration.

A separate early/late float player-family probe restores the EDX flag result,
independent world-X tail and complete884-byte extent. The shared player X/Y
bodies remain near the beginning rather than after the motion fields, so the
owner still fails. Swapping the two identical label families is byte-neutral;
it does not select the later physical copy. Both controls are restored to the
876-byte epoch already reproduced by the integer canonical colds. Do not use
size equality or the recovered register schedule as partial match credit.

## Reproduction and validation

```sh
scripts/repo-python scripts/replay-exact-units.py --unit enemy-read-int-operand --json
scripts/repo-python docs/evidence/enemy-operands-exact-20261030/verify-proof.py --artifact-dir build/match-linked/EnemyReadIntOperand
```

The new unit preserves the existing 21-input InitializeVm graph and each pinned
profile, including the real GetVm dependencies. Each canonical cold compiles
every input without reuse, serial CPU0/nice10, with a bounded job and retained
exit. All 70 transitive source/header pins are fixed. The verifier independently
decodes both complete code/table extents, inventories every field and fixed
destination, and strictly compares only the new integer owner plus GetVm21 and
EnemyAngleFromPlayer67 in the same image: 908 bytes/all100 fields. Those 88
guard bytes already have credit. Explicit affected canonical selectors also
replay the existing two callee graphs. No full cohort or host failure/lifecycle/
mutation campaign is required; comparison infrastructure is unchanged.

`results.json` retains compact cold receipts, input pins, complete field/table
reviews and selector results. Current ignored context is
`.analysis/gpt-6.1-sol/enemy-operands-context-20261030/canonical-b-context.json`.
Previous contexts containing Enemy.cpp have a historical source pin; relink the
actual declared graph before another incremental experiment. Their historical
exact reports remain evidence, not current cache receipts.

New credit is one authored owner /820 bytes: authored exact103090/275835 across
503 owners; total codegen171194/1380. Source1207/authored689/backlog186. Original
class/TU/data ownership, product/runtime validation and portability remain open.
