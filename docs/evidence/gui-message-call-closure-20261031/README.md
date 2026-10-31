# Complete GUI message call and branch review — 2026-10-31

GuiMessageVmView::Run415E90..41700F remains **NONEXACT**. The complete current
candidate is4476/4480 bytes. It now has all150 target direct calls in the target
identity sequence, including three SetCurrent calls. All54 hidden output slots
remain correct. The actual transition call now passes priority31 in EBX and
five stack operands. No Run match, canonical recipe or exact credit is added.
The [previous candidate packet](../gui-message-refinement-20261031/README.md)
records the historical4464-byte/149-call source epoch.

## New target-backed source observations

The previous source merges two wait exits into one reset. A compiler memory
barrier between the skip-path SetCurrent(0) and alternatingLine clear retains
both physical reset paths. The first complete probe is4480 bytes,337 fields
and150 calls; it still rejects. Target zero caching and the wait registers are
not recovered merely by restoring the missing call. Original barriers and
qualifiers remain unknown.

Stage7's portrait selector previously used the opposite physical branch order.
The target tests messageIndex against1 and jumps JGE over the44/4D operand
group to the2E/37 group. The maintained source now spells the <1 arm first.
All eight operand offsets and eight script IDs match their target sequence;
the two character-specific arms in each group remain separate. This corrects
a missed nested dispatch difference; all24 outer opcode groups were already
ordered. A matching outer-group order never proved these internal destinations.

The replay mode4 action now lives in stage7's explicit replay arm, shared by
the stage6 jump. Normal stage7 completion precedes that shared action in the
native image. This restores the complete150-call identity sequence, not just
per-callee counts. Both stage6/7 completion-flag writes have a compiler memory
barrier before score-input reads. Each now emits the observed memory OR of20
at guiFlags9EB4, before reading the score counter, without emitted barriers or
new calls. Original source synchronization and threading remain unknown.

The resource helper retains its scoped volatile resource-field observation
and ordinary outer owner read. Removing the excess outer-pointer qualifier
recovers separate owner/field loads at all16 second-sprite sites. It does not
close their register identities. No shared header, ABI declaration, optimizer
profile, fake parameter, padding or target-byte source changes.

## Real transition visibility and actual ABI

The old18-input graph omitted TransitionEffect.cpp. Its StageCreateTransitionEffect
call therefore targeted a data anchor and pushed six declared stack arguments.
Fresh target decoding instead shows three zero operands,120 and5 on the stack,
then EBX=31 and the real call. Supplying only the complete transition TU makes
all direct callees executable, but still selects the wrong private interface:
the81-byte factory receives kind5 in EBX and priority31 on the stack.

The existing canonical transition graph also supplies Chain.cpp and
AnmRectangle.cpp. Adding those two complete real dependencies, with their
unchanged canonical profiles, restores the82-byte factory and target priority
interface. This is21 inputs with the same genuine InitializeVm entry. The
factory passes its complete strict82-byte/all2-field comparison in both final
images. Every interpreter direct call now lands in executable code; this alone
would not prove its ABI, as the19-input counterexample demonstrates.

No canonical graph is rewritten while the parent remains nonexact. The private
diagnostic graph and all64 local source/header SHA pins are recorded in
[results.json](results.json). Source/class/TU/data ownership remains provisional.

## Complete comparison and bounded validation

The final warm image and zero-reuse21-input cold have identical complete
native receipts:4476 bytes,4304-byte code/alignment prefix,1146 decoded
instructions including alignment,337 linkage identities,121 fixed locals,
150 calls, two RET4 exits and43 table entries. The exits are10A6/10CC; theD8
frame remains. The target owns4306 executable bytes/1146 instructions, two
alignment bytes and172 selector bytes. Its base-relocation directory is empty;
all337 fields still require explicit target binding.

