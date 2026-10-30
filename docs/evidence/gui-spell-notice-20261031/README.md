# Spell finish, notices and power — 2026-10-31

This checkpoint adds565 exact authored bytes across two complete owners and
maintains one complete nonexact668-byte notice dispatcher.

| Owner | Target | Complete target bytes | Native result | Bound fields |
| --- | --- | ---: | --- | ---: |
| EnemyEndSpell | 409C00..409D8E | 399 | exact | 20 |
| GameScoreStateView::AddPower | 418930..4189D5 | 166 | exact | 4 |
| GuiShowScoreNotice | 4172E0..41757B | 668 | 660, NONEXACT | none accepted |

Two independent pinned VC7.1 `/GL` cold links reproduce each accepted owner in
its full PE/MAP/PDB extent. Every decoded address field is bound, every local
branch stays fixed, both exits per owner match, and all ordinary bytes compare
zero-difference. Names, original class/TU/storage ownership and runtime remain
open. The nonexact notice dependency receives no partial credit through its
caller.

## Complete notice boundary and source

The old640-byte extent ended at41755F after RET8. Fresh target decoding and
read-only Ghidra show the indirect JMP at4172F2 references the **following**
seven-entry table at417560..41757B. Its destinations relative to4172E0 are
19,161,180,1C0,200,278,240; all are instruction entries in the640-byte code body.
Four CC bytes separate the table from independent417580. The owner is668 bytes,
with zero intervening alignment. This corrects one reviewed boundary and adds28
reviewed authored owned bytes to the denominator, **zero exact bytes**.

Complete source retains all seven kind arms and the out-of-range exit. Kind0
replaces the score notice, creates eight digit VMs, divides the signed value by
successive powers of ten, publishes each ID, resolves its sprite and toggles
flag2 after the first nonzero digit. Kind1 replaces the failed-spell notice;
2/3/4 replace the power/high-score/extend notice;5 does nothing;6 replaces the
stage notice. These are maintained descriptions, not recovered original enum
names. The actual caller naturally yields EAX kind, two stack inputs and RET8.

The first complete candidate was652/668. Slot clearing, remainder lifetime and
branch ordering restore the digit loop, but broad memset clearing folds the
independent notice tails and emits576. That control is restored. The retained
candidate660/668 has the target28-byte frame and complete seven-entry table,
but folds the loop's load/OR/store into a memory OR, publishes a digit ID through
another register, and moves resource loads before ID clears. Local flags,
bitfield spelling, word copy, whole-ID assignment and named script controls do
not resolve the complete owner; all neutral controls are parked. Its target
inventory has35 address fields and seven direct local branches. Matching
arithmetic or field counts cannot waive bytes, extents or table destinations.

## EnemyEndSpell399

The active-state gate controls a menu DWORD bit, three actual pending-ID calls,
spell-active-bit clearing and managed-VM deletion. The success branch adds the
bonus with the existing score operation, invokes notice kind0, updates two
capture counters outside playback mode, and queues sample45/0. Failure releases
and replaces the notice with script48/layer15. Both plain RET exits are retained.

The first source emitted383 bytes because it pre-added the capture offset to a
raw count pointer. A real90-byte spell-record view retains captures at+80 and
attempts at+84. Reusing existing GameStatisticsOwner::records[group] and
records[6], with record-relative spell-table offset59C, recovers both the exact
399-byte extent and access/register schedule. The aggregate row lowers to the
target owner+19A8C base. Existing score-state storage supplies group indices at
+28/+2C. No new global allocation or guessed original class is introduced.

The actual14348-byte native EnemyRuntimeView dispatcher still contains one
direct EnemyEndSpell call at+27C9. That parent remains nonexact versus its
reviewed target; exactness is bounded to the399-byte child. The called notice
is real maintained source with the observed interface and remains660/668;
public entry binding is not a claim of callee byte equality.

## AddPower166 and scoped checks

The target takes EAX state, one signed-short stack input and RET4. It returns
zero when power is already at least100. Only overshoot caps power to100 and
replaces the power-notice VM. The final signed integer quotient comparison is
retained. A complete four-byte ID clear and a local boundary around the genuine
CreateVmVariant0 call recover224 to166 bytes, all4 fields and both RET4 exits.
No shared ANM source, header, compiler profile or ABI declaration changes.
Fresh target callers show only ItemManagerUpdateBody, with its two source calls
passing1 and20. That parent is reviewed separately and remains nonexact.

```sh
timeout 900 taskset -c 0 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit enemy-end-spell --unit game-score-add-power --unit item-rank-add --json
timeout 900 taskset -c 0 nice -n 10 scripts/repo-python scripts/replay-exact-units.py --unit enemy-end-spell --unit game-score-add-power --json
scripts/repo-python scripts/ci.py
git diff --check
```

The first scoped cold passes607 bytes; the independent second cold passes565.
Each image also passes12 selected complete owners2243/all86: the new two,
AddRank, score update, life addition, creator0, sprite setter, sound queue,
pending interrupt, VM lookup and both flag2 operations. These are scoped
native proofs; no full historical cohort or host simulations are required.
All native jobs are serial CPU0/nice10 with bounded runtime and retained exits.

results.json records canonical graph/profiles, source pins, both cold PE/PDB
identities, all accepted fields, target inventory, table review, scoped guards
and nonexact controls. Private snapshots/logs remain in
`.analysis/gpt-6.1-sol/gui-spell-notice-20261031/`; the canonical producer is
`build/match-linked/GuiSpellNotice`. Product, runtime and portability remain
separate later phases.
