# Complete GameManager initialization — 2026-10-31

The complete `GameManagerInitialize` owner at `417870..417C60` is exact:
**1009 bytes, all 102 address fields, 40 fixed internal destinations, 30 direct
calls and both RET4 exits**. This adds 1009 authored exact bytes and one owner.
Source presence, reviewed boundaries, origins and the denominator do not change.

## Recovering the complete control flow

The preceding dependency checkpoint repaired actual calls and storage layouts,
but the complete startup still emitted 1023 bytes. Independent target decoding
shows that the final factory at `42B660` in the creation path and `StageCreate`
in the restoration path converge on one result check at `417B1C`. They do not each
have their own test/error edge.

The maintained source now retains their meaningful `creationResult` and tests
it after the two modes converge. Earlier factory failures still enter the same
cleanup block. This one change produces the complete 1009-byte owner, removes
the extra internal branch, restores the shared failure-block placement and
recovers the actual call order. There is no synthetic helper, branch hint,
padding, forced register or partial-body comparison.

That complete candidate still differs in three ordinary opcode bytes. At
`4179CC`, the target reads the low flag byte before computing initial rank; an
explicit unsigned-byte conversion recovers the byte load. At `4179EE`, the
target compares current score against the stored maximum; expressing that
comparison in the observed operand order recovers CMP/JLE. The resulting whole
owner passes the strict canonical comparator before any ledger credit.

| Complete source model | Native / target | Non-field bytes |
| --- | ---: | ---: |
| Entry with repaired dependencies | 1023 / 1009 | unequal |
| Shared final creation result | 1009 / 1009 | 598 / 601 |
| Observed byte conversion and score comparison | 1009 / 1009 | 601 / 601 |

The actual startup and failure algorithms remain complete: renderer/loader
waits, new-session score/statistics setup, callback registration, owner creation,
replay restore, Enemy reset/create, music preparation, timing reset, worker
publication and chain marking. Exactness does not establish original names,
class/TU/data ownership or runtime behavior of the unresolved callees.

## Field and callee acceptance

All 102 fields bind the source expressions that actually produce them. Main
and startup-state references use their reviewed semantic bases and addends;
nearby, unrelated anchor names are not substituted for aggregate members.
Callback addresses bind the real update/draw entries. Every call binds its
reviewed target identity, including the callee-pop Replay factory and actual
Chain insertion interfaces. Complete fixed branch destinations are compared.

The final call explicitly selects static `MarkGameChains` in `GameManager.cpp`.
The comparator qualifies its actual producer object through MAP/PDB records
and requires its entire 28-byte, zero-field body to be raw-equal. This callee
gets no additional credit. Frozen copies retain that original producer proof;
no path substitution or weakened static selector is used. Independent decoding
of each frozen image rechecks the complete caller and static body.

Two zero-reuse canonical cold runs rebuild the same 32-input graph with its
existing pinned VC7.1 profiles, `/GL` and real `InitializeVm` entry. The first
also explicitly replays dummy-command41, update-callback7, draw31 and static
mark28 with their declared recipes; the second selects only the new parent.
Both frozen startup images pass 39 selected existing contracts, 5123 bytes and
203 fields. Five complete comparisons remain rejected: both Chain insertions
129/131, GUI factory68/136, laser-factory field positions and Stage destructor
460/499. These distinct contexts are not waived or migrated.

[results.json](results.json) retains the 79 source/header pins, semantic field
contract, complete independent target/native inventories, static-owner proof,
PE/PDB identities, scoped cold selections and existing-contract outcomes.
Private logs and frozen builds are under the `startup-controlflow-20261031`
campaign. Compiler/linker jobs are serial, CPU0, nice10, bounded and logged.
The comparator and existing recipes/profiles are unchanged; only the new parent
contract is added to `config/match-units.toml`.

```sh
scripts/repo-python scripts/replay-exact-units.py --unit game-manager-initialize --unit game-prepare-dummy-music --unit game-manager-update-callback --unit game-manager-mark-chains --unit decomphelp-game-manager-on-draw --json
scripts/repo-python scripts/replay-exact-units.py --unit game-manager-initialize --json
scripts/repo-python scripts/ci.py
git diff --check
```

Authored exact becomes 107100/275863 across 517 owners; canonical codegen is
175214 bytes / 1395 owners. Mappings remain 1220/authored702, backlog becomes185.
Product closure, runtime validation and portability remain later phases.
