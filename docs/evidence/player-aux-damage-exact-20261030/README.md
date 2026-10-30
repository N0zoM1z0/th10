# Complete Player auxiliary damage owner — 2026-10-30

**PlayerUpdateGate28::GetDamageAtPosition is exact across all202 owned bytes.**
The two independent canonical cold builds reproduce all76 instructions, six
bound absolute references and nine fixed local branches. The existing1170-byte
Player damage caller remains exact. New authored credit is202 bytes/one owner.
The [results](results.json) retain full bindings, target/caller review, source and
object pins, controls, cold reports and scoped preservation evidence.

## Target and source

The reviewed owner4059F0..405AB9 is followed by six CC bytes and preceded by
three. All six exits are plain RET. An exhaustive E8/signed-REL32 scan of all
412289 mapped text bytes finds one direct call,428891. The caller loads ECX
from4776EC and places the target position in EAX; the body reads those registers
and returns an integer in EAX. This is the same actual private interface as the
previous189-byte candidate, reproduced by an ordinary member without an ABI hint.

The inactive gate returns zero. The complete remaining body performs a strict
XY radius comparison, selects damage from mode44 and game flag378C, and reads
477704/474C68 for its remaining return values. The compared owner includes all
inside/outside and zero-result branches, x87 cleanup, conditional integer
selection and the late ESI push/pop. No runtime simulation replaces byte coverage.
Original names, exact original source spelling and wider type ownership remain
unknown; no layout, boundary or origin record changes in this batch.

The source initializes one integer result after the inactive early return and
uses a switch for the inside-radius mode. Case zero selects three when the game
flag is set; otherwise it falls through into the shared default path. Default
selects38 for a set flag, or38/5 through the secondary gate. Outside-radius paths
retain23/26,20/16 or zero. These are target-observed numeric roles, not inferred
product meanings.

Direct flag reads in the switch matter. A cached byte removes a required global
reload and changes register lifetimes. The direct-read switch gives the target
mode-first TEST, shared CL flag value, initial EDX zero, default join and all six
return sequences. No volatile barrier, fake declaration, copied bytes, inert
padding, extra return, new compiler option or comparator relaxation is used.

The six fields bind the same three actual identities twice each:

| Offsets (hex) | Identity | Target |
| --- | --- | --- |
| 3D /79 | g_EnemyGameState | 4776F4 |
| 64 /A0 | g_PlayerDrawGate10 | 477704 |
| 8E /B5 | g_GameStatisticsIndex0 | 474C68 |

A fresh target operand scan independently inventories these fields. Strict
comparison binds every address and preserves all nine local destinations. The
complete extent is PDB-bound to the Player.cpp support object.

## Bounded controls and the larger caller

| Source form | Complete native bytes | Observation |
| --- | ---: | --- |
| Earlier direct-return source | 189 | Geometry retained; result/control schedule differs |
| Result variable before inactive gate, duplicated mode branches | 246 | Changes receiver and duplicates fallback; rejected |
| Shared fallback with early returns | 208 | Introduces saves/direct constant returns; rejected |
| Result after gate and compound mode predicates | 201 | Restores prefix/interface; retains a redundant mode test |
| Separate mode/default-flag checks | 211 | Retains extra flag work and saves; rejected |
| Nested inside return | 234 | Duplicates fallback; rejected |
| Cached flag with nested branches | 233 | Duplicates fallback and changes receiver; rejected |
| Switch with cached flag | 197 | Removes redundant mode test but loses a global reload |
| Switch with direct flag reads | 202 | Complete target-bound exact owner; retained |

The coupled796-byte state2 owner is investigated first. Caching its manager and
using a merged local VM lookup both leave its complete786-byte native candidate
unchanged. Its actual script initializer remains197/203 with a different private
interface. Reusing the already declared plain ANM profile from the screen-creator
recipe in the current seventeen-input graph is also neutral for these complete
owners; that diagnostic recipe is restored. No profile sweep or new flags are
introduced. State2, the initializer and the3112-byte main remain nonexact.
These controls are parked until a new prediction distinguishes them.

## Reproduction and validation

The new unit shares the existing Player damage owner's unchanged seventeen-input
/GL recipe and output directory, with its real InitializeVm entry. Actual callee
definitions remain present. The graph is a diagnostic anchor image, not a claim
about the original production partition or runnable product closure.

    timeout 600 taskset -c 0 nice -n 10 scripts/repo-python \
      scripts/replay-exact-units.py \
      --unit player-auxiliary-damage-at-position \
      --unit player-calculate-damage-to-target --json

First cold explicitly selects these two units and passes1372 bytes/all34 fields
from one freshly compiled seventeen-input graph. Second cold independently
rebuilds only the new202-byte/all6-field unit. No historical cohort is replayed.
Compiler/linker jobs are serial, CPU0/nice10, bounded and logged. Source/header
pins freeze each build; exploratory warm reuse validates source and object hashes.

Thirty-one strict same-image bodies pass5027 bytes/all161 fields. Ten preceding
native candidates retain complete bytes and resolved-field identities, including
main3096, state2 786, shots876 and EnemyRuntimeUpdate2412. Their near-match status
is unchanged. Only the auxiliary damage source changes; every state2, lookup and
profile experiment is restored.

Target identity, toolchain declarations, default seven-stage CI and whitespace
checks pass. No Ghidra observations, compiler changes or comparator changes are
needed. Totals become99,996/275,752 authored owned bytes exact,491 authored exact
owners. Source mappings, boundaries and origins are unchanged. Product closure,
runtime and portability remain independent later phases.
