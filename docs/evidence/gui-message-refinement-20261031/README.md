# Complete GUI message interpreter refinement — 2026-10-31

GuiMessageVmView::Run415E90..41700F remains **NONEXACT**. Its complete current
candidate is4464 bytes against4480 target bytes. This checkpoint improves
source expression, hidden return-slot layout and observed memory schedules;
it adds zero exact owners or bytes. No Run match or canonical recipe is added.
The [previous packet](../gui-message-run-context-20261030/README.md) describes
the historical4496-byte candidate and real decoder/stage-state visibility.

## Target observations and retained source

The target owns4306 executable bytes, two alignment bytes and172 selector
bytes, with four24/7/7/5-entry tables. The target PE base-relocation directory
is empty. Independent instruction/table decoding still records337 fields,
150 direct calls and121 fixed local branches. These fields must be bound
explicitly; the absence of target PE relocations does not waive them.

Four SetVmPosition arguments and the special-VM MarkVmForDeletion argument now
pass their existing AnmVmIdView objects. This removes five integer-to-ID
constructions without changing the real interfaces. Four player portrait
operations chain ResultMenuFindScript(...).SetSprite(...), retaining the
target's operand-before-lookup evaluation and use of the returned ID address.
Together these changes recover every hidden EDI output slot:18 VM creations
and36 child lookups,54 total. Fresh target and both final native images reproduce
the complete per-callee slot sequences. Call offsets still differ; matching
stack outputs does not establish whole calls or surrounding bytes as exact.

Text opcodes14/15 explicitly share one lookup/decode/draw body; their selector
entries recover8D/92. The first alternating-line path captures its actual
manager conditionally, using a scoped volatile read after a compiler memory
barrier. The current manager register still differs. A four-byte bitfield view
of flags7C recovers the skippable assignment's18-byte copy/XOR instruction form.
The original source type, qualifiers, barriers and helper boundaries are unknown.

Each of16 enemy portrait arms observes the resource owner, resource slot and
current instruction in the target order after the first sprite setter. Two
small ordinary inline helpers express those scoped volatile reads; they emit
no new calls. This does not settle the remaining register schedule. Stage
completion preserves two byte publications and four fresh global index reads
between them. A volatile byte destination and four scoped volatile snapshots
recover those accesses. The target uses two BYTE stores, not a fused word store.
No shared header/layout, compiler profile, fake ABI or data binding changes.

The wait bound is spelled <=0. In isolation this recovers the cached zero and
third SetCurrent call. Later source changes let VC7.1 merge the zero-reset
suffix again. The final owner has149 direct calls; all per-callee counts agree
except SetCurrent2 versus target3. A complete same-length4480 intermediate also
failed comparison. Size is not an exactness percentage or a count of wrong bytes.

## Complete native reproduction and remaining differences

The final warm image and a zero-reuse18-input cold have identical complete
native receipts:4464 bytes,4292-byte code/alignment prefix,1146 decoded
instructions,336 linkage identities,121 local branches,149 direct calls and
all43 selector entries. Both RET4 offsets are109A/10C0. TheD8 frame and all24
physical opcode groups remain. Original target destinations are independently
decoded, but the native336 fields and121 locals are **not** blanket target-bound
or destination-exact claims. Register/read/block schedules, the missing physical
reset call and field/table positions still reject complete target equality.

The final source is frozen through both runs.60 source/header SHA pins, the
18-input graph/profiles, compiler object hashes, distinct PE hashes and complete
native identities are retained in [results.json](results.json). The standalone
artifact verifier reuses the historical read-only decoder and checks the full
native contribution, target decode, four tables and all54 output slots. It is
an evidence verifier, not a new acceptance comparator.

```sh
scripts/repo-python docs/evidence/gui-message-refinement-20261031/verify-native.py --artifact-dir build/gpt-6.1-sol/gui-message-close-20261031/reviewed-candidate
scripts/repo-python docs/evidence/gui-message-refinement-20261031/verify-native.py --artifact-dir build/gpt-6.1-sol/gui-message-close-20261031/candidate-cold
timeout 900 taskset -c 0 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit gui-update-stage-elements --unit player-calculate-damage-to-target --unit player-auxiliary-damage-at-position --json
```

Both18-input images pass six strict existing/provisional guards: GUI3836/all72,
damage1170/all28, auxiliary202/all6, decoder41/raw, stage-state32/raw and callback
7/all1, totaling5288/all107. The callback remains provisional with no new credit.
The separate canonical cold selects only the direct GUI caller and the two
coupled existing damage contracts. One17-input producer passes5208/all106 with
unchanged recipes. No historical full cohort or host failure/lifecycle tests.
The script binder remains197/203 nonexact in the18-input graph.

## Bounded controls parked

All22 native observations have terminal exits and compact receipts. Selected
controls below are measured complete sizes, not acceptance claims.

| Control | Bytes | Observation / disposition |
| --- | ---: | --- |
| Current entry |4496|333 fields,149 calls; baseline |
| Unconditional observed manager |4484|Hoists read above condition; restored |
| Conditional manager observation |4492|Keeps condition; register differences remain |
| Unsigned encoded time |4492|Byte-neutral; restored |
| Four existing position IDs / fifth deletion ID |4488/4484|Retained actual objects |
| Whole creation-result assignments |4484|Complete normalized receipt neutral; restored |
| Shared text dispatch |4484|Recovers selector entry15 at92; retained |
| Isolated <=0 wait bound |4496|334 fields,150 calls, three resets |
| Combined typed/conditional spelling |4480|Still nonexact |
| Chained player sprites |4440|All54 hidden slots recover; retained |
| Barrier between profile publications |4452|Does not recover all four index reads; replaced |
| Explicit profile observations |4464|336 fields,149 calls; retained |
| Flattened32 enemy sprite arguments |4464|Complete normalized receipt neutral; restored |
| Skippable bitfield |4464|Recovers18-byte assignment shape; retained |
| Resource barrier alone |4464|Fails ordered-read prediction; restored |
| Scoped resource/instruction observations |4464|Observed load order; retained |
| Explicit wait exits |4456|Still two resets; restored |
| Factored observation helpers / final comments |4464|No helper calls; retained |

The `direct-enemy-sprite-indices` label is an unchanged-source relink after a
generation assertion stopped before writing. It is not a source-control result.
Only `actual-enemy-sprite-indices` applies that experiment. Reopen parked neutral
spellings only with new evidence and a falsifiable prediction. Superseded build
directories may be removed after retaining their compact receipts; final warm,
candidate-cold and caller-cold images remain private below build/. Campaign logs
and scripts remain below .analysis/gpt-6.1-sol/gui-message-close-20261031/.

Seven lightweight CI stages and whitespace checks pass. Regenerated progress
is unchanged. Superseded builds in this campaign are removed after retaining
their receipts; the baseline and three final builds remain.

Authored exact remains111454/275863 across521 owners; canonical179568/1399,
mappings1221/authored703/backlog182. Boundaries, origins, authored denominator,
original type/TU ownership, product/runtime and portability claims stay fixed.