Matching native field/local counts and150 call identities do **not** prove
target field offsets, local destinations, ordinary bytes or table destinations.
The four-byte extent gap is not a count of incorrect bytes. Shared text-output
setup, alternating manager registers, side-setting schedules, sprite registers,
wait zero caching, profile/counter indexing, special-VM clear order and the
instruction-advance cursor still differ. The native cursor coalesces a move
present in target. A later meaningful next-instruction capture is fully
byte-neutral and restored; see the [subsequent control receipt](../spell-panel-exact-20261031/README.md).
It does not repair the cursor or grant exact credit.

```sh
scripts/repo-python docs/evidence/gui-message-call-closure-20261031/verify-native.py --artifact-dir build/gpt-6.1-sol/gui-message-wait-20261031/reviewed-candidate
scripts/repo-python docs/evidence/gui-message-call-closure-20261031/verify-native.py --artifact-dir build/gpt-6.1-sol/gui-message-wait-20261031/candidate-cold
timeout 900 taskset -c 0 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit gui-update-stage-elements --unit player-calculate-damage-to-target --unit player-auxiliary-damage-at-position --unit transition-effect-factory --json
```

The artifact verifier independently decodes the target and complete PDB-owned
native extent, all four tables,150 call identities in order, all54 EDI stack
outputs, nested stage7 operands/scripts, two flag memory updates and the actual
transition argument setup. It grants zero exact credit and does not change the
acceptance comparator. The old4464-byte frozen baseline is freshly verified.

Both21-input images pass seven existing/provisional guards: GUI3836/all72,
damage1170/all28, auxiliary202/all6, decoder41/raw, stage-state32/raw, transition
factory82/all2 and callback7/all1, totaling5370/all109. Callback7 remains
provisional with no new credit. The script binder remains197/203 nonexact.
Canonical cold replay passes four complete contracts5290/all108: the direct
GUI caller, its coupled damage pair and the transition factory, using their
existing17-input and14-input producers.
No historical cohort, all-consumer sweep or host failure/lifecycle tests.

## Controls parked

Sixteen terminal native observations retain source/image hashes and complete
extent/count receipts. Source spelling and graph visibility are distinguished.

| Control | Owned bytes | Observation / disposition |
| --- | ---: | --- |
| Separate wait reset |4480|Restores150 calls/337 fields; retained |
| Stage7 physical order |4480|Restores JGE and operand/script sequence; retained |
| Manager before active-side store |4480|Wrong read/register schedule; restored |
| Capture first line ID |4480|Partial register improvement, wrong read/index schedule; restored |
| Ordinary outer resource owner |4496|Restores16 separate pointer loads; retained |
| Barrier after first-ID capture |4496|Ordering improves, wrong registers/second call; restored |
| Barrier instead of volatile instruction read |4484|Does not recover all16 site forms; restored |
| Direct integer operand loads |4496|Byte-changing, predicted sprite registers still wrong; restored |
| Shared replay action placement |4492|All150 call identities in order; retained |
| Volatile stage flag updates |4492|Still expands memory OR; restored |
| Add transition TU alone,19 inputs |4484|Factory81 has wrong private kind interface |
| Add its real dependencies,21 inputs |4484|Factory82/all2 and priority ABI restored |
| Flag-before-score memory order |4476|Both memory OR forms restored; retained |
| Narrow profile index lifetimes |4476|Does not recover profile load/register schedule; restored |
| Final commented source / zero-reuse cold |4476/4476|Complete native receipts identical |

The private mnemonic diagnostic now stops before selector data; table words
are decoded separately and are not treated as instructions. This changes no
Oracle. Reopen parked controls with new evidence and a falsifiable prediction.
Scratch/logs remain below .analysis/gpt-6.1-sol/gui-message-wait-20261031/;
native artifacts remain below the matching build/ campaign directory.

Seven lightweight CI stages and whitespace checks pass; regenerated progress
is unchanged. Fourteen superseded build directories are removed after retaining
their receipts and the19-input wrong-ABI counterexample. Final warm/cold and
the two canonical producer snapshots remain private below build/.

Authored exact stays111454/275863 across521 owners; canonical179568/1399,
mappings1221/authored703/backlog182. Boundaries, origins, authored denominator,
product/runtime and portability claims remain unchanged.
